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
        printf("[9] Доска объявлений: проверка квестов\n");
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

              // ПУНКТ 9: Доска объявлений 
        case 9: {
            FILE* in = fopen("input.txt", "r");
            FILE* out = fopen("output.txt", "w");
            if (in == NULL || out == NULL) {
                printf("Ошибка открытия файлов input.txt или output.txt!\n");
                if (in) fclose(in);
                if (out) fclose(out);
                break;
            }

            char line[128];
            fprintf(out, "=== ДОСКА ОБЪЯВЛЕНИЙ (АКТИВНЫЕ КВЕСТЫ) ===\n");

            while (fgets(line, sizeof(line), in) != NULL) {
                int quest_day;
                char customer[32];
                char req_item[32];
                int reward;

                // Читаем строку формата: День | Заказчик | Предмет | Награда
                if (sscanf(line, "%d | %31[^|] | %31[^|] | %d", &quest_day, customer, req_item, &reward) == 4) {

                  
                  
                    char* p = customer + strlen(customer) - 1;
                    while (p >= customer && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')) *p-- = '\0';

                    p = req_item + strlen(req_item) - 1;
                    while (p >= req_item && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')) *p-- = '\0';
                    char* clean_item = req_item;
                    while (*clean_item == ' ') clean_item++;

                    // 1. Фильтрация по дню 
                    if (quest_day <= current_day) {

                        // Находим ID требуемого предмета по его названию в item_names
                        int req_id = -1;
                        for (int i = 0; i < 10; i++) {
                            if (strcmp(item_names[i], clean_item) == 0) {
                                req_id = i;
                                break;
                            }
                        }

                        // 2. Проверяем, есть ли этот предмет в инвентаре игрока
                        int has_item = 0;
                        for (int i = 0; i < INVENTORY_SIZE; i++) {
                            if (inventory[i] == req_id) {
                                has_item = 1;
                                break;
                            }
                        }

                        // 3. Выводим в output.txt 
                        if (has_item) {
                            fprintf(out, "Заказчик: %s | Предмет: %s | Награда: %d [ГОТОВО К СДАЧЕ]\n", customer, clean_item, reward);
                        }
                        else {
                            fprintf(out, "Заказчик: %s | Предмет: %s | Награда: %d [В ПРОЦЕССЕ]\n", customer, clean_item, reward);
                        }
                    }
                }
            }

            fclose(in);
            fclose(out);
            printf("Доска объявлений обработана! Результаты записаны в output.txt.\n");
            break;
        }

        default:
            printf("Неверный пункт!\n");
        }
    }
    return 0;
}
