#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

void Show();
void Display();
void DisplayPrev(int n);
void Detach(int a, int B);
void Attach(int a, int B);
void SeekPath(int stepleft);
void SideSwitch(int a, int B);
void DuplicatePos(int s, char x);
void Split(int a, int B);
void Extend(int a, int b);
void Discard(int a, int B);
void Active(int n);
void SaveStep(int i, int j, int k, int l, int activeCount, int stepleft);
void Compare(int preactiveCount, int postactiveCount, int stepleft, int beforeDetach);
bool Match();


int maxlength = 4;//precision. if no solution found, maxlength++ << will be applied later 
int maxstep = 2;//total steps allowed

//the first node is starting from array 1 for horizontal
//initial stage, all single node assumed to be placed left side
//0=empty 1=occupied 2=split 3=extend 4=discard -1/-2=original or only single side allowed, will be changed to 0 after Attach()
//[(maxstep*3)+1][space, 0 is main, 1 is temporary][vertical ---> (maxlength + 1)][horizontal ---> 2^(maxlength + 1) ]
int temppos[9][2][9][513];//to backup and restore in multiple steps mode
int pos[2][9][513]  = { { { 0,1 },{ 0,1,1},{ 0,2} } };//input initial stage here
int posTarget[2][9][513]= { { { 0,1 },{ 0,1 },{ 0,1,1 },{0, 1,1} } };//input target stage here //can only have 1 & 0

int main()
{
	printf("Initial stage:\n");
	Display();
	SeekPath(maxstep);//print all possible paths
	getchar();
	return 0;
}

void Show()//simple version of printing
{
	int i, j;
	for (i = 0;i <= maxlength;i++)
	{
		for (j = 1;j <= (int)pow(2, i);j++)
			printf("%d ", pos[0][i][j]);
		printf("\n");
	}
}

void Display()//print in a binary tree shpe
{
	int i, j, k, l;
	for (i = 0;i <= maxlength;i++)
	{
		for (k = (int)pow(2, maxlength - i) - 1;k > 0;k--)
		{
			printf(" ");
		}
		for (j = 1;j <= (int)pow(2, i);j++)
		{
			if (pos[0][i][j] == 1)
				printf("0");
			else if (pos[0][i][j] == 2)
				printf("S");
			else if (pos[0][i][j] == 3)
				printf("E");
			else if (pos[0][i][j] == 4)
				printf("X");
			else
				printf(" ");
			for (l = (int)pow(2, maxlength - i + 1) - 1;l > 0;l--)
				printf(" ");
		}
		printf("\n");
	}
}

void DisplayPrev(int n)//print previous steps
{
	int i, j, k, l;
	for (i = 0;i <= maxlength;i++)
	{
		for (k = (int)pow(2, maxlength - i) - 1;k > 0;k--)
		{
			printf(" ");
		}
		for (j = 1;j <= (int)pow(2, i);j++)
		{
			if (temppos[n][0][i][j] == 1)
				printf("0");
			else if (temppos[n][0][i][j] == 2)
				printf("S");
			else if (temppos[n][0][i][j] == 3)
				printf("E");
			else if (temppos[n][0][i][j] == 4)
				printf("X");
			else
				printf(" ");
			for (l = (int)pow(2, maxlength - i + 1) - 1;l > 0;l--)
				printf(" ");
		}
		printf("\n");
	}
}

void Detach(int a, int B)
{
	int i, j, b, tempa, tempB;
	tempa = a;
	tempB = B;

	for (i = 0, b = B;i <= maxlength;i++, a++, B *= 2, b = B)
	{
		for (j = (int)pow(2, i);j >= 1;j--, b--)
		{
			pos[1][i][j] = pos[0][a][b];
			pos[0][a][b] = 0;
		}
	}
	if (tempB % 2 == 1)
	{
		if (pos[0][tempa][tempB + 1] == 1)
		{
			SideSwitch(tempa, tempB);
			pos[0][tempa][tempB + 1] = -2;
		}
		else
		{
			pos[0][tempa][tempB] = -1;
			pos[0][tempa][tempB + 1] = -1;
		}
	}
	else pos[0][tempa][tempB] = -1;
}

void Attach(int a, int B)
{
	int i, j, b;
	for (i = 1;i <= maxlength;i++)
	{
		for (j = 1;j <= (int)pow(2, i);j++)
			if (pos[0][i][j] < 0)
				pos[0][i][j] = 0;
	}
	for (i = 0, b = B;i <= maxlength;i++, a++, B *= 2, b = B)
	{
		for (j = (int)pow(2, i);j >= 1;j--, b--)
		{

			pos[0][a][b] = pos[1][i][j];
			pos[1][i][j] = 0;
		}
	}
}

void SeekPath(int stepleft)
{
	int i = 0, j = 0, k = 0, l = 0, m, n, x, activeCount = 0;
	//int o, p;//more spaces involved
	for (m = 1;m <= maxlength;m++)
	{
		for (n = 1;n <= (int)pow(2, m);n++)
		{
			if (pos[0][m][n] > 1)
			{
				activeCount++;
				break;
			}
		}
	}
	DuplicatePos(stepleft, 'b');
	for (x = 0; x <= activeCount;x++)
	{
		Active(x);
		Compare(x, 0, stepleft, 1);
		if (stepleft > 0)
		{
			DuplicatePos((stepleft + maxstep), 'b');
			for (i = 1;i <= maxlength;i++)
			{
				for (j = 1;j <= (int)pow(2, i);j++)
				{
					if (pos[0][i][j] == 0)
						continue;
					else
					{
						Detach(i, j);
						DuplicatePos(stepleft + maxstep + maxstep, 'b');
						for (k = 1;k <= maxlength;k++)//k = 0 need to examine
						{
							for (l = 1;l <= (int)pow(2, k);l++)
							{
								if (pos[0][k][l] != 1 && pos[0][k - 1][(l + 1) / 2] >= 1)
								{
									if (l % 2 == 1 && pos[0][k][l] == 0)
									{
										Attach(k, l);
										//printf("\nstep%d %d,%d ---> %d,%d:\n", maxstep + 1 - stepleft, i, j, k, l);
										//Display();
										if (stepleft > 1)
										{
											SaveStep(i, j, k, l, x, stepleft);
											Compare(x, 0, stepleft, 0);
											SeekPath(stepleft - 1);
										}
										else
										{
											Active(activeCount - x);
											SaveStep(i, j, k, l, activeCount - x, stepleft);
											Compare(x, activeCount - x, stepleft, 0);
											//printf("1");
										}
									}
									else if (l % 2 == 0 && pos[0][k][l - 1] >= 1 && pos[0][k][l - 1] != 2)
									{
										if (pos[0][k][l] == -1)//case 1 -1 ---> 1  1
										{
											SideSwitch(k, l - 1);
											Attach(k, l - 1);
											//printf("\nstep%d %d,%d ---> %d,%d:\n", maxstep + 1 - stepleft, i, j, k, l - 1);
											//Display();
											if (stepleft > 1)
											{
												SaveStep(i, j, k, l - 1, x, stepleft);
												Compare(x, 0, stepleft, 0);
												SeekPath(stepleft - 1);
											}
											else
											{
												Active(activeCount - x);
												SaveStep(i, j, k, l - 1, x, stepleft);
												Compare(x, activeCount - x, stepleft, 0);
												//printf("2");
											}
										}
										else if (pos[0][k][l] == -2)//case 1 -2 ---> 1  1
										{
											Attach(k, l);
											//printf("\nstep%d %d,%d ---> %d,%d:\n", maxstep + 1 - stepleft, i, j, k, l);
											//Display();
											if (stepleft > 1)
											{
												SaveStep(i, j, k, l, x, stepleft);
												Compare(x, 0, stepleft, 0);
												SeekPath(stepleft - 1);
											}
											else
											{
												Active(activeCount - x);
												SaveStep(i, j, k, l, x, stepleft);
												Compare(x, activeCount - x, stepleft, 0);
												//printf("3");
											}
										}
										else if (pos[0][k][l] == 0)//case 1  0 ---> 1  1 ,two ways to attach, left and right
										{
											//attach left
											SideSwitch(k, l - 1);
											Attach(k, l - 1);
											//printf("\nstep%d %d,%d ---> %d,%d:\n", maxstep + 1 - stepleft, i, j, k, l - 1);
											//Display();
											if (stepleft > 1)
											{
												SaveStep(i, j, k, l - 1, x, stepleft);
												Compare(x, 0, stepleft, 0);
												SeekPath(stepleft - 1);
											}
											else
											{
												Active(activeCount - x);
												SaveStep(i, j, k, l - 1, x, stepleft);
												Compare(x, activeCount - x, stepleft, 0);
												//printf("4");
											}
											DuplicatePos(stepleft + maxstep + maxstep, 'r');

											//attach right
											Attach(k, l);
											//printf("\nstep%d %d,%d ---> %d,%d:\n", maxstep + 1 - stepleft, i, j, k, l);
											//Display();
											if (stepleft > 1)
											{
												SaveStep(i, j, k, l, x, stepleft);
												Compare(x, 0, stepleft, 0);
												SeekPath(stepleft - 1);
											}
											else
											{
												Active(activeCount - x);
												SaveStep(i, j, k, l, x, stepleft);
												Compare(x, activeCount - x, stepleft, 0);
												//printf("5");
											}
										}
									}
									DuplicatePos(stepleft + maxstep + maxstep, 'r');
								}
							}
						}
						DuplicatePos(stepleft + maxstep, 'r');
					}
				}
			}
		}
		DuplicatePos(stepleft, 'r');
	}
}

void SideSwitch(int a, int B)//switch between adjacent nodes
{
	int i, j, b, temp;
	if (B % 2 == 0)
		B -= 1;
	B += 1;

	for (i = 0, b = B;i <= maxlength;i++, a++, B *= 2, b = B)
	{
		for (j = (int)pow(2, i);j >= 1;j--, b--)
		{
			temp = pos[0][a][b];
			pos[0][a][b] = pos[0][a][b - (int)pow(2, i)];
			pos[0][a][b - (int)pow(2, i)] = temp;
		}
	}
}

void DuplicatePos(int s, char x)//backup and restore stages in multiple steps
{
	if (x == 'b')//backup
	{
		int i, j;
		for (i = 0;i <= maxlength;i++)
		{
			for (j = 1;j <= (int)pow(2, i);j++)
			{
				temppos[s][0][i][j] = pos[0][i][j];
				temppos[s][1][i][j] = pos[1][i][j];
			}
		}
	}
	else if (x == 'r')//restore
	{
		int i, j;
		for (i = 0;i <= maxlength;i++)
		{
			for (j = 1;j <= (int)pow(2, i);j++)
			{
				pos[0][i][j] = temppos[s][0][i][j];
				pos[1][i][j] = temppos[s][1][i][j];
			}
		}
	}
}

void Active(int n)
{
	int i, j, flag = 0;
	for (i = 0;i <= maxlength;i++)
	{
		if (n == 0) return;
		for (j = 1;j <= (int)pow(2, i);j++)
		{
			if (pos[0][i][j] > 1) flag = 1;
			if (pos[0][i][j] == 2) Split(i, j);
			if (pos[0][i][j] == 3) Extend(i, j);
			if (pos[0][i][j] == 4) Discard(i, j);
		}
		if (flag == 1)
		{
			n--;
			flag = 0;
		}
	}
}

void Split(int a, int B)
{
	pos[0][a][B] = 1;
	int i, j, b;
	if (B % 2 == 0)
		B -= 1;
	B += 1;

	for (i = 0, b = B;i <= maxlength;i++, a++, B *= 2, b = B)
		for (j = (int)pow(2, i);j >= 1;j--, b--)
			pos[0][a][b] = pos[0][a][b - (int)pow(2, i)];
}

void Extend(int a, int B)
{
	pos[0][a][B] = 1;
	int i, j, b, tempa, tempB;
	tempa = a;
	tempB = B;

	for (i = 0, b = B;i <= maxlength;i++, a++, B *= 2, b = B)
	{
		for (j = (int)pow(2, i);j >= 1;j--, b--)
		{
			pos[1][i][j] = pos[0][a][b];
			pos[0][a][b] = 0;
		}
	}
	pos[0][tempa][tempB] = 1;
	Attach(tempa + 1, (tempB * 2) - 1);
}

void Discard(int a, int B)
{
	int i, j, b;

	for (i = 0, b = B;i <= maxlength;i++, a++, B *= 2, b = B)
		for (j = (int)pow(2, i);j >= 1;j--, b--)
			pos[0][a][b] = 0;
}

void Compare(int preactiveCount, int postactiveCount, int stepleft, int beforeDetach)//compare current to target and print steps if matched
{
	if (Match())
	{
		int n, currentStep = 1;
		for (n = (maxstep * 3);n > (maxstep * 2) + stepleft - 1;n--, currentStep++)
		{
			if (beforeDetach == 1)//function actived before detach
			{
				printf("Active:%d\n", preactiveCount);
				Display();

			}
			else if (n == (maxstep * 2) + stepleft)//last step
			{
				printf("last Step %d: Active:%d (%d,%d) -> (%d,%d) Actice:%d\n", currentStep, preactiveCount, temppos[n][0][0][3], temppos[n][0][0][4], temppos[n][0][0][5], temppos[n][0][0][6], postactiveCount);
				Display();
			}
			else//normal steps
			{
				printf("normal Step %d: Active:%d (%d,%d) -> (%d,%d)\n", currentStep, temppos[n][0][0][7], temppos[n][0][0][3], temppos[n][0][0][4], temppos[n][0][0][5], temppos[n][0][0][6]);
				DisplayPrev(currentStep);
			}
		}
		getchar();
	}
}

void SaveStep(int i, int j, int k, int l, int activeCount, int stepleft)//save steps 
{
	temppos[stepleft + maxstep + maxstep][0][0][3] = i;
	temppos[stepleft + maxstep + maxstep][0][0][4] = j;
	temppos[stepleft + maxstep + maxstep][0][0][5] = k;
	temppos[stepleft + maxstep + maxstep][0][0][6] = l;
	temppos[stepleft + maxstep + maxstep][0][0][7] = activeCount;
}

bool Match()
{
	int i, j;
	for (i = 0;i <= maxlength;i++)
	{
		for (j = 1;j <= (int)pow(2, i);j++)
		{
			if (pos[0][i][j] == posTarget[0][i][j])
				continue;
			else
				return false;
		}
	}
	return true;
}