#if defined _vehicles_extras_included
#endinput
#endif

#define _vehicles_extras_included
#pragma library vehicles_extras
#include "base"

forward fuelconfig();
forward checkpointUpdate();
forward CheckFuel(playerid);
forward FuelRefill(playerid);
forward FuelCommandHandler(playerid, cmdtext[]);
forward OnPlayerEnterFuelCheckpoint(playerid);
forward OnVehicleSpawn(vehicleid);
forward autorefill();
forward CarsCommandHandler(playerid, cmdtext[]);

//Fuel (Relevant code!)
new Petrol_Time = 500;
new Petrol_Amount = 100;
new Petrol_Multiplier = 9;

// 0 for KPH, 1 for MPH
new SpeedMode = 0;
new SpeedPos = 4;
new FuelCheckSize = 50;

#define MAX_SPEEDPOS 16

#define MAX_POINTS 16 


new Float:Petrol[MAX_VEHICLES];

new Float:SavedPlayerPositions[MAX_PLAYERS][MAX_SPEEDPOS][3];


new Float:checkpoints[MAX_POINTS][4] = {
	{2109.2126,917.5845,10.8203,5.0},
	{2640.1831,1103.9224,10.8203,5.0},
	{611.8934,1694.7921,6.7193,5.0},
	{-1327.5398,2682.9771,49.7896,5.0},
	{-2413.7427,975.9317,45.0031,5.0},
	{-1672.3597,414.2950,6.8866,5.0},
	{-2244.1365,-2560.6294,31.6276,5.0},
	{-1603.0166,-2709.3589,48.2419,5.0},
	{1939.3275,-1767.6813,13.2787,5.0},
	{-94.7651,-1174.8079,1.9979,5.0},
	{1381.6699,462.6467,19.8540,5.0},
	{657.8167,-559.6507,16.0630,5.0},
	{-1478.2916,1862.8318,32.3617,5.0},
	{2147.3054,2744.9377,10.5263,5.0},
	{2204.9602,2480.3494,10.5278,5.0},
	{1590.9493,2202.2637,10.5247,5.0}

};



public fuelconfig()
{
	if (!dini_Exists(ConfigDB)) return;

	new temp[MAX_STRING];
	
	set(temp,dini_Get(ConfigDB,"Petrol_Amount"));
	if (strlen(temp) > 0) Petrol_Amount = strval(temp); 
	else { valstr(temp,Petrol_Amount); dini_Set(ConfigDB,"Petrol_Amount",temp); }
	
	set(temp,nullstr);
	set(temp,dini_Get(ConfigDB,"Petrol_Multiplier"));
	if (strlen(temp) > 0) Petrol_Multiplier = strval(temp); 
	else { valstr(temp,Petrol_Multiplier); dini_Set(ConfigDB,"Petrol_Multiplier",temp);  }
	
	set(temp,nullstr);
	set(temp,dini_Get(ConfigDB,"Petrol_Time"));
	if (strlen(temp) > 0) Petrol_Time = strval(temp); 
	else { valstr(temp,Petrol_Time); dini_Set(ConfigDB,"Petrol_Time",temp); }
	
	set(temp,nullstr);
	set(temp,dini_Get(ConfigDB,"SpeedMode"));
	if (strlen(temp) > 0) SpeedMode = strval(temp); 
	else { valstr(temp,SpeedMode); dini_Set(ConfigDB,"SpeedMode",temp); }
	
	set(temp,nullstr);
	set(temp,dini_Get(ConfigDB,"SpeedPos"));
	if (strlen(temp) > 0) SpeedPos = strval(temp); 
	else { valstr(temp,SpeedPos); dini_Set(ConfigDB,"SpeedPos",temp); }
	

}


public checkpointUpdate()
{
	for(new i=0; i<MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i) && !GetPlayerRace(i))
		{
			for (new k; k < MAX_POINTS; k++) 
			{
				new Float:X,Float:Y,Float:Z;
				GetPlayerPos(i, X, Y, Z);
				
				if (X >= checkpoints[k][0] && X <= (checkpoints[k][0]+FuelCheckSize) && Y >= checkpoints[k][1] && Y <= (checkpoints[k][1]+FuelCheckSize)    ) 
				{
						DisablePlayerCheckpoint(i);
						SetPlayerCheckpoint(i,checkpoints[k][0], checkpoints[k][1], checkpoints[k][2],checkpoints[k][3]);
						break;
				}
			}
		}
	}
}


public CheckFuel(playerid)
{
	new Float:distance,speed,message[256];
	for(new i=0;i<MAX_PLAYERS;i++)
	{
		if(IsPlayerConnected(i) && IsPlayerInAnyVehicle(i))
		{
			
			for(new a; a < SpeedPos; a++)
			{
				for(new f=0;f<3;f++) SavedPlayerPositions[i][a][f] = SavedPlayerPositions[i][a+1][f];
			}
			
			GetPlayerPos(i, SavedPlayerPositions[i][SpeedPos][0], SavedPlayerPositions[i][SpeedPos][1], SavedPlayerPositions[i][SpeedPos][2]);
			
			for(new h; h < SpeedPos; h++)
			{
				
				if (SavedPlayerPositions[i][h][0] < 1 || SavedPlayerPositions[i][h+1][0] < 1) continue;

				distance += floatpower(
				floatpower(SavedPlayerPositions[i][h][0]-SavedPlayerPositions[i][h+1][0],2)+
				floatpower(SavedPlayerPositions[i][h][1]-SavedPlayerPositions[i][h+1][1],2) /* +
				 floatpower(SavedPlayerPositions[i][h][2]-SavedPlayerPositions[i][h+1][2],2) */,
				0.62); 
				
			}

			speed = floatround(distance);

			if (SpeedMode) speed = floatround(distance/1.6);
			else speed = floatround(distance);
			
			if (speed > 90) format(message,sizeof(message), lang_texts[5][1] ,speed); 
			else if (speed > 2) format(message,sizeof(message), lang_texts[5][2] ,speed); 
			else format(message,sizeof(message), lang_texts[5][3] ,speed); 
			
			
			new pvm = GetSpawnVehicleModel(GetPlayerVehicleID(i)-1);
			if (pvm != 509 && pvm != 481 && pvm != 510) 
			{
				if (!GetPlayerRace(i))
				{
					new Vi;
					Vi = GetPlayerVehicleID(i);
					
					if (speed > 2) 
					{
						Petrol[Vi] = Petrol[Vi] - (float(Petrol_Time)/float(1000 * Petrol_Multiplier));
						if (speed > 90) Petrol[Vi] = Petrol[Vi] - (float(Petrol_Time)/float(1000 * Petrol_Multiplier));
					}

					if (Petrol[Vi] > 60) format(message, sizeof(message),  lang_texts[5][4] , message, floatround(Petrol[Vi]));
					else if (Petrol[Vi] > 40) format(message, sizeof(message),  lang_texts[5][5] , message, floatround(Petrol[Vi]));
					else if (Petrol[Vi] > 15) format(message, sizeof(message),  lang_texts[5][6] , message, floatround(Petrol[Vi]));
					else if (Petrol[Vi] <= 15) format(message, sizeof(message),  lang_texts[5][7] , message, floatround(Petrol[Vi]));

					
					if(Petrol[Vi] < 1)
					{
						RemovePlayerFromVehicle(i);
						format(message,sizeof(message), lang_texts[5][8]);
						if(Petrol[Vi] < 0) Petrol[Vi] = 0.00; 
					}
				}
			}
			format(message,sizeof(message),lang_texts[5][9],message);
			GameTextForPlayer(i, message, Petrol_Time+500, 3);
		}
	}
}


public FuelRefill(playerid)
{
	new VID;
	VID = GetPlayerVehicleID(playerid);
	if(Petrol[VID] < Petrol_Amount)
	{
		new Float:FillUp;
		FillUp = float(Petrol_Amount) - Petrol[VID];
		if(GetPlayerMoney(playerid) >= floatround(FillUp))
		{
			Petrol[VID] +=FillUp;
			new mess[64];
			format(mess, sizeof(mess), lang_texts[5][10], floatround(FillUp));
			SendClientMessage(playerid, COLOUR_WHITE, mess);
			GivePlayerMoney(playerid, -floatround(FillUp));
			return 1;
		}
		else
		{
			SendClientMessage(playerid, COLOUR_RED, lang_texts[5][11]);
			return 1;
		}
	}
	else
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[5][12]);
		return 1;
	}
}


public FuelCommandHandler(playerid, cmdtext[])
{

	if(strcmp(cmdtext, "/gasloc", true) == 0) {
		
		new Float:coord_x,Float:coord_y,Float:coord_z,Float:distance,d_n,d_w;
		
		GetPlayerPos(playerid,coord_x,coord_y,coord_z);
		
		for (new p; p < MAX_POINTS; p++) 
		{
		distance = floatsqroot(
		floatpower(floatabs(floatsub(checkpoints[p][0],coord_x)),2)+
		floatpower(floatabs(floatsub(checkpoints[p][1],coord_y)),2)+
		floatpower(floatabs(floatsub(checkpoints[p][2],coord_z)),2));
		if (distance < d_n || d_n == 0) { d_n = floatround(distance); d_w = p; }
		
		}
		
		if (PlayerQuest[playerid] != 100)
		{
			DisablePlayerCheckpoint(playerid);
			SetPlayerCheckpoint(playerid,checkpoints[d_w][0],checkpoints[d_w][1],checkpoints[d_w][2],checkpoints[d_w][3]);
        	PlayerQuest[playerid] = 100; //set a dummy quest to keep the CP fixed
			SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[5][22]);
		} else {
		    DisablePlayerCheckpoint(playerid);
           	PlayerQuest[playerid] = 0; //reset the dummy quest to keep the CP fixed
		}
		
		return 1;
	}
	return 0;
}


public OnPlayerEnterFuelCheckpoint(playerid)
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		for (new c; c < MAX_POINTS; c++)
		{
			new Float:X,Float:Y,Float:Z;
			GetPlayerPos(playerid, X, Y, Z);
			
			if (X >= checkpoints[c][0] && X <= (checkpoints[c][0]+checkpoints[c][3]) && Y >= checkpoints[c][1]  && Y <= (checkpoints[c][1]+checkpoints[c][3]))
			{
				SendClientMessage(playerid,COLOUR_WHITE, lang_texts[5][23]);
				FuelRefill(playerid);
				if (PlayerQuest[playerid] == 100) PlayerQuest[playerid] = 0; //reset the dummy quest
			
				return 1;
			}
		}
	}
	return 0;
}

public OnVehicleSpawn(vehicleid)
{
	Petrol[vehicleid] = float(Petrol_Amount);
}

public autorefill()
{
	new vip[MAX_PLAYERS];
	for (new p; p < MAX_PLAYERS; p++)
	{
		if (IsPlayerConnected(p) && IsPlayerInAnyVehicle(p)) vip[GetPlayerVehicleID(p)] = p;
	}
	for (new c; c < MAX_VEHICLES; c++)
	{
		if (!vip[c]) Petrol[c] = float(Petrol_Amount);

	}

}

new VehicleDoors[MAX_VEHICLES];
new PlayerVehicleAutoLock[MAX_PLAYERS];

public CarsCommandHandler(playerid, cmdtext[])
{
	if(!strcmp(cmdtext, "/lock", true)) 
	{
		if (!IsPlayerInAnyVehicle(playerid)) { SendClientMessage(playerid,COLOUR_RED, lang_texts[5][24]); return 1; }
		new newstate;
		new pv = GetPlayerVehicleID(playerid);
		if (VehicleDoors[pv] == 1) newstate = 0; else newstate = 1;
		VehicleDoors[pv] = newstate;
		for (new i; i < MAX_PLAYERS; i++)
		{
			if (IsPlayerConnected(i) && i != playerid) { SetVehicleParamsForPlayer(pv, i, 0, newstate); }
		}
		if (newstate == 1) SendClientMessage(playerid,COLOUR_RED, lang_texts[5][25]);
		else SendClientMessage(playerid,COLOUR_RED, lang_texts[5][26]);
		
		return 1;
	}
	
	if(!strcmp(cmdtext, "/unlock", true))
	{
	if (!IsPlayerInAnyVehicle(playerid)) { SendClientMessage(playerid,COLOUR_RED, lang_texts[5][24]); return 1; }
	new pv = GetPlayerVehicleID(playerid);	
	if (VehicleDoors[pv] == 1) return 1;	
	for (new i; i < MAX_PLAYERS; i++)
		{
			if (IsPlayerConnected(i) && i != playerid) { SetVehicleParamsForPlayer(pv, i, 0, 0); }
		}
	SendClientMessage(playerid,COLOUR_RED, lang_texts[5][26]);
	}
	
	
	if(!strcmp(cmdtext, "/autolock", true)) 
	{
		if (PlayerVehicleAutoLock[playerid] == 1) 
		{ 
			SendClientMessage(playerid,COLOUR_RED, lang_texts[5][27]);
			PlayerVehicleAutoLock[playerid] = 0;
		}
		else 
		{
			SendClientMessage(playerid,COLOUR_RED, lang_texts[5][28]);
			PlayerVehicleAutoLock[playerid] = 1;
		}
	
		return 1;
	}
	
	return 0;
}

