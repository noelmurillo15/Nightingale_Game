#pragma once

#include <map>
#include <vector>
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"


class BitmapFont {

public:
	// Constructor & Destructor
	BitmapFont( void )	= default;
	~BitmapFont( void )	= default;

	// Initialize & Terminate
	void Initialize( void );	// should have parameters
	void Terminate ( void );

	// Draw
	void Draw(std::string output, std::string FontType, SGD::Point position,
		float scale, SGD::Color color);
	
	void Draw(const wchar_t* output, std::string FontType, SGD::Point position,
		float scale, SGD::Color color);

private:
	struct Letter {
		SGD::Rectangle Rect;
		float anchor;
	};	

	std::map<std::string , Letter*> m_MLetters;
	std::map<std::string, std::map<std::string, Letter*>> m_FontType;

	// image
	SGD::HTexture	m_GameFont	= SGD::INVALID_HANDLE;
	SGD::HTexture	m_KaoFont	= SGD::INVALID_HANDLE;
	SGD::HTexture	m_JennyFont	= SGD::INVALID_HANDLE;
	SGD::HTexture	m_AsheFont	= SGD::INVALID_HANDLE;
	SGD::HTexture	m_ToddFont	= SGD::INVALID_HANDLE;
	SGD::HTexture	m_BrockFont = SGD::INVALID_HANDLE;
	SGD::HTexture	m_TestFont = SGD::INVALID_HANDLE;
	SGD::HTexture	m_TitleFont = SGD::INVALID_HANDLE;

	// cell data

	void Load(std::string obj);
	// font info
	char			m_cFirstChar		= '\0';
	bool			m_bOnlyUppercase	= false;
};