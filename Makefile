CC = g++ 
CFLAGS = -std=c++17 -g -Wall

#タスクターゲットであることを明確にする"PHONY"は必ず書く
all : Prog1 Prog2 Prog3 Prog4

Prog1: idx4axkr_sh2.cpp
	$(CC) $(CFLAGS) -o schA idx4axkr_sh2.cpp

Prog2: tet3_bitreadnew3kr.cpp
	$(CC) $(CFLAGS) -o schB tet3_bitreadnew3kr.cpp

Prog3: gcis4.cpp
	$(CC) $(CFLAGS) -o bldA gcis4.cpp

Prog4: tet3_bitwrite.cpp
	$(CC) $(CFLAGS) -o bldB tet3_bitwrite.cpp