### MACROS ###

CC=g++
CFLAGS=-Wall -O -W
CPP11=-std=gnu++11
ALL=dod.cpp state.cpp

# g++  game_tree.cpp -o a  #実行コマンド
# g++ -Wall game_tree.cpp -o a  #実行コマンド

### TESTS ###

all: TEST.cpp BoardGame.cpp
	$(CXX) $(CFLAGS) $(CPP11) $^
	./a.out
	rm a.out


# $@ 	ターゲットファイル名
# $< 	最初の依存ファイル名
# $? 	ターゲットより新しい全ての依存ファイル名
# $^ 	全ての依存ファイル名
# $+ 	Makefileと同じ順番の依存ファイル名
# $* 	suffixを除いたターゲット名
# $% 	アーカイブだった時のターゲットメンバ名 
