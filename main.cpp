#include"GameManager.h"

int main()
{
	cout << "Welcome to snake game v1.0" << endl;
	cout << "You can use move arrows or A,S,D and W keys" << endl;
	Wait(1.5);
	GameManager *G1=NULL;
	char tryAgain;

	do {
		ClearScreen();
		G1 = new GameManager;
		G1->StartGame();
		delete G1;
		cout << "try again(y/n)" << endl;
		cin >> tryAgain;
	} while (tryAgain == 'y');

	system("pause");
	return 0;
}