CC=g++
CFLAGS=-Ofast -std=gnu++17 -lboost_system -lboost_filesystem -lboost_thread -lpthread -lm -ldl -DBOOST_SYSTEM_NO_DEPRECATED -g

all: bin/ruota.out

bin/ruota.out: Compiled/Main.o Compiled/DataType.o Compiled/FileIO.o Compiled/RuotaWrapper.o Compiled/Tokenizer.o Compiled/Scope.o Compiled/Lambda.o Compiled/Node.o Compiled/DataPoint.o Compiled/Console.o
	$(CC) -o bin/ruota.out Compiled/Main.o Compiled/DataType.o Compiled/FileIO.o Compiled/RuotaWrapper.o Compiled/Tokenizer.o Compiled/Scope.o Compiled/Lambda.o Compiled/Node.o Compiled/DataPoint.o Compiled/Console.o $(CFLAGS)

Compiled/Main.o: Main.cpp Ruota/Ruota.h
	$(CC) Main.cpp -o Compiled/Main.o -c $(CFLAGS)

Compiled/FileIO.o: Ruota/FILE_IO/FileIO.cpp
	$(CC) Ruota/FILE_IO/FileIO.cpp -o Compiled/FileIO.o -c $(CFLAGS)
	
Compiled/RuotaWrapper.o: Ruota/RuotaWrapper.cpp
	$(CC) Ruota/RuotaWrapper.cpp -o Compiled/RuotaWrapper.o -c $(CFLAGS)

Compiled/Console.o: Ruota/Console.cpp
	$(CC) Ruota/Console.cpp -o Compiled/Console.o -c $(CFLAGS)

Compiled/Tokenizer.o: Ruota/Tokenizer.cpp
	$(CC) Ruota/Tokenizer.cpp -o Compiled/Tokenizer.o -c $(CFLAGS)

Compiled/Scope.o: Ruota/Scope.cpp
	$(CC) Ruota/Scope.cpp -o Compiled/Scope.o -c $(CFLAGS)

Compiled/Lambda.o: Ruota/Lambda.cpp
	$(CC) Ruota/Lambda.cpp -o Compiled/Lambda.o -c $(CFLAGS)

Compiled/Node.o: Ruota/Node.cpp
	$(CC) Ruota/Node.cpp -o Compiled/Node.o -c $(CFLAGS)

Compiled/DataPoint.o: Ruota/DataPoint.cpp
	$(CC) Ruota/DataPoint.cpp -o Compiled/DataPoint.o -c $(CFLAGS)

Compiled/DataType.o: Ruota/DataType.cpp
	$(CC) Ruota/DataType.cpp -o Compiled/DataType.o -c $(CFLAGS)