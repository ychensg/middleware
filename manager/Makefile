CC      = gcc
CFLAGS  = -I./include -O2 -MD -Wall
CFILES  = $(shell find src/ -name "*.c")
OBJS    = $(CFILES:.c=.o)

run:	main
	sudo ./manager

main:	$(OBJS)
	$(CC) $(CFLAGS) -o manager  $(OBJS) -lpthread -lmysqlclient

-include $(OBJS:.o=.d)

clean:
	rm -f main $(OBJS) $(OBJS:.o=.d) 

openc:
	gedit $(CFILES)

openh:
	gedit $(shell find include/ -name "*.h")
