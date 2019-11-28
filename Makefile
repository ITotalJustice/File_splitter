# TotalJustice

CC			:= gcc
CFLAGS		:= -Wall -O3
CFILES 		:= $(wildcard *.c)
OBJECT 		:= $(CFILES:.c=.o)

TARGET		:= $(notdir $(CURDIR))
#SOURCE		:= source
#INCLUDE	:= include
LIBS		:= 


all: $(CFILES)
	$(CC) $(CFLAGS) $(CFILES) -o $(TARGET) $(LIBS)

clean:
	rm $(TARGET)
	#rm *.o $(TARGET)

run:
	./$(TARGET)