#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "football_league_vs.h"

#define MAX_NAME_LENGTH 50
#define MAX_PLAYERS_SIZE 20

int current_player_id = 0;
int current_team_id = 0;

struct Player {
	char name[MAX_NAME_LENGTH];
	int number;
	int id;
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
	char name[MAX_NAME_LENGTH];
	struct Player players[MAX_PLAYERS_SIZE];
	int players_size;
	int id;
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

void save_teams_to_file(char* teams_filename, char* players_filename, struct Team teams[]) {
	FILE* teams_file;
	FILE* players_file;

	int teams_size = sizeof(teams) / sizeof(teams[0]);

	if (fopen_s(&teams_file, teams_filename, "wb") != 0) {
		printf("Error opening the file %s fir writing\n", teams_filename);
		return;
	}
	if (fopen_s(&players_file, players_filename, "wb") != 0) {
		printf("Error opening the file %s for writing\n", players_filename);
		return;
	}

	for (int i = 0; i < teams_size; ++i) {
		fwrite(&teams[i], sizeof(teams[i]), 1, teams_file);

		int players_size = teams[i].players_size;
		for (int j = 0; j < players_size; ++j) 
			fwrite(&teams[i].players[j], sizeof(teams[i].players[j]), 1, players_file);		
	}

	fclose(teams_file);
	fclose(players_file);
}

void load_teams_from_file(char *teams_filename, char* players_filename, struct Team* teams) {
	FILE* teams_file;
	FILE* players_file;

	if (fopen_s(&teams_file, teams_filename, "rb") != 0) {
		printf("Error opening the file %s for reading\n", teams_filename);
		return;
	}
	if (fopen_s(&players_file, players_filename, "rb") != 0) {
		printf("Error opening the file %s for reading\n", players_filename);
		return;
	}

	struct Team team;
	size_t sz = fread(&team, sizeof(team), 1, teams_file);
	print_team(team);

	fclose(teams_file);
	fclose(players_file);
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

	struct Team teams[] = { barcelona, real };
	char* teams_filename = "teams.bin";
	char* players_filename = "players.bin";
	save_teams_to_file(teams_filename, players_filename, teams);

	struct Team teams2[2];
	load_teams_from_file(teams_filename, players_filename, teams2);
	print_team(teams2[0]);
	print_team(teams2[1]);
	return 0;
}
