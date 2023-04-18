#include <a_samp>

forward TimeRun();
forward Weather();
forward CreateTimeInd();
forward BalanceTime(hh,mm);
forward HideTime(playerid);
forward ShowTime(playerid);

new Text:textTime;

new hour=7;
new minet=0;
new TimeWeather[] =
{
	600000,
	900000,
	1200000
};

public TimeRun()
{
    minet++;
	if(minet==60)
	{
	    hour++;
	    minet=0;
	}
	if(hour==24)
		hour=1;
	SetWorldTime(hour);
	BalanceTime(hour,minet);
	SetTimer("TimeRun",20000,0);
}

public Weather()
{
	new i;
	i=random(sizeof(TimeWeather));
	SetTimer("Weather",TimeWeather[i],0);
	while((i==43||i==23)&&(hour==21||hour==22||hour==23))
		i=random(45);
	SetWeather(i);
}

public CreateTimeInd()
{
    textTime=TextDrawCreate(550.0,16.0,"HH:MM");
}

public BalanceTime(hh,mm)
{
	new time[6];
	format(time,sizeof(time),"%d:%d",hh,mm);
	TextDrawHideForAll(textTime);
	TextDrawSetString(textTime,time);
	TextDrawColor(textTime,0xFF9900FF);
	TextDrawFont(textTime,2);
	TextDrawShowForAll(textTime);
	for(new i=0;i<MAX_PLAYERS;i++)
	{
	    if(!IsPlayerConnected(i))
	    {
	        TextDrawHideForPlayer(i,textTime);
	    }
	}
}

public HideTime(playerid)
{
	TextDrawHideForPlayer(playerid,textTime);
}

public ShowTime(playerid)
{
    TextDrawShowForPlayer(playerid,textTime);
}
