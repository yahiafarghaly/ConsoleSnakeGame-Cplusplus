#pragma once
using namespace std;
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<stdlib.h>
#include<ctime>
#include<cstdlib>

#if (defined(_WIN32) |defined(_WIN64)) 
	#include<Windows.h>
	#include<conio.h> //NOT Supported in linux (it's non standard library anyway!)
	#include<ShlObj.h>
	#pragma comment(lib, "shell32.lib")
	#define ClearScreen() system("cls");
	#define Wait(time_sec) Sleep(time_sec*1000); // in msec
	#define  WINDOWS_OS
#elif defined(__linux__)
	#define ClearScreen() system("clear");
    #define Wait(time_sec) sleep(time_sec);//in sec
	#define  LINUX_OS
	#include<stdio.h>
	#include <unistd.h>   //_getch*/
	#include <termios.h>  //_getch*/
	char _linux_getch();
#endif



typedef unsigned int  uint32;
enum Direction{STOP,LEFT,RIGHT,UP,DOWN};

class Snake;
class GameManager
{
private:

	Snake* _snake;
	uint32 _fruitX, _fruitY;
	uint32 _width, _height;
	Direction _nextDir;
	uint32 _score;
	bool _gameOver;
	string _playerName;

	inline unsigned int getRandom(unsigned int start, unsigned int end);
	void _getInput();
	void _draw();
	void _gameLogic();
	void _storePlayerData();
	bool _readPlayerData();
public:
	void StartGame();
	GameManager();
	~GameManager();
};


class Snake
{
private:
	uint32 _X,_Y;
	static uint32 _tailLength;
	vector<uint32>_tailPos_X;
	vector<uint32>_tailPos_Y;
	
public:
	inline void setPos(uint32 x, uint32 y);
	inline void setX(uint32 x);
	inline void setY(uint32 y);
	inline uint32 getX();
	inline uint32 getY();	

	inline uint32 getTailLength();
	inline void incrementTail();
	inline void setTailSize(uint32 size);
	bool isTailExistIn(uint32 x_pos, uint32 y_pos);
	void moveTailWithHead();
	void clearMemory();


};

