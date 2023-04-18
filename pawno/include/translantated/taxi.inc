// AUTHOR ASTUREL

#include "base"
#if defined _taxi_included
  #endinput
#endif

#define _taxi_included
#pragma library taxi


#define DRIVERS_COLOR COLOUR_YELLOW
#define TAXI_TIMER 10000
#define TAXI_MONEY_V_TAXI 200
#define TAXI_MONEY_V_CABBIE 300
#define TAXI_MONEY_V_STRETCH 400

new taxi_drivers[MAX_PLAYERS];
new old_player_colors[MAX_PLAYERS];


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
    SendClientMessage(i,COLOUR_RED,totaxis);
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
    
     format(moneyannounce, 50, "+%d", money);
     GameTextForPlayer(i, moneyannounce, 2000, 1);
     GivePlayerMoney(i, money);
    }
   }
  }
 }
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
   SendClientMessageToAll(COLOUR_RED, taxi_announce);
   SetPlayerColour(playerid, old_player_colors[playerid]);
   taxi_drivers[playerid] = 0;
   return 1;
   
  } 
  else
  {
   new playervehiclemodel = GetSpawnVehicleModel(GetPlayerVehicleID(playerid)-1);
//   if (IsPlayerInAnyVehicle(playerid) == 1) { // definied for vehicle class
   if (playervehiclemodel == V_TAXI || playervehiclemodel == V_CABBIE || playervehiclemodel == V_STRETCH) {

    new taxi_announce[60];
    format(taxi_announce,sizeof(taxi_announce), lang_texts[11][3] ,player);
    SendClientMessageToAll(COLOUR_RED, taxi_announce);
    old_player_colors[playerid] = GetPlayerColor(playerid);
    SetPlayerColour(playerid, DRIVERS_COLOR);
    taxi_drivers[playerid] = 1;
    return 1;
   }
   else
   {
    SendClientMessage(playerid,COLOUR_RED, lang_texts[11][4] ); return 1;
   }
  } 
 }
 
 if (strcmp(cmdtext, "/call taxi", true) == 0 && taxi_drivers[playerid] != 1)
 {
  taxi_driver_announce(playerid);
  SendClientMessage(playerid,COLOUR_RED, lang_texts[11][5] );
  old_player_colors[playerid] = GetPlayerColor(playerid);
  SetPlayerColour(playerid, 0xFFFFFFAA);
  taxi_driver_announce(playerid);
  return 1;
 }
 return 1;
}



public OnPlayerStateChange(playerid, newstate, oldstate)
{
 if (oldstate == PLAYER_STATE_PASSENGER && old_player_colors[playerid]) 
 {
  SetPlayerColour(playerid, old_player_colors[playerid]);
 }
 
/* if (oldstate == PLAYER_STATE_DRIVER && taxi_drivers[playerid] == 1) 
 { 
  new player[MAX_PLAYER_NAME];
  taxi_drivers[playerid] = 0;
  new taxi_announce[60];
  GetPlayerName(playerid, player, MAX_PLAYER_NAME);
  SetPlayerColour(playerid,old_player_colors[playerid]);
  format(taxi_announce,sizeof(taxi_announce), lang_texts[11][6] ,player);
  SendClientMessageToAll(COLOUR_RED, taxi_announce);
 }  */

}


public TaxiPlayerDisconnect(playerid)
{
 if (taxi_drivers[playerid] == 1) taxi_drivers[playerid] = 0;

}

public GetHEXString(number)
{
 new str[9];
 // new tmp;
 for (new i = 7; i >= 0; i--)
 {
  str[i] = ( /* tmp = */ number & 0x0F) + 0x30; // + (tmp > 9 ? 0x07 : 0x00)
  str[i] += (str[i] > '9') ? 0x07 : 0x00;
  number >>= 4;
 }
 str[8] = '\0';
 new str2[11];
 format(str2,sizeof(str2), lang_texts[11][7] ,str);
 return str2;
}
