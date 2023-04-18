//
// Created:     06.09.06
// Aurthor:    Iain Gilbert
//

#if defined _payday_included
  #endinput
#endif

#define _payday_included
#pragma library payday
#include "base"
#include "player"

forward PayDay();

#define PayDayDelay 59999 // Delay between pays
new admpayday,rpayday;

static BasePay = 500; // Base amount player recieves each pay (pay = PocketMoney + (PocketMoney * level ))
static PayCutoff = MAX_MONEY/2; // amount of money player can horde before they stop getting payed (they can still get money by killing)
static BasePayXP = 50;

// timer needs to run this.
// gives players money.
// ПайДай - раз в час, причем ХП тоже дается. Рассчитать верное кол-во и процент...
public PayDay()
{
new hour,minute,second;
gettime(hour,minute,second);
if (minute>0) {rpayday=0;}
if ( ((minute==0) && (rpayday==0)) || (admpayday>0) )
{
	if (admpayday==0) {rpayday++;}
	new BasePayM,BasePayX,string[MAX_STRING];
	admpayday=0;
	for (new id = 0; id < MAX_PLAYERS; id++)
	{
		 if (IsPlayerConnected(id))
		 {
			BasePayM=0;
			if (GetPlayerTotalMoney(id) < PayCutoff)	//MONEY
			{
			BasePayM=BasePay+(BasePay*Player[id][Level]);
			oGivePlayerMoney(id, BasePayM, 0);
			}

			BasePayX=0;
			if (GetPlayerXP(id) < MAX_LVLXP)		//XP
			{
			BasePayX=BasePayXP+(BasePayXP*Player[id][Level]); //200+(200*0) 200+(200*20)
			GivePlayerXP(id, BasePayX ,0);
			CheckPlayerLevel(id);
			}

			format(string, sizeof(string), lang_texts[2][12], BasePayM, BasePayX );
			SendClientMessage(id, COLOUR_MONEY_GOOD, string);
			format(string, sizeof(string), lang_texts[2][13], BasePayM, BasePayX );
			GameTextForPlayer(id, string ,999, 5);
			PlayerPlaySound(id, 1190, 0.0, 0.0, 0.0);
			PlayerPlaySound(id, 1101, 0.0, 0.0, 0.0);
		}
	}
}
}

PaydayLoadConfig()
{
	if (ForceOverwrite) return;
	if (!dini_Exists(ConfigDB)) return;
	new temp[MAX_STRING];
	set(temp,dini_Get(ConfigDB,"Payday_Base"));
	if (strlen(temp) > 0) BasePay = strval(temp);
	set(temp,nullstr);
	set(temp,dini_Get(ConfigDB,"Payday_Cutoff"));
	if (strlen(temp) > 0) PayCutoff = strval(temp);

	return;
}

PaydaySaveConfig()
{
	if (!dini_Exists(ConfigDB)) return;
	new temp[MAX_STRING];
	valstr(temp,BasePay);
	dini_Set(ConfigDB,"Payday_Base",temp);
	valstr(temp,PayCutoff);
	dini_Set(ConfigDB,"Payday_Cutoff",temp);
	return;
}
