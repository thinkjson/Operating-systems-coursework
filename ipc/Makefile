all : central.c external.c
	cc -o central central.c temperature.h
	cc -o external external.c temperature.h
clean : 
	rm central external
test : all
	./external 100 1 &
	./external 22 2 &
	./external 50 3 &
	./external 40 4 &
	./central 60 &