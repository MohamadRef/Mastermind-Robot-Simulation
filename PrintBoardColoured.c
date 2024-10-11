
void PrintBoardColoured(FILE* stdStream, int **board, int numRows, int numCols)
{

	for (int K = 0; K < numRows; K++)
	{
		for (int J = 0; J < numCols; J++)
		{
			switch (board[K][J])
			{
			case 1:
				fprintf(stdStream, WHITE_PRINT"X"RESET_PRINT);
				break;

			case 2:
				fprintf(stdStream, RED_PRINT"X"RESET_PRINT);
				break;

			case 3:
				fprintf(stdStream, GREEN_PRINT"X"RESET_PRINT);
				break;

			case 4:
				fprintf(stdStream, BLUE_PRINT"X"RESET_PRINT);
				break;

			default:
				fprintf(stdStream, "O");
				break;
			}
		}
		printf("\n");
	}
	printf("\n\n");
}

void PrintBoard(FILE* myStream, int **board, int numRows, int numCols)
{

	for (int K = 0; K < numRows; K++)
	{
		for (int J = 0; J < numCols; J++)
		{
			fprintf(myStream, "%d", board[K][J]);
		}
		fprintf(myStream, "\n");
	}
	fprintf(myStream, "\n\n");
}

