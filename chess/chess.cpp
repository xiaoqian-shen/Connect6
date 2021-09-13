#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<iostream>
#include <windows.h>
#include<algorithm>
#define BLACK 0
#define WHITE 1
#define EMPTY 2
#define maxDepth 2
#define inf 100000
#define historylistSize 30
using namespace std;
struct Point { //点结构
	int x, y;
};
struct Step { //步结构
	Point first, second;
	int value;
};
int Board[19][19];//存储棋盘信息，其元素值为 BLACK, WHITE, EMPTY 之一
//估值函数,根据路估值
int Grade1(int Board[19][19], int who, int computerSide)
{
	//临时棋盘
	int tempBoard[19][19];
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			tempBoard[i][j] = Board[i][j];
		}
	}
	//tempBoard[step.first.x][step.first.y] = who;
	//tempBoard[step.second.x][step.second.y] = who;

	//估值准备
	int scoreOfRoad[7] = { 0, 17, 78,141, 788, 1030, 10000 };//每路棋子数对应的分值
	int myNumber[7] = { 0 }, oppNumber[7] = { 0 };//记录各种分量值的数量

	//统计每种分量值的数量
	//横向与纵向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			//横向统计各路中对应棋子数
			int mynum = 0, oppnum = 0;//用于记录该路敌我棋子数
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i][j + k] == computerSide)
				{
					mynum++;
				}
				else if (tempBoard[i][j + k] != EMPTY)
				{
					oppnum++;
				}
			}
			myNumber[mynum]++; oppNumber[oppnum]++;

			//纵向统计各路中对应棋子数
			mynum = 0; oppnum = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[j + k][i] == computerSide)
				{
					mynum++;
				}
				else if (tempBoard[j + k][i] != EMPTY)
				{
					oppnum++;
				}
			}
			if (oppnum == 0)
				myNumber[mynum]++;
			else if (mynum == 0)
				oppNumber[oppnum]++;
		}
	}

	//左斜方向
	for (int i = 0; i < 14; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int mynum = 0, oppnum = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i + k][j + k] == computerSide)
				{
					mynum++;
				}
				else if (tempBoard[i + k][j + k] != EMPTY)
				{
					oppnum++;
				}
			}
			if (oppnum == 0)
				myNumber[mynum]++;
			else if (mynum == 0)
				oppNumber[oppnum]++;
		}
	}

	//右斜
	for (int i = 5; i < 19; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int mynum = 0, oppnum = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i - k][j + k] == computerSide)
				{
					mynum++;
				}
				else if (tempBoard[i - k][j + k] != EMPTY)
				{
					oppnum++;
				}
			}
			if (oppnum == 0)
				myNumber[mynum]++;
			else if (mynum == 0)
				oppNumber[oppnum]++;
		}
	}

	//统计敌我分值
	int myScore = 0, oppScore = 0;
	for (int i = 1; i < 7; i++)
	{
		myScore += myNumber[i] * scoreOfRoad[i];
		oppScore += oppNumber[i] * scoreOfRoad[i];
	}

	return myScore - oppScore;
}
int ndnumber = 0;

//估值函数,基于棋形估值V2
int Grade21(int tempBoard[19][19], int computerSide)
{


	/* int tempBoard[19][19];
	 for (int i = 0; i < 19; i++)
	 {
		 for (int j = 0; j < 19; j++)
		 {
			 tempBoard[i][j] = Board[i][j];
		 }
	 }
	 tempBoard[x][y] = computerSide;*/
	 //估值准备
	const int D2 = 0, S2 = 1, A2 = 2, D3 = 3, S3 = 4, I3 = 5, A3 = 6, D4 = 7, S4 = 8, A4 = 9, D5 = 10, S5 = 11, A5 = 12, C6 = 13, CI = 14;//15种棋型
	int countB[15] = { 0 }, countW[15] = { 0 };//统计各种棋型的数量
	const int score[15] = { 0, 3 , 5, 0, 10, 20, 43, 0, 200,400,0,200,400,10000,10000 };//相对于每种棋型的分值

		/*棋型统计*/

	//七窗口
	//CI(长连),A5之一
	int A51[7] = { 2,0,0,0,0,0,2 };//A5的情况之一
	//横向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			int numB = 0, numW = 0;
			int flagB = 1, flagW = 1;
			for (int k = 0; k < 7; k++)
			{
				if (tempBoard[j + k][i] == BLACK)
					numB++;
				else if (tempBoard[j + k][i] == WHITE)
					numW++;
				if (tempBoard[j + k][i] != A51[k])
					flagB = 0;
				if (tempBoard[j + k][i] != ((A51[k] == 0) ? 1 : A51[k]))
					flagW = 0;
			}
			if (numB == 7)
				countB[CI]++;
			if (numW == 7)
				countW[CI]++;
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;
		}
	}
	//纵向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			int numB = 0, numW = 0;
			int flagB = 1, flagW = 1;
			for (int k = 0; k < 7; k++)
			{
				if (tempBoard[i][j + k] == BLACK)
					numB++;
				else if (tempBoard[i][j + k] == WHITE)
					numW++;
				if (tempBoard[i][j + k] != A51[k])
					flagB = 0;
				if (tempBoard[i][j + k] != ((A51[k] == 0) ? 1 : A51[k]))
					flagW = 0;
			}
			if (numB == 7)
				countB[CI]++;
			if (numW == 7)
				countW[CI]++;
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;
		}
	}
	//左斜
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++)
		{
			int numB = 0, numW = 0;
			int flagB = 1, flagW = 1;
			for (int k = 0; k < 7; k++)
			{
				if (tempBoard[i + k][j + k] == BLACK)
					numB++;
				else if (tempBoard[i + k][j + k] == WHITE)
					numW++;
				if (tempBoard[i + k][j + k] != A51[k])
					flagB = 0;
				if (tempBoard[i + k][j + k] != ((A51[k] == 0) ? 1 : A51[k]))
					flagW = 0;
			}
			if (numB == 7)
				countB[CI]++;
			if (numW == 7)
				countW[CI]++;
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;
		}
	}
	//右斜
	for (int i = 6; i < 19; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			int numB = 0, numW = 0;
			int flagB = 1, flagW = 1;
			for (int k = 0; k < 7; k++)
			{
				if (tempBoard[i - k][j + k] == BLACK)
					numB++;
				if (tempBoard[i - k][j + k] == WHITE)
					numW++;
				if (tempBoard[i - k][j + k] != A51[k])
					flagB = 0;
				if (tempBoard[i - k][j + k] != ((A51[k] == 0) ? 1 : A51[k]))
					flagW = 0;
			}
			if (numB == 7)
				countB[CI]++;
			if (numW == 7)
				countW[CI]++;
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;
		}
	}

	//六窗口
	//C6,S5,A4,S4,S3,S2
	//横向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int numB = 0, numW = 0, numE = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[j + k][i] == BLACK)
					numB++;
				else if (tempBoard[j + k][i] == WHITE)
					numW++;
				else if (tempBoard[j + k][i] == EMPTY)
					numE++;
			}
			if (numB == 6)
				countB[C6]++;
			else if (numW == 6)
				countW[C6]++;
			else if (numB == 5 && numE == 1)
				countB[S5]++;
			else if (numW == 5 && numE == 1)
				countW[S5]++;
			else if (numB == 4 && numE == 2 && tempBoard[j][i] == EMPTY && tempBoard[j + 5][i] == EMPTY)
				countB[A4]++;
			else if (numW == 4 && numE == 2 && tempBoard[j][i] == EMPTY && tempBoard[j + 5][i] == EMPTY)
				countW[A4]++;
			else if (numB == 4 && numE == 2)
				countB[S4]++;
			else if (numW == 4 && numE == 2)
				countW[S4]++;
			else if (numB == 3 && numE == 3)
			{
				if (tempBoard[j][i] == EMPTY && tempBoard[j + 5][i] == EMPTY && (j - 1 < 0 || j + 6 > 18 || tempBoard[j - 1][i] == WHITE || tempBoard[j + 6][i] == WHITE))
					countB[I3]++;
				else if (j + 7 < 19 && tempBoard[j + 6][i] == EMPTY && tempBoard[j + 7][i] == EMPTY && (j - 1 < 0 || tempBoard[j - 1][i] == WHITE))
					countB[I3]++;
				else if (j - 2 >= 0 && tempBoard[j - 1][i] == EMPTY && tempBoard[j - 2][i] == EMPTY && (j + 6 > 18 || tempBoard[j + 6][i] == WHITE))
					countB[I3]++;
				countB[S3]++;
			}
			else if (numW == 3 && numE == 3)
			{
				if (tempBoard[j][i] == EMPTY && tempBoard[j + 5][i] == EMPTY && (j - 1 < 0 || j + 6 > 18 || tempBoard[j - 1][i] == BLACK || tempBoard[j + 6][i] == BLACK))
					countW[I3]++;
				else if (j + 7 < 19 && tempBoard[j + 6][i] == EMPTY && tempBoard[j + 7][i] == EMPTY && (j - 1 < 0 || tempBoard[j - 1][i] == BLACK))
					countW[I3]++;
				else if (j - 2 >= 0 && tempBoard[j - 1][i] == EMPTY && tempBoard[j - 2][i] == EMPTY && (j + 6 > 18 || tempBoard[j + 6][i] == BLACK))
					countW[I3]++;
				countW[S3]++;
			}
			else if (numB == 2 && numE == 4)
				countB[S2]++;
			else if (numW == 2 && numE == 4)
				countW[S2]++;
		}
	}
	//纵向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int numB = 0, numW = 0, numE = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i][j + k] == BLACK)
					numB++;
				else if (tempBoard[i][j + k] == WHITE)
					numW++;
				else if (tempBoard[i][j + k] == EMPTY)
					numE++;
			}
			if (numB == 6)
				countB[C6]++;
			else if (numW == 6)
				countW[C6]++;
			else if (numB == 5 && numE == 1)
				countB[S5]++;
			else if (numW == 5 && numE == 1)
				countW[S5]++;
			else if (numB == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i][j + 5] == EMPTY)
				countB[A4]++;
			else if (numW == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i][j + 5] == EMPTY)
				countW[A4]++;
			else if (numB == 4 && numE == 2)
				countB[S4]++;
			else if (numW == 4 && numE == 2)
				countW[S4]++;
			else if (numB == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i][j + 5] == EMPTY && (j - 1 < 0 || j + 6 > 18 || tempBoard[i][j - 1] == WHITE || tempBoard[i][j + 6] == WHITE))
					countB[I3]++;
				else if (j + 7 < 19 && tempBoard[i][j + 6] == EMPTY && tempBoard[i][j + 7] == EMPTY && (j - 1 < 0 || tempBoard[i][j - 1] == WHITE))
					countB[I3]++;
				else if (j - 2 >= 0 && tempBoard[i][j - 1] == EMPTY && tempBoard[i][j - 2] == EMPTY && (j + 6 > 18 || tempBoard[i][j + 6] == WHITE))
					countB[I3]++;
				countB[S3]++;
			}
			else if (numW == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i][j + 5] == EMPTY && (j - 1 < 0 || j + 6 > 18 || tempBoard[i][j - 1] == BLACK || tempBoard[i][j + 6] == BLACK))
					countW[I3]++;
				else if (j + 7 < 19 && tempBoard[i][j + 6] == EMPTY && tempBoard[i][j + 7] == EMPTY && (j - 1 < 0 || tempBoard[i][j - 1] == BLACK))
					countW[I3]++;
				else if (j - 2 >= 0 && tempBoard[i][j - 1] == EMPTY && tempBoard[i][j - 2] == EMPTY && (j + 6 > 18 || tempBoard[i][j + 6] == BLACK))
					countW[I3]++;
				countW[S3]++;
			}
			else if (numB == 2 && numE == 4)
				countB[S2]++;
			else if (numW == 2 && numE == 4)
				countW[S2]++;
		}
	}
	//左斜
	for (int i = 0; i < 14; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int numB = 0, numW = 0, numE = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i + k][j + k] == BLACK)
					numB++;
				else if (tempBoard[i + k][j + k] == WHITE)
					numW++;
				else if (tempBoard[i + k][j + k] == EMPTY)
					numE++;
			}
			if (numB == 6)
				countB[C6]++;
			else if (numW == 6)
				countW[C6]++;
			else if (numB == 5 && numE == 1)
				countB[S5]++;
			else if (numW == 5 && numE == 1)
				countW[S5]++;
			else if (numB == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i + 5][j + 5] == EMPTY)
				countB[A4]++;
			else if (numW == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i + 5][j + 5] == EMPTY)
				countW[A4]++;
			else if (numB == 4 && numE == 2)
				countB[S4]++;
			else if (numW == 4 && numE == 2)
				countW[S4]++;
			else if (numB == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i + 5][j + 5] == EMPTY && (i - 1 < 0 || i + 6 > 18 || j - 1 < 0 || j + 6 > 18 || tempBoard[i - 1][j - 1] == WHITE || tempBoard[i + 6][j + 6] == WHITE))
					countB[I3]++;
				else if (i + 7 < 19 && j + 7 < 19 && tempBoard[i + 6][j + 6] == EMPTY && tempBoard[i + 7][j + 7] == EMPTY && (i - 1 < 0 || j - 1 < 0 || tempBoard[i - 1][j - 1] == WHITE))
					countB[I3]++;
				else if (i - 2 >= 0 && j - 2 >= 0 && tempBoard[i - 1][j - 1] == EMPTY && tempBoard[i - 2][j - 2] == EMPTY && (i + 6 > 18 || j + 6 > 18 || tempBoard[i + 6][j + 6] == WHITE))
					countB[I3]++;
				countB[S3]++;
			}
			else if (numW == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i + 5][j + 5] == EMPTY && (i - 1 < 0 || i + 6 > 18 || j - 1 < 0 || j + 6 > 18 || tempBoard[i - 1][j - 1] == BLACK || tempBoard[i + 6][j + 6] == BLACK))
					countW[I3]++;
				else if (i + 7 < 19 && j + 7 < 1 && tempBoard[i + 6][j + 6] == EMPTY && tempBoard[i + 7][j + 7] == EMPTY && (i - 1 < 0 || j - 1 < 0 || tempBoard[i - 1][j - 1] == BLACK))
					countW[I3]++;
				else if (i - 2 > 0 && j - 2 > 0 && tempBoard[i - 1][j - 1] == EMPTY && tempBoard[i - 2][j - 2] == EMPTY && (i + 6 > 18 || j + 6 > 18 || tempBoard[i + 6][j + 6] == BLACK))
					countW[I3]++;
				countW[S3]++;
			}
			else if (numB == 2 && numE == 4)
				countB[S2]++;
			else if (numW == 2 && numE == 4)
				countW[S2]++;
		}
	}
	//右斜
	for (int i = 5; i < 19; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int numB = 0, numW = 0, numE = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i - k][j + k] == BLACK)
					numB++;
				else if (tempBoard[i - k][j + k] == WHITE)
					numW++;
				else if (tempBoard[i - k][j + k] == EMPTY)
					numE++;
			}
			if (numB == 6)
				countB[C6]++;
			else if (numW == 6)
				countW[C6]++;
			else if (numB == 5 && numE == 1)
				countB[S5]++;
			else if (numW == 5 && numE == 1)
				countW[S5]++;
			else if (numB == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i - 5][j + 5] == EMPTY)
				countB[A4]++;
			else if (numW == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i - 5][j + 5] == EMPTY)
				countW[A4]++;
			else if (numB == 4 && numE == 2)
				countB[S4]++;
			else if (numW == 4 && numE == 2)
				countW[S4]++;
			else if (numB == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i - 5][j + 5] == EMPTY && (i + 1 > 18 || i - 6 < 0 || j - 1 < 0 || j + 6 > 18 || tempBoard[i + 1][j - 1] == WHITE || tempBoard[i - 6][j + 6] == WHITE))
					countB[I3]++;
				else if (i - 7 >= 0 && j + 7 < 19 && tempBoard[i - 6][j + 6] == EMPTY && tempBoard[i - 7][j + 7] == EMPTY && (i + 1 > 18 || j - 1 < 0 || tempBoard[i + 1][j - 1] == WHITE))
					countB[I3]++;
				else if (i + 2 < 19 && j - 2 >= 0 && tempBoard[i + 2][j - 2] == EMPTY && tempBoard[i + 1][j - 1] == EMPTY && (i - 6 < 0 || j + 6 > 18 || tempBoard[i - 6][j + 6] == WHITE))
					countB[I3]++;
				countB[S3]++;
			}
			else if (numW == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i - 5][j + 5] == EMPTY && (i + 1 > 18 || i - 6 < 0 || j - 1 < 0 || j + 6 > 18 || tempBoard[i + 1][j - 1] == BLACK || tempBoard[i - 6][j + 6] == BLACK))
					countW[I3]++;
				else if (i - 7 >= 0 && j + 7 < 19 && tempBoard[i - 6][j + 6] == EMPTY && tempBoard[i - 7][j + 7] == EMPTY && (i + 1 > 18 || j - 1 < 0 || tempBoard[i + 1][j - 1] == BLACK))
					countW[I3]++;
				else if (i + 2 < 19 && j - 2 >= 0 && tempBoard[i + 2][j - 2] == EMPTY && tempBoard[i + 1][j - 1] == EMPTY && (i - 6 < 0 || j + 6 > 18 || tempBoard[i - 6][j + 6] == BLACK))
					countW[I3]++;
				countW[S3]++;
			}
			else if (numB == 2 && numE == 4)
				countB[S2]++;
			else if (numW == 2 && numE == 4)
				countW[S2]++;
		}
	}


	//八窗口
	//A5,
	int A50[2][8] = { { 2,2,0,0,0,0,2,0 } , { 0,2,0,0,0,0,2,2  } };
	int A30[4][8] = { { 2,2,0,0,0,2,2,2 } , { 2,2,0,0,2,0,2,2 } , { 2,2,0,2,0,0,2,2 } , { 2,2,2,0,0,0,2,2 } };
	int A20[6][8] = { {2,2,0,0,2,2,2,2},{2,2,0,2,0,2,2,2},{2,2,0,2,2,0,2,2},{2,2,2,0,0,2,2,2},{2,2,2,0,2,0,2,2},{2,2,2,2,0,0,2,2} };
	//横向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			//A5
			int flagB = 1, flagW = 1;
			for (int m = 0; m < 2; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[j + k][i] != A50[m][k])
						flagB = 0;
					if (tempBoard[j + k][i] != ((A50[m][k] == 0) ? 1 : A50[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;

			//A3
			flagB = 1; flagW = 1;
			for (int m = 0; m < 4; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[j + k][i] != A30[m][k])
						flagB = 0;
					if (tempBoard[j + k][i] != ((A30[m][k] == 0) ? 1 : A30[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A3]++;
			if (flagW)
				countW[A3]++;

			//A2
			flagB = 1; flagW = 1;
			for (int m = 0; m < 6; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[j + k][i] != A20[m][k])
						flagB = 0;
					if (tempBoard[j + k][i] != ((A20[m][k] == 0) ? 1 : A20[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A2]++;
			if (flagW)
				countW[A2]++;
		}
	}
	//纵向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			//A5
			int flagB = 1, flagW = 1;
			for (int m = 0; m < 2; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i][j + k] != A50[m][k])
						flagB = 0;
					if (tempBoard[i][j + k] != ((A50[m][k] == 0) ? 1 : A50[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;

			//A3
			flagB = 1; flagW = 1;
			for (int m = 0; m < 4; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i][j + k] != A30[m][k])
						flagB = 0;
					if (tempBoard[i][j + k] != ((A30[m][k] == 0) ? 1 : A30[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A3]++;
			if (flagW)
				countW[A3]++;

			//A2
			flagB = 1; flagW = 1;
			for (int m = 0; m < 6; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i][j + k] != A20[m][k])
						flagB = 0;
					if (tempBoard[i][j + k] != ((A20[m][k] == 0) ? 1 : A20[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A2]++;
			if (flagW)
				countW[A2]++;
		}
	}
	//左斜
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
		{
			//A5
			int flagB = 1, flagW = 1;
			for (int m = 0; m < 2; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i + k][j + k] != A50[m][k])
						flagB = 0;
					if (tempBoard[i + k][j + k] != ((A50[m][k] == 0) ? 1 : A50[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;

			//A3
			flagB = 1; flagW = 1;
			for (int m = 0; m < 4; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i + k][j + k] != A30[m][k])
						flagB = 0;
					if (tempBoard[i + k][j + k] != ((A30[m][k] == 0) ? 1 : A30[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A3]++;
			if (flagW)
				countW[A3]++;

			//A2
			flagB = 1; flagW = 1;
			for (int m = 0; m < 6; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i + k][j + k] != A20[m][k])
						flagB = 0;
					if (tempBoard[i + k][j + k] != ((A20[m][k] == 0) ? 1 : A20[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A2]++;
			if (flagW)
				countW[A2]++;
		}
	}
	//右斜
	for (int i = 7; i < 19; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			//A5
			int flagB = 1, flagW = 1;
			for (int m = 0; m < 2; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i - k][j + k] != A50[m][k])
						flagB = 0;
					if (tempBoard[i - k][j + k] != ((A50[m][k] == 0) ? 1 : A50[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;

			//A3
			flagB = 1; flagW = 1;
			for (int m = 0; m < 4; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i - k][j + k] != A30[m][k])
						flagB = 0;
					if (tempBoard[i - k][j + k] != ((A30[m][k] == 0) ? 1 : A30[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A3]++;
			if (flagW)
				countW[A3]++;

			//A2
			flagB = 1; flagW = 1;
			for (int m = 0; m < 6; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i - k][j + k] != A20[m][k])
						flagB = 0;
					if (tempBoard[i - k][j + k] != ((A20[m][k] == 0) ? 1 : A20[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A2]++;
			if (flagW)
				countW[A2]++;
		}
	}
	//统计分数
	int Bscore = 0;
	int Wscore = 0;
	for (int i = 0; i < 15; i++)
	{
		Bscore = Bscore + countB[i] * score[i];
		Wscore = Wscore + countW[i] * score[i];
	}
	if (computerSide == BLACK)
		return  Bscore - Wscore;
	else
		return  Wscore - Bscore;
}
int Grade22(int board[19][19], int x, int y, int computerSide)
{
	int tempBoard[19][19];
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			tempBoard[i][j] = board[i][j];
		}
	}
	tempBoard[x][y] = computerSide;

	//估值准备
	const int D2 = 0, S2 = 1, A2 = 2, D3 = 3, S3 = 4, I3 = 5, A3 = 6, D4 = 7, S4 = 8, A4 = 9, D5 = 10, S5 = 11, A5 = 12, C6 = 13, CI = 14;//15种棋型
	int countB[15] = { 0 }, countW[15] = { 0 };//统计各种棋型的数量
	const int score[15] = { 0, 30 , 50, 0, 80, 100, 150, 0, 300,700,0,300,700,10000,10000 };//相对于每种棋型的分值

		/*棋型统计*/

	//七窗口
	//CI(长连),A5之一
	int A51[7] = { 2,0,0,0,0,0,2 };//A5的情况之一
	//横向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			int numB = 0, numW = 0;
			int flagB = 1, flagW = 1;
			for (int k = 0; k < 7; k++)
			{
				if (tempBoard[j + k][i] == BLACK)
					numB++;
				else if (tempBoard[j + k][i] == WHITE)
					numW++;
				if (tempBoard[j + k][i] != A51[k])
					flagB = 0;
				if (tempBoard[j + k][i] != ((A51[k] == 0) ? 1 : A51[k]))
					flagW = 0;
			}
			if (numB == 7)
				countB[CI]++;
			if (numW == 7)
				countW[CI]++;
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;
		}
	}
	//纵向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			int numB = 0, numW = 0;
			int flagB = 1, flagW = 1;
			for (int k = 0; k < 7; k++)
			{
				if (tempBoard[i][j + k] == BLACK)
					numB++;
				else if (tempBoard[i][j + k] == WHITE)
					numW++;
				if (tempBoard[i][j + k] != A51[k])
					flagB = 0;
				if (tempBoard[i][j + k] != ((A51[k] == 0) ? 1 : A51[k]))
					flagW = 0;
			}
			if (numB == 7)
				countB[CI]++;
			if (numW == 7)
				countW[CI]++;
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;
		}
	}
	//左斜
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++)
		{
			int numB = 0, numW = 0;
			int flagB = 1, flagW = 1;
			for (int k = 0; k < 7; k++)
			{
				if (tempBoard[i + k][j + k] == BLACK)
					numB++;
				else if (tempBoard[i + k][j + k] == WHITE)
					numW++;
				if (tempBoard[i + k][j + k] != A51[k])
					flagB = 0;
				if (tempBoard[i + k][j + k] != ((A51[k] == 0) ? 1 : A51[k]))
					flagW = 0;
			}
			if (numB == 7)
				countB[CI]++;
			if (numW == 7)
				countW[CI]++;
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;
		}
	}
	//右斜
	for (int i = 6; i < 19; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			int numB = 0, numW = 0;
			int flagB = 1, flagW = 1;
			for (int k = 0; k < 7; k++)
			{
				if (tempBoard[i - k][j + k] == BLACK)
					numB++;
				if (tempBoard[i - k][j + k] == WHITE)
					numW++;
				if (tempBoard[i - k][j + k] != A51[k])
					flagB = 0;
				if (tempBoard[i - k][j + k] != ((A51[k] == 0) ? 1 : A51[k]))
					flagW = 0;
			}
			if (numB == 7)
				countB[CI]++;
			if (numW == 7)
				countW[CI]++;
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;
		}
	}

	//六窗口
	//C6,S5,A4,S4,S3,S2
	//横向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int numB = 0, numW = 0, numE = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[j + k][i] == BLACK)
					numB++;
				else if (tempBoard[j + k][i] == WHITE)
					numW++;
				else if (tempBoard[j + k][i] == EMPTY)
					numE++;
			}
			if (numB == 6)
				countB[C6]++;
			else if (numW == 6)
				countW[C6]++;
			else if (numB == 5 && numE == 1)
				countB[S5]++;
			else if (numW == 5 && numE == 1)
				countW[S5]++;
			else if (numB == 4 && numE == 2 && tempBoard[j][i] == EMPTY && tempBoard[j + 5][i] == EMPTY)
				countB[A4]++;
			else if (numW == 4 && numE == 2 && tempBoard[j][i] == EMPTY && tempBoard[j + 5][i] == EMPTY)
				countW[A4]++;
			else if (numB == 4 && numE == 2)
				countB[S4]++;
			else if (numW == 4 && numE == 2)
				countW[S4]++;
			else if (numB == 3 && numE == 3)
			{
				if (tempBoard[j][i] == EMPTY && tempBoard[j + 5][i] == EMPTY && (j - 1 < 0 || j + 6 > 18 || tempBoard[j - 1][i] == WHITE || tempBoard[j + 6][i] == WHITE))
					countB[I3]++;
				else if (j + 7 < 19 && tempBoard[j + 6][i] == EMPTY && tempBoard[j + 7][i] == EMPTY && (j - 1 < 0 || tempBoard[j - 1][i] == WHITE))
					countB[I3]++;
				else if (j - 2 >= 0 && tempBoard[j - 1][i] == EMPTY && tempBoard[j - 2][i] == EMPTY && (j + 6 > 18 || tempBoard[j + 6][i] == WHITE))
					countB[I3]++;
				countB[S3]++;
			}
			else if (numW == 3 && numE == 3)
			{
				if (tempBoard[j][i] == EMPTY && tempBoard[j + 5][i] == EMPTY && (j - 1 < 0 || j + 6 > 18 || tempBoard[j - 1][i] == BLACK || tempBoard[j + 6][i] == BLACK))
					countW[I3]++;
				else if (j + 7 < 19 && tempBoard[j + 6][i] == EMPTY && tempBoard[j + 7][i] == EMPTY && (j - 1 < 0 || tempBoard[j - 1][i] == BLACK))
					countW[I3]++;
				else if (j - 2 >= 0 && tempBoard[j - 1][i] == EMPTY && tempBoard[j - 2][i] == EMPTY && (j + 6 > 18 || tempBoard[j + 6][i] == BLACK))
					countW[I3]++;
				countW[S3]++;
			}
			else if (numB == 2 && numE == 4)
				countB[S2]++;
			else if (numW == 2 && numE == 4)
				countW[S2]++;
		}
	}
	//纵向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int numB = 0, numW = 0, numE = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i][j + k] == BLACK)
					numB++;
				else if (tempBoard[i][j + k] == WHITE)
					numW++;
				else if (tempBoard[i][j + k] == EMPTY)
					numE++;
			}
			if (numB == 6)
				countB[C6]++;
			else if (numW == 6)
				countW[C6]++;
			else if (numB == 5 && numE == 1)
				countB[S5]++;
			else if (numW == 5 && numE == 1)
				countW[S5]++;
			else if (numB == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i][j + 5] == EMPTY)
				countB[A4]++;
			else if (numW == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i][j + 5] == EMPTY)
				countW[A4]++;
			else if (numB == 4 && numE == 2)
				countB[S4]++;
			else if (numW == 4 && numE == 2)
				countW[S4]++;
			else if (numB == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i][j + 5] == EMPTY && (j - 1 < 0 || j + 6 > 18 || tempBoard[i][j - 1] == WHITE || tempBoard[i][j + 6] == WHITE))
					countB[I3]++;
				else if (j + 7 < 19 && tempBoard[i][j + 6] == EMPTY && tempBoard[i][j + 7] == EMPTY && (j - 1 < 0 || tempBoard[i][j - 1] == WHITE))
					countB[I3]++;
				else if (j - 2 >= 0 && tempBoard[i][j - 1] == EMPTY && tempBoard[i][j - 2] == EMPTY && (j + 6 > 18 || tempBoard[i][j + 6] == WHITE))
					countB[I3]++;
				countB[S3]++;
			}
			else if (numW == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i][j + 5] == EMPTY && (j - 1 < 0 || j + 6 > 18 || tempBoard[i][j - 1] == BLACK || tempBoard[i][j + 6] == BLACK))
					countW[I3]++;
				else if (j + 7 < 19 && tempBoard[i][j + 6] == EMPTY && tempBoard[i][j + 7] == EMPTY && (j - 1 < 0 || tempBoard[i][j - 1] == BLACK))
					countW[I3]++;
				else if (j - 2 >= 0 && tempBoard[i][j - 1] == EMPTY && tempBoard[i][j - 2] == EMPTY && (j + 6 > 18 || tempBoard[i][j + 6] == BLACK))
					countW[I3]++;
				countW[S3]++;
			}
			else if (numB == 2 && numE == 4)
				countB[S2]++;
			else if (numW == 2 && numE == 4)
				countW[S2]++;
		}
	}
	//左斜
	for (int i = 0; i < 14; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int numB = 0, numW = 0, numE = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i + k][j + k] == BLACK)
					numB++;
				else if (tempBoard[i + k][j + k] == WHITE)
					numW++;
				else if (tempBoard[i + k][j + k] == EMPTY)
					numE++;
			}
			if (numB == 6)
				countB[C6]++;
			else if (numW == 6)
				countW[C6]++;
			else if (numB == 5 && numE == 1)
				countB[S5]++;
			else if (numW == 5 && numE == 1)
				countW[S5]++;
			else if (numB == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i + 5][j + 5] == EMPTY)
				countB[A4]++;
			else if (numW == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i + 5][j + 5] == EMPTY)
				countW[A4]++;
			else if (numB == 4 && numE == 2)
				countB[S4]++;
			else if (numW == 4 && numE == 2)
				countW[S4]++;
			else if (numB == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i + 5][j + 5] == EMPTY && (i - 1 < 0 || i + 6 > 18 || j - 1 < 0 || j + 6 > 18 || tempBoard[i - 1][j - 1] == WHITE || tempBoard[i + 6][j + 6] == WHITE))
					countB[I3]++;
				else if (i + 7 < 19 && j + 7 < 19 && tempBoard[i + 6][j + 6] == EMPTY && tempBoard[i + 7][j + 7] == EMPTY && (i - 1 < 0 || j - 1 < 0 || tempBoard[i - 1][j - 1] == WHITE))
					countB[I3]++;
				else if (i - 2 >= 0 && j - 2 >= 0 && tempBoard[i - 1][j - 1] == EMPTY && tempBoard[i - 2][j - 2] == EMPTY && (i + 6 > 18 || j + 6 > 18 || tempBoard[i + 6][j + 6] == WHITE))
					countB[I3]++;
				countB[S3]++;
			}
			else if (numW == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i + 5][j + 5] == EMPTY && (i - 1 < 0 || i + 6 > 18 || j - 1 < 0 || j + 6 > 18 || tempBoard[i - 1][j - 1] == BLACK || tempBoard[i + 6][j + 6] == BLACK))
					countW[I3]++;
				else if (i + 7 < 19 && j + 7 < 1 && tempBoard[i + 6][j + 6] == EMPTY && tempBoard[i + 7][j + 7] == EMPTY && (i - 1 < 0 || j - 1 < 0 || tempBoard[i - 1][j - 1] == BLACK))
					countW[I3]++;
				else if (i - 2 > 0 && j - 2 > 0 && tempBoard[i - 1][j - 1] == EMPTY && tempBoard[i - 2][j - 2] == EMPTY && (i + 6 > 18 || j + 6 > 18 || tempBoard[i + 6][j + 6] == BLACK))
					countW[I3]++;
				countW[S3]++;
			}
			else if (numB == 2 && numE == 4)
				countB[S2]++;
			else if (numW == 2 && numE == 4)
				countW[S2]++;
		}
	}
	//右斜
	for (int i = 5; i < 19; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int numB = 0, numW = 0, numE = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i - k][j + k] == BLACK)
					numB++;
				else if (tempBoard[i - k][j + k] == WHITE)
					numW++;
				else if (tempBoard[i - k][j + k] == EMPTY)
					numE++;
			}
			if (numB == 6)
				countB[C6]++;
			else if (numW == 6)
				countW[C6]++;
			else if (numB == 5 && numE == 1)
				countB[S5]++;
			else if (numW == 5 && numE == 1)
				countW[S5]++;
			else if (numB == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i - 5][j + 5] == EMPTY)
				countB[A4]++;
			else if (numW == 4 && numE == 2 && tempBoard[i][j] == EMPTY && tempBoard[i - 5][j + 5] == EMPTY)
				countW[A4]++;
			else if (numB == 4 && numE == 2)
				countB[S4]++;
			else if (numW == 4 && numE == 2)
				countW[S4]++;
			else if (numB == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i - 5][j + 5] == EMPTY && (i + 1 > 18 || i - 6 < 0 || j - 1 < 0 || j + 6 > 18 || tempBoard[i + 1][j - 1] == WHITE || tempBoard[i - 6][j + 6] == WHITE))
					countB[I3]++;
				else if (i - 7 >= 0 && j + 7 < 19 && tempBoard[i - 6][j + 6] == EMPTY && tempBoard[i - 7][j + 7] == EMPTY && (i + 1 > 18 || j - 1 < 0 || tempBoard[i + 1][j - 1] == WHITE))
					countB[I3]++;
				else if (i + 2 < 19 && j - 2 >= 0 && tempBoard[i + 2][j - 2] == EMPTY && tempBoard[i + 1][j - 1] == EMPTY && (i - 6 < 0 || j + 6 > 18 || tempBoard[i - 6][j + 6] == WHITE))
					countB[I3]++;
				countB[S3]++;
			}
			else if (numW == 3 && numE == 3)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i - 5][j + 5] == EMPTY && (i + 1 > 18 || i - 6 < 0 || j - 1 < 0 || j + 6 > 18 || tempBoard[i + 1][j - 1] == BLACK || tempBoard[i - 6][j + 6] == BLACK))
					countW[I3]++;
				else if (i - 7 >= 0 && j + 7 < 19 && tempBoard[i - 6][j + 6] == EMPTY && tempBoard[i - 7][j + 7] == EMPTY && (i + 1 > 18 || j - 1 < 0 || tempBoard[i + 1][j - 1] == BLACK))
					countW[I3]++;
				else if (i + 2 < 19 && j - 2 >= 0 && tempBoard[i + 2][j - 2] == EMPTY && tempBoard[i + 1][j - 1] == EMPTY && (i - 6 < 0 || j + 6 > 18 || tempBoard[i - 6][j + 6] == BLACK))
					countW[I3]++;
				countW[S3]++;
			}
			else if (numB == 2 && numE == 4)
				countB[S2]++;
			else if (numW == 2 && numE == 4)
				countW[S2]++;
		}
	}


	//八窗口
	//A5,
	int A50[2][8] = { { 2,2,0,0,0,0,2,0 } , { 0,2,0,0,0,0,2,2  } };
	int A30[4][8] = { { 2,2,0,0,0,2,2,2 } , { 2,2,0,0,2,0,2,2 } , { 2,2,0,2,0,0,2,2 } , { 2,2,2,0,0,0,2,2 } };
	int A20[6][8] = { {2,2,0,0,2,2,2,2},{2,2,0,2,0,2,2,2},{2,2,0,2,2,0,2,2},{2,2,2,0,0,2,2,2},{2,2,2,0,2,0,2,2},{2,2,2,2,0,0,2,2} };
	//横向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			//A5
			int flagB = 1, flagW = 1;
			for (int m = 0; m < 2; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[j + k][i] != A50[m][k])
						flagB = 0;
					if (tempBoard[j + k][i] != ((A50[m][k] == 0) ? 1 : A50[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;

			//A3
			flagB = 1; flagW = 1;
			for (int m = 0; m < 4; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[j + k][i] != A30[m][k])
						flagB = 0;
					if (tempBoard[j + k][i] != ((A30[m][k] == 0) ? 1 : A30[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A3]++;
			if (flagW)
				countW[A3]++;

			//A2
			flagB = 1; flagW = 1;
			for (int m = 0; m < 6; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[j + k][i] != A20[m][k])
						flagB = 0;
					if (tempBoard[j + k][i] != ((A20[m][k] == 0) ? 1 : A20[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A2]++;
			if (flagW)
				countW[A2]++;
		}
	}
	//纵向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			//A5
			int flagB = 1, flagW = 1;
			for (int m = 0; m < 2; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i][j + k] != A50[m][k])
						flagB = 0;
					if (tempBoard[i][j + k] != ((A50[m][k] == 0) ? 1 : A50[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;

			//A3
			flagB = 1; flagW = 1;
			for (int m = 0; m < 4; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i][j + k] != A30[m][k])
						flagB = 0;
					if (tempBoard[i][j + k] != ((A30[m][k] == 0) ? 1 : A30[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A3]++;
			if (flagW)
				countW[A3]++;

			//A2
			flagB = 1; flagW = 1;
			for (int m = 0; m < 6; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i][j + k] != A20[m][k])
						flagB = 0;
					if (tempBoard[i][j + k] != ((A20[m][k] == 0) ? 1 : A20[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A2]++;
			if (flagW)
				countW[A2]++;
		}
	}
	//左斜
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
		{
			//A5
			int flagB = 1, flagW = 1;
			for (int m = 0; m < 2; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i + k][j + k] != A50[m][k])
						flagB = 0;
					if (tempBoard[i + k][j + k] != ((A50[m][k] == 0) ? 1 : A50[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;

			//A3
			flagB = 1; flagW = 1;
			for (int m = 0; m < 4; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i + k][j + k] != A30[m][k])
						flagB = 0;
					if (tempBoard[i + k][j + k] != ((A30[m][k] == 0) ? 1 : A30[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A3]++;
			if (flagW)
				countW[A3]++;

			//A2
			flagB = 1; flagW = 1;
			for (int m = 0; m < 6; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i + k][j + k] != A20[m][k])
						flagB = 0;
					if (tempBoard[i + k][j + k] != ((A20[m][k] == 0) ? 1 : A20[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A2]++;
			if (flagW)
				countW[A2]++;
		}
	}
	//右斜
	for (int i = 7; i < 19; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			//A5
			int flagB = 1, flagW = 1;
			for (int m = 0; m < 2; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i - k][j + k] != A50[m][k])
						flagB = 0;
					if (tempBoard[i - k][j + k] != ((A50[m][k] == 0) ? 1 : A50[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A5]++;
			if (flagW)
				countW[A5]++;

			//A3
			flagB = 1; flagW = 1;
			for (int m = 0; m < 4; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i - k][j + k] != A30[m][k])
						flagB = 0;
					if (tempBoard[i - k][j + k] != ((A30[m][k] == 0) ? 1 : A30[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A3]++;
			if (flagW)
				countW[A3]++;

			//A2
			flagB = 1; flagW = 1;
			for (int m = 0; m < 6; m++)
			{
				flagB = 1; flagW = 1;
				for (int k = 0; k < 8; k++)
				{
					if (tempBoard[i - k][j + k] != A20[m][k])
						flagB = 0;
					if (tempBoard[i - k][j + k] != ((A20[m][k] == 0) ? 1 : A20[m][k]))
						flagW = 0;
					if (!flagB && !flagW)
						break;
				}
				if (flagB || flagW)
					break;
			}
			if (flagB)
				countB[A2]++;
			if (flagW)
				countW[A2]++;
		}
	}

	//统计分数
	int Bscore = 0;
	int Wscore = 0;
	for (int i = 0; i < 15; i++)
	{
		Bscore = Bscore + countB[i] * score[i];
		Wscore = Wscore + countW[i] * score[i];
	}
	/*for (int i = 0; i < 15; i++)
	{
		cout << "countB[" << i << "] = " << countB[i] << endl;
		cout << "countW[" << i << "] = " << countW[i] << endl;
	}*/
	if (computerSide == BLACK)
		return 2 * Bscore - Wscore;
	else
		return 2 * Wscore - Bscore;
}
Step Hit(int tempBoard[19][19], int computerSide)
{
	Step hit;
	hit.first = { -1 , -1 };
	hit.second = { -1 , -1 };
	hit.value = 0;
	//横向与纵向
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			//纵向统计各路中对应棋子数
			int mynum = 0, oppnum = 0;//用于记录该路敌我棋子数
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i][j + k] == computerSide)
				{
					mynum++;
				}
				else if (tempBoard[i][j + k] != EMPTY)
				{
					oppnum++;
				}
			}
			//我方能六连
			if (oppnum == 0 && (mynum == 4 || mynum == 5))
			{
				hit.first = { -1 , -1 };
				hit.second = { -1 , -1 };
				for (int k = 0; k < 6; k++)
				{
					if (tempBoard[i][j + k] == EMPTY && hit.first.x == -1)
					{
						hit.first.x = i;
						hit.first.y = j + k;
					}
					else if (tempBoard[i][j + k] == EMPTY)
					{
						hit.second.x = i;
						hit.second.y = j + k;
					}
				}
				return hit;
			}
			//对方五连
			else if (mynum == 0 && oppnum == 5)
			{
				for (int k = 0; k < 6; k++)
				{
					if (tempBoard[i][j + k] == EMPTY && hit.first.x == -1)
					{
						hit.first.x = i;
						hit.first.y = j + k;
					}
					else if (tempBoard[i][j + k] == EMPTY && (hit.first.x != i || hit.first.y != j + k) && hit.second.x == -1)
					{
						hit.second.x = i;
						hit.second.y = j + k;
					}
				}
			}
			//对方四连
			else if (mynum == 0 && oppnum == 4)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i][j + 5] == EMPTY)
				{
					if (j - 1 >= 0 && j + 6 < 19 && tempBoard[i][j - 1] == EMPTY && tempBoard[i][j + 6] == EMPTY)
					{
						hit.first.x = i; hit.first.y = j;
						hit.second.x = i; hit.second.y = j + 5;
					}
				}
				else
				{
					for (int k = 1; k < 5; k++)
					{
						if (tempBoard[i][j + k] == EMPTY && hit.first.x == -1)
						{
							hit.first.x = i;
							hit.first.y = j + k;
							break;
						}
						else if (hit.first.x == i && hit.first.y == j + k)
						{
							break;
						}
						else if (tempBoard[i][j + k] == EMPTY && hit.second.x == -1)
						{
							hit.second.x = i;
							hit.second.y = j + k;
							break;
						}
					}
				}
			}

			//横向统计各路中对应棋子数
			mynum = 0; oppnum = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[j + k][i] == computerSide)
				{
					mynum++;
				}
				else if (tempBoard[j + k][i] != EMPTY)
				{
					oppnum++;
				}
			}
			//我方能六连
			if (oppnum == 0 && (mynum == 4 || mynum == 5))
			{
				hit.first = { -1 , -1 };
				hit.second = { -1 , -1 };
				for (int k = 0; k < 6; k++)
				{
					if (tempBoard[j + k][i] == EMPTY && hit.first.x == -1)
					{
						hit.first.x = j + k;
						hit.first.y = i;
					}
					else if (tempBoard[j + k][i] == EMPTY)
					{
						hit.second.x = j + k;
						hit.second.y = i;
					}
				}
				return hit;
			}
			//对方五连
			else if (mynum == 0 && oppnum == 5)
			{
				for (int k = 0; k < 6; k++)
				{
					if (tempBoard[j + k][i] == EMPTY && hit.first.x == -1)
					{
						hit.first.x = j + k;
						hit.first.y = i;
					}
					else if (tempBoard[j + k][i] == EMPTY && (hit.first.x != j + k || hit.first.y != i) && hit.second.x == -1)
					{
						hit.second.x = j + k;
						hit.second.y = i;
					}
				}
			}
			//对方四连
			else if (mynum == 0 && oppnum == 4)
			{
				if (tempBoard[j][i] == EMPTY && tempBoard[j + 5][i] == EMPTY)
				{
					if (j - 1 >= 0 && j + 6 < 19 && tempBoard[j - 1][i] == EMPTY && tempBoard[j + 6][i] == EMPTY)
					{
						hit.first.x = j; hit.first.y = i;
						hit.second.x = j + 5; hit.second.y = i;
					}

				}
				else
				{
					for (int k = 1; k < 5; k++)
					{
						if (tempBoard[j + k][i] == EMPTY && hit.first.x == -1)
						{
							hit.first.x = j + k;
							hit.first.y = i;
							break;
						}
						else if (hit.first.x == j + k && hit.first.y == i)
						{
							break;
						}
						else if (tempBoard[j + k][i] == EMPTY && hit.second.x == -1)
						{
							hit.second.x = j + k;
							hit.second.y = i;
							break;
						}
					}
				}
			}
		}
	}

	//左斜方向
	for (int i = 0; i < 14; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int mynum = 0, oppnum = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i + k][j + k] == computerSide)
				{
					mynum++;
				}
				else if (tempBoard[i + k][j + k] != EMPTY)
				{
					oppnum++;
				}
			}

			//我方能六连
			if (oppnum == 0 && (mynum == 4 || mynum == 5))
			{
				hit.first = { -1 , -1 };
				hit.second = { -1 , -1 };
				for (int k = 0; k < 6; k++)
				{
					if (tempBoard[i + k][j + k] == EMPTY && hit.first.x == -1)
					{
						hit.first.x = i + k;
						hit.first.y = j + k;
					}
					else if (tempBoard[i + k][j + k] == EMPTY)
					{
						hit.second.x = i + k;
						hit.second.y = j + k;
					}
				}
				return hit;
			}
			//对方五连
			else if (mynum == 0 && oppnum == 5)
			{
				for (int k = 0; k < 6; k++)
				{
					if (tempBoard[i + k][j + k] == EMPTY && hit.first.x == -1)
					{
						hit.first.x = i + k;
						hit.first.y = j + k;
					}
					else if (tempBoard[i + k][j + k] == EMPTY && (hit.first.x != i + k || hit.first.y != j + k) && hit.second.x == -1)
					{
						hit.second.x = i + k;
						hit.second.y = j + k;
					}
				}
			}
			//对方四连
			else if (mynum == 0 && oppnum == 4)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i + 5][j + 5] == EMPTY)
				{
					if (i - 1 >= 0 && j - 1 >= 0 && i + 6 < 19 && j + 6 < 19 && tempBoard[i - 1][j - 1] == EMPTY && tempBoard[i + 6][j + 6] == EMPTY)
					{
						hit.first.x = i; hit.first.y = j;
						hit.second.x = i + 5; hit.second.y = j + 5;
					}

				}
				else
				{
					for (int k = 1; k < 5; k++)
					{
						if (tempBoard[i + k][j + k] == EMPTY && hit.first.x == -1)
						{
							hit.first.x = i + k;
							hit.first.y = j + k;
							break;
						}
						else if (hit.first.x == i + k && hit.first.y == j + k)
						{
							break;
						}
						else if (tempBoard[i + k][j + k] == EMPTY && hit.second.x == -1)
						{
							hit.second.x = i + k;
							hit.second.y = j + k;
							break;
						}
					}
				}
			}
		}
	}

	//右斜
	for (int i = 5; i < 19; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			int mynum = 0, oppnum = 0;
			for (int k = 0; k < 6; k++)
			{
				if (tempBoard[i - k][j + k] == computerSide)
				{
					mynum++;
				}
				else if (tempBoard[i - k][j + k] != EMPTY)
				{
					oppnum++;
				}
			}
			//我方能六连
			if (oppnum == 0 && (mynum == 4 || mynum == 5))
			{
				hit.first = { -1 , -1 };
				hit.second = { -1 , -1 };
				for (int k = 0; k < 6; k++)
				{
					if (tempBoard[i - k][j + k] == EMPTY && hit.first.x == -1)
					{
						hit.first.x = i - k;
						hit.first.y = j + k;
					}
					else if (tempBoard[i - k][j + k] == EMPTY)
					{
						hit.second.x = i - k;
						hit.second.y = j + k;
					}
				}
				return hit;
			}
			//对方五连
			else if (mynum == 0 && oppnum == 5)
			{
				for (int k = 0; k < 6; k++)
				{
					if (tempBoard[i - k][j + k] == EMPTY && hit.first.x == -1)
					{
						hit.first.x = i - k;
						hit.first.y = j + k;
					}
					else if (tempBoard[i - k][j + k] == EMPTY && (hit.first.x != i - k || hit.first.y != j + k) && hit.second.x == -1)
					{
						hit.second.x = i - k;
						hit.second.y = j + k;
					}
				}
			}
			//对方四连
			else if (mynum == 0 && oppnum == 4)
			{
				if (tempBoard[i][j] == EMPTY && tempBoard[i - 5][j + 5] == EMPTY)
				{
					if (i + 1 < 19 && j - 1 >= 0 && i - 6 >= 0 && j + 6 < 19 && tempBoard[i + 1][j - 1] == EMPTY && tempBoard[i - 6][j + 6] == EMPTY)
					{
						hit.first.x = i; hit.first.y = j;
						hit.second.x = i - 5; hit.second.y = j + 5;
					}

				}
				else
				{
					for (int k = 1; k < 5; k++)
					{
						if (tempBoard[i - k][j + k] == EMPTY && hit.first.x == -1)
						{
							hit.first.x = i - k;
							hit.first.y = j + k;
							break;
						}
						else if (hit.first.x == i - k && hit.first.y == j + k)
						{
							break;
						}
						else if (tempBoard[i - k][j + k] == EMPTY && hit.second.x == -1)
						{
							hit.second.x = i - k;
							hit.second.y = j + k;
						}
					}
				}
			}
		}
	}
	return hit;
}

bool check(int Board[19][19], int x, int y)
{
	int direction[8][2]{ {-1,-1} , {0 , -1} , { 1, -1} , {-1 , 0} , {1 , 0} , {-1 , 1} , { 0 ,1} , {1 , 1} };
	for (int i = 0; i < 8; i++)
	{
		int a = x + direction[i][0], b = y + direction[i][1];
		if (a >= 0 && a < 19 && b >= 0 && b < 19) {
			if (Board[a][b] != EMPTY)
				return true;
		}
	}
	return false;
}
bool judgeValue(int Panel[19][19], int x, int y)//此函数为判断棋盘上的某个点是否有进入博弈树的必要
{
	int addx[8] = { -1,1,0,0,-1,1,-1,1 };
	int addy[8] = { 0,0,-1,1,-1,-1,1,1 };
	bool fot = false;
	for (int i = 0; i < 8; i++)
	{
		if (x + addx[i] >= 0 && x + addx[i] < 19 && y + addy[i] >= 0 && y + addy[i] < 19)
		{
			if (Panel[x + addx[i]][y + addy[i]] != 2)
			{
				//cout << i<<endl;
				return true;
			}
		}
	}
	return false;
}
struct pane
{
	int chess = 0;
	int enpty = 0;
	int otherchessd = 0;
	int successchess = 0;
};
struct pane  chessNumber(int Panel[19][19], int direction, int x, int y, int otherColor)
{
	int addx[8] = { -1,1,0,0,-1,1,-1,1 };
	int addy[8] = { 0,0,-1,1,-1,-1,1,1 };
	int chess = 0;
	int empty = 0;
	int otherchessd = 0;
	int lastx = -1;
	int lasty = -1;
	int i = 1;
	int nf = 1;
	struct pane s;
	s.chess = 0;
	s.enpty = 0;
	s.otherchessd = 0;
	s.successchess = 0;
	if (Panel[x + addx[direction]][y + addy[direction]] == 1 - otherColor)
		s.successchess = 0;
	else
		s.successchess = -1;
	while ((x + addx[direction] < 19 && x + addx[direction] >= 0) && (y + addy[direction] < 19 && y + addy[direction] >= 0) && Panel[x + addx[direction]][y + addy[direction]] != otherColor && i < 6)
	{
		if (Panel[x + addx[direction]][y + addy[direction]] == (1 - otherColor))
		{
			chess++;
			if (s.successchess >= 0 && nf == 1)
				s.successchess = chess;
			x = x + addx[direction];
			y = y + addy[direction];
			lastx = x;
			lasty = y;
			continue;
		}
		if (Panel[x + addx[direction]][y + addy[direction]] == 2)
		{
			empty++;
			x = x + addx[direction];
			y = y + addy[direction];
			nf = 0;

		}
		i++;
	}


	s.chess = chess;
	s.enpty = empty;
	if (addx[direction] != 0 && lastx != -1)
		s.otherchessd = (x - lastx) / addx[direction] - 1;
	if (addy[direction] != 0 && lasty != -1)
		s.otherchessd = (y - lasty) / addy[direction] - 1;
	return s;
}
int  judgeValueGreedy(int Panel[19][19], int x, int y, int myColor)
{
	int addx[8] = { -1,1,0,0,-1,1,-1,1 };
	int addy[8] = { 0,0,-1,1,-1,-1,1,1 };
	int com[4] = { 0,0,0,0 };
	int vup = 0;
	int vleft = 0;
	int vlu = 0;
	int vru = 0;
	if (Panel[x][y] != 2)
		return -1;
	struct pane aup = chessNumber(Panel, 0, x, y, 1 - myColor);
	struct pane adown = chessNumber(Panel, 1, x, y, 1 - myColor);
	if ((aup.chess + adown.chess + aup.enpty + adown.enpty + 1) >= 6)
	{
		vup = (aup.chess + adown.chess) * 20;
		if (aup.successchess != -1)
			vup += aup.successchess;
		if (adown.successchess != -1)
			vup += adown.successchess;
		if (aup.otherchessd > 0)
			vup++;
		if (adown.otherchessd > 0)
			vup++;
		if ((Panel[x + addx[0]][y + addy[0]] == myColor || Panel[x + addx[1]][y + addy[1]] == myColor) && (x + addx[0] < 19 && x + addx[0] >= 0 && y + addy[0] < 19 && y + addy[0] >= 0 && x + addx[1] < 19 && x + addx[1] >= 0 && y + addy[1] < 19 && y + addy[1] >= 0))
			vup++;
	}
	struct pane aleft = chessNumber(Panel, 2, x, y, 1 - myColor);
	struct pane aright = chessNumber(Panel, 3, x, y, 1 - myColor);
	if ((aleft.chess + aright.chess + aleft.enpty + aright.enpty + 1) >= 6)
	{
		if (aleft.successchess != -1)
			vleft += aleft.successchess;
		if (aright.successchess != -1)
			vleft += aright.successchess;
		vleft = (aleft.chess + aright.chess) * 20;
		if (aleft.otherchessd > 0)
			vleft++;
		if (aright.otherchessd > 0)
			vleft++;
		/*	if (aleft.otherchessd + aright.otherchessd == 0 && vleft < 60)
				vleft -= 10;*/
		if ((Panel[x + addx[2]][y + addy[2]] == myColor || Panel[x + addx[3]][y + addy[3]] == myColor) && (x + addx[2] < 19 && x + addx[2] >= 0 && y + addy[2] < 19 && y + addy[2] >= 0 && x + addx[3] < 19 && x + addx[3] >= 0 && y + addy[3] < 19 && y + addy[3] >= 0))
			vleft++;
	}
	struct pane aleftup = chessNumber(Panel, 4, x, y, 1 - myColor);
	struct pane arightdown = chessNumber(Panel, 7, x, y, 1 - myColor);
	if ((aleftup.chess + arightdown.chess + aleftup.enpty + arightdown.enpty + 1) >= 6)
	{
		if (aleftup.otherchessd > 0)
			vlu++;
		if (arightdown.otherchessd > 0)
			vlu++;
		if (aleftup.successchess != -1)
			vlu += aleftup.successchess;
		if (arightdown.successchess != -1)
			vlu += arightdown.successchess;
		vlu = (aleftup.chess + arightdown.chess) * 20;
		if (aleftup.otherchessd + arightdown.otherchessd)
			if ((Panel[x + addx[4]][y + addy[4]] == myColor || Panel[x + addx[7]][y + addy[7]] == myColor) && (x + addx[4] < 19 && x + addx[4] >= 0 && y + addy[4] < 19 && y + addy[4] >= 0 && x + addx[7] < 19 && x + addx[7] >= 0 && y + addy[7] < 19 && y + addy[7] >= 0))
				vlu++;
	}
	struct pane aleftdown = chessNumber(Panel, 5, x, y, 1 - myColor);
	struct pane arightup = chessNumber(Panel, 6, x, y, 1 - myColor);
	if ((aleftdown.chess + arightup.chess + aleftdown.enpty + arightup.enpty + 1) >= 6)
	{
		if (aleftdown.otherchessd > 0)
			vru++;
		if (arightup.otherchessd > 0)
			vru++;
		if (aleftdown.successchess != -1)
			vru += aleftdown.successchess;
		if (arightup.successchess != -1)
			vru += arightup.successchess;
		vru = (aleftdown.chess + arightup.chess) * 20;
		if ((Panel[x + addx[5]][y + addy[5]] == myColor || Panel[x + addx[6]][y + addy[6]] == myColor) && (x + addx[5] < 19 && x + addx[5] >= 0 && y + addy[5] < 19 && y + addy[5] >= 0 && x + addx[6] < 19 && x + addx[6] >= 0 && y + addy[6] < 19 && y + addy[6] >= 0))
			vru++;
	}
	com[0] = vup;
	com[1] = vleft;
	com[2] = vlu;
	com[3] = vru;
	int maxn = 0;
	int max = -1;
	for (int i = 0; i < 4; i++)
	{
		if (com[i] >= max)
		{
			max = com[i];
			maxn = i;
		}
	}
	//cout << com[maxn] << " ";
	return com[maxn];
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////历史表部分///////////////////////

struct historylist
{
	int ff; //是否为空
	struct Point step;
};
struct Node
{
	struct Point step;
	int value;
};
struct Node2
{
	struct Step step;
	int value;
};
struct treepoint
{
	int Panel[19][19];
	int tier = 0;
	int weight = 0;
	int list = 0;//此节点为父节点的第几个儿子
	struct treepoint* child;
	struct treepoint* father;
	struct Node2 childss[3001];
	struct Step nextstep;

};
historylist hl[20];
historylist hl2[20];
void initiate()
{
	for (int i = 0; i < historylistSize; i++)
	{
		hl[i].ff = 0;
		hl2[i].ff = 0;
		hl[i].step.x = 0;
		hl[i].step.y = 0;
		hl2[i].step.x = 0;
		hl2[i].step.y = 0;
	}
}
//对历史表周围结点进行入树判断
void nodeFilter1(treepoint* t, int type, int computerSide)
{
	/*
	if (t->tier == maxDepth + 1)
	{
		t->childss[1].step.first.x = -1;
		return;
	}
	*/
	Node2 childs[3000];
	for (int i = 0; i < 3000; i++)
	{
		childs[i].step.first.x = -1;
		childs[i].step.first.y = -1;
		childs[i].step.second.x = -1;
		childs[i].step.second.y = -1;
		childs[i].value = 0;
	}
	Node node[200];
	for (int i = 0; i < 200; i++)
	{
		node[i].step.x = -1;
		node[i].step.y = -1;
		node[i].value = 0;
	}
	int count = 0;
	int boards[19][19] = { NULL };
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
		{
			boards[i][j] = t->Panel[i][j];
		}
	if (t->tier != 0)
	{
		if (t->nextstep.first.x >= 0 && t->nextstep.first.y >= 0 && t->nextstep.first.x < 19 && t->nextstep.first.y < 19)
		{
			boards[t->nextstep.first.x][t->nextstep.first.y] = 1 - type;
		}
		if (t->nextstep.second.x >= 0 && t->nextstep.second.y >= 0 && t->nextstep.second.x < 19 && t->nextstep.second.y < 19)
		{
			boards[t->nextstep.second.x][t->nextstep.second.y] = 1 - type;
		}
	}
	if (t->tier == 0)
		if (t->nextstep.first.x >= 0 && t->nextstep.first.y >= 0 && t->nextstep.first.x < 19 && t->nextstep.first.y < 19)
		{
			boards[t->nextstep.first.x][t->nextstep.first.y] = computerSide;
		}
	int value[19][19] = { NULL };
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
		{
			value[i][j] = max(judgeValueGreedy(boards, i, j, computerSide), judgeValueGreedy(boards, i, j, 1 - computerSide));
		}
	int k = 0;
	int valueFilter = 0;
	while (hl[k].ff == 1 && k < historylistSize)
		k++;
	if (k > 4)
		valueFilter = 40;
	else
		valueFilter = 0;
	if (t->tier == 0 && t->nextstep.first.x >= 0 && t->nextstep.first.x < 19 && t->nextstep.first.y >= 0 && t->nextstep.first.y < 19 && t->nextstep.second.x < 0 && t->nextstep.second.y < 0)
	{
		//遍历历史表周围结点
		for (int i = 0; i < k; i++)
		{
			if (hl[i].ff != 0)
			{
				for (int x = hl[i].step.x - 1; x <= hl[i].step.x + 1; x++)
					for (int y = hl[i].step.y - 1; y <= hl[i].step.y + 1; y++)
					{
						if (x >= 0 && y >= 0 && x < 19 && y < 19 && Board[x][y] == 2 && boards[x][y] == 2)
						{
							node[count].step.x = x;
							node[count].step.y = y;
							value[x][y] = -1;
							count++;
						}
					}
			}
		}
		int c = 0;
		for (int i = 0; i < count; i++)
		{
			childs[c].step.first = t->nextstep.first;
			childs[c].step.second = node[i].step;
			c++;
		}
		for (int x = t->nextstep.first.x - 1; x <= t->nextstep.first.x + 1; x++)
			for (int y = t->nextstep.first.y - 1; y <= t->nextstep.first.y + 1; y++)
			{
				if (x >= 0 && y >= 0 && x < 19 && y < 19 && Board[x][y] == 2 && boards[x][y] == 2 && value[x][y]>0)
				{
					childs[c].step.first = t->nextstep.first;
					childs[c].step.second.x = x;
					childs[c].step.second.y = y;
					c++;
					if (c > 2999)
						break;
				}
			}
		for (int i = 0; i < c; i++)
		{
			t->childss[i + 1].step.first.x = t->nextstep.first.x;
			t->childss[i + 1].step.first.y = t->nextstep.first.y;
			t->childss[i + 1].step.second.x = childs[i].step.second.x;
			t->childss[i + 1].step.second.y = childs[i].step.second.y;
		}
	}
	else
	{
		//遍历历史表周围结点
		for (int i = 0; i < k; i++)
		{
			if (hl[i].ff != 0)
			{
				for (int x = hl[i].step.x - 1; x <= hl[i].step.x + 1; x++)
					for (int y = hl[i].step.y - 1; y <= hl[i].step.y + 1; y++)
					{
						if (x >= 0 && y >= 0 && x < 19 && y < 19 && Board[x][y] == 2 && boards[x][y] == 2 && value[x][y] >= valueFilter)
						{
							node[count].step.x = x;
							node[count].step.y = y;
							value[x][y] = -1;
							count++;
						}
					}
			}
		}
		if (count == 0)
		{
			for (int i = 0; i < historylistSize; i++)
			{
				if (hl2[i].ff != 0)
				{
					for (int x = hl2[i].step.x - 1; x <= hl2[i].step.x + 1; x++)
						for (int y = hl2[i].step.y - 1; y <= hl2[i].step.y + 1; y++)
						{
							if (x >= 0 && y >= 0 && x < 19 && y < 19 && Board[x][y] == 2 && boards[x][y] == 2 && value[x][y] >= 20)
							{
								node[count].step.x = x;
								node[count].step.y = y;
								value[x][y] = -1;
								count++;
							}
						}
				}
			}
		}
		int c = 0;
		//对两步进行组合
		for (int i = 0; i < count - 1; i++)
			for (int j = i + 1; j < count; j++)
			{
				if (t->tier != 0)
				{
					int temp1 = boards[node[i].step.x][node[i].step.y];
					int temp2 = boards[node[j].step.x][node[j].step.y];
					boards[node[i].step.x][node[i].step.y] = type;
					boards[node[j].step.x][node[j].step.y] = type;
					childs[c].value = Grade21(boards, computerSide);
					boards[node[i].step.x][node[i].step.y] = temp1;
					boards[node[j].step.x][node[j].step.y] = temp2;
				}
				childs[c].step.first = node[i].step;
				childs[c].step.second = node[j].step;
				c++;
				if (c > 2999)
					break;
			}

		if (type == 1 - computerSide)
		{
			for (int i = 0; i < count; i++)
			{
				int temp1 = boards[node[i].step.x][node[i].step.y];
				boards[node[i].step.x][node[i].step.y] = type;
				for (int x = node[i].step.x - 1; x <= node[i].step.x + 1; x++)
				{
					for (int y = node[i].step.y - 1; y <= node[i].step.y + 1; y++)
					{
						if ((x >= 0 && y >= 0 && x < 19 && y < 19) && Board[x][y] == 2 && boards[x][y] == 2 && value[x][y] >= 40)
						{
							int temp2 = boards[x][y];
							boards[x][y] = type;
							childs[c].step.first = node[i].step;
							childs[c].step.second.x = x;
							childs[c].step.second.y = y;
							childs[c].value = Grade21(boards, computerSide);
							c++;
							boards[x][y] = temp2;
							if (c > 2999)
							{
								boards[node[i].step.x][node[i].step.y] = temp1;
								break;
							}
						}
					}
				}
				boards[node[i].step.x][node[i].step.y] = temp1;
			}
		}

		//插入排序 己方降序排 敌方升序排
		if (t->tier % 2 != 0)
		{
			if (type == computerSide)
			{
				for (int i = 1; i < c; i++)
				{
					Node2 tmp = childs[i];
					int j = i - 1;
					while (j >= 0 && childs[j].value < tmp.value)
					{
						childs[j + 1] = childs[j];
						j--;
					}
					childs[j + 1] = tmp;
				}
			}
			else
			{
				for (int i = 1; i < c; i++)
				{
					Node2 tmp = childs[i];
					int j = i - 1;
					while (j >= 0 && childs[j].value > tmp.value)
					{
						childs[j + 1] = childs[j];
						j--;
					}
					childs[j + 1] = tmp;
				}
			}
		}

		//控制节点数 随时间调整
		int cc = 0;
		if (t->tier % 2 == 0)
		{
			cc = c;
		}
		else
		{
			if (c < 10)
				cc = c;
			else
				cc = 10;
		}
		for (int i = 0; i < cc; i++)
		{
			t->childss[i + 1].step.first.x = childs[i].step.first.x;
			t->childss[i + 1].step.first.y = childs[i].step.first.y;
			t->childss[i + 1].step.second.x = childs[i].step.second.x;
			t->childss[i + 1].step.second.y = childs[i].step.second.y;
			t->childss[i + 1].value = childs[i].value;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////历史表部分///////////////////////
struct treepoint* Creat(int Boards[19][19], int tier, struct treepoint* father, int m, int n, int myColor, int nowColor, int sm, int sn, int list, int weights)
{

	struct treepoint* point = new treepoint;
	point->nextstep.first.x = -1;
	point->nextstep.first.y = -1;
	point->nextstep.second.x = -1;
	point->nextstep.second.y = -1;
	point->child = NULL;
	point->tier = 0;
	point->list = 0;
	point->father = NULL;
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
		{
			point->Panel[i][j] = 0;
		}
	for (int i = 0; i < 3001; i++)
	{
		point->childss[i].step.first.x = -1;
		point->childss[i].step.first.y = -1;
		point->childss[i].step.second.x = -1;
		point->childss[i].step.second.y = -1;
		point->childss[i].value = 0;
	}
	if (father != NULL)//最初在主函数调用Creat时候传进来的指针舍弃不用，以新建的节点为根节点
	{
		point->father = father;
	}
	else
	{
		point->father = NULL;
	}
	for (int a = 0; a < 19; a++)
		for (int b = 0; b < 19; b++)
			point->Panel[a][b] = Boards[a][b];//将棋盘拷贝
	point->tier = tier;
	point->list = list;
	if (m != 2000 && n != 2000 && m >= 0 && n >= 0 && m < 19 && n < 19)
	{
		point->Panel[m][n] = nowColor;//由上一步传进来此次下棋位置
		point->nextstep.first.x = m;
		point->nextstep.first.y = n;
	}
	if (sm != 2000 && sn != 2000)
	{
		if (sm >= 0 && sn >= 0 && sm < 19 && sn < 19)
		{
			point->Panel[sm][sn] = nowColor;//由上一步传进来此次下棋位置
		}
		point->nextstep.second.x = sm;
		point->nextstep.second.y = sn;
	}

	nodeFilter1(point, 1 - nowColor, myColor);
	if (tier % 2 != 0)
	{
		point->weight = 999999;
	}
	if (tier % 2 == 0)
	{
		point->weight = -9999999;
	}


	if (tier == maxDepth)
	{
		point->weight = weights;

	}
	int i = 0;
	int j = 0;
	int nm = 1;
	while (point->childss[nm].step.first.x >= 0 && point->childss[nm].step.first.y >= 0 && point->childss[nm].step.second.x >= 0 && point->childss[nm].step.second.y >= 0 && point->childss[nm].step.first.x < 19 && point->childss[nm].step.first.y < 19 && point->childss[nm].step.second.x < 19 && point->childss[nm].step.second.y < 19)
	{
		i = point->childss[nm].step.first.x;
		j = point->childss[nm].step.first.y;
		int a = 0;
		int b = 0;

		a = point->childss[nm].step.second.x;
		b = point->childss[nm].step.second.y;
		int ss = nowColor;
		int x = point->tier;
		ss = 1 - nowColor;


		if (x != maxDepth)
		{
			point->child = Creat(point->Panel, x + 1, point, i, j, myColor, ss, a, b, nm, point->childss[nm].value);
			nm++;
		}
		else
		{
			point->child = NULL;
			goto end2;
		}
		if (point->child != NULL)
		{
			if (x == 1 || x == 3 || x == 5)
			{
				if (point->child->weight < point->weight)
				{
					point->weight = point->child->weight;

				}

				if (point->child->list != 1)
				{
					if (point->weight < point->father->weight)
					{
						delete point->child;
						point->child = NULL;
						delete point;
						point = NULL;
						goto end;
					}
				}
				delete point->child;
				point->child = NULL;

			}
			else if (x == 0 || x == 2 || x == 4)
			{
				if (point->child->weight > point->weight)
				{
					point->weight = point->child->weight;
					if (x == 0)
					{
						point->nextstep.first.x = point->child->nextstep.first.x;
						point->nextstep.first.y = point->child->nextstep.first.y;
						point->nextstep.second.x = point->child->nextstep.second.x;
						point->nextstep.second.y = point->child->nextstep.second.y;
					}
				}

				if (point->child->list != 1 && point->tier != 0)
				{
					if (point->weight > point->father->weight)
					{
						delete point->child;
						point->child = NULL;
						delete point;
						point = NULL;
						goto end;
					}
				}
				delete point->child;
				point->child = NULL;
			}
		}
	}
	/*if (point->tier != 0)
	{
		point->father->childs[list] = point;
		point->father->childs[list + 1] = NULL;
	}*/
	return point;
end:
	return NULL;
end2:
	return point;
}
int main()
{

	Step step;//临时步结构
	char message[256];//通信消息缓冲
	int computerSide;//己方执棋颜色
	int start = 0;//对局开始标记
	srand(int(time(0)));
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			Board[i][j] = 2;
	while (1)	//程序主循环
	{
		fflush(stdout);//不要删除此语句，否则程序会出问题
		scanf("%s", message);//获取平台命令消息
		//分析命令
		if (strcmp(message, "name?") == 0)//向对战平台发送队名
		{
			fflush(stdin);
			/***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
			/*******/		printf("name AlphaGo Zero\n");		/**只修改令狐冲，不要删除name空格****/
			/***********将"令狐冲"改为你的队名，不超过6个汉字或12个英文字母，否则无成绩************/
		}
		else if (strcmp(message, "new") == 0)//建立新棋局
		{
			for (int i = 0; i < historylistSize; i++)
			{
				hl[i].ff = 0;
				hl2[i].ff = 0;
			}
			int i, j;
			scanf("%s", message);//获取己方执棋颜色
			fflush(stdin);
			if (strcmp(message, "black") == 0)	computerSide = BLACK;  //执黑
			else  computerSide = WHITE;   //执白

			for (i = 0; i < 19; ++i)   //初始化棋局
				for (j = 0; j < 19; ++j)
					Board[i][j] = EMPTY;
			start = 1;

			if (computerSide == BLACK)
			{
				/**********生成第一手着法，并保存在step结构中，落子坐标为(step.first.x,step.first.y)**********/
				/****************************在下方填充代码，并替换我的示例代码******************************/


				step.first.x = 9;
				step.first.y = 9;
				/******************************在上面填充第一步行棋代码*******************************************/

				Board[step.first.x][step.first.y] = computerSide;//处理己方行棋
				printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//输出着法
			}
		}
		else if (strcmp(message, "move") == 0)//行棋,本程序核心
		{
			scanf("%s", message);//获取对手行棋着法
			fflush(stdin);
			step.first.y = message[0] - 'A';		step.first.x = message[1] - 'A';
			step.second.y = message[2] - 'A';		step.second.x = message[3] - 'A';
			//处理对手行棋
			Board[step.first.x][step.first.y] = 1 - computerSide;
			if (!(step.second.x == -1 && step.second.y == -1)) Board[step.second.x][step.second.y] = 1 - computerSide;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////记录敌方历史行棋部分///////////////////////
			int flag = 0;
			int copy[21][21] = { 0 };
			for (int i = 0; i < 19; i++)
				for (int j = 0; j < 19; j++)
				{
					copy[i + 1][j + 1] = Board[i][j];
				}
			for (int i = 0; i < 21; i++)
			{
				copy[0][i] = -1;
				copy[20][i] = -1;
				copy[i][0] = -1;
				copy[i][20] = -1;
			}
			//更新历史表 周围没地走的就删
			for (int k = 0; k < historylistSize; k++)
			{
				if (hl[k].ff != 0)
				{
					flag = 0;
					for (int i = hl[k].step.x - 1; i <= hl[k].step.x + 1; i++)
						for (int j = hl[k].step.y - 1; j <= hl[k].step.y + 1; j++)
							if (copy[i + 1][j + 1] != 2)
							{
								flag++;
							}
					if (flag == 8)
					{
						hl[k].ff = 0;
					}
				}
				if (hl2[k].ff != 0)
				{
					flag = 0;
					for (int i = hl2[k].step.x - 1; i <= hl2[k].step.x + 1; i++)
						for (int j = hl2[k].step.y - 1; j <= hl2[k].step.y + 1; j++)
							if (copy[i + 1][j + 1] != 2)
							{
								flag++;
							}
					if (flag == 8)
					{
						hl2[k].ff = 0;
					}
				}
			}
			//历史表空的往后放
			for (int k = 0; k < historylistSize; k++)
			{
				if (hl[k].ff == 0)
				{
					for (int i = k; i < historylistSize - 1; i++)
					{
						hl[i] = hl[i + 1];
					}
				}
			}
			for (int k = 0; k < historylistSize; k++)
			{
				if (hl2[k].ff == 0)
				{
					for (int i = k; i < historylistSize - 1; i++)
					{
						hl2[i] = hl2[i + 1];
					}
				}
			}
			int k = 0; //历史表当前排到几号
			while (hl[k].ff == 1 && k < historylistSize)
				k++;
			flag = 0;
			for (int i = step.first.x - 1; i <= step.first.x + 1; i++)
				for (int j = step.first.y - 1; j <= step.first.y + 1; j++)
					if (copy[i + 1][j + 1] == 2)
					{
						flag++;
						if (flag >= 1)
							break;
					}
			if (flag >= 1)
			{
				if (k < historylistSize)
				{
					hl[k].step.x = step.first.x;
					hl[k].step.y = step.first.y;
					hl[k].ff = 1;
					k++;
				}
				else
				{
					int index = (k + 1) % historylistSize;
					hl[index].step.x = step.first.x;
					hl[index].step.y = step.first.y;
					hl[index].ff = 1;
					k++;
				}
			}
			flag = 0;
			if (step.second.x != -1 && step.second.y != -1)
				for (int i = step.second.x - 1; i <= step.second.x + 1; i++)
					for (int j = step.second.y - 1; j <= step.second.y + 1; j++)
						if (copy[i + 1][j + 1] == 2)
						{
							flag++;
							if (flag >= 1)
								break;
						}

			if (flag >= 1)
			{
				if (k < historylistSize)
				{
					hl[k].step.x = step.second.x;
					hl[k].step.y = step.second.y;
					hl[k].ff = 1;
					k++;
				}
				else
				{
					int index = (k + 1) % historylistSize;
					if (!(step.second.x == -1 && step.second.y == -1))
					{
						hl[index].step.x = step.second.x;
						hl[index].step.y = step.second.y;
						hl[index].ff = 1;
						k++;
					}
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////记录敌方历史行棋部分///////////////////////

			/**********************************************************************************************************/
			/***生成落子的坐标，保存在step结构中，第1子下在(step.first.x,step.first.y)，第2子下在(step.first.x,step.first.y)*****/
			/**************************************在下方填充代码，并替换我的示例代码*****************************************/

			if (computerSide == WHITE)
			{
				int edge = 0;
				if (hl[0].step.x < 6 || hl[0].step.x >14 || hl[0].step.y < 6 || hl[0].step.y >14 && k == 1)
					edge = 1;
				if (k == 1 && edge == 1 && Board[9][9] == 2 && Board[9][10] == 2)          //第一步不被对面骗去边上
				{
					step.first.y = 9;
					step.first.x = 9;
					step.second.y = 10;
					step.second.x = 9;
				}
				else
				{
					if (k > 13)
					{
						int BoardR[19][19] = { 0 };
						for (int i = 0; i < 19; i++)
							for (int j = 0; j < 19; j++)
							{
								BoardR[j][i] = Board[i][j];
							}
						step = Hit(BoardR, computerSide);
						int valueM = -10000;
						Point s = { -1 , -1 };
						if (step.first.x == -1)
						{
							for (int i = 0; i < 19; i++)
							{
								for (int j = 0; j < 19; j++)
								{
									if (BoardR[i][j] == EMPTY)
									{
										int temp = Grade22(BoardR, i, j, computerSide);
										if (temp > valueM)
										{
											valueM = temp;
											s.x = i; s.y = j;
										}
										else if (temp == valueM && check(BoardR, i, j))
										{
											valueM = temp;
											s.x = i; s.y = j;
										}
									}
								}
							}
							step.first.x = s.x; step.first.y = s.y;
						}

						Board[step.first.y][step.first.x] = computerSide;
						BoardR[step.first.x][step.first.y] = computerSide;
						valueM = -10000;
						s = { -1 , -1 };
						if (step.second.x == -1)
						{
							for (int i = 0; i < 19; i++)
							{
								for (int j = 0; j < 19; j++)
								{
									if (BoardR[i][j] == EMPTY)
									{
										int temp = Grade22(BoardR, i, j, computerSide);
										if (temp > valueM)
										{
											valueM = temp;
											s.x = i; s.y = j;
										}
										else if (temp == valueM && check(BoardR, i, j))
										{
											valueM = temp;
											s.x = i; s.y = j;
										}
									}
								}
							}
							step.second.x = s.x; step.second.y = s.y;
						}

						Board[step.second.y][step.second.x] = computerSide;
					}
					else
					{
						Step immediate;
						immediate = Hit(Board, computerSide);
						if (immediate.first.x != -1 && immediate.first.y != -1 && immediate.second.x != -1 && immediate.second.y != -1)
						{
							step.first.y = immediate.first.x;
							step.first.x = immediate.first.y;
							step.second.y = immediate.second.x;
							step.second.x = immediate.second.y;
							Board[immediate.first.x][immediate.first.y] = computerSide;
							Board[immediate.second.x][immediate.second.y] = computerSide;
						}
						else if (immediate.first.x != -1 && immediate.first.y != -1 && immediate.second.x == -1 && immediate.second.y == -1)
						{
							struct treepoint* rootf = NULL;
							struct treepoint* root = NULL;
							root = Creat(Board, 0, rootf, immediate.first.x, immediate.first.y, computerSide, 1 - computerSide, -1, -1, 0, 7777);
							step.first.y = root->nextstep.first.x;
							step.first.x = root->nextstep.first.y;
							step.second.y = root->nextstep.second.x;
							step.second.x = root->nextstep.second.y;
							Board[root->nextstep.first.x][root->nextstep.first.y] = computerSide;
							Board[root->nextstep.second.x][root->nextstep.second.y] = computerSide;
						}
						else
						{
							struct treepoint* rootf = NULL;
							struct treepoint* root = NULL;
							root = Creat(Board, 0, rootf, 2000, 2000, computerSide, 1 - computerSide, 2000, 2000, 0, 7777);
							if (root->nextstep.first.x < 0 || root->nextstep.first.y < 0 || root->nextstep.second.x < 0 || root->nextstep.second.y < 0 || root->nextstep.first.x >= 19 || root->nextstep.first.y >= 19 || root->nextstep.second.x >= 19 || root->nextstep.second.y >= 19)
							{
								int BoardR[19][19] = { 0 };
								for (int i = 0; i < 19; i++)
									for (int j = 0; j < 19; j++)
									{
										BoardR[j][i] = Board[i][j];
									}
								step = Hit(BoardR, computerSide);
								int valueM = -10000;
								Point s = { -1 , -1 };
								if (step.first.x == -1)
								{
									for (int i = 0; i < 19; i++)
									{
										for (int j = 0; j < 19; j++)
										{
											if (BoardR[i][j] == EMPTY)
											{
												int temp = Grade22(BoardR, i, j, computerSide);
												if (temp > valueM)
												{
													valueM = temp;
													s.x = i; s.y = j;
												}
												else if (temp == valueM && check(BoardR, i, j))
												{
													valueM = temp;
													s.x = i; s.y = j;
												}
											}
										}
									}
									step.first.x = s.x; step.first.y = s.y;
								}

								Board[step.first.y][step.first.x] = computerSide;
								BoardR[step.first.x][step.first.y] = computerSide;
								valueM = -10000;
								s = { -1 , -1 };
								if (step.second.x == -1)
								{
									for (int i = 0; i < 19; i++)
									{
										for (int j = 0; j < 19; j++)
										{
											if (BoardR[i][j] == EMPTY)
											{
												int temp = Grade22(BoardR, i, j, computerSide);
												if (temp > valueM)
												{
													valueM = temp;
													s.x = i; s.y = j;
												}
												else if (temp == valueM && check(BoardR, i, j))
												{
													valueM = temp;
													s.x = i; s.y = j;
												}
											}
										}
									}
									step.second.x = s.x; step.second.y = s.y;
								}

								Board[step.second.y][step.second.x] = computerSide;
							}
							else
							{
								step.first.y = root->nextstep.first.x;
								step.first.x = root->nextstep.first.y;
								step.second.y = root->nextstep.second.x;
								step.second.x = root->nextstep.second.y;
								Board[root->nextstep.first.x][root->nextstep.first.y] = computerSide;
								Board[root->nextstep.second.x][root->nextstep.second.y] = computerSide;
							}
						}
					}
				}
			}
			else
			{
				if (k > 12)
				{
					int BoardR[19][19] = { 0 };
					for (int i = 0; i < 19; i++)
						for (int j = 0; j < 19; j++)
						{
							BoardR[j][i] = Board[i][j];
						}
					step = Hit(BoardR, computerSide);
					int valueM = -10000;
					Point s = { -1 , -1 };
					if (step.first.x == -1)
					{
						for (int i = 0; i < 19; i++)
						{
							for (int j = 0; j < 19; j++)
							{
								if (BoardR[i][j] == EMPTY)
								{
									int temp = Grade22(BoardR, i, j, computerSide);
									if (temp > valueM)
									{
										valueM = temp;
										s.x = i; s.y = j;
									}
									else if (temp == valueM && check(BoardR, i, j))
									{
										valueM = temp;
										s.x = i; s.y = j;
									}
								}
							}
						}
						step.first.x = s.x; step.first.y = s.y;
					}

					Board[step.first.y][step.first.x] = computerSide;
					BoardR[step.first.x][step.first.y] = computerSide;
					valueM = -10000;
					s = { -1 , -1 };
					if (step.second.x == -1)
					{
						for (int i = 0; i < 19; i++)
						{
							for (int j = 0; j < 19; j++)
							{
								if (BoardR[i][j] == EMPTY)
								{
									int temp = Grade22(BoardR, i, j, computerSide);
									if (temp > valueM)
									{
										valueM = temp;
										s.x = i; s.y = j;
									}
									else if (temp == valueM && check(BoardR, i, j))
									{
										valueM = temp;
										s.x = i; s.y = j;
									}
								}
							}
						}
						step.second.x = s.x; step.second.y = s.y;
					}

					Board[step.second.y][step.second.x] = computerSide;
				}
				else
				{
					Step immediate;
					immediate = Hit(Board, computerSide);
					if (immediate.first.x != -1 && immediate.first.y != -1 && immediate.second.x != -1 && immediate.second.y != -1)
					{
						step.first.y = immediate.first.x;
						step.first.x = immediate.first.y;
						step.second.y = immediate.second.x;
						step.second.x = immediate.second.y;
						Board[immediate.first.x][immediate.first.y] = computerSide;
						Board[immediate.second.x][immediate.second.y] = computerSide;
					}
					else if (immediate.first.x != -1 && immediate.first.y != -1 && immediate.second.x == -1 && immediate.second.y == -1)
					{
						struct treepoint* rootf = NULL;
						struct treepoint* root = NULL;
						root = Creat(Board, 0, rootf, immediate.first.x, immediate.first.y, computerSide, 1 - computerSide, -1, -1, 0, 7777);
						step.first.y = root->nextstep.first.x;
						step.first.x = root->nextstep.first.y;
						step.second.y = root->nextstep.second.x;
						step.second.x = root->nextstep.second.y;
						Board[root->nextstep.first.x][root->nextstep.first.y] = computerSide;
						Board[root->nextstep.second.x][root->nextstep.second.y] = computerSide;
					}
					else
					{
						struct treepoint* rootf = NULL;
						struct treepoint* root = NULL;
						root = Creat(Board, 0, rootf, 2000, 2000, computerSide, 1 - computerSide, 2000, 2000, 0, 7777);
						if (root->nextstep.first.x < 0 || root->nextstep.first.y < 0 || root->nextstep.second.x < 0 || root->nextstep.second.y < 0 || root->nextstep.first.x >= 19 || root->nextstep.first.y >= 19 || root->nextstep.second.x >= 19 || root->nextstep.second.y >= 19)
						{
							int BoardR[19][19] = { 0 };
							for (int i = 0; i < 19; i++)
								for (int j = 0; j < 19; j++)
								{
									BoardR[j][i] = Board[i][j];
								}
							step = Hit(BoardR, computerSide);
							int valueM = -10000;
							Point s = { -1 , -1 };
							if (step.first.x == -1)
							{
								for (int i = 0; i < 19; i++)
								{
									for (int j = 0; j < 19; j++)
									{
										if (BoardR[i][j] == EMPTY)
										{
											int temp = Grade22(BoardR, i, j, computerSide);
											if (temp > valueM)
											{
												valueM = temp;
												s.x = i; s.y = j;
											}
											else if (temp == valueM && check(BoardR, i, j))
											{
												valueM = temp;
												s.x = i; s.y = j;
											}
										}
									}
								}
								step.first.x = s.x; step.first.y = s.y;
							}

							Board[step.first.y][step.first.x] = computerSide;
							BoardR[step.first.x][step.first.y] = computerSide;
							valueM = -10000;
							s = { -1 , -1 };
							if (step.second.x == -1)
							{
								for (int i = 0; i < 19; i++)
								{
									for (int j = 0; j < 19; j++)
									{
										if (BoardR[i][j] == EMPTY)
										{
											int temp = Grade22(BoardR, i, j, computerSide);
											if (temp > valueM)
											{
												valueM = temp;
												s.x = i; s.y = j;
											}
											else if (temp == valueM && check(BoardR, i, j))
											{
												valueM = temp;
												s.x = i; s.y = j;
											}
										}
									}
								}
								step.second.x = s.x; step.second.y = s.y;
							}

							Board[step.second.y][step.second.x] = computerSide;
						}
						else
						{
							step.first.y = root->nextstep.first.x;
							step.first.x = root->nextstep.first.y;
							step.second.y = root->nextstep.second.x;
							step.second.x = root->nextstep.second.y;
							Board[root->nextstep.first.x][root->nextstep.first.y] = computerSide;
							Board[root->nextstep.second.x][root->nextstep.second.y] = computerSide;
						}
					}
				}
			}
			
		
					
				
				
			

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////记录己方历史行棋部分///////////////////////

			copy[step.second.y + 1][step.second.x + 1] = computerSide;
			copy[step.first.y + 1][step.first.x + 1] = computerSide;
			flag = 0;
			//更新历史表 周围没地走的就删
			for (int k = 0; k < historylistSize; k++)
			{
				if (hl[k].ff != 0)
				{
					flag = 0;
					for (int i = hl[k].step.x - 1; i <= hl[k].step.x + 1; i++)
						for (int j = hl[k].step.y - 1; j <= hl[k].step.y + 1; j++)
							if (copy[i + 1][j + 1] != 2)
							{
								flag++;
							}
					if (flag == 8)
					{
						hl[k].ff = 0;
					}
				}
			}
			for (int k = 0; k < historylistSize; k++)
			{
				if (hl2[k].ff != 0)
				{
					flag = 0;
					for (int i = hl2[k].step.x - 1; i <= hl2[k].step.x + 1; i++)
						for (int j = hl2[k].step.y - 1; j <= hl2[k].step.y + 1; j++)
							if (copy[i + 1][j + 1] != 2)
							{
								flag++;
							}
					if (flag == 8)
					{
						hl2[k].ff = 0;
					}
				}
			}
			//历史表空的往后放
			for (int k = 0; k < historylistSize; k++)
			{
				if (hl[k].ff == 0)
				{
					for (int i = k; i < historylistSize - 1; i++)
					{
						hl[i] = hl[i + 1];
					}
				}
			}
			k = 0; //历史表当前排到几号
			while (hl[k].ff == 1 && k < historylistSize)
				k++;
			if (k == 0)
			{
				flag = 0;
				for (int i = step.first.y - 1; i <= step.first.y + 1; i++)
					for (int j = step.first.x - 1; j <= step.first.x + 1; j++)
						if (copy[i + 1][j + 1] == 2)
						{
							flag++;
							if (flag >= 1)
								break;
						}
				if (flag >= 1)
				{
					if (k < historylistSize)
					{
						hl[k].step.x = step.first.y;
						hl[k].step.y = step.first.x;
						hl[k].ff = 1;
						k++;
					}
					else
					{
						int index = (k + 1) % historylistSize;
						hl[index].step.x = step.first.y;
						hl[index].step.y = step.first.x;
						hl[index].ff = 1;
						k++;
					}
				}
				flag = 0;
				if (step.second.x != -1 && step.second.y != -1)
					for (int i = step.second.x - 1; i <= step.second.x + 1; i++)
						for (int j = step.second.y - 1; j <= step.second.y + 1; j++)
							if (copy[i + 1][j + 1] == 2)
							{
								flag++;
								if (flag >= 1)
									break;
							}
				if (flag == 1)
				{
					if (k < historylistSize)
					{
						hl[k].step.x = step.second.y;
						hl[k].step.y = step.second.x;
						hl[k].ff = 1;
						k++;
					}
					else
					{
						int index = (k + 1) % historylistSize;
						if (!(step.second.x == -1 && step.second.y == -1))
						{
							hl[index].step.x = step.second.y;
							hl[index].step.y = step.second.x;
							hl[index].ff = 1;
							k++;
						}
					}
				}
			}
			k = 0; //历史表当前排到几号
			while (hl2[k].ff == 1 && k < historylistSize)
				k++;
			flag = 0;
			for (int i = step.first.y - 1; i <= step.first.y + 1; i++)
				for (int j = step.first.x - 1; j <= step.first.x + 1; j++)
					if (copy[i + 1][j + 1] == 2)
					{
						flag++;
						if (flag >= 1)
							break;
					}
			if (flag >= 1)
			{
				if (k < historylistSize)
				{
					hl2[k].step.x = step.first.y;
					hl2[k].step.y = step.first.x;
					hl2[k].ff = 1;
					k++;
				}
				else
				{
					int index = (k + 1) % historylistSize;
					hl2[index].step.x = step.first.y;
					hl2[index].step.y = step.first.x;
					hl2[index].ff = 1;
					k++;
				}
			}
			flag = 0;
			if (step.second.x != -1 && step.second.y != -1)
				for (int i = step.second.x - 1; i <= step.second.x + 1; i++)
					for (int j = step.second.y - 1; j <= step.second.y + 1; j++)
						if (copy[i + 1][j + 1] == 2)
						{
							flag++;
							if (flag >= 1)
								break;
						}
			if (flag == 1)
			{
				if (k < historylistSize)
				{
					hl2[k].step.x = step.second.y;
					hl2[k].step.y = step.second.x;
					hl2[k].ff = 1;
					k++;
				}
				else
				{
					int index = (k + 1) % historylistSize;
					if (!(step.second.x == -1 && step.second.y == -1))
					{
						hl2[index].step.x = step.second.y;
						hl2[index].step.y = step.second.x;
						hl2[index].ff = 1;
						k++;
					}
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////记录己方历史行棋部分///////////////////////

			/*****************************************在上面填充代码******************************************************/
			/**********************************************************************************************************/

			printf("move %c%c%c%c\n", step.first.x + 'A', step.first.y + 'A', step.second.x + 'A', step.second.y + 'A');//输出着法
		}
		else if (strcmp(message, "error") == 0)//着法错误
		{
			fflush(stdin);
		}
		else if (strcmp(message, "end") == 0)//对局结束
		{
			fflush(stdin);
			start = 0;
		}
		else if (strcmp(message, "quit") == 0)//退出引擎
		{
			fflush(stdin);
			printf("Quit!\n");
			break;
		}
	}
	return 0;
}

