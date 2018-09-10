#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Player {
    char name[50];
};

struct Player* Player_new(char* name) {
    struct Player* player = malloc(sizeof(struct Player));
    strcpy(player->name, name);
    return player;
};

struct Team {
    char name[50];
    struct Player* players;
};

struct Team* Team_new(char* name, struct Player* players) {
    struct Team* team = malloc(sizeof(struct Team));
    strcpy(team->name, name);
    int players_size = sizeof(players) / sizeof(players[0]);
    for (int i = 0; i < players_size; ++i)
        team->players[i] = players[i];
    return team;
};

void print_team(struct Team* team) {
    printf("%s\n", team->name);
    int size = sizeof(team->players) / sizeof(team->players[0]);
    for (int i = 0; i < size; ++i)
        printf("%s ", team->players[i].name);
    printf("\n");
}

int main()
{
    struct Player* messi = Player_new("Lionel Messi");
    struct Player* busquets = Player_new("Sergio Busquets");
    struct Player* ramos = Player_new("Sergio Ramos");
    struct Player* asensio = Player_new("Asensio");

    struct Player* barcelona_players[] = {messi, busquets};
    int size = sizeof(barcelona_players) / sizeof(barcelona_players[0]);
    for (int i = 0; i < size; ++i)
        printf("%s\n", barcelona_players[i]->name);
    struct Team* barcelona = Team_new("Barcelona", barcelona_players);

    struct Player* real_players[] = {ramos, asensio};
    size = sizeof(real_players) / sizeof(real_players[0]);
    for (int i = 0; i < size; ++i)
        printf("%s\n", real_players[i]->name);
    struct Team* real = Team_new("Real", real_players);

    print_team(barcelona);
    print_team(real);
    return 0;
}
