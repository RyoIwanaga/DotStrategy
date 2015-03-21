### Macro ###

CC=gcc
CCXX=g++
# CFLAGS=-Wall -O -W
# CPP11=-std=gnu++11

### Target ###

Sqlite: Sqlite.cpp ../sqlite3.c
#	$(CC) -c ../sqlite3.c
	$(CCXX) $(CFLAGS) $(CPP11) Sqlite.cpp sqlite3.o
	a.exe


# $@ 	ターゲットファイル名
# $< 	最初の依存ファイル名
# $? 	ターゲットより新しい全ての依存ファイル名
# $^ 	全ての依存ファイル名
# $+ 	Makefileと同じ順番の依存ファイル名
# $* 	suffixを除いたターゲット名
# $% 	アーカイブだった時のターゲットメンバ名 
