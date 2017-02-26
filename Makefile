EXE_NAME := Smart_LED_exe

OBJS := \
				Smart_LED.o

CC := ${CROSS_COMPILE}gcc
CFLAGS = -W -Wall
LDFLAGS = -lwiringPi -lmysqlclient

%.o: %.c
			$(CC) -c -o $@ $< $(CFLAGS) -I/usr/include/mysql 

$(EXE_NAME): $(OBJS)
							 $(CC) -L/usr/lib/mysql -o $@ $^ $(LDFLAGS)

clean:
		rm -f *.o $(EXE_NAME)
