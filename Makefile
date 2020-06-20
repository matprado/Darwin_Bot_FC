run:
	./bin/darwin_bot_fc	< 1.in

all:
	gcc ./src/cartola_utils.c ./src/algoritmo_genetico.c ./src/darwin_bot_fc.c -o ./bin/darwin_bot_fc -g -Wall

clean:
	rm ./src/*.o
	rm ./bin/darwin_bot_fc

valgrind:
	valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/darwin_bot_fc < 1.in