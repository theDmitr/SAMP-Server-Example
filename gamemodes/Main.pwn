#include <a_samp>
#include <a_mysql>
#include <sscanf2>
#include <foreach>
#include <shakal\mapping.pwn>

#define MYSQL_HOST "localhost"
#define MYSQL_USER "root"
#define MYSQL_PASSWORD ""
#define MYSQL_DATABASE "shakal"

#undef MAX_PLAYERS
#define MAX_PLAYERS 50
#define MAX_PASSWORD_LENGTH 65
#define SALT_LENGTH 17

new MySQL:dbHandle;
new pickup1;

enum player_info
{
	id,
	name[MAX_PLAYER_NAME],
	password[MAX_PASSWORD_LENGTH],
	salt[SALT_LENGTH],
	Cache:cache_id,
	Float:x_pos,
	Float:y_pos,
	Float:z_pos,
	Float:a_pos,
	money,

	bool:isLogged,
}
new Player[MAX_PLAYERS][player_info];
new dbHandleRaceCheck[MAX_PLAYERS];

enum
{
	DLG_LOGIN,
	DLG_REGISTER,
	DLG_SUCCESFUL_REGISTER,
	DLG_GET_GUN,
}

main() {}

public OnPlayerSpawn(playerid)
{
	SetCameraBehindPlayer(playerid);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	if (pickupid == pickup1)
	{
		GiveMoney(playerid, 1000);
	}
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case DLG_LOGIN:
		{
			if (!response) return Kick(playerid);

			new hash_password[MAX_PASSWORD_LENGTH];
			SHA256_PassHash(inputtext, Player[playerid][salt], hash_password, MAX_PASSWORD_LENGTH);
			if (strcmp(hash_password, Player[playerid][password]) == 0)
			{
				cache_set_active(Player[playerid][cache_id]);
				AssignPlayerData(playerid);
				cache_delete(Player[playerid][cache_id]);
				Player[playerid][cache_id] = MYSQL_INVALID_CACHE;
				SetSpawnInfo(playerid, NO_TEAM, 105, Player[playerid][x_pos], Player[playerid][y_pos], Player[playerid][z_pos], Player[playerid][a_pos], 0, 0, 0, 0, 0, 0);
				GivePlayerMoney(playerid, Player[playerid][money]);
				SpawnPlayer(playerid);
				Player[playerid][isLogged] = true;
			}
			else
			{
				SendClientMessage(playerid, 0xFF0000AA, "{A60000}[ERROR] {FFFFFF}Некорректно введён пароль! Попробуйте ещё раз!");
				ShowPlayerDialogLogin(playerid);
			}
		}
		case DLG_REGISTER:
		{
			if (!response) return Kick(playerid);

			for (new i = 0; i < SALT_LENGTH - 1; i++)  Player[playerid][salt][i] = random(94) + 33; 
			SHA256_PassHash(inputtext, Player[playerid][salt], Player[playerid][password], MAX_PASSWORD_LENGTH);
			new query[163];
			mysql_format(dbHandle, query, sizeof query, "INSERT INTO `players` (`name`, `password`, `salt`) VALUES ('%e', '%e', '%e')", Player[playerid][name], Player[playerid][password], Player[playerid][salt]);
			mysql_tquery(dbHandle, query, "OnPlayerRegister", "d", playerid);
		}
		case DLG_SUCCESFUL_REGISTER:
		{
			return 1;
		}
		case DLG_GET_GUN:
		{
			if (response)
			{
				switch(listitem)
				{
					case 0: GivePlayerWeapon(playerid, 24, 1000);
					case 1: GivePlayerWeapon(playerid, 25, 1000);
					case 2: GivePlayerWeapon(playerid, 34, 1000);
					case 3: GivePlayerWeapon(playerid, 30, 1000);
				}
			}
			return 1;
		}
	}
	return 1;
}

stock ShowPlayerDialogLogin(playerid)
{
	ShowPlayerDialog(playerid, DLG_LOGIN, DIALOG_STYLE_PASSWORD, "{FFFFFF}• {6463BA}Авторизация", "{FFFFFF}Введите свой пароль в поле ниже", "Ввод", "Выйти");
}

stock ShowPlayerDialogRegister(playerid)
{
	ShowPlayerDialog(playerid, DLG_REGISTER, DIALOG_STYLE_INPUT, "{FFFFFF}• {6463BA}Регистрация", "{FFFFFF}Придумайте и введите пароль в поле ниже", "Ввод", "Выйти");
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    return 1;
}

forward GiveMoney(playerid, amount);
public GiveMoney(playerid, amount)
{
	GivePlayerMoney(playerid, amount);
	Player[playerid][money] += amount;
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	new cmd[129], params[127];
	sscanf(cmdtext, "s[129]s[127]", cmd, params);

	if (strcmp(cmd, "/gun") == 0)
	{
		ShowPlayerDialog(playerid, DLG_GET_GUN, DIALOG_STYLE_LIST, "{FFFFFF}• {6463BA}Выберите оружие", "Пустынный орёл\nДробовик\nСнайперская винтовка\nАК-47", "Выбрать", "");
	}

	else if (strcmp(cmd, "/me") == 0)
	{
		new string[152];
		format(string, sizeof string, "%s %s", Player[playerid][name], params);
		ProxDetector(50, playerid, string, 0xFF00FFAA);
	}

	else if (strcmp(cmd, "/do") == 0)
	{
		new string[152];
		format(string, sizeof string, "%s (%s)", params, Player[playerid][name]);
		ProxDetector(50, playerid, string, 0xFF00FFAA);
	}

	else if (strcmp(cmd, "/b") == 0)
	{
		new string[152];
		format(string, sizeof string, "{767380}(( %s[%d]: %s ))", Player[playerid][name], playerid, params);
		ProxDetector(50, playerid, string, 0xFF00FFAA);
	}

	else if (strcmp(cmd, "/try") == 0)
	{
		new string[152];
		format(string, sizeof string, "%s %s %s", Player[playerid][name], params, random(2) == 1 ? "{1ABD15}(Удача)" : "{BD0215}(Неудача)");
		ProxDetector(50, playerid, string, 0xFF00FFAA);
	}

	else if (strcmp(cmd, "/s") == 0)
	{
		new string[152];
		format(string, sizeof string, "{767380}%s[%d] крикнул: %s!", Player[playerid][name], playerid, params);
		ProxDetector(100, playerid, string, 0xFF00FFAA);
	}

	else if (strcmp(cmd, "/w") == 0)
	{
		new string[152];
		format(string, sizeof string, "{767380}%s[%d] прошептал: %s!", Player[playerid][name], playerid, params);
		ProxDetector(5, playerid, string, 0xFF00FFAA);
	}

	else if (strcmp(cmd, "/pay") == 0)
	{
		new plr_id, amount;
		sscanf(params, "dd", plr_id, amount);

		if (playerid == plr_id)
		{
			SendClientMessage(playerid, -1, "{A60000}[ERROR] {FFFFFF}Вы не можете передать деньги самому себе!");
		}
		
		else if (Player[playerid][money] < amount)
		{
			SendClientMessage(playerid, -1, "{A60000}[ERROR] {FFFFFF}У вас недостаточно денег!");
		}

		else if (Player[plr_id][isLogged])
		{
			GiveMoney(playerid, -amount);
			GiveMoney(plr_id, amount);
		}
		 
		else
		{
			SendClientMessage(playerid, -1, "{A60000}[ERROR] {FFFFFF}Такого игрока сейчас нет на сервере!");
		}
	}
	return 1;
}

public OnPlayerText(playerid, text[])
{
    new string[183];
    format(string, sizeof string, "{767380}%s[%d]: %s", Player[playerid][name], playerid, text);
	ProxDetector(50, playerid, string, 0xFFFF00AA);
    return 0;
}

stock ProxDetector(Float:radi, playerid, const str[], color)
{
        new Float:posx, Float:posy, Float:posz;
        new Float:oldposx, Float:oldposy, Float:oldposz;
        new Float:tempposx, Float:tempposy, Float:tempposz;
        GetPlayerPos(playerid, oldposx, oldposy, oldposz);
        foreach(Player, i)
        if(IsPlayerConnected(i))
        {
            if(!IsPlayerConnected(i)) continue;
            if(GetPlayerVirtualWorld(i) != GetPlayerVirtualWorld(playerid)) continue;
            GetPlayerPos(i, posx, posy, posz);
            tempposx = (oldposx -posx);
            tempposy = (oldposy -posy);
            tempposz = (oldposz -posz);
            if (((tempposx < radi) && (tempposx > -radi)) && ((tempposy < radi) && (tempposy > -radi)) && ((tempposz < radi) && (tempposz > -radi))) SendClientMessage(i, color, str);
        }
        return 1;
}

public OnPlayerConnect(playerid){
	dbHandleRaceCheck[playerid]++;
	ViewMapping();

	static const empty_player[player_info];
	Player[playerid] = empty_player;

	GetPlayerName(playerid, Player[playerid][name], MAX_PLAYER_NAME);

	new query[63];
	mysql_format(dbHandle, query, sizeof query, "SELECT * FROM `players` WHERE `name` = '%e' LIMIT 1", Player[playerid][name]);
	mysql_tquery(dbHandle, query, "OnPlayerLoadData", "dd", playerid, dbHandleRaceCheck[playerid]);

	return 1;
}

forward OnPlayerLoadData(playerid, race_check);
public OnPlayerLoadData(playerid, race_check){
	if (race_check != dbHandleRaceCheck[playerid]) return Kick(playerid);

	if (cache_num_rows() > 0)
	{
		cache_get_value_name(0, "password", Player[playerid][password], MAX_PASSWORD_LENGTH);
		cache_get_value_name(0, "salt", Player[playerid][salt], SALT_LENGTH);

		Player[playerid][cache_id] = cache_save();

		new string[85];
		format(string, sizeof string, "{6463BA}Привет, {FFFFFF}%s{6463BA}! Вы зарегистрированы на сервере!", Player[playerid][name]);
		SendClientMessage(playerid, -1, string);

		ShowPlayerDialogLogin(playerid);
	}

	else
	{
		new string[92];
		format(string, sizeof string, "{6463BA}Привет, {FFFFFF}%s{6463BA}! Вашего аккаунта не найдено в базе данных!", Player[playerid][name]);
		SendClientMessage(playerid, -1, string);
		ShowPlayerDialogRegister(playerid);
	}

	return 1;
}

forward OnPlayerRegister(playerid);
public OnPlayerRegister(playerid){
	Player[playerid][id] = cache_insert_id();
	Player[playerid][isLogged] = true;

	SetSpawnInfo(playerid, NO_TEAM, 105, 1958.3783, 1343.1572, 15.3746, 270.1425, 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);

	ShowPlayerDialog(playerid, DLG_SUCCESFUL_REGISTER, DIALOG_STYLE_MSGBOX, "{FFFFFF}• {6463BA}Info", "{FFFFFF}Ваш аккаунт успешно зарегистрирован!", "Закрыть", "");
}

AssignPlayerData(playerid)
{
	cache_get_value_int(0, "id", Player[playerid][id]);
	cache_get_value_int(0, "money", Player[playerid][money]);
	cache_get_value_float(0, "x_pos", Player[playerid][x_pos]);
	cache_get_value_float(0, "y_pos", Player[playerid][y_pos]);
	cache_get_value_float(0, "z_pos", Player[playerid][z_pos]);
	cache_get_value_float(0, "a_pos", Player[playerid][a_pos]);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	GetPlayerPos(playerid, Player[playerid][x_pos], Player[playerid][y_pos], Player[playerid][z_pos]);
	GetPlayerFacingAngle(playerid, Player[playerid][a_pos]);

	Player[playerid][isLogged] = false;

	new query[310];
	mysql_format(dbHandle, query, sizeof query, "UPDATE `players` SET `x_pos` = '%f', `y_pos` = '%f', `z_pos` = '%f', `a_pos` = '%f', `money` = '%d' LIMIT 1", Player[playerid][x_pos], Player[playerid][y_pos], Player[playerid][z_pos], Player[playerid][a_pos], Player[playerid][money], Player[playerid][id], Player[playerid][id]);
	mysql_tquery(dbHandle, query, "OnPlayerDisconnectUpdate", "d", playerid);
	return 1;
}

forward OnPlayerDisconnectUpdate(playerid);
public OnPlayerDisconnectUpdate(playerid)
{
	return 1;
}

public OnGameModeInit()
{
	SetGameModeText("Shakal by theDmitr");
	
	new MySQLOpt: option_id = mysql_init_options();
	mysql_set_option(option_id, AUTO_RECONNECT, true);
	dbHandle = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE, option_id);

	if (mysql_errno())
	{
		print("MySQL connection failed!");
		SendRconCommand("exit");
	}

	print("MySQL connected succesfull!");
	mysql_log(ALL);

	pickup1 = CreatePickup(1550, 23, 2047.95081, 1370.08447, 9.66010);
	
	return 1;
}

public OnGameModeExit()
{
	mysql_close(dbHandle);
	return 1;
}