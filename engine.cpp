#include <Windows.h>;
#include <iostream>;
#include <string>;
#include <chrono>;
using namespace std;

int nScreenHeight = 40;
int nScreenWidth = 120;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 0.55f;

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
	map += L"#..........#...#";
	map += L"#..........#...#";
	map += L"###............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#........#######";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	//game loop
	while (1)
	{
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fDeltaTime = elapsedTime.count();

		//controls
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			fPlayerA -= (0.8f) * fDeltaTime;

		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			fPlayerA += (0.8f) * fDeltaTime;

		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			fPlayerX += sinf(fPlayerA) * 5.0f * fDeltaTime;
			fPlayerY += cosf(fPlayerA) * 5.0f * fDeltaTime;

			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX -= sinf(fPlayerA) * 5.0f * fDeltaTime;
				fPlayerY -= cosf(fPlayerA) * 5.0f * fDeltaTime;
			}
		}

		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			fPlayerX -= sinf(fPlayerA) * 5.0f * fDeltaTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fDeltaTime;

			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX += sinf(fPlayerA) * 5.0f * fDeltaTime;
				fPlayerY += cosf(fPlayerA) * 5.0f * fDeltaTime;
			}
		}
		//end of controls

		for (int x = 0; x < nScreenWidth; x++)
		{
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / nScreenWidth * fFOV); //field of view angle
			float fDistanceToWall = 0.0f;
			bool bHitWall = false;
			
			float fDirectionX = sinf(fRayAngle);
			float fDirectionY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += 0.1f;

				int nTestX = fPlayerX + fDirectionX * fDistanceToWall;
				int nTestY = fPlayerY + fDirectionY * fDistanceToWall;

				if (nTestX < 0 || nTestY < 0 || nTestX >= nMapWidth || nTestY > nMapHeight)
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
			short nShade = ' ';
			if (fDistanceToWall <= fDepth / 4.0f)		nShade = 0x2588;
			else if (fDistanceToWall < fDepth / 3.0f)	nShade = 0x2593;
			else if (fDistanceToWall < fDepth / 2.0f)	nShade = 0x2592;
			else if (fDistanceToWall < fDepth)			nShade = 0x2591;
			else										nShade = ' ';

			int nCeiling = (nScreenHeight / 2.0) - (nScreenHeight / fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y < nCeiling)
					screen[y * nScreenWidth + x] = ' ';
				else if (y > nCeiling && y < nFloor)
					screen[y * nScreenWidth + x] = nShade;
				else
				{
					short nFloorShade = ' ';
					float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25)		nFloorShade = '#';
					else if (b < 0.5)	nFloorShade = 'x';
					else if (b < 0.75)	nFloorShade = '.';
					else if (b < 0.9)	nFloorShade = '-';
					else				nFloorShade = ' ';
					screen[y * nScreenWidth + x] = nFloorShade;
				}
			}
		}

		screen[nScreenHeight * nScreenWidth - 1] = '\0'; //escape
		WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight * nScreenWidth, { 0, 0 }, &dwBytesWritten);
	}

	return 0;
}