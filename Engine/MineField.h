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
		void SetResult(bool result);
		void SetAdjacentBombs(int bombs);
		int GetAdjacentBombs();
		void ChangeState(State newState);
		bool HasNoNeighbourMemes();
		void AddMeme(bool newMeme);
		void DrawTile(Graphics& gfx);
		MineField::Tile::State MineField::Tile::GetState() const;
		static constexpr int size = 16;
	private:
		int adjacentBombs = 0;
		bool GameLost = false;
		State state = Tile::State::Hidden;
		Vei2 pos = {0,0};
		bool hasMeme = false;
	};
public:  
	MineField(Vei2 pos);
	void DrawField(Graphics& gfx);
	void HandleLeftClick(Vei2 mousePos);
	void SetAdjacentBombs();
	bool CheckGameLost();
	bool CheckGameWon();
	void HandleRightClick(Vei2 mousePos);
	void CheckAdjacentMemes(Vei2 GridPos);
	void RevealTile(Vei2 GridPos, int index);
	static constexpr int nDimensions = 16;
private:
	Vei2 pos;
	Tile tiles[nDimensions * nDimensions];
};