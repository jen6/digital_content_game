#include "server_logic.h"


namespace Logic {
	UNIT_TYPE* UNIT_TYPE::Instance = nullptr;
	UNIT_TYPE* UNIT_TYPE::GetInstance()
	{
		if (Instance == nullptr)
			Instance = new UNIT_TYPE;
		return Instance;
	}
	void UNIT_TYPE::Destroy()
	{
		delete Instance;
	}
	void UNIT_TYPE::AddUnitInfo(std::wstring name, ABILITY_VALUE value, 
		int size, int range, int exp)
	{
		UnitType.push_back(UNIT_INFO(name, value, size, range, exp));
	}
	UNIT_INFO UNIT_TYPE::GetUnitInfo(int TypeNum)
	{
		return UnitType[TypeNum];
	}
	UNIT::UNIT(std::wstring name, ABILITY_VALUE value, int nowhp, 
		D2D1_POINT_2F locate, UNIT_ROTATE rotate) : Name(name), TypeNum(0),
		Monster(false), UnitState(UNIT_STATE::DEFAULT), AbilityValue(value), 
		NowHp(nowhp), Locate(locate), Rotate(rotate), Target(-1), LastAttack(0),
		DeathTime(0) {}
	UNIT::UNIT(int typenum, D2D1_POINT_2F locate, UNIT_ROTATE rotate) :
		Name(UNIT_TYPE::GetInstance()->GetUnitInfo(typenum).GetName()),
		TypeNum(typenum), Monster(true), UnitState(UNIT_STATE::DEFAULT),
		AbilityValue(UNIT_TYPE::GetInstance()->GetUnitInfo(typenum).GetAbility()),
		NowHp(AbilityValue.HP), Locate(locate), Rotate(rotate), Target(-1), 
		LastAttack(0), DeathTime(0) {}
	void UNIT::Move(D2D1_POINT_2F locate)
	{
		Locate = locate;
	}
	void UNIT::SetState(UNIT_STATE state)
	{
		UnitState = state;
	}
	void UNIT::Hit(int Damage)
	{
		NowHp -= Damage;
		if (NowHp <= 0)
		{
			NowHp = 0;
			UnitState = UNIT_STATE::DIE;
			DeathTime = Utils::GetTime() * 0.001;
		}
	}
	std::wstring UNIT::GetName()
	{
		return Name;
	}
	int UNIT::GetTypeNum() { return TypeNum; }
	bool UNIT::isMonster()
	{
		return Monster;
	}
	UNIT_STATE UNIT::GetState()
	{
		return UnitState;
	}
	ABILITY_VALUE UNIT::GetAbility()
	{
		return AbilityValue;
	}
	int UNIT::GetNowHp()
	{
		return NowHp;
	}
	D2D1_POINT_2F UNIT::GetLocate() { return Locate; }

	UNIT_ROTATE UNIT::GetRotate()
	{
		return Rotate;
	}

	void UNIT::SetTarget(int target)
	{
		Target = target;
	}
	int UNIT::GetTarget()
	{
		return Target;
	}

	float UNIT::GetLastAttack()
	{
		return LastAttack;
	}

	float UNIT::GetDeathTIme()
	{
		return DeathTime;
	}

	
	UNIT_INFO::UNIT_INFO(std::wstring name, ABILITY_VALUE value,
		int size, int range, int exp) : Name(name), 
		AbilityValue(value), Size(size), Range(range), Exp(exp)
	{
	}
	std::wstring UNIT_INFO::GetName()
	{
		return Name;
	}
	ABILITY_VALUE UNIT_INFO::GetAbility()
	{
		return AbilityValue;
	}
	int UNIT_INFO::GetSize()
	{
		return Size;
	}
	int UNIT_INFO::GetExp()
	{
		return Exp;
	}
	int UNIT_INFO::GetRange()
	{
		return Range;
	}

	void MAP::UnitAI(int UnitNum)
	{
		float x, y;
		int range;
		switch (Unit[UnitNum].GetState())
		{
		case UNIT_STATE::DEFAULT:
		case UNIT_STATE::WALK:
			x = Unit[UnitNum].GetLocate().x;
			y = Unit[UnitNum].GetLocate().y;
			range = UNIT_TYPE::GetInstance()->
				GetUnitInfo(Unit[UnitNum].GetTypeNum()).GetRange();
			if (Unit[UnitNum].GetTarget() == -1) // -1 == Å¸°Ù¾øÀ½
			{
				Unit[UnitNum].SetTarget(rand() % Unit.size());
				if (Unit[UnitNum].GetTarget() == UnitNum ||
					Unit[Unit[UnitNum].GetTarget()].isMonster())
					Unit[UnitNum].SetTarget(-1);
			}
			else if (Unit[UnitNum].GetRotate() == UNIT_ROTATE::LEFT)
			{
				if (Crash(Unit[UnitNum].GetTarget(), D2D1_RECT_F(x - range, y, x, y )))
				{
					Unit[UnitNum].SetState(UNIT_STATE::ATTACK);
					UnitAttack(UnitNum, D2D1_RECT_F(x - range, y, x, y), 100);
				}
			}
			else
			{
				if (Crash(Unit[UnitNum].GetTarget(), D2D1_RECT_F(x, y, x + range, y )))
				{
					Unit[UnitNum].SetState(UNIT_STATE::ATTACK);
					UnitAttack(UnitNum, D2D1_RECT_F(x, y, x + range, y), 100);
				}
			}
			break;
		case UNIT_STATE::ATTACK:
			if (Utils::GetTime() - Unit[UnitNum].GetLastAttack() > 1)
				Unit[UnitNum].SetState(UNIT_STATE::DEFAULT);
			break;
		}
	}
	void MAP::SpawnUnit(int TypeNum, D2D1_POINT_2F Locate, UNIT_ROTATE Rotate)
	{
		Unit.push_back(UNIT(TypeNum, Locate, Rotate));

	}
	void MAP::EraseUnit(int UnitNum)
	{
		Unit.erase(Unit.begin() + UnitNum);

	}
	void MAP::UnitHit(int UnitNum, int Damage)
	{
		Unit[UnitNum].Hit(Damage);

	}
	void MAP::GiveExp(int UnitNum, int exp)
	{

	}
	bool MAP::Crash(int unit, D2D1_RECT_F rect)
	{
		int UnitSize = UNIT_TYPE::GetInstance()->GetUnitInfo(unit).GetSize() / 2;
		return (Unit[unit].GetLocate().x - UnitSize <= rect.right &&
			Unit[unit].GetLocate().x + UnitSize >= rect.left &&
			Unit[unit].GetLocate().y - UnitSize <= rect.top &&
			Unit[unit].GetLocate().y + UnitSize >= rect.bottom);
	}

	MAP::MAP(int mapnum, int unitnum) : MapNum(mapnum), UnitNum(unitnum)
	{
	}

	void MAP::Work()
	{
		for (int i = 0; i < Unit.size(); i++)
		{
			if (Unit[i].isMonster())
				UnitAI(i);

			if (Unit[i].GetState() == UNIT_STATE::DIE &&
				Utils::GetTime() - Unit[i].GetDeathTIme() > 5)
				EraseUnit(i);
		}
	}

	void MAP::AddUnit(std::wstring name, ABILITY_VALUE value, 
		int nowhp, D2D1_POINT_2F locate, UNIT_ROTATE rotate)
	{
		Unit.push_back(UNIT(name, value, nowhp, locate, rotate));

	}
	void MAP::UnitMove(int object_idx, D2D1_POINT_2F locate)
	{
		Unit[object_idx].Move(locate);

	}
	void MAP::UnitChangeState(int object_idx, UNIT_STATE state)
	{
		Unit[object_idx].SetState(state);

	}
	void MAP::UnitChat(int UnitNum, std::wstring Chat)
	{

	}
	void MAP::UnitAttack(int UnitNum, D2D1_RECT_F Scale, int DamagePer)
	{
		for (int i = 0; i < Unit.size(); i++)
		{
			if (Unit[i].isMonster() != Unit[UnitNum].isMonster() &&
				(i == UnitNum || Unit[i].GetState() == UNIT_STATE::DIE))
				continue;

			if (Crash(i, Scale))
				UnitHit(i, Unit[UnitNum].GetAbility().ATT * DamagePer / 100);
			if (Unit[i].GetState() == UNIT_STATE::DIE)
				GiveExp(UnitNum, UNIT_TYPE::GetInstance()->GetUnitInfo(i).GetExp());
		}
	}
	D2D1_POINT_2F::D2D1_POINT_2F(int _x, int _y) : x(_x), y(_y)
	{
	}
	D2D1_RECT_F::D2D1_RECT_F(int l, int t, int r, int b) :
		left(l), top(t), right(r), bottom(b)
	{
	}
}