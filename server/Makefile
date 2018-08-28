CC      = gcc
CFLAGS  = -I./include -O2 -MD
CFILES  = $(shell find src/ -name "*.c")
OBJS    = $(CFILES:.c=.o)

run:	main
	./server

main:	$(OBJS)
	$(CC) $(CFLAGS) -o server $(OBJS) -lmysqlclient

-include $(OBJS:.o=.d)

clean:
	rm -f main $(OBJS) $(OBJS:.o=.d)

openc:
	gedit $(CFILES)

openh:
	gedit $(shell find include/ -name "*.h")
