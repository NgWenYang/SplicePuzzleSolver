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

int maxlength = 5;//precision. if no solution found, maxlength++ << will be applied later 
int maxstep = 2;//total steps allowed


int pos[2][8][128] = { { { 0,1 },{ 0,1,1 }, { 0,1,1 } } };//initial stage, all single pos assumed as left side
														 //0 = empty, 1 = occupied, 2~4 = special, -1 = original 
int temppos[13][2][8][128];//[step +11 ][space, 0 is main, 1 is temporary][vertical][horizontal]

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
	printf("Initial stage:\n");
	Show();
	SeekPath(maxstep);
	getchar();
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
			pos[0][tempa][tempB+1] = -2;
		}
		else 
		{
			pos[0][tempa][tempB] = -1;
			pos[0][tempa][tempB+1] = -1;
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
	DuplicPos(stepleft, 'b');
	for (i = 1;i <= maxlength;i++)
	{
		for (j = 1;j <= (int)pow(2, i);j++)
		{
			if (pos[0][i][j] == 0)
				continue;
			else
			{
				Detach(i, j);
				DuplicPos(stepleft+10, 'b');
				for (k = 1;k <= maxlength;k++)
				{
					for (l = 1;l <= (int)pow(2, i);l++)
					{
						if (pos[0][k][l] != 1 && pos[0][k - 1][(l + 1) / 2] == 1)
						{
							if (l % 2 == 1 && pos[0][k][l] != -1)
							{
								Attach(k, l);
								printf("\nstep%d %d,%d ---> %d,%d:\n", 3 - stepleft, i, j, k, l);
								Show();
								if (stepleft > 1)
									SeekPath(stepleft - 1);
							}
							else if (l % 2 == 0 && pos[0][k][l - 1] == 1)
							{
								if (pos[0][k][l] == -1)//case 1 -1 ---> 1  1
								{
									Sideswitch(k, l - 1);
									Attach(k, l - 1);
									printf("\nstep%d %d,%d ---> %d,%d:\n", 3 - stepleft, i, j, k, l - 1);
									Show();
									if (stepleft > 1)
										SeekPath(stepleft - 1);
								}
								else if (pos[0][k][l] == -2)//case 1 -2 ---> 1  1
								{
									Attach(k, l);
									printf("\nstep%d %d,%d ---> %d,%d:\n", 3 - stepleft, i, j, k, l);
									Show();
									if (stepleft > 1)
										SeekPath(stepleft - 1);
								}
								else if (pos[0][k][l] == 0)//case 1  0 ---> 1  1 ,two ways to attach, left and right
								{
									//attach left
									Sideswitch(k, l - 1);
									Attach(k, l - 1);
									printf("\nstep%d %d,%d ---> %d,%d:\n", 3 - stepleft, i, j, k, l - 1);
									Show();
									if (stepleft > 1)
										SeekPath(stepleft - 1);
									DuplicPos(stepleft + 10, 'r');
									
									//attach right
									Attach(k, l);
									printf("\nstep%d %d,%d ---> %d,%d:\n", 3 - stepleft, i, j, k, l);
									Show();
									if (stepleft > 1)
										SeekPath(stepleft - 1);
								}
							}
							DuplicPos(stepleft + 10, 'r');
						}
						/*if (pos[0][k][l] != 1 && pos[0][k][l] != -1 && pos[0][k - 1][(l + 1) / 2] == 1)
						{
							if (l % 2 == 1 && pos[0][k][l + 1] != 1)//both left and right side are empty
							{
								Attach(k, l);
								printf("\nstep%d %d,%d ---> %d,%d:\n",3-stepleft , i, j, k, l);
								Show();
								if (stepleft > 1)
									SeekPath(stepleft - 1);
							}
							else if (l % 2 == 0 && pos[0][k][l - 1] == 1)//left side is occupied but right is empty, so there are two ways to attach
							{
								//left side

								if (pos[0][k][l] != -2)
								{
									Sideswitch(k, l - 1);
									Attach(k, l - 1);
									printf("\nstep%d %d,%d ---> %d,%d:\n", 3 - stepleft, i, j, k, l - 1);
									Show();
									if (stepleft > 1)
										SeekPath(stepleft - 1);
									DuplicPos(stepleft + 10, 'r');
								}

								//right side
								Attach(k, l);
								printf("\nstep%d %d,%d ---> %d,%d:\n", 3 - stepleft, i, j, k, l);
								Show();
								if (stepleft > 1)
									SeekPath(stepleft - 1);

							}
							DuplicPos(stepleft+10, 'r');
						}*/
					}
				}
				DuplicPos(stepleft, 'r');
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


