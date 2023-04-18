//
// Created:     27.03.07
// Aurthor:    Peter Steenbergen
//

#if defined _logging_included
  #endinput
#endif

#define _logging_included
#pragma library logging
#include "base"

forward LoggingInit();
forward WriteLog(string[]);
forward WriteChatLog(string[]);
forward WriteCMDLog(string[]);
forward Debug(string[]);

new LogFilePath[MAX_STRING] = "GTO/Logging/Gamelog/";
new ChatLogFilePath[MAX_STRING] = "GTO/Logging/Chatlog/";
new CMDLogFilePath[MAX_STRING] = "GTO/Logging/CMDlog/";
new File:LogFile;

new LoggingToggle = 1;
new LoggingDebug = 1;
new LoggingChatToggle = 1;
new LoggingCMDToggle = 1;

LoggingLoadConfig()
{
	if (ForceOverwrite) return;
	if (!dini_Exists(ConfigDB)) return;
	new temp[MAX_STRING];
	set(temp,dini_Get(ConfigDB,"Logging_on"));
	if (strlen(temp) > 0) LoggingToggle = strval(temp);
	set(temp,nullstr);
	set(temp,dini_Get(ConfigDB,"Debug_on"));
	if (strlen(temp) > 0) LoggingDebug = strval(temp);
    set(temp,nullstr);
    set(temp,dini_Get(ConfigDB,"ChatLog_on"));
	if (strlen(temp) > 0) LoggingChatToggle = strval(temp);
    set(temp,nullstr);
    set(temp,dini_Get(ConfigDB,"CMDLog_on"));
	if (strlen(temp) > 0) LoggingCMDToggle = strval(temp);
    set(temp,nullstr);
	return;
}

LoggingSaveConfig()
{
	if (!dini_Exists(ConfigDB)) return;
	new temp[MAX_STRING];
	valstr(temp,LoggingToggle);
	dini_Set(ConfigDB,"Logging_on",temp);
	valstr(temp,LoggingDebug);
	dini_Set(ConfigDB,"Debug_on",temp);
	valstr(temp,LoggingChatToggle);
	dini_Set(ConfigDB,"ChatLog_on",temp);
	valstr(temp,LoggingCMDToggle);
	dini_Set(ConfigDB,"CMDLog_on",temp);
	return;
}

public LoggingInit()
{

    WriteLog("========== GameMode (re)started ==========");
}

stock FormatFigures(start[], f1, sep1[], f2, sep2[], f3, end[])
{
	new string[256];
	if (f1 < 10)
	{
	    format(string, sizeof (string), "%s0%d%s", start, f1, sep1);
	}
	else
	{
	    format(string, sizeof (string), "%s%d%s", start, f1, sep1);
	}
	if (f2 < 10)
	{
	    format(string, sizeof (string), "%s0%d%s", string, f2, sep2);
	}
	else
	{
	    format(string, sizeof (string), "%s%d%s", string, f2, sep2);
	}
	if (f3 < 10)
	{
	    format(string, sizeof (string), "%s0%d%s", string, f3, end);
	}
	else
	{
	    format(string, sizeof (string), "%s%d%s", string, f3, end);
	}
	return string;
}

stock MakeFilename()
{
	new d, m, y;
	getdate(y, m, d);
	new string[256];
	string = FormatFigures("GTO_", d, "-", m, "-", y, ".log");
	return string;
}

stock MakeFormattedTime()
{
	new h, m, s;
	gettime(h, m, s);
	new string[256];
	string = FormatFigures("[", h, ":", m, ":", s, "]");
	return string;
}

public WriteLog(string[])
{
	if (LoggingToggle > 0)
	{
		new filestring[256];
	    format(filestring,sizeof(filestring),"%s%s",LogFilePath,MakeFilename());

		if (fexist(filestring))
		{
	        LogFile = fopen(filestring, io_append);
		}
		else
		{
	        LogFile = fopen(filestring, io_write);
		}

		fwrite(LogFile, MakeFormattedTime());
		fwrite(LogFile, "  ");
		fwrite(LogFile, string);
		fwrite(LogFile, "\r\n");

		fclose(LogFile);
	}
}

public WriteChatLog(string[])
{
	if (LoggingChatToggle > 0)
	{
		new filestring[256];
	    format(filestring,sizeof(filestring),"%s%s",ChatLogFilePath,MakeFilename());

		if (fexist(filestring))
		{
	        LogFile = fopen(filestring, io_append);
		}
		else
		{
	        LogFile = fopen(filestring, io_write);
		}

		fwrite(LogFile, MakeFormattedTime());
		fwrite(LogFile, "  ");
		fwrite(LogFile, string);
		fwrite(LogFile, "\r\n");

		fclose(LogFile);
	}
}

public WriteCMDLog(string[])
{
	if (LoggingCMDToggle > 0)
	{
	    new filestring[256];
		format(filestring,sizeof(filestring),"%s%s",CMDLogFilePath,MakeFilename());

		if (fexist(filestring))
		{
	        LogFile = fopen(filestring, io_append);
		}
		else
		{
	        LogFile = fopen(filestring, io_write);
		}

		fwrite(LogFile, MakeFormattedTime());
		fwrite(LogFile, "  ");
		fwrite(LogFile, string);
		fwrite(LogFile, "\r\n");

		fclose(LogFile);
	}
}

public Debug(string[])
{
    if (LoggingDebug > 0)
    {
        WriteLog(string);
    }
}
