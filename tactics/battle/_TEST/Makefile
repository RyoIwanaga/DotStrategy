### Macro ###

CC=c++
CFLAGS=-Wall -O
CPP11=-std=c++11
EXECUTE=a.exe
#EXECUTE=./a.out
ALL=../Unit.cpp ../State.cpp ../Battle.cpp ../Floor.cpp ../../../boardgame/BoardGame.cpp
LIBS=../../../db/sqlite3.o ../../../boardgame/BoardGame.o

### Target ###

BoardGame.o:
	cd ../../../boardgame; make all

TEST_Path: TEST_Path.cpp
	${CC} ${CPP11} TEST_Path.cpp
	${EXECUTE}

TEST_Attack: TEST_Attack.cpp
	${CC} ${CPP11} TEST_Attack.cpp
	${EXECUTE}

TEST_Actions: TEST_Actions.cpp
	${CC} ${CPP11} TEST_Actions.cpp
	${EXECUTE}

TEST_Floor: TEST_Floor.cpp
	${CC} ${CPP11} TEST_Floor.cpp ../Floor.cpp
	${EXECUTE}

TEST_Unit: TEST_Unit.cpp ../Unit.cpp ../../../boardgame/BoardGame.o
	${CC} ${CPP11} ${LIBS} TEST_Unit.cpp ../Unit.cpp
	${EXECUTE}

TEST_State: TEST_State.cpp 
	${CC} ${CPP11} TEST_State.cpp ../Unit.cpp ../State.cpp ../Floor.cpp ../../../boardgame/BoardGame.cpp
	${EXECUTE}

TEST_Battle: TEST_Battle.cpp ../Unit.cpp ../UnitRanged.cpp ../Floor.cpp ../State.cpp ../Battle.cpp  ../../../boardgame/BoardGame.cpp
	${CC} ${CPP11} TEST_Battle.cpp ../Unit.cpp ../UnitRanged.cpp ../Floor.cpp ../State.cpp ../Battle.cpp  ../../../boardgame/BoardGame.cpp
	${EXECUTE}



# $@ 	ターゲットファイル名
# $< 	最初の依存ファイル名
# $? 	ターゲットより新しい全ての依存ファイル名
# $^ 	全ての依存ファイル名
# $+ 	Makefileと同じ順番の依存ファイル名
# $* 	suffixを除いたターゲット名
# $% 	アーカイブだった時のターゲットメンバ名 
