// AUTHOR ASTUREL

#include "base"
#if defined _taxi_included
#endinput
#endif

#define _taxi_included
#pragma library taxi

forward taxi_driver_announce(caller);
forward taxischeck();
forward TaxiCommandHandler(playerid, cmdtext[]);
forward OnPlayerStateChange(playerid, newstate, oldstate);
forward TaxiPlayerDisconnect(playerid);

#define DRIVERS_COLOR COLOUR_YELLOW
#define TAXI_TIMER 10000
#define TAXI_MONEY_V_TAXI 200
#define TAXI_MONEY_V_CABBIE 300
#define TAXI_MONEY_V_STRETCH 400

new taxi_drivers[MAX_PLAYERS];
new old_player_colors[MAX_PLAYERS];
new checked[MAX_PLAYERS] = 0;

public taxi_driver_announce(caller)
{
	new callername[MAX_PLAYER_NAME];
	new totaxis[50];
	GetPlayerName(caller, callername, MAX_PLAYER_NAME);
	for(new i = 0; i < MAX_PLAYERS; i++) 
	{
		if(IsPlayerConnected(i) && taxi_drivers[i] == 1 && IsPlayerInAnyVehicle(i) == 1) 
		{
			format(totaxis,sizeof(totaxis), lang_texts[11][1] ,callername);
			SendClientMessage(i,COLOUR_YELLOW,totaxis);
		}
	}
	return 1;
}

public taxischeck()
{
	for(new i = 0; i < MAX_PLAYERS; i++) 
	{
		if (IsPlayerConnected(i) && IsPlayerInAnyVehicle(i) && GetPlayerState(i) == PLAYER_STATE_DRIVER && taxi_drivers[i] == 1) 
		{ 
			for(new p = 0; p < MAX_PLAYERS; p++) 
			{
				if (IsPlayerConnected(p) && IsPlayerInVehicle(p,GetPlayerVehicleID(i)) && GetPlayerState(p) == PLAYER_STATE_PASSENGER)
				{
					new playervehiclemodel = GetSpawnVehicleModel(GetPlayerVehicleID(i)-1);
					new moneyannounce[50];
					new money;
					switch (playervehiclemodel)
					{
					case V_TAXI: {  money = TAXI_MONEY_V_TAXI; }
					case V_CABBIE: { money = TAXI_MONEY_V_CABBIE; }
					case V_STRETCH: { money = TAXI_MONEY_V_STRETCH; }
					}
					if (GetPlayerMoney(p) <= 0 && checked[p] == 0 && checked[i] == 0)	
					{
						//GameTextForPlayer(p,"Have not enough money!",2000,1);
						//GameTextForPlayer(i,"Passenger does not pay!",2000,1);
						return 1;
					}
					format(moneyannounce, 50, "+ $%d", money);
					GameTextForPlayer(i, moneyannounce, 2000, 1);
					GivePlayerMoney(i, money);
					format(moneyannounce, 50, "- $%d", money);
					GameTextForPlayer(p, moneyannounce, 2000, 1);
					GivePlayerMoney(p, 0 - money);
				}
			}
		}
	}
	return 1;
}

public TaxiCommandHandler(playerid, cmdtext[])
{
	new player[MAX_PLAYER_NAME];
	GetPlayerName(playerid, player, MAX_PLAYER_NAME);
	if (strcmp(cmdtext, "/taxi", true) == 0)
	{
		if (taxi_drivers[playerid] == 1)
		{
			new taxi_announce[60];
			format(taxi_announce,sizeof(taxi_announce), lang_texts[11][2] ,player);
			SendClientMessageToAll(COLOUR_YELLOW, taxi_announce);
			SetPlayerColour(playerid, old_player_colors[playerid]);
			taxi_drivers[playerid] = 0;
			return 1;
		} 
		else
		{
			new playervehiclemodel = GetSpawnVehicleModel(GetPlayerVehicleID(playerid)-1);
			if (playervehiclemodel == V_TAXI || playervehiclemodel == V_CABBIE || playervehiclemodel == V_STRETCH)
			{
				new taxi_announce[60];
				format(taxi_announce,sizeof(taxi_announce), lang_texts[11][3] ,player);
				SendClientMessageToAll(COLOUR_YELLOW, taxi_announce);
				old_player_colors[playerid] = GetPlayerColor(playerid);
				SetPlayerColour(playerid, DRIVERS_COLOR);
				taxi_drivers[playerid] = 1;
				return 1;
			}
			else
			{
				SendClientMessage(playerid,COLOUR_YELLOW, lang_texts[11][4] ); return 1;
			}
		} 
	}
	if (strcmp(cmdtext, "/call taxi", true) == 0 && taxi_drivers[playerid] != 1)
	{
		taxi_driver_announce(playerid);
		SendClientMessage(playerid,COLOUR_YELLOW, lang_texts[11][5] );
		old_player_colors[playerid] = GetPlayerColor(playerid);
		SetPlayerColour(playerid, 0xFFFFFFAA);
		taxi_driver_announce(playerid);
		return 1;
	}
	return 1;
}

/*
public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (oldstate == PLAYER_STATE_PASSENGER && old_player_colors[playerid]) 
	{
		SetPlayerColour(playerid, old_player_colors[playerid]);
	}
}
*/

public TaxiPlayerDisconnect(playerid)
{
	if (taxi_drivers[playerid] == 1) taxi_drivers[playerid] = 0;
}
