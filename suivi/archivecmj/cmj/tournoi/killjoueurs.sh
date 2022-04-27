#! /bin/sh 

echo "Process actifs :"
ps ax | grep joueur | grep exe
ps ax | grep humain | grep exe

echo -n "destruction de : "
ps ax | grep humain | grep exe | cut -c1-5 |  tr '\n' ' '

echo ""
echo -n "OK ? [Y-n] "
read REP
if [ "$REP" != "n" ]; then 
	ps ax | grep joueur | grep exe | cut -c1-5 |  xargs kill 2> /dev/null
	ps ax | grep humain | grep exe | cut -c1-5 |  xargs kill 2> /dev/null
fi

