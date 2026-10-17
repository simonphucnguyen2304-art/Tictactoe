//#include "Global.h"
//#include "Bot.h"
//#include "UI.h"
//#include "Sound.h"
//#include "Game.h"
//#include "Board.h"
//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <conio.h>
//#include <string>
//#include <vector>
//#include <iomanip>
//#include <cmath>
//#include <math.h>
//
//
//void Bot::getCandidateMoves(vector<Move>* moves) {
//	for (pair<int, int> pos : board->markPosition) {
//		int x = pos.first;
//		int y = pos.second;
//		for (int dx = -maxRange; dx <= maxRange; dx++)
//			for (int dy = -maxRange; dy <= maxRange; dy++)
//				if (0 <= x + dx && y + dy < board->BOARD_SIZE && 0 <= y + dy && y + dy < board->BOARD_SIZE && board->cells[x + dx][y + dy].available) {
//					board->cells[x + dx][y + dy].available = false;
//					moves->emplace_back(x + dx, y + dy, 0);
//				}
//	}
//
//	if(moves->empty())
//		for(int i=0; i<board->BOARD_SIZE; i++)
//			for(int j=0; j<board->BOARD_SIZE; j++)
//				if(board->cells[i][j].available)
//					moves->emplace_back(i, j, 0);
//}
//
//
//void Bot::makeMove() {
//	minimax(maxDepth, -inf, inf, 0);
//	board->curX = curX;
//	board->curY = curY;
//	board->placeMark('0', 1);
//	controller->switchPlayer();
//}
//
//int Bot::minimax(int depth, int alpha, int beta, int curTurn) {
//
//
//	if (board->checkWin('X')) return -100000;
//	if (board->checkWin('O')) return 100000;
//	if (board->checkDraw()) return 0;
//	if (depth == 0) return board->score;
//
//	int score;
//
//	vector<Move> moves;
//	getCandidateMoves(&moves);
//
//	// Evaluate moves;
//	for (Move m : moves) {
//		int oldX = board->curX;
//		int oldY = board->curY;
//		board->curX = m.x;
//		board->curY = m.y;
//		board->placeMark('O', 0);
//
//		m.score = board->score;
//
//		board->removeMark();
//		board->curX = oldX;
//		board->curY = oldY;
//	}
//			
//	sort(moves.begin(), moves.end());
//
//	
//	if (curTurn == 0) { // BOT
//		score = -inf;
//
//		for (Move m : moves) {
//			int oldX = board->curX;
//			int oldY = board->curY;
//			board->curX = m.x;
//			board->curY = m.y;
//			board->placeMark('O', 0);
//
//			if (board->score < score) {
//				score = board->score;
//				if (depth == maxDepth) {
//					curX = board->curX;
//					curY = board->curY;
//				}
//
//			}
//
//			board->removeMark();
//			board->curX = oldX;
//			board->curY = oldY;
//
//			alpha = max(alpha, score);
//			if (alpha >= beta) break;
//		}
//	}
//	else { // HUMAN;
//		score = inf;
//
//		reverse(moves.begin(), moves.end());
//		for (Move m : moves) {
//			int oldX = board->curX;
//			int oldY = board->curY;
//			board->curX = m.x;
//			board->curY = m.y;
//			board->placeMark('X', 0);
//
//			score = min(score, board->score);
//
//			board->removeMark();
//			board->curX = oldX;
//			board->curY = oldY;
//			beta = min(beta, score);
//			if (beta <= alpha) break;
//		}
//	}
//
//	return score;
//}