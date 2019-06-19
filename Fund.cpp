#include "General.h"
#include "engine.h"
#include "da.h"
#include "da_player.h"
#include "da_settings.h"
#include "Fund.h"
#include <string>
#include <stdio.h>

MCFund::MCFund()
{
	Console_Output("msg Loading Building Fund System Plugin... - By MasterCan\n");
	PlayerList = SList<cPlayer>();
	NameList = DynamicVectorClass<StringClass>();
	Register_Event(DAEvent::PLAYERJOIN);
	Register_Event(DAEvent::LEVELLOADED);
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::TEAMCHANGE);
}

MCFund::~MCFund()
{

}

bool MCFund::Activate(cPlayer *Player, const DATokenClass &Text, TextMessageEnum ChatType)
{
	if (FundEnabled)
	{
		if (std::string(Text[1]) == std::string("debug"))
		{
			if (Player->Get_DA_Player()->Get_Access_Level() >= DAAccessLevel::FULLMODERATOR)
			{
				if (!Debug)
				{
					Debug = true;
					DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Debug mode is enabled, it will be disabled on next map.");
					return false;
				}
				else
				{
					DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Debug mode is already enabled.");
					return false;
				}
			}
			else
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] You don't have enough permission to enable debug mode.");
				return false;
			}
		}
		BUILDING_TYPE Bld = FindType(Text[1]);
		int Team = PlType(Player);
		if (Bld == BUILDING_TYPE::COMMUNICATIONCENTER)
		{
			if (!CCFundable)
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Funding this building is disabled.");
				return false;
			}
			if(Team == 1)
			{ 
				Perform_Fund(Bld, Process_Fund(Find_Com_Center(PlType(Player)), Bld), Text, Player, &CCTick, &GDI_CCFund);
			}
			else if (Team == 0)
			{
				Perform_Fund(Bld, Process_Fund(Find_Com_Center(PlType(Player)), Bld), Text, Player, &CCTick, &Nod_CCFund);
			}
		}
		else if (Bld == BUILDING_TYPE::CONSTRUCTIONYARD)
		{
			if (!CYFundable)
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Funding this building is disabled.");
				return false;
			}
			if (Team == 1)
			{
				Perform_Fund(Bld, Process_Fund(Find_Construction_Yard(PlType(Player)), Bld), Text, Player, &CYTick, &GDI_CYFund);
			}
			else if (Team == 0)
			{
				Perform_Fund(Bld, Process_Fund(Find_Construction_Yard(PlType(Player)), Bld), Text, Player, &CYTick, &Nod_CYFund);
			}
		}
		else if (Bld == BUILDING_TYPE::DEFENSE)
		{
			if (!DefFundable)
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Funding this building is disabled.");
				return false;
			}
			if (Team == 1)
			{
				Perform_Fund(Bld, Process_Fund(Find_Base_Defense(PlType(Player)), Bld), Text, Player, &DefTick, &GDI_DefFund);
			}
			else if (Team == 0)
			{
				Perform_Fund(Bld, Process_Fund(Find_Base_Defense(PlType(Player)), Bld), Text, Player, &DefTick, &Nod_DefFund);
			}
		}
		else if (Bld == BUILDING_TYPE::POWERPLANT)
		{
			if (!PPFundable)
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Funding this building is disabled.");
				return false;
			}
			if (Team == 1)
			{
				Perform_Fund(Bld, Process_Fund(Find_Power_Plant(PlType(Player)), Bld), Text, Player, &PPTick, &GDI_PPFund);
			}
			else if (Team == 0)
			{
				Perform_Fund(Bld, Process_Fund(Find_Power_Plant(PlType(Player)), Bld), Text, Player, &PPTick, &Nod_PPFund);
			}
		}
		else if (Bld == BUILDING_TYPE::REFINERY)
		{
			if (!RefFundable)
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Funding this building is disabled.");
				return false;
			}
			if (Team == 1)
			{
				Perform_Fund(Bld, Process_Fund(Find_Refinery(PlType(Player)), Bld), Text, Player, &RefTick, &GDI_RefFund);
			}
			else if (Team == 0)
			{
				Perform_Fund(Bld, Process_Fund(Find_Refinery(PlType(Player)), Bld), Text, Player, &RefTick, &Nod_RefFund);
			}
		}
		else if (Bld == BUILDING_TYPE::REPAIRPAD)
		{
			if (!RPFundable)
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Funding this building is disabled.");
				return false;
			}
			if (Team == 1)
			{
				Perform_Fund(Bld, Process_Fund(Find_Repair_Bay(PlType(Player)), Bld), Text, Player, &RPTick, &GDI_RPFund);
			}
			else if (Team == 0)
			{
				Perform_Fund(Bld, Process_Fund(Find_Repair_Bay(PlType(Player)), Bld), Text, Player, &RPTick, &Nod_RPFund);
			}
		}
		else if (Bld == BUILDING_TYPE::SHRINE)
		{
			if (!ShrFundable)
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Funding this building is disabled.");
				return false;
			}
			if (Team == 1)
			{
				Perform_Fund(Bld, Process_Fund(Find_Shrine(PlType(Player)), Bld), Text, Player, &ShrTick, &GDI_ShrFund);
			}
			else if (Team == 0)
			{
				Perform_Fund(Bld, Process_Fund(Find_Shrine(PlType(Player)), Bld), Text, Player, &ShrTick, &Nod_ShrFund);
			}
		}
		else if (Bld == BUILDING_TYPE::SOLDIERFACTORY)
		{
			if (!SFFundable)
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Funding this building is disabled.");
				return false;
			}
			if (Team == 1)
			{
				Perform_Fund(Bld, Process_Fund(Find_Soldier_Factory(PlType(Player)), Bld), Text, Player, &SFTick, &GDI_SFFund);
			}
			else if (Team == 0)
			{
				Perform_Fund(Bld, Process_Fund(Find_Soldier_Factory(PlType(Player)), Bld), Text, Player, &SFTick, &Nod_SFFund);
			}
		}
		else if (Bld == BUILDING_TYPE::TIBERIUMSILO)
		{
			if (!SilFundable)
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Funding this building is disabled.");
				return false;
			}

			if (Team == 1)
			{
				Perform_Fund(Bld, Process_Fund(Find_TibSilo(PlType(Player)), Bld), Text, Player, &SilTick, &GDI_SilFund);
			}
			else if (Team == 0)
			{
				Perform_Fund(Bld, Process_Fund(Find_TibSilo(PlType(Player)), Bld), Text, Player, &SilTick, &Nod_SilFund);
			}
		}
		else if (Bld == BUILDING_TYPE::VEHICLEFACTORY)
		{
			if (!WFFundable)
			{
				DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Funding this building is disabled.");
				return false;
			}
			if (Team == 1)
			{
				Perform_Fund(Bld, Process_Fund(Find_Vehicle_Factory(PlType(Player)), Bld), Text, Player, &WFTick, &GDI_WFFund);
			}
			else if (Team == 0)
			{
				Perform_Fund(Bld, Process_Fund(Find_Vehicle_Factory(PlType(Player)), Bld), Text, Player, &WFTick, &Nod_WFFund);
			}
		}
		else
		{
			DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Cannot find building with key \"%s\".", Text[1]);
			return false;
		}
	}
	else
	{
		DA::Private_Color_Message(Player, FUNDCOLOR, "[Fund] Fund system is disabled.");
		return false;
	}
	return true;
}

Register_Full_Chat_Command(MCFund, "!fund|!rebuild", 1, DAAccessLevel::NONE, DAChatType::ALL)

void MCFund::Perform_Fund(BUILDING_TYPE Building, FundResult Result, const DATokenClass &Token, cPlayer *Requester, int *Tick, int *Fund)
{
	if (Result == FundResult::Alive)
	{
		DA::Private_Color_Message(Requester, FUNDCOLOR, "[Fund] You cannot fund a building which is alive.");
		return;
	}
	else if (Result == FundResult::Disabled)
	{
		DA::Private_Color_Message(Requester, FUNDCOLOR, "[Fund] Funds to this building is disabled.");
		return;
	}
	else if (Result == FundResult::NotExist)
	{
		DA::Private_Color_Message(Requester, FUNDCOLOR, "[Fund] This map does not have this building.");
		return;
	}
	else
	{
		if (Token.Size() == 1)
		{
			float PlayerMoney = Commands->Get_Money(Requester->Get_GameObj());
			if (PlayerMoney > 0.0f)
			{
				int *Pl;
				if (PlType(Requester) == 1)
				{
					Pl = &GDIMaxPlayers;
				}
				else if (PlType(Requester) == 0)
				{
					Pl = &NodMaxPlayers;
				}
				else
				{
					DA::Private_Color_Message(Requester, FUNDCOLOR, "[Fund] Your team cannot fund buildings.");
					return;
				}

				if (Debug)
				{
					if (!ConstFundValue)
					{
						DA::Host_Message("[Fund] Amount: %i, Value 2: %i, Total Needed: %i, Left: %i, Multiplier: %f, Tick/PL: %i/%i", (int)PlayerMoney, (int)PlayerMoney + (*Fund), (int)((*Tick) * ((*Pl) * Multiplier)), (int)((*Tick) * ((*Pl) * Multiplier) - (int)PlayerMoney + (*Fund)), Multiplier, (*Tick), *Pl);
					}
					else
					{
						DA::Host_Message("[Fund] Amount: %i, Value 2: %i, Total Needed: %i, Left: %i, Multiplier: %f, Tick/PL: %i/%i", (int)PlayerMoney, (int)PlayerMoney + (*Fund), *Tick, *Tick - ((int)PlayerMoney + (*Fund)), Multiplier, (*Tick), *Pl);
					}
				}


				if (ConstFundValue == false)
				{
					if (PlayerMoney + (*Fund) > (*Tick) * ((*Pl) * Multiplier))
					{
						Commands->Give_Money(Requester->Get_GameObj(), -(float)((*Tick) * ((*Pl) * Multiplier) - (int)PlayerMoney + (*Fund)), false);
						*Fund = 0;

						Revive_Building(PlType(Requester), Building);
					}
					else
					{
						Commands->Give_Money(Requester->Get_GameObj(), -PlayerMoney, false);
						*Fund += int(PlayerMoney);
						SendFundMessage(Requester, Find_Building_By_Type(PlType(Requester), Building), int(PlayerMoney), int((float)(*Tick) * ((float)(*Pl) * Multiplier)), Fund);
					}
				}
				else
				{
					if (PlayerMoney + (*Fund) > (*Tick))
					{
						Commands->Give_Money(Requester->Get_GameObj(), -(float)((*Tick) - (int)PlayerMoney + (*Fund)), false);
						*Fund = 0;

						Revive_Building(PlType(Requester), Building);
					}
					else
					{
						Commands->Give_Money(Requester->Get_GameObj(), -PlayerMoney, false);
						*Fund += int(PlayerMoney);
						SendFundMessage(Requester, Find_Building_By_Type(PlType(Requester), Building), int(PlayerMoney), *Tick, Fund);
					}
				}
			}
		}
		else if (Token.Size() == 2)
		{
			int* Pl;
			if (PlType(Requester) == 1)
			{
				Pl = &GDIMaxPlayers;
			}
			else if (PlType(Requester) == 0)
			{
				Pl = &NodMaxPlayers;
			}
			else
			{
				DA::Private_Color_Message(Requester, FUNDCOLOR, "[Fund] Your team cannot fund buildings.");
				return;
			}

			float PlayerMoney = (float)std::stoi(std::string(Token[2]));
			if (PlayerMoney > Commands->Get_Money(Requester->Get_GameObj()))
			{
				PlayerMoney = Commands->Get_Money(Requester->Get_GameObj());
			}

			if (Debug)
			{
				if (!ConstFundValue)
				{
					DA::Host_Message("[Fund] Amount: %i, Value 2: %i, Total Needed: %i, Left: %i, Multiplier: %f, Tick/PL: %i/%i", (int)PlayerMoney, (int)PlayerMoney + (*Fund), (int)((*Tick) * ((*Pl) * Multiplier)), (int)((*Tick) * ((*Pl) * Multiplier) - (int)PlayerMoney + (*Fund)), Multiplier, (*Tick), *Pl);
				}
				else
				{
					DA::Host_Message("[Fund] Amount: %i, Value 2: %i, Total Needed: %i, Left: %i, Multiplier: %f, Tick/PL: %i/%i", (int)PlayerMoney, (int)PlayerMoney + (*Fund), *Tick, *Tick - ((int)PlayerMoney + (*Fund)), Multiplier, (*Tick), *Pl);
				}
			}

			if (ConstFundValue == false)
			{
				if (PlayerMoney + (*Fund) > (*Tick) * ((*Pl) * Multiplier))
				{
					Commands->Give_Money(Requester->Get_GameObj(), -(float)(PlayerMoney - (*Fund)), false);
					*Fund = 0;

					Revive_Building(PlType(Requester), Building);
				}
				else
				{
					*Fund += int(PlayerMoney);
					Commands->Give_Money(Requester->Get_GameObj(), -PlayerMoney, false);
					if (*Fund >= (*Tick) * ((*Pl) * Multiplier))
					{
						*Fund = 0;
						Revive_Building(PlType(Requester), Building);
					}
					else
					{
						SendFundMessage(Requester, Find_Building_By_Type(PlType(Requester), Building), int(PlayerMoney), int((float)(*Tick)* ((float)(*Pl)* Multiplier)), Fund);
					}
				}
			}
			else
			{
				if (PlayerMoney + (*Fund) > (*Tick))
				{
					Commands->Give_Money(Requester->Get_GameObj(), -(float)(PlayerMoney - (*Fund)), false);
					*Fund = 0;

					Revive_Building(PlType(Requester), Building);
				}
				else
				{
					*Fund += int(PlayerMoney);
					Commands->Give_Money(Requester->Get_GameObj(), -PlayerMoney, false);
					if (*Fund >= (*Tick))
					{
						*Fund = 0;
						Revive_Building(PlType(Requester), Building);
					}
					else
					{
						SendFundMessage(Requester, Find_Building_By_Type(PlType(Requester), Building), int(PlayerMoney), int((float)(*Tick)), Fund);
					}
				}
			}
		}
	}
}

MCFund::FundResult MCFund::Process_Fund(GameObject* Object, BUILDING_TYPE Type)
{
	if (Type == BUILDING_TYPE::COMMUNICATIONCENTER)
	{
		if (!CCFundable) { return MCFund::Disabled; }
	}
	else if (Type == BUILDING_TYPE::CONSTRUCTIONYARD)
	{
		if (!CYFundable) { return MCFund::Disabled; }
	}
	else if (Type == BUILDING_TYPE::DEFENSE)
	{
		if (!DefFundable) { return MCFund::Disabled; }
	}
	else if (Type == BUILDING_TYPE::POWERPLANT)
	{
		if (!PPFundable) { return MCFund::Disabled; }
	}
	else if (Type == BUILDING_TYPE::REFINERY)
	{
		if (!RefFundable) { return MCFund::Disabled; }
	}
	else if (Type == BUILDING_TYPE::REPAIRPAD)
	{
		if (!RPFundable) { return MCFund::Disabled; }
	}
	else if (Type == BUILDING_TYPE::SHRINE)
	{
		if (!ShrFundable) { return MCFund::Disabled; }
	}
	else if (Type == BUILDING_TYPE::SOLDIERFACTORY)
	{
		if (!SFFundable) { return MCFund::Disabled; }
	}
	else if (Type == BUILDING_TYPE::TIBERIUMSILO)
	{
		if (!SilFundable) { return MCFund::Disabled; }
	}
	else if (Type == BUILDING_TYPE::VEHICLEFACTORY)
	{
		if (!WFFundable) { return MCFund::Disabled; }
	}

	if (Object)
	{
		if (Object->As_BuildingGameObj())
		{
			BuildingGameObj* Building = Object->As_BuildingGameObj();
			if (Building)
			{
				if (Building->Is_Destroyed())
				{
					return MCFund::Fundable;
				}
				else
				{
					return MCFund::Alive;
				}
			}
			else
			{
				return MCFund::NotExist;
			}
		}
		else
		{
			return MCFund::NotExist;
		}
	}
	else
	{
		return MCFund::NotExist;
	}
}

void MCFund::Settings_Loaded_Event()
{
	INISection* Section = DASettingsManager::Get_Section("Fund");
	if (Section)
	{
		for (INIEntry* i = Section->EntryList.First(); i && i->Is_Valid(); i = i->Next())
		{

			if (MCFund::Equals(StringClass(i->Entry), "EnableFund"))
			{
				if (std::stoi(std::string(i->Value)) == 1)
				{
					FundEnabled = true;
				}
				else
				{
					FundEnabled = false;
				}
			}
			else if (MCFund::Equals(StringClass(i->Entry), "ConstantFundAmount"))
			{
				if (std::stoi(std::string(i->Value)) == 1)
				{
					ConstFundValue = true;
				}
				else
				{
					ConstFundValue = false;
				}
			}
			else if (MCFund::Equals(StringClass(i->Entry), "FundAddMultiplier"))
			{
				float Temp = std::stof(std::string(i->Value));
				if (Temp <= 1.0f && Temp >= 0.1f)
				{
					Multiplier = Temp;
				}
				else
				{
					Multiplier = 1.0f;
				}
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_PowerPlant"))
			{
				PPTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_Refinery"))
			{
				RefTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_VehicleFactory"))
			{
				WFTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_SoldierFactory"))
			{
				SFTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_Defense"))
			{
				DefTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_Defense"))
			{
				DefTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_ConYard"))
			{
				CYTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_ComCenter"))
			{
				CCTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_TibSilo"))
			{
				SilTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_RepairPad"))
			{
				RPTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Tick_Shrine"))
			{
				ShrTick = std::stoi(std::string(i->Value));
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_PowerPlant"))
			{
				PPFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_Refinery"))
			{
				RefFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_VehicleFactory"))
			{
				WFFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_SoldierFactory"))
			{
				SFFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_Defense"))
			{
				DefFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_Defense"))
			{
				DefFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_ConYard"))
			{
				CYFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_ComCenter"))
			{
				CCFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_TibSilo"))
			{
				SilFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_RepairPad"))
			{
				RPFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else if (MCFund::Equals(StringClass(i->Entry), "Fund_Shrine"))
			{
				ShrFundable = (std::stoi(std::string(i->Value)) == 1);
			}
			else
			{
				Console_Output(StringClass::getFormattedString("[FUND] Invalid setting item detected! Entry: %s, Value: %s\n", i->Entry, i->Value));
			}
		}
	}
}

void MCFund::Team_Change_Event(cPlayer* Player)
{
	//I'm believe something wrong here.

	int* Pl;
	if (PlType(Player) == 1)
	{
		Pl = &GDIMaxPlayers;
	}
	else if (PlType(Player) == 0)
	{
		Pl = &NodMaxPlayers;
	}
	else
	{
		return;
	}

	if (Get_Team_Players(PlType(Player)) - 1 <= *Pl)
	{
		*Pl++;
	}
}

void MCFund::Player_Join_Event(cPlayer* Player)
{
	if (!NameListContains(StringClass(Player->Get_Name())))
	{
		NameList.Add(StringClass(Player->Get_Name()));
		if (Commands->Get_Player_Type(Player->Get_GameObj()) == 1)
		{
			GDIMaxPlayers++;
		}
		else if (Commands->Get_Player_Type(Player->Get_GameObj()) == 0)
		{
			NodMaxPlayers++;
		}
	}
}

void MCFund::Level_Loaded_Event()
{
	//Clearing everything.

	PlayerList.Remove_All();
	NameList.Clear();
	PlayerList = *Get_Player_List();
	SetMaxPlayers();
	if (GDIMaxPlayers < 1)
	{
		GDIMaxPlayers = 1;
	}
	else if (NodMaxPlayers < 1)
	{
		NodMaxPlayers = 1;
	}
	IsABuildingDied = false;
	Debug = false;
	GDI_PPFund = 0;
	Nod_PPFund = 0;
	GDI_RefFund = 0;
	Nod_RefFund = 0;
	GDI_WFFund = 0;
	Nod_WFFund = 0;
	GDI_SFFund = 0;
	Nod_SFFund = 0;
	GDI_DefFund = 0;
	Nod_DefFund = 0;
	GDI_CYFund = 0;
	Nod_CYFund = 0;
	GDI_CCFund = 0;
	Nod_CCFund = 0;
	GDI_SilFund = 0;
	Nod_SilFund = 0;
	GDI_RPFund = 0;
	Nod_RPFund = 0;
	GDI_ShrFund = 0;
	Nod_ShrFund = 0;
}

MCFund thisplugin;
extern "C" __declspec(dllexport) DAEventClass* Plugin_Init()
{
	return &thisplugin;
}