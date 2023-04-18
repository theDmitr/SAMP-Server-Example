/*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
^		    Title: LuxurY GangZones (LGZ)                              ^
^		    Version: 1.0                                               ^
^		    Date: 21 November 2007                                     ^
^		    Developer: LuxurY                                          ^
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

######################## LuxurY GangZones functions: ###########################

1. GZ_Create(Float:minx,Float:miny,Float:maxx,Float:maxy);
   Description: It creates gangzone. Returns a value.
   Usage: new gz1 = GZ_Create(Float:minx,Float:miny,Float:maxx,Float:maxy);
   
2. GZ_Destroy(zoneid);
   Description: It destroys gangzone
   Usage: GZ_Destroy(zoneid);
   
3. GZ_IsValidGZ(zoneid);
   Description: It checks if the GangZone is valid or not. Returns a value.
   Usage: GZ_IsValidGZ(zoneid);
   
4. GZ_IsPlayerInGZ(playerid,zoneid);
   Description: It checks if the player is in GangZone or not. Returns a value.
   Usage: GZ_IsPlayerInGZ(playerid,zoneid);
   
5. GZ_IsPlayerInAnyGZ(playerid);
   Description: It checks if the player is any GangZone or not. Returns a value.
   Usage: GZ_IsPlayerInAnyGZ(playerid);
   
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
   
######################## LuxurY GangZones callbacks: ###########################

1. GZ_OnPlayerEnter(playerid,zoneid);
   Description: It starts when the player enter the GangZone.
   Params: playerid - ID of player.
           zoneid   - ID of the GangZone.
   Usage: public GZ_OnPlayerEnter(playerid,zoneid) {
          printf("Player ID:%d entered the GangZone ID:%d",playerid,zoneid);
          return 1;
          }

2. GZ_OnPlayerLeave(playerid,zoneid);
   Description: It starts when the player leave the GangZone.
   Params: playerid - ID of player.
           zoneid   - ID of the GangZone.
   Usage: public GZ_OnPlayerLeave(playerid,zoneid) {
          printf("Player ID:%d left the GangZone ID:%d",playerid,zoneid);
          return 1;
          }

<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
	      
##################### LuxurY GangZones physical aspects: #######################
1. LuxurY GangZones system works only with it's own functions.
2. You use it on your horror and risk
3. This not a final version. Lots of thing would be changed.
4. SetTimer("AreaCheck",1000,1);

<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

                               #################################################
                               ## (c) LDT (LuxurY Development Team) 2006-2007 ##
                               ##                      (c) LuxurY forum 2007  ##
                               ##                         www.luxury.mybb.ru  ##
                               #################################################
                               
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/

#include <a_samp>

#define MAX_GZ 1024

forward GZ_OnPlayerEnter(playerid,zoneid);
forward GZ_OnPlayerLeave(playerid,zoneid);
forward AreaCheck();

new Float:gzmas[MAX_GZ][4], gzcount = 0, gzel[MAX_PLAYERS];
new version[] = "1.0 beta";

stock GZ_Create(Float:minx,Float:miny,Float:maxx,Float:maxy) {
gzcount = GangZoneCount();
GangZoneCreate(Float:minx,Float:miny,Float:maxx,Float:maxy);
gzmas[gzcount][0] = Float:minx;
gzmas[gzcount][1] = Float:miny;
gzmas[gzcount][2] = Float:maxx;
gzmas[gzcount][3] = Float:maxy;
gzcount++;
return gzcount;
}

stock GZ_Destroy(zoneid) {
GangZoneDestroy(zoneid);
gzmas[zoneid][0] = 0;
gzmas[zoneid][1] = 0;
gzmas[zoneid][2] = 0;
gzmas[zoneid][3] = 0;
}

stock GZ_IsValidGZ(zoneid) {
new result;
if (gzmas[zoneid][0] == 0 && gzmas[zoneid][1] == 0 && gzmas[zoneid][2] == 0 && gzmas[zoneid][3] == 0) {
result = 0;
} else {
result = 1;
}
return result;
}

stock GZ_IsPlayerInGZ(playerid,zoneid) {
new result;
if (IsPlayerInArea(playerid,gzmas[zoneid][0],gzmas[zoneid][2],gzmas[zoneid][1],gzmas[zoneid][3]) == 1) {
result = 1;
} else {
result = 0;
}
return result;
}

stock GZ_IsPlayerInAnyGZ(playerid) {
new result;
for (new gz=0;gz<MAX_GZ;gz++) {
if (IsPlayerInArea(playerid,gzmas[gz][0],gzmas[gz][2],gzmas[gz][1],gzmas[gz][3]) == 1) {
result = 1;
return true;
} else {
result = 0;
}}
return result;
}

//by Y_Less
stock IsPlayerInArea(playerid, Float:minx, Float:maxx, Float:miny, Float:maxy) {
new Float:x, Float:y, Float:z;
GetPlayerPos(playerid, x, y, z);
if (x > minx && x < maxx && y > miny && y < maxy) return 1;
return 0;
}

//by LuxurY TYS 1.4
stock GangZoneCount() {
new cz = GangZoneCreate(3,3,5,5);
GangZoneDestroy(cz);
return cz;
}

public AreaCheck() {
for (new i=0,gz=0; i<MAX_PLAYERS, gz<MAX_GZ; i++, gz++) {
if (gzel[i] == 0) {
if (GZ_IsPlayerInGZ(i,gz) == 1) {
gzel[i] = 1;
GZ_OnPlayerEnter(i,gz);
}}
else if (gzel[i] == 1) {
if (GZ_IsPlayerInGZ(i,gz) == 0) {
gzel[i] = 0;
GZ_OnPlayerLeave(i,gz);
}}}
return 1;
}