TARGET = adns-socket-client

C_SRCS = socket-client.c main.c viewer.c

INLCUDES = -I.

#C_CFLAGS = -Wall -pedantic -O0 -std=c99
C_CFLAGS = -Wall `pkg-config --cflags --libs gtk+-2.0`
C_DFLAGS =
C_LDFLAGS =

C_EXT = c
C_OBJS = $(patsubst %.$(C_EXT), %.o, $(C_SRCS))

C = gcc

all: $(TARGET)

$(TARGET): $(CPP_OBJS) $(C_OBJS)
	$(C) $(C_CFLAGS) $(C_LDFLAGS) -o $(TARGET) $(C_OBJS)

$(C_OBJS): %.o: %.$(C_EXT)
	$(C) $(C_CFLAGS) $(C_DFLAGS) $(INCLUDES) -c $< -o $@ 

clean:
	$(RM) $(TARGET) $(C_OBJS)
