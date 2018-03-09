#include "gomoku.h"

bool check_piece(char piece, int player)
{
	if(player == PLAYER1)
	{
		if(piece >= 'a' && piece <= 'z')
			return true;
	}
	else
	{
		if(piece >= 'A' && piece <= 'Z')
			return true;	
	}


	return false;
}

int gomoku::row_checker(int player, int piece_number)
{

	int oppo = (player == PLAYER1) ? PLAYER2 : PLAYER1;

	for(int row = 0; row < BOARD_DIM; row++)
	{
		bool cons_flag = false;
		int curr_pieces_player = 0;
		for(int col = 0; col < BOARD_DIM; col++)
		{
				if(check_piece((char)board[row][col], oppo))
				{
					if(cons_flag)
					{
						curr_pieces_player = 0;
						cons_flag = false;
					}
				}
				else if(check_piece((char)board[row][col], player))
				{
					cons_flag = true;
					curr_pieces_player++;
					if(curr_pieces_player == piece_number)
					{
						if(col - piece_number > -1)
							if(board[row][col - piece_number] == '.' )
								return (row * BOARD_DIM) + col - piece_number;

						if(col + 1 < BOARD_DIM)
							if(board[row][col + 1] == '.')
								return (row * BOARD_DIM) + col + 1;
					}
				}
				else
				{
					if(cons_flag)
					{
						curr_pieces_player = 0;
						cons_flag = false;
					}
				}
		}
	}
	return -1;
}

int gomoku::col_checker(int player, int piece_number)
{

	int oppo = (player == PLAYER1) ? PLAYER2 : PLAYER1;

	for(int col = 0; col < BOARD_DIM; col++)
	{
		bool cons_flag = false;
		int curr_pieces_player = 0;
		for(int row = 0; row < BOARD_DIM; row++)
		{
				if(check_piece((char)board[row][col], oppo))
				{
					if(cons_flag)
					{
						curr_pieces_player = 0;
						cons_flag = false;
					}
				}
				else if(check_piece((char)board[row][col], player))
				{
					cons_flag = true;
					curr_pieces_player++;
					if(curr_pieces_player == piece_number)
					{

						if(row + 1 < BOARD_DIM)
							if(board[row+1][col] == '.')
								return ( (row + 1) * BOARD_DIM) + col;

						if(row - piece_number > -1)
							if(board[row - piece_number][col] == '.')
								return ( (row - piece_number) * BOARD_DIM) + col;

					}
				}
				else
				{
					if(cons_flag)
					{
						curr_pieces_player = 0;
						cons_flag = false;
					}
				}
			
		}
		
	}
	return -1;
}

int gomoku::ldiag_checker(int player, int piece_number)
{
	int oppo = (player == PLAYER1) ? PLAYER2 : PLAYER1;

	for(int row = BOARD_DIM - 1; row > -1; row--)
	{
		for(int col = 0; col < BOARD_DIM; col++)
		{
			int idx = 0;
			int idy = 0;
			bool cons_flag = false;
			int curr_pieces_player = 0;
			for(idx = row, idy = col; idx > -1 && idy < BOARD_DIM; idx--,idy++)
			{			
				//std::cout << "row: " << idx << "col: " << idy << std::endl;
				if(check_piece((char)board[idx][idy], oppo))
				{
					if(cons_flag)
					{
						curr_pieces_player = 0;
						cons_flag = false;
					}
				}
				else if(check_piece((char)board[idx][idy], player))
				{
					cons_flag = true;
					curr_pieces_player++;
					if(curr_pieces_player == piece_number)
					{
						if(idx + piece_number < BOARD_DIM && idy - piece_number > -1)
							if(board[idx + piece_number][idy - piece_number] == '.')
								return ( (idx + piece_number) * BOARD_DIM) + idy - piece_number;

						if(idx - 1 > -1 && idy + 1 < BOARD_DIM)
							if(board[idx - 1][idy + 1] == '.')
								return ((idx - 1) * BOARD_DIM) + idy + 1;
					}
				}
				else
				{
					if(cons_flag)
					{						
						curr_pieces_player = 0;
						cons_flag = false;
					}
				}
			}
		}
	}

	return -1;

}

int gomoku::rdiag_checker(int player, int piece_number)
{
	int oppo = (player == PLAYER1) ? PLAYER2 : PLAYER1;

	for(int row = 0; row < BOARD_DIM; row++)
	{
		for(int col = 0; col < BOARD_DIM; col++)
		{
			int idx = 0;
			int idy = 0;
			bool cons_flag = false;
			int curr_pieces_player = 0;
			for(idx = row, idy = col; idx < BOARD_DIM && idy < BOARD_DIM; idx++,idy++)
			{
					if(check_piece((char)board[idx][idy], oppo))
					{
						if(cons_flag)
						{
							curr_pieces_player = 0;
							cons_flag = false;
						}
					}
					else if(check_piece((char)board[idx][idy], player))
					{
						cons_flag = true;
						curr_pieces_player++;
						if(curr_pieces_player == piece_number)
						{
							if(idx - piece_number > -1 && idy - piece_number > -1)
								if(board[idx - piece_number][idy - piece_number] == '.')
									return ( (idx - piece_number) * BOARD_DIM) + idy - piece_number;

							if(idx + 1 < BOARD_DIM && idy + 1 < BOARD_DIM)
								if(board[idx + 1][idy + 1] == '.')
									return ((idx + 1) * BOARD_DIM) + idy + 1;
						}
					}
					else
					{
						if(cons_flag)
						{
							curr_pieces_player = 0;
							cons_flag = false;
						}
					}
			}
		}
	}

	return -1;
}

pair<int,int> gomoku::winning_row(int player)
{

	std::vector<pair<int,int> > winning_rows;
	int best_player_count = 0;
	int oppo = (player == PLAYER1) ? PLAYER2 : PLAYER1;

	for(int row = 0; row < BOARD_DIM; row++)
		for(int col = 0; col < 3; col++)
		{
			int curr_player_count = 0;
			bool found_oppo = false;
			for(int idx = col; idx < col + 5; idx++)
			{
					// //std::cout << "Row: " << row << "Col: " << idx << std::endl;
					if(check_piece((char)board[row][idx], oppo))
					{
						found_oppo = true;
						break;
					}
					else if(check_piece((char)board[row][idx], player))
						curr_player_count++;

			}
			if(!found_oppo && curr_player_count >= best_player_count)
			{
				best_player_count = curr_player_count;

				// Store the linear index and the number of peices for future trimming
				pair<int,int> index_and_count(col + (BOARD_DIM*row), curr_player_count);
				winning_rows.push_back(index_and_count);
			}
		}


	std::vector<int> best_row;
	// Now we trim away the winning rows and get the row with most amount of current player's peices
	for(int i = 0; i < winning_rows.size(); i++)
	{
		pair<int,int> index_and_count = winning_rows[i];
		if(index_and_count.second == best_player_count)		
			best_row.push_back(index_and_count.first);
		
	}
	pair<int,int> fail_return = std::pair<int,int>(-1,-1);
	if(best_row.size() == 0)
		return fail_return;
	//If there are more than one good row, we pick the lowest and leftmost
	if(best_row.size() > 1)
	{
		int best_y = best_row[0] / BOARD_DIM;
		int best_x = best_row[0] % BOARD_DIM;
		for(int i = 1; i < best_row.size(); i++)
		{
			int curr_y = best_row[i] / BOARD_DIM;
			int curr_x = best_row[i] % BOARD_DIM;

			if(curr_x < best_x)
			{
				best_x = curr_x;
				best_y = curr_y;
			}
			else if(curr_x == best_x && curr_y > best_y)
			{
				best_x = curr_x;
				best_y = curr_y;
			}
		}

		for(int i = 0; i < 5 && best_x + i < BOARD_DIM; i++)
		{
			if(board[best_y][best_x + i] == '.')
			{
				best_x = best_x + i;
				break;
			}
		}

		pair<int, int> return_pair(best_y*BOARD_DIM + best_x, best_player_count);
		return return_pair;
	}

	int best_x = best_row[0] % BOARD_DIM;
	int best_y = best_row[0] / BOARD_DIM;

	for(int i = 0; i < 5 && best_x + i < BOARD_DIM; i++)
		{
			if(board[best_y][best_x + i] == '.')
			{
				best_x = best_x + i;
				break;
			}
		}


	pair<int, int> return_pair(best_y*BOARD_DIM + best_x, best_player_count);
	return return_pair;
}



pair<int,int> gomoku::winning_col(int player)
{
	std::vector<pair<int,int> > winning_cols;
	int best_player_count = 0;
	int oppo = (player == PLAYER1) ? PLAYER2 : PLAYER1;
	for(int col = 0; col < BOARD_DIM; col++)
		for(int row = 0; row < 3; row++)
		{
			int curr_player_count = 0;
			bool found_oppo = false;
			for(int idx = row; idx < row + 5; idx++)
			{
				// //std::cout << "Row: " << idx << " Col: " << col << std::endl;
				if(check_piece((char)board[idx][col], oppo))
				{
					found_oppo = true;
					break;
				}
				else if(check_piece((char)board[idx][col], player))
					curr_player_count++;
			}
			if(!found_oppo && curr_player_count >= best_player_count)
			{
				best_player_count = curr_player_count;

				// Store the linear index and the number of peices for future trimming
				pair<int,int> index_and_count(col + (BOARD_DIM*row), curr_player_count);
				winning_cols.push_back(index_and_count);
			}
		}

	// //std::cout << "winning_cols.size() = "<< winning_cols.size() << std::endl;
	std::vector<int> best_col;
	// Now we trim away the winning rows and get the row with most amount of current player's peices
	for(int i = 0; i < winning_cols.size(); i++)
	{
		pair<int,int> index_and_count = winning_cols[i];
		if(index_and_count.second == best_player_count)
		{
			best_col.push_back(index_and_count.first);
		}
	}
	pair<int,int> fail_return = std::pair<int,int>(-1,-1);
	if(best_col.size() == 0)
		return fail_return;
	// //std::cout << "best_col.size() = " << best_col.size() << std::endl;
	//If there are more than one good row, we pick the lowest and leftmost
	if(best_col.size() > 1)
	{
		int best_y = best_col[0] / BOARD_DIM;
		int best_x = best_col[0] % BOARD_DIM;
		for(int i = 1; i < best_col.size(); i++)
		{
			int curr_y = best_col[i] / BOARD_DIM;
			int curr_x = best_col[i] % BOARD_DIM;

			if(curr_x < best_x)
			{
				best_x = curr_x;
				best_y = curr_y;
			}
			else if(curr_x == best_x && curr_y > best_y)
			{
				best_x = curr_x;
				best_y = curr_y;
			}
		}

		for(int i = 4; i > -1; i--)
		{
			if(board[best_y + i][best_x] == '.')
			{
				best_y = best_y + i;
				break;
			}
		}

		std::pair<int,int> return_pair(best_y*BOARD_DIM + best_x,best_player_count);
		return return_pair;
	}

	int best_x = best_col[0]%BOARD_DIM;
	int best_y = best_col[0]/BOARD_DIM;

	for(int i = 4; i > -1; i--)
	{
		if(board[best_y + i][best_x] == '.')
		{
			best_y = best_y + i;
			break;
		}
	}

	std::pair<int,int> return_pair(best_y*BOARD_DIM + best_x,best_player_count);
	return return_pair;


}

pair<int,int> gomoku::winning_ldiag(int player)
{

	std::vector<pair<int,int> > winning_ldiags;
	int best_player_count = 0;
	int oppo = (player == PLAYER1) ? PLAYER2 : PLAYER1;

	for(int row = (BOARD_DIM - 1); row > BOARD_DIM - 4; row--)
		for(int col = 0; col < 3; col++)
		{
			int curr_player_count = 0;
			bool found_oppo = false;
			int idx_y = 0;
			int idx_x = 0;
			for(idx_y = row,idx_x = col; idx_y > row - 5 || idx_x < col + 5; idx_x++, idx_y--)
			{
				////std::cout << "Row: " << idx_y << " Col: " << idx_x << std::endl;
					if(check_piece(board[idx_y][idx_x], oppo))
					{
						////std::cout << "Got to this point" << endl;
						found_oppo = true;
						break;
					}
					else if(check_piece(board[idx_y][idx_x], player))
						curr_player_count++;
			}
			////std::cout << "curr_player_count = " << best_player_count << endl;
			if(!found_oppo && curr_player_count >= best_player_count)
			{
				////std::cout << "Got to this point " << std::endl;
				best_player_count = curr_player_count;
				pair<int,int> index_and_count(row*BOARD_DIM + col, curr_player_count);
				winning_ldiags.push_back(index_and_count);
			}

		}

	////std::cout << "winning_ldiags.size() = " << winning_ldiags.size() << endl;
	std::vector<int> best_ldiag;
	for(int i = 0; i < winning_ldiags.size(); i++)
	{
		if(winning_ldiags[i].second == best_player_count)
		{
			best_ldiag.push_back(winning_ldiags[i].first);
		}
	}

	pair<int,int> fail_return = std::pair<int,int>(-1,-1);
	if(best_ldiag.size() == 0)
		return fail_return;

	if(best_ldiag.size() > 1)
	{
		int best_x = best_ldiag[0] % BOARD_DIM;
		int best_y = best_ldiag[0] / BOARD_DIM;
		for(int i = 1; i < best_ldiag.size(); i++)
		{
			int curr_x = best_ldiag[i] % BOARD_DIM;
			int curr_y = best_ldiag[i] / BOARD_DIM;
			if(curr_x < best_x)
			{
				best_x = curr_x;
				best_y = curr_y;
			}
			else if(curr_x == best_x && curr_y > best_y)
			{
				best_x = curr_x;
				best_y = curr_y;
			}
		}

		for(int i = 0; i < 5 && best_y - i > -1 && best_x + i < BOARD_DIM; i++)
		{
			if(board[best_y - i][best_x + i] == '.')
			{
				best_x = best_x + i;
				best_y = best_y - i;
				break;
			}
		}

		pair<int,int> return_pair(best_y*BOARD_DIM + best_x,best_player_count);
		return return_pair;
	}

	int best_x = best_ldiag[0]%BOARD_DIM;
	int best_y = best_ldiag[0]/BOARD_DIM;
	for(int i = 0; i < 5 && best_y - i < BOARD_DIM && best_x + i > -1; i++)
		{
			if(board[best_y - i][best_x + i] == '.')
			{
				best_x = best_x + i;
				best_y = best_y - i;
				break;
			}
		}
	pair<int,int> return_pair(best_y*BOARD_DIM + best_x,best_player_count);	
	return return_pair;
}

pair<int,int> gomoku::winning_rdiag(int player)
{
	std::vector<pair<int,int> > winning_rdiags;
	int best_player_count = 0;
	int oppo = (player == PLAYER1) ? PLAYER2 : PLAYER1;

	for(int row = 0; row < 3; row++)
		for(int col = 0; col < 3; col++)
		{
			int curr_player_count = 0;
			bool found_oppo = false;
			int idx_y = 0;
			int idx_x = 0;
			for(idx_y = row,idx_x = col; idx_y < 5 + row || idx_x < 5 + col; idx_x++, idx_y++)
			{
				////std::cout << "Row: " << idx_y << " Col: " << idx_x << std::endl;
				if(check_piece(board[idx_y][idx_x], player))
				{
					////std::cout << "Got to this point" << endl;
					found_oppo = true;
					break;
				}
				else if(check_piece(board[idx_y][idx_x], player))
					curr_player_count++;
				
			}
			////std::cout << "curr_player_count = " << best_player_count << endl;
			if(!found_oppo && curr_player_count >= best_player_count)
			{
				////std::cout << "Got to this point " << std::endl;
				best_player_count = curr_player_count;
				pair<int,int> index_and_count(row*BOARD_DIM + col, curr_player_count);
				winning_rdiags.push_back(index_and_count);
			}

		}

	////std::cout << "winning_rdiags.size() = " << winning_rdiags.size() << endl;
	std::vector<int> best_rdiag;
	for(int i = 0; i < winning_rdiags.size(); i++)
	{
		if(winning_rdiags[i].second == best_player_count)
		{
			best_rdiag.push_back(winning_rdiags[i].first);
		}
	}
	pair<int,int> fail_return = std::pair<int,int>(-1,-1);
	if(best_rdiag.size() == 0)
		return fail_return;
	if(best_rdiag.size() > 1)
	{
		int best_x = best_rdiag[0] % BOARD_DIM;
		int best_y = best_rdiag[0] / BOARD_DIM;
		for(int i = 1; i < best_rdiag.size(); i++)
		{
			int curr_x = best_rdiag[i] % BOARD_DIM;
			int curr_y = best_rdiag[i] / BOARD_DIM;
			// if(curr_x <= best_x && curr_y >= best_y)
			// {
			// 	best_x = curr_x;
			// 	best_y = curr_y;
			// }
			if(curr_x < best_x)
			{
				best_x = curr_x;
				best_y = curr_y;
			}
			else if(curr_x == best_x && curr_y > best_y)
			{
				best_x = curr_x;
				best_y = curr_y;
			}

		}

		for(int i = 0; i < 5 && best_y + i < BOARD_DIM && best_x + i < BOARD_DIM; i++)
		{
			if(board[best_y + i][best_x + i] == '.')
			{
				best_x = best_x + i;
				best_y = best_y + i;
				break;
			}
		}

		pair<int,int> return_pair(best_y*BOARD_DIM + best_x,best_player_count);
		return return_pair;
	}

	int best_x = best_rdiag[0] % BOARD_DIM;
	int best_y = best_rdiag[0] / BOARD_DIM;
	for(int i = 0; i < 5 && best_y + i < BOARD_DIM && best_x + i < BOARD_DIM; i++)
	{
		if(board[best_y + i][best_x + i] == '.')
		{
			best_x = best_x + i;
			best_y = best_y + i;
			break;
		}
	}
	pair<int,int> return_pair(best_y*BOARD_DIM + best_x, best_player_count);
	return return_pair;
}



