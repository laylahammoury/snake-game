# include <time.h>  
# include <map>  
# include<queue>
# include <Windows.h>
# include <vector>
# include <string>
# include<ColorDlg.h>

/*
Welcome to the Snake game programming challenge. You are required to add code to the following three functions to implement the Snake game.
You can of course write any code that you need to outside of these functions as well.
The functions are:

Initialize()
OnTimer()
OnKeyDown()e

The file shows you how to draw rectangles with specific colors and how to draw text if you need to.
You should not need anything else because we are looking to create a visually-simple Snake game, so flat squared should be good enough.

Good luck!

* Exceptions
* classes (constructors, destructors, copy constructors, operator overloading, virtual methods, abstract classes)
* Templates (template functions and classes).

std::vector<RECT> snake;


CVs and interviews:
techlead: top resume mistakes
Deniz Sasal: very useful discussions about intervies, CVs... etc. (white background)
*/


// COLORREF is a Windows data type. It is used by the API to represent 32-bit colors.
// Create a COLORREF value using the RGB() macro:
// COLORREF myColor = RGB(red, green, blue);
// where red, green, and blue are integers from 0-255.

// Another Windows data type is RECT. It represents a rectangle. It has the members:
// top, left, right, bottom.


// Use this to fill a rectangle of the hDC with a specific color.
void FillRect(HDC hDC, const RECT* pRect, COLORREF color)
{
	COLORREF oldColor = SetBkColor(hDC, color);
	ExtTextOut(hDC, 0, 0, ETO_OPAQUE, pRect, TEXT(""), 0, 0);
	SetBkColor(hDC, oldColor);
}

//This is used to make a point and initialize it
//POINT MakePoint(int x, int y ) { POINT pt = {x, y}; return pt; }



// Use this to draw text if you need to.
void DrawTextLine(HWND hWnd, HDC hDC, const char* sText, RECT* prText, COLORREF clr);


namespace game
{
	enum Direction{up, down, left, right, stop};
	unsigned int windowWidth = 0;
	unsigned int windowHeight = 0;
	bool gameover ;
	static POINT tail , head , food;	
	int score;
	static int size = 20;
	int padding = 2;
	static int foodRandomizing;
	std::vector<POINT> pointbody;
	std::queue<Direction> moves;
	//COLORREF snakeColor = RGB(255,100,0);
	
	
	Direction dir;
	
	// This is called when the application is launched.
	bool Initialize(HWND hWnd)
	{
		RECT rClient;
		GetClientRect(hWnd, &rClient);
		windowWidth = rClient.right - rClient.left; // rClient.top and rClient.left are always 0.
		windowHeight = rClient.bottom - rClient.top;
		gameover = false;
		pointbody.clear();
		
		dir = stop;
		score = 0;
		foodRandomizing = windowWidth/size;
		srand (time(NULL));
		 
	
		OutputDebugStringA("My game has been initialized. This text should be shown in the 'output' window in VS");
	
		
		POINT initArray[3];
		int initX = windowWidth /2;
		int initY = windowHeight/2;

		for (int i = 0; i < 3; i++)
		{
			initArray[i].x = initX ;
			initArray[i].y = initY - (i*size);
			pointbody.push_back(initArray[i]);
		}
		

		food.x = initArray[0].x ;
		food.y = initArray[0].y + (2* size);

		return true;
	}
	// This is called to draw the snake
	void DrawSnake(const std::vector<POINT>& Snakbody , HDC hDC)
	{
		RECT temp ;
		int green = 255;
		for (int i = Snakbody.size()-1; i >= 0; i--)
			{
				temp.top = Snakbody[i].x ;
				temp.left = Snakbody[i].y ;
				temp.bottom = temp.top + (size-padding);
				temp.right = temp.left + (size-padding);
				if(green > 105)
					green-=15;
				if (i == 0)
					FillRect(hDC, &temp, RGB(255, 77, 50)); //Draw a red square.
				else
					FillRect(hDC, &temp, RGB(50, green, 50)); //Draw a green square.
			}
	}

	void checkGameover ( HWND hWnd)
	{

		for (int i = 1; i < pointbody.size()-1; i++)
		{
			if(head.x ==  pointbody[i].x && head.y == pointbody[i].y)
				{
					gameover = true;
					MessageBox(0,TEXT("You ate yourself :P "), TEXT("GAMEOVER !!!!"), MB_OK);
					Initialize(hWnd);
				}
			
		}
		
		if( (pointbody[0].y + size) > windowWidth ||  pointbody[0].y  < 0 ||  (pointbody[0].x + size) > windowHeight ||  pointbody[0].x  < 0)//Gameover states
			{
				gameover = true;
				MessageBox(0,TEXT("Try again :O "), TEXT("GAMEOVER !!!!"), MB_OK);
				Initialize(hWnd);
			}
	}

	void MoveSnake( Direction direction){

		int newTop = pointbody[0].x;
		int newLeft = pointbody[0].y;
		switch (dir)
		{
			
		case up:
			newTop -= (size);
			
			break;

		case down:
			newTop += (size);
			
			break;

		case left:
			newLeft -= (size);
			
			break;

		case right:
			newLeft += (size);
		
			break;

		}
		
		head.x = newTop;
		head.y = newLeft;
	
		if (dir != stop){
			pointbody.insert(pointbody.begin(), head);
		
			tail = pointbody[pointbody.size()-1];
	
			pointbody.erase(pointbody.end()-1);
	
		}
	}

	void eatFood(){
	
	if (food.x == pointbody[0].x && food.y == pointbody[0].y)
		{
			food.x  = (rand()% foodRandomizing )* size;
			food.y  = (rand() % foodRandomizing )* size;
			pointbody.push_back(tail);
			score += 10; 
			
		} 
	}
	
	


	// This is called periodically. Use it to update your game state and draw to the window.
	void CALLBACK OnTimer(HWND hWnd, UINT Msg, UINT_PTR idTimer, DWORD dwTime)
	{
		
		HDC hDC = GetDC(hWnd);
		RECT rClient;
		GetClientRect(hWnd, &rClient);
		FillRect(hDC, &rClient, RGB(0, 0, 0)); // Clear the window to blackness.
		char text[256] = { 0 };
		sprintf_s(text, " Score : %d", score);
		RECT rText = { 0, 0, rClient.right, 15 };
		DrawTextLine(hWnd, hDC, text, &rText, RGB(120, 120, 120));

		if( moves.size() >=1 )
		{
			dir = moves.front();
			moves.pop();
		}
		
		
		if ( gameover )
		{
			DrawSnake(pointbody, hDC);//to draw it and see how you died
			return ;
		}

		MoveSnake( dir);
		eatFood();
		
		RECT foodTemp;
		foodTemp.top = food.x ;
		foodTemp.left = food.y ;
		foodTemp.bottom = foodTemp.top + (size-padding);
		foodTemp.right= foodTemp.left+ (size-padding);

		FillRect(hDC, &foodTemp, RGB(255, 255, 77)); //Draw a food square.
		DrawSnake(pointbody , hDC);
		checkGameover ( hWnd);

	
	}//end of OnTimer


	// This is called when the user presses a key on the keyboard.
	// Use this to change the direction of your snake.
	bool OnKeyDown(WPARAM keyCode)
	{
		
		if (keyCode == VK_UP && dir !=Direction::down)
				{
					dir = up;	
					moves.push(dir);
				}
		else if (keyCode == VK_DOWN && dir !=up)
				{
					dir = down;
					moves.push(dir);
				}
			else if ( keyCode == VK_LEFT && dir !=right && dir !=stop)
				{
					dir = left;
					moves.push(dir);
				}
				else if( keyCode == VK_RIGHT && dir!=left)
				{
					dir = right;
					moves.push(dir);
				}
			
		
		return false; // They key pressed does not interest us. Let the OS handle it.
	}


}//end of namespace game
