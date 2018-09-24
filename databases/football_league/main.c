#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

#define MAX_TEAMS_COUNT 100
#define MAX_PLAYERS_IN_TEAM 20

int last_player_id = -1;
int last_team_id = -1;

char *teams_filename = "teams.bin";
char *players_filename = "players.bin";
char *teams_index_filename = "teams_index.bin";
char *players_index_filename = "players_index.bin";

struct Player
{
    int id;
    char name[MAX_NAME_LENGTH];
    int number;
    int team_id;
    int deleted;
};

struct PlayerIndex
{
    int player_id;
    long int file_position;
};

struct PlayerIndex PlayerIndex_new(int player_id, long int file_position)
{
    struct PlayerIndex playerIndex;
    playerIndex.player_id = player_id;
    playerIndex.file_position = file_position;
    return playerIndex;
}

struct PlayersArray
{
    struct Player data[MAX_PLAYERS_IN_TEAM];
    int size;
};

struct Player create_player(char *name, int number, int team_id)
{
    struct Player player;

    strcpy(player.name, name);
    player.number = number;
    player.team_id = team_id;
    player.deleted = 0;

    if (last_player_id == -1)
    {
        FILE *players_file = fopen(players_filename, "rb");
        fseek(players_file, 0, SEEK_END);
        int size = ftell(players_file);
        last_player_id = size / sizeof(struct Player);
        fclose(players_file);
    }

    player.id = last_player_id;
    last_player_id += 1;

    FILE *players_file = fopen(players_filename, "ab");
    if (players_file == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    FILE *players_index_file = fopen(players_index_filename, "ab");
    struct PlayerIndex playerIndex = PlayerIndex_new(player.id, ftell(players_file));
    fwrite(&playerIndex, sizeof(struct PlayerIndex), 1, players_index_file);
    fclose(players_index_file);

    fwrite(&player, sizeof(struct Player), 1, players_file);
    fclose(players_file);

    return player;
}

struct Team
{
    int id;
    char name[MAX_NAME_LENGTH];
    int deleted;
};

struct TeamsArray
{
    struct Team data[MAX_TEAMS_COUNT];
    int size;
};

struct TeamIndex
{
    int team_id;
    int file_position;
};

struct TeamIndex TeamIndex_new(int team_id, int file_position)
{
    struct TeamIndex teamIndex;
    teamIndex.team_id = team_id;
    teamIndex.file_position = file_position;
    return teamIndex;
};

struct Team create_team(char *name)
{
    struct Team team;

    strcpy(team.name, name);

    if (last_team_id == -1)
    {
        FILE *teams_file = fopen(teams_filename, "rb");
        fseek(teams_file, 0, SEEK_END);
        int file_size = ftell(teams_file);
        last_team_id = file_size / sizeof(struct Team);
        fclose(teams_file);
    }

    team.id = last_team_id;
    team.deleted = 0;
    last_team_id += 1;

    FILE *teams_file = fopen(teams_filename, "ab");
    if (teams_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", teams_filename);
        exit(1);
    }

    /*FILE *teams_index_file = fopen(teams_index_filename, "ab");
    struct TeamIndex teamIndex = TeamIndex_new(team.id, ftell(teams_file));
    fwrite(&teamIndex, sizeof(struct TeamIndex), 1, teams_index_file);
    fclose(teams_index_file);*/

    fwrite(&team, 1, sizeof(struct Team), teams_file);
    fclose(teams_file);

    return team;
}

void delete_player_by_id(int player_id);
void delete_team_by_id(int team_id);

struct Team get_team_by_id(int team_id)
{
    FILE *teams_file = fopen(teams_filename, "rb");
    if (teams_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", teams_filename);
        exit(1);
    }

    struct Team team;

    struct Team found_team;
    found_team.id = -1;
    strcpy(found_team.name, "");

    while (fread(&team, sizeof(struct Team), 1, teams_file))
    {
        if (!team.deleted && team.id == team_id)
        {
            found_team = team;
            break;
        }
    }
    fclose(teams_file);

    return found_team;
}

struct PlayersArray get_players_by_team_id(int team_id)
{
    struct PlayersArray players;
    players.size = 0;

    FILE *players_file = fopen(players_filename, "rb");
    if (players_file == NULL)
    {
        printf("\nError opening %s\n", players_filename);
        exit(1);
    }

    struct Player player;
    while (fread(&player, sizeof(struct Player), 1, players_file))
    {
        if (player.team_id == team_id && !player.deleted)
        {
            players.data[players.size] = player;
            players.size += 1;
        }
    }

    return players;
}

void print_player(struct Player player, int show_team)
{
    char *team_name = get_team_by_id(player.team_id).name;
    printf("Player %d) %s, %d %s\n", player.id, player.name, player.number, show_team ? team_name : "");
}

void print_team(struct Team team, int show_players)
{
    printf("Team %d) %s\n", team.id, team.name);
    struct PlayersArray players = get_players_by_team_id(team.id);

    if (show_players)
    {
        int players_size = players.size;
        for (int i = 0; i < players.size; ++i)
            if (!players.data[i].deleted)
                print_player(players.data[i], 0);
    }
}

struct Player get_player_by_id(int player_id)
{
    FILE *players_file = fopen(players_filename, "rb");
    if (players_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", players_filename);
        exit(1);
    }

    struct Player player;

    struct Player found_player;
    found_player.id = -1;
    strcpy(found_player.name, "");

    while (fread(&player, sizeof(struct Player), 1, players_file))
    {
        if (!player.deleted && player.id == player_id)
        {
            found_player = player;
            break;
        }
    }
    fclose(players_file);

    return found_player;
}

struct TeamsArray load_teams_from_file()
{
    FILE *teams_file = fopen(teams_filename, "rb");
    if (teams_file == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    struct TeamsArray teams;
    teams.size = 0;
    struct Team team;

    while (fread(&team, sizeof(struct Team), 1, teams_file))
    {
        if (!team.deleted)
        {
            teams.data[teams.size] = team;
            teams.size += 1;
        }
    }
    fclose(teams_file);

    return teams;
}

struct PlayersArray load_players_from_file()
{
    FILE *players_file = fopen(players_filename, "rb");
    if (players_file == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    struct PlayersArray players;
    players.size = 0;
    struct Player player;

    while (fread(&player, sizeof(struct Player), 1, players_file))
    {
        if (!player.deleted)
        {
            players.data[players.size] = player;
            players.size += 1;
        }
    }
    fclose(players_file);

    return players;
}

void update_player_by_id(int player_id, char *new_name, int new_number)
{
    FILE *players_file = fopen(players_filename, "r+b");
    if (players_file == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    struct Player player;
    while (fread(&player, sizeof(struct Player), 1, players_file))
    {
        if (player.id == player_id)
        {
            long int cur_pos = ftell(players_file);
            fseek(players_file, -sizeof(struct Player), SEEK_CUR);
            if (strcmp(new_name, "") != 0)
                strcpy(player.name, new_name);
            if (new_number != -1)
                player.number = new_number;
            fwrite(&player, sizeof(struct Player), 1, players_file);
            break;
        }
    }
    fclose(players_file);
}

void update_team_by_id(int team_id, char *new_name)
{
    FILE *teams_file = fopen(teams_filename, "r+b");
    if (teams_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", teams_filename);
        exit(1);
    }

    struct Team team;
    while (fread(&team, sizeof(struct Team), 1, teams_file))
        if (team.id == team_id)
        {
            long int cur_pos = ftell(teams_file);
            fseek(teams_file, -sizeof(struct Team), SEEK_CUR);
            if (strcmp(new_name, "") != 0)
                strcpy(team.name, new_name);
            printf("%s, new name %s\n", new_name, team.name);
            fwrite(&team, sizeof(struct Team), 1, teams_file);
            break;
        }
    fclose(teams_file);
}

int get_team_count()
{
    FILE *teams_file = fopen(teams_filename, "rb");
    if (teams_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", teams_filename);
        exit(1);
    }

    struct Team team;
    int count = 0;

    while (fread(&team, sizeof(struct Team), 1, teams_file))
    {
        if (!team.deleted)
            count += 1;
    }

    fclose(teams_file);

    return count;
}

int get_players_count()
{
    FILE *players_file = fopen(players_filename, "rb");
    if (players_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", players_filename);
        exit(1);
    }

    struct Player player;
    int count = 0;

    while (fread(&player, sizeof(struct Player), 1, players_file))
    {
        if (!player.deleted)
            count += 1;
    }
    fclose(players_file);

    return count;
}

void rewrite_all_files()
{
    FILE *f;
    f = fopen(players_filename, "wb");
    fclose(f);
    f = fopen(teams_filename, "wb");
    fclose(f);
    f = fopen(players_index_filename, "wb");
    fclose(f);
    f = fopen(teams_index_filename, "wb");
    fclose(f);
}

void show_all_teams()
{
    FILE *teams_file = fopen(teams_filename, "rb");
    if (teams_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", players_filename);
        exit(1);
    }

    struct Team team;
    while (fread(&team, sizeof(struct Team), 1, teams_file))
    {
        if (!team.deleted)
            print_team(team, 1);
    }

    fclose(teams_file);
}

void delete_player_by_id(int player_id)
{
    FILE *players_file = fopen(players_filename, "r+b");
    if (players_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", players_filename);
        exit(1);
    }

    struct Player player;
    while (fread(&player, sizeof(struct Player), 1, players_file))
    {
        if (player.id == player_id)
        {
            player.deleted = 1;

            fseek(players_file, -sizeof(struct Player), SEEK_CUR);
            fwrite(&player, sizeof(struct Player), 1, players_file);
        }
    }
    fclose(players_file);
}

void show_all_players()
{
    FILE *players_file = fopen(players_filename, "rb");
    if (players_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", players_filename);
        exit(1);
    }

    struct Player player;
    while (fread(&player, sizeof(struct Player), 1, players_file))
    {
        if (!player.deleted)
        {
            struct Team team = get_team_by_id(player.team_id);
            if (team.id == -1)
            {
                delete_player_by_id(player.id);
                continue;
            }
            printf("Player %d) %s %d %s \n", player.id, player.name, player.number, team.name);
        }
    }

    fclose(players_file);
}

void create_some_players_and_teams()
{
    struct Team barcelona = create_team("Barcelona");
    struct Team real = create_team("Real");
    struct Team juventus = create_team("Juventus");

    struct Player ramos = create_player("Sergio Ramos", 4, real.id);
    struct Player asensio = create_player("Asensio", 22, real.id);
    struct Player messi = create_player("Lionel Messi", 10, barcelona.id);
    struct Player ronaldo = create_player("Cristiano Ronaldo", 7, juventus.id);
    struct Player isco = create_player("Isco", 9, real.id);
    struct Player ter_stegen = create_player("Mark ter Stegen", 1, barcelona.id);
    struct Player dybala = create_player("Paolo Dybala", 10, juventus.id);
    struct Player busquets = create_player("Sergio Busquets", 4, barcelona.id);
}

void show_players_count_in_teams()
{
    FILE *teams_file = fopen(teams_filename, "rb");
    if (teams_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", players_filename);
        exit(1);
    }

    struct Team team;
    while (fread(&team, sizeof(struct Team), 1, teams_file))
    {
        if (!team.deleted)
        {
            struct PlayersArray players = get_players_by_team_id(team.id);
            printf("Team %d) %s has %d players\n", team.id, team.name, players.size);
        }
    }

    fclose(teams_file);
}

void delete_team_by_id(int team_id)
{
    FILE *teams_file = fopen(teams_filename, "r+b");
    if (teams_file == NULL)
    {
        fprintf(stderr, "\nError opening %s\n", teams_filename);
        exit(1);
    }

    struct Team team;
    while (fread(&team, sizeof(struct Team), 1, teams_file))
    {
        if (team.id == team_id)
        {
            team.deleted = 1;

            struct PlayersArray players = get_players_by_team_id(team.id);
            for (int i = 0; i < players.size; ++i)
                delete_player_by_id(players.data[i].id);

            fseek(teams_file, -sizeof(struct Team), SEEK_CUR);
            fwrite(&team, sizeof(struct Team), 1, teams_file);
        }
    }
    fclose(teams_file);
}

void optimize()
{
    struct PlayersArray players = load_players_from_file();
    struct TeamsArray teams = load_teams_from_file();

    rewrite_all_files();
    last_player_id = 0;
    last_team_id = 0;

    int old_to_new_team_id[MAX_TEAMS_COUNT][2];
    for (int i = 0; i < teams.size; ++i)
    {
        old_to_new_team_id[i][0] = teams.data[i].id;
        struct Team new_team = create_team(teams.data[i].name);
        old_to_new_team_id[i][1] = new_team.id;
    }

    for (int i = 0; i < players.size; ++i)
    {
        int old_team_id = players.data[i].team_id;
        int new_team_id = 0;
        for (int j = 0; j < teams.size; ++j)
        {
            if (old_to_new_team_id[j][0] == old_team_id)
            {
                new_team_id = old_to_new_team_id[j][1];
                break;
            }
        }

        create_player(players.data[i].name, players.data[i].number, new_team_id);
    }
}

int main()
{
    while (1)
    {
        printf("0 - get team by id\n");
        printf("1 - get player by id\n");
        printf("2 - delete team by id\n");
        printf("3 - delete player by id\n");
        printf("4 - update team by id\n");
        printf("5 - update player by id\n");
        printf("6 - create team\n");
        printf("7 - create player\n");
        printf("8 - show all teams\n");
        printf("9 - show all players\n");
        printf("10 - count all\n");
        printf("11 - optimize\n");
        printf("12 - exit\n");
        printf("13 - delete all\n");
        printf("14 - create some teams and players\n");

        int key;
        scanf("%d", &key);

        switch (key)
        {
        case 0:
        {
            int id;
            printf("Enter team id:");
            scanf("%d", &id);
            struct Team team = get_team_by_id(id);
            if (team.id == -1)
                printf("Team with such id is not present\n");
            else
                print_team(team, 1);
            break;
        }
        case 1:
        {
            int id;
            printf("Enter player id:");
            scanf("%d", &id);
            struct Player player = get_player_by_id(id);
            if (player.id == -1)
                printf("Player with this id is not present\n");
            else
                print_player(player, 1);
            break;
        }
        case 2:
        {
            int id;
            printf("Enter team id:");
            scanf("%d", &id);
            delete_team_by_id(id);
            break;
        }
        case 3:
        {
            int id;
            printf("Enter player id:");
            scanf("%d", &id);
            delete_player_by_id(id);
            break;
        }
        case 4:
        {
            int id;
            printf("Enter team id:");
            scanf("%d", &id);
            char new_name[MAX_NAME_LENGTH];
            printf("Enter new team name:");
            scanf("%s", new_name);
            printf("new-name=%s**\n", new_name);
            update_team_by_id(id, new_name);
            break;
        }
        case 5:
        {
            int id;
            printf("Enter player id:");
            scanf("%d", &id);
            char new_name[MAX_NAME_LENGTH];
            printf("Enter new player name:");
            scanf("%s", new_name);
            int new_number;
            printf("Enter new player number:");
            scanf("%d", &new_number);
            update_player_by_id(id, new_name, new_number);
            break;
        }
        case 6:
        {
            char name[MAX_NAME_LENGTH];
            printf("Enter team name:");
            scanf("%s", name);
            create_team(name);
            break;
        }
        case 7:
        {
            char name[MAX_NAME_LENGTH];
            printf("Enter player name:");
            scanf("%s", name);
            int number;
            printf("Enter player number:");
            scanf("%d", &number);
            int team_id;
            printf("Enter team id:");
            scanf("%d", &team_id);
            create_player(name, number, team_id);
            break;
        }
        case 8:
        {

            show_all_teams();
            break;
        }
        case 9:
        {
            show_all_players();
            break;
        }
        case 10:
        {
            printf("Count of teams is: %d\n", get_team_count());
            printf("Count of players is %d\n", get_players_count());
            show_players_count_in_teams();
            break;
        }
        case 11:
        {
            optimize();
            break;
        }
        case 12:
        {
            exit(0);
            break;
        }
        case 13:
        {
            rewrite_all_files();
            break;
        }
        case 14:
        {
            create_some_players_and_teams();
            break;
        }
        default:
        {
            return 0;
            break;
        }
        }

        printf("\n\n\n");
    }

    return 0;
}
