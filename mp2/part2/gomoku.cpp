#include "gomoku.h"


gomoku::gomoku()
{
	for(int j = 0; j < BOARD_DIM; j++)
	{
		std::vector<char> row;
		this-> board.push_back(row);
		for(int i = 0; i < BOARD_DIM; i++)
		{
			board[j].push_back('.');
		}
	}

}

void gomoku::print_board()
{
	for(int j = 0; j < BOARD_DIM; j++)
	{
		for(int i = 0; i < BOARD_DIM; i++)
		{
			char to_print = this->board[j][i];
			std::cout << to_print;
			std::cout << ' ';
		}
		std::cout << std::endl;
	}
}

void gomoku::play_game()
{
	// Tossing a coin to see who starts first

	// std::cout << "got into the function" <<endl;
	int first_player = std::rand() % 2;
    int next_player = 0;
    next_player = (first_player == PLAYER1) ? PLAYER2 : PLAYER1;
    std::cout << "first_player: " << first_player << endl;
    std::cout << "next_player: " << next_player << endl;

    // std::cout << first_player << endl;
    // std::cout << "getting the start index" <<endl;

    first_player = 0;
    next_player = 1;
    int start_index = winning_block(first_player);
    // cout << "placing the move" << endl;
    place_move(first_player,start_index);

    // cout << "before the players assignment" << endl;
    this->curr_player = next_player;
    this->prev_player = first_player;

    int num_moves = 0;
    // cout << "before the while loop" << endl;
    while(!is_winner(this->prev_player) && num_moves < 15)
    {
    	// First, we check if we have four stones any where
    	cout << "num_moves " << num_moves << endl;
    	int out1 = first_rule_checker(this->curr_player);
    	if(out1 != -1)
    	{
    		std::cout << "got into first rule checker" << std::endl;
    		place_move(this->curr_player, out1);
    		num_moves++;
    		int temp = this-> curr_player;
    		this -> curr_player = this-> prev_player;
    		this -> prev_player = temp;
    		continue;
    	}

    	int out2 = second_rule_checker(this->curr_player);
    	if(out2 != -1)
    	{
    		std::cout << "got into second rule checker" << std::endl;
    		place_move(this->curr_player, out2);
    		num_moves++;
    		int temp = this-> curr_player;
    		this -> curr_player = this-> prev_player;
    		this -> prev_player = temp;
    		continue;
    	}

    	int out3 = third_rule_checker(this->curr_player);
    	if(out3 != -1)
    	{
    		std::cout << "got into third rule checker" << std::endl;
    		place_move(this->curr_player, out3);
    		num_moves++;
    		int temp = this-> curr_player;
    		this -> curr_player = this-> prev_player;
    		this -> prev_player = temp;
    		continue;
    	}

    	int win_index = winning_block(this->curr_player);
    	place_move(this->curr_player, win_index);
    	num_moves++;
    	int temp = this-> curr_player;
    	this -> curr_player = this-> prev_player;
    	this -> prev_player = temp;

    	print_board();
    }

    if(num_moves < 48)
    {
    	cout << "Congrats to player " << this->prev_player << " !!!" << std::endl;
    }
    else
    	cout << "No winner was found :-(" << std::endl;
}

void gomoku::place_move(int player, int lin_idx)
{
	int x = lin_idx % BOARD_DIM;
	int y = lin_idx / BOARD_DIM;

	if(player == PLAYER1)
	{
		this->board[y][x] = 'a' + this->player_one_char;
		this->player_one_char++;
	}
	else
	{
		this->board[y][x] = 'A' + this->player_two_char;	
		this->player_two_char++;
	}

	// this -> prev_player = player;
	// this -> curr_player = (player == 0) ? 1 : 0;
}

bool gomoku::is_winner(int player)
{
	pair<int,int> best_row = winning_row(player);
	pair<int,int> best_col = winning_col(player);
	pair<int,int> best_ldiag = winning_ldiag(player);
	pair<int,int> best_rdiag = winning_rdiag(player);


	vector<int> best_number_pieces;
	best_number_pieces.push_back(best_row.second);
	best_number_pieces.push_back(best_col.second);
	best_number_pieces.push_back(best_ldiag.second);
	best_number_pieces.push_back(best_rdiag.second);


	auto iter_to_max = std::max_element(best_number_pieces.begin(), best_number_pieces.end());
	int max_element = *iter_to_max;

	if(max_element == 5)
		return true;

	return false;
}

int gomoku::first_rule_checker(int player)
{
	int from_rows = row_checker(player, 4);
	int from_cols = col_checker(player, 4);
	int from_rdiag= rdiag_checker(player, 4);
	int from_ldiag = ldiag_checker(player, 4);

	if(from_rows != -1 || from_cols != -1 || from_rdiag != -1 || from_ldiag != -1)
	{
		vector<int> vec;
		vec.push_back(from_rows);
		vec.push_back(from_cols);
		vec.push_back(from_ldiag);
		vec.push_back(from_rdiag);

		int store;
		for(int i = 0; i < 4; i++)
		{
			if(vec[i] != -1)
				return vec[i];

		}
	}

	return -1;

}

int gomoku::second_rule_checker(int player)
{
	int oppo = (player == PLAYER1) ? PLAYER2 : PLAYER1;

	int from_rows = row_checker(oppo, 4);
	int from_cols = col_checker(oppo, 4);
	int from_rdiag= rdiag_checker(oppo, 4);
	int from_ldiag = ldiag_checker(oppo, 4);

	if(from_rows != -1 || from_cols != -1 || from_rdiag != -1 || from_ldiag != -1)
	{
		vector<int> vec;
		vec.push_back(from_rows);
		vec.push_back(from_cols);
		vec.push_back(from_ldiag);
		vec.push_back(from_rdiag);

		for(int i = 0; i < 4; i++)
		{
			if(vec[i] != -1)
				return vec[i];
		}

	}
	return -1;
}

int gomoku::third_rule_checker(int player)
{

	int oppo = (player == PLAYER1) ? PLAYER2 : PLAYER1;

	int from_rows = row_checker(oppo, 3);
	int from_cols = col_checker(oppo, 3);
	int from_rdiag= rdiag_checker(oppo, 3);
	int from_ldiag = ldiag_checker(oppo, 3);

	if(from_rows != -1 || from_cols != -1 || from_rdiag != -1 ||from_ldiag != -1)
	{
		vector<int> vec;
		vec.push_back(from_rows);
		vec.push_back(from_cols);
		vec.push_back(from_ldiag);
		vec.push_back(from_rdiag);

		for(int i = 0; i < 4; i++)
		{
			if(vec[i] != -1)
				return vec[i];
		}

	}

	return -1;
}


int gomoku::winning_block(int player)
{
	// Returns the linear index of the best row, col, ldiag, and rdiag
	// Here best is defined as containing most of player's peices, and 
	pair<int,int> best_row = winning_row(player);
	pair<int,int> best_col = winning_col(player);
	pair<int,int> best_ldiag = winning_ldiag(player);
	pair<int,int> best_rdiag = winning_rdiag(player);

	// cout << "best_row.first = " << best_row.first << " best_row.second = " << best_row.second << endl;
	// cout << "best_col.first = " << best_col.first << " best_col.second = " << best_col.second << endl;
	// cout << "best_ldiag.first = " << best_ldiag.first << " best_ldiag.second = " << best_ldiag.second << endl;
	// cout << "best_rdiag.first = " << best_rdiag.first << " best_rdiag.second = " << best_rdiag.second << endl;


	vector<int> best_number_pieces;
	best_number_pieces.push_back(best_row.second);
	best_number_pieces.push_back(best_col.second);
	best_number_pieces.push_back(best_ldiag.second);
	best_number_pieces.push_back(best_rdiag.second);

	vector<int> linear_indices;
	linear_indices.push_back(best_row.first);
	linear_indices.push_back(best_col.first);
	linear_indices.push_back(best_ldiag.first);
	linear_indices.push_back(best_rdiag.first);

	//We got to return the best index in terms of not only agent peices but also left most and down most
	auto iter_to_max = std::max_element(best_number_pieces.begin(), best_number_pieces.end());
	int max_element = *iter_to_max;
	//std::cout << "max_element: " << max_element << endl;
	std::vector<int> best_indices;
	for(int i = 0; i < 4; i++)
	{
		if(max_element == best_number_pieces[i])
		{
			best_indices.push_back(linear_indices[i]);
		}
	}

	if(best_indices.size() > 1)
	{
		int best_x = best_indices[0] % BOARD_DIM;
		int best_y = best_indices[0] / BOARD_DIM;
		for(int i = 1; i < best_indices.size(); i++)
		{
			int curr_x = best_indices[i] % BOARD_DIM;
			int curr_y = best_indices[i] / BOARD_DIM;
			if(curr_x <= best_x && curr_y >= best_y)
			{
				best_x = curr_x;
				best_y = curr_y;
			}
		}

		// std::cout << "Index to be returned " << (BOARD_DIM*best_y) + best_x << endl;
		return (BOARD_DIM*best_y) + best_x;
	}

	// std::cout << "Index to be returned " << best_indices[0] << endl;
	return best_indices[0];
}












