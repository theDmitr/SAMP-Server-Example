///////////////////////////////////////////////////
////                                           ////
////        tAxI's COMPLETE ALL-IN-ONE         ////
////                STREAMER TOOLKIT           ////
////                                           ////
////     COPYRIGHT: RELEASED UNDER the GNU     ////
////                 LICENCE                   ////
////                                           ////
///////////////////////////////////////////////////  

/*
native SetActiveStreamers(bool:vehicle,bool:object,bool:pickup,bool:minimap)
native ResetVehicleStream(vehicleid,delay);
native SetVehicleUsed(vehicleid,bool:toggle);
native CreateStreamVehicle(modelid,Float:x,Float:y,Float:z,Float:a,col1,col2,spawn_distance)
native DestroyStreamVehicle(vstreamid)
native GetPlayerVehicleStreamID(playerid)
native SetVehicleStreamPos(vstreamid,Float:x,Float:y,Float:z,Float:a)
native SetVehicleStreamModel(vstreamid,model)
native ChangeStreamVehiclePaintjob(vstreamid,paintjobid)
native ChangeStreamVehicleColor(vstreamid,col1,col2)
native AddStreamVehicleComponent(vstreamid,componentid)
native CreateStreamMapIcon(markermodel,Float:x,Float:y,Float:z,Float:spawn_distance);
native DestroyStreamMapIcon(istreamid);
native CreateStreamObject(objectmodel,Float:x,Float:y,Float:z,Float:spawn_distance);
native DestroyStreamObject(ostreamid);
*/

#define MAX_OVERALL_CIRC 5000
#define VEHICLE_STREAMER 1
#define OBJECT_STREAMER 2
#define MINIMAP_STREAMER 3
#define PICKUP_STREAMER 4
#define MAX_ACTIVE_VEHICLES 680
#define MAX_MODEL_NUMBER 700
#define MAX_ACTIVE_MODELS 68
#define MAX_ACTIVE_MM_ICONS 32
#define MAX_ACTIVE_PICKUPS 100
#define MAX_ACTIVE_OBJECTS 200

forward proxcheck();
forward ResetStreamVehicle(vehicleid);
forward Automod(vstreamid);

new spoiler[20][0] = {
	{1000},
	{1001},
	{1002},
	{1003},
	{1014},
	{1015},
	{1016},
	{1023},
	{1058},
	{1060},
	{1049},
	{1050},
	{1138},
	{1139},
	{1146},
	{1147},
	{1158},
	{1162},
	{1163},
	{1164}
};

new nitro[3][0] = {
    {1008},
    {1009},
    {1010}
};

new fbumper[23][0] = {
    {1117},
    {1152},
    {1153},
    {1155},
    {1157},
    {1160},
    {1165},
    {1167},
    {1169},
    {1170},
    {1171},
    {1172},
    {1173},
    {1174},
    {1175},
    {1179},
    {1181},
    {1182},
    {1185},
    {1188},
    {1189},
    {1192},
    {1193}
};

new rbumper[22][0] = {
    {1140},
    {1141},
    {1148},
    {1149},
    {1150},
    {1151},
    {1154},
    {1156},
    {1159},
    {1161},
    {1166},
    {1168},
    {1176},
    {1177},
    {1178},
    {1180},
    {1183},
    {1184},
    {1186},
    {1187},
    {1190},
    {1191}
};

new exhaust[28][0] = {
    {1018},
    {1019},
    {1020},
    {1021},
    {1022},
    {1028},
    {1029},
    {1037},
    {1043},
    {1044},
    {1045},
    {1046},
    {1059},
    {1064},
    {1065},
    {1066},
    {1089},
    {1092},
    {1104},
    {1105},
    {1113},
    {1114},
    {1126},
    {1127},
    {1129},
    {1132},
    {1135},
    {1136}
};

new bventr[2][0] = {
    {1042},
    {1044}
};

new bventl[2][0] = {
    {1043},
    {1045}
};

new bscoop[4][0] = {
	{1004},
	{1005},
	{1011},
	{1012}
};

new rscoop[13][0] = {
    {1006},
    {1032},
    {1033},
    {1035},
    {1038},
    {1053},
    {1054},
    {1055},
    {1061},
    {1067},
    {1068},
    {1088},
    {1091}
};

new lskirt[21][0] = {
    {1007},
    {1026},
    {1031},
    {1036},
    {1039},
    {1042},
    {1047},
    {1048},
    {1056},
    {1057},
    {1069},
    {1070},
    {1090},
    {1093},
    {1106},
    {1108},
    {1118},
    {1119},
    {1133},
    {1122},
    {1134}
};

new rskirt[21][0] = {
    {1017},
    {1027},
    {1030},
    {1040},
    {1041},
    {1051},
    {1052},
    {1062},
    {1063},
    {1071},
    {1072},
    {1094},
    {1095},
    {1099},
    {1101},
    {1102},
    {1107},
    {1120},
    {1121},
    {1124},
    {1137}
};

new hydraulics[1][0] = {
    {1087}
};

new base[1][0] = {
    {1086}
};

new rbbars[2][0] = {
    {1109},
    {1110}
};

new fbbars[2][0] = {
    {1115},
    {1116}
};

new wheels[17][0] = {
    {1025},
    {1073},
    {1074},
    {1075},
    {1076},
    {1077},
    {1078},
    {1079},
    {1080},
    {1081},
    {1082},
    {1083},
    {1084},
    {1085},
    {1096},
    {1097},
    {1098}
};

new lights[2][0] = {
	{1013},
	{1024}
};

new vstreamactive = false;
new mmstreamactive = false;
new obstreamactive = false;
//new pustreamactive = false;
new modelcount[MAX_MODEL_NUMBER];
new vehcount = 0;
new Vstreamcount = 0;
new vehused[MAX_ACTIVE_VEHICLES];
new vehdead[MAX_ACTIVE_VEHICLES];
new MMstreamcount[MAX_PLAYERS];
//new PUstreamcount[MAX_PLAYERS];
new OBstreamcount[MAX_PLAYERS];
new MIactive[MAX_PLAYERS][MAX_OVERALL_CIRC];
new OBactive[MAX_PLAYERS][MAX_OVERALL_CIRC];
//new PUactive[MAX_PLAYERS][MAX_OVERALL_CIRC];
new MIidnum[MAX_PLAYERS][MAX_OVERALL_CIRC];
new OBidnum[MAX_PLAYERS][MAX_OVERALL_CIRC];
//new PUidnum[MAX_PLAYERS][MAX_OVERALL_CIRC];

enum vehinfo
{
	vmodel,
	Float:vx_spawn,
	Float:vy_spawn,
	Float:vz_spawn,
 	Float:vza_spawn,
 	vcolor_1,
 	vcolor_2,
	vspawned,
	vidnum,
	Float:vspawndist,
 	vvalid,
 	mod1,
	mod2,
	mod3,
	mod4,
	mod5,
	mod6,
	mod7,
	mod8,
	mod9,
	mod10,
	mod11,
	mod12,
	mod13,
	mod14,
	mod15,
	mod16,
	mod17,
	paintjob,
};
new VehInfo[MAX_OVERALL_CIRC][vehinfo];

enum objectinfo
{
	omodel,
	Float:ox_spawn,
	Float:oy_spawn,
	Float:oz_spawn,
 	Float:orx_spawn,
 	Float:ory_spawn,
 	Float:orz_spawn,
	Float:ospawndist,
	ovalid,
};
new ObjectInfo[MAX_OVERALL_CIRC][objectinfo];

enum mmapinfo
{
	mmodel,
	Float:mx_spawn,
	Float:my_spawn,
	Float:mz_spawn,
	Float:mspawndist,
	mvalid,
};
new MapIconInfo[MAX_OVERALL_CIRC][mmapinfo];

enum pickupinfo
{
	pmodel,
	Float:px_spawn,
	Float:py_spawn,
	Float:pz_spawn,
	Float:pspawndist,
	pidnum,
	ptype,
	pvalid,
};
new PickupInfo[MAX_OVERALL_CIRC][pickupinfo];

stock SetActiveStreamers(bool:vehicle,bool:object,bool:pickup,bool:minimap)
{
	SetTimer("proxcheck",1000,1);
	if(vehicle == true) {vstreamactive = 1;}
	if(object == true) {obstreamactive = 1;}
	if(minimap == true) {mmstreamactive = 1;}
	//if(pickup == true) {pustreamactive = 1;}
	if(vehicle == false) {vstreamactive = 0;}
	if(object == false) {obstreamactive = 0;}
	if(minimap == false) {mmstreamactive = 0;}
	//if(pickup == false) {pustreameractive = 0;}
	return 1;
}

stock ResetVehicleStream(vehicleid,delay)
{
	SetVehicleToRespawn(vehicleid);
	SetVehicleVirtualWorld(vehicleid,10);
	if(delay < 5000) {
	    delay = 5000;
	}
	SetTimerEx("ResetStreamVehicle",delay,0,"x",vehicleid);
	vehdead[vehicleid] = 1;
	return 1;
}

stock SetVehicleUsed(vehicleid,bool:toggle)
{
	vehused[vehicleid] = toggle;
	return 1;
}

public proxcheck()
{
    for(new i = 1;i<MAX_OVERALL_CIRC;i++) {
		if(vstreamactive == 1) {
			if(VehInfo[i][vspawned] == 0 && VehInfo[i][vvalid] == 1) {
				if(IsPlayerCloseToStream(VEHICLE_STREAMER,i,VehInfo[i][vspawndist]) == 1) {
				    if(Vstreamcount <= MAX_ACTIVE_VEHICLES) {
				        if(modelcount[VehInfo[i][vmodel]] < MAX_ACTIVE_MODELS) {
							VehInfo[i][vidnum] = CreateVehicle(VehInfo[i][vmodel], VehInfo[i][vx_spawn], VehInfo[i][vy_spawn], VehInfo[i][vz_spawn], VehInfo[i][vza_spawn], VehInfo[i][vcolor_1], VehInfo[i][vcolor_2],11000);
							VehInfo[i][vspawned] = 1;
  	   	              		modelcount[VehInfo[i][vmodel]]++;
							Vstreamcount++;
							SetTimerEx("Automod",250,0,"x",i);
						}
					}
				}
			}
			else {
				if(vehused[VehInfo[i][vidnum]] == 0 && VehInfo[i][vvalid] == 1) {
		    		if(IsPlayerCloseToStream(VEHICLE_STREAMER,i,VehInfo[i][vspawndist]) == 0 || vehdead[VehInfo[i][vidnum]] == 1) {
   		         		DestroyVehicle(VehInfo[i][vidnum]);
						vehdead[VehInfo[i][vidnum]] = 0;
						VehInfo[i][vspawned] = 0;
		         		vehused[VehInfo[i][vidnum]] = 0;
						modelcount[VehInfo[i][vmodel]]--;
						Vstreamcount--;
						VehInfo[i][vidnum] = 0;
					}
				}
				else {
					if(VehInfo[i][vvalid] == 0) {
				    	SetVehicleToRespawn(VehInfo[i][vidnum]);
						SetVehicleVirtualWorld(VehInfo[i][vidnum],10);
						SetTimerEx("RemoveVehicleStream",5000,0,"x",VehInfo[i][vidnum]);
					}
				}
			}
		}
		for(new j = 0;j<MAX_PLAYERS;j++) {
	   		if(mmstreamactive == 1) {
				if(MIactive[j][i] == 0 && MapIconInfo[i][mvalid] == 1) {
				   	if(MMstreamcount[j] < MAX_ACTIVE_MM_ICONS) {
						if(IsPlayerClose(j,MapIconInfo[i][mx_spawn],MapIconInfo[i][my_spawn],MapIconInfo[i][mz_spawn],MapIconInfo[i][mspawndist]) == 1) {
							SetPlayerMapIcon(j,MIidnum[j][i],MapIconInfo[i][mx_spawn],MapIconInfo[i][my_spawn],MapIconInfo[i][mz_spawn],MapIconInfo[i][mmodel],1);
							MMstreamcount[j]++;
							MIactive[j][i] = 1;
						}
					}
				}
				if(MapIconInfo[i][mvalid] == 0) {
					if(MIactive[j][MIidnum[j][i]] == 1) {
						RemovePlayerMapIcon(j,MIidnum[j][i]);
						MIactive[j][i] = 0;
						MMstreamcount[j]--;
					}
				}
				else {
					if(IsPlayerClose(j,MapIconInfo[i][mx_spawn],MapIconInfo[i][my_spawn],MapIconInfo[i][mz_spawn],MapIconInfo[i][mspawndist]) == 0) {
						RemovePlayerMapIcon(j,MIidnum[j][i]);
						MIactive[j][i] = 0;
						MMstreamcount[j]--;
					}
				}
			}
			if(obstreamactive == 1) {
	  			if(OBactive[j][i] == 0 && ObjectInfo[i][ovalid] == 1) {
	   			   	if(OBstreamcount[j] < MAX_ACTIVE_OBJECTS) {
						if(IsPlayerClose(j,ObjectInfo[i][ox_spawn],ObjectInfo[i][oy_spawn],ObjectInfo[i][oz_spawn],ObjectInfo[i][ospawndist]) == 1) {
							OBidnum[j][i] = CreatePlayerObject(j,ObjectInfo[i][omodel],ObjectInfo[i][ox_spawn],ObjectInfo[i][oy_spawn],ObjectInfo[i][oz_spawn],ObjectInfo[i][orx_spawn],ObjectInfo[i][ory_spawn],ObjectInfo[i][orz_spawn]);
                            OBactive[j][i] = 1;
							OBstreamcount[j]++;
						}
					}
				}
				if(ObjectInfo[i][ovalid] == 0) {
			       	if(OBactive[j][i] == 1) {
						DestroyPlayerObject(j,OBidnum[j][i]);
						OBactive[j][i] = 0;
						OBstreamcount[j]--;
					}
				}
				else {
					if(IsPlayerClose(j,ObjectInfo[i][ox_spawn],ObjectInfo[i][oy_spawn],ObjectInfo[i][oz_spawn],ObjectInfo[i][ospawndist]) == 1) {
						DestroyPlayerObject(j,OBidnum[j][i]);
						OBactive[j][i] = 0;
						OBstreamcount[j]--;
					}
	   			}
			}
		}
	}
}

stock RemoveVehicleStream(streamid)
{
    DestroyVehicle(VehInfo[streamid][vidnum]);
	VehInfo[streamid][vspawned] = 0;
	modelcount[VehInfo[streamid][vmodel]]--;
	Vstreamcount--;
	VehInfo[streamid][vidnum] = 0;
}

stock CreateStreamVehicle(modelid,Float:x,Float:y,Float:z,Float:a,col1,col2,Float:spawn_distance)
{
	vehcount++;
	for(new i = 1;i<MAX_OVERALL_CIRC;i++) {
	    if(VehInfo[i][vvalid] == 0) {
			VehInfo[i][vmodel] = modelid;
			VehInfo[i][vx_spawn] = x;
			VehInfo[i][vy_spawn] = y;
			VehInfo[i][vz_spawn] = z;
			VehInfo[i][vza_spawn] = a;
			VehInfo[i][vcolor_1] = col1;
			VehInfo[i][vcolor_2] = col2;
			VehInfo[i][vspawndist] = spawn_distance;
			VehInfo[i][vvalid] = 1;
			VehInfo[i][vidnum] = 0;
			VehInfo[i][paintjob] = -1;
			return i;
		}
	}
	return 0;
}

stock DestroyStreamVehicle(vstreamid)
{
	vehcount--;
   	if(VehInfo[vstreamid][vvalid] == 1) {
		VehInfo[vstreamid][vvalid] = 0;
		VehInfo[vstreamid][vidnum] = 0;
		return 1;
	}
	return 0;
}

stock GetPlayerVehicleStreamID(playerid)
{
	if(IsPlayerInAnyVehicle(playerid)) {
	    new vehicleid = GetPlayerVehicleID(playerid);
	    for(new i = 1;i<MAX_OVERALL_CIRC;i++) {
			if(vehicleid == VehInfo[i][vidnum]) {
			    return i;
			}
		}
	}
	return 0;
}

stock SetVehicleStreamPos(vstreamid,Float:x,Float:y,Float:z,Float:a)
{
	if(VehInfo[vstreamid][vvalid] == 1) {
	    VehInfo[vstreamid][vx_spawn] = x;
	    VehInfo[vstreamid][vy_spawn] = y;
	    VehInfo[vstreamid][vz_spawn] = z;
	    VehInfo[vstreamid][vza_spawn] = a;
	    return 1;
	}
	return 0;
}

stock SetVehicleStreamModel(vstreamid,model)
{
	if(VehInfo[vstreamid][vvalid] == 1) {
	    VehInfo[vstreamid][vmodel] = model;
	    return 1;
	}
	return 0;
}

stock ChangeStreamVehiclePaintjob(vstreamid,paintjobid)
{
    if(VehInfo[vstreamid][vvalid] == 1) {
        VehInfo[vstreamid][paintjob] = paintjobid;
        if(VehInfo[vstreamid][vspawned] == 1) {
            ChangeVehiclePaintjob(VehInfo[vstreamid][idnum],paintjobid);
        }
        return 1;
    }
    return 0;
}

stock ChangeStreamVehicleColor(vstreamid,col1,col2)
{
    if(VehInfo[vstreamid][vvalid] == 1) {
        VehInfo[vstreamid][color_1] = col1;
        VehInfo[vstreamid][color_2] = col2;
        if(VehInfo[vstreamid][vspawned] == 1) {
            ChangeVehicleColor(VehInfo[vstreamid][idnum],col1,col2);
        }
        return 1;
    }
    return 0;
}

stock AddStreamVehicleComponent(vstreamid,componentid)
{
	if(VehInfo[vstreamid][vvalid] == 1) {
	    for(new s=0; s<20; s++) {
   			if(componentid == spoiler[s][0]) {
   				VehInfo[vstreamid][mod1] = componentid;
   	   		}
		}
		for(new s=0; s<3; s++) {
			if(componentid == nitro[s][0]) {
				VehInfo[vstreamid][mod2] = componentid;
    		}
		}
		for(new s=0; s<23; s++) {
			if(componentid == fbumper[s][0]) {
					VehInfo[vstreamid][mod3] = componentid;
    		}
		}
		for(new s=0; s<22; s++) {
			if(componentid == rbumper[s][0]) {
				VehInfo[vstreamid][mod4] = componentid;
	  		}
		}
		for(new s=0; s<28; s++) {
			if(componentid == exhaust[s][0]) {
				VehInfo[vstreamid][mod5] = componentid;
     		}
		}
		for(new s=0; s<2; s++) {
			if(componentid == bventr[s][0]) {
				VehInfo[vstreamid][mod6] = componentid;
	   		}
		}
		for(new s=0; s<2; s++) {
			if(componentid == bventl[s][0]) {
				VehInfo[vstreamid][mod7] = componentid;
   			}
		}
		for(new s=0; s<4; s++) {
			if(componentid == bscoop[s][0]) {
				VehInfo[vstreamid][mod8] = componentid;
			}
		}
		for(new s=0; s<13; s++) {
			if(componentid == rscoop[s][0]) {
				VehInfo[vstreamid][mod9] = componentid;
   			}
		}
		for(new s=0; s<21; s++) {
			if(componentid == lskirt[s][0]) {
				VehInfo[vstreamid][mod10] = componentid;
    		}
		}
		for(new s=0; s<21; s++) {
			if(componentid == rskirt[s][0]) {
				VehInfo[vstreamid][mod11] = componentid;
   			}
		}
		for(new s=0; s<1; s++) {
			if(componentid == hydraulics[s][0]) {
				VehInfo[vstreamid][mod12] = componentid;
    		}
		}
		for(new s=0; s<1; s++) {
			if(componentid == base[s][0]) {
				VehInfo[vstreamid][mod13] = componentid;
    		}
		}
		for(new s=0; s<2; s++) {
			if(componentid == rbbars[s][0]) {
				VehInfo[vstreamid][mod14] = componentid;
     		}
		}
		for(new s=0; s<2; s++) {
			if(componentid == fbbars[s][0]) {
				VehInfo[vstreamid][mod15] = componentid;
     		}
		}
		for(new s=0; s<17; s++) {
			if(componentid == wheels[s][0]) {
				VehInfo[vstreamid][mod16] = componentid;
    		}
		}
		for(new s=0; s<2; s++) {
			if(componentid == lights[s][0]) {
				VehInfo[vstreamid][mod17] = componentid;
	  		}
		}
	    if(VehInfo[vstreamid][vspawned] == 1) {
	        AddVehicleComponent(VehInfo[vstreamid][vidnum],componentid);
	    }
	    return 1;
	}
	return 0;
}

public Automod(vstreamid)
{
    ChangeVehicleColor(vstreamid,VehInfo[vstreamid][vcolor_1],VehInfo[vstreamid][vcolor_2]);
	if(VehInfo[vstreamid][mod1] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod1]);
	}
	if(VehInfo[vstreamid][mod2] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod2]);
	}
	if(VehInfo[vstreamid][mod3] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod3]);
	}
	if(VehInfo[vstreamid][mod4] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod4]);
	}
	if(VehInfo[vstreamid][mod5] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod5]);
	}
	if(VehInfo[vstreamid][mod6] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod6]);
	}
	if(VehInfo[vstreamid][mod7] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod7]);
	}
	if(VehInfo[vstreamid][mod8] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod8]);
 	}
	if(VehInfo[vstreamid][mod9] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod9]);
	}
	if(VehInfo[vstreamid][mod10] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod10]);
	}
	if(VehInfo[vstreamid][mod11] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod11]);
	}
	if(VehInfo[vstreamid][mod12] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod12]);
	}
	if(VehInfo[vstreamid][mod13] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod13]);
	}
	if(VehInfo[vstreamid][mod14] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod14]);
	}
	if(VehInfo[vstreamid][mod15] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod15]);
	}
	if(VehInfo[vstreamid][mod16] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod16]);
	}
	if(VehInfo[vstreamid][mod17] != 0) {
		AddVehicleComponent(vstreamid,VehInfo[vstreamid][mod17]);
	}
	if(VehInfo[vstreamid][paintjob] > -1) {
		ChangeVehiclePaintjob(vstreamid,VehInfo[vstreamid][paintjob]);
 	}
 	return 1;
}

public ResetStreamVehicle(vehicleid)
{
    SetVehicleUsed(vehicleid,false);
	return 1;
}

stock CreateStreamMapIcon(markermodel,Float:x,Float:y,Float:z,Float:spawn_distance)
{
	for(new i = 1;i<MAX_OVERALL_CIRC;i++) {
		if(MapIconInfo[i][mvalid] == 0) {
			MapIconInfo[i][mmodel] = markermodel;
			for(new j=0;j<MAX_PLAYERS;j++) {
				MIidnum[j][i] = i;
			}
			MapIconInfo[i][mx_spawn] = x;
			MapIconInfo[i][my_spawn] = y;
			MapIconInfo[i][mz_spawn] = z;
			MapIconInfo[i][mspawndist] = spawn_distance;
			MapIconInfo[i][mvalid] = 1;
			return i;
		}
	}
	return 0;
}

stock DestroyStreamMapIcon(istreamid)
{
	MapIconInfo[istreamid][mvalid] = 0;
	return 1;
}

stock CreateStreamObject(objectmodel,Float:x,Float:y,Float:z,Float:xr,Float:yr,Float:zr,Float:spawn_distance)
{
	for(new i = 1;i<MAX_OVERALL_CIRC;i++) {
		if(ObjectInfo[i][ovalid] == 0) {
			ObjectInfo[i][omodel] = objectmodel;
			ObjectInfo[i][ox_spawn] = x;
			ObjectInfo[i][oy_spawn] = y;
			ObjectInfo[i][oz_spawn] = z;
			ObjectInfo[i][orx_spawn] = xr;
			ObjectInfo[i][ory_spawn] = yr;
			ObjectInfo[i][orz_spawn] = zr;
			ObjectInfo[i][ospawndist] = spawn_distance;
			ObjectInfo[i][ovalid] = 1;
			return i;
		}
	}
	return 0;
}

stock DestroyStreamObject(ostreamid)
{
	ObjectInfo[ostreamid][ovalid] = 0;
	return 1;
}

/*public pickupcheck()
{
	for(new i = 1;i<pucount;i++) {
		if(PickupInfo[i][pspawned] == 0) {
			if(IsPlayerCloseToStream(PICKUP_STREAMER,i,PickupInfo[i][pspawndist]) == 1) {
			    if(PUstreamcount < MAX_ACTIVE_PICKUPS) {
					PickupInfo[i][pidnum] = CreatePickup(PickupInfo[i][pmodel],PickupInfo[i][ptype],PickupInfo[i][px_spawn],PickupInfo[i][py_spawn],PickupInfo[i][pz_spawn]);
      				PickupInfo[i][pspawned] = 1;
					PUstreamcount++;
				}
			}
		}
		else {
			if(IsPlayerCloseToStream(PICKUP_STREAMER,i,PickupInfo[i][pspawndist]) == 0) {
				DestroyPickup(PickupInfo[i][pidnum]);
 				PickupInfo[i][pspawned] = 0;
				PUstreamcount--;
			}
		}
	}
}

stock AddStreamPickup(pickupmodel,type,Float:x,Float:y,Float:z,Float:spawn_distance)
{
	pucount++;
	PickupInfo[pucount][pmodel] = pickupmodel;
	PickupInfo[pucount][ptype] = type;
	PickupInfo[pucount][px_spawn] = x;
	PickupInfo[pucount][py_spawn] = y;
	PickupInfo[pucount][py_spawn] = z;
	PickupInfo[pucount][pspawndist] = spawn_distance;
	return 1;
}*/

stock IsPlayerClose(playerid,Float:x,Float:y,Float:z,Float:MAX)
{
	new Float:PPos[3];
	GetPlayerPos(playerid, PPos[0], PPos[1], PPos[2]);
	if (PPos[0] >= floatsub(x, MAX) && PPos[0] <= floatadd(x, MAX)
	&& PPos[1] >= floatsub(y, MAX) && PPos[1] <= floatadd(y, MAX)
	&& PPos[2] >= floatsub(z, MAX) && PPos[2] <= floatadd(z, MAX))
	{
		return 1;
	}
	return 0;
}

stock IsPlayerCloseToStream(streamtype,streamid,Float:MAX)
{
    new Float:PPos[3];
	if(streamtype == VEHICLE_STREAMER) {
   		for(new i = 0;i<MAX_PLAYERS;i++) {
   		    if(IsPlayerConnected(i)) {
   				GetPlayerPos(i, PPos[0], PPos[1], PPos[2]);
				if (PPos[0] >= floatsub(VehInfo[streamid][vx_spawn], MAX) && PPos[0] <= floatadd(VehInfo[streamid][vx_spawn], MAX)
				&& PPos[1] >= floatsub(VehInfo[streamid][vy_spawn], MAX) && PPos[1] <= floatadd(VehInfo[streamid][vy_spawn], MAX)
				&& PPos[2] >= floatsub(VehInfo[streamid][vz_spawn], MAX) && PPos[2] <= floatadd(VehInfo[streamid][vz_spawn], MAX))
				{
					return 1;
				}
			}
		}
	}
	if(streamtype == OBJECT_STREAMER) {
    	for(new i = 0;i<MAX_PLAYERS;i++) {
    	    if(IsPlayerConnected(i)) {
    			GetPlayerPos(i, PPos[0], PPos[1], PPos[2]);
				if (PPos[0] >= floatsub(ObjectInfo[streamid][ox_spawn], MAX) && PPos[0] <= floatadd(ObjectInfo[streamid][ox_spawn], MAX)
				&& PPos[1] >= floatsub(ObjectInfo[streamid][oy_spawn], MAX) && PPos[1] <= floatadd(ObjectInfo[streamid][oy_spawn], MAX)
				&& PPos[2] >= floatsub(ObjectInfo[streamid][oz_spawn], MAX) && PPos[2] <= floatadd(ObjectInfo[streamid][oz_spawn], MAX))
				{
					return 1;
				}
			}
		}
	}
	if(streamtype == MINIMAP_STREAMER) {
    	for(new i = 0;i<MAX_PLAYERS;i++) {
    	    if(IsPlayerConnected(i)) {
    			GetPlayerPos(i, PPos[0], PPos[1], PPos[2]);
				if (PPos[0] >= floatsub(MapIconInfo[streamid][mx_spawn], MAX) && PPos[0] <= floatadd(MapIconInfo[streamid][mx_spawn], MAX)
				&& PPos[1] >= floatsub(MapIconInfo[streamid][my_spawn], MAX) && PPos[1] <= floatadd(MapIconInfo[streamid][my_spawn], MAX))
				{
					return 1;
				}
			}
		}
	}
	if(streamtype == PICKUP_STREAMER) {
    	for(new i = 0;i<MAX_PLAYERS;i++) {
    		GetPlayerPos(i, PPos[0], PPos[1], PPos[2]);
			if (PPos[0] >= floatsub(PickupInfo[streamid][px_spawn], MAX) && PPos[0] <= floatadd(PickupInfo[streamid][px_spawn], MAX)
			&& PPos[1] >= floatsub(PickupInfo[streamid][py_spawn], MAX) && PPos[1] <= floatadd(PickupInfo[streamid][py_spawn], MAX)
			&& PPos[2] >= floatsub(PickupInfo[streamid][pz_spawn], MAX) && PPos[2] <= floatadd(PickupInfo[streamid][pz_spawn], MAX))
			{
				return 1;
			}
		}
	}
	return 0;
}
