#if defined _mapicon
#endinput
#endif

#define _mapicon
#pragma library mapicon

MapIconFunc(playerid) {
//-------------------------------MAP ICONES------------------------------------------//
//		SetPlayerMapIcon(playerid, 5, -1981.6111, 287.9597, 35.0547, 32, 0);

		new MapIconID = 0;
		//HOUSES ICONES ++ 31
		HousesIcones(playerid, MapIconID);

		//TUNING ICONES 27
		SetPlayerMapIcon( playerid, MapIconID++, -2716.514404,218.293624,4.077047, 27, 0 );
		SetPlayerMapIcon( playerid, MapIconID++, -1936.977172,236.758300,33.975135, 27, 0 );
		SetPlayerMapIcon( playerid, MapIconID++, 2386.543457,1042.123413,10.483552, 27, 0 );
		SetPlayerMapIcon( playerid, MapIconID++, 1041.941894,-1027.738281,31.764158, 27, 0 );
		SetPlayerMapIcon( playerid, MapIconID++, 2644.489746,-2037.680908,13.215667, 27, 0 );

		//GYM CLUBS ICONES 54
//		SetPlayerMapIcon( playerid, MapIconID++, -2268.922607,-155.624435,35.320312, 54, 0 );
//		SetPlayerMapIcon( playerid, MapIconID++, 656.276306,-1865.521240,5.460937, 54, 0 );
//		SetPlayerMapIcon( playerid, MapIconID++, 2229.041992,-1722.099121,13.173413, 54, 0 );
//		SetPlayerMapIcon( playerid, MapIconID++, 1966.224365,2295.162109,16.455863, 54, 0 );

		//GIRLS HOME ICONES 21
//		SetPlayerMapIcon( playerid, MapIconID++, 2402.599609,-1712.472778,13.696617, 21, 0 );
//		SetPlayerMapIcon( playerid, MapIconID++, -383.845184,-1438.749511,25.893049, 21, 0 );
//		SetPlayerMapIcon( playerid, MapIconID++, -1800.316040,1203.542846,24.691484, 21, 0 );
//		SetPlayerMapIcon( playerid, MapIconID++, -2573.435791,1154.484741,55.297195, 21, 0 );
//		SetPlayerMapIcon( playerid, MapIconID++, -1390.157470,2643.111328,55.549030, 21, 0 );
//		SetPlayerMapIcon( playerid, MapIconID++, 2037.411987,2718.329833,11.108150, 21, 0 );



		//GROUNDHOLDS ICONES ++ 25
//		GroundholdIcones(playerid, MapIconID);


		//BANK ICONES 52
//		BankIcones(playerid, MapIconID);
//		SetPlayerMapIcon( playerid, MapIconID++, 2204.9468, 1986.2877, 16.7380, 52, 0 );

		//AIRPORTS ICONES 5
 		SetPlayerMapIcon( playerid, MapIconID++, 415.108459,2526.588623,17.554307, 5, 0 );  //OLD

		//CARYERES ICONES 11
 		SetPlayerMapIcon( playerid, MapIconID++, 777.150329,834.015563,6.862725, 11, 0 );

		//GRUZ ICONES 51
 		SetPlayerMapIcon( playerid, MapIconID++, -69.993255,-1122.022460,0.924500, 51, 0 );
//-----------------------------------------------------------------------------------
}

HousesIcones(playerid, MapIconID)
{
	for (new id=0;id<MAX_HOUSES;id++)
	{
	SetPlayerMapIcon( playerid, MapIconID++, HousesCP[id][Coord_X], HousesCP[id][Coord_Y], HousesCP[id][Coord_Z], 31, 0 );
	}
	WriteLog("Housing Icones initialised");
	return MapIconID;
}

//GroundholdIcones(playerid, MapIconID)
//{
//	for (new i=0;i<GROUNDHOLD_SIZE;i++)
//	{
//	SetPlayerMapIcon( playerid, MapIconID++, groundholdCP[i][gh_x],groundholdCP[i][gh_y],groundholdCP[i][gh_z], 25, 0 );
//	}
//	WriteLog("Groundhold Icones initialised");
//	return MapIconID;
//}


//BankIcones(playerid, MapIconID)
//{
//	for (new bankid=0;bankid<BanksCount;bankid++)
//	{
//
//SetPlayerMapIcon( playerid, MapIconID++, Banks[bankid][Coord_X], Banks[bankid][Coord_Y], Banks[bankid][Coord_Z], 52, 0);
//	}
////	WriteLog("Banking Icones initialised");
//	return MapIconID;
//}