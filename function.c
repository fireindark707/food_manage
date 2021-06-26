#include "function.h"
#define MAX 10

void pause()
{
    printf("請輸入任意字符後按Enter...");
    char c;
    scanf(" %c", &c);
}

void clear_screen()
{
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

int input_food(int num, fd stock[200])
{
    char food_type[5][20] = {"零食", "肉", "蔬菜", "主食", "水果"};
    int type, fresh_days;
    char name[50], buff[50];
    int add_qty = 0;
    printf("名稱：");
    scanf("%s", name);
    for (int i = 0; i < num; i++)
    {
        if (strncmp(stock[i].Name, name, sizeof(name)) == 0)
        {
            printf("發現已有食物的名稱相同，是否新增數量即可？\n");
            printf("\n  %d：%s %s %d%s %s\n\n", i + 1, stock[i].Name, stock[i].Type, stock[i].Quantity, stock[i].Unit, stock[i].Fresh_date);
            printf("輸入0來額外新增單獨項目，輸入其他數字來增加數量：");
            scanf(" %d", &add_qty);
            if (add_qty == 0)
            {
                continue;
            }
            else if (add_qty < 0)
            {
                puts("請不要使用此功能來消耗食物");
            }
            else if (add_qty > 0)
            {
                stock[i].Quantity += add_qty;
                return num;
            }
        }
    }
    strcpy(stock[num].Name, name);
    printf("類別（輸入數字：0零食|1肉|2蔬菜|3主食|4水果）：");
    scanf("%d", &type);
    memcpy(stock[num].Type, food_type[type], strlen(food_type[type]) + 1);
    printf("數量（為了簡化，僅支持整數）：");
    scanf("%d", &stock[num].Quantity);
    printf("單位：");
    scanf("%s", stock[num].Unit);
    printf("剩餘保質天數（估算整數即可，系統會自動轉換為日期）：");
    scanf("%d", &fresh_days);
    strcpy(stock[num].Fresh_date, format_date(day_add(fresh_days), buff));
    puts("=============");
    return num + 1;
}

void print_food(int num, fd stock[200])
{
    int warning = 0, warning_index[200];
    puts("▣==============================================================▣");
    char title[5][20] = {"名稱", "類別", "數量", "單位", "保質期(天數)"};
    printf("序號 \t %*s\t%*s \t  %*s \t %*s \t %*s\n", -MAX, title[0], -5, title[1], 5, title[2], -5, title[3], -MAX, title[4]);
    for (int i = 0; i < num; i++)
    {
        printf("%*d \t %*s\t%*s \t %*d \t  %*s \t %*s(%d)\n", -5, i + 1, -MAX, stock[i].Name, -5, stock[i].Type, 5, stock[i].Quantity, -5, stock[i].Unit, -MAX, stock[i].Fresh_date, day_diff(stock[i].Fresh_date));
        if (day_diff(stock[i].Fresh_date) <= 2)
        {
            warning_index[warning] = i;
            warning += 1;
        }
    }
    if (warning > 0)
    {
        printf("▣--------------------------------------------------------------▣\n");
        printf("【提醒】");
        for (int i = 0; i < warning; i++)
        {
            if (day_diff(stock[warning_index[i]].Fresh_date) >= 0)
            {
                printf("%s(%d)", stock[warning_index[i]].Name, warning_index[i] + 1);
            }
        }
        printf("的保質天數已經少於2天了，請加速食用。\n");
        for (int i = 0; i < warning; i++)
        {
            if (day_diff(stock[warning_index[i]].Fresh_date) < 0)
            {
                printf("【警告】%s(%d)已經過期了！\n", stock[warning_index[i]].Name, warning_index[i] + 1);
            }
        }
    }
    printf("▣==============================================================▣\n");
}

void write_food(char tsv_path[100], int num, FILE *FD_F, fd stock[200])
{
    FD_F = fopen(tsv_path, "w");
    fprintf(FD_F, "名稱\t類別\t數量\t單位\t保質期\n");
    for (int i = 0; i < num; i++)
    {
        if (stock[i].Quantity == 0)
        {
            continue;
        }
        fprintf(FD_F, "%s\t%s\t%d\t%s\t%s\n", stock[i].Name, stock[i].Type, stock[i].Quantity, stock[i].Unit, stock[i].Fresh_date);
    }
    fclose(FD_F);
}

int read_food(char tsv_path[100], FILE *FD_F, fd stock[200])
{
    int num = 0;
    FD_F = fopen(tsv_path, "r");
    if (FD_F == NULL)
    {
        return num;
    }
    else
    {
        fscanf(FD_F, "%*[^\n]");
        while (1)
        {
            fscanf(FD_F, "%s\t%s\t%d\t%s\t%s ", stock[num].Name, stock[num].Type, &stock[num].Quantity, stock[num].Unit, stock[num].Fresh_date);
            if (stock[num].Quantity <= 0)
            {
                return 0;
            }
            num += 1;
            if (feof(FD_F))
            {
                break;
            }
        }
    }
    fclose(FD_F);
    return num;
}

int delete_food(int num, fd stock[200])
{
    int delete_index;
    int delete_quantity;
    printf("請輸入要消耗的食物序號：");
    scanf("%d", &delete_index);
    delete_index -= 1;
    if (delete_index >= num || delete_index < 0)
    {
        puts("找不到該序號的食物");
    }
    else
    {
        printf("序號%d的食物%s目前有%d%s\n", delete_index + 1, stock[delete_index].Name, stock[delete_index].Quantity, stock[delete_index].Unit);
        printf("請輸入要消耗的數量：");
        scanf("%d", &delete_quantity);
        if (stock[delete_index].Quantity <= delete_quantity)
        {
            for (int i = 0; i < num - 1; i++)
            {
                if (i >= delete_index)
                {
                    stock[i] = stock[i + 1];
                }
            }
            num -= 1;
        }
        else
        {
            stock[delete_index].Quantity -= delete_quantity;
        }
    }
    return num;
}

void edit_food(int num, fd stock[200])
{
    int edit_index = 0;
    char n, buff[50];
    char food_type[5][20] = {"零食", "肉", "蔬菜", "主食", "水果"};
    int type, fresh_days;
    printf("請輸入要修改的食物序號：");
    scanf("%d", &edit_index);
    edit_index -= 1;
    if (edit_index >= num || edit_index < 0)
    {
        printf("輸入序號錯誤");
        pause();
        return;
    }
    printf("請問要修改[%d-%s]的哪一個欄位？\n  1名稱\n  2類型\n  3數量\n  4單位\n  5保質期\n輸入對應數字：", edit_index + 1, stock[edit_index].Name);
    scanf(" %c", &n);
    switch (n)
    {
    case '1':
        printf("現在的名稱為「%s」,請輸入新名稱：", stock[edit_index].Name);
        scanf("%s", stock[edit_index].Name);
        break;
    case '2':
        printf("現在的類別為「%s」,請輸入新類別（數字：0零食|1肉|2蔬菜|3主食|4水果）：", stock[edit_index].Type);
        scanf("%d", &type);
        memcpy(stock[edit_index].Type, food_type[type], strlen(food_type[type]) + 1);
        break;
    case '3':
        printf("現在的數量為「%d」,請輸入新數量：", stock[edit_index].Quantity);
        scanf("%d", &stock[edit_index].Quantity);
        break;
    case '4':
        printf("現在的單位為「%s」,請輸入新單位：", stock[edit_index].Unit);
        scanf("%s", stock[edit_index].Unit);
        break;
    case '5':
        printf("現在的保質期為「%s」,請輸入剩餘保質【天數】：", stock[edit_index].Fresh_date);
        scanf(" %d", &fresh_days);
        strcpy(stock[edit_index].Fresh_date, format_date(day_add(fresh_days), buff));
        break;
    default:
        printf("輸入編號錯誤");
        pause();
        break;
    }
}

char *now(char *datetime)
{
    time_t timep;
    struct tm *p;

    time(&timep);
    p = localtime(&timep);
    sprintf(datetime, "%4d-%02d-%02d %02d:%02d:%02d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
    return datetime;
}

char *format_date(time_t timep, char *datetime)
{
    struct tm *p;
    p = localtime(&timep);
    sprintf(datetime, "%4d-%02d-%02d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
    return datetime; //生成格式化日期
}

time_t day_add(int n)
{
    time_t timep;
    time(&timep);
    timep += 24 * 60 * 60 * n;
    return timep; //返回秒數
}

int day_diff(char Fresh_date[20])
{
    char date[20];
    time_t now = day_add(0);
    struct tm *date_now = localtime(&now); //目前的格式化日期
    char *pch;                             //切string用的
    int date_int[3];                       //日期[年，月，日]

    strcpy(date, Fresh_date);
    pch = strtok(date, "-");
    for (int i = 0; i < 3; i++)
    {
        date_int[i] = atoi(pch);
        pch = strtok(NULL, "-");
    }

    date_now->tm_year = date_int[0] - 1900;
    date_now->tm_mon = date_int[1] - 1;
    date_now->tm_mday = date_int[2];

    time_t end = mktime(date_now);
    return (int)(end - now) / (60 * 60 * 24);
}