run:
	./../bin/darwin_bot_fc	< 1.in

all: cartola_utils_o darwin_bot_fc_o
	gcc cartola_utils.o darwin_bot_fc.o -o ../bin/darwin_bot_fc -g -Wall

cartola_utils_o:
	gcc -c cartola_utils.c

darwin_bot_fc_o:
	gcc -c darwin_bot_fc.c

clean:
	rm *.o
	rm ../bin/darwin_bot_fc

valgrind:
	valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./../bin/darwin_bot_fc < 1.in