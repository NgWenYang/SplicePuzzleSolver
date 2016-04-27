#include<stdio.h>
#include<stdlib.h>
#include<math.h>
void Show();
void Display();
void Detach(int a, int B);
void Attach(int a, int B);
void SeekPath(int stepleft);
void Sideswitch(int a, int B);
void DuplicPos(int s, char x);

int maxlength = 4;//precision. if no solution found, maxlength++ << will be applied later 
int maxstep = 2;//total steps allowed

//the first node is starting from array 1 for horizontal
//initial stage, all single node assumed to be placed left side
//0 = empty, 1 = occupied, 2~4 = special, -1/-2 = original or only single side allowed, will be changed to 0 after Attach()
//[maxstep*2][space, 0 is main, 1 is temporary][vertical ---> (maxlength + 1)][horizontal ---> 2^(maxlength + 1) ]
int temppos[6][2][9][513];//to backup and restore in multiple steps mode
int pos[2][9][513] = { { { 0,1 },{ 0,1,1 },{ 0, 1, 0, 1 } } };//input initial stage here

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
			else printf(" ");
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
			Sideswitch(tempa, tempB);
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
			if (pos[0][i][j] == -2 || pos[0][i][j] == -1)
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
	int i, j, k, l;
	DuplicPos(stepleft - 1, 'b');
	for (i = 1;i <= maxlength;i++)
	{
		for (j = 1;j <= (int)pow(2, i);j++)
		{
			if (pos[0][i][j] == 0)
				continue;
			else
			{
				Detach(i, j);
				DuplicPos(stepleft + maxstep - 1, 'b');
				for (k = 1;k <= maxlength;k++)
				{
					for (l = 1;l <= (int)pow(2, k);l++)
					{
						if (pos[0][k][l] != 1 && pos[0][k - 1][(l + 1) / 2] == 1)
						{
							if (l % 2 == 1 && pos[0][k][l] != -1)
							{
								Attach(k, l);
								printf("\nstep%d %d,%d ---> %d,%d:\n", maxstep + 1 - stepleft, i, j, k, l);
								Display();
								if (stepleft > 1)
									SeekPath(stepleft - 1);
							}
							else if (l % 2 == 0 && pos[0][k][l - 1] == 1)
							{
								if (pos[0][k][l] == -1)//case 1 -1 ---> 1  1
								{
									Sideswitch(k, l - 1);
									Attach(k, l - 1);
									printf("\nstep%d %d,%d ---> %d,%d:\n", maxstep + 1 - stepleft, i, j, k, l - 1);
									Display();
									if (stepleft > 1)
										SeekPath(stepleft - 1);
								}
								else if (pos[0][k][l] == -2)//case 1 -2 ---> 1  1
								{
									Attach(k, l);
									printf("\nstep%d %d,%d ---> %d,%d:\n", maxstep + 1 - stepleft, i, j, k, l);
									Display();
									if (stepleft > 1)
										SeekPath(stepleft - 1);
								}
								else if (pos[0][k][l] == 0)//case 1  0 ---> 1  1 ,two ways to attach, left and right
								{
									//attach left
									Sideswitch(k, l - 1);
									Attach(k, l - 1);
									printf("\nstep%d %d,%d ---> %d,%d:\n", maxstep + 1 - stepleft, i, j, k, l - 1);
									Display();
									if (stepleft > 1)
										SeekPath(stepleft - 1);
									DuplicPos(stepleft + maxstep - 1, 'r');

									//attach right
									Attach(k, l);
									printf("\nstep%d %d,%d ---> %d,%d:\n", maxstep + 1 - stepleft, i, j, k, l);
									Display();
									if (stepleft > 1)
										SeekPath(stepleft - 1);
								}
							}
							DuplicPos(stepleft + maxstep - 1, 'r');
						}
					}
				}
				DuplicPos(stepleft - 1, 'r');
			}
		}
	}
}

void Sideswitch(int a, int B)//switch between adjacent nodes
{
	int i, j, b, temp;
	if (B % 2 == 0)
	{
		B -= 1;
	}
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

void DuplicPos(int s, char x)//backup and restore stages in multiple steps
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

void Active()// not implemanted yet
{
	int i = 0;
}


