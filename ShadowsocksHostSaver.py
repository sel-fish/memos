# use AppleScript to get the url

from subprocess import Popen
import subprocess
import os
from urlparse import urlparse
from os.path import expanduser

gfwlist_path = expanduser("~") + "/.ShadowsocksX/gfwlist.js"
# gfwlist_path = os.path.dirname(os.path.realpath(__file__)) + "/gfwlist.js"


def run_command(args):
    child = Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = child.communicate()

    if 0 != child.returncode:
        return False, err
    else:
        return True, out


def insert_one_hostname(hostname):
    # gsed -i '/var rules = \[/a \  "||mogujie.org",' gfwlist.js
    # insert_rule_args = ['gsed', '-i', '/var rules = \[/a \  \"||%s\",' % hostname, gfwlist_path]

    # sed -i '' 's/var rules = \[/&\'$'\n''  "||mogujie.org",/' gfwlist.js
    insert_rule_args = ['sed', '-i', '\'\'', "s/var rules = \[/&\\\n  \"||%s\",/" % hostname, gfwlist_path]
    ok, msg = run_command(insert_rule_args)
    if not ok:
        print "err while insert rule: " + msg
    else:
        print hostname + " inserted..."


def get_cur_hostname():
    # osascript -e 'tell application "Google Chrome" to return URL of active tab of front window'
    as_script_args = ['osascript', '-e', 'tell application "Google Chrome" to return URL of active tab of front window']
    ok, msg = run_command(as_script_args)
    if ok:
        url = msg.strip()
        hostname = urlparse(url).hostname
        return hostname
    else:
        print "err while run applescript: " + msg

    return None


if __name__ == '__main__':
    cur_hostname = get_cur_hostname()
    if cur_hostname:
        insert_one_hostname(cur_hostname)
