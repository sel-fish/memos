#!/bin/bash

port=10022
p=$(dirname $0)
echo $p
argnum=1
expfile=$p/deploy.exp
sshdir=".ssh"
rsafile="id_rsa.pub"
user=
passwd=

if [ $# -lt $argnum ] ; then
  echo "usage : deploy.sh list"
  exit -1
fi

if [ -d "$HOME/$sshdir" ] ; then
  if [ -f "$HOME/$sshdir/$rsafile" ] ; then
    echo "local id_rsa.pub is ready"
  else
    echo "no $HOME/$sshdir/$rsafile, to generate..."
    # to do generate
    expect $expfile 0
    if [ -f "$HOME/$sshdir/$rsafile" ] ; then
      echo "generate $HOME/$sshdir/$rsafile ok"
    else
      echo "fatal error, can't generate file $HOME/$sshdir/$rsafile"
      exit
    fi
  fi
else
  echo "fatal error, no ssh dir $HOME/$sshdir"
  exit -1
fi

filename=$1
user=`echo $USER`
#echo $user

echo "please input passwd of $user:"

stty -echo
trap 'echo ""; stty echo; exit 1' SIGTERM SIGINT
read passwd
echo ""
stty echo

expect $expfile 1 $filename $user $passwd $port $HOME
