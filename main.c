
#include <stdio.h>
#include<stdlib.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>
#include <time.h>

#define MAX_i 12
#define MAX_j 12
int number_of_savegames = 0;
char the_main_menu[7][30] = {"<<WELCOME TO SEA BATTLE!>>", "MAIN MENU : ", "-Play With A Friend", "-Single Player",
                             "-Load Game<Doesn't Work>", "-Settings", "-Exit"};
char settings_menu[5][60] = {"SETTINGS: To change map size and ships :", "-Map", "-Ships",
                             "-Back"};
char game_modes[5][100] = {"Choose one of game modes :",
                           "1 : ( 5*1 -> 1 ship, 3*1 -> 2 ships, 2*1 -> 3 ships, 1*1 -> 4 ships ) (Default mode)",
                           "2 : ( 5*1 -> 1 ships, 3*1 -> 2 ships, 2*1 -> 2 ships, 1*1 -> 3 ships ) (Recommended for small maps)",
                           "3 : ( 5*1 -> 2 ships, 3*1 -> 3 ships, 2*1 -> 3 ships, 1*1 -> 5 ships ) (Recommended for big maps)",
                           "-Back"};
char users_menu[3][30] = {"-Make new user", "-Choose from available users<Doesn't Work>", "-Back"};
char map_menu[2][10] = {"-Auto Map", "-Put Ships"};
//char play_with_friend_menu[5][30] = {"Set users, then start game:","-Start Game", "-User1", "-User2", "-Back"};
int i_len = 10, j_len = 10;
enum arrow_key {
    up = 72, down = 80, right = 77, left = 75
};
FILE *save_nums;
FILE *save_list;
char map1[MAX_i + 2][MAX_j + 2], map2[MAX_i + 2][MAX_j + 2];

struct ship_setting {
    int number_of_ships;
    int five_num;
    int three_num;
    int two_num;
    int one_num;
} ships_mode;

struct components {
    int row;
    int column;
};
struct USER {
    char username[20];
    int score;
    char user_map[MAX_i + 2][MAX_j + 2];
    struct ships *user_ships;
} user1, user2;
struct ships {
    char orientation;
    int length_of_ship;
    struct components start;
    struct components end;
    struct ships *next;
} ships_user1, ships_user2;
struct ships *head1, *head2;


void main_menu(char map[MAX_i + 2][MAX_j + 2]);

void move_on_menu(int direction, int *y, int upside, int downside);

void move_on_map(int directoin, int *x_counter, int *y_counter, int *x, int *y, int upside, int downside, int rightside,
                 int leftside);

void play_with_friend(char map[MAX_i + 2][MAX_j + 2]);

void play_game(int load_or_new);

void check_to_remove(int user_num, int row, int column);

void save_game(int turn);

int save_users(struct USER *user, int user_num, char map[MAX_i + 2][MAX_j + 2]);

void input(struct USER *the_user, int user_num, char map[MAX_i + 2][MAX_j + 2]);

void auto_map(struct USER *user_to_set, int user_num, int mode);// mode : 1 with firend. 0 : with computer

int is_valid_location(struct USER *the_user, int length, int orientation, int start_row, int start_column, int end_row,
                      int end_column);

void
add_ship(int user_num, int length, char orientation, int start_row, int start_column,
         int end_row,
         int end_column);

//void save_ship_list()
void add_on_map(int user_num, char orientation, struct components start, struct components end);

void fill_map_with_water(char map[MAX_i + 2][MAX_j + 2]);

void single_player(char map[MAX_i + 2][MAX_j + 2]);

void load_game(char map[MAX_i + 2][MAX_j + 2]);

void settings(char map[MAX_i + 2][MAX_j + 2]);

void game_mode(int mode);

void draw(char map[MAX_i + 2][MAX_j + 2]);

void go_to(int x, int y);

void to_order(int *x1, int *x2);

void empty_map(char map[MAX_i + 2][MAX_j + 2]);

void empty_list(int user_num);

int main() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
    printf("Starting ...");
    ships_mode.number_of_ships = 10;
    ships_mode.five_num = 1;
    ships_mode.three_num = 2;
    ships_mode.two_num = 3;
    ships_mode.one_num = 4;
    strcpy(user1.username, "user1");
    strcpy(user2.username, "user2");

    char map[MAX_i + 2][MAX_j + 2];
    save_nums = fopen("d:\\SAVE_GAMES\\save_num.bin", "rb");
    fread(&number_of_savegames, sizeof(int), 1, save_nums);
    fclose(save_nums);
    empty_map(map);
    empty_map(map1);
    empty_map(map2);
    empty_map(user1.user_map);
    empty_map(user2.user_map);
    for (int i = 0; i < MAX_i; i++) {
        strcpy(user1.user_map[i], map[i]);
    }
    Sleep(1000);
    main_menu(map);
    return 0;
}

void main_menu(char map[MAX_i + 2][MAX_j + 2]) {
    while (1) {
        user1.user_ships = (struct ships *) malloc(sizeof(struct ships));
        user1.user_ships = head1;
        user2.user_ships = (struct ships *) malloc(sizeof(struct ships));
        user2.user_ships = head2;
        head1 = (struct ships *) malloc(sizeof(struct ships));
        head1 = NULL;
        head2 = (struct ships *) malloc(sizeof(struct ships));
        head2 = NULL;
        save_nums = fopen("d:\\SAVE_GAMES\\save_num.bin", "rb");
        fread(&number_of_savegames, sizeof(int), 1, save_nums);
        fclose(save_nums);
        char ch, key;
        int y_main_menu = 2;
        go_to(0, 0);
        for (int i = 0; i < 7; i++)
            printf("%s\n", the_main_menu[i]);
        go_to(0, 2);
        while (getch() == 224) {
            move_on_menu(getch(), &y_main_menu, 2, 6);
        }
        system("cls");
        go_to(0, 0);
        switch (y_main_menu) {
            case 2 : // play with friend
                play_with_friend(map);
                break;
            case 3 : // single player
                single_player(map);
            case 4 : // load game
                load_game(map);
                break;
            case 5 : // settings
                settings(map);
                break;
            case 6 : // exit
                system("cls");
                exit(-1);
            default:
                break;
        }
    }
}

void move_on_menu(int direction, int *y, int upside, int downside) {
    switch (direction) {
        case up:
            if (*y > upside) {
                go_to(0, (*y) - 1);
                (*y)--;
            }
            break;
        case down:
            if (*y < downside) {
                go_to(0, (*y) + 1);
                (*y)++;
            }
            break;
        default:
            break;
    }
}

void play_with_friend(char map[MAX_i + 2][MAX_j + 2]) {
    int k = save_users(&user1, 1, map);
    if (k == 1)
        k = save_users(&user2, 2, map);
    if (k == 1) play_game(0);
    return;
}

void play_game(int load_or_new) {
    int counter;
    if (load_or_new == 0) counter = 1;
    else counter = load_or_new;
    while (head1 != NULL && head2 != NULL) {
        while (counter == 1) {
            int x_map = 9;
            int y_map = 3;
            printf("\"%s\" it is your turn,     your score: %d  \n", user1.username, user1.score);
            draw(map2);
            printf("         -Save   -Back   -Exit");
            go_to(9, 3);
            //while (1)
            int row, column;
            int x_counter = 0, y_counter = 0;
            while (getch() == 224) {
                move_on_map(getch(), &x_counter, &y_counter, &x_map, &y_map, 3, 2 * i_len + 2, 8 * j_len - 1, 9);
            }
            row = 1 + y_counter;
            column = 1 + x_counter;
            //printf("%d %d", row, column);
            if (row == i_len + 1) {
                if (column == 1) {
                    save_game(counter);
                    system("cls");
                    printf("saving ...");
                    Sleep(800);
                    system("cls");
                    continue;
                } else if (column == 3) {
                    exit(-1);
                } else if (column == 2) {
                    empty_map(user1.user_map);
                    empty_map(user2.user_map);
                    empty_map(map1);
                    empty_map(map2);
                    empty_list(1);
                    empty_list(2);
                    system("cls");
                    return;
                } else {
                    system("cls");
                    continue;
                }
            } else {
                if (map2[row][column] != '-') {
                    system("cls");
                    continue;
                }
                if (user2.user_map[row][column] == 'S') {
                    map2[row][column] = 'X';
                    user1.score++;
                    check_to_remove(1, row, column);
                    system("cls");
                    if (head2 == NULL)
                        break;
                    continue;
                } else if (user2.user_map[row][column] == 'W') {
                    map2[row][column] = 'W';
                    system("cls");
                    printf("Wrong chice! :( %s ... see the %s map while turn changes :\n", user1.username,
                           user2.username);
                    draw(map2);
                    Sleep(1000);
                    system("cls");
                    counter = 2;
                    break;
                }
            }
        }
        while (counter == 2) {
            int x_map = 9;
            int y_map = 3;
            printf(" \"%s\" it is your turn,     your score: %d  \n", user2.username, user2.score);
            draw(map1);
            printf("         -Save   -Back   -Exit");
            go_to(9, 3);
            int row, column;
            int x_counter = 0, y_counter = 0;
            while (getch() == 224) {
                move_on_map(getch(), &x_counter, &y_counter, &x_map, &y_map, 3, 2 * i_len + 2, 8 * j_len - 1, 9);
            }
            row = 1 + y_counter;
            column = 1 + x_counter;
            if (row == i_len + 1) {
                if (column == 1) {
                    save_game(counter);
                    system("cls");
                    printf("is saving ...");
                    Sleep(800);
                    system("cls");
                    continue;
                } else if (column == 3) {
                    exit(-1);
                } else if (column == 2) {
                    empty_map(user1.user_map);
                    empty_map(user2.user_map);
                    empty_map(map1);
                    empty_map(map2);
                    empty_list(1);
                    empty_list(2);
                    system("cls");
                    return;
                } else {
                    system("cls");
                    continue;
                }
            } else {
                if (map1[row][column] != '-') {
                    system("cls");
                    continue;
                }
                if (user1.user_map[row][column] == 'S') {
                    map1[row][column] = 'X';
                    user2.score++;
                    check_to_remove(2, row, column);
                    system("cls");
                    if (head1 == NULL)
                        break;
                    continue;
                } else if (user1.user_map[row][column] == 'W') {
                    map1[row][column] = 'W';
                    system("cls");
                    printf("Wrong chice! :( %s ... see the %s map while turn changes :\n", user2.username,
                           user1.username);
                    draw(map1);
                    Sleep(1000);
                    system("cls");
                    counter = 1;
                    break;
                }
            }
        }
    }
    system("cls");
    if (head1 == NULL) {
        printf("| *** %s Won! *** |", user2.username);
        printf(" %s map :\n", user1.username);
        draw(map1);
        printf("\n");
        printf(" %s map :\n", user2.username);
        draw(map2);
        printf("\nFinishing game ...");
        Sleep(3000);
    } else if (head2 == NULL) {
        printf("| *** %s Won! *** |", user1.username);
        printf(" %s map :\n", user2.username);
        draw(map2);
        printf("\n");
        printf(" %s map :\n", user1.username);
        draw(map1);
        printf("\nFinishing game ...");
        Sleep(3000);
    }
    empty_list(1);
    empty_list(2);
    empty_map(map1);
    empty_map(map2);
    system("cls");
    return;
}

void check_to_remove(int user_num, int row, int column) {
    struct ships *to_check;
    struct ships *prev;
    if (user_num == 1) {
        prev = head2;
        to_check = head2;
        int position = 1;
        while (to_check->length_of_ship == 1) {
            if (row == to_check->start.row && column == to_check->start.column) {
                map2[row][column] = 'S';
                map2[row + 1][column] = 'W';
                map2[row + 1][column - 1] = 'W';
                map2[row + 1][column + 1] = 'W';
                map2[row - 1][column] = 'W';
                map2[row - 1][column - 1] = 'W';
                map2[row - 1][column + 1] = 'W';
                map2[row - 1][column + 1] = 'W';
                map2[row][column - 1] = 'W';
                map2[row][column + 1] = 'W';
                user1.score += 25;
                if (position == 1) {
                    head2 = to_check->next;
                    free(prev);
                } else {
                    prev->next = to_check->next;
                    free(to_check);
                }
                return;
            } else {
                position++;
                prev = to_check;
                to_check = to_check->next;
            }
        }
        while (1) {
            int breaker;
            if (to_check->next == NULL)
                breaker = 1;
            if (to_check->orientation == 'H') {
                int count = 0;
                to_order(&(to_check->start.column), &(to_check->end.column));
                for (int j = to_check->start.column; j <= to_check->end.column; j++) {
                    if (map2[to_check->start.row][j] == 'X') count++;
                }
                if (count == to_check->length_of_ship) {
                    for (int j = to_check->start.column; j <= to_check->end.column; j++) {
                        map2[to_check->start.row][j] = 'S';
                        map2[to_check->start.row - 1][j] = 'W';
                        map2[to_check->start.row + 1][j] = 'W';
                    }
                    map2[to_check->start.row][to_check->start.column - 1] = 'W';
                    map2[to_check->start.row - 1][to_check->start.column - 1] = 'W';
                    map2[to_check->start.row + 1][to_check->start.column - 1] = 'W';
                    map2[to_check->end.row][to_check->end.column + 1] = 'W';
                    map2[to_check->end.row - 1][to_check->end.column + 1] = 'W';
                    map2[to_check->end.row + 1][to_check->end.column + 1] = 'W';
                    if (to_check->length_of_ship == 2) user1.score += 12;
                    else if (to_check->length_of_ship == 3) user1.score += 8;
                    else if (to_check->length_of_ship == 5) user1.score += 5;
                    if (position == 1) {
                        head2 = to_check->next;
                        free(prev);
                    } else {
                        prev->next = to_check->next;
                        free(to_check);
                    }
                    return;
                }

            } else {
                int count = 0;
                to_order(&(to_check->start.row), &(to_check->end.row));
                for (int i = to_check->start.row; i <= to_check->end.row; i++) {
                    if (map2[i][to_check->start.column] == 'X') count++;
                }
                if (count == to_check->length_of_ship) {
                    for (int i = to_check->start.row; i <= to_check->end.row; i++) {
                        map2[i][to_check->start.column] = 'S';
                        map2[i][to_check->start.column - 1] = 'W';
                        map2[i][to_check->start.column + 1] = 'W';
                    }
                    map2[to_check->start.row - 1][to_check->start.column] = 'W';
                    map2[to_check->start.row - 1][to_check->start.column - 1] = 'W';
                    map2[to_check->start.row - 1][to_check->start.column + 1] = 'W';
                    map2[to_check->end.row + 1][to_check->start.column] = 'W';
                    map2[to_check->end.row + 1][to_check->start.column - 1] = 'W';
                    map2[to_check->end.row + 1][to_check->start.column + 1] = 'W';
                    if (to_check->length_of_ship == 2) user1.score += 12;
                    else if (to_check->length_of_ship == 3) user1.score += 8;
                    else if (to_check->length_of_ship == 5) user1.score += 5;
                    if (position == 1) {
                        head2 = to_check->next;
                        free(prev);
                    } else {
                        prev->next = to_check->next;
                        free(to_check);
                    }
                    return;
                }
            }
            position++;
            prev = to_check;
            to_check = to_check->next;
            if (breaker == 1) break;
        }
        return;
    } else {
        to_check = head1;
        prev = head1;
        int position = 1;
        while (to_check->length_of_ship == 1) {
            if (row == to_check->start.row && column == to_check->start.column) {
                map1[row][column] = 'S';
                map1[row + 1][column] = 'W';
                map1[row + 1][column - 1] = 'W';
                map1[row + 1][column + 1] = 'W';
                map1[row - 1][column] = 'W';
                map1[row - 1][column - 1] = 'W';
                map1[row - 1][column + 1] = 'W';
                map1[row - 1][column + 1] = 'W';
                map1[row][column - 1] = 'W';
                map1[row][column + 1] = 'W';
                user2.score += 25;
                if (position == 1) {
                    head1 = to_check->next;
                    free(prev);
                } else {
                    prev->next = to_check->next;
                    free(to_check);
                }
                return;
            } else {
                position++;
                prev = to_check;
                to_check = to_check->next;
            }
        }
        while (1) {
            int breaker;
            if (to_check->next == NULL)
                breaker = 1;
            if (to_check->orientation == 'H') {
                int count = 0;
                to_order(&(to_check->start.column), &(to_check->end.column));
                for (int j = to_check->start.column; j <= to_check->end.column; j++) {
                    if (map1[to_check->start.row][j] == 'X') count++;
                }
                if (count == to_check->length_of_ship) {
                    for (int j = to_check->start.column; j <= to_check->end.column; j++) {
                        map1[to_check->start.row][j] = 'S';
                        map1[to_check->start.row - 1][j] = 'W';
                        map1[to_check->start.row + 1][j] = 'W';
                    }
                    map1[to_check->start.row][to_check->start.column - 1] = 'W';
                    map1[to_check->start.row - 1][to_check->start.column - 1] = 'W';
                    map1[to_check->start.row + 1][to_check->start.column - 1] = 'W';
                    map1[to_check->end.row][to_check->end.column + 1] = 'W';
                    map1[to_check->end.row - 1][to_check->end.column + 1] = 'W';
                    map1[to_check->end.row + 1][to_check->end.column + 1] = 'W';
                    if (to_check->length_of_ship == 2) user2.score += 12;
                    else if (to_check->length_of_ship == 3) user2.score += 8;
                    else if (to_check->length_of_ship == 5) user2.score += 5;
                    if (position == 1) {
                        head1 = to_check->next;
                        free(prev);
                    } else {
                        prev->next = to_check->next;
                        free(to_check);
                    }
                    return;
                }

            } else {
                int count = 0;
                to_order(&(to_check->start.row), &(to_check->end.row));
                for (int i = to_check->start.row; i <= to_check->end.row; i++) {
                    if (map1[i][to_check->start.column] == 'X') count++;
                }
                if (count == to_check->length_of_ship) {
                    for (int i = to_check->start.row; i <= to_check->end.row; i++) {
                        map1[i][to_check->start.column] = 'S';
                        map1[i][to_check->start.column - 1] = 'W';
                        map1[i][to_check->start.column + 1] = 'W';
                    }
                    map1[to_check->start.row - 1][to_check->start.column] = 'W';
                    map1[to_check->start.row - 1][to_check->start.column - 1] = 'W';
                    map1[to_check->start.row - 1][to_check->start.column + 1] = 'W';
                    map1[to_check->end.row + 1][to_check->start.column] = 'W';
                    map1[to_check->end.row + 1][to_check->start.column - 1] = 'W';
                    map1[to_check->end.row + 1][to_check->start.column + 1] = 'W';
                    if (to_check->length_of_ship == 2) user2.score += 12;
                    else if (to_check->length_of_ship == 3) user2.score += 8;
                    else if (to_check->length_of_ship == 5) user2.score += 5;
                    if (position == 1) {
                        head1 = to_check->next;
                        free(prev);
                    } else {
                        prev->next = to_check->next;
                        free(to_check);
                    }
                    return;
                }
            }
            position++;
            prev = to_check;
            to_check = to_check->next;
            if (breaker == 1) break;
        }
    }
}


void
move_on_map(int direction, int *x_counter, int *y_counter, int *x, int *y, int upside, int downside, int rightside,
            int leftside) {
    switch (direction) {
        case up:
            if ((*y) > upside) {
                go_to(*x, (*y) - 2);
                (*y) -= 2;
                (*y_counter)--;
            }
            break;
        case down:
            if ((*y) < downside) {
                go_to(*x, (*y) + 2);
                (*y) += 2;
                (*y_counter)++;
            }
            break;
        case left :
            if ((*x) > leftside) {
                go_to((*x) - 8, *y);
                (*x) -= 8;
                (*x_counter)--;
            }
            break;
        case right :
            if ((*x) < rightside) {
                go_to((*x) + 8, *y);
                (*x) += 8;
                (*x_counter)++;
            }

        default:
            break;
    }
}


int save_users(struct USER *theuser, int user_num, char map[MAX_i + 2][MAX_j + 2]) {
    while (1) {
        FILE *save_user;
        int y_users_menu = 1;
        printf("User %d :\n", user_num);
        for (int i = 0; i <= 2; i++) {
            printf("%s\n", users_menu[i]);
        }
        go_to(0, 1);
        while (getch() == 224) {
            move_on_menu(getch(), &y_users_menu, 1, 3);
        }
        system("cls");
        switch (y_users_menu) {
            case 1 ://New user
                input(theuser, user_num, map);
                save_user = fopen("d:\\game\\users.txt", "a+");
                fprintf(save_user, "%s : %d\n", theuser->username, theuser->score);
                fclose(save_user);
                break;
            case 2:
                system("cls");
                printf("no users availabe!");
                Sleep(1000);
                system("cls");
                continue;
                break;
            case 3://back
                return 0;
            default:
                break;
        }
        break;
    }
    return 1;
}

void input(struct USER *the_user, int user_num, char map[MAX_i + 2][MAX_j + 2]) {
    printf("Please Enter a username : ");
    gets(the_user->username);
    int y_map_menu = 0;
    system("cls");
    for (int i = 0; i <= 1; i++)
        printf("%s\n", map_menu[i]);
    go_to(0, 0);
    while (getch() == 224) {
        move_on_menu(getch(), &y_map_menu, 0, 1);
    }
    if (y_map_menu == 0) {
        auto_map(the_user, user_num, 1);
        return;
    } else {
        system("cls");
        printf("OK \"%s\"! Let's start. First You should fill a map with ships for yourself , your ships include : \n ",
               the_user->username);

        int i_start_loc, j_start_loc, i_end_loc, j_end_loc;
        char orientation, ok;

        while (1) {

            int ship_num = 1;
            printf("\n%d Ships :  5*1 Ships -> %d, 3*1 Ships -> %d, 2*1 Ships -> %d, 1*1 Ships -> %d. ",
                   ships_mode.number_of_ships, ships_mode.five_num, ships_mode.three_num, ships_mode.two_num,
                   ships_mode.one_num);
            printf("Your ships can be Vertical(V) or Horizontal(H)\n");
            printf("This is the empty map :\n");
            empty_map(the_user->user_map);
            draw(the_user->user_map);
            printf("\nNow please enter your ships locations (orientation , start point and end point based on the map). Please click <Enter> at the end of each point you input! :\n");
            for (int i = 1; i <= ships_mode.five_num; i++) {
                printf("Ship %d (5*1) : \n", ship_num);
                while (1) {
                    while (1) {
                        printf("Orientation : (H : horizontal, V : vertical) \n");
                        scanf("%c", &orientation);
                        getchar();
                        fflush(stdin);
                        orientation = toupper(orientation);
                        if (orientation == 'H') {
                            while (1) {
                                printf("Row : ");
                                scanf("%d", &i_start_loc);
                                getchar();
                                fflush(stdin);
                                i_end_loc = i_start_loc;
                                printf("Start Column and End Column : ");
                                scanf("%d %d", &j_start_loc, &j_end_loc);
                                getchar();
                                fflush(stdin);
                                if (is_valid_location(the_user, 5, orientation, i_start_loc, j_start_loc, i_end_loc,
                                                      j_end_loc) ==
                                    0) {
                                    printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                                    continue;
                                }
                                break;
                            }
                        } else if (orientation == 'V') {
                            while (1) {
                                printf("Column : ");
                                scanf("%d", &j_start_loc);
                                getchar();
                                fflush(stdin);
                                j_end_loc = j_start_loc;
                                printf("Start Row and End Row : ");
                                scanf("%d %d", &i_start_loc, &i_end_loc);
                                getchar();
                                fflush(stdin);
                                if (is_valid_location(the_user, 5, orientation, i_start_loc, j_start_loc, i_end_loc,
                                                      j_end_loc) ==
                                    0) {
                                    printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                                    continue;
                                }
                                break;
                            }
                        } else {
                            printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                            continue;
                        }
                        break;
                    }
                    printf("You Sure about your inputs : Enter Y for YES to continue or N for NO to enter new inputs : ");
                    scanf("%c", &ok);
                    getchar();
                    fflush(stdin);
                    ok = toupper(ok);
                    if (ok == 'Y') break;
                    else continue;

                }
                add_ship(user_num, 5, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc);
                system("cls");
                draw(the_user->user_map);
                ship_num++;
            }
            for (int i = 1; i <= ships_mode.three_num; i++) {
                printf("Ship %d (3*1) : \n", ship_num);
                while (1) {

                    while (1) {
                        printf("Orientation : (H : horizontal, V : vertical)\n");
                        scanf("%c", &orientation);
                        getchar();
                        fflush(stdin);
                        orientation = toupper(orientation);
                        if (orientation == 'H') {
                            while (1) {
                                printf("Row : ");
                                scanf("%d", &i_start_loc);
                                getchar();
                                fflush(stdin);
                                i_end_loc = i_start_loc;
                                printf("Start Column and End Column : ");
                                scanf("%d %d", &j_start_loc, &j_end_loc);
                                getchar();
                                fflush(stdin);
                                if (is_valid_location(the_user, 3, orientation, i_start_loc, j_start_loc, i_end_loc,
                                                      j_end_loc) == 0) {
                                    printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                                    continue;
                                }
                                break;
                            }
                        } else if (orientation == 'V') {
                            while (1) {
                                printf("Column : ");
                                scanf("%d", &j_start_loc);
                                getchar();
                                fflush(stdin);
                                j_end_loc = j_start_loc;
                                printf("Start Row and End Row : ");
                                scanf("%d %d", &i_start_loc, &i_end_loc);
                                getchar();
                                fflush(stdin);
                                if (is_valid_location(the_user, 3, orientation, i_start_loc, j_start_loc, i_end_loc,
                                                      j_end_loc) == 0) {
                                    printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                                    continue;
                                }
                                break;
                            }
                        } else {
                            printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                            continue;
                        }
                        break;
                    }
                    printf("You Sure about your inputs : Enter Y for YES to continue or N for NO to enter new inputs : ");
                    scanf("%c", &ok);
                    getchar();
                    fflush(stdin);
                    ok = toupper(ok);
                    if (ok == 'Y') break;
                    else continue;
                }
                add_ship(user_num, 3, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc);
                system("cls");
                draw(the_user->user_map);
                ship_num++;
            }
            for (int i = 1; i <= ships_mode.two_num; i++) {
                printf("Ship %d (2*1) : \n", ship_num);
                while (1) {

                    while (1) {
                        printf("Orientation : (H : horizontal, V : vertical)\n");
                        scanf("%c", &orientation);
                        getchar();
                        fflush(stdin);
                        orientation = toupper(orientation);
                        if (orientation == 'H') {
                            while (1) {
                                printf("Row : ");
                                scanf("%d", &i_start_loc);
                                getchar();
                                fflush(stdin);
                                i_end_loc = i_start_loc;
                                printf("Start Column and End Column : ");
                                scanf("%d %d", &j_start_loc, &j_end_loc);
                                getchar();
                                fflush(stdin);
                                if (is_valid_location(the_user, 2, orientation, i_start_loc, j_start_loc, i_end_loc,
                                                      j_end_loc) == 0) {
                                    printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                                    continue;
                                }
                                break;
                            }
                        } else if (orientation == 'V') {
                            while (1) {
                                printf("Column : ");
                                scanf("%d", &j_start_loc);
                                getchar();
                                fflush(stdin);
                                j_end_loc = j_start_loc;
                                printf("Start Row and End Row : ");
                                scanf("%d %d", &i_start_loc, &i_end_loc);
                                getchar();
                                fflush(stdin);
                                if (is_valid_location(the_user, 2, orientation, i_start_loc, j_start_loc, i_end_loc,
                                                      j_end_loc) == 0) {
                                    printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                                    continue;
                                }
                                break;
                            }
                        } else {
                            printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                            continue;
                        }
                        break;
                    }
                    printf("You Sure about your inputs : Enter Y for YES to continue or N for NO to enter new inputs : ");
                    scanf("%c", &ok);
                    getchar();
                    fflush(stdin);
                    ok = toupper(ok);
                    if (ok == 'Y') break;
                    else continue;
                }
                add_ship(user_num, 2, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc);
                system("cls");
                draw(the_user->user_map);
                ship_num++;
            }
            for (int i = 1; i <= ships_mode.one_num; i++) {
                printf("Ship %d ( 1*1 ) :\n", ship_num);
                while (1) {

                    orientation = '1';
                    printf("Column and Row : (first column number, the row number)\n ");
                    scanf("%d %d", &j_start_loc, &i_start_loc);
                    getchar();
                    fflush(stdin);
                    i_end_loc = i_start_loc;
                    j_end_loc = j_start_loc;
                    if (is_valid_location(the_user, 1, orientation, i_start_loc, j_start_loc, i_end_loc,
                                          j_end_loc) == 0) {
                        printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                        continue;
                    }
                    printf("You Sure about your inputs : Enter Y for YES to continue or N for NO to enter new inputs : ");
                    scanf("%c", &ok);
                    getchar();
                    fflush(stdin);
                    ok = toupper(ok);
                    if (ok == 'Y') break;
                    else continue;
                }
                add_ship(user_num, 1, '1', i_start_loc, j_start_loc, i_end_loc, j_end_loc);
                system("cls");
                draw(the_user->user_map);
                ship_num++;
            }
            fill_map_with_water(the_user->user_map);
            system("cls");
            printf("OK %s! this is your final map :\n", the_user->username);
            draw(the_user->user_map);
            printf("You Sure about your map : Enter Y for YES to continue or N for NO to fill the map again :\n");
            scanf("%c", &ok);
            getchar();
            fflush(stdin);
            ok = toupper(ok);
            if (ok == 'Y') {
                system("cls");
                break;
            } else {
                empty_list(user_num);
                system("cls");
                continue;
            }
        }
    }
}

void auto_map(struct USER *user_to_set, int user_num, int mode) {
    while (1) {
        system("cls");
        printf("Setting a map ...");
        srand(time(NULL));
        for (int i = 1; i <= ships_mode.five_num; i++) {
            while (1) {
                char orientation = rand() % 2;
                int i_start_loc, j_start_loc;
                int i_end_loc, j_end_loc;
                if (orientation == 0) {
                    orientation = 'H';
                    i_start_loc = rand() % i_len + 1;
                    i_end_loc = i_start_loc;
                    j_start_loc = rand() % j_len + 1;

                    if (j_start_loc <= i_len - 4)
                        j_end_loc = j_start_loc + 4;
                    else
                        j_end_loc = j_start_loc - 4;
                } else {
                    orientation = 'V';
                    j_start_loc = rand() % j_len + 1;
                    j_end_loc = j_start_loc;
                    i_start_loc = rand() % i_len + 1;
                    if (i_start_loc <= j_len - 4)
                        i_end_loc = i_start_loc + 4;
                    else
                        i_end_loc = i_start_loc - 4;
                }
                if (is_valid_location(user_to_set, 5, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc) ==
                    1) {
                    add_ship(user_num, 5, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc);
                } else {
                    continue;
                }
                break;
            }
        }
        for (int i = 1; i <= ships_mode.three_num; i++) {
            while (1) {
                char orientation = rand() % 2;
                int i_start_loc, j_start_loc;
                int i_end_loc, j_end_loc;
                if (orientation == 0) {
                    orientation = 'H';
                    i_start_loc = rand() % i_len + 1;
                    i_end_loc = i_start_loc;
                    j_start_loc = rand() % j_len + 1;
                    if (j_start_loc <= j_len - 2)
                        j_end_loc = j_start_loc + 2;
                    else
                        j_end_loc = j_start_loc - 2;
                } else {
                    orientation = 'V';
                    j_start_loc = rand() % j_len + 1;
                    j_end_loc = j_start_loc;
                    i_start_loc = rand() % i_len + 1;
                    if (i_start_loc <= i_len - 2)
                        i_end_loc = i_start_loc + 2;
                    else
                        i_end_loc = i_start_loc - 2;
                }
                if (is_valid_location(user_to_set, 3, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc) ==
                    1) {
                    add_ship(user_num, 3, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc);
                } else {
                    continue;
                }
                break;
            }
        }
        for (int i = 1; i <= ships_mode.two_num; i++) {
            while (1) {
                char orientation = rand() % 2;
                int i_start_loc, j_start_loc;
                int i_end_loc, j_end_loc;
                if (orientation == 0) {
                    orientation = 'H';
                    i_start_loc = rand() % i_len + 1;
                    i_end_loc = i_start_loc;
                    j_start_loc = rand() % j_len + 1;
                    if (j_start_loc <= j_len - 1)
                        j_end_loc = j_start_loc + 1;
                    else
                        j_end_loc = j_start_loc - 1;
                } else {
                    orientation = 'V';
                    j_start_loc = rand() % j_len + 1;
                    j_end_loc = j_start_loc;
                    i_start_loc = rand() % i_len + 1;
                    if (i_start_loc <= i_len - 1)
                        i_end_loc = i_start_loc + 1;
                    else
                        i_end_loc = i_start_loc - 1;
                }
                if (is_valid_location(user_to_set, 2, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc) ==
                    1) {
                    add_ship(user_num, 2, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc);
                } else {
                    continue;
                }
                break;
            }
        }
        for (int i = 1; i <= ships_mode.one_num; i++) {
            while (1) {
                char orientation;
                int i_start_loc, j_start_loc;
                int i_end_loc, j_end_loc;
                orientation = '1';
                i_start_loc = rand() % i_len + 1;
                i_end_loc = i_start_loc;
                j_start_loc = rand() % j_len + 1;
                j_end_loc = j_start_loc;
                if (is_valid_location(user_to_set, 1, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc) ==
                    1) {
                    add_ship(user_num, 1, orientation, i_start_loc, j_start_loc, i_end_loc, j_end_loc);
                } else {
                    continue;
                }
                break;
            }
        }
        if (mode == 1) {
            Sleep(1000);
            system("cls");
            fill_map_with_water(user_to_set->user_map);
            draw(user_to_set->user_map);
            char ok;
            printf("You like this map ? : Y for Yes, N for No to regenerate!\n");
            scanf("%c", &ok);
            getchar();
            fflush(stdin);
            ok = toupper(ok);
            if (ok == 'Y') {
                system("cls");
                break;
            } else {
                empty_map(user_to_set->user_map);
                empty_list(user_num);
                continue;
            }
        } else {
            fill_map_with_water(user_to_set->user_map);
            system("cls");
            break;
        }
    }
}

int
is_valid_location(struct USER *the_user, int length, int orientation, int start_row, int start_column, int end_row,
                  int end_column) {
    if (orientation == '1') {
        if (start_row < 1 || start_row > i_len || start_column > j_len || end_column < 1) {
            return 0;
        }
        if (the_user->user_map[start_row][start_column] != '-' || the_user->user_map[end_row][end_column] != '-') {
            return 0;
        }
        return 1;
    } else if (orientation == 'H') {
        if (start_row > i_len || start_row < 1) {

            return 0;
        }
        if (start_column > j_len || start_column < 1 || end_column > j_len || end_column < 1) {

            return 0;
        }
        if (abs(start_column - end_column) != length - 1) {

            return 0;
        }
        if (the_user->user_map[start_row][start_column] != '-' || the_user->user_map[end_row][end_column] != '-') {

            return 0;
        }
        return 1;
    } else if (orientation == 'V') {
        if (start_column > i_len || start_column < 1) {

            return 0;
        }
        if (start_row > j_len || start_row < 1 || end_row > j_len || end_row < 1) {

            return 0;
        }
        if (abs(start_row - end_row) != length - 1) {
            return 0;
        }
        if (the_user->user_map[start_row][start_column] != '-' || the_user->user_map[end_row][end_column] != '-') {
            return 0;
        }
        return 1;
    }

}

void
add_ship(int user_num, int length, char orientation, int start_row,
         int start_column, int end_row,
         int end_column) {
    struct ships *new = (struct ships *) malloc(sizeof(struct ships));
    new->orientation = orientation;
    new->length_of_ship = length;
    new->start.row = start_row;
    new->start.column = start_column;
    new->end.row = end_row;
    new->end.column = end_column;
    if (user_num == 1) {
        new->next = head1;
        head1 = new;
    } else {
        new->next = head2;
        head2 = new;
    }
    add_on_map(user_num, orientation, new->start, new->end);

}


void add_on_map(int user_num, char orientation, struct components start, struct components end) {
    struct USER *the_user;
    if (user_num == 1) the_user = &user1;
    else the_user = &user2;
    if (orientation == '1') {
        the_user->user_map[start.row][start.column] = 'S';
        the_user->user_map[start.row + 1][start.column] = 'W';
        the_user->user_map[start.row + 1][start.column - 1] = 'W';
        the_user->user_map[start.row + 1][start.column + 1] = 'W';
        the_user->user_map[start.row - 1][start.column] = 'W';
        the_user->user_map[start.row - 1][start.column - 1] = 'W';
        the_user->user_map[start.row - 1][start.column + 1] = 'W';
        the_user->user_map[start.row - 1][start.column + 1] = 'W';
        the_user->user_map[start.row][start.column - 1] = 'W';
        the_user->user_map[start.row][start.column + 1] = 'W';

    } else if (orientation == 'H') {
        to_order(&start.column, &end.column);
        for (int j = start.column; j <= end.column; j++) {
            the_user->user_map[start.row][j] = 'S';
            the_user->user_map[start.row - 1][j] = 'W';
            the_user->user_map[start.row + 1][j] = 'W';
        }
        the_user->user_map[start.row][start.column - 1] = 'W';
        the_user->user_map[start.row - 1][start.column - 1] = 'W';
        the_user->user_map[start.row + 1][start.column - 1] = 'W';
        the_user->user_map[end.row][end.column + 1] = 'W';
        the_user->user_map[end.row - 1][end.column + 1] = 'W';
        the_user->user_map[end.row + 1][end.column + 1] = 'W';
    } else if (orientation == 'V') {
        to_order(&start.row, &end.row);
        for (int i = start.row; i <= end.row; i++) {
            the_user->user_map[i][start.column] = 'S';
            the_user->user_map[i][start.column - 1] = 'W';
            the_user->user_map[i][start.column + 1] = 'W';
        }
        the_user->user_map[start.row - 1][start.column] = 'W';
        the_user->user_map[start.row - 1][start.column - 1] = 'W';
        the_user->user_map[start.row - 1][start.column + 1] = 'W';
        the_user->user_map[end.row + 1][start.column] = 'W';
        the_user->user_map[end.row + 1][start.column - 1] = 'W';
        the_user->user_map[end.row + 1][start.column + 1] = 'W';
    }
}

void fill_map_with_water(char map[MAX_i + 2][MAX_j + 2]) {
    for (int i = 1; i < i_len + 1; i++)
        for (int j = 1; j < j_len + 1; j++)
            if (map[i][j] == '-')
                map[i][j] = 'W';
}

void single_player(char map[MAX_i + 2][MAX_j + 2]) {
    srand(time(NULL));
    FILE *save_user;
    int smart_i = -1, smart_j = -1;
    int y_users_menu = 1;
    printf("Choose one :\n");
    for (int i = 0; i <= 2; i++) {
        printf("%s\n", users_menu[i]);
    }
    go_to(0, 1);
    while (getch() == 224) {
        move_on_menu(getch(), &y_users_menu, 1, 3);
    }
    system("cls");
    switch (y_users_menu) {
        case 1 ://New user
            input(&user1, 1, map);
            save_user = fopen("d:\\game\\users.txt", "a+");
            fprintf(save_user, "%s : %d\n", user1.username, user1.score);
            fclose(save_user);
            break;
        case 2://availabe user
            break;
        case 3://back
            return;
        default:
            break;
    }
    printf("user 2 is computer, wait ... :)\n");
    Sleep(1000);
    auto_map(&user2, 2, 0);
    int counter = 1;
    while (head1 != NULL && head2 != NULL) {
        while (counter == 1) {
            int x_map = 9;
            int y_map = 3;
            printf("\"%s\" it is your turn,     your score: %d  \n", user1.username, user1.score);
            draw(map2);
            printf("         S:save          E:exit");
            go_to(9, 3);
            //while (1)
            int row, column;
            int x_counter = 0, y_counter = 0;
            while (getch() == 224) {
                move_on_map(getch(), &x_counter, &y_counter, &x_map, &y_map, 3, 2 * i_len + 2, 8 * j_len - 1, 9);
            }
            row = 1 + y_counter;
            column = 1 + x_counter;
            if (row == i_len + 1) {
                if (column == 1) {
                    save_game(counter);
                    system("cls");
                    printf("saving ...");
                    Sleep(800);
                    system("cls");
                    continue;
                } else if (column == 3) {
                    exit(-1);
                } else {
                    system("cls");
                    continue;
                }
            } else {
                if (map2[row][column] != '-') {
                    system("cls");
                    continue;
                }
                if (user2.user_map[row][column] == 'S') {
                    map2[row][column] = 'X';
                    user1.score++;
                    check_to_remove(1, row, column);
                    system("cls");
                    if (head2 == NULL)
                        break;
                    continue;
                } else if (user2.user_map[row][column] == 'W') {
                    map2[row][column] = 'W';
                    smart_i = -1;
                    smart_j = -1;
                    system("cls");
                    printf("Wrong chice! :( %s ... see the %s map while turn changes :\n", user1.username,
                           user2.username);
                    draw(map2);
                    Sleep(1000);
                    system("cls");
                    counter = 2;
                    break;
                }
            }
        }
        while (counter == 2) {
            int row, column;
            printf("Computer turn! wait ...,    score : %d \n", user2.score);
            Sleep(800);
            while (1) {
                if (smart_i == -1 && smart_j == -1) {
                    row = rand() % i_len + 1;
                    column = rand() % j_len + 1;
                } else {
                    if (user1.user_map[smart_i][smart_j + 1] == 'S') {
                        row = smart_i;
                        column = (smart_j + 1) % (j_len + 1);
                    } else if (user1.user_map[smart_i][smart_j - 1] == 'S') {
                        row = smart_i;
                        column = (smart_j - 1) % (j_len + 1);
                    } else if (user1.user_map[smart_i + 1][smart_j] == 'S') {
                        row = (smart_i + 1) % (i_len + 1);
                        column = smart_j;
                    } else if (user1.user_map[smart_i - 1][smart_j] == 'S') {
                        row = (smart_i - 1) % (i_len + 1);
                        column = smart_j;
                    } else {
                        smart_i = -1;
                        smart_j = -1;
                        row = rand() % i_len + 1;
                        column = rand() % j_len + 1;
                    }
                }
                if (map1[row][column] != '-') {
                    smart_i = -1;
                    smart_j = -1;
                    continue;
                }
                if (user1.user_map[row][column] == 'S') {
                    map1[row][column] = 'X';
                    user2.score++;
                    check_to_remove(2, row, column);
                    smart_i = row;
                    smart_j = column;
                    //system("cls");
                    if (head2 == NULL)
                        break;
                    continue;
                } else if (user1.user_map[row][column] == 'W') {
                    map1[row][column] = 'W';
                    smart_i = -1;
                    smart_j = -1;
                    //system("cls");
                    //printf("Wrong chice! :( %s ... see the %s map while turn changes :\n", user1.username,user2.username);

                    system("cls");
                    counter = 1;
                    break;
                }
                break;
            }
            check_to_remove(2, row, column);
            printf("computer turn finished!      score :%d\n", user2.score);
            draw(map1);
            Sleep(800);
            printf("\npoint : <row : %d column : %d>\nenter to contiue!\n", row, column);
            getchar();
            fflush(stdin);
            system("cls");
            break;
        }
    }
    system("cls");
    if (head1 == NULL) {
        printf("| *** Computer Won! *** |");
        printf(" %s map :\n", user1.username);
        draw(map1);
        printf("\n");
        printf("Computer map :\n");
        draw(map2);
        printf("\nFinishing game ...");
        Sleep(3000);
    } else if (head2 == NULL) {
        printf("| *** %s Won! *** |", user1.username);
        printf(" Computer map :\n");
        draw(map2);
        printf("\n");
        printf(" %s map :\n", user1.username);
        draw(map1);
        printf("\nFinishing game ...");
        Sleep(3000);
    }
    empty_list(1);
    empty_list(2);
    empty_map(map1);
    empty_map(map2);
    system("cls");
    return;
}

void load_game(char map[MAX_i + 2][MAX_j + 2]) {
    static struct ships ships1, ships2;
    FILE *game_loader;
    char *address[100];
    char load_address[203] = "d:\\SAVE_GAMES\\";
    int load_nums;
    load_nums = number_of_savegames;
    if (load_nums == 0) {
        printf("NO GAMES TO LOAD!");
        Sleep(1000);
        return;
    }
    for (int i = 0; i < 100; i++) {
        address[i] = (char *) malloc(sizeof(char) * 203);
    }
    save_list = fopen("d:\\SAVE_GAMES\\save_lists.bin", "r");

    for (int i = 0; i < load_nums; i++) {
        fread(address[i], sizeof(char), 203, save_list);
    }
    fclose(save_list);
    printf("GAMES TO LOAD :\n");
    for (int i = 0; i < load_nums; i++) {
        printf("-%s\n", address[i]);
    }
    int y_load_menu = 1;
    go_to(0, 1);
    while (getch() == 224) {
        move_on_menu(getch(), &y_load_menu, 1, load_nums);
    }
    strcat(address[y_load_menu - 1], ".bin");
    strcat(load_address, address[y_load_menu - 1]);
    game_loader = fopen(load_address, "rb");
    if (game_loader == NULL) {
        system("cls");
        printf("Couldnt load game!");
        Sleep(500);
        return;
    }
    int counter;
    fread(&i_len, sizeof(int), 1, game_loader);
    fread(&j_len, sizeof(int), 1, game_loader);
    fread(&counter, sizeof(int), 1, game_loader);
    fread(&ships_mode, sizeof(struct ship_setting), 1, game_loader);
    fread(&ships1, sizeof(struct ships), 1, game_loader);
    fread(&ships2, sizeof(struct ships), 1, game_loader);
    fread(&user1, sizeof(struct USER), 1, game_loader);
    fread(&user2, sizeof(struct USER), 1, game_loader);
    fread(map1, sizeof(char), 196, game_loader);
    fread(map2, sizeof(char), 196, game_loader);
    fclose(game_loader);
    head1 = &ships1;
    head2 = &ships2;
    printf("len : %d jahat : %c start r and c : %d %d--oh oh :", head1->length_of_ship, head1->orientation,
           head1->start.row, head1->start.column, head1->next->length_of_ship);
    getchar();
    system("cls");
    //printf("%d %d",i_len,j_len);
    //printf("user1.username : %s",user1.username);
    //printf("\nuser2.username : %s\n",user2.username);
    //printf("user 1:\n");
    //draw(user1.user_map);
    //printf("user2 :\n");
    //draw(user2.user_map);
    //draw(map1);
    //draw(map2);
    //getchar();
    system("cls");
    play_game(counter);
    //main_menu(map);
    return;
}

void save_game(int turn) {
    number_of_savegames++;
    struct ships *to_save;
    to_save = NULL;
    FILE *SAVER;
    char address1[120] = "d:\\SAVE_GAMES\\";
    char address2[80];
    system("cls");
    printf("enter save name:\n");
    gets(address2);
    //add the save name in the list :
    save_list = fopen("d:\\SAVE_GAMES\\save_lists.bin", "a+b");
    fwrite(address2, sizeof(char), 203, save_list);
    fclose(save_list);
    // ------------------------------

    //saving number of saved games :
    save_nums = fopen("d:\\SAVE_GAMES\\save_num.bin", "wb");
    fwrite(&number_of_savegames, sizeof(int), 1, save_nums);
    fclose(save_nums);
    //---------------------------------
    strcat(address2, ".bin");
    strcat(address1, address2);
    SAVER = fopen(address1, "wb");
    if (SAVER == NULL) {
        system("cls");
        printf("couldn't save");
        return;
    }
    fwrite(&i_len, sizeof(int), 1, SAVER);
    fwrite(&j_len, sizeof(int), 1, SAVER);
    fwrite(&turn, sizeof(int), 1, SAVER);
    fwrite(&ships_mode, sizeof(struct ship_setting), 1, SAVER);
    fwrite(&user1, sizeof(struct USER), 1, SAVER);
    fwrite(&user2, sizeof(struct USER), 1, SAVER);
    fwrite(map1, sizeof(char), 196, SAVER);
    fwrite(map2, sizeof(char), 196, SAVER);
    to_save = head1;
    do {
        fwrite(to_save, sizeof(struct ships), 1, SAVER);
        to_save = to_save->next;
    } while (to_save == NULL);
    to_save = head2;
    do {
        fwrite(to_save, sizeof(struct ships), 1, SAVER);
        to_save = to_save->next;
    } while (to_save == NULL);
    /*fprintf(SAVER, "user 1 : %s\n", user1.username);
    for (int i = 1; i < i_len + 1; i++) {
        for (int j = 1; j < j_len + 1; j++) {
            fprintf(SAVER, "%c ", user1.user_map[i][j]);
        }
        fprintf(SAVER, "\n");
    }
    fprintf(SAVER, "struct ships1 :\n");
    int num = 1;
    while (1) {
        fprintf(SAVER, "num : %d\n", num);
        fprintf(SAVER, "to_print->orientation : %c\n", to_print->orientation);
        fprintf(SAVER, "to_print->length_of_ship : %d\n", to_print->length_of_ship);
        fprintf(SAVER, "to_print->start.row ship : %d\n", to_print->start.row);
        fprintf(SAVER, "to_print->start.column ship : %d\n", to_print->start.column);
        fprintf(SAVER, "to_print->end.row ship : %d\n", to_print->end.row);
        fprintf(SAVER, "to_print->end.column ship : %d\n", to_print->end.column);
        num++;
        if (to_print->next != NULL)
            to_print = to_print->next;
        else break;
    }
    fprintf(SAVER, "user 2 : %s\n", user2.username);
    for (int i = 1; i < i_len + 1; i++) {
        for (int j = 1; j < j_len + 1; j++) {
            fprintf(SAVER, "%c ", user2.user_map[i][j]);
        }
        fprintf(SAVER, "\n");
    }
    num = 1;
    to_print = head2;
    fprintf(SAVER, "struct ships2 :\n");
    //int i = 0;
    while (1) {
        fprintf(SAVER, "num : %d\n", num);
        fprintf(SAVER, "to_print->orientation : %c\n", to_print->orientation);
        fprintf(SAVER, "to_print->length_of_ship : %d\n", to_print->length_of_ship);
        fprintf(SAVER, "to_print->start.row ship : %d\n", to_print->start.row);
        fprintf(SAVER, "to_print->start.column ship : %d\n", to_print->start.column);
        fprintf(SAVER, "to_print->end.row ship : %d\n", to_print->end.row);
        fprintf(SAVER, "to_print->end.column ship : %d\n", to_print->end.column);
        num++;
        if (to_print->next != NULL)
            to_print = to_print->next;
        else break;
    }*/
    fclose(SAVER);
    return;
}

void settings(char map[MAX_i + 2][MAX_j + 2]) {
    while (1) {
        int i_new_map = 0;
        int j_new_map = 0;
        char ok;
        int y_setting_menu = 1;
        int y_mode_menu = 1;
        for (int i = 0; i < 4; i++)
            printf("%s\n", settings_menu[i]);
        go_to(0, 1);
        while (getch() == 224) {
            move_on_menu(getch(), &y_setting_menu, 1, 3);
        }
        system("cls");
        go_to(0, 0);
        switch (y_setting_menu) {
            case 1 : // map
                while (1) {
                    while (1) {
                        printf("Enter new number of columns of Map (a number betwen 8 and 12) : ");
                        scanf("%d", &j_new_map);
                        getchar();
                        fflush(stdin);
                        if (j_new_map > 12 || j_new_map < 8) {
                            printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                            continue;
                        }
                        j_len = j_new_map;
                        break;
                    }
                    while (1) {
                        printf("Enter new number of row of Map (a number betwen 8 and 12) : ");
                        scanf("%d", &i_new_map);
                        getchar();
                        fflush(stdin);
                        if (i_new_map > 12 || i_new_map < 8) {
                            printf("*** WRONG INPUT, PLEASE TRY AGAIN ***\n");
                            continue;
                        }
                        i_len = i_new_map;
                        break;
                    }
                    printf("You Sure about the changes? : Enter Y for YES to continue or N for NO to enter new inputs : ");
                    scanf("%c", &ok);
                    getchar();
                    fflush(stdin);
                    ok = toupper(ok);
                    if (ok == 'Y') {
                        empty_map(user1.user_map);
                        empty_map(user2.user_map);
                        empty_map(map1);
                        empty_map(map2);
                        printf("\nSaving...");
                        Sleep(1000);
                        break;
                        break;
                    } else {
                        system("cls");
                        continue;
                    }
                }
                system("cls");
                continue;
            case 2 : // ships
                while (1) {
                    while (1) {
                        for (int i = 0; i < 5; i++)
                            printf("%s\n", game_modes[i]);
                        go_to(0, 1);
                        while (getch() == 224) {
                            move_on_menu(getch(), &y_mode_menu, 1, 4);
                        }
                        system("cls");
                        if (y_mode_menu <= 3) {
                            printf("You Sure about the changes? : Enter Y for YES to continue or N for NO to enter new inputs : ");
                            scanf("%c", &ok);
                            getchar();
                            fflush(stdin);
                            ok = toupper(ok);
                            if (ok == 'Y') {
                                printf("\nSaving...");
                                Sleep(1000);
                                break;
                            } else {
                                y_mode_menu = 1;
                                system("cls");
                                continue;
                            }
                        } else {
                            break;
                        }
                    }
                    switch (y_mode_menu) {
                        case 1 :
                            game_mode(1);
                            empty_map(user1.user_map);
                            empty_map(user2.user_map);
                            empty_map(map1);
                            empty_map(map2);
                            system("cls");
                            break;
                        case 2 :
                            game_mode(2);
                            empty_map(user1.user_map);
                            empty_map(user2.user_map);
                            empty_map(map1);
                            empty_map(map2);
                            system("cls");
                            break;
                        case 3 :
                            game_mode(3);
                            empty_map(user1.user_map);
                            empty_map(user2.user_map);
                            empty_map(map1);
                            empty_map(map2);
                            system("cls");
                            break;
                        case 4 :
                            system("cls");
                            break;
                        default:
                            break;

                    }
                    break;
                }
                break;
            case 3 : // back
                return;
            default:
                break;

        }

    }
}

void game_mode(int mode) {
    switch (mode) {
        case 1 :
            ships_mode.number_of_ships = 10;
            ships_mode.five_num = 1;
            ships_mode.three_num = 2;
            ships_mode.two_num = 3;
            ships_mode.one_num = 4;
            break;
        case 2 :
            ships_mode.number_of_ships = 8;
            ships_mode.five_num = 1;
            ships_mode.three_num = 2;
            ships_mode.two_num = 2;
            ships_mode.one_num = 3;
            break;
        case 3:
            ships_mode.number_of_ships = 13;
            ships_mode.five_num = 2;
            ships_mode.three_num = 3;
            ships_mode.two_num = 3;
            ships_mode.one_num = 5;
            break;
        default:
            break;

    }
}

void draw(char map[MAX_i + 2][MAX_j + 2]) {
    printf("%2c   ", ' ');//3 spaces
    for (int j = 1; j <= j_len; j++) {
        printf("   %2d   ", j); //3space -<2> 3 spaces
    }
    printf("\n");
    printf("%2c   ", ' '); // 3spaces - 5 chars
    for (int j = 0; j <= j_len; j++) {
        if (j < j_len) printf("--------"); // 8char
        else printf("-");
    }
    printf("\n");
    for (int i = 1; i < i_len + 1; i++) {
        printf("%2d   ", i); // 3 spaces
        for (int j = 1; j <= j_len + 1; j++) {
            if (j < j_len + 1) {
                printf("|");
                if (map[i][j] == 'W') {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_INTENSITY);
                    printf("  %2c   ", map[i][j]); // 1char 2spaces-<2> 3spaces : 8
                } else if (map[i][j] == 'S') {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                    printf("  %2c   ", map[i][j]);
                } else if (map[i][j] == 'X') {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_INTENSITY);
                    printf("  %2c   ", map[i][j]);
                } else {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                            BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
                    printf("  %2c   ", map[i][j]);
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                        BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
            } else {
                printf("|");
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                    BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
        }
        printf("\n");
        printf("%2c   ", ' ');
        for (int j = 1; j <= j_len + 1; j++) {
            if (j < j_len + 1) printf("--------"); // 8char
            else printf("-");
        }
        printf("\n");
    }
}

void empty_map(char map[MAX_i + 2][MAX_j + 2]) {
    for (int i = 0; i < i_len + 2; i++) {
        for (int j = 0; j < j_len + 2; j++) {
            if ((i == 0) || (i == i_len + 1) || (j == 0) || (j == j_len + 1))
                map[i][j] = '0';
            else map[i][j] = '-';
        }
    }
}

void empty_list(int user_num) {
    struct ships *cur, *next;
    if (user_num == 1) cur = head1;
    else cur = head2;
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    if (user_num == 1) head1 = NULL;
    else head2 = NULL;
}

void go_to(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void to_order(int *x1, int *x2) {
    int temp = 0;
    if (*x1 > *x2) {
        temp = *x2;
        *x2 = *x1;
        *x1 = temp;
    } else {
        return;
    }
}
/*
user1 :
H
1
2 6
Y
V
1
3 5
Y
V
10
1 3
Y
H
10
1 2
Y
H
10
5 6
Y
V
8
7 8
Y
5 5
Y
10 8
Y
3 8
Y
8 4
Y
user2 :
H
1
2 6
Y
V
1
3 5
Y
V
10
1 3
Y
H
10
1 2
Y
H
10
5 6
Y
V
8
7 8
Y
5 5
Y
10 6
Y
8 1
Y
7 3
Y

*/
