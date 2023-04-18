//
// Created: 05.09.06
// Aurthor: Iain Gilbert
//


#if defined _weapons_included
#endinput
#endif

#define _weapons_included
#pragma library weapons
#include "base"

forward AmmunationInit();
forward IsPlayerAtAmmunation(playerid);
forward OnPlayerEnterAmmoCheckpoint(playerid);
forward IsPlayerAllowedWeapon(playerid,weaponid);
forward BuyWeapon(playerid,weaponid,bullets);
forward AmmunationCommandHandler(playerid,text[]);

#define MAX_WEAPONS 50
#define MAX_BULLETS 999

enum WeaponInfo {
	Weapon_ID, // id number of weapon
	Weapon_Name[MAX_NAME], // weapon name
	Weapon_Allowed, // is weapon allowed
	Weapon_MaxBullets, // max bullets
	Weapon_Cost, // cost per bullet
	Weapon_Level // player level required to buy this weapon
}
#define WeaponsSize 47
new Weapons[WeaponsSize][WeaponInfo] = {

	//id, "Name" ,Allowed,bullets, cost , level
	{0, "Невооружен" , 0 , 1 , 0 , 0},

	{1, "Кастет" , 0 , 1 , 6000 , 3},
	{2, "Клюшка для гольфа" , 1 , 1 , 6000 , 2},
	{3, "Night Stick" , 0 , 1 , 10000 , 18},
	{4, "Нож" , 1 , 1 , 10000 , 4},
	{5, "Бейсбольная Бита" , 1 , 1 , 15000 , 7},
	{6, "Shovel" , 0 , 1 , 1000 , 0},
	{7, "Кий" , 1 , 1 , 1000 , 11},
	{8, "Меч Катана" , 1 , 1 , 20000 , 8},
	{9, "Бензопила" , 1 , 1 , 7500 , 6},
	{10, "Пурпурный Dildo" , 0 , 1 , 5000 , 9},
	{11, "Вибратор" , 0 , 1 , 5000 , 5},
	{12, "Длинный Белый Dildo" , 0 , 1 , 7000 , 15},
	{13, "Вибратор 2" , 0 , 1 , 5000 , 11},
	{14, "Букет Цветов" , 0 , 1 , 1000 , 22},
	{15, "Трость" , 1 , 1 , 1000 , 8},
	{16, "Граната" , 1 , 1 , 7500 , 16},
	{17, "Газовый Баллон" , 0 , 1 , 20000 , 25},
	{18, "Коктейль Молотова" , 1 , 1 , 30000 , 20},

	{19, "Неизвестное" , 0 , 1 , 1000 , 25},
	{20, "Неизвестное" , 0 , 1 , 1000 , 25},
	{21, "Неизвестное" , 0 , 1 , 1000 , 25},

	{22, "Пистолет" , 1 , 999 , 125 , 0},
	{23, "Пистолет с глушителем" , 1 , 999 , 125 , 10},
	{24, "Пустынный Орел" , 1 , 999 , 1200 , 12},
	{25, "Дробовик" , 1 , 999 , 1100 , 5},
	{26, "Разрывной Дробовик" , 1 , 999 , 2300 , 14},
	{27, "Боевой Дробовик" , 1 , 999 , 3750 , 15},
	{28, "Микро УЗИ (ПолуАвтомат)" , 1 , 999 , 2100 , 13},
	{29, "МП5(НК5) (ПолуАвтомат)" , 1 , 999 , 2200 , 18},
	{30, "АК47 (Автомат Калашникова)" , 1 , 999 , 1500 , 17},
	{31, "M4 (Автомат)" , 1 , 999 , 4300 , 19},
	{32, "Tec9 (ПолуАвтомат)" , 1 , 999 , 4150 , 7},
	{33, "Винтовка" , 1 , 999 , 11500 , 20},
	{34, "Снайперская Винтовка" , 1 , 1 , 31000 , 21},
	{35, "Гранатомет РПГ" , 1 , 50 , 99000 , 25},
	{36, "Реактивная Пусковая установка" , 1 , 50 , 99000 , 25},
	{37, "Огнемет" , 1 , 999 , 1000 , 24},
	{38, "Ручной Пулемет (Миниган)" , 0 , 999 , 5000 , 25},

	{39, "Заряженный Рюкзак" , 0 , 9 , 10000 , 25},
	{40, "Детонатор к Рюкзаку" , 0 , 1 , 100 , 25},

	{41, "Баллончик с краской" , 1 , 1 , 4000 , 23},
	{42, "Огнетушитель" , 1 , 999 , 1000 , 5},
	{43, "Фотокамера" , 0 , 1 , 6000 , 24},
	{44, "Очки ночного видения", 0 , 1 , 1000 , 0},
	{45, "Тепловые Очки" , 0 , 1 , 1000 , 0},
	{46, "Парашут" , 0 , 1 , 70000 , 24}
};

#define AmmunationsCount 3
new Ammunations[AmmunationsCount][CoordInfo] = {
	{291.0004,-84.5168,1001.5156}, // LVammunation1
	{293.2710,-108.9645,1001.5156}, // LSAmmunation1
	{290.3177,-38.7184,1001.5156} // LSAmmunation2
};

new WeaponDB[MAX_STRING] = "GTO/Weapon/";

WeaponLoadAll()
{
	if (ForceOverwrite) return;
	new temp[MAX_STRING];
	if (!dini_Exists(ConfigDB)) dini_Create(ConfigDB);
	set(temp,dini_Get(ConfigDB,"Weapon_DB"));
	if (strlen(temp) > 0) set(WeaponDB,temp);

	if (!dini_Exists(WeaponDB))
	{

		WeaponSaveAll();

		return;
	}
	for (new i=0;i<WeaponsSize;i++)
	{
		new weapondbname[MAX_STRING];
		format(weapondbname,sizeof(weapondbname),"%sGTO.Weapon.%s.txt",WeaponDB,Weapons[i][Weapon_Name]);
		set(temp,dini_Get(weapondbname,"Name"));
		if (strlen(temp) > 0) set(Weapons[i][Weapon_Name],temp);
		set(temp,nullstr);
		set(temp,dini_Get(weapondbname,"IsAllowed"));
		if (strlen(temp) > 0) Weapons[i][Weapon_Allowed] = strval(temp);
		set(temp,nullstr);
		set(temp,dini_Get(weapondbname,"Level"));
		if (strlen(temp) > 0) Weapons[i][Weapon_Level] = strval(temp);
		set(temp,nullstr);
		set(temp,dini_Get(weapondbname,"Price"));
		if (strlen(temp) > 0) Weapons[i][Weapon_Cost] = strval(temp);
		set(temp,nullstr);
	}
	return;
}

WeaponSaveAll()
{

	new temp[MAX_STRING];
	if (dini_Exists(ConfigDB))
	{
		dini_Set(ConfigDB,"Weapon_DB",WeaponDB);
	}

	for (new i=0;i<WeaponsSize;i++)
	{
		new weapondbname[MAX_STRING];
		format(weapondbname,sizeof(weapondbname),"%sGTO.Weapon.%s.txt",WeaponDB,Weapons[i][Weapon_Name]);
		if (!dini_Exists(weapondbname)) dini_Create(weapondbname);
		dini_Set(weapondbname,"Name",Weapons[i][Weapon_Name]);
		valstr(temp,Weapons[i][Weapon_Allowed]);
		dini_Set(weapondbname,"IsAllowed",temp);
		set(temp,nullstr);
		valstr(temp,Weapons[i][Weapon_Level]);
		dini_Set(weapondbname,"Level",temp);
		set(temp,nullstr);
		valstr(temp,Weapons[i][Weapon_Cost]);
		dini_Set(weapondbname,"Price",temp);
		set(temp,nullstr);
	}
	return;
}

public AmmunationInit()
{
	for (new id=0;id<AmmunationsCount;id++)
	{
		AddActiveCheckpoint(Ammunations[id][Coord_X],Ammunations[id][Coord_Y],Ammunations[id][Coord_Z],20,2);
	}
	WriteLog("Ammunition checkpoints initialised");
}

public IsPlayerAtAmmunation(playerid)
{
	for (new id=0;id<AmmunationsCount;id++)
	{
		if (oIsPlayerInCheckpoint(playerid,Ammunations[id][Coord_X],Ammunations[id][Coord_Y],Ammunations[id][Coord_Z],10))
		{
			return 1;
		}
	}
	return 0;
}

public OnPlayerEnterAmmoCheckpoint(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtAmmunation(playerid)) return;
	SendPlayerFormattedText(playerid,  lang_texts[16][1]  , 0,COLOUR_GREEN);
	SendPlayerFormattedText(playerid,  lang_texts[16][2]  , 0,COLOUR_GREEN);
	SendPlayerFormattedText(playerid,  lang_texts[16][3]  , 0,COLOUR_GREEN);
	SendPlayerFormattedText(playerid,  lang_texts[16][4]  , 0,COLOUR_GREEN);

}

oGetWeaponName(weaponid)
{
	new name[MAX_NAME];
	new lookupid=GetWeaponLookupID(weaponid);
	if (lookupid == -1) return name;
	set(name,Weapons[lookupid][Weapon_Name]);
	return name;
}

GetWeaponMaxAmmo(weaponid)
{
	new ammo;
	new lookupid=GetWeaponLookupID(weaponid);
	if (lookupid == -1) return ammo;
	ammo = Weapons[lookupid][Weapon_MaxBullets];
	return ammo;
}

public IsPlayerAllowedWeapon(playerid,weaponid)
{
	new lookupid=GetWeaponLookupID(weaponid);
	if (lookupid == -1) return 0;
	if (Weapons[lookupid][Weapon_Allowed] == 0) return 0;
	if (Weapons[lookupid][Weapon_Level] > GetPlayerLevel(playerid)) return 0;
	return 1;
}

IsWeaponHandToHand(weaponid)
{
	new weaponlookupid=GetWeaponLookupID(weaponid);
	if (weaponlookupid == -1) return 0;
	if (Weapons[weaponlookupid][Weapon_MaxBullets] == 1) return 1;
	return 0;
}

GetWeaponCost(weaponid)
{
	new weaponlookupid=GetWeaponLookupID(weaponid);
	if (weaponlookupid == -1) return 0;
	return Weapons[weaponlookupid][Weapon_Cost];
}

GetWeaponLookupID(weaponid)
{
	new lookupid=-1;
	for (new i=0;i<WeaponsSize;i++)
	{
		if (weaponid == Weapons[i][Weapon_ID])
		{
			lookupid = i;
		}
	}
	return lookupid;
}

IsWeapon(weaponid)
{
	if ((weaponid >= 0) && (weaponid <= 46)) return 1;
	return 0;
}

IsStoreWeapon(weaponid)
{

	if ((weaponid >= 22) && (weaponid <=32)) return 1;
	return 0;
	/*
22 Pistol, 
23 Silenced Pistol, 
24 Desert Eagle, 
25 Shotgun, 
26 Sawn Off Shotgun, 
27 Combat Shotgun, 
28 Micro Uzi (Mac 10), 
29 MP5, 
30 AK47, 
31 M4, 
32 Tec9,
*/
}

SendPlayerVendorWeapons(playerid)
{
	new string[MAX_STRING];
	SendPlayerFormattedText(playerid,  lang_texts[16][5]  , 0,COLOUR_GREEN);
	for (new weaponid=1;weaponid<MAX_WEAPONS;weaponid++)
	{
		if (IsPlayerAllowedWeapon(playerid,weaponid))
		{
			format(string,sizeof(string), lang_texts[16][6] ,oGetWeaponName(weaponid),GetWeaponCost(weaponid),weaponid);
			SendPlayerScrollingText(playerid, string);
		}
	}
}

public BuyWeapon(playerid,weaponid,bullets)
{
	new string[MAX_STRING];
	if ((weaponid < 1) || (weaponid > 45))
	{
		SendPlayerFormattedText(playerid,  lang_texts[16][7]  , 0,COLOUR_RED);
		return 0;
	}
	if ((bullets < 1) || (bullets > 999))
	{
		SendPlayerFormattedText(playerid,  lang_texts[16][8]  , 0,COLOUR_RED);
		return 0;
	}
	if (!IsPlayerAtAmmunation(playerid))
	{
		SendPlayerFormattedText(playerid,  lang_texts[16][9]  , 0,COLOUR_RED);
		return 0;
	}

	if (!IsPlayerAllowedWeapon(playerid,weaponid))
	{
		format(string,sizeof(string), lang_texts[16][10] ,oGetWeaponName(weaponid));
		SendPlayerFormattedText(playerid,string,0,COLOUR_RED);
		return 0;
	}
	new purchasecost;
	purchasecost = GetWeaponCost(weaponid) * bullets;
	if (purchasecost < 1)
	{
		format(string,sizeof(string), lang_texts[16][11] ,oGetWeaponName(weaponid));
		SendPlayerFormattedText(playerid, string,0,COLOUR_RED);
		return 0;
	}
	if (oGetPlayerMoney(playerid) < purchasecost)
	{
		format(string,sizeof(string), lang_texts[16][12]  , purchasecost);
		SendPlayerFormattedText(playerid, string,0,COLOUR_RED);
		return 0;
	}

	oGivePlayerMoney(playerid,0-purchasecost,1);
	oGivePlayerWeapon(playerid,weaponid,bullets);
	if (bullets > 1)
	{
		format(string,sizeof(string), lang_texts[16][13] ,bullets,oGetWeaponName(weaponid),purchasecost);
	}
	else
	{
		format(string,sizeof(string), lang_texts[16][14] ,oGetWeaponName(weaponid),purchasecost);
	}
	SendPlayerFormattedText(playerid, string, 0,COLOUR_GREEN);
	SendPlayerFormattedText(playerid,  lang_texts[16][15]  , 0,COLOUR_GREEN);
	return 1;
}

public AmmunationCommandHandler(playerid,text[]) // process player commands
{
	if (!IsPlayerRegistered(playerid))
	{
		return 0; // non registered players dont use weapon commands
	}

	new cmd[20];
	new idx;

	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if (strlen(cmd) == 0) return 0;

	if(strcomp(cmd, "/weapons", true) == 1)
	{

		SendPlayerOwnedWeapons(playerid);
		return 1;
	}

	if((strcomp(cmd, "/wl", true) == 1) || (strcomp(cmd, "/weaponlist", true) == 1))
	{
		if (!IsPlayerAtAmmunation(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[16][16]  , 0,COLOUR_RED);
			return 1;
		}
		SendPlayerVendorWeapons(playerid);
		return 1;
	}


	if((strcomp(cmd, "/wb", true) == 1) || (strcomp(cmd, "/weaponbuy", true) == 1))
	{
		if (!IsPlayerAtAmmunation(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[16][17]  , 0,COLOUR_RED);
			return 1;
		}

		// get weaponid
		new weaponid;
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if (strlen(cmd) == 0)
		{
			SendPlayerVendorWeapons(playerid);
			//SendPlayerFormattedText(playerid,  lang_texts[16][18]  , 0,COLOUR_RED);
			return 1;
		}
		weaponid = strval(cmd);
		if (weaponid <= 0)
		{
			SendPlayerFormattedText(playerid,  lang_texts[16][19]  , 0,COLOUR_RED);
			return 1;
		}

		// get bullets
		new bullets;
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if (strlen(cmd) == 0)
		{
			bullets=1;
		}
		else
		{
			bullets = strval(cmd);
		}
		if (bullets < 0)
		{
			SendPlayerFormattedText(playerid,  lang_texts[16][20]  , 0,COLOUR_RED);
			return 1;
		}
		BuyWeapon(playerid,weaponid,bullets);
		return 1;
	}

	if(strcomp(cmd, "/weapon", true) == 1)
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));

		if(strcomp(cmd, "help", true) == 1)
		{
			SendPlayerFormattedText(playerid,  lang_texts[16][21]  , 0,COLOUR_RED);
			return 1;
		}

		if (strcomp(cmd, "list", true) == 1)
		{
			if (!IsPlayerAtAmmunation(playerid))
			{
				SendPlayerFormattedText(playerid,  lang_texts[16][22]  , 0,COLOUR_RED);
				return 1;
			}
			SendPlayerVendorWeapons(playerid);
			return 1;
		}

		if(strcomp(cmd, "buy", true) == 1)
		{
			if (!IsPlayerAtAmmunation(playerid))
			{
				SendPlayerFormattedText(playerid,  lang_texts[16][23]  , 0,COLOUR_RED);
				return 1;
			}

			// get weaponid
			new weaponid;
			set(cmd,strcharsplit(text, idx,strchar(" ")));
			if (strlen(cmd) == 0)
			{
				SendPlayerVendorWeapons(playerid);
				//SendPlayerFormattedText(playerid,  lang_texts[16][24]  , 0,COLOUR_RED);
				return 1;
			}
			weaponid = strval(cmd);
			if (weaponid <= 0)
			{
				SendPlayerFormattedText(playerid,  lang_texts[16][25]  , 0,COLOUR_RED);
				return 1;
			}

			// get bullets
			new bullets;
			set(cmd,strcharsplit(text, idx,strchar(" ")));
			if (strlen(cmd) == 0)
			{
				bullets=1;
			}
			else
			{
				bullets = strval(cmd);
			}
			if (bullets < 0)
			{
				SendPlayerFormattedText(playerid,  lang_texts[16][26]  , 0,COLOUR_RED);
				return 1;
			}
			BuyWeapon(playerid,weaponid,bullets);
			return 1;
		}
	}
	return 0;
}

