CC= /usr/bin/gcc
all: sender receiver

sender: sender.c;
	${CC} sender.c -o sender -lm

receiver: receiver.c;
	${CC} receiver.c -o receiver

clean:
	rm sender receiver
