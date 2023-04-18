// Petrol system example by Donny k (c) 2006
#if defined _fuel_included
#endinput
#endif

#define _fuel_included
#pragma library fuel
#include "base"

//Fuel (Relevant code!)
new Petrol_Time = 500;
new Petrol_Amount = 100;
new Petrol_Multiplier = 9;

// 0 for KPH, 1 for MPH
new SpeedMode = 0;
new SpeedPos = 4;

#define MAX_SPEEDPOS 16

//Gas stations (Relevant code!)
#define CP_STATION1   0
#define CP_STATION2   1
#define CP_STATION3   2
#define CP_STATION4   3
#define CP_STATION5   4
#define CP_STATION6   5
#define CP_STATION7   6
#define CP_STATION8   7
#define CP_STATION9   8
#define CP_STATION10  9
#define CP_STATION11  10
#define CP_STATION12  11
#define CP_STATION13  12
#define CP_STATION14  13
#define CP_STATION15  14
#define CP_STATION16  15
/* #define CP_STATION17  16
#define CP_STATION18  17
#define CP_STATION19  18
#define CP_STATION20  19 */


#define MAX_POINTS 16  //(Relevant code!)


new playerCheckpoint[MAX_PLAYERS];

new Float:Petrol[MAX_VEHICLES];

new Float:SavedPlayerPositions[MAX_PLAYERS][MAX_SPEEDPOS][3];


//Coordinate check var (Relevant code!)
new Float:checkCoords[MAX_POINTS][4] = {
 {2098.1316,901.7380,2137.7456,963.0146},     
 {2617.5967,1062.8710,2656.6526,1142.5109},     
 {501.7475,1626.7821,653.2095,1774.1093},        
 {-1351.2833,2638.8943,-1265.1661,2738.6450},      
 {-2450.3767,949.5080,-2400.8530,1069.8329},      
 {-1715.5112,349.5567,-1658.8451,458.3686},        
 {-2265.3027,-2586.3762,-2219.1868,-2558.2539},   
 {-1657.4102,-2763.3518,-1501.5303,-2666.7454},   
 {1903.7450,-1795.7990,1955.5667,-1759.5187},     
 {-135.2077,-1199.8291,-43.7008,-1134.7999},   
 {1336.7378,454.7961,1430.0760,483.9776},
 {648.2494,-592.9003,670.9278,-540.8264},         
 {-1494.2593,1854.1290,-1449.4076,1885.2032},     
 {2097.0452,2708.1218,2172.7161,2762.7495},        
 {2187.6587,2462.8057,2215.5042,2495.0332},        
 {1577.7729,2182.5112,1616.5842,2242.3628}
 // added by ADDAM 
/* {1573.7111,-2495.9067,13.3860,2495.0332},
 {1478.0878,1593.5178,10.5658,2242.3628},
 {343.2518,2508.6926,17.0805,483.977},
 {-1482.8566,6.5929,14.6950,458.3686} */
};

//Checkpoint check var (Relevant code!)
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
 // added by ADDAM
/* {1573.7111,-2495.9067,13.3860,5.0},
 {1478.0878,1593.5178,10.5658,5.0},
 {343.2518,2508.6926,17.0805,5.0},
 {-1482.8566,6.5929,14.6950,5.0} */
};

//Checkpoint type var (Relevant code!)
new checkpointType[MAX_POINTS] = {
 CP_STATION1,
 CP_STATION2,
 CP_STATION3,
 CP_STATION4,
 CP_STATION5,
 CP_STATION6,
 CP_STATION7,
 CP_STATION8,
 CP_STATION9,
 CP_STATION10,
 CP_STATION11,
 CP_STATION12,
 CP_STATION13,
 CP_STATION14,
 CP_STATION15,
 CP_STATION16
/* CP_STATION17,
 CP_STATION18,
 CP_STATION19,
 CP_STATION20 */
};

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//Pre made Functions (Relevant code!)
//------------------------------------------------------------------------------

//Check point type function
public getCheckpointType(playerID) {
 return checkpointType[playerCheckpoint[playerID]];
}

//IsPlayerInArea function
public isPlayerInArea(playerID, Float:data[4])
{
 new Float:X, Float:Y, Float:Z;
 GetPlayerPos(playerID, X, Y, Z);
 if( X>= data[0] &&  X<= data[2] && Y >= data[1] && Y <= data[3])
 {
  return 1;
 }
 return 0;
}

//Update the players checkpoint function
public checkpointUpdate()
{
 for(new i=0; i<MAX_PLAYERS; i++)
 {
  if(IsPlayerConnected(i) && !GetPlayerRace(i))
  {
   for(new j=0; j < MAX_POINTS; j++)
   {
    if(isPlayerInArea(i, checkCoords[j]))
    {
     if(playerCheckpoint[i]!=j)
     {
      DisablePlayerCheckpoint(i);
      SetPlayerCheckpoint(i, checkpoints[j][0],checkpoints[j][1],checkpoints[j][2],checkpoints[j][3]);
      playerCheckpoint[i] = j;
     }
    }
    else
    {
     if(playerCheckpoint[i]==j)
     {
      DisablePlayerCheckpoint(i);
      playerCheckpoint[i] = 999;
     }
    }
   }
  }
 }
}

//Fuel check function
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
   
   for(new h=0;h < SpeedPos; h++)
   {
/*    distance += floatsqroot(
    floatpower(floatabs(floatsub(SavedPlayerPositions[i][h][0],SavedPlayerPositions[i][h+1][0])),2)+
    floatpower(floatabs(floatsub(SavedPlayerPositions[i][h][1],SavedPlayerPositions[i][h+1][1])),2)+
    floatpower(floatabs(floatsub(SavedPlayerPositions[i][h][2],SavedPlayerPositions[i][h+1][2])),2)) * 1;  */
    if (SavedPlayerPositions[i][h][0] < 1 || SavedPlayerPositions[i][h+1][0] < 1) continue;

    distance += floatpower(
    floatpower(SavedPlayerPositions[i][h][0]-SavedPlayerPositions[i][h+1][0],2)+
    floatpower(SavedPlayerPositions[i][h][1]-SavedPlayerPositions[i][h+1][1],2)+
    floatpower(SavedPlayerPositions[i][h][2]-SavedPlayerPositions[i][h+1][2],2),
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
      format(message,sizeof(message),  lang_texts[5][8] );
      if(Petrol[Vi] < 0)
      {
       Petrol[Vi] = 0.00;
      }
     }
    }
   }
   format(message,sizeof(message),"~n~~n~~n~~n~~n~~n~~n~~n~~n~~n~ %s",message);
   GameTextForPlayer(i, message, Petrol_Time+500, 3);
  }
 }
}

//Refill fuel function
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
   format(mess, sizeof(mess),  lang_texts[5][10] , floatround(FillUp));
   SendClientMessage(playerid, COLOUR_WHITE, mess);
   GivePlayerMoney(playerid, -floatround(FillUp));
   return 1;
  }
  else
  {
   SendClientMessage(playerid, COLOUR_RED,  lang_texts[5][11] );
   return 1;
  }
 }
 else
 {
  SendClientMessage(playerid, COLOUR_RED,  lang_texts[5][12] );
  return 1;
 }
 return 1;
}


public FuelCommandHandler(playerid, cmdtext[])
{
 //Gas station locations command (Relevant code!)
 if(strcmp(cmdtext, "/gasloc", true) == 0) {
/*  SendClientMessage(playerid, COLOUR_GREEN,  lang_texts[5][13] );
  SendClientMessage(playerid, COLOUR_YELLOW,  lang_texts[5][14] );
  SendClientMessage(playerid, COLOUR_YELLOW,  lang_texts[5][15] );
  SendClientMessage(playerid, COLOUR_YELLOW,  lang_texts[5][16] );
  SendClientMessage(playerid, COLOUR_YELLOW,  lang_texts[5][17] );
  SendClientMessage(playerid, COLOUR_YELLOW,  lang_texts[5][18] );
  SendClientMessage(playerid, COLOUR_YELLOW,  lang_texts[5][19] );
  SendClientMessage(playerid, COLOUR_YELLOW,  lang_texts[5][20] );
  SendClientMessage(playerid, COLOUR_YELLOW,  lang_texts[5][21] ); */
  
  new Float:coord_x,Float:coord_y,Float:coord_z,Float:distance,d_n=99999999,d_w;
  
  GetPlayerPos(playerid,coord_x,coord_y,coord_z);
  
  for (new p; p < MAX_POINTS; p++) 
  {
  distance = floatsqroot(
  floatpower(floatabs(floatsub(checkCoords[p][0],coord_x)),2)+
  floatpower(floatabs(floatsub(checkCoords[p][1],coord_y)),2)+
  floatpower(floatabs(floatsub(checkCoords[p][2],coord_z)),2));
  if (distance < d_n) { d_n = floatround(distance); d_w = p; }
  
  }
  DisablePlayerCheckpoint(playerid);
  SetPlayerCheckpoint(playerid,checkCoords[d_w][0],checkCoords[d_w][1],checkCoords[d_w][2],8.0);
  
  SendClientMessage(playerid, COLOUR_YELLOW,  lang_texts[5][22] );
  
  return 1;
 }
 return 0;
}


public OnPlayerEnterFuelCheckpoint(playerid)
{
 //(The next 13 lines (the return should always be at the end) is Relevant code!)
 if(getCheckpointType(playerid)>= 0 && getCheckpointType(playerid)<= 15)
 {
  if(IsPlayerInAnyVehicle(playerid))
  {
   SendClientMessage(playerid,COLOUR_WHITE, lang_texts[5][23] );
   FuelRefill(playerid);
  }
  else
  {
   SendClientMessage(playerid, COLOUR_RED,  lang_texts[5][24] );
  }
  return 1;
 }
 return 1;
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