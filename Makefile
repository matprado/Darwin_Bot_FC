run:
	./bin/darwin_bot_fc

all:
	gcc ./src/cartola_utils.c ./src/algoritmo_genetico.c ./src/darwin_bot_fc.c -o ./bin/darwin_bot_fc -g -Wall

clean:
	rm ./src/*.o
	rm ./bin/darwin_bot_fc

valgrind:
	valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/darwin_bot_fc < entrada.in

plot: ./data/melhor_fitness.dat ./data/media_fitness.dat
	gnuplot -p -e "set autoscale; set xlabel 'Geração'; set ylabel 'Fitness'; plot './data/melhor_fitness.dat' with linespoints linestyle 16, './data/media_fitness.dat' with linespoints linestyle 11;"