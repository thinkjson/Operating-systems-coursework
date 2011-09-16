all : central.c external.c
	cc -o central central.c temperature.h
	cc -o external external.c temperature.h
clean : 
	-killall external
	-killall central
	rm central external errlog output
test : 
	@echo "Loading all external processes into the background, and central server into the foreground..."
	@./external 100 1 2>> errlog &
	@./external 22 2 2>> errlog &
	@./external 50 3 2>> errlog &
	@./external 40 4 2>> errlog &
	@./central 60 | tee output