#include "Movement.h"

namespace MOVEMENT
{
	CMovement::CMovement()
		: jumpspeed(0)
		, AnimationCounter(0)
		, OnGround(true)
		, InAir(false)
	{
		Projectile = new PROJECTILE::Projectile();
	}


	CMovement::~CMovement()
	{
	}

	void CMovement::Init()
	{
		position = Vector3(0, 0, 0);
	}

	int CMovement::GetPos_X()
	{
		return position.x;
	}

	int CMovement::GetPos_Y()
	{
		return position.y;
	}

	int CMovement::GetAnimationCounter()
	{
		return AnimationCounter;
	}

	int CMovement::GetJumpspeed()
	{
		return jumpspeed;
	}

	void CMovement::SetPos_X(int pos_X)
	{
		this->position.x = pos_X;
	}

	void CMovement::SetPos_Y(int pos_Y)
	{
		this->position.y = pos_Y;
	}

	void CMovement::SetAnimationCounter(int AnimationCounter)
	{
		this->AnimationCounter = AnimationCounter;
	}

	void CMovement::SetToJump(bool jump)
	{
		if (jump)
		{
			InAir = true;
			Drop = false;
			jumpspeed = 10;
		}
	}

	void CMovement::SetJumpspeed(int jumpspeed)
	{
		this->jumpspeed = jumpspeed;
	}

	void CMovement::SetAnimationInvert(bool AnimationInvert)
	{
		this->AnimationInvert = AnimationInvert;
	}

	bool CMovement::GetAnimationInvert()
	{
		return AnimationInvert;
	}

	void CMovement::UpdateJumpUpwards()
	{
		position.y -= jumpspeed;
		jumpspeed -= 1;
		if (jumpspeed == 0)
		{
			InAir = false;
			Drop = true;
		}
	}

	void CMovement::MoveLeftRight(const bool mode, const float timeDiff)
	{
		if (mode)
		{
			position.x = position.x - (int)(5.0f * timeDiff);
			AnimationInvert = true;
			AnimationCounter--;
			if (AnimationCounter < 0)
				AnimationCounter = 2;
		}
		else
		{
			position.x = position.x + (int)(5.0f * timeDiff);
			AnimationInvert = false;
			AnimationCounter++;
			if (AnimationCounter > 2)
				AnimationCounter = 0;
		}
	}

	void CMovement::AnimationUpdate(double dt) // Collision Check for Normal Stage
	{
		if (!InAir && !Drop)
		{
			//jumpspeed = 0;
			//InAir = false;
			//Drop = true;
		}

		else if (InAir && !Drop)
		{
			position.y += jumpspeed;
			jumpspeed -= 1;
			if (jumpspeed == 0)
			{
				InAir = false;
				Drop = true;
			}
		}

		else if (!InAir & Drop)
		{
			if (position.y > 10) // for moment until collision
			{
				position.y -= jumpspeed;
				jumpspeed += 1;
			}
		}
	}

	Projectile* CMovement::FetchProjectile()
	{
		for (std::vector<PROJECTILE::Projectile *>::iterator it = m_projectileList.begin(); it != m_projectileList.end(); ++it)
		{
			PROJECTILE::Projectile *projectile = (PROJECTILE::Projectile *)*it;
			std::vector<PROJECTILE::Projectile *>::iterator it2 = it;
			if (!projectile->active)
			{
				projectile->Init();
				projectile->active = true;

				return projectile;
			}
		}
		for (unsigned i = 0; i < 10; ++i)
		{
			PROJECTILE::Projectile *projectile = new PROJECTILE::Projectile();
			m_projectileList.push_back(projectile);
		}
		PROJECTILE::Projectile *projectile = m_projectileList.back();
		projectile->active = true;
		return projectile;
	}

	void CMovement::ProjectileUpdate(const float timeDiff, double dt, int scale)
	{
		
		if (AnimationInvert == false)
		{
			Projectile = FetchProjectile();
			Projectile->pos.Set(position.x, position.y, 0);
			Projectile->vel.Set(500, 0, 0);
			Projectile->scale.Set(scale, scale, scale);
		}
		else
		{
			Projectile = FetchProjectile();
			Projectile->pos.Set(position.x, position.y, 0);
			Projectile->vel.Set(500, 0, 0);
			Projectile->scale.Set(scale, scale, scale);
		}
	}
}
