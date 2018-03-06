#include <iostream>
#include <stdio.h>
#include <stdlib.h>    
#include <fstream>
#include <algorithm>
#include <vector>
#define BOARD_DIM 7
#define PLAYER1 0
#define PLAYER2 1
using namespace std;

class gomoku{

	private:
		std::vector<std::vector<char> > board;

		//Relating to the players of the game
		int prev_player;
		int curr_player;
		int player_one_char = 0;
		int player_two_char = 0;

	public:

		gomoku();
		void play_game();
		int return_move(int store, int index);

		int first_rule_checker(int player);
		int second_rule_checker(int player);
		int third_rule_checker(int player);

		int row_checker(int player, int piece_number);
		int col_checker(int player, int piece_number);
		int ldiag_checker(int player, int piece_number);
		int rdiag_checker(int player, int piece_number);


		int winning_block(int player);
		pair<int,int> winning_row(int player);
		pair<int,int> winning_col(int player);
		pair<int,int> winning_ldiag(int player);
		pair<int,int> winning_rdiag(int player);

		void place_move(int player, int lin_idx);
		void print_board();
		bool is_winner(int player);


};
