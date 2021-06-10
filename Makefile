CC = g++ 
CFLAGS = -std=c++17 -g -Wall

all : Prog1 Prog2 Prog3 Prog4 Prog5

Prog1: gcis_nep_locate.cpp
	$(CC) $(CFLAGS) -o gcis_nep_locate gcis_nep_locate.cpp

Prog2: gcis_uni_locate.cpp
	$(CC) $(CFLAGS) -o gcis_uni_locate gcis_uni_locate.cpp

Prog3: gcis_nep_build.cpp
	$(CC) $(CFLAGS) -o gcis_nep_build gcis_nep_build.cpp

Prog4: gcis_uni_build.cpp
	$(CC) $(CFLAGS) -o gcis_uni_build gcis_uni_build.cpp

Prog5: genpattern.cpp
	$(CC) $(CFLAGS) -o genpattern genpattern.cpp