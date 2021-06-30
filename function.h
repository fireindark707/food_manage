#ifndef FUNCTION_H
#define FUNCTION_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct Food
{
    char Name[50];
    char Type[50];
    int Quantity;
    char Unit[20];
    char Fresh_date[20];
};

typedef struct Food fd;

void clear_screen();
void pause();
int input_food(int num, fd stock[200], char food_type[50][50], int category_num);
void print_food(int num, fd stock[200]);
void write_food(char tsv_path[100], int num, FILE *FD_F, fd stock[200]);
int read_food(char tsv_path[100], FILE *FD_F, fd stock[200]);
void edit_food(int num, fd stock[200], char food_type[50][50], int category_num);
int delete_food(int num, fd stock[200]);
char *now(char *datetime);
char *format_date(time_t timep, char *datetime);
time_t day_add(int n);
int day_diff(char *date);
int read_category(char food_type[50][50], char *c_path, FILE *FD_F);
void create_category(char *c_path, FILE *FD_F);
void save_category(char *c_path, FILE *FD_F, int category_num, char food_type[50][50]);
int edit_category(int category_num, char food_type[50][50]);
#endif