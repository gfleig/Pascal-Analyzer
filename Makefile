analisador:*.cpp
	g++ -std=c++14 -Wall -Werror *.cpp -o analisador

.PHONY: clean

clean:
	rm analisador

debug:
	g++ -std=c++14 -Wall -Werror *.cpp -g -o analisador