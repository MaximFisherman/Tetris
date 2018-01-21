// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include "TestApp.h"
#include "VectorOperation.h"

#define SIZE_FIELD_Y 25
#define SIZE_FIELD_X 10

TestApp::TestApp() : Parent(SIZE_FIELD_X + 10, SIZE_FIELD_Y)
{
	FlagGetNewFigure = false;
	countRotate = 0;
	countDelayIteration = 0;

	/* Initialization field */
	for(int i = 0; i < SIZE_FIELD_X * SIZE_FIELD_Y; i++)		
		Field.push_back(0);

	typeFigure = static_cast<TypeFigure>(getRandomNumber());
	GenereteFigure();
}

void TestApp::DrawField()
{
	for(int i = 0; i < SIZE_FIELD_X; i++)
	{
		for(int j = 0; j < SIZE_FIELD_Y; j++)
		{
			if(Field[(j) * SIZE_FIELD_X + i] == 0)
				SetChar(i, j, L'*');

			if(Field[(j) * SIZE_FIELD_X + i] == 2 || Field[(j) * SIZE_FIELD_X + i] == 1)
				SetChar(i, j, L'O');
		}
	}
	PanelViewFigure();
}


int TestApp::getRandomNumber()
{
	srand (time(NULL));
	int randomNumber;

	/* generate number between 1 and 7: */
	randomNumber = rand() % 7 + 0;

	return randomNumber;
}

void TestApp::UpdateF(float deltaTime)  /*Virtual function from BaseApp */
{
	if(countDelayIteration > 10)
	{ 
		MoveForIteration();
		countDelayIteration = 0;
	}
	
	CheckLine();
	DrawField();

	if(FlagGetNewFigure == true)
	{
		setFigureOnField(); /* Set last figure on field */

		typeFigure = static_cast<TypeFigure>(getRandomNumber()); /* Set new figure on field */
		GenereteFigure();

		FlagGetNewFigure = false;
		countRotate = 0;
	}
	countDelayIteration++;
}

void TestApp::CheckLine()
{
	bool deleteLine = false;
	bool flagDefeat = false; 

	vector<int> numberLine;
	for(int i = 0; i < SIZE_FIELD_Y; i++)
	{
		int countElement = 0; /* Count element on line */
		for(int j = 0; j < SIZE_FIELD_X ; j++)
		{
			if(Field[(i) * SIZE_FIELD_X + j] == 1)
				countElement++;
			
			if(Field[(i) * SIZE_FIELD_X + j] == 1 && i == 2)
				flagDefeat = true;

			if(countElement == SIZE_FIELD_X)
			{
				deleteLine = true;
				numberLine.push_back(i);
			}
		}
	}

	/* User lose, clear field and get new figure for new round*/
	if(flagDefeat == true)
	{
		Clear();
		FlagGetNewFigure = false;
		countRotate = 0;
		countDelayIteration = 0;
	
		/* Initialization field */
		for(int i = 0; i < SIZE_FIELD_X * SIZE_FIELD_Y; i++)		
			Field.push_back(0);

		typeFigure = static_cast<TypeFigure>(getRandomNumber());
		GenereteFigure();
	}

	if(deleteLine == true && numberLine.empty() == false )
	{
		std::vector<int>::iterator result_max = vect::max_element(numberLine.begin(), numberLine.end());
		std::vector<int>::iterator result_min = vect::min_element(numberLine.begin(), numberLine.end());

			for(int i = 0; i < SIZE_FIELD_X; i++)
			{
				for(int j = *result_min; j <= *result_max; j++)
				{
					Field[(j) * SIZE_FIELD_X + i] = 0;
				}
			}
			
			/* Redraw field */
			vector<int> pointsX;
			vector<int> pointsY;
			for(int i = 0; i < SIZE_FIELD_X; i++)
			{
				for(int j = 0; j < *result_max ; j++)
				{
					if(Field[(j) * SIZE_FIELD_X + i] == 1)
					{
						pointsX.push_back(i);
						pointsY.push_back(j + (numberLine.size()));
						Field[(j) * SIZE_FIELD_X + i] = 0;
					}
					
				}
			}

			for(int i = 0; i < pointsX.size(); i++)
			{
				Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] = 1;
			}
	}
}

void TestApp::setFigureOnField()
{
	for(int i = 0; i < SIZE_FIELD_X; i++)
		for(int j = 0; j < SIZE_FIELD_Y; j++)
		{
			if(Field[(j) * SIZE_FIELD_X + i] == 2)
				Field[(j) * SIZE_FIELD_X + i] = 1;
		}
}

void TestApp::Clear()
{
	for(int i = 0; i < SIZE_FIELD_X; i++)
	{
		for(int j = 0; j < SIZE_FIELD_Y; j++)
		{
			Field[(j) * SIZE_FIELD_X + i] = 0;
		}
	}
}

void TestApp::MoveForIteration()
{
			/* Find movement elements */
			vector<int> pointsY;
			vector<int> pointsX;
			for(int i = 0; i < SIZE_FIELD_X; i++)
			{
				for(int j = 0; j < SIZE_FIELD_Y; j++)
				{
					if(Field[(j) * SIZE_FIELD_X + i] == 2 )
					{
						pointsY.push_back(j);
						pointsX.push_back(i);
					}
				}
			}

			if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
			{
				bool flagReDraw = true; 
				for(int i = pointsY.size() - 1; i >= 0 ; i-- )
				{
					/* Check figure in bottom board */
					if(pointsY[i] == SIZE_FIELD_Y - 1 || Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 1)
					{
						flagReDraw = false;
						break;
					}
				}

				if(flagReDraw == true )
				{
					for(int i = pointsY.size() - 1; i >= 0 ; i-- )
					{
						Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] = 0;
					}
				}
				
				bool flagDrawFigure = true;
				for(int i = 0; i < pointsY.size(); i++ )
				{	
					/* Check figure in bottom board */
					if(pointsY[i] + 1 == SIZE_FIELD_Y || Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 1)
					{
						flagDrawFigure = false;
						FlagGetNewFigure = true;
						break;
					}
				}
				
				/* Set Figure on field */
				if(flagDrawFigure == true )
				{
					for(int i = 0; i < pointsY.size(); i++ )
					{	
						Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] = 2;
					}
				}
			}
}

void TestApp::KeyPressed(int btnCode) /*Virtual function from BaseApp */
{
	if (btnCode == 119) //w 
	{
		MoveFastDown();
	}
	else if (btnCode == 115) //s
	{
		MoveDown();
	}
	else if (btnCode == 97) //a
	{
		MoveLeft();
	}
	else if (btnCode == 100) //d
	{
		MoveRight();
	}
	else if (btnCode == 32) //Space
	{
		Rotate(countRotate);
		if(countRotate == 3 && (typeFigure == PIRAMIDA || typeFigure == RIGHT_G || typeFigure == LEFT_G))/* Stop rotate, that figure have only fourth position */
			countRotate = -1;

		if(countRotate == 1 && (typeFigure == LINE || typeFigure == RIGHT_Z || typeFigure == LEFT_Z)) /* Stop rotate, that figure have only two position */
			countRotate = -1; 

		countRotate++;
	}
}

void TestApp::MoveDown()
{
			/* Find movement elements */
			vector<int> pointsY;
			vector<int> pointsX;
			for(int i = 0; i < SIZE_FIELD_X; i++)
			{
				for(int j = 0; j < SIZE_FIELD_Y; j++)
				{
					if(Field[(j) * SIZE_FIELD_X + i] == 2 )
					{
						pointsY.push_back(j);
						pointsX.push_back(i);
					}
				}
			}

			if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
			{
					//bool flagStopLoop = false;
					bool flagReDraw = true; 
					for(int i = pointsY.size() - 1; i >= 0 ; i-- )
					{
						/* Check figure in bottom board */
						if(pointsY[i] == SIZE_FIELD_Y - 1 || Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 1)
						{
							flagReDraw = false;
							break;
						}
					}
					
					//if(flagReDraw == false) break; /* Stop redrawing figure */
					if(flagReDraw == true )
					{
						for(int i = pointsY.size() - 1; i >= 0 ; i-- )
						{
							Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] = 0;
						}
					}
					
					bool flagDrawFigure = true;
					for(int i = 0; i < pointsY.size(); i++ )
					{	
						/* Check figure in bottom board */
						if(pointsY[i] + 1 == SIZE_FIELD_Y || Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 1)
						{
							flagDrawFigure = false;
							FlagGetNewFigure = true;
							break;
						}
					}
					
					/* Set Figure on field */
					if(flagDrawFigure == true )
					{
						for(int i = 0; i < pointsY.size(); i++ )
						{	
							Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] = 2;
						}
					}
			}
}

void TestApp::MoveLeft()
{
			/* Find movement elements */
			vector<int> pointsY;
			vector<int> pointsX;
			for(int i = 0; i < SIZE_FIELD_X; i++)
			{
				for(int j = 0; j < SIZE_FIELD_Y; j++)
				{
					if(Field[(j) * SIZE_FIELD_X + i] == 2 )
					{
						pointsY.push_back(j);
						pointsX.push_back(i);
					}
				}
			}

			if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
			{ 
				bool flagReDraw = true;
				for(int i = pointsX.size() - 1; i >= 0 ; i-- )
				{
					if(pointsX[i] == 0 || Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 1) 
						flagReDraw = false;
				}

				if(flagReDraw == true)
				{
					for(int i = pointsX.size() - 1; i >= 0 ; i-- )
					{
							Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] = 0;
					}

					for(int i = 0; i < pointsX.size(); i++ )
					{	
							Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] = 2;
					}
				}
			}
}

void TestApp::MoveRight()
{
			/* Find movement elements */
			vector<int> pointsY;
			vector<int> pointsX;
			for(int i = 0; i < SIZE_FIELD_X; i++)
			{
				for(int j = 0; j < SIZE_FIELD_Y; j++)
				{
					if(Field[(j) * SIZE_FIELD_X + i] == 2 )
					{
						pointsY.push_back(j);
						pointsX.push_back(i);
					}
				}
			}

			if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
			{ 
				bool flagReDraw = true;
				for(int i = pointsX.size() - 1; i >= 0 ; i-- )
				{
					if(pointsX[i] == SIZE_FIELD_X - 1|| Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 1) 
						flagReDraw = false;
				}

				if(flagReDraw == true)
				{
					for(int i = pointsY.size() - 1; i >= 0 ; i-- )
					{
							Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] = 0;
					}

					for(int i = 0; i < pointsY.size(); i++ )
					{	
							Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] = 2;
					}
				}
			}
}


void TestApp::MoveFastDown()
{
	for(int i = 0; i < SIZE_FIELD_Y; i++)
	{
			/* Find movement elements */
			vector<int> pointsY;
			vector<int> pointsX;
			for(int i = 0; i < SIZE_FIELD_X; i++)
			{
				for(int j = 0; j < SIZE_FIELD_Y; j++)
				{
					if(Field[(j) * SIZE_FIELD_X + i] == 2 )
					{
						pointsY.push_back(j);
						pointsX.push_back(i);
					}
				}
			}

			if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
			{
					//bool flagStopLoop = false;
					bool flagReDraw = true; 
					for(int i = pointsY.size() - 1; i >= 0 ; i-- )
					{
						/* Check figure in bottom board */
						if(pointsY[i] == SIZE_FIELD_Y - 1 || Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 1)
						{
							flagReDraw = false;
							break;
						}
					}
					
					//if(flagReDraw == false) break; /* Stop redrawing figure */
					if(flagReDraw == true )
					{
						for(int i = pointsY.size() - 1; i >= 0 ; i-- )
						{
							Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] = 0;
						}
					}
					
					bool flagDrawFigure = true;
					for(int i = 0; i < pointsY.size(); i++ )
					{	
						/* Check figure in bottom board */
						if(pointsY[i] + 1 == SIZE_FIELD_Y || Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 1)
						{
							flagDrawFigure = false;
							FlagGetNewFigure = true;
							break;
						}
					}
					
					/* Set Figure on field */
					if(flagDrawFigure == true )
					{
						for(int i = 0; i < pointsY.size(); i++ )
						{	
							Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] = 2;
						}
					}
			}
	}
}

void TestApp::GenereteFigure()
{
	switch (typeFigure)
	{
		case PIRAMIDA:
				Field[(1) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 4] = 2;
				Field[(2) * SIZE_FIELD_X + 6] = 2;
				break;

		case LINE:
				Field[(1) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 5] = 2;
				Field[(3) * SIZE_FIELD_X + 5] = 2;
				Field[(4) * SIZE_FIELD_X + 5] = 2;
				break;
		
		case RIGHT_G:
				Field[(1) * SIZE_FIELD_X + 6] = 2;
				Field[(1) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 5] = 2;
				Field[(3) * SIZE_FIELD_X + 5] = 2;
				break;
		
		case LEFT_G:
				Field[(1) * SIZE_FIELD_X + 4] = 2;
				Field[(1) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 5] = 2;
				Field[(3) * SIZE_FIELD_X + 5] = 2;
				break;
		
		case RECTANGLE:
				Field[(1) * SIZE_FIELD_X + 4] = 2;
				Field[(1) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 4] = 2;
				break;

		case LEFT_Z:
				Field[(1) * SIZE_FIELD_X + 4] = 2;
				Field[(1) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 6] = 2;
				break;

		case RIGHT_Z:
				Field[(1) * SIZE_FIELD_X + 6] = 2;
				Field[(1) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 5] = 2;
				Field[(2) * SIZE_FIELD_X + 4] = 2;
				break;
	}
	
}

void TestApp::Rotate(int countRotate)
{
	switch(typeFigure)
	{
		case PIRAMIDA:
			{
				vector<int> pointsY;
				vector<int> pointsX;

				for(int i = 0; i < SIZE_FIELD_X; i++)
				{
					for(int j = 0; j < SIZE_FIELD_Y; j++)
					{
						if(Field[(j) * SIZE_FIELD_X + i] == 2 )
						{
							pointsY.push_back(j);
							pointsX.push_back(i);
						}
					}
				} 
				if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
				{
					bool flagReDraw = true; 
					for(int i = pointsY.size() - 1; i >= 0 ; i-- )
					{
					
						if(pointsY[i] == SIZE_FIELD_Y - 1 || Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] == 1)
						{
							flagReDraw = false;
							break;
						}
					}

					if(flagReDraw == true )
					{
						vector<int> pointsYNew;
						vector<int> pointsXNew;
						for(int i = pointsY.size() - 1; i >= 0 ; i-- )
						{
							if(countRotate == 0)
							{
								if ( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
											Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2)
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] - 1);
									Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] = 0;
								}else 
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 1)
							{
								if( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2)
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] + 1);
									Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] = 0;
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 2)
							{
								if( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i] + 1] == 2 && 
									Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] + 1);
									Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] = 0;
								} else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 3)
							{
								if( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] - 1);
									Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i]] = 0;
								} else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}
						}
						
						for(int i = 0; i < pointsYNew.size(); i++)
						{
							Field[(pointsYNew[i]) * SIZE_FIELD_X + pointsXNew[i]] = 2;
						}
					}
				}
			break;
			}

		case LINE:
			{
				vector<int> pointsY;
				vector<int> pointsX;

				for(int i = 0; i < SIZE_FIELD_X; i++)
				{
					for(int j = 0; j < SIZE_FIELD_Y; j++)
					{
						if(Field[(j) * SIZE_FIELD_X + i] == 2 )
						{
							pointsY.push_back(j);
							pointsX.push_back(i);
						}
					}
				} 
				if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
				{
					bool flagReDraw = true; 
					for(int i = 0; i < pointsY.size() ; i++ )
					{
					
						if(pointsY[i] == SIZE_FIELD_Y - 1 || 
							Field[(pointsY[i] + 2) * SIZE_FIELD_X + pointsX[i]] == 1 || 
							pointsX[i] - 2 == 0 || 
							pointsX[i] + 2 == SIZE_FIELD_X )
						{
							flagReDraw = false;
							break;
						}
					}

					if(flagReDraw == true )
					{
						vector<int> pointsYNew;
						vector<int> pointsXNew;
						vector<int> pointsYClearOld;
						vector<int> pointsXClearOld;
						for(int i = 0; i < pointsY.size() ; i++ )
						{
							if(countRotate == 0)
							{
								if ( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									 Field[(pointsY[i] - 2) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									 Field[(pointsY[i] - 3) * SIZE_FIELD_X + pointsX[i]] == 2)
								{
									pointsYNew.push_back(pointsY[i] - 2);
									pointsXNew.push_back(pointsX[i] + 2);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i] - 2) * SIZE_FIELD_X + pointsX[i]] == 2) 
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] + 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i] + 2) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i] + 3) * SIZE_FIELD_X + pointsX[i]] == 2) 
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] - 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 1)
							{
								if( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 2] == 2 && 
									Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2 &&
									Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] - 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								} else if ( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2 &&
										    Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 2] == 2 &&
											Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 3] == 2)
								{
									pointsYNew.push_back(pointsY[i] + 2);
									pointsXNew.push_back(pointsX[i] - 2);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								} else if ( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2 &&
											Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 2] == 2 &&
											Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 3] == 2)
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] + 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}
						}

						/* Clear old figure */
						for(int i = 0; i < pointsYClearOld.size(); i++)
						{
							Field[(pointsYClearOld[i]) * SIZE_FIELD_X + pointsXClearOld[i]] = 0;
						}

						for(int i = 0; i < pointsYNew.size(); i++)
						{
							Field[(pointsYNew[i]) * SIZE_FIELD_X + pointsXNew[i]] = 2;
						}
					}
				}
				break;
			}
		
		case RIGHT_G:
			{
				vector<int> pointsY;
				vector<int> pointsX;

				for(int i = 0; i < SIZE_FIELD_X; i++)
				{
					for(int j = 0; j < SIZE_FIELD_Y; j++)
					{
						if(Field[(j) * SIZE_FIELD_X + i] == 2 )
						{
							pointsY.push_back(j);
							pointsX.push_back(i);
						}
					}
				} 
				if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
				{
					bool flagReDraw = true; 
					for(int i = 0; i < pointsY.size() ; i++ )
					{
					
						if(pointsY[i] == SIZE_FIELD_Y - 1 || 
							Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 1 || 
							pointsX[i] - 1 == 0 || 
							pointsX[i] + 1 == SIZE_FIELD_X )
						{
							flagReDraw = false;
							break;
						}
					}

					if(flagReDraw == true )
					{
						vector<int> pointsYNew;
						vector<int> pointsXNew;
						vector<int> pointsYClearOld;
						vector<int> pointsXClearOld;
						for(int i = 0; i < pointsY.size() ; i++ )
						{
							if(countRotate == 0)
							{
								if ( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									 Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i] - 1] == 2)
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i] - 2);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2 && 
									      Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2) 
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] - 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i] - 2) * SIZE_FIELD_X + pointsX[i]] == 2) 
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] + 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 1)
							{
								if ( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									 Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i] + 2);
									pointsXNew.push_back(pointsX[i]);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2) 
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] + 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 2] == 2) 
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] - 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 2)
							{
								if ( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] + 1] == 2 && 
									 Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i] + 2);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2) 
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] + 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i] + 2) * SIZE_FIELD_X + pointsX[i]] == 2) 
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] - 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 3)
							{
								if ( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									 Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2)
								{
									pointsYNew.push_back(pointsY[i] - 2);
									pointsXNew.push_back(pointsX[i]);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2) 
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] - 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 2] == 2) 
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] + 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}
						}
						
						/* Clear old figure */
						for(int i = 0; i < pointsYClearOld.size(); i++)
						{
							Field[(pointsYClearOld[i]) * SIZE_FIELD_X + pointsXClearOld[i]] = 0;
						}

						for(int i = 0; i < pointsYNew.size(); i++)
						{
							Field[(pointsYNew[i]) * SIZE_FIELD_X + pointsXNew[i]] = 2;
						}
					}
				}
				break;
			}
			
		
		case LEFT_G:
				{
				vector<int> pointsY;
				vector<int> pointsX;

				for(int i = 0; i < SIZE_FIELD_X; i++)
				{
					for(int j = 0; j < SIZE_FIELD_Y; j++)
					{
						if(Field[(j) * SIZE_FIELD_X + i] == 2 )
						{
							pointsY.push_back(j);
							pointsX.push_back(i);
						}
					}
				} 
				if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
				{
					bool flagReDraw = true; 
					for(int i = 0; i < pointsY.size() ; i++ )
					{
					
						if(pointsY[i] == SIZE_FIELD_Y - 1 || 
							Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 1 || 
							pointsX[i] - 1 == 0 || 
							pointsX[i] + 1 == SIZE_FIELD_X )
						{
							flagReDraw = false;
							break;
						}
					}

					if(flagReDraw == true )
					{
						vector<int> pointsYNew;
						vector<int> pointsXNew;
						vector<int> pointsYClearOld;
						vector<int> pointsXClearOld;
						for(int i = 0; i < pointsY.size() ; i++ )
						{
							if(countRotate == 0)
							{
								if ( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2 && 
									 Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i] + 2);
									pointsXNew.push_back(pointsX[i]);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									      Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2) 
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] - 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i] - 2) * SIZE_FIELD_X + pointsX[i]] == 2) 
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] + 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 1)
							{
								if ( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									 Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i] + 2);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2) 
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] + 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 2] == 2) 
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] - 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 2)
							{
								if ( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									 Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2)
								{
									pointsYNew.push_back(pointsY[i] - 2);
									pointsXNew.push_back(pointsX[i]);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2) 
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] + 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i] + 2) * SIZE_FIELD_X + pointsX[i]] == 2) 
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] - 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 3)
							{
								if ( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									 Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2)
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i] - 2);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2) 
								{
									pointsYNew.push_back(pointsY[i] - 1);
									pointsXNew.push_back(pointsX[i] - 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 2] == 2) 
								{
									pointsYNew.push_back(pointsY[i] + 1);
									pointsXNew.push_back(pointsX[i] + 1);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}
						}
						
						/* Clear old figure */
						for(int i = 0; i < pointsYClearOld.size(); i++)
						{
							Field[(pointsYClearOld[i]) * SIZE_FIELD_X + pointsXClearOld[i]] = 0;
						}

						for(int i = 0; i < pointsYNew.size(); i++)
						{
							Field[(pointsYNew[i]) * SIZE_FIELD_X + pointsXNew[i]] = 2;
						}
					}
				}
				break;
			}
		
		case LEFT_Z:
				{
				vector<int> pointsY;
				vector<int> pointsX;

				for(int i = 0; i < SIZE_FIELD_X; i++)
				{
					for(int j = 0; j < SIZE_FIELD_Y; j++)
					{
						if(Field[(j) * SIZE_FIELD_X + i] == 2 )
						{
							pointsY.push_back(j);
							pointsX.push_back(i);
						}
					}
				} 
				if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
				{
					bool flagReDraw = true; 
					for(int i = 0; i < pointsY.size() ; i++ )
					{
					
						if(pointsY[i] == SIZE_FIELD_Y - 1 || 
							Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 1 || 
							pointsX[i] - 1 == 0 || 
							pointsX[i] + 1 == SIZE_FIELD_X )
						{
							flagReDraw = false;
							break;
						}
					}

					if(flagReDraw == true )
					{
						vector<int> pointsYNew;
						vector<int> pointsXNew;
						vector<int> pointsYClearOld;
						vector<int> pointsXClearOld;
						for(int i = 0; i < pointsY.size() ; i++ )
						{
							if(countRotate == 0)
							{
								if ( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i] + 1] == 2 && 
									 Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i] + 2);
									pointsXNew.push_back(pointsX[i]);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2) 
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i] - 2);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 1)
							{
								if( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2 && 
									Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i] - 2);
									pointsXNew.push_back(pointsX[i]);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								} else if ( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2 &&
										    Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2 &&
											Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i] + 2);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								} else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}
						}
						
						/* Clear old figure */
						for(int i = 0; i < pointsYClearOld.size(); i++)
						{
							Field[(pointsYClearOld[i]) * SIZE_FIELD_X + pointsXClearOld[i]] = 0;
						}

						for(int i = 0; i < pointsYNew.size(); i++)
						{
							Field[(pointsYNew[i]) * SIZE_FIELD_X + pointsXNew[i]] = 2;
						}
					}
				}
				break;
			}

		case RIGHT_Z:
			{
				vector<int> pointsY;
				vector<int> pointsX;

				for(int i = 0; i < SIZE_FIELD_X; i++)
				{
					for(int j = 0; j < SIZE_FIELD_Y; j++)
					{
						if(Field[(j) * SIZE_FIELD_X + i] == 2 )
						{
							pointsY.push_back(j);
							pointsX.push_back(i);
						}
					}
				} 
				if((pointsX.empty() == false && pointsY.empty() == false) && FlagGetNewFigure == false)
				{
					bool flagReDraw = true; 
					for(int i = 0; i < pointsY.size() ; i++ )
					{
					
						if(pointsY[i] == SIZE_FIELD_Y - 1 || 
							Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 1 || 
							pointsX[i] + 1 == 0 || 
							pointsX[i] + 1 == SIZE_FIELD_X )
						{
							flagReDraw = false;
							break;
						}
					}

					if(flagReDraw == true )
					{
						vector<int> pointsYNew;
						vector<int> pointsXNew;
						vector<int> pointsYClearOld;
						vector<int> pointsXClearOld;
						for(int i = 0; i < pointsY.size() ; i++ )
						{
							if(countRotate == 0)
							{
								if ( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] + 1] == 2 && 
									 Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] + 1] == 2)
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i] + 2);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else if( Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									      Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2) 
								{
									pointsYNew.push_back(pointsY[i] + 2);
									pointsXNew.push_back(pointsX[i]);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								}else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}

							if(countRotate == 1)
							{
								if( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									Field[(pointsY[i]) * SIZE_FIELD_X + pointsX[i] - 1] == 2 && 
									Field[(pointsY[i] + 1) * SIZE_FIELD_X + pointsX[i]] == 2)
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i] - 2);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								} else if ( Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i] - 1] == 2 &&
										    Field[(pointsY[i] - 1) * SIZE_FIELD_X + pointsX[i]] == 2)
								{
									pointsYNew.push_back(pointsY[i] - 2);
									pointsXNew.push_back(pointsX[i]);
									pointsYClearOld.push_back(pointsY[i]);
									pointsXClearOld.push_back(pointsX[i]);
								} else
								{
									pointsYNew.push_back(pointsY[i]);
									pointsXNew.push_back(pointsX[i]);
								}
							}
						}
						
						/* Clear old figure */
						for(int i = 0; i < pointsYClearOld.size(); i++)
						{
							Field[(pointsYClearOld[i]) * SIZE_FIELD_X + pointsXClearOld[i]] = 0;
						}

						for(int i = 0; i < pointsYNew.size(); i++)
						{
							Field[(pointsYNew[i]) * SIZE_FIELD_X + pointsXNew[i]] = 2;
						}
					}
				}
				break;
			}
	}
}

void TestApp::PanelViewFigure()
{
	for(int i = 0; i < 7; i++)
	{
		SetChar((SIZE_FIELD_X + 2) + i, 0, L'#');
		SetChar((SIZE_FIELD_X + 2) + i, 1, L'*');
		SetChar((SIZE_FIELD_X + 2) + i, 2, L'*');
		SetChar((SIZE_FIELD_X + 2) + i, 3, L'*');
		SetChar((SIZE_FIELD_X + 2) + i, 4, L'*');
		SetChar((SIZE_FIELD_X + 2) + i, 5, L'#');
	}
	for(int i = 0; i < 6; i++)
	{
		SetChar((SIZE_FIELD_X + 1), i, L'#');
		SetChar((SIZE_FIELD_X + 8), i, L'#');
	}

	switch(typeFigure)
	{
		case PIRAMIDA: 
				SetChar((SIZE_FIELD_X + 4), 2, L'0');
				SetChar((SIZE_FIELD_X + 4), 3, L'0');
				SetChar((SIZE_FIELD_X + 3), 3, L'0');
				SetChar((SIZE_FIELD_X + 5), 3, L'0');
				break;
		case LINE:
				SetChar((SIZE_FIELD_X + 3), 2, L'0');
				SetChar((SIZE_FIELD_X + 4), 2, L'0');
				SetChar((SIZE_FIELD_X + 5), 2, L'0');
				SetChar((SIZE_FIELD_X + 6), 2, L'0');
				break;
		
		case RIGHT_G:
				SetChar((SIZE_FIELD_X + 3), 2, L'0');
				SetChar((SIZE_FIELD_X + 3), 3, L'0');
				SetChar((SIZE_FIELD_X + 4), 3, L'0');
				SetChar((SIZE_FIELD_X + 5), 3, L'0');
				break;
		
		case LEFT_G:
				SetChar((SIZE_FIELD_X + 3), 3, L'0');
				SetChar((SIZE_FIELD_X + 3), 2, L'0');
				SetChar((SIZE_FIELD_X + 4), 2, L'0');
				SetChar((SIZE_FIELD_X + 5), 2, L'0');
				break;
		
		case RECTANGLE:
				SetChar((SIZE_FIELD_X + 5), 2, L'0');
				SetChar((SIZE_FIELD_X + 4), 2, L'0');
				SetChar((SIZE_FIELD_X + 5), 3, L'0');
				SetChar((SIZE_FIELD_X + 4), 3, L'0');
				break;

		case LEFT_Z:
				SetChar((SIZE_FIELD_X + 3), 2, L'0');
				SetChar((SIZE_FIELD_X + 4), 2, L'0');
				SetChar((SIZE_FIELD_X + 4), 3, L'0');
				SetChar((SIZE_FIELD_X + 5), 3, L'0');
				break;

		case RIGHT_Z:
				SetChar((SIZE_FIELD_X + 6), 2, L'0');
				SetChar((SIZE_FIELD_X + 5), 2, L'0');
				SetChar((SIZE_FIELD_X + 5), 3, L'0');
				SetChar((SIZE_FIELD_X + 4), 3, L'0');
				break;
	}
}
