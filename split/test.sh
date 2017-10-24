#!/usr/bin/bash
echo "[TEST 1] s='123' d='2'"
	result=`echo -e "123\n2" | ./example.run`
	correct=`echo -e "1\n3"`
	if [[ $result=$correct ]]
		then echo "[ok]"
		else echo "[fail]"
	fi
	

echo "[TEST 2] s='this is a test' d=' '"
	result=`echo -e "this is a test\n " | ./example.run`
	correct=`echo -e "this\nis\na\ntest"`
	if [[ $result=$correct ]]
		then echo "[ok]"
		else echo "[fail]"
	fi

echo "[TEST 3] s='1243qw23qwefjnvdqwe2' d='qwe'"
	result=`echo -e "1243qw23qwefjnvdqwe2\nqwe" | ./example.run`
	correct=`echo -e "1243qw23\nfjnvd\n2"`
	if [[ $result=$correct ]]
		then echo "[ok]"
		else echo "[fail]"
	fi
	
echo "[TEST 4] s='1243qw23qwefjnvdqwe' d='qwe'"
	result=`echo -e "1243qw23qwefjnvdqwe2\nqwe" | ./example.run`
	correct=`echo -e "1243qw23\nfjnvd\n"`
	if [[ $result=$correct ]]
		then echo "[ok]"
		else echo "[fail]"
	fi
