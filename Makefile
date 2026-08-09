CC = gcc
CFLAGS = -Wall -Wextra -Iinc

TARGET = mini-vmm

SRC = src/main.c src/utils.c src/kvm.c
OBJ = $(SRC:.c=.o)

all: $(TARGET) guest

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

guest: guest/guest.S
	as --32 guest/guest.S -o guest/guest.OBJ
	ld -m elf_i386 --oformat binary -N -e _start \
		-Ttext 0x0 -o guest/guest guest/guest.o

clean:
	rm -f $(OBJ) $(TARGET) guest/guest.o guest/guest