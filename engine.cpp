#include <Windows.h>;
#include <iostream>;
#include <string>;
using namespace std;

int nScreenHeight = 40;
int nScreenWidth = 120;

float fPlayerX = 0.0f;
float fPlayerY = 0.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.0 / 4.0;

int main()
{	
	wchar_t* screen = new wchar_t[nScreenHeight * nScreenWidth];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	
	wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";

	while (1)
	{
		for (int i = 0; i < nScreenWidth; i++)
		{
			float fRayAngle = (fFOV / 2.0f) + ((float)i / nScreenWidth * fFOV); //field of view angle
			float fDistanceToWall = 0.0f;
			bool bHitWall = false;
			
			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall)
				fDistanceToWall += 0.1f;
		}

		screen[nScreenHeight * nScreenWidth - 1] = '\0'; //escape
		WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight * nScreenWidth, { 0, 0 }, &dwBytesWritten);
	}

	return 0;
}