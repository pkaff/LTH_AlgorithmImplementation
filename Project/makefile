# for power.ludat.lth.se
#CFLAGS	= -m64 -O3 -g -Wall -Wextra -Werror -std=c99 -mcpu=970 -mtune=970 -maltivec

# the following works on any machine
#CFLAGS	= -O3 -g -Wall -Wextra -Werror -std=c99
CFLAGS	= -g -Wall -Wextra -std=c99


CC	= gcc
CC	= clang
OUT	= fm
OBJS	= main.o danielUlricaNils_fm.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)
	./fm
	size danielUlricaNils_fm.o

clean:
	rm -f $(OUT) $(OBJS)
