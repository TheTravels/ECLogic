#!/bin/bash
#echo $#
#echo $@

if (("$#" < 1)) ; then
echo "Please input commit info"
exit
else
echo "commit is " $1
git add .
git commit -m "$1"
git push origin master
fi

