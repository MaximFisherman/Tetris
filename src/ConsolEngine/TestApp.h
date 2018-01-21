// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include "BaseApp.h"
#include <vector>
#include <random>
#include <iostream>
#include <stdlib.h>     
#include <time.h>       

enum TypeFigure {PIRAMIDA, LINE, RIGHT_G, LEFT_G, RECTANGLE, LEFT_Z, RIGHT_Z};

class TestApp : public BaseApp
{
	typedef BaseApp Parent;

	private:
		TypeFigure typeFigure;
		vector<int> Field;
		
		int countRotate;
		int countDelayIteration; 
		bool FlagGetNewFigure;

	public:
		TestApp();
		void PanelViewFigure();

		void CheckLine();
		void DrawField();
		void GenereteFigure();
		void MoveForIteration();

		void Rotate(int countRotate);
		void MoveLeft();
		void MoveRight();
		void MoveDown();
		void MoveFastDown();

		void setFigureOnField();
		
		int getRandomNumber();
		void Clear();


		virtual void KeyPressed(int btnCode);
		virtual void UpdateF(float deltaTime);
};
