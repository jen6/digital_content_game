#pragma once
#include <vector>
#include <mutex>
#include <memory>
#include "utils.h"
namespace Logic {
	typedef struct D2D1_POINT_2F
	{
		D2D1_POINT_2F() = default;
		D2D1_POINT_2F(int, int);
		float x, y;
	}D2D1_POINT_2F;
	
	typedef struct D2D1_RECT_F
	{
		D2D1_RECT_F() = default;
		D2D1_RECT_F(int, int, int, int);
		float left, top, bottom, right;
	}D2D1_RECT_F;


	enum class UNIT_STATE : int { DEFAULT = 0, WALK = 1, RUN = 2, ATTACK = 3, DIE = 4 };
	enum class UNIT_ROTATE : int { LEFT = 0, RIGHT = 1 };

	struct ABILITY_VALUE
	{
		int HP, ATT, DEF;
	};

	class UNIT_INFO
	{
		std::wstring Name;
		ABILITY_VALUE AbilityValue;
		int Size, Range, Exp;
	public:
		UNIT_INFO() = delete;
		UNIT_INFO(std::wstring, ABILITY_VALUE, int, int, int);

		std::wstring GetName();
		ABILITY_VALUE GetAbility();
		int GetSize();
		int GetRange();
		int GetExp();
	};

	class UNIT_TYPE
	{
		UNIT_TYPE() = default;
		~UNIT_TYPE() = default;

		static UNIT_TYPE* Instance;

		std::vector<UNIT_INFO> UnitType;
	public:
		static UNIT_TYPE* GetInstance();
		static void Destroy();

		void AddUnitInfo(std::wstring, ABILITY_VALUE, int, int, int);
		UNIT_INFO GetUnitInfo(int);
	};

	class UNIT
	{
		std::wstring Name;
		int TypeNum;
		bool Monster;

		UNIT_STATE UnitState;
		ABILITY_VALUE AbilityValue;
		int NowHp;
		
		D2D1_POINT_2F Locate;
		UNIT_ROTATE Rotate;

		float LastAttack, DeathTime;
		int Target;
	public:
		UNIT() = delete;
		UNIT(std::wstring, ABILITY_VALUE, int, D2D1_POINT_2F, UNIT_ROTATE);
		UNIT(int, D2D1_POINT_2F, UNIT_ROTATE);
		~UNIT() = default;

		std::wstring GetName();
		int GetTypeNum();
		bool isMonster();

		UNIT_STATE GetState();
		ABILITY_VALUE GetAbility();
		int GetNowHp();

		D2D1_POINT_2F GetLocate();
		UNIT_ROTATE GetRotate();

		int GetTarget();
		float GetLastAttack();
		float GetDeathTIme();

		void SetTarget(int);
		void Move(D2D1_POINT_2F);
		void SetState(UNIT_STATE);
		void Hit(int);
	};

	class MAP
	{
		int MapNum, UnitNum;
		std::vector<UNIT> Unit;
		float SpawnTime;
	private:
		void UnitAI(int);
		void SpawnUnit(int, D2D1_POINT_2F, UNIT_ROTATE);
		void EraseUnit(int);

		void UnitHit(int, int);
		void GiveExp(int, int);
		bool Crash(int, D2D1_RECT_F);
	public:
		MAP() = delete;
		MAP(int, int);
		~MAP() = default;

		void Work();

		void AddUnit(std::wstring, ABILITY_VALUE, int, D2D1_POINT_2F, UNIT_ROTATE);
		void UnitMove(int, D2D1_POINT_2F);
		void UnitChangeState(int, UNIT_STATE);
		void UnitChat(int, std::wstring);
		void UnitAttack(int, D2D1_RECT_F, int);

		template <typename T>
		void SendUnitInfo(std::shared_ptr<T> ptr)
		{
			Log::Logger::Instance()->L("setup user info");

			Packet::GameInfoBody gbody;
			gbody.map_idx = MapNum;
			gbody.object_idx = Unit.size() - 1;
			ptr.get()->send(gbody.Make_packet());
			
			std::vector<Packet::packet_ptr> packet_buf;
			{
				std::lock_guard<std::mutex>(ptr.get()->_game_room.mtx);
				for (auto i : Unit) {
					Packet::LoadUnitBody body;
					body.name = i.GetName();
					body.object_type = i.GetTypeNum();
					body.ismonster = i.isMonster();
					body.state = (int)i.GetState();
					body.fullhp = i.GetAbility().HP;
					body.nowhp = i.GetNowHp();
					body.x = (int)i.GetLocate().x;
					body.y = (int)i.GetLocate().y;
					body.rotate = (int)i.GetRotate();
					packet_buf.emplace_back(body.Make_packet());

					std::cout << std::endl 
						<< i.GetTypeNum() << "  "
						<< i.isMonster() << "  "
						<< (int)i.GetState() << "  "
						<< i.GetAbility().HP << "  "
						<< i.GetAbility().ATT << "  "
						<< i.GetAbility().DEF << "  "
						<< i.GetLocate().x << "  "
						<< i.GetLocate().y << "  " << std::endl;
				}
			}
			for (auto i : packet_buf)
			{
				ptr.get()->send(i);
			}
			Packet::InfoBody ibody(Packet::PACKET_EVENT::END_LOAD);
			ptr.get()->send(ibody.Make_packet());
			
		}
	};
}