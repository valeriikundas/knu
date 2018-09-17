#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_NAME_LENGTH 50
#define MAX_PLAYERS_SIZE 20

int current_player_id = 0;
int current_team_id = 0;

struct Player {
	int id;
	char name[MAX_NAME_LENGTH];
	int number;
	int team_id;
};

struct Player Player_new(char* name, int number) {
	struct Player player;
	strcpy_s(player.name, sizeof(player.name), name);
	player.number = number;
	player.id = current_player_id;
	current_player_id++;
	return player;
};

struct Team {
	int id;
	char name[MAX_NAME_LENGTH];
	struct Player players[MAX_PLAYERS_SIZE];
	int players_size;
};

struct Team Team_new(char* name, struct Player players[], int players_size) {
	struct Team team;
	strcpy_s(team.name, sizeof(team.name), name);
	for (int i = 0; i < players_size; ++i)
		team.players[i] = players[i];
	team.players_size = players_size;
	team.id = current_team_id;
	current_team_id++;
	return team;
};

void print_team(struct Team team) {
	printf("%s: ", team.name);
	printf("%s(%d)", team.players[0].name, team.players[0].number);
	int players_size = team.players_size;
	for (int i = 1; i < players_size; ++i)
		printf(", %s(%d)", team.players[i].name, team.players[i].number);
	printf("\n");
}

void save_teams_to_file(char* teams_filename, char* players_filename, struct Team teams[], int teams_size) {
	FILE *teams_file;
	FILE* players_file;

	fopen_s(&teams_file, teams_filename, "w");
	if (teams_file == NULL)
	{
		fprintf(stderr, "\nError opening file\n");
		exit(1);
	}
	fopen_s(&players_file, players_filename, "w");
	if (players_file == NULL) {
		fprintf(stderr, "\nError opening file\n");
		exit(1);
	}

	if (fwrite == 0)
		printf("error writing file !\n");

	for (int i = 0; i < teams_size; ++i) {
		fwrite(&teams[i], sizeof(struct Team), 1, teams_file);

		int players_size = teams[i].players_size;
		for (int j = 0; j < players_size; ++j)
			fwrite(&teams[i].players[j], sizeof(struct Player), 1, players_file);
	}

	fclose(teams_file);
	fclose(players_file);
}

void load_teams_from_file(char *teams_filename, char* players_filename, struct Team* teams) {
	FILE* teams_file;
	struct Team team;

	fopen_s(&teams_file, teams_filename, "r");
	if (teams_file == NULL)
	{
		fprintf(stderr, "\nError opening file\n");
		exit(1);
	}

	FILE* players_file;
	fopen_s(&players_file, players_filename, "r");
	if (players_file == NULL) {
		fprintf(stderr, "\nError opening file\n");
		exit(1);
	}

	while (fread(&team, sizeof(struct Team), 1, teams_file)) 
		printf("read id = %d name = %s %d\n", team.id, team.name, team.players_size);
	
	struct Player player;
	while (fread(&player, sizeof(struct Player), 1, players_file)) {
		printf("read player %d %s %d\n", player.id, player.name, player.number);
	}

	fclose(teams_file);
}

int main()
{
	struct Player messi = Player_new("Lionel Messi", 10);
	struct Player busquets = Player_new("Sergio Busquets", 5);
	struct Player stegen = Player_new("Mark ter Stegen", 1);

	struct Player barcelona_players[] = { messi, busquets, stegen };
	struct Team barcelona = Team_new("Barcelona", barcelona_players, 3);

	struct Player ramos = Player_new("Sergio Ramos", 4);
	struct Player asensio = Player_new("Asensio", 20);
	struct Player isco = Player_new("Isco", 22);

	struct Player real_players[] = { ramos, asensio, isco };
	struct Team real = Team_new("Real", real_players, 2);

	print_team(barcelona);
	print_team(real);

	struct Team teams[2] = { barcelona,real };

	char* teams_filename = "teams.dat";
	char* players_filename = "players.dat";

	int teams_size = 2;
	save_teams_to_file(teams_filename, players_filename, teams, teams_size);

	struct Team teams2[2];

	load_teams_from_file(teams_filename, players_filename, teams2);
	return 0;
}
