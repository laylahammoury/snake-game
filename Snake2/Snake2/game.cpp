# include <time.h>  
# include <map>  
# include <Windows.h>
# include <vector>
# include <string>

/*
Welcome to the Snake game programming challenge. You are required to add code to the following three functions to implement the Snake game.
You can of course write any code that you need to outside of these functions as well.
The functions are:

Initialize()
OnTimer()
OnKeyDown()

The file shows you how to draw rectangles with specific colors and how to draw text if you need to.
You should not need anything else because we are looking to create a visually-simple Snake game, so flat squared should be good enough.

Good luck!
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

void FillSnake(std::vector<RECT> body , HDC hDC)
{
	
	for (int i = 0; i < body.size(); i++)
		{
			
			RECT temp = body[i];
			temp.left+=2;
			temp.top+=2;
			if (i == 0)
				FillRect(hDC, &temp, RGB(255, 0, 0)); //Draw a red square.
			else
				FillRect(hDC, &temp, RGB(120, 255, 10)); //Draw a red square.
		}
}


// Use this to draw text if you need to.
void DrawTextLine(HWND hWnd, HDC hDC, const char* sText, RECT* prText, COLORREF clr);

enum Direction{up, down, left, right, stop};

namespace game
{
	unsigned int windowWidth = 0;
	unsigned int windowHeight = 0;
	bool gameover ;
	static int size = 20;
	//int Length = 2;
	//static int score;
	std::vector<RECT> body;
	//std::map <RECT,bool> visited;
	
	RECT food;
	Direction dir;
	
	
	// This is called when the application is launched.
	bool Initialize(HWND hWnd)
	{
		RECT rClient;
		GetClientRect(hWnd, &rClient);
		windowWidth = rClient.right - rClient.left; // rClient.top and rClient.left are always 0.
		windowHeight = rClient.bottom - rClient.top;
		 gameover = false;
		 body.clear();
		 dir = stop;
		//score = 0;
	
		OutputDebugStringA("My game has been initialized. This text should be shown in the 'output' window in VS");
		RECT r , r2 , r3;

		r.left = windowWidth / 2 ;
		r.top = windowHeight / 2;
		r.right = r.left + size;
		r.bottom = r.top + size;

		r2.left = r.left-size;
		r2.top = r.top;
		r2.right = r2.left + size;
		r2.bottom = r2.top + size;
		
		r3.left = r2.left-size;
		r3.top = r2.top;
		r3.right = r3.left + size;
		r3.bottom = r3.top + size;

		body.push_back(r);
		
		//visited.insert(std::pair<RECT,int>(r,true));
		body.push_back(r2);
		//visited.insert(std::pair<RECT,int>(r2,true));
		body.push_back(r3);
		//visited.insert(std::pair<RECT,int>(r3,true));
		
		
		    food.left = r.left - 60;
			food.top = r.top;
			food.right = food.left + (size);
			food.bottom = food.top + (size);
		
		return true;
	}


	// This is called periodically. Use it to update your game state and draw to the window.
	void CALLBACK OnTimer(HWND hWnd, UINT Msg, UINT_PTR idTimer, DWORD dwTime)
	{
		if ( gameover )
			return;
		HDC hDC = GetDC(hWnd);
		RECT rClient;
		GetClientRect(hWnd, &rClient);
		FillRect(hDC, &rClient, RGB(0, 0, 0)); // Clear the window to blackness.
		char text[256] = { 0 };
		sprintf_s(text, "OnTimer. Time: %d", dwTime);
		RECT rText = { 0, 0, rClient.right, 15 };
		DrawTextLine(hWnd, hDC, text, &rText, RGB(120, 120, 120));

	
		
	

	
		int newTop = body[0].top;
		int newLeft = body[0].left;
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
		default:
			
			break;
		}
		
		RECT s;

	s.top = newTop;
	s.left = newLeft;
	s.bottom =s.top + size ;
	s.right =s.left + size ;
	//TODO : check new == any existing()
	//map of bolean to th RECTs
	
	//for (int i = 1; i < body.size(); i++)
	//{
	//	if(s.top ==  body[i].top && s.left == body[i].left)
	//		gameover = true;
	//}

	body.insert(body.begin(), s);
	RECT tail;
	tail = body[body.size()-1];
	//TODO: 
	//visited [body.size()-1] = false;

	body.erase(body.end()-1);
	
	

		
		if (food.left == body[0].left && food.top == body[0].top )
		{
			srand (time(NULL));
			food.left  = rand()%30 * size;
			food.top  = rand() % 30 * size;
			food.right = food.left + (size);
			food.bottom = food.top + (size);
			body.push_back(tail);
			//score ++; 
			//Length++;

		} 
		
		
		

		RECT foodTemp = food;
		foodTemp.top +=2;
		foodTemp.left +=2;

		FillRect(hDC, &foodTemp, RGB(255, 255, 10)); //Draw a food square.

		
		FillSnake(body , hDC);
		if(body[0].right > windowWidth || body[0].left < 0 || body[0].bottom > windowHeight || body[0].top < 0)//Gameover states
			{
				gameover = true;
				MessageBox(0,TEXT("Try again :P "), TEXT("GAMEOVER !!!!"), MB_OK);
				Initialize(hWnd);
			}

	}


	// This is called when the user presses a key on the keyboard.
	// Use this to change the direction of your snake.
	bool OnKeyDown(WPARAM keyCode)
	{
		
		if (keyCode == VK_UP && dir !=down)
				dir = up;	
		else if (keyCode == VK_DOWN && dir !=up)
				dir = down;
			else if ( keyCode == VK_LEFT && dir !=right && dir !=stop)
					dir = left;
				else if( keyCode == VK_RIGHT && dir!=left)
						dir = right;
			
		
		return false; // They key pressed does not interest us. Let the OS handle it.
	}
}
