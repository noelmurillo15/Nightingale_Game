#pragma once

#include "Tile.h"

#include <vector>

#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Handle.h"


class TileEngine{

		//	Textures
	SGD::HTexture _World0 = SGD::INVALID_HANDLE;
	SGD::HTexture _World1 = SGD::INVALID_HANDLE;
	SGD::HTexture _World2 = SGD::INVALID_HANDLE;
	SGD::HTexture _World3 = SGD::INVALID_HANDLE;
	SGD::HTexture _World4 = SGD::INVALID_HANDLE;
	SGD::HTexture _World5 = SGD::INVALID_HANDLE;

	SGD::HTexture Background = SGD::INVALID_HANDLE;
	SGD::HTexture Icon = SGD::INVALID_HANDLE;
		
		//	BGM
	SGD::HAudio m_LoadingBGM = SGD::INVALID_HANDLE; 

		//	Tiles
	SGD::Size m_TileSize;
	SGD::Size m_TileSetSize;

	std::vector<Tile*> m_tiles;		

	bool m_renderMinimap = false;

	enum {
		E_BASEWORLD,
		E_WORLD1,
		E_WORLD2,
		E_WORLD3,
		E_WORLD4,
		E_WORLD5
	};

	unsigned int m_World;

	float red, green, blue;
	bool _red, _green, _blue;

public:

		//	Singleton
	static TileEngine* GetInstance(void);

	void Enter(void);	// load resources
	void Exit(void);	// unload resources

		//	Interface
	void Update(float elapsedtime);
	void Render();	
	void RenderMiniMap(SGD::Rectangle);

		//	File I/O
	void LoadMap(unsigned int worldnum);
	void LoadSpawnPts(std::string filename, unsigned int);
	void LoadEvents(std::string filename, unsigned int);

		//	Tile conversions
	int Convert2d_1d(int x, int y, int width);
	SGD::Size Convert1d_2d(int i);

	float Rotation = 0;

	bool GetMinimapOn(){ return m_renderMinimap; }
	void ToggleMinimap(){ m_renderMinimap = !m_renderMinimap; }

		//	Accessors	
	std::vector<Tile*>	GetTilesVec(){ return m_tiles; }
	unsigned int		GetWorldNum(){ return m_World; }

		//	Mutators
	void SetTileSize(SGD::Size size)	{ m_TileSize = size; }
	void SetTileSetSize(SGD::Size size)	{ m_TileSetSize = size; }
	void SetWorldNum(unsigned int world){ m_World = world; }
	void UpdateRender();
	float RotateIcon();
	void UpdateColor();
	
private:
	TileEngine(void) = default;	// default constructor
	virtual ~TileEngine(void) = default;	// destructor
	TileEngine(const TileEngine&) = delete;	// copy constructor
	TileEngine& operator= (const TileEngine&) = delete;	// assignment operator

	float barLength = 0;
};