// created by asturel based on business system, thx to Peter Steenbergen

#if defined _housing_included
#endinput
#endif

#define _housing_included
#pragma library housing
#include "base"
#include "utils\gtoutils"
#include "utils\dutils"
#include "utils\gtodudb"
#include "utils\dini"
#include "account"
#include "player"
#include "world"

forward HousesInit();
forward IsPlayerAtHouse(playerid);
forward OnPlayerEnterHousesCheckpoint(playerid);
forward GetHouseID(playerid);
forward HousesCommandHandler(playerid,text[]);
forward ganghouse(playerid);
forward gang2house(playerid,i);
forward HouseKeepUp();

#define MAX_HOUSEBUYSELL 500000
#define MAX_HOUSES 31
#define HOUSE_DELAY 600000

enum HouseInfo {
	Houses_Name[MAX_STRING],  // house name
	Houses_Cost,     // cost
	Houses_Owner[MAX_NAME],   // owner
	Houses_Gang[MAX_NAME],  // gang
	Houses_UpKeep,    // keep up
	Houses_UpKeepLeft,   // keep up left
	Houses_Buyout             // Buyout price
}

new Houses[MAX_HOUSES][HouseInfo] = {
	//NAME, COST, OWNER, GANG, UpKeep, UpKeepLeft, BUYOUT
	{"[LV]Four Dragons Mansion",1250000,"Unknown","Unknown",500,0,0},
	{"[LV]Caligula Mansion",850000,"Unknown","Unknown",500,0,0},
	{"[LS]Willowfield",17000,"Unknown","Unknown",50,0,0},
	{"[LS]Verdant Bluffs",17000,"Unknown","Unknown",50,0,0},
	{"[LS]Jefferson",30000,"Unknown","Unknown",50,0,0},
	{"[LS]Mulholland",190000,"Unknown","Unknown",250,0,0},
	{"[LS]Verona Beach",40000,"Unknown","Unknown",80,0,0},
	{"[LS]Santa Maria Beach",70000,"Unknown","Unknown",100,0,0},
	{"[LS]Palomino Creek",390000,"Unknown","Unknown",500,0,0},
	{"[LS]Blueberry",30000,"Unknown","Unknown",50,0,0},
	{"[LS]Dillimore",70000,"Unknown","Unknown",150,0,0},
	{"[SF]Angel Pine",80000,"Unknown","Unknown",50,0,0},
	{"[SF]Whetstone",200000,"Unknown","Unknown",300,0,0},
	{"[SF]Hashbury",90000,"Unknown","Unknown",100,0,0},
	{"[SF]Doherty",50000,"Unknown","Unknown",50,0,0},
	{"[SF]Queens",150000,"Unknown","Unknown",120,0,0},
	{"[SF]Chinatown",120000,"Unknown","Unknown",50,0,0},
	{"[SF]Paradiso",120000,"Unknown","Unknown",50,0,0},
	{"[SF]Calton Heights",200000,"Unknown","Unknown",300,0,0},
	{"[LV]El Quebradas",120000,"Unknown","Unknown",50,0,0},
	{"[LV]Tierra Robada",120000,"Unknown","Unknown",50,0,0},
	{"[LV]Fort Carson",130000,"Unknown","Unknown",100,0,0},
	{"[LV]Whitewood Estates",130000,"Unknown","Unknown",100,0,0},
	{"[LV]Prickle Pine",250000,"Unknown","Unknown",200,0,0},
	{"[LV]Redsands West",230000,"Unknown","Unknown",100,0,0},
	{"[LV]Pirates In Mens Pants",46000,"Unknown","Unknown",20,0,0},
	{"[LV]The Camels Toe",46000,"Unknown","Unknown",20,0,0},
	{"[LV]Rockshore West",120000,"Unknown","Unknown",100,0,0},
	{"[LV]The Clowns Pocket",99000,"Unknown","Unknown",20,0,0},
	{"[LV]Old Venturas Strip",46000,"Unknown","Unknown",30,0,0},
	{"[LV]Creek",110000,"Unknown","Unknown",50,0,0}
};

new HousesCP[MAX_HOUSES][CoordInfo] = {
	{991.8711,-668.1383,121.9763},
	{300.2951,-1154.5029,81.3909},
	{2486.8389,-1997.4480,13.8343},
	{1687.8954,-2100.4861,13.8343},
	{2108.2073,-1280.1827,25.6875},
	{1325.8687,-633.3270,109.1349},
	{890.5464,-1638.7795,14.9616},
	{312.6361,-1772.2003,4.6282},
	{2238.4756,166.0871,28.1535},
	{206.6301,-109.8167,4.8965},
	{791.9865,-507.5847,18.0129},
	{-2080.1499,-2312.3855,30.6250},
	{-1442.8309,-1541.9430,101.7578},
	{-2456.8408,-131.7211,26.0440},
	{-2027.6254,-44.5550,38.4414},
	{-2421.6580,335.3531,35.7626},
	{-2210.9714,723.4057,49.4141},
	{-2703.1348,818.3945,49.9844},
	{-2103.9272,900.6589,76.7109},
	{-1534.4202,2649.6294,55.8359},
	{-1045.4006,1552.8932,33.3112},
	{-364.1446,1168.8844,19.7422},
	{926.3783,2010.8345,11.4609},
	{1282.0720,2525.0994,10.8203},
	{1405.6171,1900.5596,11.4609},
	{1969.4005,1623.1444,12.8625},
	{2233.6375,1288.4495,10.8203},
	{2446.2251,695.2101,11.4609},
	{2223.1616,1840.9108,10.8203},
	{2371.1787,2169.1052,10.8257},
	{2820.9004,2140.8269,14.6615}
};

new HousesDB[MAX_STRING] = "GTO/Housing/";

HousesLoadAll()
{
	if (ForceOverwrite) return;
	new temp[MAX_STRING];
	if (!dini_Exists(ConfigDB)) dini_Create(ConfigDB);
	set(temp,dini_Get(ConfigDB,"Houses_DB"));
	if (strlen(temp) > 0) set(HousesDB,temp);
	if (!dini_Exists(HousesDB))
	{

		HousesSaveAll();

		return;
	}
	for (new i=0;i<MAX_HOUSES;i++)
	{
		new housesdbname[MAX_STRING];
		format(housesdbname,sizeof(housesdbname),"%sGTO.Houses.%s.txt",HousesDB,Houses[i][Houses_Name]);
		if (!dini_Exists(housesdbname))
		{
			// continue;
			dini_Create(housesdbname); HousesSaveAll();
		}
		set(temp,dini_Get(housesdbname,"Name"));
		if (strlen(temp) > 0) set(Houses[i][Houses_Name],temp);
		set(temp,nullstr);
		set(temp,dini_Get(housesdbname,"Cost"));
		if (strlen(temp) > 0) Houses[i][Houses_Cost] = strval(temp);
		set(temp,nullstr);
		set(temp,dini_Get(housesdbname,"Owner"));
		if (strlen(temp) > 0) set(Houses[i][Houses_Owner],temp);
		set(temp,nullstr);
		set(temp,dini_Get(housesdbname,"Gang"));
		if (strlen(temp) > 0) set(Houses[i][Houses_Gang],temp);
		set(temp,nullstr);
		set(temp,dini_Get(housesdbname,"UpKeep"));
		if (strlen(temp) > 0) Houses[i][Houses_UpKeep] = strval(temp); 
		set(temp,nullstr);
		set(temp,dini_Get(housesdbname,"UpKeepLeft"));
		if (strlen(temp) > 0) Houses[i][Houses_UpKeepLeft] = strval(temp); 
		set(temp,nullstr);
		set(temp,dini_Get(housesdbname,"Buyout"));
		if (strlen(temp) > 0) Houses[i][Houses_Buyout] = strval(temp);
		set(temp,nullstr);
		
	}
	return;

}

HousesSaveAll()
{
	new temp[MAX_STRING];
	if (dini_Exists(ConfigDB))
	{
		dini_Set(ConfigDB,"Houses_DB",HousesDB);
	}

	for (new i=0;i<MAX_HOUSES;i++)
	{
		new housesdbname[MAX_STRING];
		format(housesdbname,sizeof(housesdbname),"%sGTO.Houses.%s.txt",HousesDB,Houses[i][Houses_Name]);
		if (!dini_Exists(housesdbname)) dini_Create(housesdbname);
		dini_Set(housesdbname,"Name",Houses[i][Houses_Name]);
		set(temp,nullstr);

		valstr(temp,Houses[i][Houses_Cost]);
		dini_Set(housesdbname,"Cost",temp);
		set(temp,nullstr);

		valstr(temp,Houses[i][Houses_Owner]);
		dini_Set(housesdbname,"Owner",Houses[i][Houses_Owner]);
		set(temp,nullstr);

		valstr(temp,Houses[i][Houses_Gang]);
		dini_Set(housesdbname,"Gang",Houses[i][Houses_Gang]);
		set(temp,nullstr);

		valstr(temp,Houses[i][Houses_UpKeep]);
		dini_Set(housesdbname,"UpKeep",temp);
		set(temp,nullstr);

		valstr(temp,Houses[i][Houses_UpKeepLeft]);
		dini_Set(housesdbname,"UpKeepLeft",temp);
		set(temp,nullstr);

		valstr(temp,Houses[i][Houses_Buyout]);
		dini_Set(housesdbname,"Buyout",temp);
		set(temp,nullstr);

	}
	return;

}

public HousesInit()
{
	for (new id=0;id<MAX_HOUSES;id++)
	{
		AddActiveCheckpoint(HousesCP[id][Coord_X],HousesCP[id][Coord_Y],HousesCP[id][Coord_Z],20,2);
		AddStaticPickup(1273,49,HousesCP[id][Coord_X],HousesCP[id][Coord_Y],HousesCP[id][Coord_Z]);	//зеленый домик
	}
	WriteLog("Housing checkpoints initialised");
}

public IsPlayerAtHouse(playerid)
{
	for (new id=0;id<MAX_HOUSES;id++)
	{
		if (oIsPlayerInCheckpoint(playerid,HousesCP[id][Coord_X],HousesCP[id][Coord_Y],HousesCP[id][Coord_Z],10))
		{
			return 1;
		}
	}
	return 0;
}

public OnPlayerEnterHousesCheckpoint(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtHouse(playerid)) return;

	for (new id=0;id<MAX_HOUSES;id++)
	{
		if (oIsPlayerInCheckpoint(playerid,HousesCP[id][Coord_X],HousesCP[id][Coord_Y],HousesCP[id][Coord_Z],10))
		{
			new temp[MAX_STRING];
			format(temp,sizeof(temp), lang_texts[8][1] ,Houses[id][Houses_Name]);
			SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
		}
	}
}

public GetHouseID(playerid)
{
	new lookupid=-1;
	for (new id=0;id<MAX_HOUSES;id++)
	{
		if (oIsPlayerInCheckpoint(playerid,HousesCP[id][Coord_X],HousesCP[id][Coord_Y],HousesCP[id][Coord_Z],10)) lookupid = id;
	}
	return lookupid;
}


public HousesCommandHandler(playerid,text[]) 
{
	if (!IsPlayerConnected(playerid)) return 0;

	new cmd[20];
	new idx;

	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if (strlen(cmd) == 0) return 0;

	//-- houseinfo --
	if((strcomp(cmd, "/houseinfo", true) == 1) || (strcomp(cmd, "/hinfo", true) == 1))
	{
		if (!IsPlayerAtHouse(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[8][2]  , 0,COLOUR_RED);
			return 1;
		} 
		else 
		{
			new id=GetHouseID(playerid);
			if (id > -1)
			{
				new temp[MAX_STRING];
				format(temp,sizeof(temp), lang_texts[8][3] ,Houses[id][Houses_Name]);
				SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
				set(temp,nullstr);
				if(strcomp(Houses[id][Houses_Owner], "Unknown", true) == 0 && strcomp(Houses[id][Houses_Gang], "Unknown", true) == 0)
				{
					format(temp,sizeof(temp), lang_texts[8][4] ,Houses[id][Houses_Gang],Houses[id][Houses_Owner]);
					SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
					set(temp,nullstr);
					new price;
					price = Houses[id][Houses_Cost] + Houses[id][Houses_Buyout];
					new UpKeep;
					UpKeep = Houses[id][Houses_UpKeep];
					if(strcomp(Houses[id][Houses_Owner], oGetPlayerName(playerid), true) == 0 || strcomp(Houses[id][Houses_Gang], PlayerGangName[playerid], true) == 0)
					{
						format(temp,sizeof(temp), lang_texts[8][5] ,price);
						SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
						set(temp,nullstr);
						
						format(temp,sizeof(temp), lang_texts[8][6] ,UpKeep);
						SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
						set(temp,nullstr);
						
						format(temp,sizeof(temp), lang_texts[8][7] ,Houses[id][Houses_UpKeepLeft], UpKeep*2800);
						SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
						set(temp,nullstr);
						
						
						SendPlayerFormattedText(playerid,  lang_texts[8][8] , 0,COLOUR_GREEN);
					} 
				} 
				else 
				{
					SendPlayerFormattedText(playerid,  lang_texts[8][9] , 0,COLOUR_GREEN);
					format(temp,sizeof(temp), lang_texts[8][10] ,Houses[id][Houses_Cost]);
					SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
					set(temp,nullstr);
					
					format(temp,sizeof(temp), lang_texts[8][11] ,Houses[id][Houses_UpKeep],Houses[id][Houses_UpKeepLeft]);
					SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
					set(temp,nullstr);
					
					SendPlayerFormattedText(playerid,  lang_texts[8][12] , 0,COLOUR_GREEN);
				}
			} else { SendPlayerFormattedText(playerid,  lang_texts[8][13] , 0,COLOUR_RED); }
			return 1;
		}
	}
	//---------------------

	//-- buyhouses --
	if((strcomp(cmd, "/housebuy", true) == 1) || (strcomp(cmd, "/hbuy", true) == 1))
	{
		if (!IsPlayerAtHouse(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[8][14]  , 0,COLOUR_RED);
			return 1;
		} else {
			new id=GetHouseID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if (PlayerGangid[playerid] != 0) {
						new price;
						price = Houses[id][Houses_Cost] + Houses[id][Houses_Buyout];
						if(Player[playerid][Money] >= price)
						{
							if(strcomp(Houses[id][Houses_Owner], oGetPlayerName(playerid), true) == 1)
							{
								SendPlayerFormattedText(playerid,  lang_texts[8][15] , 0,COLOUR_RED);
							} 
							else 
							{
								new owner=-1;
								for (new ownerid = 0; ownerid < MAX_PLAYERS; ownerid++)
								{
									if (IsPlayerConnected(ownerid))
									{
										if(strcomp(Houses[id][Houses_Owner], oGetPlayerName(ownerid), true) == 1)
										{
											format(temp,sizeof(temp), lang_texts[8][16] ,Houses[id][Houses_Name],oGetPlayerName(playerid));
											SendPlayerFormattedText(ownerid, temp, 0,COLOUR_RED);
											set(temp,nullstr);
											owner=ownerid;
										}
									}
								}
								oGivePlayerMoney(playerid,0-price,1);
								if (owner > -1)
								{
									oGivePlayerMoney(owner,0+price,1);
								} 
								else 
								{
									new dinifilename[MAX_STRING];
									format(dinifilename,sizeof(dinifilename),"%sGTO.Player.%s.txt",PlayerDB,EncodeName(Houses[id][Houses_Owner]));
									if (dini_Exists(dinifilename))
									{
										new tempmoney;
										tempmoney = strval(dini_Get(dinifilename,"Money"));
										tempmoney = tempmoney + price;
										valstr(temp,tempmoney);
										dini_Set(dinifilename,"Money",temp);
										set(temp,nullstr);
									}
								}
								set(Houses[id][Houses_Owner],oGetPlayerName(playerid));
								
								set(Houses[id][Houses_Gang],PlayerGangName[playerid]);
								
								Houses[id][Houses_Buyout] = 0;
								new dinifilename[MAX_STRING];
								format(dinifilename,sizeof(dinifilename),"%sGTO.Houses.%s.txt",HousesDB,Houses[id][Houses_Name]);
								if (dini_Exists(dinifilename))
								{
									dini_Set(dinifilename,"Owner",Houses[id][Houses_Owner]);
									dini_Set(dinifilename,"Gang",Houses[id][Houses_Gang]);
									
									valstr(temp,Houses[id][Houses_Buyout]);
									dini_Set(dinifilename,"Buyout",temp);
									set(temp,nullstr);
									
									
								}
								format(temp,sizeof(temp), lang_texts[8][17] ,Houses[id][Houses_Name]);
								SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
								set(temp,nullstr);
								
								new logstring[256];
								format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (house)",playerid,oGetPlayerName(playerid),Houses[id][Houses_Name]);
								WriteLog(logstring);
							}
						} else {
							SendPlayerFormattedText(playerid,  lang_texts[8][18] , 0,COLOUR_RED);
						}
					} else { SendPlayerFormattedText(playerid,  lang_texts[8][19] , 0,COLOUR_RED); }
				} else {
					SendPlayerFormattedText(playerid,  lang_texts[8][20] , 0,COLOUR_RED);
				}
			} else {
				SendPlayerFormattedText(playerid,  lang_texts[8][21] , 0,COLOUR_RED);
			}
			return 1;
		}
	}
	//---------------------

	//-- sell house --
	if((strcomp(cmd, "/housesell", true) == 1) || (strcomp(cmd, "/hsell", true) == 1))
	{
		if (!IsPlayerAtHouse(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[8][22]  , 0,COLOUR_RED);
			return 1;
		} else {
			new id=GetHouseID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(Houses[id][Houses_Owner], oGetPlayerName(playerid), true) == 0)
					{
						SendPlayerFormattedText(playerid,  lang_texts[8][23] , 0,COLOUR_RED);
					} else {
						new price;
						price = Houses[id][Houses_Cost] + Houses[id][Houses_Buyout];
						price = (price * 85) / 100;
						oGivePlayerMoney(playerid,0+price,1);
						set(Houses[id][Houses_Owner],"Unknown");
						Houses[id][Houses_Buyout] = 0;
						new dinifilename[MAX_STRING];
						format(dinifilename,sizeof(dinifilename),"%sGTO.Houses.%s.txt",HousesDB,Houses[id][Houses_Name]);
						if (dini_Exists(dinifilename))
						{
							dini_Set(dinifilename,"Owner","Unknown");
							dini_Set(dinifilename,"Gang","Unknown");
							dini_Set(dinifilename,"UpKeepLeft","Unknown");
							valstr(temp,Houses[id][Houses_Buyout]);
							dini_Set(dinifilename,"Buyout",temp);
							set(temp,nullstr);
						}
						format(temp,sizeof(temp), lang_texts[8][24] ,Houses[id][Houses_Name]);
						SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
						set(temp,nullstr);
						SendPlayerFormattedText(playerid,  lang_texts[8][25] , 0,COLOUR_GREEN);
						
						new logstring[256];
						format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (house)",playerid,oGetPlayerName(playerid),Houses[id][Houses_Name]);
						WriteLog(logstring);
					}
				} else {
					SendPlayerFormattedText(playerid,  lang_texts[8][26] , 0,COLOUR_RED);
					SendPlayerFormattedText(playerid,  lang_texts[8][27] , 0,COLOUR_RED);
				}
			} else {
				SendPlayerFormattedText(playerid,  lang_texts[8][28] , 0,COLOUR_RED);
			}
			return 1;
		}
	}
	//--------------------

//	//-- set buyout --
//	if(strcomp(cmd, "/sethbuyout", true) == 1)
//	{
//		new buyoutamount;
//		buyoutamount = strval(strcharsplit(text, idx,strchar(" ")));
//
//		if ((buyoutamount < -9999) || (buyoutamount == 0) || (buyoutamount > 9999) || (buyoutamount > Player[playerid][Money]))
//		{
//			SendPlayerFormattedText(playerid, lang_texts[8][29] , 0,COLOUR_RED);
//			return 1;
//		}
//
//		if (!IsPlayerAtHouse(playerid))
//		{
//			SendPlayerFormattedText(playerid,  lang_texts[8][30]  , 0,COLOUR_RED);
//			return 1;
//		} else {
//			new id=GetHouseID(playerid);
//			new temp[MAX_STRING];
//			if (id > -1)
//			{
//				if (IsPlayerRegistered(playerid))
//				{
//					if(strcomp(Houses[id][Houses_Owner], oGetPlayerName(playerid), true) == 0)
//					{
//						SendPlayerFormattedText(playerid,  lang_texts[8][31] , 0,COLOUR_RED);
//					} else if (GetPlayerMoney(playerid) < Houses[id][Houses_UpKeepLeft] || GetPlayerMoney(playerid) == 0){
//						SendPlayerFormattedText(playerid,  lang_texts[8][46] , 0,COLOUR_RED);
//						return 1;
//						} else {
//						new newbuyout;
//						newbuyout = Houses[id][Houses_Buyout] + buyoutamount;
//							
//						if(newbuyout < 0 || newbuyout > MAX_HOUSEBUYSELL)
//						{
//							SendPlayerFormattedText(playerid, lang_texts[8][32] , 0,COLOUR_RED);
//							return 1;
//						}
//						
//						oGivePlayerMoney(playerid,0-buyoutamount,1);
//						Houses[id][Houses_Buyout] = newbuyout;
//						new dinifilename[MAX_STRING];
//						format(dinifilename,sizeof(dinifilename),"%sGTO.Houses.%s.txt",HousesDB,Houses[id][Houses_Name]);
//						if (dini_Exists(dinifilename))
//						{
//							valstr(temp,Houses[id][Houses_Buyout]);
//							dini_Set(dinifilename,"Buyout",temp);
//							set(temp,nullstr);
//						}
//						if(buyoutamount > 0)
//						{
//							format(temp,sizeof(temp), lang_texts[8][33] ,Houses[id][Houses_Buyout]);
//							SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
//							set(temp,nullstr);
//							new logstring[256];
//							format(logstring, sizeof (logstring), "player: %d:  %s: increased the buy out of '%s' (house) to $%d",playerid,oGetPlayerName(playerid),Houses[id][Houses_Name],Houses[id][Houses_Buyout]);
//							WriteLog(logstring);
//						} else {
//							format(temp,sizeof(temp), lang_texts[8][34] ,Houses[id][Houses_Buyout]);
//							SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
//							set(temp,nullstr);
//							new logstring[256];
//							format(logstring, sizeof (logstring), "player: %d:  %s: decreased the buy out of '%s' (house) to $%d",playerid,oGetPlayerName(playerid),Houses[id][Houses_Name],Houses[id][Houses_Buyout]);
//							WriteLog(logstring);
//						}
//					}
//				} else {
//					SendPlayerFormattedText(playerid,  lang_texts[8][35] , 0,COLOUR_RED);
//					SendPlayerFormattedText(playerid,  lang_texts[8][36] , 0,COLOUR_RED);
//				}
//			} else {
//				SendPlayerFormattedText(playerid,  lang_texts[8][37] , 0,COLOUR_RED);
//			}
//		}
//		return 1;
//	}
//	//----------------------


	//-- houses list --
	if(strcomp(cmd, "/houses", true) == 1)
	{
		new temp[MAX_STRING];
		for (new id=0;id<MAX_HOUSES;id++)
		{
			if(strcomp(Houses[id][Houses_Owner], "Unknown", true) == 0)
			{
				format(temp,sizeof(temp), lang_texts[8][38] ,Houses[id][Houses_Name],Houses[id][Houses_Gang],Houses[id][Houses_Owner]);
				SendPlayerScrollingText(playerid,temp);
				set(temp,nullstr);
			} 
			else 
			{
				format(temp,sizeof(temp), lang_texts[8][39] ,Houses[id][Houses_Name]);
				SendPlayerScrollingText(playerid,temp);
				set(temp,nullstr);
			}
		}
		return 1;
	}
	//-------------------------

	//--  my houses list --
	if(strcomp(cmd, "/myhouses", true) == 1)
	{
		new temp[MAX_STRING];
		new count = 0;
		for (new id=0;id<MAX_HOUSES;id++)
		{
			if(strcomp(Houses[id][Houses_Gang], PlayerGangName[playerid], true) == 1)
			{
				count++;
				format(temp,sizeof(temp), lang_texts[8][40] ,Houses[id][Houses_Name],Houses[id][Houses_Owner],Houses[id][Houses_Cost]);
				SendPlayerScrollingText(playerid,temp);
				set(temp,nullstr);
				
				format(temp,sizeof(temp), lang_texts[8][41] ,Houses[id][Houses_UpKeep], Houses[id][Houses_UpKeepLeft]);
				SendPlayerScrollingText(playerid,temp);
				set(temp,nullstr);
				
			}
		}
		if(count < 1)
		{
			SendPlayerFormattedText(playerid,  lang_texts[8][42] , 0,COLOUR_RED);
		}
		return 1;
	}
	//-------------------------
	// -- upkeep -- 
	if(strcomp(cmd, "/upkeep", true) == 1)
	{
		if (!IsPlayerAtHouse(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[8][43]  , 0,COLOUR_RED);
			return 1;
		} 
		else 
		{
			new id=GetHouseID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcmp(Houses[id][Houses_Gang], PlayerGangName[playerid], true) == 0)
					{
						if (GetPlayerMoney(playerid) > Houses[id][Houses_UpKeepLeft]) {
							if(Houses[id][Houses_UpKeepLeft] > 0)
							{
								Player[playerid][Money] = Player[playerid][Money] - Houses[id][Houses_UpKeepLeft];
								SendPlayerFormattedText(playerid,  lang_texts[8][44] , 0,COLOUR_GREEN);
								Houses[id][Houses_UpKeepLeft] = 0;
								new dinifilename[MAX_STRING];
								format(dinifilename,sizeof(dinifilename),"%sGTO.Houses.%s.txt",HousesDB,Houses[id][Houses_Name]);
								if (dini_Exists(dinifilename))
								{
									valstr(temp,Houses[id][Houses_UpKeepLeft]);
									dini_Set(dinifilename,"UpKeepLeft",temp);
									set(temp,nullstr);
								}
							} else {  SendPlayerFormattedText(playerid,  lang_texts[8][45] , 0,COLOUR_RED); }
						} else { SendPlayerFormattedText(playerid,  lang_texts[8][46] , 0,COLOUR_RED); } 
					} else { SendPlayerFormattedText(playerid,  lang_texts[8][47] , 0,COLOUR_RED); }
				} else {
					SendPlayerFormattedText(playerid,  lang_texts[8][48] , 0,COLOUR_RED);
					SendPlayerFormattedText(playerid,  lang_texts[8][49] , 0,COLOUR_RED);
				}
			} else { SendPlayerFormattedText(playerid,  lang_texts[8][50] , 0,COLOUR_RED); }
		}
		return 1;
	}


	//--------------------------
	return 0;
}

public ganghouse(playerid) {
	new hova = -1;
	if (!IsPlayerRegistered(playerid)) return hova;
	if (!strlen(PlayerGangName[playerid])) return hova;
	for (new id=0;id<MAX_HOUSES;id++)
	{
		if (!strcmp(Houses[id][Houses_Gang], PlayerGangName[playerid], true)) 
		{
			hova = id;
			new tmp[MAX_STRING];
			format(tmp,sizeof(tmp), lang_texts[8][51] , Houses[id][Houses_Name], Houses[id][Houses_UpKeepLeft],Houses[id][Houses_UpKeep]*2800);
			SendPlayerFormattedText(playerid,tmp,0, COLOUR_RED);
		}

	}
	return hova;
}

public gang2house(playerid,i)
{
	SetPlayerPos(playerid, HousesCP[i][Coord_X], HousesCP[i][Coord_Y], HousesCP[i][Coord_Z]);
}


public HouseKeepUp()
{
	new temp[MAX_STRING];
	new upkeepamount;
	new upkeep;
	new upkeepleft;
	for (new id=0;id<MAX_HOUSES;id++)
	{
		if(strcmp(Houses[id][Houses_Gang], "Unknown", true) == 0) { } else 
		{
			upkeep = Houses[id][Houses_UpKeep];
			upkeepleft = Houses[id][Houses_UpKeepLeft];
			upkeepamount = upkeep + upkeepleft;
			if (upkeepamount < upkeep*2800) {
				Houses[id][Houses_UpKeepLeft ] = upkeepamount;

				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%sGTO.Houses.%s.txt",HousesDB,Houses[id][Houses_Name]);
				if (dini_Exists(dinifilename))
				{
					valstr(temp,upkeepamount);
					dini_Set(dinifilename,"UpKeepLeft",temp);
					set(temp,nullstr);
				}
				if (upkeepamount > upkeep*500) 
				{
					// pay or suck.. the choice is yours..
				}
			} 
			else 
			{ 
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%sGTO.Houses.%s.txt",HousesDB,Houses[id][Houses_Name]);
				if (dini_Exists(dinifilename)) {
					dini_Set(dinifilename,"Gang","Unknown");
					dini_Set(dinifilename,"Owner","Unknown");
					dini_Set(dinifilename,"UpKeepLeft","0");
				}   
			}
		} 
	}
}


