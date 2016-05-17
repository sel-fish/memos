#!/bin/bash

# mkdir /var/run/salt/master
# chown to fenqi:wheel

dir=/var/run/salt
group=fenqi:wheel
expfile=`dirname $0`/saltInit.exp

echo "please input passwd of $user:"

stty -echo
trap 'echo ""; stty echo; exit 1' SIGTERM SIGINT
read passwd
echo ""
stty echo

expect $expfile $dir $group $passwd
