CC= /usr/bin/gcc
all:	tcpclient tcpserver udpclient udpserver

tcpclient: tcpclient.c;
	${CC} tcpclient.c -o tcpclient

tcpserver: tcpserver.c;
	${CC} tcpserver.c -o tcpserver

udpclient: udpclient.c;
	${CC} udpclient.c -o udpclient

udpserver: udpserver.c;
	${CC} udpserver.c -o udpserver

clean:
	rm tcpclient tcpserver udpclient udpserver
