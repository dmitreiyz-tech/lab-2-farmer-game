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

    int current_day = 3;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 0, 4, 5, 0, 7, 1, 9 };

    char item_names[10][32] = {
        {"Пусто"}, {"Дерево"}, {"Камень"}, {"Семена"}, {"Пшеница"},
        {"Яблоко"}, {"Лопата"}, {"Вода"}, {"Удобрение"}, {"Золото"}
    };

    FILE* f = fopen("items.txt", "r");
    if (f != NULL) {
        int id;
        char name[32];
        while (fscanf(f, "%d %31s", &id, name) == 2) {
            if (id >= 0 && id < 10) {
                strncpy(item_names[id], name, sizeof(item_names[id]) - 1);
                item_names[id][sizeof(item_names[id]) - 1] = '\0';
            }
        }
        fclose(f);
    }

    int choice = -1;

    while (1) {
        printf("\n=== ВЕСЕЛЫЙ ФЕРМЕР (Имя: %s) [День: %d] ===\n", farmer_name, current_day);
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Ревизия ресурсов\n");
        printf("[7] Поиск предмета по названию\n");
        printf("[8] Записать состояние в дневник (diary.txt)\n");
        printf("[9] Проверка рецептов крафта\n");
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

        case 8: {
            FILE* df = fopen("diary.txt", "a");
            if (df != NULL) {
                // 1. Формируем строку и записываем в файл diary.txt
                fprintf(df, "Фермер: %s | День: %d, Время: %02d:00\n", farmer_name, current_day, current_hour);
                fprintf(df, "Инвентарь: ");
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    fprintf(df, "[%d:%s] ", inventory[i], item_names[inventory[i]]);
                }
                fprintf(df, "\n-------------------\n");
                fclose(df);

                // 2. Дублируем вывод на экран 
                printf("\n--- Запись в дневнике (diary.txt) ---\n");
                printf("Фермер: %s | День: %d, Время: %02d:00\n", farmer_name, current_day, current_hour);
                printf("Инвентарь: ");
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    printf("[%d:%s] ", inventory[i], item_names[inventory[i]]);
                }
                printf("\n-----------------------------------\n");
                printf("Успешно записано в diary.txt и выведено на экран!\n");
            }
            else {
                printf("Ошибка открытия файла diary.txt!\n");
            }
            break;
        }

              // ПУНКТ 9: Проверка рецептов крафта
        case 9: {
            FILE* in = fopen("input.txt", "r");
            FILE* out = fopen("output.txt", "w");
            if (in == NULL || out == NULL) {
                printf("Ошибка открытия файлов input.txt или output.txt!\n");
                if (in) fclose(in);
                if (out) fclose(out);
                break;
            }

            // Считаем количество каждого предмета в инвентаре игрока
            int inv_counts[10] = { 0 };
            for (int i = 0; i < INVENTORY_SIZE; i++) {
                int id = inventory[i];
                if (id >= 0 && id < 10) {
                    inv_counts[id]++;
                }
            }

            char line[256];
            fprintf(out, "=== РЕЗУЛЬТАТЫ ПРОВЕРКИ РЕЦЕПТОВ КРАФТА ===\n");

            while (fgets(line, sizeof(line), in) != NULL) {
                line[strcspn(line, "\r\n")] = 0;
                if (strlen(line) == 0) continue;

                char recipe_name[64];
                char* colon = strchr(line, ':');
                if (colon == NULL) continue;

                int name_len = (int)(colon - line);
                if (name_len >= sizeof(recipe_name)) name_len = sizeof(recipe_name) - 1;
                strncpy(recipe_name, line, name_len);
                recipe_name[name_len] = '\0';

                // Считаем требуемое количество ресурсов для рецепта
                int req_counts[10] = { 0 };
                char* ptr = colon + 1;
                int req_id;
                while (sscanf(ptr, "%d", &req_id) == 1) {
                    if (req_id >= 0 && req_id < 10) {
                        req_counts[req_id]++;
                    }
                    while (*ptr != '\0' && *ptr != ' ' && *ptr != '\t') ptr++;
                    while (*ptr == ' ' || *ptr == '\t') ptr++;
                }

                int possible = 1;
                int missing_id = -1;
                for (int i = 0; i < 10; i++) {
                    if (req_counts[i] > inv_counts[i]) {
                        possible = 0;
                        missing_id = i;
                        break;
                    }
                }

                // Вывод результата в output.txt
                if (possible) {
                    fprintf(out, "Рецепт \"%s\": [ДОСТУПНО]\n", recipe_name);
                }
                else {
                    const char* miss_name = (missing_id >= 0 && missing_id < 10) ? item_names[missing_id] : "Неизвестно";
                    fprintf(out, "Рецепт \"%s\": [НЕ ХВАТАЕТ РЕСУРСОВ] (не хватает: %s)\n", recipe_name, miss_name);
                }
            }

            fclose(in);
            fclose(out);
            printf("Проверка рецептов завершена! Результаты записаны в output.txt.\n");
            break;
        }

        default:
            printf("Неверный пункт!\n");
        }
    }
    return 0;
}