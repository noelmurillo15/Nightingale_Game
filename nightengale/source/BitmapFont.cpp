#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include <ctype.h>
#include <cassert>
#include "tinyxml.h"

/**************************************************************/
// Initialize
//	- configure the font for "SGD_Font_Glow.png"
//	- probably should have parameters / config file
void BitmapFont::Initialize( void )
{
	// Load the image
	m_GameFont = SGD::GraphicsManager::GetInstance()->LoadTexture(
				L"resource/graphics/GameFont.png" );

	m_KaoFont = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/KaoFont.png");

	m_TestFont = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/TestFont.png");

	m_TitleFont = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/TitleFont.png"); 

	m_JennyFont = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/JennyFont.png");

	m_ToddFont = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/ToddFont.png"); 

	m_AsheFont = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/AshFont.png");

	m_BrockFont = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/BrockFont.png");

	Load("Game");
	Load("Kao");
	Load("Test"); 
	Load("Title"); 
	Load("Jenny"); 
	Load("Todd"); 
	Load("Ash"); 
	Load("Brock");

	// Validate the images
	assert(m_GameFont != SGD::INVALID_HANDLE
		&& "BitmapFont::Draw - image was not loaded");

	assert(m_KaoFont != SGD::INVALID_HANDLE
		&& "BitmapFont::Draw - image was not loaded");

	assert(m_TestFont != SGD::INVALID_HANDLE
		&& "BitmapFont::Draw - image was not loaded");

	assert(m_TitleFont != SGD::INVALID_HANDLE
		&& "BitmapFont::Draw - image was not loaded");

	assert(m_JennyFont != SGD::INVALID_HANDLE
		&& "BitmapFont::Draw - image was not loaded");

	assert(m_ToddFont != SGD::INVALID_HANDLE
		&& "BitmapFont::Draw - image was not loaded");

	assert(m_AsheFont != SGD::INVALID_HANDLE
		&& "BitmapFont::Draw - image was not loaded");

	assert(m_BrockFont != SGD::INVALID_HANDLE
		&& "BitmapFont::Draw - image was not loaded");
}

void BitmapFont::Load(std::string obj) {

	std::string xmll = "resource/XMLFiles/Fonts/" + obj + "Font.xml";
	TiXmlDocument doc(xmll.c_str());
	if (!doc.LoadFile())
		return;

	TiXmlHandle hDoc(&doc);

	TiXmlElement* AnimationFiles = hDoc.ChildElement(0).Element();
	if (!AnimationFiles) return;
	// *

	TiXmlElement* Object = AnimationFiles->FirstChildElement();
	int AnimNum;
	Object->QueryIntAttribute("AnimationCount", &AnimNum);
	Object = Object->NextSiblingElement();
	// /*


	TiXmlElement* Animation = Object->FirstChildElement();

	//forloop for animations
	//Load animation name, Store in Loaded[][x]
	for (int i = 0; i < AnimNum; i++, Animation = Animation->NextSiblingElement())
	{
		int FrameNum;
		Animation->QueryIntAttribute("FrameCount", &FrameNum);
		std::string Aname = Animation->Attribute("AnimName");

		TiXmlElement* frame = Animation->FirstChildElement();

		//forloop for frames
		//Load in frames, store in Loaded[][][x]
		for (int i = 0; i < FrameNum; i++)
		{
			SGD::Rectangle t;

			frame->QueryFloatAttribute("DrawLeft", &t.left);
			frame->QueryFloatAttribute("DrawTop", &t.top);
			frame->QueryFloatAttribute("DrawRight", &t.right);
			frame->QueryFloatAttribute("DrawBottom", &t.bottom);

			float y;
			frame->QueryFloatAttribute("AnchorTop", &y);

			std::string let;
			let = frame->Attribute("Event");
			m_MLetters[let] = new Letter;
			m_MLetters[let]->Rect = t;
			m_MLetters[let]->anchor = y;
		
			frame = frame->NextSiblingElement();
		}
	}


	m_FontType[obj] = m_MLetters;

	m_MLetters.clear();
}

/**************************************************************/
// Terminate
//	- clean up resources
void BitmapFont::Terminate( void )
{
	// Unload the image
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_GameFont);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_KaoFont);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_TestFont); 
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_TitleFont); 
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_JennyFont);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_AsheFont);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_ToddFont);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_BrockFont);


	auto iter = m_FontType["Game"].begin();
	for (; iter != m_FontType["Game"].end(); iter++)
	{
		delete iter->second;
	}

	iter = m_FontType["Kao"].begin();
	for (; iter != m_FontType["Kao"].end(); iter++)
	{
		delete iter->second;
	}

	iter = m_FontType["Test"].begin();
	for (; iter != m_FontType["Test"].end(); iter++)
	{
		delete iter->second;
	}

	iter = m_FontType["Title"].begin();
	for (; iter != m_FontType["Title"].end(); iter++)
	{
		delete iter->second;
	}
	
	iter = m_FontType["Ash"].begin();
	for (; iter != m_FontType["Ash"].end(); iter++)
	{
		delete iter->second;
	}

	iter = m_FontType["Jenny"].begin();
	for (; iter != m_FontType["Jenny"].end(); iter++)
	{
		delete iter->second;
	}

	iter = m_FontType["Todd"].begin();
	for (; iter != m_FontType["Todd"].end(); iter++)
	{
		delete iter->second;
	}

	iter = m_FontType["Brock"].begin();
	for (; iter != m_FontType["Brock"].end(); iter++)
	{
		delete iter->second;
	}
}



/**************************************************************/
// Draw
//	- draw the text one character at a time,
//	  using the Cell Algorithm to calculate the source rect
void BitmapFont::Draw(const std::string output, std::string FontType, SGD::Point position,
			float scale, SGD::Color color )  {

	// Is this string invisible?
	if( output[ 0 ] == '\0'			// string is empty?
		|| scale == 0.0f			// scale is invisible?
		|| color.alpha == 0 )		// color is invisible?
		return;
	
	
	// Store the starting X position for newlines
	float colStart = position.x;

	// Iterate through the characters in the string
	for (int i = 0; output[i]; i++)
	{
		// Get the current character
		std::string ch;
		const std::string wtf = "A";
		ch = output[i];

		// Check for whitespace
		if (ch == " ")
		{
			position.x += m_FontType[FontType][wtf]->Rect.ComputeWidth() * scale;
			continue;
		}
		else if (ch == "\n")
		{
			// Move to the next row
			position.y += m_FontType[FontType]["A"]->Rect.ComputeHeight() * scale;
			position.x = colStart;
			continue;
		}
		else if (ch == "\t")
		{
			// Calculate the number of pixels from the start
			float pixels = position.x - colStart;

			// Calculate the number of characters from the start
			int chars = int(pixels / (m_FontType[FontType]["A"]->Rect.ComputeWidth() * scale));

			// to get a 4-space alignment
			int spaces = 4 - (chars % 4);

			// Move to the next position
			position.x += spaces * (m_FontType[FontType]["A"]->Rect.ComputeWidth() * scale);
			continue;
		}


#pragma region Render Based On Font Type
		if (FontType == "Kao")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_KaoFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Test")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_TestFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Jenny")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_JennyFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Ash")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_AsheFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Brock")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_BrockFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Todd")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_ToddFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Title")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_TitleFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });
		else
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_GameFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });
#pragma endregion

		
		// Move to the next position
		position.x += (int)(m_FontType[FontType][ch]->Rect.ComputeWidth() * scale);
	}
}

	
/**************************************************************/
// Draw
//	- draw the text one character at a time,
//	  using the Cell Algorithm to calculate the source rect
void BitmapFont::Draw(const wchar_t* output, std::string FontType, SGD::Point position,
	float scale, SGD::Color color) {

	// Validate the parameter
	assert(output != nullptr
		&& "BitmapFont::Draw - string cannot be null");


	// Is this string invisible?
	if (output[0] == L'\0'		// string is empty?
		|| scale == 0.0f			// scale is invisible?
		|| color.alpha == 0)		// color is invisible?
		return;

	// Store the starting X position for newlines
	float colStart = position.x;

	// Iterate through the characters in the string
	for (int i = 0; output[i]; i++)
	{
		std::string ch;
		ch = (char)(output[i]);

		// Check for whitespace
		if (ch == " ")
		{
			// Move to the next position
			position.x += m_FontType[FontType]["A"]->Rect.ComputeWidth()  * scale;
			continue;
		}
		else if (ch == "\n")
		{
			// Move to the next row
			position.y += m_FontType[FontType]["A"]->Rect.ComputeHeight() * scale;
			position.x = colStart;
			continue;
		}
		else if (ch == "\t")
		{
			// Calculate the number of pixels from the start
			float pixels = position.x - colStart;

			// Calculate the number of characters from the start
			int chars = int(pixels / (m_FontType[FontType]["A"]->Rect.ComputeWidth()  * scale));

			// to get a 4-space alignment
			int spaces = 4 - (chars % 4);

			// Move to the next position
			position.x += spaces * (m_FontType[FontType]["A"]->Rect.ComputeWidth()  * scale);
			continue;
		}

#pragma region Render Based On Font Type
		if (FontType == "Kao")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_KaoFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Test")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_TestFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Jenny")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_JennyFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Ash")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_AsheFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Brock")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_BrockFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Todd")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_ToddFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else if (FontType == "Title")
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_TitleFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });

		else
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_GameFont, { position.x, position.y - (m_FontType[FontType][ch]->anchor - m_FontType[FontType][ch]->Rect.top) },
			m_FontType[FontType][ch]->Rect, 0.0f, {},
			color, { scale, scale });
#pragma endregion


		// Move to the next position
		position.x += (int)(m_FontType[FontType]["A"]->Rect.ComputeWidth()  * scale);
	}
}
