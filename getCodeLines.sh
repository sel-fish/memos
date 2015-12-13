#!/bin/bash

hppCount=`find * -name '*.hpp' |xargs wc -l |tail -1 |awk '{print $1}'`
echo "hppCount is "$hppCount 
hCount=`find * -name '*.h' |xargs wc -l |tail -1 |awk '{print $1}'`
echo "hCount is "$hCount 
cppCount=`find * -name '*.cpp' |xargs wc -l |tail -1 |awk '{print $1}'`
echo "cppCount is "$cppCount 
cCount=`find * -name '*.c' |xargs wc -l |tail -1 |awk '{print $1}'`
echo "cCount is "$cCount 

((total=hppCount+hCount+cppCount+cCount))

echo "total is "$total
