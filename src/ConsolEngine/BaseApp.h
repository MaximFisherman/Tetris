// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

class BaseApp
{
private:
	HANDLE mConsole;
	HANDLE mConsoleIn;

	CHAR_INFO* mChiBuffer;
	COORD mDwBufferSize;
	COORD mDwBufferCoord;
	SMALL_RECT mLpWriteRegion;

	void Render();

public:
	//размеры области вывода по горизонтали и вертикали в символах
	const int X_SIZE;
	const int Y_SIZE;

	//аргументами €вл€ютс€ размеры области вывода по горизонтали и вертикали в символах
	BaseApp(int xSize=100, int ySize=80);
	virtual ~BaseApp();

	//запускает игровой цикл
	void Run();

	//можно заполнить x,y-символ экрана определенным символом, или считать его
	void SetChar(int x, int y, wchar_t c);
	wchar_t GetChar(int x, int y);

	/*эта функци€ вызываетс€ каждую игровую итерацию, еЄ можно переопределить, в наследнике класса.
	в неЄ приходит deltaTime - разница во времени между предыдущей итерацией и этой, в секундах*/
	virtual void UpdateF (float deltaTime){}
	/*эта функци€ вызываетс€ при нажатии клавиши на клавиатуре, в неЄ приходит код клавиши - btnCode.
	если использовать стрелки или функциональные клавиши, то придет общее дл€ них число, например 224, а следующее за ним - 
	будет уже непосредственно код самой клавиши, его можно получить, вызвав метод getch().
	ћетод KeyPressed так же можно переопределить в наследнике*/
	virtual void KeyPressed (int btnCode){}
};