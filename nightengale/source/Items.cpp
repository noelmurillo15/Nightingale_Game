#include "Items.h"

#include "../SGD Wrappers\SGD_GraphicsManager.h"


Items::Items(int type, SGD::Point Pos) {

	m_itype = type;
	m_fLifetime = 7.5f;
	m_Size = {16,16};
	m_SmellEmitSz = {400,400};
	m_Position = Pos;

	m_Rect.top = Pos.y - m_Size.height;
	m_Rect.left = Pos.x - m_Size.width;
	m_Rect.right = Pos.x + m_Size.width;
	m_Rect.bottom = Pos.y + m_Size.height;
}

Items::~Items()
{

}

void Items::Update(float elapsedTime){
	m_SmellEmitRect = this->GetRect();	
	m_SmellEmitRect.Resize(m_SmellEmitSz);
	m_SmellEmitRect.MoveTo(SGD::Point(this->GetRect().left - m_SmellEmitSz.width / 2 + (m_Size.width / 2), this->GetRect().top - m_SmellEmitSz.height / 2 + (m_Size.height / 2)));
}

void Items::Render(SGD::Point bawlz)
{
	SGD::Color color;
	if (m_itype == INT_BAIT)
		color = { 255, 0, 0 };
	if (m_itype == INT_REPELLENT)
		color = { 0, 0, 255 };

	SGD::Rectangle tmp = m_Rect;
	tmp.left -= bawlz.x;
	tmp.right -= bawlz.x;
	tmp.top -= bawlz.y;
	tmp.bottom -= bawlz.y;

	if (!tmp.IsEmpty())
		SGD::GraphicsManager::GetInstance()->DrawRectangle(tmp, { 0, 0, 0, 0 }, color, 1);

	tmp = m_SmellEmitRect;
	tmp.left -= bawlz.x;
	tmp.right -= bawlz.x;
	tmp.top -= bawlz.y;
	tmp.bottom -= bawlz.y;

	if (!tmp.IsEmpty())
		SGD::GraphicsManager::GetInstance()->DrawRectangle(tmp, { 50, 0, 0, 155 }, { 0, 0, 155 }, 1);
}

bool Items::Age(float elapsedTime)
{
	m_fLifetime -= elapsedTime;

	if (m_fLifetime <= 0)
		return 1;
	else
		return 0;
}

bool Items::isDying()
{
	if (m_fLifetime < .5)
		return true;
	return false;
}