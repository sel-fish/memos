#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

void *do_write(void *arg);
void *do_fsync(void *arg);
bool create_thread(pthread_t *thread_handle, void *(*routine)(void *), int *fd_ptr, const char *thread_name);
void get_time_str(char* buf, time_t *timestamp);

int main(int argc, char **argv) {
    printf("fsync test start\n");

    if (argc <= 1) {
        printf("usage : %s file_path\n", argv[0]);
        return -1;
    }

    const char *filename = argv[1];

    int fd = open(filename, O_CREAT | O_WRONLY, 0644);
    if (-1 == fd) {
        printf("open file %s err\n", filename);
        return -1;
    }

    pthread_t write_thread, fsync_thread;
    create_thread(&write_thread, &do_write, &fd, "write_thread");
    create_thread(&fsync_thread, &do_fsync, &fd, "fsync_thread");

    pthread_join(write_thread, NULL);
    pthread_join(fsync_thread, NULL);

    close(fd);

    return 0;

}

void *do_write(void *arg) {
    int fd = *((int *) arg), loop = 1, threshold_secs = 2;
    char buf[30];
    const char *content = "test";
    bool stop = false;

    while (!stop) {
        int offset = lseek(fd, 0, SEEK_SET);
        if (offset == -1) {
            printf("lseek err...\n");
            break;
        }

        time_t start = time(NULL);
        int written = write(fd, content, strlen(content));
        if (written == -1) {
            printf("write err...\n");
            break;
        }

        time_t end = time(NULL);

	if (end - start > threshold_secs) {
            get_time_str(buf, &end);
            printf("%s, %d, write cost: %lds\n", buf, loop, end - start);
	}

        loop++;
    }

    return NULL;
}

void *do_fsync(void *arg) {
    int fd = *((int *) arg), loop = 1;
    char buf[30];
    bool stop = false;

    while (!stop) {
        time_t start = time(NULL);
        get_time_str(buf, &start);
        printf("%s, %d, start to fsync\n", buf, loop);
        fsync(fd);
        time_t end = time(NULL);
        get_time_str(buf, &end);
        printf("%s, %d, fsync cost: %lds\n", buf, loop, end - start);
        loop++;
    }

    return NULL;
}

bool create_thread(pthread_t *thread_handle, void *(*routine)(void *),
                                            int *fd_ptr, const char *thread_name) {
    int err = pthread_create(thread_handle, NULL, routine, fd_ptr);
    if (err != 0) {
        printf("can't create thread %s:[%s]\n", thread_name, strerror(err));
        return false;
    } else {
        printf("Thread %s created successfully..\n", thread_name);
        return true;
    }
}

void get_time_str(char* buf, time_t *timestamp) {
    struct tm* tm_info = localtime(timestamp);
    strftime(buf, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}
