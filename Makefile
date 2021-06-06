CC = g++ 
CFLAGS = -std=c++17 -g -Wall

all : Prog1 Prog2 Prog3 Prog4 Prog5

Prog1: idx_nep.cpp
	$(CC) $(CFLAGS) -o idx_nep idx_nep.cpp

Prog2: idx_uni.cpp
	$(CC) $(CFLAGS) -o idx_uni idx_uni.cpp

Prog3: gcis_nep.cpp
	$(CC) $(CFLAGS) -o gcis_nep gcis_nep.cpp

Prog4: gcis_uni.cpp
	$(CC) $(CFLAGS) -o gcis_uni gcis_uni.cpp

Prog5: genpattern.cpp
	$(CC) $(CFLAGS) -o genpattern genpattern.cpp