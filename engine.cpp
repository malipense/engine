#include <Windows.h>;
#include <iostream>;
#include <string>;
using namespace std;

int nScreenHeight = 40;
int nScreenWidth = 120;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.0 / 4.0;

float fDepth = 16.0f;

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
		for (int x = 0; x < nScreenWidth; x++)
		{
			float fRayAngle = (fFOV / 2.0f) + ((float)x / nScreenWidth * fFOV); //field of view angle
			float fDistanceToWall = 0.0f;
			bool bHitWall = false;
			
			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += 0.1f;

				int nTestY = fPlayerY + fEyeY * fDistanceToWall;
				int nTestX = fPlayerX + fEyeX * fDistanceToWall;

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY > nMapHeight)
				{
					fDistanceToWall = fDepth;
					bHitWall = true;
				}
				else
				{
					if (map[nTestY * nMapWidth + nTestX] == '#')
						bHitWall = true;
				}
			}

			//draw ceiling and floor
			int nCeiling = (nScreenHeight / 2.0) - (nScreenHeight / fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y < nCeiling)
					screen[y * nScreenWidth + x] = ' ';
				else if (y > nCeiling && y < nFloor)
					screen[y * nScreenWidth + x] = '#';
				else
					screen[y * nScreenWidth + x] = ' ';
			}
		}

		screen[nScreenHeight * nScreenWidth - 1] = '\0'; //escape
		WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight * nScreenWidth, { 0, 0 }, &dwBytesWritten);
	}

	return 0;
}