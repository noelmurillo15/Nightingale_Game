#pragma once

#include "IEntity.h"						// IEntity type
#include "Items.h"
#include "Camera.h"
#include "AnimationSystem.h"
#include "Senses.h"
#include "DataWereHouse.h"
#include "Items.h"
#include <vector>

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#define VISION_ANGLE 45.0f


struct StankNode
{
	SGD::Size m_StankSize;
	SGD::Point m_OriginOffset;
	SGD::Point m_origin;
	SGD::Rectangle m_recStankVolume;

	StankNode* Next;
	float m_iDecayTime = 5.0f;

	bool Update(float ElaspedTime) {
		//m_recStankVolume.MoveTo(SGD::Point(m_recStankVolume.left - m_StankSize.width + (m_StankSize.width / 2), m_recStankVolume.top - m_StankSize.height + (m_StankSize.height / 2)));
		m_iDecayTime -= ElaspedTime;

		if (m_iDecayTime <= 0)
			return false;

		return true;
	}

	void Terminate(void)
	{
		delete Next;
	}

	bool operator==(const StankNode& rhs) {

		if (rhs.m_origin == this->m_origin && rhs.m_iDecayTime == this->m_iDecayTime &&
			rhs.m_OriginOffset == this->m_OriginOffset && rhs.m_recStankVolume == this->m_recStankVolume &&
			rhs.m_recStankVolume == this->m_recStankVolume && rhs.m_StankSize == this->m_StankSize)
			return true;

		return false;
	}

	bool operator!=(const StankNode& rhs) {

		if (rhs.m_origin != this->m_origin || rhs.m_iDecayTime != this->m_iDecayTime ||
			rhs.m_OriginOffset != this->m_OriginOffset || rhs.m_recStankVolume != this->m_recStankVolume ||
			rhs.m_recStankVolume != this->m_recStankVolume || rhs.m_StankSize != this->m_StankSize)
			return true;

		return false;
	}

};


class Entity : public IEntity {

#define	SMELL_CREATE_TIMER  0.35f	

public:

	Entity(void) = default;			// default constructor
	virtual ~Entity(void);			// VIRTUAL destructor	

#pragma region Virtual Methods

		//	Interface
	virtual void	Update(float elapsedTime);
	virtual void	Render(SGD::Point);
	
		//	Updates
	virtual void	UpdateMoveSpeed();
	virtual void	UpdateEmitRects();	
	virtual void	UpdateSenseRects();
	virtual void	UpdateBaseRect(SGD::Point);
	virtual void	HandleCollision(const IEntity* pOther);
	virtual void	PreformItemBehavaior(const IEntity* pOther);
	virtual float	CheckLoS(IEntity* ToCheck);

		//	Accessors
	virtual int		GetDamage() const	{ return 0; }
	virtual int		GetType() const		{ return ENT_BASE; }
	virtual int		GetCreature() const	{ return CRT_BASE; }	
	virtual SGD::Rectangle GetRect(void)const { return m_BaseRect; }
	
		//	Misc
	virtual vector<Entity*> InLineOfSight();
	virtual void SetDirectionIdle();
#pragma endregion

#pragma region Normal Functions

		//	Creation Methods
	void LoadData(SGD::Size soundRange, SGD::Size sightRange, SGD::Size smellRange, SGD::Point spawnPoint);
	Entity::StankNode* CreateSmellNode(Entity* _Entity);

		// Accessors:
	int				GetHealth()const				{ return m_health; }
	int				GetMoveMode()const				{ return m_moveMode; }
	int				GetMoveSpeed()const				{ return m_moveSpeed; }	

	float			GetAlpha()						{ return m_alpha; }

	bool			GetAlive()						{ return m_alive; }
	bool			GetInHiding()const				{ return m_inHiding; }

	string			GetName() const					{ return m_Name; }
	string			GetDirection_8(SGD::Vector loc);
	string			GetDirection_4(SGD::Vector loc);

	SGD::Point		GetPosition(void) const			{ return m_currPosition; }
	SGD::Point		GetPrevPosition(void) const		{ return m_prevPosition; }
	SGD::Point		GetDestination()const			{ return m_destination; }

	SGD::Size		GetSize(void) const				{ return m_szSize; }
	SGD::Size		GetSoundSenseSz(void)const		{ return m_SoundSenseSz; }
	SGD::Size		GetSightSenseSz(void)const		{ return m_SightSenseSz; }
	SGD::Size		GetSmellSenseSz(void)const		{ return m_SmellSenseSz; }
	SGD::Size		GetSightEmitSz(void)const		{ return m_SightEmitSz; }
	SGD::Size		GetSoundEmitSz(void)const		{ return m_SoundEmitSz; }

	SGD::Rectangle	GetSoundSenseRect(void)const	{ return m_SoundSenseRect; }
	SGD::Rectangle	GetSightSenseRect(void)const	{ return m_SightSenseRect; }
	SGD::Rectangle	GetSmellSenseRect(void)const	{ return m_SmellSenseRect; }
	SGD::Rectangle	GetSightEmitRect(void)const		{ return m_SightEmitRect; }
	SGD::Rectangle	GetSoundEmitRect(void)const		{ return m_SoundEmitRect; }

	Senses*			GetSenses()						{ return LocalSenses; }
	Entity*			GetTarget()						{ return Target; }
	DataWereHouse*	GetDataWareHouse()				{ return m_ptBrain; }
	AnimationTimestamp* GetTimestamp()				{ return TimeStamp; }	
	

		// Mutators:
	void SetHealth(int hp)					{ m_health = hp; }
	void SetMoveMode(int mode)				{ m_moveMode = mode; }
	void SetMoveSpeed(int speed)			{ m_moveSpeed = speed; }

	void SetAlive(bool A)					{ m_alive = A; }
	void SetInHiding(bool boo);
	void SetRespawnTimer(float t)			{ m_respawnTimer = t; }
	void SetAlpha(float a)					{ m_alpha = a; }
	void SetName(std::string name)			{ m_Name = name; }

	void SetPosition(SGD::Point	pos);
	void SetPrevPosition(SGD::Point pos)	{ m_prevPosition = pos; }	
	void SetDestination(SGD::Point pos)		{ m_destination = pos; }
	void SetInitialPosition(SGD::Point pos)	{ m_currPosition = pos; m_prevPosition = pos; }

	void SetSize(SGD::Size size) 			{ m_szSize = size; }
	void SetSightEmitSz(SGD::Size NewSz)	{ m_SightEmitSz = NewSz; }
	void SetSoundEmitSz(SGD::Size NewSz)	{ m_SoundEmitSz = NewSz; }
	
	void SetSightEmitRect(SGD::Rectangle _newRect)	{ m_SightEmitRect = _newRect; }
	void SetSoundEmitRect(SGD::Rectangle _newRect)	{ m_SoundEmitRect = _newRect; }

	void SetTarget(Entity* t)				{ Target = t; }

		//	Misc
	void Warp();	
	void Respawn();

	SGD::Vector Zoolander(SGD::Vector);
#pragma endregion

#pragma region Enums
	enum EntityType {
		ENT_BASE,
		ENT_BUSH,
		ENT_PACK,		
		ENT_ANIMAL,
		ENT_ITEM,
		ENT_PLAYER
	};

	enum MoveMode{
		MOVE_STAND,
		MOVE_WALK,
		MOVE_TROT,
		MOVE_RUN,
		MOVE_EXHAUSTED,
	};

	enum CreatureType{
		CRT_BASE,
		CRT_PASSANT,
		CRT_BISHOP,
		CRT_BUMBLES,
		CRT_CASTLE,
		CRT_KING,
		CRT_KNIGHT,
		CRT_PACK,
		CRT_PLAYER,
		CRT_PAWN,
		CRT_QUEEN,
		CRT_ROOK,
		CRT_PRANCER
	};

	enum EntityState {
		STATE_PASSIVE,
		STATE_CURIOUS,
		STATE_AGGRO,
		STATE_FEAR,
		STATE_ALERT,
		STATE_ASLEEP,
		STATE_PAWN_LOVE_MODE
	};
#pragma endregion

protected:

	int		m_health = 1;
	int		m_moveSpeed = 0;
	int		m_moveMode = MOVE_STAND;
	int		m_currentSmellIndex = 0;
	float	m_alpha = 255.0f;
	float	m_soundTimer = 0.0f;
	float	m_warpTimer = 0.0f;
	float	m_respawnTimer = 0.0f;

	bool	m_inHiding = false;
	bool	m_alive = true;

	string	m_Name = "";
	string	m_previousDirection = "";

	SGD::Point	m_currPosition = SGD::Point( 1.0f, 1.0f );
	SGD::Point	m_prevPosition = SGD::Point( 0.0f, 0.0f);
	SGD::Point	m_destination = SGD::Point(-10.0f, -10.0f);

	SGD::Vector	m_vecDirection = SGD::Vector(0.0f, 0.0f);

	SGD::Size	m_szSize = SGD::Size{ 0, 0 };		// 2D size
	SGD::Size	m_SoundSenseSz = SGD::Size(0, 0);
	SGD::Size	m_SightSenseSz = SGD::Size(0, 0);
	SGD::Size	m_SmellSenseSz = SGD::Size(0, 0);
	SGD::Size	m_SightEmitSz = SGD::Size(0, 0);
	SGD::Size	m_SoundEmitSz = SGD::Size(0, 0);

	SGD::HAudio	m_aSound = SGD::INVALID_HANDLE;

	SGD::Rectangle	m_BaseRect = SGD::Rectangle(0, 0, 0, 0);
	SGD::Rectangle	m_SoundSenseRect = SGD::Rectangle(0, 0, 0, 0);
	SGD::Rectangle	m_SightSenseRect = SGD::Rectangle(0, 0, 0, 0);
	SGD::Rectangle	m_SmellSenseRect = SGD::Rectangle(0, 0, 0, 0);
	SGD::Rectangle	m_SightEmitRect = SGD::Rectangle(0, 0, 0, 0);
	SGD::Rectangle	m_SoundEmitRect = SGD::Rectangle(0, 0, 0, 0);

	Entity*				Target = nullptr;
	DataWereHouse*		m_ptBrain = nullptr;
	Senses*				LocalSenses = nullptr;
	AnimationTimestamp* TimeStamp = nullptr;
};