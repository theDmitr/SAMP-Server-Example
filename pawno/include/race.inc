//
// Created:     08.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_included
#endinput
#endif

#define _race_included
#pragma library race
#include "base"
#include "player"
#include "world"
#include "vehicles"

forward CheckRace();
forward RaceCommandHandler(playerid,text[]);
forward RegisterRace(name[]);
forward OnPlayerEnterRaceCheckpoint(playerid);

#define MAX_CP 100
#define MAX_RACES 50

#define RACE_STATE_DISABLED 0
#define RACE_STATE_SLEEPING 1
#define RACE_STATE_LINEUP 2
#define RACE_STATE_COUNTDOWN 3
#define RACE_STATE_RACING 4

#define RACE_TYPE_SPRINT 0
#define RACE_TYPE_LAPS 1
#define RACE_TYPE_STUNT 2

#define INVALID_RACE_ID 0

new RacesCount;


enum RaceInfo{
	race_name[MAX_NAME],    // name of race
	race_type,             // race state
	race_minlevel,         // minimum level required to enter race
	race_minracers,         // minimum racers needed to race
	race_maxracetime,       // max time player can take to complete a race
	race_frequency,        // frquency that race runs
	race_lineupdelay,       // time to wait for players to linup
	race_cashprize,   // cash earned for 1st position
	race_cashentry,   // Cash required to enter (earned by winner)
	race_xpprize,    // xp earned for 1st position
	race_xpbonus,   // xp earned per player still in race
	race_vehicles[MAX_VEHICLES], // vehicles allowed in race
Float:race_startheading // startline heading
}
new Race[MAX_RACES][RaceInfo];


enum RaceStatsInfo{
	race_state,             // race state
	race_questid,           // quest id race is assigned
	race_activecpid,        // active cp id race is assigned
	race_timer,       // race timer
	race_racercount,    // count of racers
	race_position           // how many ppl have completed race alrready
}
new RaceStats[MAX_RACES][RaceStatsInfo];


enum RacePlayerInfo{
	race_player_time,   // time player has been in race
	race_player_cp
}
new RacePlayerStats[MAX_PLAYERS][RacePlayerInfo];

enum RaceScoreInfo {
	race_score_player[MAX_NAME],
	race_score_time,
	race_score_vehicle[MAX_NAME]
};
new BestScore[MAX_RACES][RaceScoreInfo];


new RaceSize[MAX_RACES]; // number of checkpoints, including start and finish
new CPSize=8; // size of checkpoints
new Float:RaceCheckpoints[MAX_RACES][MAX_CP][CoordInfo]; //  someone tell me... why does this work? i thought pawn only suppors 2d arrays

//--------------------------------------------------------------

new RaceDB[MAX_STRING] = "GTO/Race/GTO.Races.txt";
new RaceBaseDB[MAX_STRING] = "GTO/Race/";
new RaceRecordDB[MAX_STRING] = "GTO/Race/Record/";

RaceLoadAll()
{
	if (ForceOverwrite) return;
	new temp[MAX_STRING];
	if (!dini_Exists(ConfigDB)) dini_Create(ConfigDB);
	set(temp,dini_Get(ConfigDB,"Race_DB"));
	if (strlen(temp) > 0) set(RaceDB,temp);
	set(temp,dini_Get(ConfigDB,"Race_Base_DB"));
	if (strlen(temp) > 0) set(RaceBaseDB,temp);
	set(temp,dini_Get(ConfigDB,"Race_Record_DB"));
	if (strlen(temp) > 0) set(RaceRecordDB,temp);
	if (!dini_Exists(RaceDB))
	{
		dini_Create(RaceDB);
	}

	for (new racedbid=0;racedbid<MAX_RACES;racedbid++)
	{ // load all our races from db
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname), lang_texts[10][1] ,racedbid);
		set(temp,dini_Get(RaceDB,cellname));
		if (strlen(temp) == 0) continue;
		if (!RaceBaseDBExists(temp)) continue;
		new raceid = RegisterRace(temp);
		if (raceid == INVALID_RACE_ID) continue;
		RaceLoadBaseDB(raceid);
		RaceLoadRecordDB(raceid);
	}

	return;
}

RaceBaseDBExists(racename[MAX_STRING])
{
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%sGTO.Race.%s.txt",RaceBaseDB,racename);
	if (!dini_Exists(rdbname))
	{
		return 0;
	}
	return 1;
}


RaceLoadBaseDB(raceid)
{ // load race from db
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%sGTO.Race.%s.txt",RaceBaseDB,Race[raceid][race_name]);
	if (!dini_Exists(rdbname))
	{
		return INVALID_RACE_ID;
	}
	set(temp,dini_Get(rdbname,"Name"));
	if (strlen(temp) > 0) set(Race[raceid][race_name],temp);
	set(temp,nullstr);

	if (raceid == INVALID_RACE_ID)
	{
		printf("Race: '%s' Failed to load. (db)",Race[raceid][race_name]);
		new logstring[256];
		format(logstring, sizeof (logstring), "Race (DB): %s Failed to load", Race[raceid][race_name]);
		WriteLog(logstring);
		return INVALID_RACE_ID;
	}

	set(temp,dini_Get(rdbname,"Race_Frequency"));
	if (strlen(temp) > 0) Race[raceid][race_frequency] = strval(temp);
	set(temp,nullstr);

	set(temp,dini_Get(rdbname,"Lineup_Delay"));
	if (strlen(temp) > 0) Race[raceid][race_lineupdelay] = strval(temp);
	set(temp,nullstr);

	set(temp,dini_Get(rdbname,"Min_Racers"));
	if (strlen(temp) > 0) Race[raceid][race_minracers] = strval(temp);
	set(temp,nullstr);

	set(temp,dini_Get(rdbname,"Min_Level"));
	if (strlen(temp) > 0) Race[raceid][race_minlevel] = strval(temp);
	set(temp,nullstr);

	set(temp,dini_Get(rdbname,"Cash_Prize"));
	if (strlen(temp) > 0) Race[raceid][race_cashprize] = strval(temp);
	set(temp,nullstr);

	set(temp,dini_Get(rdbname,"Cash_Entry"));
	if (strlen(temp) > 0) Race[raceid][race_cashentry] = strval(temp);
	set(temp,nullstr);

	set(temp,dini_Get(rdbname,"XP_Prize"));
	if (strlen(temp) > 0) Race[raceid][race_xpprize] = strval(temp);
	set(temp,nullstr);

	set(temp,dini_Get(rdbname,"XP_Bonus"));
	if (strlen(temp) > 0) Race[raceid][race_xpbonus] = strval(temp);
	set(temp,nullstr);

	set(temp,dini_Get(rdbname,"Race_Time"));
	if (strlen(temp) > 0) Race[raceid][race_maxracetime] = strval(temp);
	set(temp,nullstr);

	set(temp,dini_Get(rdbname,"Startline_Heading"));
	if (strlen(temp) > 0) Race[raceid][race_startheading] = floatstr(temp);
	set(temp,nullstr);

	CPSize = 8;
	new cpcount;
	for (new cpid=0;cpid<MAX_CP;cpid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname), lang_texts[10][2] ,cpid);
		set(temp,dini_Get(rdbname,cellname));
		if (strlen(temp) == 0) break;

		new Float:X;
		new Float:Y;
		new Float:Z;
		new idx=0;
		X = floatstr(strcharsplit(temp,idx,strchar(",")));
		Y = floatstr(strcharsplit(temp,idx,strchar(",")));
		Z = floatstr(strcharsplit(temp,idx,strchar(",")));
		set(temp,nullstr);
		if (( X== 0.0) && (Y == 0.0)) break;
		RaceCheckpoints[raceid][cpid][Coord_X] = X;
		RaceCheckpoints[raceid][cpid][Coord_Y] = Y;
		RaceCheckpoints[raceid][cpid][Coord_Z] = Z;
		cpcount++;
	}

	RaceSize[raceid] = cpcount;
	RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;
	RaceStats[raceid][race_timer] = MakeRaceSleepTime(raceid);
	printf("Race (DB): '%s' Loaded.",Race[raceid][race_name]);
	new logstring[256];
	format(logstring, sizeof (logstring), "Race (DB): %s Loaded", Race[raceid][race_name]);
	WriteLog(logstring);
	return raceid;
}

Float:GetRaceCPX(raceid,cpid)
{
	return RaceCheckpoints[raceid][cpid][Coord_X];
}
Float:GetRaceCPY(raceid,cpid)
{
	return RaceCheckpoints[raceid][cpid][Coord_Y];
}
Float:GetRaceCPZ(raceid,cpid)
{
	return RaceCheckpoints[raceid][cpid][Coord_Z];
}

MakeRaceSleepTime(raceid)
{
	new sleeptime;
	if (Race[raceid][race_frequency] == 0) Race[raceid][race_frequency] = 5;
	sleeptime = ((Race[raceid][race_frequency] * RacesCount) * 75);
	return sleeptime;
}
RaceLoadRecordDB(raceid)
{
	new rdbname[MAX_STRING];
	new temp[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%sGTO.Race.Record.%s.txt",RaceRecordDB,Race[raceid][race_name]);
	if (!dini_Exists(rdbname)) return;
	set(temp,dini_Get(rdbname,"Best_Time_Record"));
	if (strlen(temp) > 0) BestScore[raceid][race_score_time] = strval(temp);
	set(temp,nullstr);
	set(temp,dini_Get(rdbname,"Best_Time_Player"));
	if (strlen(temp) > 0) set(BestScore[raceid][race_score_player],temp);
	set(temp,nullstr);
	set(temp,dini_Get(rdbname,"Best_Time_Vehicle"));
	if (strlen(temp) > 0) set(BestScore[raceid][race_score_vehicle],temp);
	set(temp,nullstr);
}

RaceSaveAll()
{

	if (!dini_Exists(ConfigDB)) dini_Create(ConfigDB);
	dini_Set(ConfigDB,"Race_Base_DB",RaceBaseDB);
	dini_Set(ConfigDB,"Race_Record_DB",RaceRecordDB);

	if (!dini_Exists(RaceDB)) dini_Create(RaceDB);
	for (new raceid=1;raceid<=RacesCount;raceid++)
	{ // load all our races from db
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname), lang_texts[10][3] ,raceid);
		dini_Set(RaceDB,cellname,Race[raceid][race_name]);

		RaceSaveBaseDB(raceid);
		RaceSaveRecordDB(raceid);
	}
}

RaceSaveScores()
{
	if (!dini_Exists(ConfigDB)) dini_Create(ConfigDB);
	dini_Set(ConfigDB,"Race_Base_DB",RaceBaseDB);
	dini_Set(ConfigDB,"Race_Record_DB",RaceRecordDB);

	if (!dini_Exists(RaceDB)) dini_Create(RaceDB);
	for (new raceid=1;raceid<=RacesCount;raceid++)
	{ // load all our races from db
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname), lang_texts[10][4] ,raceid);
		dini_Set(RaceDB,cellname,Race[raceid][race_name]);
		RaceSaveRecordDB(raceid);
	}
}

RaceSaveBaseDB(raceid)
{ // save race to db
	if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) return;
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%sGTO.Race.%s.txt",RaceBaseDB,Race[raceid][race_name]);
	if (!dini_Exists(rdbname))
	{
		dini_Create(rdbname);
	}
	dini_Set(rdbname,"Name",Race[raceid][race_name]);

	valstr(temp,Race[raceid][race_frequency]);
	dini_Set(rdbname,"Race_Frequency",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_lineupdelay]);
	dini_Set(rdbname,"Lineup_Delay",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_minracers]);
	dini_Set(rdbname,"Min_Racers",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_minlevel]);
	dini_Set(rdbname,"Min_Level",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_cashprize]);
	dini_Set(rdbname,"Cash_Prize",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_cashentry]);
	dini_Set(rdbname,"Cash_Entry",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_xpprize]);
	dini_Set(rdbname,"XP_Prize",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_xpbonus]);
	dini_Set(rdbname,"XP_Bonus",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_maxracetime]);
	dini_Set(rdbname,"Race_Time",temp);
	set(temp,nullstr);

	format(temp,sizeof(temp),"%f",Race[raceid][race_startheading]);
	dini_Set(rdbname,"Startline_Heading",temp);
	set(temp,nullstr);

	for (new cpid=0;cpid<RaceSize[raceid];cpid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname), lang_texts[10][5] ,cpid);
		format(temp,sizeof(temp), lang_texts[10][6] ,RaceCheckpoints[raceid][cpid][Coord_X],RaceCheckpoints[raceid][cpid][Coord_Y],RaceCheckpoints[raceid][cpid][Coord_Z]);
		dini_Set(rdbname,cellname,temp);
	}

	new logstring[256];
	format(logstring, sizeof (logstring), "Race base saved: %s",Race[raceid][race_name]);
	WriteLog(logstring);
}

RaceSaveRecordDB(raceid)
{
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%sGTO.Race.Record.%s.txt",RaceRecordDB,Race[raceid][race_name]);
	if (!dini_Exists(rdbname)) dini_Create(rdbname);
	dini_Set(rdbname,"Name",Race[raceid][race_name]);
	set(temp,nullstr);
	valstr(temp,BestScore[raceid][race_score_time]);
	dini_Set(rdbname,"Best_Time_Record",temp);
	set(temp,nullstr);
	set(temp,BestScore[raceid][race_score_player]);
	dini_Set(rdbname,"Best_Time_Player",temp);
	set(temp,nullstr);
	set(temp,BestScore[raceid][race_score_vehicle]);
	dini_Set(rdbname,"Best_Time_Vehicle",temp);
	return;
}

public CheckRace() // must be ran by timer every second
{
	new string [MAX_STRING];
	for (new raceid=1; raceid<=RacesCount;raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;
		
		if (RaceStats[raceid][race_state] == RACE_STATE_SLEEPING)
		{
			if (RaceStats[raceid][race_timer] <= 0) // if it is time to run
			{
				RaceStats[raceid][race_timer] = 0;
				StartRaceLineup(raceid);
				continue;
			}
			RaceStats[raceid][race_timer]--;
			continue;
		}

		RaceStats[raceid][race_timer]++;
		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (RaceStats[raceid][race_timer] >= Race[raceid][race_lineupdelay]) // if it is time to run
			{
				RaceStats[raceid][race_timer] = 0;
				StartRaceCountdown(raceid);
			}
			else
			{
				if (RaceStats[raceid][race_racercount] > 0)
				{
					new racecountdown = Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer];
					if ((racecountdown == 5) || (racecountdown == 10) || (racecountdown == 20) || (racecountdown == 30) || (racecountdown == 45) || (racecountdown == 60) || (racecountdown == 90))
					{
						format(string, sizeof(string),  lang_texts[10][7] ,(Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer]));
						SendMessageToRacers(raceid,string,COLOUR_RACE);
						
					}
				}
			}
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_COUNTDOWN)
		{
			for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
			{
				if (IsPlayerConnected(playerid))
				{
					if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
					{
						new Countdown = MAX_COUNTDOWN - RaceStats[raceid][race_timer];
						if (Countdown >= 1)
						{
							format(string, sizeof(string), "%d",Countdown);
							PlayerPlaySound(playerid,1056,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]));
						}
						else
						{
							format(string, sizeof(string), "GO!");
							PlayerPlaySound(playerid,1057,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]));
						}
						GameTextForPlayer(playerid, string,1000,6);
					}
				}
			}
			if (RaceStats[raceid][race_timer] >= MAX_COUNTDOWN)
			{
				StartRace(raceid);
				RaceStats[raceid][race_timer] = 0;
			}
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
		{
			if (RaceStats[raceid][race_timer] >= Race[raceid][race_maxracetime]) // if time limit reached
			{
				EndRace(raceid);
				RaceStats[raceid][race_timer] = 0;
			}
		}
	}
}



public RaceCommandHandler(playerid,text[]) // process player commands
{
	if (!IsPlayerRegistered(playerid))
	{
		return 0; // non registered players dont use weapon commands
	}

	new cmd[20];
	new idx;
	//new string[MAX_STRING];

	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if (strlen(cmd) == 0) return 0;

	if(strcomp(cmd, "/races", true) == 1)
	{

		SendPlayerCurrentRaces(playerid);
		return 1;
	}

	if(strcomp(cmd, "/race", true) == 1)
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if(strcomp(cmd, "help", true) == 1)
		{
			SendPlayerScrollingText(playerid, lang_texts[10][8] );
			SendPlayerScrollingText(playerid, lang_texts[10][9] );
			SendPlayerScrollingText(playerid, lang_texts[10][10] );
			SendPlayerScrollingText(playerid, lang_texts[10][11] );
			SendPlayerScrollingText(playerid, lang_texts[10][12] );
			SendPlayerScrollingText(playerid, lang_texts[10][13] );
			SendPlayerScrollingText(playerid, lang_texts[10][14] );
			SendPlayerScrollingText(playerid, lang_texts[10][15] );
			SendPlayerScrollingText(playerid, lang_texts[10][16] );
			SendPlayerScrollingText(playerid, lang_texts[10][17] );
			return 1;
		}

		if(strcomp(cmd, "join", true) == 1)
		{
			new raceid;
			raceid = strval(strcharsplit(text, idx,strchar(" ")));
			if (PlayerQuest[playerid] != 0)
			{
				SendPlayerFormattedText(playerid, lang_texts[10][18] , 0,COLOUR_RED);
				return 1;
			}
			if ((raceid == 0) || (raceid >= MAX_RACES))
			{
				SendPlayerFormattedText(playerid, lang_texts[10][19] , 0,COLOUR_RED);
				return 1;
			}
			if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED)
			{
				SendPlayerFormattedText(playerid, lang_texts[10][20] , 0,COLOUR_RED);
				return 1;
			}
			if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
			{
				SendPlayerFormattedText(playerid, lang_texts[10][21] , 0,COLOUR_RED);
				return 1;
			}
			if (RaceStats[raceid][race_state] != RACE_STATE_LINEUP)
			{
				SendPlayerFormattedText(playerid, lang_texts[10][22] , 0,COLOUR_RED);
				return 1;
			}
			JoinRace(raceid,playerid);
			return 1;
		}
		if((strcomp(cmd, "leave", true) == 1) || (strcomp(cmd, "quit", true) == 1))
		{
			if (PlayerQuest[playerid] == 0)
			{
				SendPlayerFormattedText(playerid, lang_texts[10][23] , 0,COLOUR_RED);
				return 1;
			}
			new raceid = GetPlayerRace(playerid);
			if (raceid == 0)
			{
				SendPlayerFormattedText(playerid, lang_texts[10][24] , 0,COLOUR_RED);
				return 1;
			}
			LeaveRace(raceid,playerid);
			return 1;
		}
		return 0;
	}
	return 0;
}

AddRaceCP(raceid,Float:x,Float:y,Float:z)
{
	if (RaceSize[raceid] > MAX_CP) return 0;
	new cpid = RaceSize[raceid];
	RaceCheckpoints[raceid][cpid][Coord_X] = x;
	RaceCheckpoints[raceid][cpid][Coord_Y] = y;
	RaceCheckpoints[raceid][cpid][Coord_Z] = z;
	RaceSize[raceid]++;
	return 1;
}

RemoveLastRaceCP(raceid)
{
	if (RaceSize[raceid] < 1) return 0;
	new cpid = RaceSize[raceid]-1;
	RaceCheckpoints[raceid][cpid][Coord_X] = 0.0;
	RaceCheckpoints[raceid][cpid][Coord_Y] = 0.0;
	RaceCheckpoints[raceid][cpid][Coord_Z] = 0.0;
	RaceSize[raceid]--;
	return 1;
}

public RegisterRace(name[])
{
	for (new i=0;i<MAX_RACES;i++)
	{
		if (RaceStats[i][race_state] == RACE_STATE_DISABLED) continue;
		if (strcomp(name,Race[i][race_name],true)==1)
		{
			return INVALID_RACE_ID;
		}
	}

	RacesCount++;
	if (RacesCount >= MAX_RACES) return INVALID_RACE_ID;
	new raceid = RacesCount;
	RaceStats[raceid][race_questid] = RegisterQuest(name);
	if (RaceStats[raceid][race_questid] == INVALID_RACE_ID)
	{
		RacesCount--;
		return INVALID_RACE_ID;
	}
	set(Race[raceid][race_name],name);
	return raceid;
}

StartRace(raceid)
{
	RaceStats[raceid][race_state] = RACE_STATE_RACING;
	RaceStats[raceid][race_timer]=0;
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (IsPlayerConnected(playerid))
		{
			if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
			{
				TogglePlayerControllable(playerid, 1);
				NextRaceCP(raceid,playerid);
			}
		}
	}
}

JoinRace(raceid,playerid)
{
	new PlayerState = GetPlayerState(playerid);
	new string[MAX_STRING];
	if (oGetPlayerMoney(playerid) < Race[raceid][race_cashentry])
	{
		format(string, sizeof(string),  lang_texts[10][25] , Race[raceid][race_cashentry]);
		SendPlayerFormattedText(playerid,string, 0,COLOUR_RED);
		return;
	}
	if (GetPlayerLevel(playerid) < Race[raceid][race_minlevel])
	{
		format(string, sizeof(string),  lang_texts[10][26] , Race[raceid][race_minlevel]);
		SendPlayerFormattedText(playerid,string, 0,COLOUR_RED);
		return;
	}

	RaceStats[raceid][race_racercount]++;
	if (Race[raceid][race_minracers] > 1)
	{
		format(string, sizeof(string),  lang_texts[10][27] , oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: has joined the race. (Racers:%d/%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
		WriteLog(logstring);
	}
	else
	{
		format(string, sizeof(string),  lang_texts[10][28] , oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: has joined the race. (Racers:%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
		WriteLog(logstring);
	}
	SendMessageToRacers(raceid,string,COLOUR_WHITE);

	oGivePlayerMoney(playerid,0-Race[raceid][race_cashentry],1);
	GameTextForPlayer(playerid, "~g~Race Entered.",3000,6);
	PlayerQuest[playerid] = GetRaceQuestID(raceid);
	RacePlayerStats[playerid][race_player_cp] = 0;
	oSetPlayerCheckpoint(playerid,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]),CPSize);
	if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
	{
		format(string, sizeof(string),  lang_texts[10][29] ,(Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer]));
		SendPlayerFormattedText(playerid, string, 0,COLOUR_RACE);
	}

	if(PlayerState != PLAYER_STATE_DRIVER && PlayerState != PLAYER_STATE_PASSENGER)
	{
		SendPlayerFormattedText(playerid, lang_texts[10][30] , 0,COLOUR_RED);
	}
	else if(PlayerState == PLAYER_STATE_PASSENGER)
	{
		SendPlayerFormattedText(playerid, lang_texts[10][31] , 0,COLOUR_RED);
	}

	new playervehiclemodel = GetSpawnVehicleModel(GetPlayerVehicleID(playerid)-1);
	if (playervehiclemodel >= 400)
	{
		if (IsVehicleAllowedInRace(raceid,playervehiclemodel) == 0)
		{
			SendClientMessage(playerid,COLOUR_RED,  lang_texts[10][32] );
			SendPlayerAllowedRaceVehicles(playerid,raceid);
		}
	}
}

SendPlayerAllowedRaceVehicles(playerid,raceid)
{
	new string[MAX_STRING];
	if (strlen(Race[raceid][race_vehicles]) <= 10)
	{
		set(string,"Allowed vehicles: ");
		for (new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
		{
			if (Race[raceid][race_vehicles][i] == 0)  break;
			new carmodel = Race[raceid][race_vehicles][i];
			if (i>0) strins(string, ", ", strlen(string));
			strins(string, GetVehicleName(carmodel), strlen(string));
		}
		SendClientMessage(playerid,COLOUR_GREY, string);
	}
	else
	{
		SendPlayerScrollingText(playerid, lang_texts[10][33] );
		for (new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
		{
			if (Race[raceid][race_vehicles][i] == 0)  break;
			new carmodel = Race[raceid][race_vehicles][i];
			format(string,sizeof(string),"%s",GetVehicleName(carmodel));
			SendPlayerScrollingText(playerid,string);
		}
	}
}

LeaveRace(raceid,playerid)
{
	new string[MAX_STRING];
	ResetQuest(playerid);  // reset checkpoints
	RaceStats[raceid][race_racercount]--;
	RacePlayerStats[playerid][race_player_cp] = 0;
	RacePlayerStats[playerid][race_player_time] = 0;

	if (!IsPlayerConnected(playerid)) return;
	if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
	{
		if (Race[raceid][race_minracers] > 1)
		{
			format(string, sizeof(string),  lang_texts[10][34] , oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: has left the race. (Racers:%d/%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
			WriteLog(logstring);
		}
		else
		{
			format(string, sizeof(string),  lang_texts[10][35] , oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: has left the race. (Racers:%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
			WriteLog(logstring);
		}
		SendMessageToRacers(raceid,string,COLOUR_GREY);
		oGivePlayerMoney(playerid,Race[raceid][race_cashentry],1); // give player back thier cash entry
		GameTextForPlayer(playerid, "~r~Race Aborted.",5000,6);
		SendPlayerFormattedText(playerid,  lang_texts[10][36] , 0,COLOUR_RED);
	}
	else
	{
		format(string, sizeof(string),  lang_texts[10][37] , oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
		SendMessageToRacers(raceid,string,COLOUR_GREY);
		SendPlayerFormattedText(playerid,  lang_texts[10][38] , 0,COLOUR_RED);
	}
}

SendMessageToRacers(raceid,string[],colour)
{
	for (new racerid=0; racerid<MAX_PLAYERS;racerid++)
	{
		if (!IsPlayerConnected(racerid)) continue;
		if (PlayerQuest[racerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			SendPlayerFormattedText(racerid, string, 0,colour);
		}
	}
}

IsPlayerInRace(playerid,raceid)
{
	if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) return 1;
	return 0;
}


GetRaceColourForPlayer(playerid,raceid)
{
	new colour = COLOUR_RACE;
	if (RaceStats[raceid][race_racercount] >= Race[raceid][race_minracers]-1)
	{
		colour = COLOUR_RACE;
	}
	//else if (RaceStats[raceid][race_racercount] == (Race[raceid][race_minracers]-1))
	//{
	// colour = COLOUR_YELLOW;
	//}
	else if (RaceStats[raceid][race_racercount] < (Race[raceid][race_minracers]-1))
	{
		colour = COLOUR_RACE_BAD;
	}

	if (GetPlayerLevel(playerid) < Race[raceid][race_minlevel])
	{
		colour = COLOUR_RACE_BAD;
	}
	return colour;
}

SendPlayerCurrentRaces(playerid)
{
	if (!IsPlayerConnected(playerid)) return;
	new string[MAX_STRING];
	new racefound;
	for (new raceid=1; raceid<=RacesCount;raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (racefound == 0)
			{
				SendPlayerFormattedText(playerid,  lang_texts[10][39] , 0,COLOUR_RACE);
				racefound++;
			}
			if (RaceStats[raceid][race_racercount] >= Race[raceid][race_minracers])
			{
				format(string, sizeof(string),  lang_texts[10][40] ,Race[raceid][race_name],raceid,Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer],RaceStats[raceid][race_racercount],GetRaceCPZoneName(raceid,0));
			}
			else 
			{
				format(string, sizeof(string),  lang_texts[10][41] ,Race[raceid][race_name],raceid,Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer],RaceStats[raceid][race_racercount],Race[raceid][race_minracers],GetRaceCPZoneName(raceid,0));
			}
			SendPlayerFormattedText(playerid, string, 0,GetRaceColourForPlayer(playerid,raceid));
		}
		else if (IsPlayerAdmin(playerid))
		{
			if (strlen(Race[raceid][race_name]) > 0)
			{
				format(string, sizeof(string),  lang_texts[10][42] ,Race[raceid][race_name],raceid);
				SendPlayerFormattedText(playerid, string, 0,COLOUR_RED);
			}
		}
	}
	if (racefound == 0)
	{
		SendPlayerFormattedText(playerid,  lang_texts[10][43] , 0,COLOUR_RED);
	}
}

GetPlayerRace(playerid)
{
	if (!IsPlayerConnected(playerid)) return 0;
	for (new raceid=1; raceid<=RacesCount;raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			return raceid;
		}
	}
	return 0;
}

GetRaceQuestID(raceid)
{
	return RaceStats[raceid][race_questid];
}

CleanupRace(raceid)
{
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			LeaveRace(raceid,playerid);
		}
		if (IsPlayerConnected(playerid))
		{
			oDisablePlayerCheckpoint(playerid);
		}
	}
	RaceStats[raceid][race_position] = 0;
	RaceStats[raceid][race_racercount] = 0;
	RaceStats[raceid][race_timer] = MakeRaceSleepTime(raceid);
	RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;

	if (RaceStats[raceid][race_activecpid] != 0)
	{
		RemoveActiveCheckpoint(RaceStats[raceid][race_activecpid]);
		RaceStats[raceid][race_activecpid] = 0;
	}
}

NextRaceCP(raceid,playerid)
{

	if (RacePlayerStats[playerid][race_player_cp] == RaceSize[raceid]-1) // if finish line
	{
		FinishRace(raceid,playerid);
		return;
	}

	PlayerPlaySound(playerid,1058,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]));
	RacePlayerStats[playerid][race_player_cp]++;
	oSetPlayerCheckpoint(playerid,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]),CPSize);
}

StartRaceCountdown(raceid)
{
	RaceStats[raceid][race_timer]=0;
	if (RaceStats[raceid][race_activecpid] != 0)
	{
		RemoveActiveCheckpoint(RaceStats[raceid][race_activecpid]);
		RaceStats[raceid][race_activecpid] = 0;
	}
	new string[100];
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			if (!oIsPlayerInCheckpoint(playerid,GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),50))
			{
				LeaveRace(raceid,playerid);
				SendPlayerFormattedText(playerid,  lang_texts[10][44] , 0,COLOUR_RED);
			}
		}
	}

	if (RaceStats[raceid][race_racercount] < Race[raceid][race_minracers])
	{
		format(string, sizeof(string),  lang_texts[10][45] ,Race[raceid][race_name]);
		if (PlayerCount() > 0)
		{
			new logstring[256];
			format(logstring, sizeof (logstring), "Race: '%s' did not run.",Race[raceid][race_name]);
			WriteLog(logstring);
		}
		SendClientMessageToRegistered(COLOUR_RED, string);
		CleanupRace(raceid);
		return;
	}

	format(string, sizeof(string),  lang_texts[10][46] ,Race[raceid][race_name]);
	SendClientMessageToRegistered(COLOUR_RACE, string);
	new logstring[256];
	format(logstring, sizeof (logstring), "Race: %s starting.",Race[raceid][race_name]);
	WriteLog(logstring);
	RaceStats[raceid][race_state] = RACE_STATE_COUNTDOWN;

	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			TogglePlayerControllable(playerid, 0);
			SetCameraBehindPlayer(playerid);
			if (Race[raceid][race_startheading] != 0.0)
			{
				SetVehicleZAngle(GetPlayerVehicleID(playerid), Race[raceid][race_startheading]);
			}
		}
	}
}

EndRace(raceid)
{
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			GameTextForPlayer(playerid, "~r~Race Failed!",5000,6);
			SendPlayerFormattedText(playerid,  lang_texts[10][47] , 0,COLOUR_RED);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: his race ended. He failed to quilify.",playerid,oGetPlayerName(playerid));
			WriteLog(logstring);
			GivePlayerXP(playerid,0-(Race[raceid][race_xpprize]/3),1);
		}
	}
	CleanupRace(raceid);
}


FinishRace(raceid,playerid)
{
	RaceStats[raceid][race_position]++;
	new string[MAX_STRING];
	RacePlayerStats[playerid][race_player_time] = RaceStats[raceid][race_timer];
	
	if (RaceStats[raceid][race_position] == 1)
	{
		format(string, sizeof(string),  lang_texts[10][48] ,Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessage(playerid,COLOUR_GREEN, string);

		format(string, sizeof(string),  lang_texts[10][49] ,oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessageToRegistered(COLOUR_RACE, string);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: has won race '%s'! Time: %s",playerid,oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		WriteLog(logstring);
		oGivePlayerMoney(playerid,Race[raceid][race_cashprize],1);

		if ( (GetPlayerXP(playerid) + (Race[raceid][race_xpprize] + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount])) ) <= MAX_LVLXP)
		{
		GivePlayerXP(playerid,Race[raceid][race_xpprize] + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount]),1);
		} else {
			GivePlayerXP(playerid,MAX_LVLXP - (Race[raceid][race_xpprize] + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount])),1);
			}
	}
	else if (RaceStats[raceid][race_position] == 2)
	{
		format(string, sizeof(string),  lang_texts[10][50] ,Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessage(playerid,COLOUR_GREEN, string);

		format(string, sizeof(string),  lang_texts[10][51] ,oGetPlayerName(playerid),FormatPosition(RaceStats[raceid][race_position]),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessageToRegistered(COLOUR_RACE, string);
		if (RaceStats[raceid][race_racercount] > 2)
		{
			oGivePlayerMoney(playerid,(Race[raceid][race_cashprize]/2),1);
			GivePlayerXP(playerid,(Race[raceid][race_xpprize]/2) + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount]),1);

		}
		else
		{
			GivePlayerXP(playerid,Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount],1);
		}
	}
	else if ((RaceStats[raceid][race_position] == 3) && (RaceStats[raceid][race_racercount] > 3))
	{
		format(string, sizeof(string),  lang_texts[10][52] ,Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessage(playerid,COLOUR_GREEN, string);

		format(string, sizeof(string),  lang_texts[10][53] ,oGetPlayerName(playerid),FormatPosition(RaceStats[raceid][race_position]),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessageToRegistered(COLOUR_RACE, string);
		if (RaceStats[raceid][race_racercount] > 3)
		{
			oGivePlayerMoney(playerid,(Race[raceid][race_cashprize]/3),1);
			GivePlayerXP(playerid,(Race[raceid][race_xpprize]/3) + (Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount]),1);

		}
		else
		{
			GivePlayerXP(playerid,Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount],1);
		}
	}
	else
	{
		format(string, sizeof(string),  lang_texts[10][54] ,Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SendClientMessage(playerid,COLOUR_GREEN, string);
		GivePlayerXP(playerid,Race[raceid][race_xpbonus] * RaceStats[raceid][race_racercount],1);
	}

	string = FormatPosition(RaceStats[raceid][race_position]);
	GameTextForPlayer(playerid,string,5000,6);
	PlayerPlaySound(playerid,1057,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]));

	if ((RaceStats[raceid][race_timer] < BestScore[raceid][race_score_time]) || (BestScore[raceid][race_score_time] == 0))
	{
		format(string, sizeof(string),  lang_texts[10][55] ,oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]),ConvertSeconds(BestScore[raceid][race_score_time]));
		SendClientMessageToRegistered(COLOUR_RACE, string);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: has set a new record for race '%s'! Time: %s. Old record: %s.",playerid,oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]),ConvertSeconds(BestScore[raceid][race_score_time]));
		WriteLog(logstring);
		oGivePlayerMoney(playerid,(Race[raceid][race_cashprize]*3),1);
		GivePlayerXP(playerid,(Race[raceid][race_xpprize]*3),1);
		BestScore[raceid][race_score_time] = RaceStats[raceid][race_timer];
		set(BestScore[raceid][race_score_player],oGetPlayerName(playerid));
		new playervehiclemodel = GetSpawnVehicleModel(GetPlayerVehicleID(playerid)-1);
		set(BestScore[raceid][race_score_vehicle],GetVehicleName(playervehiclemodel));
	}

	LeaveRace(raceid,playerid);
}

StartRaceLineup(raceid)
{
	RaceStats[raceid][race_state] = RACE_STATE_LINEUP;
	new string1[MAX_STRING];
	new string2[MAX_STRING];
	format(string1, sizeof(string1),  lang_texts[10][56] ,Race[raceid][race_name],raceid,Race[raceid][race_lineupdelay],GetRaceCPZoneName(raceid,0));
	if (Race[raceid][race_minracers] > 1)
	{
		format(string2, sizeof(string2),  lang_texts[10][57] ,Race[raceid][race_cashentry],Race[raceid][race_minlevel],Race[raceid][race_minracers]);
	}
	else
	{
		format(string2, sizeof(string2),  lang_texts[10][58] ,Race[raceid][race_cashentry],Race[raceid][race_minlevel]);
	}
	for (new playerid=0; playerid<MAX_PLAYERS;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		SendClientMessage(playerid,GetRaceColourForPlayer(playerid,raceid), string1);
		SendClientMessage(playerid,GetRaceColourForPlayer(playerid,raceid), string2);
		if (PlayerQuest[playerid] == 0) // if player not on a quest
		{
			oSetPlayerCheckpoint(playerid,GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),12);
		}
	}
	RaceStats[raceid][race_activecpid] = AddActiveCheckpoint(GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),10000,10);

}

GetRaceCPZoneName(raceid,cpid)
{
	new zone[MAX_NAME];
	zone = GetXYZZoneName(GetRaceCPX(raceid,cpid),GetRaceCPY(raceid,cpid),GetRaceCPZ(raceid,cpid));
	return zone;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	for (new raceid=1; raceid<=RacesCount;raceid++) //  for each race
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) break;

		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (PlayerQuest[playerid] == 0) // if player not on a quest
			{
				// if player at startline
				if (oIsPlayerInCheckpoint(playerid,GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),(CPSize*3)))
				{
					if (!IsPlayerInRace(playerid,raceid))
					{
						JoinRace(raceid,playerid);
					}
					else
					{
						new playervehiclemodel = GetSpawnVehicleModel(GetPlayerVehicleID(playerid)-1);
						if (IsVehicleAllowedInRace(raceid,playervehiclemodel) == 0)
						{
							SendClientMessage(playerid,COLOUR_RED,  lang_texts[10][59] );
							SendPlayerAllowedRaceVehicles(playerid,raceid);
						}
						else
						{
							SendClientMessage(playerid,COLOUR_RACE,  lang_texts[10][60] );
						}
					}
				}
			}
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
		{
			if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
			{
				if (oIsPlayerInCheckpoint(playerid,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]),CPSize*3))
				{ 
					new playervehiclemodel = GetSpawnVehicleModel(GetPlayerVehicleID(playerid)-1);
					
					if (IsVehicleAllowedInRace(raceid,playervehiclemodel) == 0)
					{
						SendClientMessage(playerid,COLOUR_RED,  lang_texts[10][61] );
						SendPlayerAllowedRaceVehicles(playerid,raceid);
						continue;

					}
					NextRaceCP(raceid,playerid);
				}
			}
		}
	}
}

IsVehicleAllowedInRace(raceid,vehiclemodel)
{
	if (Race[raceid][race_vehicles][0] == 0) return 1;
	new vehicleallowed;
	
	for (new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
	{
		if (vehiclemodel == Race[raceid][race_vehicles][i])
		{
			vehicleallowed = 1;
			break;
		}
	}
	return vehicleallowed;
}
