#!/bin/sh

#installation script
echo Installing linux-dictionary

#root check
if [ $EUID != 0 ]; then
	echo Error: insufficient privileges
	exit 1;
fi

#find compiler
COMPILER='none'
if [ -x '/usr/bin/g++' ]; then
	COMPILER='/usr/bin/g++';
elif [ -x '/usr/local/bin/g++' ]; then
	COMPILER='/usr/local/bin/g++';
else
	echo 'Error: could not find C++ compiler'
	exit 2;
fi

#compile
echo Building
$COMPILER main.cpp -o /usr/local/bin/linux-dictionary || exit -2

#create dictionary dir
if [ ! -d '/etc/linux-dictionary/' ]; then
	mkdir '/etc/linux-dictionary/' ||\
	echo 'Error: could not create /etc/linux-dictionary/' &&\
	exit 3;
fi

#copy dictionary
cp oxford.txt /etc/linux-dictionary/

echo Successfully installed linux-dictionary
exit 0
