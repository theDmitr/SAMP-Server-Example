#if defined _sampst2_included
  #endinput
#endif

#define _sampst2_included
#pragma library dutils
#include <dini>
new h,m,s;
new y,md,d;
new Spree[MAX_PLAYERS];
stock SAddPlayerKills(playerid, killid)
{	
	new str[256];
	str = SGetAccountStr(playerid);
	SWriteInt(str,"kills", dini_Int(str,"kills")+1);
	if(killid != 0)
	{
    	switch (killid)
		{
			case 24: { SWriteInt(str,"gun_24",SReadInt(str,"gun_24")+1); }
			case 26: { SWriteInt(str,"gun_26",SReadInt(str,"gun_26")+1); }
			case 28: { SWriteInt(str,"gun_28",SReadInt(str,"gun_28")+1); }
			case 29: { SWriteInt(str,"gun_29",SReadInt(str,"gun_29")+1); }
			case 30: { SWriteInt(str,"gun_30",SReadInt(str,"gun_30")+1); }
			case 31: { SWriteInt(str,"gun_31",SReadInt(str,"gun_31")+1); }
			case 32: { SWriteInt(str,"gun_32",SReadInt(str,"gun_32")+1); }
		}
	}
	Spree[playerid] += 1;
    if(SReadInt(str,"spree") < Spree[playerid]) { SWriteInt(str,"spree",Spree[playerid]); }
	return 1;
}
stock SAddPlayerDeaths(playerid, deathid)
{
    new str[256];
	str = SGetAccountStr(playerid);
	SWriteInt(str,"deaths",SReadInt(str,"deaths")+1);
	if(deathid != 0)
	{
    	switch (deathid)
		{
			case 24: { SWriteInt(str,"gun_24d",SReadInt(str,"gun_24d")+1); }
			case 28: { SWriteInt(str,"gun_28d",SReadInt(str,"gun_28d")+1); }
			case 29: { SWriteInt(str,"gun_29d",SReadInt(str,"gun_29d")+1); }
			case 30: { SWriteInt(str,"gun_30d",SReadInt(str,"gun_30d")+1); }
			case 31: { SWriteInt(str,"gun_31d",SReadInt(str,"gun_31d")+1); }
			case 32: { SWriteInt(str,"gun_32d",SReadInt(str,"gun_32d")+1); }
		}
	}
	return 1;
}
stock SCreatePlayerAccount(playerid)
{
	new str[256], pip[256];
	getdate(y, md, d);
	gettime(h, m, s);
	GetPlayerIp(playerid, pip, 256);
	str = SGetAccountStr(playerid);
	dini_Create(str);
	SWriteStr(str,"ip",pip);
	SWriteInt(str,"money",0);
	SWriteInt(str,"kills",0);
	SWriteInt(str,"deaths",0);
	SWriteInt(str, "posltimeh",h);
	SWriteInt(str, "posltimem",m);
	SWriteInt(str, "posltimes",s);
	SWriteInt(str, "posltimey",y);
	SWriteInt(str, "posltimemd",md);
	SWriteInt(str, "posltimed",d);
	SWriteInt(str, "gun_24",0);
	SWriteInt(str, "gun_26",0);
	SWriteInt(str, "gun_28",0);
	SWriteInt(str, "gun_29",0);
	SWriteInt(str, "gun_30",0);
	SWriteInt(str, "gun_31",0);
	SWriteInt(str, "gun_32",0);
	SWriteInt(str, "gun_24d",0);
	SWriteInt(str, "gun_26d",0);
	SWriteInt(str, "gun_28d",0);
	SWriteInt(str, "gun_29d",0);
	SWriteInt(str, "gun_30d",0);
	SWriteInt(str, "gun_31d",0);
	SWriteInt(str, "gun_32d",0);
	SWriteInt(str, "spree",0);
	return 1;
}
stock SWriteDateTime(playerid)
{
    new str[256];
	getdate(y, md, d);
	gettime(h, m, s);
	str = SGetAccountStr(playerid);
	SWriteInt(str, "posltimeh",h);
	SWriteInt(str, "posltimem",m);
	SWriteInt(str, "posltimes",s);
	SWriteInt(str, "posltimey",y);
	SWriteInt(str, "posltimemd",md);
	SWriteInt(str, "posltimed",d);
	return 1;
}
stock SSaveMoney(playerid)
{
    new str[256];
	new pmoney = GetPlayerMoney(playerid);
	str = SGetAccountStr(playerid);
	dini_IntSet(str,"money", pmoney);
	return 1;
}
stock SWriteIntP(playerid, key[], value)
{
	new playername[256], str[256];
	GetPlayerName(playerid, playername, 256);
	format(str,sizeof(str),"stats/%s", playername);
	dini_IntSet(str, key, value);
	return 1;
}
stock SWriteStrP(playerid, key[], value[])
{
	new playername[256], str[256];
	GetPlayerName(playerid, playername, 256);
	format(str,sizeof(str),"stats/%s", playername);
	dini_Set(str, key, value);
	return 1;
}
stock SReadIntP(playerid, key[])
{
	new playername[256], str[256];
	GetPlayerName(playerid, playername, 256);
	format(str,sizeof(str),"stats/%s", playername);
    return strval(dini_Get(str, key));
}
stock SReadStrP(playerid, key[])
{
	new playername[256], str[256];
	GetPlayerName(playerid, playername, 256);
	format(str,sizeof(str),"stats/%s", playername);
	return dini_Get(str, key);
}
stock SDelKeyP(playerid, key[])
{
	new playername[256], str[256];
	GetPlayerName(playerid, playername, 256);
	format(str,sizeof(str),"stats/%s", playername);
	dini_Unset(str, key)
	return 1;
}
stock SWriteInt(str[], key[], value)
{
	dini_IntSet(str, key, value);
	return 1;
}
stock SWriteStr(str[], key[], value[])
{
	dini_Set(str, key, value);
	return 1;
}
stock SReadInt(str[], key[])
{	
    return strval(dini_Get(str, key));
}
stock SReadStr(str[], key[])
{	
	new str2[256];
	str2[0]=0;
	str2 = dini_Get(str, key);
	return str2;
}
stock SDelKey(str[], key[])
{
	dini_Unset(str, key);
	return 1;
}
stock SAccountExists(playerid)
{
	new playername[256], str[256];
	GetPlayerName(playerid, playername, 256);
	format(str,sizeof(str),"stats/%s", playername);
	return dini_Exists(str);
}
stock SGetAccountStr(playerid)
{
	new playername[256], str[256];
	str[0]=0;
	GetPlayerName(playerid, playername, 256);
	format(str, 256,"stats/%s", playername);
	return str;
}