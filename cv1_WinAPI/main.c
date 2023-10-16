#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


// Global variable
HINSTANCE hInst;
UINT  MessageCount = 0;
UINT  Count = 0;
int posX = 0;
int posY = 0;

// my own variables
int ellipseX = 150;
int ellipseY = 150;
COLORREF newObjectColor = RGB(0, 255, 0);
BOOL changeNewObjectColor = FALSE;

// car variables
int carX = 150;
int carY = 300;
COLORREF carColor = RGB(255, 230, 100);
BOOL isMouseOverCar = FALSE;

// Function prototypes.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition);
void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps);

// car function prototype
void paintCar(HDC hDC);

// Application entry point. This is the same as main() in standart C.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	WNDCLASS wcx;          // register class
	HWND hWnd;

	hInst = hInstance;     // Save the application-instance handle.
	// Fill in the window class structure with parameters that describe the main window.

	wcx.style = CS_HREDRAW | CS_VREDRAW;              // redraw if size changes
	wcx.lpfnWndProc = (WNDPROC)MainWndProc;          // points to window procedure
	wcx.cbClsExtra = 0;                               // no extra class memory
	wcx.cbWndExtra = 0;                               // no extra window memory
	wcx.hInstance = hInstance;                        // handle to instance
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);      // predefined app. icon
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);        // predefined arrow
	wcx.hbrBackground = GetStockObject(WHITE_BRUSH);  // white background brush
	wcx.lpszMenuName = (LPCSTR)"MainMenu";          // name of menu resource
	wcx.lpszClassName = (LPCSTR)"MainWClass";        // name of window class

	// Register the window class.

	if (!RegisterClass(&wcx)) return FALSE;

	// create window of registered class

	hWnd = CreateWindow(
		"MainWClass",        // name of window class
		"ITU",               // title-bar string
		WS_OVERLAPPEDWINDOW, // top-level window
		200,                  // default horizontal position
		25,                 // default vertical position
		1000,                // default width
		700,                 // default height
		(HWND)NULL,         // no owner window
		(HMENU)NULL,        // use class menu
		hInstance,           // handle to application instance
		(LPVOID)NULL);      // no window-creation data
	if (!hWnd) return FALSE;

	// Show the window and send a WM_PAINT message to the window procedure.
	// Record the current cursor position.

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// loop of message processing
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}


LRESULT CALLBACK MainWndProc(
	HWND hWnd,        // handle to window
	UINT uMsg,        // message identifier
	WPARAM wParam,    // first message parameter
	LPARAM lParam)    // second message parameter
{
	HDC         hDC;
	PAINTSTRUCT ps;
	POINT cursorPosition;

	// init cursor position 
	GetCursorPos(&cursorPosition);
	ScreenToClient(hWnd, &cursorPosition);

	switch (uMsg)
	{
	case WM_CREATE:
		break;

		// character input 
	case WM_CHAR:
		switch (wParam) {
		case 0x08:  // backspace
		case 0x0A:  // linefeed
		case 0x1B:  // escape
			break;

		case 0x09:  // tab
			break;

		default:
			break;
		}
		break;

		// key input
	case WM_KEYDOWN:
		switch (wParam) {
			// update posX and posY in order to move object
		case VK_LEFT: // left arrow
			// ellipseX -= 10;
			carX -= 20;
			break;
		case VK_RIGHT: // right arrow
			// ellipseX += 10;
			carX += 20;
			break;
		case VK_UP: // up arrow
			// ellipseY -= 10;
			carY -= 20;
			break;
		case VK_DOWN: // down arrow
			// ellipseY += 10;
			carY += 20;
			break;

			// react on the other pressed keys 
		case VK_SPACE: // space
			ellipseX += 10;
			break;
		case VK_BACK: // backspace
			ellipseX -= 10;
			break;
		case VK_TAB: // tab
			break;
			// more virtual codes can be found here: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

		// get cursor position 
	case WM_MOUSEMOVE:
		if (cursorPosition.x >= carX && cursorPosition.x <= carX + 100 && cursorPosition.y >= carY && cursorPosition.y <= carY + 50)
			isMouseOverCar = TRUE;  // cursor is above car
		else
			isMouseOverCar = FALSE;  // cursor is somewhere else
		InvalidateRect(hWnd, NULL, TRUE);  // window redraw request
		break;

		// react on mouse clicks
	case WM_LBUTTONDOWN:
		changeNewObjectColor = TRUE;
		newObjectColor = RGB(255, 0, 0); // changing color to red
		InvalidateRect(hWnd, NULL, TRUE); // window redraw request
		if (isMouseOverCar)
		{
			carColor = RGB(255, 128, 0);  // car color change to orange
			InvalidateRect(hWnd, NULL, TRUE);  // window redraw request
		}
		break;
	case WM_LBUTTONUP:
		carColor = RGB(255, 230, 100);  // color change back to gold-ish
		if (changeNewObjectColor) {
			newObjectColor = RGB(0, 255, 0); // color change back to green
			InvalidateRect(hWnd, NULL, TRUE); // window redraw request
		}
		changeNewObjectColor = FALSE;
		break;

		// paint objects
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		paintObject(hWnd, hDC, ps, posX, posY, cursorPosition);
		paintCar(hDC);
		paintPosition(hWnd, hDC, ps);
		// paint other objects
		// paintObject2(hWnd, hDC, ps, posX, posY, cursorPosition);
		// paintObject3(hWnd, hDC, ps, posX, posY, cursorPosition);
		EndPaint(hWnd, &ps);
		DeleteDC(hDC);
		break;

		//
		// Process other messages.
		//

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}


void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition)
{
	// Paint rectangles, ellipses, polygons, lines etc.

	RECT rect; // rectangle structure
	int rect_width = 100;
	int rect_height = 50;

	// rectangle positioning
	rect.left = posX;
	rect.top = posY;
	rect.right = posX + rect_width;
	rect.bottom = posY + rect_height;

	FillRect(hDC, &rect, (HBRUSH)GetStockObject(GRAY_BRUSH));

	HBRUSH blue_brush = CreateSolidBrush(RGB(0, 0, 255)); // blue brush creation
	HBRUSH red_brush = CreateSolidBrush(RGB(255, 0, 0));

	// painting the ellipse blue
	HBRUSH old_brush = (HBRUSH)SelectObject(hDC, blue_brush);
	Ellipse(hDC, posX + 100, posY + 200, posX + 200, posY + 150);

	// renewing the former brush (to none) before drawing the last ellipse
	// THIS ELLIPSE MOVES WHEN SPACE OR BACKSPACE
	SelectObject(hDC, GetStockObject(NULL_BRUSH));
	Ellipse(hDC, ellipseX + 200, ellipseY + 300, ellipseX + 300, ellipseY + 250);

	// we draw a red triangle
	POINT polygonPoints[] = { {posX + 50, posY + 400}, {posX + 100, posY + 450}, {posX + 150, posY + 400} };
	SelectObject(hDC, red_brush);
	Polygon(hDC, polygonPoints, 3);

	HBRUSH newObjectBrush = CreateSolidBrush(newObjectColor); // current brush with new object color

	// painting the new ellipse with current color
	SelectObject(hDC, newObjectBrush);
	Ellipse(hDC, posX + 300, posY + 200, posX + 400, posY + 150);

	// deleting the brushes
	SelectObject(hDC, old_brush);
	DeleteObject(blue_brush);
	DeleteObject(red_brush);
	DeleteObject(newObjectBrush);
}

void paintCar(HDC hDC)
{
	HBRUSH bodyBrush = CreateSolidBrush(carColor);
	HBRUSH wheelBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH windowBrush = CreateSolidBrush(RGB(153, 255, 255));

	// car body
	SelectObject(hDC, bodyBrush);
	Rectangle(hDC, carX, carY, carX + 100, carY + 50);
	Rectangle(hDC, carX - 20, carY + 50, carX, carY + 30);

	// car windows
	SelectObject(hDC, windowBrush);
	Rectangle(hDC, carX, carY + 5, carX + 45, carY + 30);
	Rectangle(hDC, carX + 50, carY + 5, carX + 95, carY + 30);

	// car wheels
	SelectObject(hDC, wheelBrush);
	Ellipse(hDC, carX + 5, carY + 40, carX + 25, carY + 60);
	Ellipse(hDC, carX + 65, carY + 40, carX + 85, carY + 60);

	DeleteObject(bodyBrush);
	DeleteObject(windowBrush);
	DeleteObject(windowBrush);
}

void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps)
{
	char        text[40];          // buffer to store an output text
	HFONT       font;              // new large font
	HFONT       oldFont;           // saves the previous font

	font = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 0);
	oldFont = (HFONT)SelectObject(hDC, font);
	sprintf(text, "Position -- x:%d, y:%d", posX, posY);
	TextOut(hDC, 50, 600, text, (int)strlen(text));
	SelectObject(hDC, oldFont);
	DeleteObject(font);
}
