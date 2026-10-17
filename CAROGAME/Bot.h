//#ifndef BOT_H
//#define BOT_H
//
//#include "Board.h"
//#include <vector>
//
//struct Move {
//	int x, y, score;
//
//	bool operator < (const Move& other) {
//		return score > other.score;
//	}
//};
//
//struct Bot {
//	GameController* controller = nullptr;
//	Board* board = nullptr;
//	const int maxDepth = 5;
//	const int inf = 0x3f3f3f3f;
//	const int maxRange = 2;
//	int curX, curY;
//	
//
//	void makeMove();
//	void getCandidateMoves(vector<Move>*);
//	int evaluateBoard(int);
//	int minimax(int, int, int, int);
//
//	Bot() {}
//};
//
//#endif