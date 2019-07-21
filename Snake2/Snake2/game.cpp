
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
			FillRect(hDC, &body[i], RGB(120, 255, 10)); //Draw a red square.
		}
}


// Use this to draw text if you need to.
void DrawTextLine(HWND hWnd, HDC hDC, const char* sText, RECT* prText, COLORREF clr);

enum Direction{up, down, left, right, stop};

namespace game
{
	unsigned int windowWidth = 0;
	unsigned int windowHeight = 0;
	unsigned int vertical;
	unsigned int horizontal;
	bool gameover = false;
	static int size = 15;
	int Length = 2;
	int score;
	std::vector<RECT> body;
	RECT food;
	Direction dir = stop;
	
	// This is called when the application is launched.
	bool Initialize(HWND hWnd)
	{
		RECT rClient;
		GetClientRect(hWnd, &rClient);
		windowWidth = rClient.right - rClient.left; // rClient.top and rClient.left are always 0.
		windowHeight = rClient.bottom - rClient.top;
		vertical = windowHeight;
		horizontal = windowWidth;
		score = 0;
	
		OutputDebugStringA("My game has been initialized. This text should be shown in the 'output' window in VS");
		/*RECT r , r2 , r3;

		r.left = horizontal / 2;
		r.top = vertical / 2;
		r.right = r.left + size;
		r.bottom = r.top + size;

		r2.left = r.left-size-2;
		r2.top = r.top;
		r2.right = r2.left + size;
		r2.bottom = r2.top + size;
		
		r3.left = r2.left-size-2;
		r3.top = r2.top;
		r3.right = r3.left + size;
		r3.bottom = r3.top + size;

		body.push_back(r);
		body.push_back(r2);
		body.push_back(r3);*/
		
		    food.left = windowWidth - 150;
			food.top = windowHeight - 120;
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

	
		
	

		//RECT r ;
		//RECT r2 , r3;
		RECT r , r2 , r3;

		r.left = horizontal / 2;
		r.top = vertical / 2;
		r.right = r.left + size;
		r.bottom = r.top + size;

		r2.left = r.left-size-2;
		r2.top = r.top;
		r2.right = r2.left + size;
		r2.bottom = r2.top + size;
		
		r3.left = r2.left-size-2;
		r3.top = r2.top;
		r3.right = r3.left + size;
		r3.bottom = r3.top + size;

		body.push_back(r);
		body.push_back(r2);
		body.push_back(r3);

	/*	r.left = horizontal / 2;
		r.top = vertical / 2;
		r.right = r.left + size;
		r.bottom = r.top + size;*/
		//int prevTop  = r.top;
		switch (dir)
		{
			int newTop = body[0].top;
			int newLeft = body[0].left;
		case up:
			newTop -= (size+2);
			 //vertical-=size;
			 //prevTop +=(size+2);
			break;
		case down:
			newTop += (size+2);

			 //vertical+=size;
			  //prevTop -=(size+2);
			break;
		case left:

			newLeft -= (size+2);

			//horizontal-=size;
			break;
		case right:
			newLeft += (size+2);

			//horizontal+=size;
			break;
		default:
			
			break;
		}
		
		/*r2.left = r.left-size-2;
		r2.top = prevTop;
		r2.right = r2.left + size;
		r2.bottom = r2.top + size;*/

		//if (food.left == r.left && food.top == r.top )
		if (food.left == body[0].left && food.top == body[0].top )
		{
			
			food.left = rand() % windowWidth;
			food.top = rand() % windowHeight;
			food.right = food.left + (size);
			food.bottom = food.top + (size);
			score ++; 
			Length++;

		} 
		
		//if(r.right== windowWidth || r.left==0 || r.bottom== windowHeight || r.top==0)//Gameover states
		if(body[0].right== windowWidth || body[0].left==0 || body[0].bottom== windowHeight || body[0].top==0)//Gameover states
		{
			gameover = true;
			MessageBox(0,TEXT("Try again :P "), TEXT("GAMEOVER !!!!"), MB_OK);
		}

		FillRect(hDC, &food, RGB(255, 255, 10)); //Draw a food square.

		
		//FillSnake(body , hDC);

		FillRect(hDC, &body[0], RGB(225, 255, 10)); //Draw a red square.
		FillRect(hDC, &body[1], RGB(225, 255, 10)); //Draw a red square.
		FillRect(hDC, &body[2], RGB(225, 255, 10)); //Draw a red square.

		//FillRect(hDC, &r2, RGB(120, 255, 10)); //Draw a red square.

	}


	// This is called when the user presses a key on the keyboard.
	// Use this to change the direction of your snake.
	bool OnKeyDown(WPARAM keyCode)
	{
		if (keyCode == VK_UP )
				dir = up;	
		else if ( keyCode == VK_DOWN )
				dir = down;
			else if ( keyCode == VK_LEFT )
					dir = left;
				else if( keyCode == VK_RIGHT)
						dir = right;
			
		
		return false; // They key pressed does not interest us. Let the OS handle it.
	}
}
