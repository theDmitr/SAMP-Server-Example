//
// Created:     27.11.06
// Aurthor:     Peter Steenbergen
//
#if defined _business_included
#endinput
#endif

#define _business_included
#pragma library business
#include "base"
#include "utils\gtoutils"
#include "utils\dutils"
#include "utils\gtodudb"
#include "utils\dini"
#include "account"
#include "player"
#include "world"

forward BusinessInit();
forward IsPlayerAtBusiness(playerid);
forward OnPlayerEnterBusinessCheckpoint(playerid);
forward GetBusinessID(playerid);
forward TurnAround();
forward BusinessCommandHandler(playerid,text[]);

#define MAX_BUSINESBUYSELL 1000000
#define MAX_BUSINESSES 31

#define TurnAroundDelay 200000 // Delay between businesses turnaround

enum BusinessInfo {
	Business_Name[MAX_STRING],  // business name
	Business_Cost,     // cost
	Business_Value,        // value
	Business_Level,            // minumum level
	Business_Owner[MAX_NAME],   // owner
	Business_Buyout,            // Buyout price
	Business_Vault              // Vault
}

new Businesses[MAX_BUSINESSES][BusinessInfo] = {
	//NAME,COST,VALUE,LEVEL,OWNER,BUYOUT
	{"Botique",35000,800,5,"Unknown",0,0},
	{"Shithole Bar",25000,1300,8,"Unknown",0,0},
	{"Tattoo Parlor",40000,600,3,"Unknown",0,0},
	{"Binco Clothing",15000,800,5,"Unknown",0,0},
	{"Train Hard Clothing",15000,800,5,"Unknown",0,0},
	{"Urban Clothing",15000,800,5,"Unknown",0,0},
	{"Victim Clothing",15000,800,5,"Unknown",0,0},
	{"Zip Clothing",15000,800,5,"Unknown",0,0},
	{"DidierSachs Clothing",20000,950,7,"Unknown",0,0},
	{"Gym",20000,1000,7,"Unknown",0,0},
	{"Zero RC",10000,900,3,"Unknown",0,0},
	{"Zombotech",35000,1600,12,"Unknown",0,0},
	{"Verdant Meadows Air Strip",20000,1200,7,"Unknown",0,0},
	{"Blueberry Liquor Store",15000,850,5,"Unknown",0,0},
	{"Club-Disco",50000,1600,10,"Unknown",0,0},
	{"Lil Probe-Inn",47000,1100,6,"Unknown",0,0},
	{"Sex Shop",40000,950,7,"Unknown",0,0},
	{"Strip Club",60000,1400,10,"Unknown",0,0},
	{"The Big Spread Ranch",48000,1450,9,"Unknown",0,0},
	{"Jizzys",58000,1675,13,"Unknown",0,0},
	{"The Sherman Dam",58000,1800,16,"Unknown",0,0},
	{"Cobra Marital Arts",15000,800,5,"Unknown",0,0},
	{"City Planning Department",35000,1600,12,"Unknown",0,0},
	{"Inside Track",20000,1200,7,"Unknown",0,0},
	{"Coutt and Schutz Auto",25000,1250,8,"Unknown",0,0},
	{"Ottos Auto",25000,1250,8,"Unknown",0,0},
	{"Wang Cars",25000,1250,8,"Unknown",0,0},
	{"Emerald Isle",160000,4100,17,"Unknown",0,0},
	{"The Visage",145000,3800,17,"Unknown",0,0},
	{"Caligulas",3750000,19900,20,"Unknown",0,0},
	{"Four Dragons",2050000,16000,20,"Unknown",0,0}
};

new BusinessCP[MAX_BUSINESSES][CoordInfo] = {
	{418.5547,-80.1667,1001.8047},
	{501.4927,-75.4323,998.7578},
	{-203.4864,-41.2045,1002.2734},
	{207.5640,-97.8188,1005.2578},
	{210.1111,-129.1273,1003.5152},
	{203.8557,-40.4717,1001.8047},
	{210.3724,-8.1825,1005.2109},
	{161.1875,-79.9915,1001.8047},
	{204.2810,-157.2614,1000.5234},
	{773.6138,-72.1616,1000.6484},
	{-2231.2478,131.9623,1035.4141},
	{-1951.5938,704.3567,46.5625},
	{418.1210,2536.8762,10.0000},
	{252.4851,-57.3515,1.5703},
	{500.7049,-13.3777,1000.6797},
	{-225.7790,1403.9459,27.7734},
	{-103.5525,-22.4661,1000.7188},
	{1212.3918,-10.9459,1000.9219},
	{1208.5027,-32.6044,1000.9531},
	{-2650.0667,1409.2106,906.2734},
	{-955.5327,1948.5414,9.0000},
	{768.2014,-36.9461,1000.6865},
	{357.0584,173.5787,1008.3820},
	{823.1819,3.2361,1004.1797},
	{2131.6455,-1148.2836,24.3898},
	{-1657.8334,1207.3041,7.2500},
	{-1957.5327,300.2131,35.4688},
	{2127.5940,2370.4255,10.8203},
	{2022.5179,1916.6848,12.3397},
	{2235.5408,1679.0402,1008.3594},
	{1993.9758,1017.8945,994.8906}
};

new BusinessDB[MAX_STRING] = "GTO/Business/";

BusinessLoadAll()
{
	if (ForceOverwrite) return;
	new temp[MAX_STRING];
	if (!dini_Exists(ConfigDB)) dini_Create(ConfigDB);
	set(temp,dini_Get(ConfigDB,"Business_DB"));
	if (strlen(temp) > 0) set(BusinessDB,temp);

	if (!dini_Exists(BusinessDB))
	{

		BusinessSaveAll();

		return;
	}
	for (new i=0;i<MAX_BUSINESSES;i++)
	{
		new businessdbname[MAX_STRING];
		format(businessdbname,sizeof(businessdbname),"%sGTO.Business.%s.txt",BusinessDB,Businesses[i][Business_Name]);
		if (!dini_Exists(businessdbname))
		{
			continue;
		}
		set(temp,dini_Get(businessdbname,"Name"));
		if (strlen(temp) > 0) set(Businesses[i][Business_Name],temp);
		set(temp,nullstr);
		set(temp,dini_Get(businessdbname,"Cost"));
		if (strlen(temp) > 0) Businesses[i][Business_Cost] = strval(temp);
		set(temp,nullstr);
		set(temp,dini_Get(businessdbname,"Value"));
		if (strlen(temp) > 0) Businesses[i][Business_Value] = strval(temp);
		set(temp,nullstr);
		set(temp,dini_Get(businessdbname,"Level"));
		if (strlen(temp) > 0) Businesses[i][Business_Level] = strval(temp);
		set(temp,nullstr);
		set(temp,dini_Get(businessdbname,"Owner"));
		if (strlen(temp) > 0) set(Businesses[i][Business_Owner],temp);
		set(temp,nullstr);
		set(temp,dini_Get(businessdbname,"Buyout"));
		if (strlen(temp) > 0) Businesses[i][Business_Buyout] = strval(temp);
		set(temp,nullstr);
		set(temp,dini_Get(businessdbname,"Vault"));
		if (strlen(temp) > 0) Businesses[i][Business_Vault] = strval(temp);
		set(temp,nullstr);
	}
	return;

}

BusinessSaveAll()
{
	new temp[MAX_STRING];
	if (dini_Exists(ConfigDB))
	{
		dini_Set(ConfigDB,"Business_DB",BusinessDB);
	}

	for (new i=0;i<MAX_BUSINESSES;i++)
	{
		new businessdbname[MAX_STRING];
		format(businessdbname,sizeof(businessdbname),"%sGTO.Business.%s.txt",BusinessDB,Businesses[i][Business_Name]);
		if (!dini_Exists(businessdbname)) dini_Create(businessdbname);
		dini_Set(businessdbname,"Name",Businesses[i][Business_Name]);
		valstr(temp,Businesses[i][Business_Cost]);
		dini_Set(businessdbname,"Cost",temp);
		set(temp,nullstr);
		valstr(temp,Businesses[i][Business_Value]);
		dini_Set(businessdbname,"Value",temp);
		set(temp,nullstr);
		valstr(temp,Businesses[i][Business_Level]);
		dini_Set(businessdbname,"Level",temp);
		set(temp,nullstr);
		dini_Set(businessdbname,"Owner",Businesses[i][Business_Owner]);
		valstr(temp,Businesses[i][Business_Buyout]);
		dini_Set(businessdbname,"Buyout",temp);
		set(temp,nullstr);
		valstr(temp,Businesses[i][Business_Vault]);
		dini_Set(businessdbname,"Vault",temp);
		set(temp,nullstr);
	}
	return;

}

public BusinessInit()
{
	for (new id=0;id<MAX_BUSINESSES;id++)
	{
		AddActiveCheckpoint(BusinessCP[id][Coord_X],BusinessCP[id][Coord_Y],BusinessCP[id][Coord_Z],20,2);
		AddStaticPickup(1274,49,BusinessCP[id][Coord_X],BusinessCP[id][Coord_Y],BusinessCP[id][Coord_Z]);	//синий домик
	}
	WriteLog("Business checkpoints initialised");
}

public IsPlayerAtBusiness(playerid)
{
	for (new id=0;id<MAX_BUSINESSES;id++)
	{
		if (oIsPlayerInCheckpoint(playerid,BusinessCP[id][Coord_X],BusinessCP[id][Coord_Y],BusinessCP[id][Coord_Z],10))
		{
			return 1;
		}
	}
	return 0;
}

public OnPlayerEnterBusinessCheckpoint(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtBusiness(playerid)) return;

	for (new id=0;id<MAX_BUSINESSES;id++)
	{
		if (oIsPlayerInCheckpoint(playerid,BusinessCP[id][Coord_X],BusinessCP[id][Coord_Y],BusinessCP[id][Coord_Z],10))
		{
			new temp[MAX_STRING];
			format(temp,sizeof(temp), lang_texts[3][1] ,Businesses[id][Business_Name]);
			SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
		}
	}
}

public GetBusinessID(playerid)
{
	new lookupid=-1;
	for (new id=0;id<MAX_BUSINESSES;id++)
	{
		if (oIsPlayerInCheckpoint(playerid,BusinessCP[id][Coord_X],BusinessCP[id][Coord_Y],BusinessCP[id][Coord_Z],10))
		{
			lookupid = id;
		}
	}
	return lookupid;
}

public TurnAround()
{
	new temp[MAX_STRING];
	new vaultamount;
	new vaultmax;
	for (new id=0;id<MAX_BUSINESSES;id++)
	{
		if(strcomp(Businesses[id][Business_Owner], "Unknown", true) == 0)
		{
			vaultmax = Businesses[id][Business_Value] * 5;
			vaultamount = Businesses[id][Business_Vault] + Businesses[id][Business_Value];
			if(vaultamount < vaultmax)
			{
				Businesses[id][Business_Vault] = vaultamount;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%sGTO.Business.%s.txt",BusinessDB,Businesses[id][Business_Name]);
				if (dini_Exists(dinifilename))
				{
					valstr(temp,Businesses[id][Business_Vault]);
					dini_Set(dinifilename,"Vault",temp);
					set(temp,nullstr);
				}
			}
		}
	}
}

public BusinessCommandHandler(playerid,text[]) // process player commands
{
	if (!IsPlayerConnected(playerid)) return 0;

	new cmd[20];
	new idx;

	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if (strlen(cmd) == 0) return 0;

	//-- businessinfo --
	if((strcomp(cmd, "/businessinfo", true) == 1) || (strcomp(cmd, "/binfo", true) == 1))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][2]  , 0,COLOUR_RED);
			return 1;
		} else {
			new id=GetBusinessID(playerid);
			if (id > -1)
			{
				new temp[MAX_STRING];
				format(temp,sizeof(temp), lang_texts[3][3] ,Businesses[id][Business_Name]);
				SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
				set(temp,nullstr);
				if(strcomp(Businesses[id][Business_Owner], "Unknown", true) == 0)
				{
					format(temp,sizeof(temp), lang_texts[3][4] ,Businesses[id][Business_Owner]);
					SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
					set(temp,nullstr);
					new price;
					price = Businesses[id][Business_Cost] + Businesses[id][Business_Buyout];
					if(strcomp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true) == 0)
					{
						format(temp,sizeof(temp), lang_texts[3][5] ,price);
						SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
						set(temp,nullstr);
						format(temp,sizeof(temp), lang_texts[3][6] ,Businesses[id][Business_Level]);
						SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
						set(temp,nullstr);
						format(temp,sizeof(temp), lang_texts[3][7] ,Businesses[id][Business_Value]);
						SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
						set(temp,nullstr);
						SendPlayerFormattedText(playerid,  lang_texts[3][8] , 0,COLOUR_GREEN);
					} else {
						price = (price * 85) / 100;
						if (Businesses[id][Business_Vault] > 0)
						{
							format(temp,sizeof(temp), lang_texts[3][9] ,Businesses[id][Business_Vault]);
							SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
							set(temp,nullstr);
							SendPlayerFormattedText(playerid,  lang_texts[3][10] , 0,COLOUR_GREEN);
						}
						format(temp,sizeof(temp), lang_texts[3][11] ,price);
						SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
						set(temp,nullstr);
						SendPlayerFormattedText(playerid,  lang_texts[3][12] , 0,COLOUR_GREEN);
					}
				} else {
					SendPlayerFormattedText(playerid,  lang_texts[3][13] , 0,COLOUR_GREEN);
					format(temp,sizeof(temp), lang_texts[3][14] ,Businesses[id][Business_Cost]);
					SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
					set(temp,nullstr);
					format(temp,sizeof(temp), lang_texts[3][15] ,Businesses[id][Business_Level]);
					SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
					set(temp,nullstr);
					format(temp,sizeof(temp), lang_texts[3][16] ,Businesses[id][Business_Value]);
					SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
					set(temp,nullstr);
					SendPlayerFormattedText(playerid,  lang_texts[3][17] , 0,COLOUR_GREEN);
				}
			} else {
				SendPlayerFormattedText(playerid,  lang_texts[3][18] , 0,COLOUR_RED);
			}
			return 1;
		}
	}
	//---------------------

	//-- buybusiness --
	if((strcomp(cmd, "/businessbuy", true) == 1) || (strcomp(cmd, "/bbuy", true) == 1))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][19]  , 0,COLOUR_RED);
			return 1;
		} else {
			new id=GetBusinessID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if (Player[playerid][Level] >= Businesses[id][Business_Level])
					{
						new price;
						price = Businesses[id][Business_Cost] + Businesses[id][Business_Buyout];
						if(Player[playerid][Money] >= price)
						{
							if(strcomp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true) == 1)
							{
								SendPlayerFormattedText(playerid,  lang_texts[3][20] , 0,COLOUR_RED);
							} else {
								new owner=-1;
								for (new ownerid = 0; ownerid < MAX_PLAYERS; ownerid++)
								{
									if (IsPlayerConnected(ownerid))
									{
										if(strcomp(Businesses[id][Business_Owner], oGetPlayerName(ownerid), true) == 1)
										{
											format(temp,sizeof(temp), lang_texts[3][21] ,Businesses[id][Business_Name],oGetPlayerName(playerid));
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
								} else {
									new dinifilename[MAX_STRING];
									format(dinifilename,sizeof(dinifilename),"%sGTO.Player.%s.txt",PlayerDB,EncodeName(Businesses[id][Business_Owner]));
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
								set(Businesses[id][Business_Owner],oGetPlayerName(playerid));
								Businesses[id][Business_Buyout] = 0;
								new dinifilename[MAX_STRING];
								format(dinifilename,sizeof(dinifilename),"%sGTO.Business.%s.txt",BusinessDB,Businesses[id][Business_Name]);
								if (dini_Exists(dinifilename))
								{
									dini_Set(dinifilename,"Owner",Businesses[id][Business_Owner]);
									valstr(temp,Businesses[id][Business_Buyout]);
									dini_Set(dinifilename,"Buyout",temp);
									set(temp,nullstr);
								}
								format(temp,sizeof(temp), lang_texts[3][22] ,Businesses[id][Business_Name]);
								SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
								set(temp,nullstr);

								new logstring[256];
								format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (business)",playerid,oGetPlayerName(playerid),Businesses[id][Business_Name]);
								WriteLog(logstring);

								if (Businesses[id][Business_Vault] > 0)
								{
									format(temp,sizeof(temp), lang_texts[3][23] ,Businesses[id][Business_Vault]);
									SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
									set(temp,nullstr);
								}
							}
						} else {
							SendPlayerFormattedText(playerid,  lang_texts[3][24] , 0,COLOUR_RED);
						}
					} else {
						format(temp,sizeof(temp), lang_texts[3][25] ,Businesses[id][Business_Level]);
						SendPlayerFormattedText(playerid, temp, 0,COLOUR_RED);
						set(temp,nullstr);
					}
				} else {
					SendPlayerFormattedText(playerid,  lang_texts[3][26] , 0,COLOUR_RED);
				}
			} else {
				SendPlayerFormattedText(playerid,  lang_texts[3][27] , 0,COLOUR_RED);
			}
			return 1;
		}
	}
	//---------------------

	//-- sellbusiness --
	if((strcomp(cmd, "/businesssell", true) == 1) || (strcomp(cmd, "/bsell", true) == 1))
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][28]  , 0,COLOUR_RED);
			return 1;
		} else {
			new id=GetBusinessID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true) == 0)
					{
						SendPlayerFormattedText(playerid,  lang_texts[3][29] , 0,COLOUR_RED);
					} else {
						new price;
						price = Businesses[id][Business_Cost] + Businesses[id][Business_Buyout];
						price = (price * 85) / 100;
						oGivePlayerMoney(playerid,0+price,1);
						set(Businesses[id][Business_Owner],"Unknown");
						Businesses[id][Business_Buyout] = 0;
						new dinifilename[MAX_STRING];
						format(dinifilename,sizeof(dinifilename),"%sGTO.Business.%s.txt",BusinessDB,Businesses[id][Business_Name]);
						if (dini_Exists(dinifilename))
						{
							dini_Set(dinifilename,"Owner","Unknown");
							valstr(temp,Businesses[id][Business_Buyout]);
							dini_Set(dinifilename,"Buyout",temp);
							set(temp,nullstr);
						}
						format(temp,sizeof(temp), lang_texts[3][30] ,Businesses[id][Business_Name]);
						SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
						set(temp,nullstr);
						SendPlayerFormattedText(playerid,  lang_texts[3][31] , 0,COLOUR_GREEN);
						if (Businesses[id][Business_Vault] > 0)
						{
							format(temp,sizeof(temp), lang_texts[3][32] ,Businesses[id][Business_Vault]);
							SendPlayerFormattedText(playerid, temp, 0,COLOUR_RED);
							set(temp,nullstr);
							SendPlayerFormattedText(playerid,  lang_texts[3][33] , 0,COLOUR_RED);
						}
						new logstring[256];
						format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (business)",playerid,oGetPlayerName(playerid),Businesses[id][Business_Name]);
						WriteLog(logstring);
					}
				} else {
					SendPlayerFormattedText(playerid,  lang_texts[3][34] , 0,COLOUR_RED);
					SendPlayerFormattedText(playerid,  lang_texts[3][35] , 0,COLOUR_RED);
				}
			} else {
				SendPlayerFormattedText(playerid,  lang_texts[3][36] , 0,COLOUR_RED);
			}
			return 1;
		}
	}
	//--------------------

//	//-- set buyout --
//	if(strcomp(cmd, "/setbbuyout_closed", true) == 1)
//	{
//		new buyoutamount;
//		buyoutamount = strval(strcharsplit(text, idx,strchar(" ")));
//
//		if ((buyoutamount < -9999) || (buyoutamount == 0) || (buyoutamount > 9999) || (buyoutamount > Player[playerid][Money]))
//		{
//			SendPlayerFormattedText(playerid, lang_texts[3][37] , 0,COLOUR_RED);
//			return 1;
//		}
//
//		if (!IsPlayerAtBusiness(playerid))
//		{
//			SendPlayerFormattedText(playerid,  lang_texts[3][38]  , 0,COLOUR_RED);
//			return 1;
//		} else {
//			new id=GetBusinessID(playerid);
//			new temp[MAX_STRING];
//			if (id > -1)
//			{
//				if (IsPlayerRegistered(playerid))
//				{
//					if(strcomp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true) == 0)
//					{
//						SendPlayerFormattedText(playerid,  lang_texts[3][39] , 0,COLOUR_RED);
//					} else {
//						new newbuyout;
//						newbuyout = Businesses[id][Business_Buyout] + buyoutamount;
//						if(newbuyout < 0 || newbuyout > MAX_BUSINESBUYSELL)
//						{
//							SendPlayerFormattedText(playerid, lang_texts[3][40] , 0,COLOUR_RED);
//							return 1;
//						}
//						oGivePlayerMoney(playerid,0-buyoutamount,1);
//						Businesses[id][Business_Buyout] = newbuyout;
//						new dinifilename[MAX_STRING];
//						//format(dinifilename,sizeof(dinifilename),"%sGTO.Business.%s.txt",BusinessDB,Businesses[id][Business_Name]);
//						if (dini_Exists(dinifilename))
//						{
//							valstr(temp,Businesses[id][Business_Buyout]);
//							dini_Set(dinifilename,"Buyout",temp);
//							set(temp,nullstr);
//						}
//						if(buyoutamount > 0)
//						{
//							format(temp,sizeof(temp), lang_texts[3][41] ,Businesses[id][Business_Buyout]);
//							SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
//							set(temp,nullstr);
//							new logstring[256];
//							format(logstring, sizeof (logstring), "player: %d:  %s: increased the buy out of '%s' (business) to $%d",playerid,oGetPlayerName(playerid),Businesses[id][Business_Name],Businesses[id][Business_Buyout]);
//							WriteLog(logstring);
//						} else {
//							format(temp,sizeof(temp), lang_texts[3][42] ,Businesses[id][Business_Buyout]);
//							SendPlayerFormattedText(playerid, temp, 0,COLOUR_GREEN);
//							set(temp,nullstr);
//							new logstring[256];
//							format(logstring, sizeof (logstring), "player: %d:  %s: decreased the buy out of '%s' (business) to $%d",playerid,oGetPlayerName(playerid),Businesses[id][Business_Name],Businesses[id][Business_Buyout]);
//							WriteLog(logstring);
//						}
//					}
//				} else {
//					SendPlayerFormattedText(playerid,  lang_texts[3][43] , 0,COLOUR_RED);
//					SendPlayerFormattedText(playerid,  lang_texts[3][44] , 0,COLOUR_RED);
//				}
//			} else {
//				SendPlayerFormattedText(playerid,  lang_texts[3][45] , 0,COLOUR_RED);
//			}
//		}
//		return 1;
//	}
//	//----------------------

	//-- collect vault --
	if(strcomp(cmd, "/collect", true) == 1)
	{
		if (!IsPlayerAtBusiness(playerid))
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][46]  , 0,COLOUR_RED);
			return 1;
		} else {
			new id=GetBusinessID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true) == 0)
					{
						SendPlayerFormattedText(playerid,  lang_texts[3][47] , 0,COLOUR_RED);
					} else {
						if(Businesses[id][Business_Vault] > 0)
						{
							oGivePlayerMoney(playerid,0+Businesses[id][Business_Vault],1);
							SendPlayerFormattedText(playerid,  lang_texts[3][48] , 0,COLOUR_GREEN);
							Businesses[id][Business_Vault] = 0;
							new dinifilename[MAX_STRING];
							format(dinifilename,sizeof(dinifilename),"%sGTO.Business.%s.txt",BusinessDB,Businesses[id][Business_Name]);
							if (dini_Exists(dinifilename))
							{
								valstr(temp,Businesses[id][Business_Vault]);
								dini_Set(dinifilename,"Vault",temp);
								set(temp,nullstr);
							}
						} else {
							SendPlayerFormattedText(playerid,  lang_texts[3][49] , 0,COLOUR_RED);
						}
					}
				} else {
					SendPlayerFormattedText(playerid,  lang_texts[3][50] , 0,COLOUR_RED);
					SendPlayerFormattedText(playerid,  lang_texts[3][51] , 0,COLOUR_RED);
				}
			} else {
				SendPlayerFormattedText(playerid,  lang_texts[3][52] , 0,COLOUR_RED);
			}
		}
		return 1;
	}
	//---------------------

	//-- businesses list --
	if(strcomp(cmd, "/businesses", true) == 1)
	{
		new temp[MAX_STRING];
		for (new id=0;id<MAX_BUSINESSES;id++)
		{
			if(strcomp(Businesses[id][Business_Owner], "Unknown", true) == 0)
			{
				format(temp,sizeof(temp), lang_texts[3][53] ,Businesses[id][Business_Name],Businesses[id][Business_Owner]);
				SendPlayerScrollingText(playerid,temp);
				set(temp,nullstr);
			} else {
				format(temp,sizeof(temp), lang_texts[3][54] ,Businesses[id][Business_Name]);
				SendPlayerScrollingText(playerid,temp);
				set(temp,nullstr);
			}
		}
		return 1;
	}
	//-------------------------

	//--  my businesses list --
	if(strcomp(cmd, "/mybusinesses", true) == 1)
	{
		new temp[MAX_STRING];
		new count = 0;
		for (new id=0;id<MAX_BUSINESSES;id++)
		{
			if(strcomp(Businesses[id][Business_Owner], oGetPlayerName(playerid), true) == 1)
			{
				count++;
				format(temp,sizeof(temp), lang_texts[3][55] ,Businesses[id][Business_Name],Businesses[id][Business_Vault]);
				SendPlayerScrollingText(playerid,temp);
				set(temp,nullstr);
			}
		}
		if(count < 1)
		{
			SendPlayerFormattedText(playerid,  lang_texts[3][56] , 0,COLOUR_RED);
		}
		return 1;
	}
	//-------------------------
	return 0;
}
