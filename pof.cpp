#include <iostream>;
#include <string>;
using namespace std;

int test()
{
	int nScreenHeight = 40;
	float fDistanceToWall = 10;
	int nCeiling = (nScreenHeight / 2.0) - (nScreenHeight / fDistanceToWall);
	//						20					40				5	-> 8 - 12
	int nFloor = nScreenHeight - nCeiling;
		
	cout << nCeiling << " | " << nFloor << endl;


	return 0;
}