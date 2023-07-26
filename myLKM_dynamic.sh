#!/bin/bash -eu
MODNAME=my_dlink_test

echo "--menu--"
echo "install module : 1"
echo "remove module  : 2"
echo ""
echo "choose menu:"
read MENU

if [ $MENU -eq 1 ];then
	echo "input function number:"
	read FUNC
	make -s -C myLKM/ dlink_test FUNC=$FUNC
elif [ $MENU -eq 2 ];then
	sudo rmmod $MODNAME
fi


