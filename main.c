#include "function.h"
int main(int argc, char **argv)
{
    char op;
    fd stock[200];
    int num = 0; //標記stock數量
    int type;
    int ret;
    char pass[20]; //確認用
    char reminder[100] = ">> 歡迎使用個人食物庫存管理系統v0.3(作者：張郁) <<";
    char tsv_path[100];
    char backup_name[100];
    char example_date[6][20];
    char buf[100]; //時間用存儲器

    FILE *FD_F;
    strcpy(tsv_path, argv[0]);
    strcat(tsv_path, ".tsv");
    strcpy(backup_name, argv[0]);
    strcat(backup_name, now(buf));
    strcat(backup_name, "_backup.tsv");

    while (1)
    {
        clear_screen();
        puts("\n【功能菜單】");
        puts("a 新增食物（入庫）");
        puts("r 消耗食物（出庫）");
        puts("g 打造健康食物庫（生成一個範例）");
        puts("e 修改一項食物");
        puts("d 清空所有食物");
        puts("q 離開（請勿使用左上角X或Ctrl + C方式退出）");
        puts("");
        //開啟之後先嘗試讀檔
        num = read_food(tsv_path, FD_F, stock);
        if (num >= 1)
        {
            printf("【food.tsv食物庫內容】\n");
            print_food(num, stock);
            puts("");
        }
        else
        {
            remove(tsv_path);
            puts("目前尚無任何食物，請新增\n");
        }
        printf("【消息區】%s\n\n", reminder);
        printf("請輸入命令對應的字母（例如a）> ");
        scanf(" %c", &op);

        switch (op)
        {
        case 'a':
            clear_screen();
            printf("正在添加第%d筆\n", num + 1);
            num = input_food(num, stock);
            write_food(tsv_path, num, FD_F, stock);
            strcpy(reminder, "食物加入完成並已寫入food.tsv");
            break;
        case 'r':
            num = delete_food(num, stock);
            if (num == 0)
            {
                remove(tsv_path);
                strcpy(reminder, "無剩餘食物，刪除food.tsv");
            }
            else
            {
                write_food(tsv_path, num, FD_F, stock);
                strcpy(reminder, "食物消耗完成並已更新food.tsv");
            }
            break;
        case 'g':
            if (num == 0)
            {
                FD_F = fopen(tsv_path, "w");
                strcpy(example_date[0], format_date(day_add(10), buf));
                strcpy(example_date[1], format_date(day_add(30), buf));
                strcpy(example_date[2], format_date(day_add(1), buf));
                strcpy(example_date[3], format_date(day_add(21), buf));
                strcpy(example_date[4], format_date(day_add(60), buf));
                strcpy(example_date[5], format_date(day_add(7), buf));
                fprintf(FD_F, "名稱\t類別\t數量\t單位\t保質期\n雞腿\t肉\t3\t隻\t%s\n豬肉水餃\t主食\t3\t包\t%s\n青菜\t蔬菜\t6\t包\t%s\n竹莢魚\t肉\t2\t片\t%s\n泰國香米\t主食\t1\t公斤\t%s\n金針菇\t蔬菜\t1\t包\t%s\n", example_date[0], example_date[1], example_date[2], example_date[3], example_date[4], example_date[5]);
                fclose(FD_F);
                strcpy(reminder, "已生成food.tsv為範例");
            }
            else
            {
                strcpy(reminder, "食物庫內仍有食物，必須完全清空才可生成範例");
            }
            break;
        case 'd':
            puts("請問你真的要清空所有食物嗎？");
            printf("輸入「清空」來確認，輸入其他字符來取消：");
            scanf(" %s", pass);
            if (strncmp(pass, "清空", sizeof("清空")) == 0)
            {
                ret = remove(tsv_path);
                if (ret == 0)
                {
                    strcpy(reminder, "食物已完全清空");
                }
                else
                {
                    strcpy(reminder, "清空失敗");
                }
            }
            else
            {
                strcpy(reminder, "清空被取消");
            }
            break;
        case 'e':
            edit_food(num, stock);
            write_food(tsv_path, num, FD_F, stock);
            strcpy(reminder, "修改已完成");
            break;
        case 'q':
            clear_screen();
            write_food(backup_name, num, FD_F, stock);
            printf("已存儲備份檔案food%s_backup.tsv\n若食物庫出錯需恢復僅需將對應tsv檔案更改為food.tsv即可\n", now(buf));
            puts("\n感謝使用～～～\n");
            return 0;
            break;

        default:
            strcpy(reminder, "輸入指令錯誤，請重新輸入");
            break;
        }
        clear_screen();
    }
    return 0;
}
