#include <a_samp>
#include <a_mysql>
#include <sscanf2>
#include <foreach>

#undef MAX_PLAYERS

#define MYSQL_HOST "localhost"
#define MYSQL_USER "root"
#define MYSQL_PASSWORD ""
#define MYSQL_DATABASE "shakal"

#define MAX_PLAYERS 50
#define MAX_PASSWORD_LENGTH 65
#define SALT_LENGTH 17

new MySQL: dbHandle;

enum player_info
{
    id,
    name[MAX_PLAYER_NAME],
    password[MAX_PASSWORD_LENGTH],
    salt[SALT_LENGTH],
    money,
    bool: sex,
    job,
    skin,
    experience,

    Float: health,
    Float: armor,

	Float: x_pos,
	Float: y_pos,
	Float: z_pos,
	Float: a_pos,
    interior,

    bool: isLogged,
    Cache:cache_id,
}
new Player[MAX_PLAYERS][player_info];
new dbHandleRaceCheck[MAX_PLAYERS];

enum
{
    CHAT_RP,
    CHAT_NONRP,
    CHAT_INFO,
    CHAT_ERROR,
}

new hours, minutes, seconds;

main() {}

public OnPlayerConnect(playerid)
{
    dbHandleRaceCheck[playerid]++;
    static const empty_player[player_info];
    Player[playerid] = empty_player;

    GetPlayerName(playerid, Player[playerid][name], MAX_PLAYER_NAME);

    new query[63];
    mysql_format(dbHandle, query, sizeof query, "SELECT * FROM `players` WHERE `name` = '%e' LIMIT 1", Player[playerid][name]);
    mysql_tquery(dbHandle, query, "OnPlayerDataLoaded", "dd", playerid, dbHandleRaceCheck[playerid]);
    
    return 1;
}

forward OnPlayerDataLoaded(playerid, race_check);
public OnPlayerDataLoaded(playerid, race_check)
{
    if (race_check != dbHandleRaceCheck[playerid]) return Kick(playerid);

    if (cache_num_rows() > 0)
    {
        cache_get_value_name(0, "name", Player[playerid][name], MAX_PLAYER_NAME);
        cache_get_value_name(0, "password", Player[playerid][password], MAX_PASSWORD_LENGTH);
        Player[playerid][cache_id] = cache_save();

        //new string[];
        //format(string, sizeof string, "");
        //SendClientMessage(playerid, color, const message[]);
        //ShowPlayerDialogLogin(playerid);
        //SendMessagePlayer(playerid, CHAT_INFO, "qwerty");
        gettime(hours, minutes, seconds);
        new string[1];
        format(string, strlen(string), "[%d:%d:%d] [INFO]: Ваш аккаунт найден в базе данных!", hours, minutes, seconds);
        SendClientMessage(playerid, -1, string);
        SendMessagePlayer(playerid, CHAT_INFO, "Ваш аккаунт найден в базе данных!", 1);
    }

    else
    {
        //new string[];
        //format(string, sizeof string, "");
        //SendClientMessage(playerid, color, const message[]);
        //ShowPlayerDialogRegister(playerid);
    }

    return 1;
}

stock SendMessagePlayer(playerid, type, const string[])
{
    gettime(hours, minutes, seconds);
    switch(type)
    {
        case CHAT_RP:
        {
            new str[1];
            format(str, sizeof str, "[%d:%d:%d] %s(%d): %s", hours, minutes, seconds, Player[playerid][name], playerid, str);
        }

        case CHAT_NONRP:
        {
            new str[1];
            format(str, sizeof str, "[%d:%d:%d] (( %s(%d): %s ))", hours, minutes, seconds, Player[playerid][name], playerid, str);
        }

        case CHAT_ERROR:
        {
            new str[1];
            format(str, sizeof str, "[%d:%d:%d] [ERROR]: %s", hours, minutes, seconds, str);
        }
        
        case CHAT_INFO:
        {
            new str[1];
            format(str, sizeof str, "[%d:%d:%d] [INFO]: %s", hours, minutes, seconds, str);
        }
    }
    SendClientMessage(playerid, -1, str);
}

// ==================================== GAMEMODE INIT/EXIT ============================================ //
public OnGameModeInit()
{
    new MySQLOpt: option_id = mysql_init_options();
    mysql_set_option(option_id, AUTO_RECONNECT, true);
    
    dbHandle = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE, option_id);

    if (mysql_errno() != 0)
    {
        print("MySQL connaction error!");
    }

    print("MySQL connaction succesful!");
    mysql_log(ALL);
    return 1;
}

public OnGameModeExit()
{
    mysql_close(dbHandle);
}
// ==================================================================================================== //