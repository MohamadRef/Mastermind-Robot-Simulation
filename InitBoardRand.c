


void InitBoardRand(int **board, int numRows, int numCols, unsigned int seed)
{
	/* Please note that NUM_COLOURS is a global constant or a MACRO*/
	srand(seed);
	for (int K = 0; K < numRows; K++)
	{
		for (int J = 0; J < numCols; J++)
		{
			board[K][J] = rand() % NUM_COLOURS + 1;
		}
	}
}

