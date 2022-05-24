/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	field({ fieldX, fieldY })
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (field.CheckGameLost())
	{
		field.SetAdjacentBombs();
		while (!wnd.mouse.IsEmpty())
		{
			const Mouse::Event e = wnd.mouse.Read();
			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				//Change State of Tile
				field.HandleLeftClick(e.GetPos());
			}
			else if (e.GetType() == Mouse::Event::Type::RPress)
			{
				field.HandleRightClick(e.GetPos());
			}
		}
	}
}

void Game::ComposeFrame()
{
	field.DrawField(gfx);
	if (field.CheckGameWon())
	{
		SpriteCodex::DrawWinFace(ResultPos, gfx);
	}
	else if (!field.CheckGameLost())
	{
		//field.RevealEverything(!field.CheckGameLost());
		SpriteCodex::DrawLoseFace(ResultPos, gfx);
	}
	else
	{
		SpriteCodex::DrawNormalFace(ResultPos, gfx);
	}
}
