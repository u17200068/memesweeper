#include "MineField.h"

MineField::Tile::Tile(State state, Vei2 pos)
{
	this->state = state;
	this->pos = pos;
}

Vei2 MineField::Tile::GetPos() const
{
	return pos;
}

void MineField::Tile::DrawTile(Graphics& gfx)
{
	SpriteCodex::DrawTileButton(pos, gfx);
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
			tiles[index] = { Tile::State::Hidden, {x,y} };
			index++;
		}
	}
}

void MineField::DrawField(Graphics& gfx)
{
	const int StartX = pos.x;
	const int EndX = pos.x + (nDimensions * Tile::size);
	const int StartY = pos.y;
	const int EndY = pos.y + (nDimensions * Tile::size);
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
}
