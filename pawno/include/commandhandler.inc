//
// Created: 05.09.06
// Aurthor: Iain Gilbert
//

#if defined _commandhandler_included
#endinput
#endif

#define _commandhandler_included
#pragma library commandhandler
#include "base"
#include "utils\gtoutils"
#include "player"
#include "account"

forward CommandHandler(playerid,text[]);

public CommandHandler(playerid,text[]) // process player commands
{
	if (!IsPlayerConnected(playerid)) return 0;
	new cmd[20];
	new idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));

	if(strcomp(cmd, "/sound", true) == 1)
	{
	new string[MAX_STRING];
	set(string,text);
	strdel(string, 0, idx);
	new soundid;
	soundid = strval(strcharsplit(text, idx,strchar(" ")));
	if (soundid <= 0)
	{
		SendPlayerFormattedText(playerid, lang_texts[1][15] , 0,COLOUR_RED);
		return 1;
	}
	new Float:playerx,Float:playery,Float:playerz;
	GetPlayerPos(playerid,playerx,playery,playerz);
	PlayerPlaySound(playerid,soundid,playerx,playery,playerz);
	format(string,sizeof(string), lang_texts[1][16] ,soundid);
	SendPlayerFormattedText(playerid,string, 0,COLOUR_XP_GOOD);
	return 1;
	}

	if(strcomp(cmd, "/dropammo", true) == 1)
	{
	ADMDropAmmo(playerid);
//	for (new i=1;i<MAX_WEAPONS;i++)
//		{
//		PlayerWeapons[playerid][i] = 0;
//		}
	SendPlayerFormattedText(playerid, lang_texts[1][33] , 0,COLOUR_WHITE);
	CheckPlayerLevel(playerid);
	return 1;
	}


	if(strcomp(cmd, "/skydive", true) == 1)
	{
	if (Player[playerid][Jailed] == 1) {return 0;}
	new string[MAX_STRING];
	SendPlayerFormattedText(playerid, lang_texts[1][19] , 0,COLOUR_WHITE);
	new Float:playerx,Float:playery,Float:playerz;
	GivePlayerWeapon(playerid,46,1);
	GetPlayerPos(playerid,playerx,playery,playerz);
	PlayerPlaySound(playerid,1057,0,0,0);
	SetPlayerPos(playerid,playerx,playery,playerz+1200);
//	format(string,sizeof(string), lang_texts[1][16] ,soundid);
//	SendPlayerFormattedText(playerid,string, 0,COLOUR_XP_GOOD);
	format(string,sizeof(string), lang_texts[1][20]);
	GameTextForPlayer(playerid,string,4000,5);
	format(string,sizeof(string), lang_texts[1][21] ,oGetPlayerName(playerid),playerid);
	SendClientMessageToRegistered(COLOUR_GREY, string);
	PlayerPlaySound(playerid,1039,0,0,0);
	return 1;
	}


	if(strcomp(cmd, "/help", true) == 1)
	{
	  SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][2]);
	  SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][3]);
	  SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][4]);
	  SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][5]);

//		for (new i=2; i<9; i++) SendPlayerScrollingText(playerid, lang_texts[15][i]);

	return 1;
	}

	if( (strcomp(cmd, "/objective", true) == 1) || (strcomp(cmd, "/info", true) == 1) )
	{
	  SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][6]);
	  SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][7]);
	  SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][8]);
	  SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][9]);
	  SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][10]);
	  SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][11]);
	  SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][12]);
	  SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][13]);

//		for (new i=10; i<17; i++) SendPlayerScrollingText(playerid, lang_texts[15][i]);

		return 1;
	}


	if(strcomp(cmd, "/commands", true) == 1)
	{
	  SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][15]);
	  SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][16]);
	  SendClientMessage(playerid, COLOUR_LIGHTGREEN, lang_texts[15][17]);
	  SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][18]);
	  SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][19]);
	  SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][20]);
	  SendClientMessage(playerid, COLOUR_LIGHTBLUE, lang_texts[15][21]);
	  SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][22]);
	  SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][23]);
	  SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][24]);
	  SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][25]);
	  SendClientMessage(playerid, COLOUR_LIGHTRED, lang_texts[15][26]);
	  SendClientMessage(playerid, COLOUR_PURPLE, lang_texts[15][27]);
	  SendClientMessage(playerid, COLOUR_WHITE, lang_texts[15][28]);


//		for (new i=18; i<61; i++) SendPlayerScrollingText(playerid, lang_texts[15][i] );
//		for (new i=84; i<92; i++) SendPlayerScrollingText(playerid, lang_texts[15][i] );
		return 1;
	}

	if((strcomp(cmd, "/stats", true) == 1) || (strcomp(cmd, "/status", true) == 1) || (strcomp(cmd, "/stat", true) == 1))
	{
		new string[MAX_STRING];
		SendClientMessage(playerid,COLOUR_GREEN,  lang_texts[15][35] );

		new sid;
		sid=Player[playerid][Status];
		if (sid>=1 && sid<=10) {format(string,sizeof(string),lang_texts[12][87],sid);}
		else if (sid>10) {format(string,sizeof(string),lang_texts[12][86],sid);}
		else {format(string,sizeof(string),lang_texts[12][88]);}
		//Status
		SendClientMessage(playerid,COLOUR_LIGHTRED,string);

		format(string, sizeof(string),  lang_texts[15][37] , GetLevelName(Player[playerid][Level]), PlayerGangName[playerid] );
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,string);
		format(string, sizeof(string),  lang_texts[15][38] , Player[playerid][Level], Player[playerid][XP], Player[playerid][Money], Player[playerid][Bank], GetPlayerTotalMoney(playerid) );
		SendClientMessage(playerid,COLOUR_LIGHTBLUE,string);
		format(string, sizeof(string),  lang_texts[15][39] , Player[playerid][Deaths], Player[playerid][Kills], Player[playerid][Jailed], Player[playerid][Muted] );
		SendClientMessage(playerid,COLOUR_LIGHTRED,string);
		return 1;
	}

	if(strcomp(cmd, "/level", true) == 1)
	{
		CheckPlayerLevel(playerid);
		new string[256];
		SendClientMessage(playerid,COLOUR_GREEN,  lang_texts[15][70] );
		format(string, sizeof(string),  lang_texts[15][71] , Player[playerid][Level]);
		SendClientMessage(playerid,COLOUR_XP_GOOD, string);
		format(string, sizeof(string),  lang_texts[15][72] , Player[playerid][XP]);
		SendClientMessage(playerid,COLOUR_XP_GOOD, string);
		format(string, sizeof(string),  lang_texts[15][73] , GetXPToLevel(playerid, Player[playerid][Level] + 1));
		SendClientMessage(playerid,COLOUR_XP_BAD, string);
		return 1;
	}

	if(strcomp(cmd, "/version", true) == 1)
	{
		new string[MAX_STRING];
		SendClientMessage(playerid, COLOUR_MISC,  lang_texts[15][74] );
		format(string, sizeof(string),  lang_texts[15][75] , VERSION);
		SendClientMessage(playerid, COLOUR_GREEN,string);
		SendClientMessage(playerid, COLOUR_MISC,  lang_texts[15][76] );
		return 1;
	}

	if(strcomp(cmd, "/kill", true) == 1)
	{
		SetPlayerHealth(playerid,0.0);
		return 1;
	}

	if(strcomp(cmd, "/handsup", true) == 1)
	{
	if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
		{
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_HANDSUP);
//		new Float:playerx,Float:playery,Float:playerz;
//		GetPlayerPos(playerid,playerx,playery,playerz);
//		PlayerPlaySound(playerid,1187,playerx,playery,playerz);
  	  	return 1;
		}
	}

 	if(strcomp(cmd, "/dance", true) == 1)
	{
	new dancestyle;
	dancestyle = strval(strcharsplit(text, idx,strchar(" ")));
	if ((dancestyle < 1) || (dancestyle > 3))
	{
	SendPlayerFormattedText(playerid, lang_texts[15][83] , 0,COLOUR_RED);
	return 1;
	}

	if(dancestyle == 1) {
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE1);
		} else if(dancestyle == 2) {
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE2);
		} else if(dancestyle == 3) {
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE3);
		}
	return 1;
	}



if(strcomp(cmd, "/admins", true) == 1)
{
new string[MAX_STRING];
new idsa=0;
SendClientMessage(playerid, COLOUR_WHITE, lang_texts[1][28]);

	for (new id = 0; id < MAX_PLAYERS; id++)
	{
		if ( (IsPlayerConnected(id) && IsPlayerAdmin(id)) || (IsPlayerConnected(id) && IsPlayerAdm(id)) )
		{
		format(string, sizeof(string), lang_texts[1][29] , oGetPlayerName(id), id);
		SendClientMessage(playerid, COLOUR_LIGHTBLUE,string);
		idsa++;
		}
	}
	for (new id = 0; id < MAX_PLAYERS; id++)
	{
		if ( IsPlayerConnected(id) && IsPlayerMod(id) )
		{
		format(string, sizeof(string), lang_texts[1][30] , oGetPlayerName(id), id);
		SendClientMessage(playerid, COLOUR_LIGHTGREEN,string);
		idsa++;
		}
	}

	if (idsa==0)
	{
	SendClientMessage(playerid, COLOUR_GREY,lang_texts[1][31]);
	}
return 1;
}


if(strcomp(cmd, "/time", true) == 1)
{
new string[MAX_STRING];
new hour,minute,second;
gettime(hour,minute,second);
if ( (hour <= 9) && (minute <= 9) ) {format(string,sizeof(string),"0%d:0%d",hour,minute);}
if ( (hour <= 9) && (minute > 9) )  {format(string,sizeof(string),"0%d:%d",hour,minute);}
if ( (hour > 9) && (minute <= 9) ) {format(string,sizeof(string),"%d:0%d",hour,minute);}
if ( (hour > 9) && (minute > 9) )  {format(string,sizeof(string),"%d:%d",hour,minute);}

GameTextForPlayer(playerid,string,999,1);
//SendClientMessage(playerid, COLOUR_WHITE,string );

PlayerPlaySound(playerid,1085,0,0,0);

format(string, sizeof(string),lang_texts[1][32], oGetPlayerName(playerid), playerid);
SendClientMessageToAll(COLOUR_GREY, string);
return 1;
}


	if(strcomp(cmd, "/givecash", true) == 1)
	{
		new string[256];
		new giveplayerid, moneys;
		new sendername[MAX_PLAYER_NAME];
		new playermoney;
		new giveplayer[MAX_PLAYER_NAME];
		new tmp[256];
		tmp = strcharsplit(text, idx,strchar(" "));

		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOUR_MISC,  lang_texts[15][77] );
			return 1;
		}
		giveplayerid = strval(tmp);

		tmp = strcharsplit(text, idx,strchar(" "));
		if(!strlen(tmp))
		{
			SendClientMessage(playerid, COLOUR_MISC,  lang_texts[15][78] );
			return 1;
		}
		moneys = strval(tmp);

		if (IsPlayerConnected(giveplayerid))
		{
			GetPlayerName(giveplayerid, giveplayer, sizeof(giveplayer));
			GetPlayerName(playerid, sendername, sizeof(sendername));
			playermoney = oGetPlayerMoney(playerid);
			if (moneys > 0 && playermoney >= moneys)
			{
				oGivePlayerMoney(playerid, (0 - moneys),1);
				oGivePlayerMoney(giveplayerid, moneys,1);
				format(string, sizeof(string),  lang_texts[15][79] , giveplayer,giveplayerid, moneys);
				SendClientMessage(playerid, COLOUR_MONEY_GOOD, string);
				format(string, sizeof(string),  lang_texts[15][80] , moneys, sendername, playerid);
				SendClientMessage(giveplayerid, COLOUR_MONEY_GOOD, string);

				new logstring[256];
				format(logstring, sizeof (logstring), "%s(playerid:%d) has transfered %d to %s(playerid:%d)",sendername, playerid, moneys, giveplayer, giveplayerid);
				WriteLog(logstring);
			}
			else
			{
				SendClientMessage(playerid, COLOUR_MISC,  lang_texts[15][81] );
			}
		}
		else
		{
			format(string, sizeof(string),  lang_texts[15][82] , giveplayerid);
			SendClientMessage(playerid, COLOUR_MISC, string);
		}
		return 1;
	}
	return 0;
}
