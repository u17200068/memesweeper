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
		Tile(State state, Vei2 pos, bool hasMeme);
		Vei2 GetPos() const;
		bool HasMeme() const;
		bool IsGameLost() const;
		void ChangeState(State newState);
		void AddMeme(bool newMeme);
		void DrawTile(Graphics& gfx);
		MineField::Tile::State MineField::Tile::GetState() const;
		static constexpr int size = 16;
	private:
		bool GameLost = false;
		State state;
		Vei2 pos;
		bool hasMeme = false;
	};
public:
	MineField(Vei2 pos);
	void DrawField(Graphics& gfx);
	void HandleLeftClick(Vei2 mousePos);
	bool CheckGameLost();
	void HandleRightClick(Vei2 mousePos);
	static constexpr int nDimensions = 16;
private:
	Vei2 pos;
	Tile tiles[nDimensions * nDimensions];
};