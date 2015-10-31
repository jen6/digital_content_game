#pragma once

#include <vector>
namespace Logic {
	class UNIT
	{
		int TypeNum;
		int x, y;
	public:
		UNIT() = default;
		UNIT(int _TypeNum);
		~UNIT() = default;

		void Move(int _x, int _y);
		int GetTypeNum();
		int GetLocateX();
		int GetLocateY();
	};

	class MAP
	{
		std::vector<UNIT> Unit;
	public:
		MAP() = default;
		~MAP() = default;

		void AddUnit(int TypeNum);
		int GetUnitNum();
		void MoveUnit(int UnitNum, int x, int y);
		int GetUnitType(int UnitNum);
		int GetUnitLocateX(int UnitNum);
		int GetUnitLocateY(int UnitNum);
	};
}