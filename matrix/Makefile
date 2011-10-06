all : 
	@gcc -o2 iteration/matrix.c -o iteration/matrix
	@g++ -o2 pthreads/matrix.cc -o pthreads/matrix -lpthread
#	@gcc -o2 fork/matrix.c -o fork/matrix
	@javac java-threads/src/*.java -d java-threads/bin

test : all
	@grep "model name" /proc/cpuinfo
	@echo "\n         Method             Size        Time"
	@time -f "%e" ./iteration/matrix 10
	@time -f "%e" ./iteration/matrix 50
	@time -f "%e" ./iteration/matrix 100
	@time -f "%e" ./iteration/matrix 500
	@time -f "%e" ./iteration/matrix 1000
	@time -f "%e" ./pthreads/matrix 10
	@time -f "%e" ./pthreads/matrix 50
	@time -f "%e" ./pthreads/matrix 100
	@time -f "%e" ./pthreads/matrix 500
	@time -f "%e" ./pthreads/matrix 1000
#	@time -f "%e" ./fork/matrix 10
#	@time -f "%e" ./fork/matrix 50
#	@time -f "%e" ./fork/matrix 100
#	@time -f "%e" ./fork/matrix 500
#	@time -f "%e" ./fork/matrix 1000
	@time -f "%e" java -cp ./java-threads/bin/ Matrix 10
	@time -f "%e" java -cp ./java-threads/bin/ Matrix 50
	@time -f "%e" java -cp ./java-threads/bin/ Matrix 100
	@time -f "%e" java -cp ./java-threads/bin/ Matrix 500
	@time -f "%e" java -cp ./java-threads/bin/ Matrix 1000