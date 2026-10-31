#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVENTORY_SIZE 10
#define TOTAL_ITEMS 10

void load_item_catalog(char item_names[TOTAL_ITEMS][32]);
void show_clock(int current_day, int current_hour);
void pass_time(int* current_day, int* current_hour);
void show_inventory(const int inventory[], const char item_names[TOTAL_ITEMS][32]);
void place_item(int inventory[]);
void clear_slot(int inventory[]);
void audit_resource_by_id(const int inventory[]);
void search_item_by_name(const int inventory[], const char item_names[TOTAL_ITEMS][32]);
void write_diary(const char* farmer_name, int current_day, int current_hour, const int inventory[], const char item_names[TOTAL_ITEMS][32]);
void check_crafting_recipes(const int inventory[], const char item_names[TOTAL_ITEMS][32]);

int get_safe_int(const char* prompt, int min_val, int max_val) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            printf("Ошибка: введено не число! Попробуйте снова: ");
            while (getchar() != '\n');
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("Ошибка: выберите число от %d до %d: ", min_val, max_val);
            continue;
        }
        return value;
    }
}

int main() {

    system("chcp 65001 > nul");

    char farmer_name[32];
    printf("Введите имя вашего фермера: ");
    if (scanf("%31s", farmer_name) != 1) {
        strcpy(farmer_name, "Фермер");
    }

    int current_day = 3;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 0, 4, 5, 0, 7, 1, 9 };

    char item_names[TOTAL_ITEMS][32] = {
        {"Пусто"}, {"Дерево"}, {"Камень"}, {"Семена"}, {"Пшеница"},
        {"Яблоко"}, {"Лопата"}, {"Вода"}, {"Удобрение"}, {"Золото"}
    };

    load_item_catalog(item_names);

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

        choice = get_safe_int("Выберите пункт: ", 0, 9);

        switch (choice) {
        case 0:
            printf("Выход из игры. До свидания!\n");
            return 0;
        case 1:
            show_clock(current_day, current_hour);
            break;
        case 2:
            pass_time(&current_day, &current_hour);
            break;
        case 3:
            show_inventory(inventory, item_names);
            break;
        case 4:
            place_item(inventory);
            break;
        case 5:
            clear_slot(inventory);
            break;
        case 6:
            audit_resource_by_id(inventory);
            break;
        case 7:
            search_item_by_name(inventory, item_names);
            break;
        case 8:
            write_diary(farmer_name, current_day, current_hour, inventory, item_names);
            break;
        case 9:
            check_crafting_recipes(inventory, item_names);
            break;
        default:
            printf("Неверный пункт!\n");
        }
    }
    return 0;
}
void load_item_catalog(char item_names[TOTAL_ITEMS][32]) {
    FILE* f = fopen("items.txt", "r");
    if (f == NULL) return; 
    int id;
    char name[32];
    while (fscanf(f, "%d %31s", &id, name) == 2) {
        if (id >= 0 && id < TOTAL_ITEMS) {
            strncpy(item_names[id], name, sizeof(item_names[id]) - 1);
            item_names[id][sizeof(item_names[id]) - 1] = '\0';
        }
    }
    fclose(f);
}

// --- ПУНКТ 1: ПОСМОТРЕТЬ НА ЧАСЫ
void show_clock(int current_day, int current_hour) {
    printf("День %d, %02d:00\n", current_day, current_hour);
}

// --- ПУНКТ 2: ПРОМОТАТЬ ВРЕМЯ 
void pass_time(int* current_day, int* current_hour) {
    int hours = get_safe_int("Сколько часов поработать? ", 1, 168);
    *current_hour += hours;
    if (*current_hour >= 24) {
        *current_day += *current_hour / 24;
        *current_hour %= 24;
    }
    printf("Время обновлено. День %d, %02d:00\n", *current_day, *current_hour);
}

// --- ПУНКТ 3: ПОСМОТРЕТЬ ИНВЕНТАРЬ 
void show_inventory(const int inventory[], const char item_names[TOTAL_ITEMS][32]) {
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = inventory[i];
        const char* name = (id >= 0 && id < TOTAL_ITEMS) ? item_names[id] : "Неизвестно";
        printf("Слот %d: [%d] (%s)\n", i, id, name);
    }
}

// --- ПУНКТ 4: ПОЛОЖИТЬ ПРЕДМЕТ В СЛОТ 
void place_item(int inventory[]) {
    int slot = get_safe_int("Слот (0-9): ", 0, INVENTORY_SIZE - 1);
    int id = get_safe_int("ID предмета (0-9): ", 0, TOTAL_ITEMS - 1);
    inventory[slot] = id;
    printf("Предмет успешно добавлен в слот %d!\n", slot);
}

// --- ПУНКТ 5: ВЫБРОСИТЬ ПРЕДМЕТ 
void clear_slot(int inventory[]) {
    int slot = get_safe_int("Слот для очистки (0-9): ", 0, INVENTORY_SIZE - 1);
    inventory[slot] = 0;
    printf("Слот %d очищен!\n", slot);
}

// --- ПУНКТ 6: РЕВИЗИЯ РЕСУРСОВ ПО ID 
void audit_resource_by_id(const int inventory[]) {
    int target = get_safe_int("ID предмета для поиска (0-9): ", 0, TOTAL_ITEMS - 1);
    printf("Слоты: ");
    int count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == target) {
            printf("%d ", i);
            count++;
        }
    }
    printf("\nВсего: %d\n", count);
}

// --- ПУНКТ 7: ПОИСК ПРЕДМЕТА ПО НАЗВАНИЮ 
void search_item_by_name(const int inventory[], const char item_names[TOTAL_ITEMS][32]) {
    char query[32];
    printf("Введите название для поиска: ");
    if (scanf("%31s", query) != 1) return;

    int found_id = -1;
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        if (strcmp(item_names[i], query) == 0) {
            found_id = i;
            break;
        }
    }

    if (found_id == -1) {
        printf("Предмет не найден в каталоге!\n");
        return;
    }

    printf("Найден в слотах: ");
    int count = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == found_id) {
            printf("%d ", i);
            count++;
        }
    }
    printf("(Всего: %d)\n", count);
}

// --- ПУНКТ 8: ЗАПИСАТЬ СОСТОЯНИЕ 
void write_diary(const char* farmer_name, int current_day, int current_hour, const int inventory[], const char item_names[TOTAL_ITEMS][32]) {
    FILE* df = fopen("diary.txt", "a");
    if (df == NULL) {
        printf("Ошибка открытия diary.txt!\n");
        return;
    }


    fprintf(df, "Фермер: %s | День: %d, Время: %02d:00\n", farmer_name, current_day, current_hour);
    fprintf(df, "Инвентарь: ");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        fprintf(df, "[%d:%s] ", inventory[i], item_names[inventory[i]]);
    }
    fprintf(df, "\n-------------------\n");
    fclose(df);

   
    printf("\n--- Запись в дневнике (diary.txt) ---\n");
    printf("Фермер: %s | День: %d, Время: %02d:00\n", farmer_name, current_day, current_hour);
    printf("Инвентарь: ");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("[%d:%s] ", inventory[i], item_names[inventory[i]]);
    }
    printf("\n-----------------------------------\n");
    printf("Успешно записано в diary.txt и выведено на экран!\n");
}

// --- ПУНКТ 9: ПРОВЕРКА РЕЦЕПТОВ КРАФТА 
void check_crafting_recipes(const int inventory[], const char item_names[TOTAL_ITEMS][32]) {
    FILE* in = fopen("input.txt", "r");
    if (in == NULL) {
        printf("Ошибка: файл input.txt не найден!\n");
        return;
    }

    FILE* out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("Ошибка: не удалось создать output.txt!\n");
        fclose(in);
        return;
    }

    int inv_counts[TOTAL_ITEMS] = { 0 };
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = inventory[i];
        if (id >= 0 && id < TOTAL_ITEMS) {
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

        int req_counts[TOTAL_ITEMS] = { 0 };
        char* ptr = colon + 1;
        int req_id;
        while (sscanf(ptr, "%d", &req_id) == 1) {
            if (req_id >= 0 && req_id < TOTAL_ITEMS) {
                req_counts[req_id]++;
            }
            while (*ptr != '\0' && *ptr != ' ' && *ptr != '\t') ptr++;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
        }

        int possible = 1;
        char missing_details[256] = "";
        int first_missing = 1;

        for (int i = 0; i < TOTAL_ITEMS; i++) {
            if (req_counts[i] > inv_counts[i]) {
                possible = 0;
                int diff = req_counts[i] - inv_counts[i]; 
                const char* miss_name = (i >= 0 && i < TOTAL_ITEMS) ? item_names[i] : "Неизвестно";

                char temp[64];
                snprintf(temp, sizeof(temp), "%s (%d шт.)", miss_name, diff);

                if (first_missing) {
                    snprintf(missing_details, sizeof(missing_details), "%s", temp);
                    first_missing = 0;
                }
                else {
                    strncat(missing_details, ", ", sizeof(missing_details) - strlen(missing_details) - 1);
                    strncat(missing_details, temp, sizeof(missing_details) - strlen(missing_details) - 1);
                }
            }
        }

        if (possible) {
            fprintf(out, "Рецепт \"%s\": [ДОСТУПНО]\n", recipe_name);
        }
        else {
            fprintf(out, "Рецепт \"%s\": [НЕ ХВАТАЕТ РЕСУРСОВ] (не хватает: %s)\n", recipe_name, missing_details);
        }
    }

    fclose(in);
    fclose(out);
    printf("Проверка рецептов завершена! Результаты записаны в output.txt.\n");
}