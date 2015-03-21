### Macro ###

CC=c++
CFLAGS=-Wall -O
CPP11=-std=c++11
#EXECUTE=./a.out
EXECUTE=a.exe

### Target ###

diagram: diagram.cpp
	${CC} ${CPP11} diagram.cpp
	${EXECUTE}

random: random.cpp
	${CC} ${CPP11} random.cpp
	${EXECUTE}

# $@ 	ターゲットファイル名
# $< 	最初の依存ファイル名
# $? 	ターゲットより新しい全ての依存ファイル名
# $^ 	全ての依存ファイル名
# $+ 	Makefileと同じ順番の依存ファイル名
# $* 	suffixを除いたターゲット名
# $% 	アーカイブだった時のターゲットメンバ名 
