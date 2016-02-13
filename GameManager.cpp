#include "GameManager.h"
using namespace std;


inline unsigned int GameManager::getRandom(unsigned int start, unsigned int end)
{
	srand(time(0)); // This will ensure a really randomized number by help of time.
	return (rand() % end + start); // Randomizing the number between start-end
}

GameManager::GameManager()
{
	_snake = new Snake;
	_height = _width = 20;
	_fruitX = getRandom(6, 10);
	_fruitY = getRandom(7, 14);
	_nextDir = STOP;
	_score = 0;
	_gameOver = false;
	_snake->setPos(_width / 2, _height / 2);
	_snake->setTailSize(3);//inilial value
}

GameManager::~GameManager()
{
	delete _snake;
}

#ifdef WINDOWS_OS
void GameManager::_getInput()
{
	if (_kbhit())
	{
		switch (_getch())
		{
			//lower case
		case 'a':
			_nextDir = LEFT;
			break;
		case 'w':
			_nextDir = UP;
			break;
		case 's':
			_nextDir = DOWN;
			break;
		case 'd':
			_nextDir = RIGHT;
			break;
			//upper case
		case 'A':
			_nextDir = LEFT;
			break;
		case 'W':
			_nextDir = UP;
			break;
		case 'S':
			_nextDir = DOWN;
			break;
		case 'D':
			_nextDir = RIGHT;
			break;
		case 27: //ESC key
			_gameOver = true;
			break;
		default: //Probably the arrows keys
				 //Arrow keys on windows have 2 ascii as they are special keys 224 for releasing
				 //a special event key and the other value for key.
				 //won't work on linux as it has 3 values ! and neither getch nor conio lib is supported 
			switch (_getch())
			{
			case 75:
				_nextDir = LEFT;
				break;
			case 72:
				_nextDir = UP;
				break;
			case 80:
				_nextDir = DOWN;
				break;
			case 77:
				_nextDir = RIGHT;
				break;
			default:
				break;
			}
			break;
		}
	}

}
#endif

#ifdef LINUX_OS
void GameManager::_getInput()
{
	switch (_linux_getch())
	{
		//lower case
	case 'a':
		_nextDir = LEFT;
		break;
	case 'w':
		_nextDir = UP;
		break;
	case 's':
		_nextDir = DOWN;
		break;
	case 'd':
		_nextDir = RIGHT;
		break;
		//upper case
	case 'A':
		_nextDir = LEFT;
		break;
	case 'W':
		_nextDir = UP;
		break;
	case 'S':
		_nextDir = DOWN;
		break;
	case 'D':
		_nextDir = RIGHT;
		break;
	case 27: //ESC key
		_gameOver = true;
		break;
	}
}
#endif
void GameManager::_draw()
{
	ClearScreen();
	uint32 x, y;
	bool printSpace;

	//first line
	for (x = 0; x < _width; x++)
		cout << "#";
	cout << endl;


	for (y = 0; y < _height; y++)
	{
		for (x = 0; x < _width; x++)
		{
			//Right and Left border for each line
			if ((x == 0) || (x == _width - 1)) cout << "#";
			//is the location of head of the snake?
			else if ((x == _snake->getX()) && (y == _snake->getY())) cout << "O";
			//is the location of fruit?
			else if ((x == _fruitX) && (y == _fruitY))cout << "F";
			//Draw remaining (spaces and the the snake tail)
			else
			{
				if (_snake->isTailExistIn(x, y)) cout << "o";
				else cout << " ";
			}
		}
		//next line
		cout << endl;
	}

	//last line
	for (x = 0; x < _width; x++)
		cout << "#";
	cout << endl;
	cout << endl << "Score: " << _score << endl;
}

void GameManager::_gameLogic()
{

	//Assign tail to new position
	_snake->moveTailWithHead();

	//snake next move
	int curr_x = _snake->getX();
	int curr_y = _snake->getY();
	switch (_nextDir)
	{
	case LEFT:
		--curr_x;
		_snake->setX(curr_x);
		break;
	case RIGHT:
		++curr_x;
		_snake->setX(curr_x);
		break;
	case UP:
		--curr_y;
		_snake->setY(curr_y);
		break;
	case DOWN:
		++curr_y;
		_snake->setY(curr_y);
		break;
	default:
		break;
	}

	//allow non collinding with borders and get across other side
	if (curr_x >= _width) _snake->setX(0);
	else if (curr_x <= 0) _snake->setX(_width - 1);
	if (curr_y >= _height) _snake->setY(0);
	else if (curr_y <= 0) _snake->setY(_height - 1);

	//is eating himself
	if (_snake->isTailExistIn(curr_x, curr_y)) _gameOver = true;

	//increase score when hit fruit
	if (curr_x == _fruitX && curr_y == _fruitY)
	{
		_score += 10;
		_fruitX = getRandom(3, _width - 4);
		Wait(0.01); //to get a different value for Random Y to X so Y!=X for a bigger range
		_fruitY = getRandom(2, _height - 5);
		_snake->incrementTail();
	}

}


string getUserDocumentPath()
{
#ifdef WINDOWS_OS
	TCHAR path[MAX_PATH];
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS, NULL,
		SHGFP_TYPE_CURRENT, path);
	// Your wchar_t*
	wchar_t* txt = path;
	wstring ws(txt);
	// your new String
	string str(ws.begin(), ws.end());
	return str;
#endif
}

void GameManager::_storePlayerData()
{
#ifdef WINDOWS_OS
	string file_location = getUserDocumentPath();
	file_location.append("\\SnakeGameScoreBoard.data");
	ofstream myfile(file_location, ios::out | ios_base::app);
	if (myfile.fail())
	{
		perror("error creating the file or writting to it !\n");
	}
	else
	{
		myfile << _playerName << " " << _score << endl;;
	}
	myfile.close();
#endif
}

bool GameManager::_readPlayerData()
{
#ifdef WINDOWS_OS
	string file_location = getUserDocumentPath();
	file_location.append("\\SnakeGameScoreBoard.data");
	ifstream myfile(file_location, ios::in);
	if (myfile.fail())
	{
		myfile.close();
		return false;
	}
	else
	{
		string line;
		while (getline(myfile, line))
			cout << "Old Score Was " << line << endl;
		myfile.close();
		return true;
	}
#endif
	return false;
}

void GameManager::StartGame()
{
	if (_readPlayerData() == false)
	{
		cout << "No Old Scores Found,let's make a one" << endl;
		cout << "Enter Your Name:";
		cin >> _playerName;
	}
	cout << "Press Any Key to Start The Game " << endl;
#ifdef WINDOWS_OS
	_getch();
#else
	_linux_getch();
#endif
	//start the game
	ClearScreen();
	while (!_gameOver)
	{
		_draw();
		_getInput();
		_gameLogic();
		Wait(0.1);
	}
	cout << "Game Over :(" << endl;
	_storePlayerData();
	_snake->clearMemory();

}

uint32 Snake::_tailLength = 0; //inilialize static varible
void Snake::setPos(uint32 x, uint32 y)
{
	_X = x;
	_Y = y;
}

inline void Snake::setX(uint32 x)
{
	_X = x;
}

inline void Snake::setY(uint32 y)
{
	_Y = y;
}

inline uint32 Snake::getX()
{
	return _X;
}

inline uint32 Snake::getY()
{
	return _Y;
}

inline uint32 Snake::getTailLength()
{
	return _tailLength;
}

inline void Snake::incrementTail()
{
	++_tailLength;
	_tailPos_X.resize(_tailLength);
	_tailPos_Y.resize(_tailLength);
}

inline void Snake::setTailSize(uint32 size)
{
	_tailPos_X.resize(size);
	_tailPos_Y.resize(size);
}

bool Snake::isTailExistIn(uint32 x_pos, uint32 y_pos)
{
	for (uint32 i = 0; i < _tailLength; i++)
		if (_tailPos_X.at(i) == x_pos && _tailPos_Y.at(i) == y_pos) return true;
	return false;
}

void Snake::moveTailWithHead()
{
	int prevX = _tailPos_X[0];
	int prevY = _tailPos_Y[0];
	int prev2X, prev2Y;
	_tailPos_X[0] = _X;
	_tailPos_Y[0] = _Y;
	for (int i = 1; i < _tailLength; i++)
	{
		prev2X = _tailPos_X[i];
		prev2Y = _tailPos_Y[i];
		_tailPos_X[i] = prevX;
		_tailPos_Y[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
}

void Snake::clearMemory()
{
	this->_tailPos_X.clear();
	this->_tailPos_Y.clear();
	this->_tailLength = 0;
	_tailPos_X.resize(3);
	_tailPos_Y.resize(3);
}

#ifdef LINUX_OS
char _linux_getch() {
	char buf = 0;
	struct termios old = { 0 };
	fflush(stdout);
	if (tcgetattr(0, &old)<0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old)<0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1)<0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old)<0)
		perror("tcsetattr ~ICANON");
	return buf;
}
#endif