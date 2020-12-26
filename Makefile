CC = gcc
CFLAGS= -Wall -Ofast
EXEC=MainP
SRC=$(filter-out test_array.c, $(wildcard *.c))
OBJ=$(SRC:.c=.o)
INC= TicketToRide.o ClientAPI.o

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o MainP $(OBJ) $(INC)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean	
