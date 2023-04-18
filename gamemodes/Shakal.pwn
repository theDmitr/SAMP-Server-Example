#include <a_samp>
#include <a_mysql>
#include <core>

#define MYSQL_HOST "localhost"
#define MYSQL_LOGIN "root"
#define MYSQL_PASSWORD ""
#define MYSQL_DATABASE "shakal"

#define COLOR_MAGENTA 0xFF00FFAA
#define COLOR_PURPLE 0x8A30FFAA
#define COLOR_RED 0xb80300AA

#define MAX_PASSWORD_LENGTH 65
#define SALT_LENGTH 17

#define DEFAULT_X_POS 1958.3783
#define DEFAULT_Y_POS 1343.1572
#define DEFAULT_Z_POS 15.3746
#define DEFAULT_A_POS 270.1425

new MySQL:dbHandle;

enum player_info
{
	id,
	name[MAX_PLAYER_NAME],
	password[MAX_PASSWORD_LENGTH],
	salt[SALT_LENGTH],
	kills,
	deaths,
	Float: x_pos,
	Float: y_pos,
	Float: z_pos,
	Float: a_pos,
	interior,

	Cache: cache_data
};
new Player[MAX_PLAYERS][player_info];
new g_MysqlRaceCheck[MAX_PLAYERS];

enum
{
	DLG_LOGIN,
	DLG_REGISTER,
	DIALOG_UNUSED
};

main() {}

public OnGameModeInit()
{
	new MySQLOpt:option_id = mysql_init_options();
	mysql_set_option(option_id, AUTO_RECONNECT, true);
	dbHandle = mysql_connect(MYSQL_HOST, MYSQL_LOGIN, MYSQL_PASSWORD, MYSQL_DATABASE, MySQLOpt:option_id = MySQLOpt:0);
	if (mysql_errno() == 0) print("MySQL connected!");
	else print("MySQL error!");
	mysql_log(ALL);
	return 1;
}

public OnPlayerConnect(playerid)
{
	g_MysqlRaceCheck[playerid]++;
	static const empty_player[player_info];
	Player[playerid] = empty_player;
	new string[] = "Добро пожаловать на Shakal server, %s!";
	GetPlayerName(playerid, Player[playerid][name], MAX_PLAYER_NAME);
	format(string, strlen(string) - 2 + MAX_PLAYER_NAME, string, Player[playerid][name]);
	SendClientMessage(playerid, COLOR_MAGENTA, string);
	new query[103];
	mysql_format(dbHandle, query, sizeof query, "SELECT * FROM `users` WHERE `name` = '%e'", Player[playerid][name]);
	mysql_tquery(dbHandle, query, "OnPlayerDataLoaded", "dd", playerid, g_MysqlRaceCheck[playerid]);
	return 1;
}

forward OnPlayerDataLoaded(playerid, race_check);
public OnPlayerDataLoaded(playerid, race_check)
{
	if (race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);
	
	printf("Row: %s; Num: %d", Player[playerid][name], cache_num_rows());
	
	if (cache_num_rows() > 0)
	{
		cache_get_value_name(0, "password", Player[playerid][password], MAX_PASSWORD_LENGTH);
		cache_get_value_name(0, "salt", Player[playerid][salt], SALT_LENGTH);
		Player[playerid][cache_data] = cache_save();

		new string[] = "Привет, %s, ты зарегистрирован на Shakal server!";
		format(string, sizeof(string) - 2 + MAX_PLAYER_NAME, string, Player[playerid][name]);
		SendClientMessage(playerid, COLOR_PURPLE, string);
		ShowPlayerDialogLogin(playerid);
	}
	else 
	{
		new string[] = "Привет, %s, ты первый раз на Shakal server!";
		format(string, sizeof(string) - 2 + MAX_PLAYER_NAME, string, Player[playerid][name]);
		SendClientMessage(playerid, COLOR_PURPLE, string);
		ShowPlayerDialogRegister(playerid);
	}
	return 1;
}

forward OnPlayerRegister(playerid);
public OnPlayerRegister(playerid)
{
	Player[playerid][id] = cache_insert_id();
	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Registration", "Account successfully registered, you have been automatically logged in.", "Okay", "");
	Player[playerid][x_pos] = DEFAULT_X_POS;
	Player[playerid][y_pos] = DEFAULT_Y_POS;
	Player[playerid][z_pos] = DEFAULT_Z_POS;
	Player[playerid][a_pos] = DEFAULT_A_POS;
	SetSpawnInfo(playerid, NO_TEAM, 105, Player[playerid][x_pos], Player[playerid][y_pos], Player[playerid][z_pos], Player[playerid][a_pos], 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
        switch(dialogid)
		{
			case DIALOG_UNUSED: return 1;
			case DLG_LOGIN:
			{
				if (!response) return Kick(playerid);
				new hashed_password[MAX_PASSWORD_LENGTH];
				SHA256_PassHash(inputtext, Player[playerid][salt], hashed_password, MAX_PASSWORD_LENGTH);
				if (strcmp(hashed_password, Player[playerid][password]) == 0)
				{
					cache_set_active(Player[playerid][cache_data]);
					AssignPlayerData(playerid);
					cache_delete(Player[playerid][cache_data]);
					Player[playerid][cache_data] = MYSQL_INVALID_CACHE;
					SetSpawnInfo(playerid, NO_TEAM, 105, Player[playerid][x_pos], Player[playerid][y_pos], Player[playerid][z_pos], Player[playerid][a_pos], 0, 0, 0, 0, 0, 0);
					SpawnPlayer(playerid);
				}
				else
				{
					SendClientMessage(playerid, COLOR_RED, "Вы неверно ввели пароль!");
					ShowPlayerDialogLogin(playerid);
				}
			}
			case DLG_REGISTER:
			{
				if (!response) Kick(playerid);
				for (new i = 0; i < SALT_LENGTH - 1; i++)  Player[playerid][salt][i] = random(94) + 33; 
				SHA256_PassHash(inputtext, Player[playerid][salt], Player[playerid][password], MAX_PASSWORD_LENGTH);

				//new query[] = "INSERT INTO `users` (`name`, `password`, `salt`) VALUES ('%e', '%s', '%e')";
				//mysql_format(dbHandle, query, sizeof(query) - 6 + MAX_PLAYER_NAME + MAX_PASSWORD_LENGTH + SALT_LENGTH, query, Player[playerid][name], Player[playerid][password], Player[playerid][salt]);
				
				new query[221];
				mysql_format(dbHandle, query, sizeof query, "INSERT INTO `users` (`name`, `password`, `salt`) VALUES ('%e', '%s', '%e')", Player[playerid][name], Player[playerid][password], Player[playerid][salt]);
				mysql_tquery(dbHandle, query, "OnPlayerRegister", "d", playerid);
			}
			default: return 0;
		}
        return 1;
}

stock ShowPlayerDialogLogin(playerid)
{
	ShowPlayerDialog(playerid, DLG_LOGIN, DIALOG_STYLE_PASSWORD, "Авторизация", "Для того, чтобы войти на сервер введите пароль в поле ниже.", "Вход", "");
	return 1;
}

stock ShowPlayerDialogRegister(playerid)
{
	ShowPlayerDialog(playerid, DLG_REGISTER, DIALOG_STYLE_INPUT, "Регистрация", "Для регистрации придумайте и введите пароль в поле ниже.", "Готово", "");
	return 1;
}

stock AssignPlayerData(playerid)
{
	cache_get_value_int(0, "id", Player[playerid][id]);
	cache_get_value_int(0, "kills", Player[playerid][kills]);
	cache_get_value_int(0, "deaths", Player[playerid][deaths]);
	cache_get_value_name_float(0, "x_pos", Player[playerid][x_pos]);
	cache_get_value_name_float(0, "y_pos", Player[playerid][y_pos]);
	cache_get_value_name_float(0, "z_pos", Player[playerid][z_pos]);
	cache_get_value_name_float(0, "a_pos", Player[playerid][a_pos]);
	cache_get_value_int(0, "interior", Player[playerid][interior]);
	return 1;
}

public OnGameModeExit()
{
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	return 0;
}

public OnPlayerSpawn(playerid)
{
	SetPlayerInterior(playerid, Player[playerid][interior]);
	SetPlayerPos(playerid, Player[playerid][x_pos], Player[playerid][y_pos], Player[playerid][z_pos]);
	SetPlayerFacingAngle(playerid, Player[playerid][a_pos]);
	SetCameraBehindPlayer(playerid);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
   	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	return 1;
}