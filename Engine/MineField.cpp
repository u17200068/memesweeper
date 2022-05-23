#include "MineField.h"
#include <assert.h>
#include <random>

MineField::Tile::Tile(State state, Vei2 pos, bool hasMeme)
{
	this->state = state;
	this->pos = pos;
	this->hasMeme = hasMeme;
}

Vei2 MineField::Tile::GetPos() const
{
	return pos;
}

bool MineField::Tile::HasMeme() const
{
	return hasMeme;
}

bool MineField::Tile::IsGameLost() const
{
	return GameLost;
}

void MineField::Tile::SetAdjacentBombs(int bombs)
{
	adjacentBombs = bombs;
}

int MineField::Tile::GetAdjacentBombs()
{
	return adjacentBombs;
}

void MineField::Tile::ChangeState(State newState)
{
	state = newState;
	if (state == Tile::State::Revealed && hasMeme)
	{
		GameLost = true;
	}
}


void MineField::Tile::AddMeme(bool newMeme)
{
	hasMeme = newMeme;
}

void MineField::Tile::DrawTile(Graphics& gfx)
{
	switch (state)
	{
	case Tile::State::Hidden:
		SpriteCodex::DrawTileButton(pos, gfx);
		break;
	case Tile::State::Flagged:
		SpriteCodex::DrawTileFlag(pos, gfx);
		break;
	case Tile::State::Revealed:
		if (hasMeme)
		{
			SpriteCodex::DrawTile0(pos, gfx);
			SpriteCodex::DrawTileBomb(pos, gfx);
		}
		else
		{
			SpriteCodex::DrawTile(pos, adjacentBombs ,gfx);
		}
		break;
	}
}

MineField::Tile::State MineField::Tile::GetState() const
{
	return state;
}

MineField::MineField(Vei2 pos_in)
	:
	pos(pos_in)
{
	const int StartX = pos.x;
	const int EndX = pos.x + (nDimensions * Tile::size);
	const int StartY = pos.y;
	const int EndY = pos.y + (nDimensions * Tile::size);
	int index = 0;
	for (int x = StartX; x < EndX; x+=16)
	{
		for (int y = StartY; y < EndY; y+=16)
		{
			// Add Hidden tiles with x, y positions and no meme.
			tiles[index] = { Tile::State::Hidden, {x,y}, false};
			index++;
		}
	}
	
	//Choose amount of memes in board by modifying the divisor
	const int numberOfMemes = (nDimensions * nDimensions) / 4;
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> rand_int(0, (nDimensions * nDimensions) - 1);
	int randomIndex = rand_int(rng);

	for (int i = 0; i < numberOfMemes; i++)
	{
		do
		{
			randomIndex = rand_int(rng);

		} while (tiles[randomIndex].HasMeme());

		tiles[randomIndex].AddMeme(true);
	}
}

void MineField::DrawField(Graphics& gfx)
{
	const int StartX = pos.x;
	const int EndX = pos.x + (nDimensions * Tile::size);
	const int StartY = pos.y;
	const int EndY = pos.y + (nDimensions * Tile::size);

	for (int x = StartX-50; x < EndX + 50; x++)
	{
		for (int y = StartY-50; y < EndY + 50; y++)
		{
			gfx.PutPixel(x, y, Colors::LightGray);
		}
	}
	
	for (int x = StartX; x < EndX; x++)
	{
		for (int y = StartY; y < EndY; y++)
		{
			gfx.PutPixel(x, y, Colors::Gray);
		}
	}
	for (int i=0; i < nDimensions*nDimensions; i++)
	{
		tiles[i].DrawTile(gfx);
	}
	for (int x = StartX - 50; x < EndX + 50; x++)
	{
		for (int y = StartY - 50; y < StartY - 50 + 3; y++)
		{
			gfx.PutPixel(x, y, Colors::White);
		}
	}
	for (int x = StartX - 50; x < EndX + 50; x++)
	{
		for (int y = EndY + 50; y > EndY + 50 - 3; y--)
		{
			gfx.PutPixel(x, y, Colors::White);
		}
	}
	for (int x = StartX - 50; x < StartX - 50 + 3; x++)
	{
		for (int y = StartY - 50; y < EndY + 50; y++)
		{
			gfx.PutPixel(x, y, Colors::White);
		}
	}
	for (int x = EndX + 50; x > EndX + 50 - 3; x--)
	{
		for (int y = StartY - 50; y < EndY + 50; y++)
		{
			gfx.PutPixel(x, y, Colors::White);
		}
	}
}

void MineField::HandleLeftClick(Vei2 mousePos)
{
	assert(mousePos.x >= pos.x && mousePos.x <= pos.x + (nDimensions * Tile::size) &&
		   mousePos.y >= pos.y && mousePos.y <= pos.y + (nDimensions * Tile::size));

	// Get the indeces of Virtual 2D array
	int x_index = (mousePos.x - pos.x) / nDimensions;
	int y_index = (mousePos.y - pos.y) / nDimensions;

	//Map 2D array to 1D index
	int index = (x_index * nDimensions) + y_index;
	if (tiles[index].GetState() == Tile::State::Hidden)
	{
		tiles[index].ChangeState(Tile::State::Revealed);
	}
}

void MineField::SetAdjacentBombs()
{
	for (int i = 0; i < nDimensions * nDimensions; i++)
	{
		int numOfBombs = 0;
		//Check Top left
		if (i == 0)
		{
			if (tiles[i + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions + 1].HasMeme())
			{
				numOfBombs++;
			}
		}
		//Check 1st column
		else if (i % 16 == 0 && i!=240)
		{
			if (tiles[i + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions + 1].HasMeme())
			{
				numOfBombs++;
			}
		}
		//Check Top Right
		if (i == 15)
		{
			if (tiles[i - 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions - 1].HasMeme())
			{
				numOfBombs++;
			}
		}
		//Check Last Column
		else if (i % 16 == 15 && i!=255)
		{
			if (tiles[i - 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions - 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions - 1].HasMeme())
			{
				numOfBombs++;
			}
		}
		//Check Bottom Left
		if(i==240)
		{
			if (tiles[i + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions + 1].HasMeme())
			{
				numOfBombs++;
			}
		}
		//Check Last Row
		else if (i > 240 && i < 255)
		{
			if (tiles[i + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions - 1].HasMeme())
			{
				numOfBombs++;
			}
		}
		//Check Bottom Right
		if (i == 255)
		{
			if (tiles[i - 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions - 1].HasMeme())
			{
				numOfBombs++;
			}
		}
		//Check First Row
		else if (i > 0 && i < 15)
		{
			if (tiles[i + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions - 1].HasMeme())
			{
				numOfBombs++;
			}
		}
		//Check Everything Else
		else if(i%16>=1 && i%16<=14 && i<=238)
		{
			if (tiles[i + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i + nDimensions - 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions + 1].HasMeme())
			{
				numOfBombs++;
			}
			if (tiles[i - nDimensions - 1].HasMeme())
			{
				numOfBombs++;
			}
		}
		tiles[i].SetAdjacentBombs(numOfBombs);
	}
}

bool MineField::CheckGameLost()
{
	bool NotGameLost = true;
	for (int i = 0; i < nDimensions * nDimensions; i++)
	{
		NotGameLost = NotGameLost && !tiles[i].IsGameLost();
	}
	return NotGameLost;
}

bool MineField::CheckGameWon()
{
	bool allRevealed = true;
	for (const Tile& tile : tiles)
	{
		allRevealed = allRevealed && tile.GetState() == Tile::State::Revealed;
	}
	return allRevealed && CheckGameLost();
}

void MineField::HandleRightClick(Vei2 mousePos)
{
	assert(mousePos.x >= pos.x && mousePos.x <= pos.x + (nDimensions * Tile::size) &&
		mousePos.y >= pos.y && mousePos.y <= pos.y + (nDimensions * Tile::size));

	// Get the indeces of Virtual 2D array
	const int x_index = (mousePos.x - pos.x) / nDimensions;
	const int y_index = (mousePos.y - pos.y) / nDimensions;

	//Map 2D array to 1D index
	const int index = (x_index * nDimensions) + y_index;
	if (tiles[index].GetState() == Tile::State::Hidden)
	{
		tiles[index].ChangeState(Tile::State::Flagged);
	}
	else if (tiles[index].GetState() == Tile::State::Flagged)
	{
		tiles[index].ChangeState(Tile::State::Hidden);
	}
}
