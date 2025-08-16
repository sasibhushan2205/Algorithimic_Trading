#------------------------
# Do not modify this file
#------------------------

all: trader iml

trader: trader.cpp receiver.h
	g++ -std=c++20 trader.cpp -o trader

iml: iml.cpp
	g++ -std=c++20 iml.cpp -o iml

runpart2: iml trader
	./trader 2 > output.txt &
	sleep 1
	./iml inputs/data2.txt

runpart3: iml trader
	./trader 3 > output.txt &
	sleep 1
	./iml inputs/data3.txt

clean:
	rm -rf output.txt trader iml
	rm -rf *~