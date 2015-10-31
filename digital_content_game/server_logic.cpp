#include "server_logic.h"

namespace Logic {
	UNIT::UNIT(int _TypeNum) : TypeNum(_TypeNum), x(0), y(0) {}
	void UNIT::Move(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	int UNIT::GetTypeNum() { return TypeNum; }
	int UNIT::GetLocateX() { return x; }
	int UNIT::GetLocateY() { return y; }
	void MAP::AddUnit(int TypeNum)
	{
		Unit.push_back(UNIT(TypeNum));
	}
	int MAP::GetUnitNum()
	{
		return Unit.size();
	}
	void MAP::MoveUnit(int UnitNum, int x, int y)
	{
		Unit[UnitNum].Move(x, y);
	}
	int MAP::GetUnitType(int UnitNum) { return Unit[UnitNum].GetTypeNum(); }
	int MAP::GetUnitLocateX(int UnitNum) { return Unit[UnitNum].GetLocateX(); }
	int MAP::GetUnitLocateY(int UnitNum) { return Unit[UnitNum].GetLocateY(); }
}