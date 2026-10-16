#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVENTORY_SIZE 10

//функция вывода инвентаря
void print_inventory(const int inventory[], int size) {
    printf("--- Инвентарь ---\n");
    for (int i = 0; i < size; i++) {
        printf("Слот %d: [%d] (", i, inventory[i]);
        switch (inventory[i]) {
        case 0: printf("Пусто"); break;
        case 1: printf("Дерево"); break;
        case 2: printf("Камень"); break;
        case 3: printf("Семена"); break;
        case 4: printf("Пшеница"); break;
        case 5: printf("Яблоко"); break;
        case 6: printf("Лопата"); break;
        case 7: printf("Вода"); break;
        case 8: printf("Удобрение"); break;
        case 9: printf("Золото"); break;
        default: printf("Неизвестно"); break;
        }
        printf(")\n");
    }
}

int main() {
    system("chcp 65001 > nul");

    char farmer_name[32];
    printf("Введите имя вашего фермера: ");
    scanf("%31s", farmer_name);

    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 0, 4, 5, 0, 7, 1, 9 };
    int choice = -1;

    while (1) {
        printf("\n=== ВЕСЕЛЫЙ ФЕРМЕР (Имя: %s) ===\n", farmer_name);
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("[6] Ревизия ресурсов\n");
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
            print_inventory(inventory, INVENTORY_SIZE);
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

        default:
            printf("Неверный пункт!\n");
        }
    }
    return 0;
}