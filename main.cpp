#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;
char tmp_map[18][32];
void setcolor(int tint);
void setcolor(int tint)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tint);
}
char map[18][32] = {
	"+^^^^^^^^^^^^^^^^^^^^^^^^^^^^^+",
	"|                             >",
	"|                             >",
	"|## ########## ##   ######### >",
	"|   |                         >",
	"| | |### |  |           |     >",
	"| |      |  | |###  |   |  |  >",
	"| | #####|  | |      ## |     >",
	"| |           |###  |      |  >",
	"| |##### ###         ##       >",
	"|          ######  ####### ###>",
	"|                             >",
	"|# ### ####      ###   #######>",
	"|               #      #      >",
	"|####       ######     #      >",
	"|                             >",
	"|                             >",
	"+vvvvvvvvvvvvvvvvvvvvvvvvvvvvv+"
	};

void ShowMap()
{	setcolor(4); // DARK RED
	for(int i = 0; i < 18; i++) 
	{
		
		printf("%s\n",map[i] );
	}
}

void gotoxy( short x, short y )
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ;
    COORD position = { x, y } ;
    SetConsoleCursorPosition( hStdout, position ) ;
}

class entity {
public:
	entity( int x, int y )
	{
		this ->x = x;
		this ->y = y;
	}

	void move_x( int p )
	{
		if( map[y][x+p] == ' ' ) 
		{
			x += p;
		}	
	}

	void move_y( int p )
	{
		if( map[y+p][x] == ' ' )
		{
			y += p;
		}
	}
	

	void move( int p, int q )
	{
		x += p;
		y += q;
	}

	int get_x()
	{ 
		return x; 
	}
	int get_y()
	{
		return y; 
	}

	void draw( char p )
	{
		map[x][y] = p;
		gotoxy( x, y ); 
		setcolor(7); // LIGHT GRAY
		printf( "%c", p );
	}

private:
	int x;
	int y;
};

struct walk 
{
	short walk_count;
	short x;
	short y;
	short back;
};

struct target 
{
	short x;
	short y;
};

vector<target> walk_queue;

vector<walk> BFSArray;

void AddArray( int x, int y, int wc , int back )
{
	if( tmp_map[y][x] == ' ' || tmp_map[y][x] == '.' )
	{
		tmp_map[y][x] = '#';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.walk_count = wc;
		tmp.back = back;
		BFSArray.push_back( tmp );
	}
}

void FindPath( int sx, int sy, int x, int y )
{
	memcpy( tmp_map, map, sizeof(map) );
	BFSArray.clear();
	walk tmp;
	tmp.x = sx;
	tmp.y = sy;
	tmp.walk_count = 0;
	tmp.back = -1;
	BFSArray.push_back( tmp );

	int i = 0;
	while( i < BFSArray.size() )
	{
		if( BFSArray[i].x == x && BFSArray[i].y == y )
		{
			walk_queue.clear();
			target tmp2;
			while( BFSArray[i].walk_count != 0 )
			{
				tmp2.x = BFSArray[i].x;
				tmp2.y = BFSArray[i].y;
				walk_queue.push_back( tmp2 );

				i = BFSArray[i].back;
			}
			break;
		}
		AddArray( BFSArray[i].x+1, BFSArray[i].y, BFSArray[i].walk_count+1, i );
		AddArray( BFSArray[i].x-1, BFSArray[i].y, BFSArray[i].walk_count+1, i );
		AddArray( BFSArray[i].x, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
		AddArray( BFSArray[i].x, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );
		i++;
	}
	BFSArray.clear();
}

int main()
{
    bool running = true;
	int x = 15; // hero x
	int y = 16; // hero y
	int old_x;
	int old_y;
	
	int ex = 1;
	int ey = 1;

	int pts = 0;
	setcolor(13);	//pink;
	printf(":::::::::::::Pacman:::::::::::::\n\n");
	setcolor(14);	//yellow
	printf("\nINSTRUCTIONS\n");
	setcolor(11);	//light blue
	printf("\n1) You have to move the pacman using arrow key.");
	printf("\n2) You hava to choose the difficulty level  ");
	printf("\n3) Eneny is denoted by 'E' its speed will depend on the difficulty level of the game.");
	printf("\n4) You have to eat all the dots without getting caught by the enemy.");
	printf("\n5) You will also see a score board on screen.");
	setcolor(12); 	//red
	printf("\n\nE -> Easy ");
	printf("\nN -> Normal");
	printf("\nH -> Hard");
	printf("\nChoose the level: ");
	char level;
	int speedmod = 3;

	cin >> level;

	if( level == 'N' )
	{
		speedmod = 2;
	}
	else if( level == 'H' )
	{
		speedmod = 1;
	}

	system("cls");
    ShowMap();

	gotoxy( x, y ); cout << "H";

	int frame = 0;

	FindPath( ex,ey,x,y );

	while( running )
	{
		gotoxy( x, y ); cout << " ";

		old_x = x;
		old_y = y;

		if ( GetAsyncKeyState( VK_UP ) )
		{
			if( map[y-1][x] == '.' )
			{ 
				y--; 
				pts++; 
			}
			else
			{
				if( map[y-1][x] == ' ' )
				{
					y--;
				}
			}
		}
		
		if ( GetAsyncKeyState( VK_DOWN ) )
		{
			if( map[y+1][x] == '.' )
			{	
				y++; 
				pts++; 
			} 
			else
			{
				if( map[y+1][x] == ' ' ) 
				{
					y++;
				}
			}
		}
		if ( GetAsyncKeyState( VK_LEFT ) )
		{
			if( map[y][x-1] == '.' )
			{ 
				x--; 
				pts++; 
			} 
			else
			{
				if( map[y][x-1] == ' ' ) 
				{
					x--;
				}
			}
		}
		if ( GetAsyncKeyState( VK_RIGHT ) )
		{
			if( map[y][x+1] == '.' )
			{ 
				x++; 
				pts++; 
			} 
			else
			{
				if( map[y][x+1] == ' ' )
				{
					x++;
				}	
			}
		}

		if( old_x != x || old_y != y )
		{
			FindPath( ex,ey,x,y );
		}
		
		gotoxy( x,y ); 
		setcolor(14);	//yellow
		cout << "H";

		map[ey][ex] = '.';
		gotoxy( ex, ey ); 
		setcolor(14);	//yellow
		cout << ".";

		if( frame%speedmod == 0 && walk_queue.size() != 0 )
		{
			ex = walk_queue.back().x;
			ey = walk_queue.back().y;
			walk_queue.pop_back();
		}

		gotoxy( ex, ey ); 
		setcolor(9);	//BLUE
		cout << "E";

		if( ex == x && ey == y )
		{			
			break;
		}

		gotoxy( 32, 18 );
		gotoxy( 32, 1 ); 
		setcolor(10);	//green
		cout << pts;
		Sleep( 100 );
		frame++;
	}

	system("cls");			//CLEAR SCREEN
	setcolor(9);	
	printf("\nWell Played....\n");
	setcolor(10);
	printf("\nYou Lose and your score is : %d",pts);
	setcolor(11);
	printf("\nThanks for playing...hope you liked it...");
		return 0;
}
