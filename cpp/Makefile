all:
	g++ log2hist.cc -o log2hist -std=gnu++98

test:
	g++ -lgtest t_log2hist.cc -o t_log2hist
	./t_log2hist

clean:
	rm log2hist t_log2hist
