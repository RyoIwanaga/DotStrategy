### MACROS ###

CC=g++
CFLAGS=-Wall -O -W
CPP11=-std=gnu++11

### TESTS ###

all: BoardGame.cpp
	$(CC) $(CFLAGS) $(CPP11) -c BoardGame.cpp

# $@ 	ターゲットファイル名
# $< 	最初の依存ファイル名
# $? 	ターゲットより新しい全ての依存ファイル名
# $^ 	全ての依存ファイル名
# $+ 	Makefileと同じ順番の依存ファイル名
# $* 	suffixを除いたターゲット名
# $% 	アーカイブだった時のターゲットメンバ名 
