#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;

#define BOARD_DIM 7

int main()
{
	char * board = new char*[BOARD_DIM];
	for(int i = 0; i < BOARD_DIM; i++)
	{
		board[i] = new char[BOARD_DIM];
	}

	// Read in the text file
	char x;
	int idx = 0;
	ifstream infile;
	infile.open("board.txt");
	if(!infile)
	{
		while(infile >> x)
		{
			int idx_x = 
		}
	}    

}












