
BOARD_DIM = 7

board =[ ['.' for x in range(BOARD_DIM)] for y in range(BOARD_DIM)]

# board[1][5] = 'A'
# board[5][1] = 'a'

f = open('board.txt', 'w+')

for row in range(BOARD_DIM):
	for col in range(BOARD_DIM):
		f.write(board[row][col])
	if(row < BOARD_DIM - 1):
		f.write('\r\n')    
    
f.close()
