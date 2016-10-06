#pragma once

#include <atlstr.h>
#include <vector>

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../resource.h"


class DialogSystem {

	bool m_empty = true;
	bool m_renderQuest = false;	
	float m_DisplayDialogTimer = 0.0f;
	
	struct DialogMsg {
		CString m_Text;
		std::string m_Sender;
	};
	
	DialogMsg m_currDialog;
	std::vector<DialogMsg> m_DialogQueue;

	SGD::HTexture dialogBox = SGD::INVALID_HANDLE;
	SGD::HTexture questLogBox = SGD::INVALID_HANDLE;

public:

	DialogSystem();
	~DialogSystem();

		//	Interface
	void Update(float elapsedTime);
	void Render(SGD::Rectangle, SGD::Point);
	void RenderQuest(SGD::Rectangle, SGD::Point, unsigned int);

		//	Accessors
	bool GetIsEmpty() { return m_empty; }
	bool GetIsRenderingQuest(){ return m_renderQuest; }

		//	Mutators
	void ToggleRenderQuest(){ m_renderQuest = !m_renderQuest; }

		//	Add Dialog
	void AddDialogToQueue(int stringtablenum, std::string sender);
	void AddDialogToQueue(CString dialog, std::string sender);
	void AddDialogToQueue(std::string dialog, std::string sender);
};