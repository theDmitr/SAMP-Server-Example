
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

stock udb_hash(buf[]) {
	new length=strlen(buf);
    new s1 = 1;
    new s2 = 0;
    new n;
    for (n=0; n<length; n++)
    {
       s1 = (s1 + buf[n]) % 65521;
       s2 = (s2 + s1)     % 65521;
    }
    return (s2 << 16) + s1;
}

stock udb_encode(nickname[])
{
	new tmp[MAX_STRING];
	set(tmp,nickname);
	new newchar = strchar("_");
	
	for (new i=0;i<MAX_STRING;i++)
	{
		if (tmp[i]==0) break; // null
	    if (((tmp[i]>=strchar("a")) && (tmp[i]<=strchar("z"))) || ((tmp[i]>=strchar("A")) && (tmp[i]<=strchar("Z"))) || ((tmp[i]>=strchar("0")) && (tmp[i]<=strchar("9"))))
	    {
			continue;
	    }
	    else
	    {
	        if (tmp[i] == strchar(".")) continue; // temp fix until new db handler is finished
	        tmp[i] = newchar;
		}
	}
	tmp=CharReplace(DBSeperator,newchar,tmp);
	tmp=CharReplace(strchar(" "),newchar,tmp);
	tmp=CharReplace(strchar("\n"),newchar,tmp);
	return tmp;
}


stock udb_Exists(filename[]) {
  new tmp[MAX_STRING];
  new tmp2[MAX_STRING];
  set(tmp2,udb_encode(filename));
  set(tmp,tmp2);
  strcat(tmp,".GTO.DB.1.sav");
  if (fexist(tmp)) return true;
  set(tmp,tmp2);
  strcat(tmp,".GTO.DB.2.sav");
  if (fexist(tmp)) return true;
  set(tmp,tmp2);
  strcat(tmp,".GTO.DB.3.sav");
  if (fexist(tmp)) return true;
  return false;
}


stock udb_Remove(filename[]) {
  new tmp[MAX_STRING];
  new tmp2[MAX_STRING];
  if (!udb_Exists(filename)) return false;
  set(tmp2,udb_encode(filename));
  set(tmp,tmp2);
  strcat(tmp,".GTO.DB.1.sav");
  fremove(tmp);
  set(tmp,tmp2);
  strcat(tmp,".GTO.DB.2.sav");
  fremove(tmp);
  set(tmp,tmp2);
  strcat(tmp,".GTO.DB.3.sav");
  fremove(tmp);
  return true;
}

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

stock udb_Create(filename[],dbstring[]) {
  if (udb_Exists(filename)) return false;
  udb_Savefile(filename,dbstring);
  return true;
}
