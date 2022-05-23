#pragma once
#include "Vei2.h"
#include "Graphics.h"
#include "SpriteCodex.h"

class MineField
{
private:
	class Tile
	{
	public:
		enum State
		{
			Hidden,
			Flagged,
			Revealed
		}; 
		Tile() = default;
		Tile(State state, Vei2 pos);
		Vei2 GetPos() const;
		void DrawTile(Graphics& gfx);
		MineField::Tile::State MineField::Tile::GetState() const;
		static constexpr int size = 16;
	private:
		State state;
		Vei2 pos;
	};
public:
	MineField(Vei2 pos);
	void DrawField(Graphics& gfx);
	static constexpr int nDimensions = 16;
private:
	Vei2 pos;
	Tile tiles[nDimensions * nDimensions];
};