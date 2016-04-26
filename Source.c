#include<stdio.h>
#include<stdlib.h>
#include<math.h>
void Show();
void Display();
void Detach(int a, int B);
void Attach(int a, int B);
void Possiblepath(int stepleft);
void Sideswitch(int a, int B);
void DuplicPos(int s, char x);

int maxlength = 5;//if no solution found, maxlength++ << will be applied later 
int maxstep = 1;//total steps allowed


int pos[2][8][128] = { { { 0,1 },{ 0,1,1 },{ 0,1,0 } } };//initial stage, all single pos assumed as left side
														 //0 = empty, 1 = occupied, 2~4 = special, -1 = original 
int temppos[2][2][8][128];//[step x 2][space, 0 is main, 1 is temporary][vertical][horizontal]

int main()
{
	/*printf("Initial stage:\n");
	Display();
	Detach(1,2);
	printf("\nDetach node (1,2):\n");
	Display();
	Attach(2, 1);
	printf("\nAttach to position (2,1):\n");
	Display();
	Sideswitch(1, 2);
	printf("\nSwitch node (1,2) with node (1,1) (switching adjacent nodes with the same parent):\n");
	Display();*/
	Possiblepath(1);
	system("pause");
	return 0;
}

void Show()
{
	int i, j;
	for (i = 0;i <= maxlength;i++)
	{
		for (j = 1;j <= (int)pow(2, i);j++)
			printf("%d ", pos[0][i][j]);
		printf("\n");
	}
}

void Display()
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
	int i, j, b, flag = 0, tempa, tempB;
	tempa = a;
	tempB = B;
	if (B % 2 == 1)
	{
		flag = 1;
	}
	for (i = 0, b = B;i <= maxlength;i++, a++, B *= 2, b = B)
	{
		for (j = (int)pow(2, i);j >= 1;j--, b--)
		{
			pos[1][i][j] = pos[0][a][b];
			pos[0][a][b] = 0;
		}
	}
	//pos[0][tempa][tempB] = -1;
	if (flag == 1) Sideswitch(tempa, tempB);
}

void Attach(int a, int B)
{
	int i, j, b;
	for (i = 0, b = B;i <= maxlength;i++, a++, B *= 2, b = B)
	{
		for (j = (int)pow(2, i);j >= 1;j--, b--)
		{
			pos[0][a][b] = pos[1][i][j];
			pos[1][i][j] = 0;
		}
	}
}

void Possiblepath(int stepleft)
{
	int i, j, k, l;
	DuplicPos(0, 'b');
	for (i = 1;i <= maxlength;i++)
	{
		for (j = 1;j <= (int)pow(2, i);j++)
		{
			if (pos[0][i][j] == 0)
				continue;
			else
			{
				Detach(i, j);
				DuplicPos(1, 'b');
				for (k = 1;k <= maxlength;k++)
				{
					for (l = 1;l <= (int)pow(2, i);l++)
					{
						if (pos[0][k][l] == 0 && pos[0][k - 1][(l + 1) / 2] == 1)
						{
							if (l % 2 == 1)
							{
								Attach(k, l);
								printf("%d,%d ---> %d,%d:\n", i, j, k, l);
								Display();
							}
							else if (l % 2 == 0 && pos[0][k][l - 1] == 1)
							{
								Sideswitch(k, l - 1);
								Attach(k, l - 1);
								//if (stepleft > 1)
								//{
								//	Possiblepath(stepleft - 1);
								//}
								//else 
								printf("%d,%d ---> %d,%d:\n", i, j, k, l - 1);
								Display();
								DuplicPos(1, 'r');
								Attach(k, l);
								printf("%d,%d ---> %d,%d:\n", i, j, k, l);
								Display();
							}
							DuplicPos(1, 'r');
						}
						//if (pos[0][i][j] == -1) pos[0][i][j] = 0;
					}
				}
				DuplicPos(0, 'r');
			}
		}
	}
}

void Sideswitch(int a, int B)//a,b are the position of the left side
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
			pos[0][a][b] = pos[0][a][b - (B / 2)];
			pos[0][a][b - (B / 2)] = temp;
		}
	}

}

void DuplicPos(int s, char x)
{
	if (x == 'b')
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
	else if (x == 'r')
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

void Active()
{
	int i = 0;
}


