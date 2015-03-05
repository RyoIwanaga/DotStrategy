### Macro ###

CC=c++
CFLAGS=-Wall -O
CPP11=-std=c++11
EXECUTE=a.exe
#EXECUTE=./a.out
ALL=../Unit.cpp ../State.cpp ../Battle.cpp ../Floor.cpp ../../../boardgame/BoardGame.cpp

### Target ###

TEST_Floor: TEST_Floor.cpp
	${CC} ${CPP11} TEST_Floor.cpp ../Floor.cpp
	${EXECUTE}

TEST_Unit: TEST_Unit.cpp ../Unit.cpp
	${CC} ${CPP11} TEST_Unit.cpp ../Unit.cpp
	${EXECUTE}

TEST_State: TEST_State.cpp 
	${CC} ${CPP11} TEST_State.cpp ../Unit.cpp ../State.cpp ../Floor.cpp ../../../boardgame/BoardGame.cpp
	${EXECUTE}

TEST_Battle: TEST_Battle.cpp ../Unit.cpp ../UnitRanged.cpp ../Floor.cpp ../State.cpp ../Battle.cpp  ../../../boardgame/BoardGame.cpp
	${CC} ${CPP11} TEST_Battle.cpp ../Unit.cpp ../UnitRanged.cpp ../Floor.cpp ../State.cpp ../Battle.cpp  ../../../boardgame/BoardGame.cpp
	${EXECUTE}

TEST_Path: TEST_Path.cpp ../Battle.cpp ../Unit.cpp ../State.cpp ../../../boardgame/BoardGame.cpp
	${CC} ${CPP11} $^
	${EXECUTE}


# $@ 	ターゲットファイル名
# $< 	最初の依存ファイル名
# $? 	ターゲットより新しい全ての依存ファイル名
# $^ 	全ての依存ファイル名
# $+ 	Makefileと同じ順番の依存ファイル名
# $* 	suffixを除いたターゲット名
# $% 	アーカイブだった時のターゲットメンバ名 
