
/*
//  DUDB GTO 1.2.01 Mod
*/

/*
 *            DUDB 1.2
 *       (c) Copyright 2006 by DracoBlue
 *
 * @author    : DracoBlue (http://dracoblue.com)
 * @date      : 8th May 2006
 * @update    : 31st July 2006
 * @require   : DUtils 1.3
 *
 * This file is provided as is (no warranties).
 *
 * Feel free to use it, a little message in 
 * about box is honouring thing, isn't it?
 *
 */

/*
//
//  Modification information:
//
//                          Author: 	Roach
//                          Date:       14.08.06
//
//                          Notes:      
//
//										I needed file/db saving, modding DracoBlue's
//                                      DUDB to do what I want is just quicker and already tested.
//                                      Thanks go to DracoBlue.
//
//
//
*/

#if defined _gtodudb_included
  #endinput
#endif

#define _gtodudb_included
#pragma library dutils

#include "dutils"

#define MAX_DBSTRING 255 // sending a single string to write whole db is a prolly bad way to do it, but i dont know how else i can send as many values as i need without hardcoding. feel free to advise me :/



stock udp_PRIVATE_clearbroken(filename[]) {
  new tmp2[MAX_STRING];
  new tmp_a[MAX_STRING];
  new tmp_b[MAX_STRING];
  set(tmp2,filename);
  set(tmp_a,tmp2);strcat(tmp_a,".GTO.DB.1.sav");
  set(tmp_b,tmp2);strcat(tmp_b,".GTO.DB.2.sav");
  if ((fexist(tmp_a))&&(fexist(tmp_b))) {
	 fremove(tmp_b);
  } else {
    set(tmp_a,tmp2);strcat(tmp_a,".GTO.DB.2.sav");
    set(tmp_b,tmp2);strcat(tmp_b,".GTO.DB.3.sav");
    if ((fexist(tmp_a))&&(fexist(tmp_b))) {
  	   fremove(tmp_b);
    } else {
       set(tmp_a,tmp2);strcat(tmp_a,".GTO.DB.3.sav");
       set(tmp_b,tmp2);strcat(tmp_b,".GTO.DB.1.sav");
       if ((fexist(tmp_a))&&(fexist(tmp_b))) {
         fremove(tmp_b);
       } else {

       }
    }
  }
}

stock udb_Savefile(filename[], dbstring[]) {
  new File:fhnd;
  new name[MAX_STRING];
  new tmp_a[MAX_STRING];
  new tmp_b[MAX_STRING];
  set(name,udb_encode(filename));
  udp_PRIVATE_clearbroken(name);
  set(tmp_a,name);strcat(tmp_a,".GTO.DB.1.sav");
  set(tmp_b,name);strcat(tmp_b,".GTO.DB.2.sav");
  if ((fexist(tmp_a))) {
	 fhnd=fopen(tmp_b,io_write);
	 fwrite(fhnd,dbstring);
	 fclose(fhnd);
	 fremove(tmp_a);
     return true;
  }

  set(tmp_a,name);strcat(tmp_a,".GTO.DB.2.sav");
  set(tmp_b,name);strcat(tmp_b,".GTO.DB.3.sav");
  if ((fexist(tmp_a))) {
	 fhnd=fopen(tmp_b,io_write);
	 fwrite(fhnd,dbstring);
	 fclose(fhnd);
	 fremove(tmp_a);
     return true;
  }

  set(tmp_a,name);strcat(tmp_a,".GTO.DB.3.sav");
  set(tmp_b,name);strcat(tmp_b,".GTO.DB.1.sav");
  if ((fexist(tmp_a))) {
	 fhnd=fopen(tmp_b,io_write);
	 fwrite(fhnd,dbstring);
	 fclose(fhnd);
	 fremove(tmp_a);
     return true;
  }
	 fhnd=fopen(tmp_b,io_write);
	 fwrite(fhnd,dbstring);
	 fclose(fhnd);
  return false;
}

stock udb_Loadfile(filename[]) {
  new tmpres[MAX_DBSTRING];
  tmpres[0]=0;
  new File:fhnd;
  new name[MAX_STRING];
  new tmp_a[MAX_STRING];
  set(name,udb_encode(filename));
  udp_PRIVATE_clearbroken(name);
  set(tmp_a,name);strcat(tmp_a,".GTO.DB.1.sav");
  if ((fexist(tmp_a))) {
	 fhnd=fopen(tmp_a,io_read);
	 fread(fhnd,tmpres);
	 fclose(fhnd);
  }
  set(tmp_a,name);strcat(tmp_a,".GTO.DB.2.sav");
  if ((fexist(tmp_a))) {
	 fhnd=fopen(tmp_a,io_read);
	 fread(fhnd,tmpres);
	 fclose(fhnd);
  }
  set(tmp_a,name);strcat(tmp_a,".GTO.DB.3.sav");
  if ((fexist(tmp_a))) {
	 fhnd=fopen(tmp_a,io_read);
	 fread(fhnd,tmpres);
	 fclose(fhnd);
  }
  StripNewLine(tmpres);
  return tmpres;
}
