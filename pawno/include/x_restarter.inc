/*----------------------------------------------------------------------------*/																			  	/*
	Include: xoma_restart
	Purpose: function to restart mode by timer
	Author: xomka
	                                                                          																					*/
/*----------------------------------------------------------------------------*/
#include <a_samp>
#if defined _xoma_restart_included
	#endinput
#endif
#define _xoma_restart_included

#pragma unused SetModeRestartTime
#pragma unused CancelModeRestart
/*----------------------------------------------------------------------------*/
/*
native SetModeRestartTime(time);
native CancelModeRestart();
*/

forward GMRestart();
new g_ModeRestartTimer=(-1);
/*----------------------------------------------------------------------------*/
SetModeRestartTime(time)
{
	g_ModeRestartTimer=SetTimer("GMRestart", time, 0);
	return 1;
}
/*----------------------------------------------------------------------------*/
CancelModeRestart()
{
	KillTimer(g_ModeRestartTimer);
	return 1;
}
/*----------------------------------------------------------------------------*/
public GMRestart()
{
	GameModeExit();
	return 1;
}
/*---------------------------------- EOF -------------------------------------*/

