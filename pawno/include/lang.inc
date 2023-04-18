#if defined _lang_included
#endinput
#endif

#define _lang_included
#pragma library lang
#include "base"

forward LanguageInit();
forward lang_read(filename[]);

new lang_db[MAX_STRING] = "GTO/Languages/";
new lang[4] = "ru";
new lang_texts[18][99][MAX_STRING];

public LanguageInit()
{

	if (dini_Exists(ConfigDB)) {

		new tmp[sizeof(lang_db)], tmp2[sizeof(lang)];
		
		set(tmp,dini_Get(ConfigDB,"Language_DB"));
		if (strlen(tmp) > 0)  lang_db = tmp; 
		

		set(tmp2,dini_Get(ConfigDB,"Language"));
		if (strlen(tmp2) == 2)  lang = tmp2; 
	}

	new lang_file[50];

	format(lang_file,sizeof(lang_file),"%sGTO.Language.%s.txt",lang_db,lang);
	
	if (!dini_Exists(lang_file)) { printf("no such file %s", lang_file); dini_Create(lang_file);  return; }
	
	lang_read(lang_file);

}

public lang_read(filename[]) {

	new File:fohnd, tmpres[MAX_STRING], tmp1[MAX_STRING], tmp2[MAX_STRING], i;

	fohnd=fopen(filename,io_read);

	if (!fohnd) return;

	while (fread(fohnd,tmpres,sizeof(tmpres))) {
		
		if (strlen(tmpres) == 0) return;
		
		if (strfind(tmpres, "//",true) != -1) continue;
		
		if (strfind(tmpres, ":", true) != -1) 
		{

			strmid(tmp1, tmpres, 0, strfind(tmpres, ":", true));

			strmid(tmp2, tmpres, strfind(tmpres, ":", true)+1, (strlen(tmpres)-strlen(tmp1))); 
			
			lang_texts[i][strval(tmp1)] = tmp2;

		} 
		//		else continue; // tmp2 = tmpres; 

		else if (strfind(tmpres, ":", true) == -1) i++;
		
	}

	fclose(fohnd);
}
