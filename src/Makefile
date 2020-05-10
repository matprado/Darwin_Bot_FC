run:
	./../bin/darwin_bot_fc	< 1.in

all: cartola_utils_o darwin_bot_fc_o
	gcc cartola_utils.o darwin_bot_fc.o -o ../bin/darwin_bot_fc -g -Wall

cartola_utils_o:
	gcc -c cartola_utils.c

darwin_bot_fc_o:
	gcc -c darwin_bot_fc.c

clean: darwin_bot_fc
	rm *.o darwin_bot_fc

valgrind: darwin_bot_fc
	valgrind -v --leak-check=full --track-origins=yes ./../bin/darwin_bot_fc < 1.in