#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVENTORY_SIZE 10

int main() {
    system("chcp 65001 > nul");

    char farmer_name[32];
    printf("Введите имя вашего фермера: ");
    scanf("%31s", farmer_name);

    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 0, 4, 5, 0, 7, 1, 9 };

    //двумерный массив для названий из файла items.txt
    char item_names[10][32] = {
        {"Пусто"}, {"Дерево"}, {"Камень"}, {"Семена"}, {"Пшеница"},
        {"Яблоко"}, {"Лопата"}, {"Вода"}, {"Удобрение"}, {"Золото"}
    };

    // Чтение items.txt 
    FILE* f = fopen("items.txt", "r");
    if (f != NULL) {
        int id;
        while (fscanf(f, "%d", &id) == 1) {
            if (id < 0 || id >= 10) {
                char tmp[64];
                if (fscanf(f, "%63s", tmp) != 1) break;
                continue;
            }
            if (fscanf(f, "%31s", item_names[id]) != 1) break;
        }
        fclose(f);
    }

    int choice = -1;

    while (1) {
        printf("\n=== ВЕСЕЛЫЙ ФЕРМЕР (Имя: %s) ===\n", farmer_name);
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Ревизия ресурсов\n");
        printf("[7] Поиск предмета по названию\n");
        printf("[8] Записать состояние в дневник (diary.txt)\n");
        printf("[0] Выход\n");
        printf("Выберите пункт: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка!\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 0:
            return 0;

        case 1:
            printf("День %d, %02d:00\n", current_day, current_hour);
            break;

        case 2: {
            int hours;
            printf("Сколько часов поработать? ");
            scanf("%d", &hours);
            current_hour += hours;
            if (current_hour >= 24) {
                current_day += current_hour / 24;
                current_hour %= 24;
            }
            break;
        }

        case 3:
            printf("--- Инвентарь ---\n");
            for (int i = 0; i < INVENTORY_SIZE; i++) {
                printf("Слот %d: [%d] (%s)\n", i, inventory[i], item_names[inventory[i]]);
            }
            break;

        case 4: {
            int slot, id;
            printf("Слот (0-9): ");
            scanf("%d", &slot);
            printf("ID предмета (0-9): ");
            scanf("%d", &id);
            if (slot >= 0 && slot < INVENTORY_SIZE) inventory[slot] = id;
            break;
        }

        case 5: {
            int slot;
            printf("Слот для очистки (0-9): ");
            scanf("%d", &slot);
            if (slot >= 0 && slot < INVENTORY_SIZE) inventory[slot] = 0;
            break;
        }

        case 6: {
            int target, count = 0;
            printf("ID предмета для поиска: ");
            scanf("%d", &target);
            printf("Слоты: ");
            for (int i = 0; i < INVENTORY_SIZE; i++) {
                if (inventory[i] == target) {
                    printf("%d ", i);
                    count++;
                }
            }
            printf("\nВсего: %d\n", count);
            break;
        }

              // ПУНКТ 7: Поиск по названию 
        case 7: {
            char query[32];
            printf("Введите название для поиска: ");
            scanf("%s", query);

            int found_id = -1;
            for (int i = 0; i < 10; i++) {
                if (strcmp(item_names[i], query) == 0) {
                    found_id = i;
                    break;
                }
            }

            if (found_id == -1) {
                printf("Предмет не найден в каталоге!\n");
                break;
            }

            printf("Найден в слотах: ");
            int c = 0;
            for (int i = 0; i < INVENTORY_SIZE; i++) {
                if (inventory[i] == found_id) {
                    printf("%d ", i);
                    c++;
                }
            }
            printf("(Всего: %d)\n", c);
            break;
        }

              // ПУНКТ 8: Запись в дневник 
        case 8: {
            FILE* df = fopen("diary.txt", "a");
            if (df != NULL) {
                fprintf(df, "Фермер: %s | День: %d, Время: %02d:00\n", farmer_name, current_day, current_hour);
                fprintf(df, "Инвентарь: ");
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    fprintf(df, "[%d:%s] ", inventory[i], item_names[inventory[i]]);
                }
                fprintf(df, "\n-------------------\n");
                fclose(df);
                printf("Записано в diary.txt!\n");
            }
            break;
        }

        default:
            printf("Неверный пункт!\n");
        }
    }
    return 0;
}