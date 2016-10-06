#pragma once

#include "Entity.h"
#include "Layer.h"


class Camera {

	SGD::Point m_CamPos;
	SGD::Point m_Offset;
	SGD::Size m_ScreenSize;
	SGD::Size m_WorldSize;
	SGD::Rectangle m_CameraBounds;

	Entity* Focus = nullptr;
	Layer* m_mapLayer = nullptr;

public:

	//	Singleton
	static Camera* GetInstance(void);

	void Enter(void);	// load resources
	void Exit(void);	// unload resources

	void Update(float elapsedtime);
	void ResetCamera();

	void InitializeLayer(bool bush, bool collider, SGD::Point);

	SGD::Point GetCameraPosition(){ return m_CamPos; }
	SGD::Point GetCameraOffset(){ return m_Offset; }
	SGD::Size GetWorldSize(){ return m_WorldSize; }
	SGD::Rectangle GetCameraBounds(){ return m_CameraBounds; }
	SGD::Size GetScreenRes(){ return m_ScreenSize; }
	Layer* GetMapLayer(){ return m_mapLayer; }
	
	void SetCameraPos(SGD::Point pos){ m_CamPos = pos; }
	void SetCameraOffset(SGD::Point pos){ m_Offset = pos; }
	void SetWorldSize(SGD::Size sz){ m_WorldSize = sz; }

		//	Camera Focus
	Entity* GetCamFocus(){ return Focus; }
	void SetCamFocus(Entity* f);

private:
	// SINGLETON (not-dynamically allocated)
	Camera(void) = default;	// default constructor
	virtual ~Camera(void) = default;	// destructor
	Camera(const Camera&) = delete;	// copy constructor
	Camera& operator= (const Camera&) = delete;	// assignment operator
};