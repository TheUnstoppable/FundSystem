#ifndef FUNDEMENT
#define FUNDEMENT
#pragma once

#include "General.h"
#include "da_chatcommand.h"
#include "da_gameobj.h"
#include "da_ssgm.h"
#include "da_event.h"
#include "da_translation.h"
#include <stdio.h>
#include <string>

#define FUNDCOLOR 0,255,146

enum BUILDING_TYPE
{
	POWERPLANT,
	REFINERY,
	VEHICLEFACTORY,
	SOLDIERFACTORY,
	DEFENSE,
	CONSTRUCTIONYARD,
	COMMUNICATIONCENTER,
	TIBERIUMSILO,
	REPAIRPAD,
	SHRINE,
	UNKNOWN
};

class MCFund : public DAEventClass, public DAChatCommandClass
{
	public:
	MCFund();
	~MCFund();
	SList<cPlayer> PlayerList;
	DynamicVectorClass<StringClass> NameList;
	virtual bool Activate(cPlayer *Player, const DATokenClass &Text, TextMessageEnum ChatType);
	virtual void Team_Change_Event(cPlayer* Player);
	//virtual void Kill_Event(DamageableGameObj* Victim, ArmedGameObj* Killer, float Damage, unsigned int Warhead, float Scale, DADamageType::Type Type);
	virtual void Player_Join_Event(cPlayer *Player);
	virtual void Level_Loaded_Event();
	virtual void Settings_Loaded_Event();

	bool IsABuildingDied;

	static const char *ppkeys[5];
	static const char *refkeys[7];
	static const char *vehfackeys[7];
	static const char *solfackeys[6];
	static const char *defensekeys[8];
	static const char *conyardkeys[4];
	static const char *commcenterkeys[4];
	static const char *silokeys[4];
	static const char *reppadkeys[4];
	static const char *shrinekeys[4];

	bool FundEnabled;
	bool ConstFundValue;
	bool Debug;
	float Multiplier;

	int GDI_PPFund;
	int Nod_PPFund;
	int GDI_RefFund;
	int Nod_RefFund;
	int GDI_WFFund;
	int Nod_WFFund;
	int GDI_SFFund;
	int Nod_SFFund;
	int GDI_DefFund;
	int Nod_DefFund;
	int GDI_CYFund;
	int Nod_CYFund;
	int GDI_CCFund;
	int Nod_CCFund;
	int GDI_SilFund;
	int Nod_SilFund;
	int GDI_RPFund;
	int Nod_RPFund;
	int GDI_ShrFund;
	int Nod_ShrFund;

	int GDIMaxPlayers;
	int NodMaxPlayers;

	bool PPFundable;
	bool RefFundable;
	bool WFFundable;
	bool SFFundable;
	bool DefFundable;
	bool CYFundable;
	bool CCFundable;
	bool SilFundable;
	bool RPFundable;
	bool ShrFundable;

	int PPTick;
	int RefTick;
	int WFTick;
	int SFTick;
	int DefTick;
	int CYTick;
	int CCTick;
	int SilTick;
	int RPTick;
	int ShrTick;

	BUILDING_TYPE FindType(StringClass Type)
	{
		if (FindItem(ppkeys, 5, Type))
		{
			return BUILDING_TYPE::POWERPLANT;
		}
		else if (FindItem(refkeys, 7, Type))
		{
			return BUILDING_TYPE::REFINERY;
		}
		else if (FindItem(vehfackeys, 7, Type))
		{
			return BUILDING_TYPE::VEHICLEFACTORY;
		}
		else if (FindItem(solfackeys, 6, Type))
		{
			return BUILDING_TYPE::SOLDIERFACTORY;
		}
		else if (FindItem(conyardkeys, 4, Type))
		{
			return BUILDING_TYPE::CONSTRUCTIONYARD;
		}
		else if (FindItem(commcenterkeys, 4, Type))
		{
			return BUILDING_TYPE::COMMUNICATIONCENTER;
		}
		else if (FindItem(silokeys, 4, Type))
		{
			return BUILDING_TYPE::TIBERIUMSILO;
		}
		else if (FindItem(reppadkeys, 4, Type))
		{
			return BUILDING_TYPE::REPAIRPAD;
		}
		else if (FindItem(shrinekeys, 4, Type))
		{
			return BUILDING_TYPE::SHRINE;
		}
		else if (FindItem(defensekeys, 8, Type))
		{
			return BUILDING_TYPE::DEFENSE;
		}
		else
		{
			return BUILDING_TYPE::UNKNOWN;
		}
	}

	bool NameListContains(StringClass toFind)
	{
		for (int i = 0; i < NameList.Count(); i++)
		{
			if (std::string(NameList[i]) == std::string(toFind))
			{
				return true;
			}
		}
		return false;
	}

	void SetMaxPlayers()
	{
		for (SLNode<cPlayer>* i = PlayerList.Head(); i; i = i->Next())
		{
			cPlayer* Player = i->Data();
			if (Commands->Get_Player_Type(Player->Get_GameObj()) == 1)
			{
				GDIMaxPlayers++;
			}
			else if (Commands->Get_Player_Type(Player->Get_GameObj()) == 0)
			{
				NodMaxPlayers++;
			}
			NameList.Add(StringClass(Player->Get_Name()));
		}
	}

	int Get_Team_Players(int Team)
	{
		int temp = 0;
		for (SLNode<cPlayer>* i = PlayerList.Head(); i; i = i->Next())
		{
			cPlayer* Player = i->Data();
			if (Commands->Get_Player_Type(Player->Get_GameObj()) == Team)
			{
				temp++;
			}
		}
		return temp;
	}

	void Revive_Building(int Team, BUILDING_TYPE Building)
	{
		GameObject* Bld = Find_Building_By_Type(Team, Building);
		if (Bld)
		{
			Restore_Building(Bld);
			DA::Color_Message(FUNDCOLOR, "[Fund] %ws funded their %s and rebuilded it.", Get_Wide_Team_Name(Team), DATranslationManager::Translate(Bld));
		}
	}

	GameObject *Find_Building_By_Type(int Team, BUILDING_TYPE Building)
	{
		GameObject* DeadBld = nullptr;
		if (Building == BUILDING_TYPE::COMMUNICATIONCENTER)
		{
			DeadBld = Find_Com_Center(Team);
		}
		else if (Building == BUILDING_TYPE::CONSTRUCTIONYARD)
		{
			DeadBld = Find_Construction_Yard(Team);
		}
		else if (Building == BUILDING_TYPE::DEFENSE)
		{
			DeadBld = Find_Base_Defense(Team);
		}
		else if (Building == BUILDING_TYPE::POWERPLANT)
		{
			DeadBld = Find_Power_Plant(Team);
		}
		else if (Building == BUILDING_TYPE::REFINERY)
		{
			DeadBld = Find_Refinery(Team);
		}
		else if (Building == BUILDING_TYPE::REPAIRPAD)
		{
			DeadBld = Find_Repair_Bay(Team);
		}
		else if (Building == BUILDING_TYPE::SHRINE)
		{
			DeadBld = Find_Shrine(Team);
		}
		else if (Building == BUILDING_TYPE::SOLDIERFACTORY)
		{
			DeadBld = Find_Soldier_Factory(Team);
		}
		else if (Building == BUILDING_TYPE::TIBERIUMSILO)
		{
			//DeadBld = Find_Repair_Bay(Team);
		}
		else if (Building == BUILDING_TYPE::VEHICLEFACTORY)
		{
			DeadBld = Find_Vehicle_Factory(Team);
		}

		return DeadBld;
	}

	bool Equals(StringClass string, const char* find)
	{
		return (std::string(string).find(find) != std::string::npos);
	}

	enum FundResult
	{
		Fundable,
		NotExist,
		Disabled,
		Alive,
	};

	int PlType(cPlayer* InsignificantCreature)
	{
		return Commands->Get_Player_Type(InsignificantCreature->Get_GameObj());
	}

	void SendFundMessage(cPlayer* Player, GameObject *Building, int Amount, int RequiredMoney, int *Fund)
	{
		DA::Team_Color_Message(PlType(Player), FUNDCOLOR, "[Fund] %ws funded %i credits for %s (%i/%i)", Player->Get_Name(), Amount, DATranslationManager::Translate(Building), *Fund, RequiredMoney);
	}

	BuildingGameObj* Find_TibSilo(int Team)
	{
		return Find_Building_By_Preset(Team, "Silo")->As_BuildingGameObj();
	}

	virtual FundResult Process_Fund(GameObject* Building, BUILDING_TYPE Type);
	virtual void Perform_Fund(BUILDING_TYPE Building, FundResult Result, const DATokenClass &Token, cPlayer *Requester, int* Tick, int* Fund);

	bool FindItem(const char* array[], int size, const char *target) {
		for (int counter = 0; counter < size; counter++)
		{
			if (std::string(array[counter]) == std::string(target))
			{
				return true;
			}
		}
		return false;
	}
};
const char * MCFund::ppkeys[5] = { "pp", "power", "plant", "powerplant", "power_plant" };
const char * MCFund::refkeys[7] = { "ref", "refinery", "tiberiumrefinery", "tiberium", "tib", "tibref", "tiberium_refinery" };
const char * MCFund::vehfackeys[7] = { "wf", "weapons_factory", "wepfac", "weapons", "factory", "air", "airstrip" };
const char * MCFund::solfackeys[6] = { "hon", "hand", "handofnod", "bar", "barracks", "infantry" };
const char * MCFund::defensekeys[8] = { "obl", "obi", "obby", "obelisk", "agt", "advanced", "gt", "ob" };
const char * MCFund::conyardkeys[4] = { "conyard", "cy", "construction", "yard" };
const char * MCFund::commcenterkeys[4] = { "cc", "comm", "commcenter", "communications" };
const char * MCFund::reppadkeys[4] = { "repair", "repfac", "reppad", "repair_pad" };
const char * MCFund::silokeys[4] = { "sil", "tiberiumsilo", "tibsilo", "silo" };
const char * MCFund::shrinekeys[4] = { "shrine", "temple", "son", "ton" };
#endif