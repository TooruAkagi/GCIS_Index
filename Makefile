CC = g++ 
CFLAGS = -std=c++17 -g -Wall

all : Prog1 Prog2 Prog3 Prog4 Prog5

Prog1: idx_nep.cpp
	$(CC) $(CFLAGS) -o schA idx_nep.cpp

Prog2: idx_uni.cpp
	$(CC) $(CFLAGS) -o schB idx_uni.cpp

Prog3: gcis.cpp
	$(CC) $(CFLAGS) -o bldA gcis.cpp

Prog4: gcis_uni.cpp
	$(CC) $(CFLAGS) -o bldB gcis_uni.cpp

Prog5: makequery.cpp
	$(CC) $(CFLAGS) -o genpattern makequery.cpp