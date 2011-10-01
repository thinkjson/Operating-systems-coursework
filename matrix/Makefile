all : 
	@gcc -o2 iteration/matrix.c -o iteration/matrix
test : all
	@grep "model name" /proc/cpuinfo
	@echo "\n         Method             Size        Time"
	@time -f "%e" ./iteration/matrix 10
	@time -f "%e" ./iteration/matrix 100
	@time -f "%e" ./iteration/matrix 1000
	@time -f "%e" ./iteration/matrix 10000