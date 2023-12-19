#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h> // Для использования uint8_t

// Функция для вывода массива битов на экран
void print_arrey(uint8_t arr[], int start, int size) {
    for (int i = start; i < size; i++) {
        printf("%u ", arr[i]);
    }
    printf("\n");
}

int main() {
    srand(time(NULL));
    int c = 0;
    int len_N = 250, len_G = 7, len_N_0 = len_N + len_G - 1;
    uint8_t N[len_N];    // Исходные данные
    uint8_t N_0[len_N_0]; // Расширенные данные с нулями в конце
    uint8_t N_tx[len_N_0]; // Данные, которые будут переданы

    uint8_t G[] = {1, 1, 1, 1, 1, 1, 1}; // Порождающий полином G

    // Генерация случайных данных
    for (int i = 0; i < len_N; i++) {
        N[i] = rand() % 2;
        N_0[i] = N[i];
        N_tx[i] = N[i];
    }

    // Добавление нулей в конец исходных данных для умножения на x^(len_G-1)
    for (int i = len_N; i < len_N_0; i++) {
        N_0[i] = 0;
    }

    // Рассчет CRC (TX)
    for (int i = 0; i < len_N; i++) {
        if (N_0[i] == 1) {
            for (int j = 0; j < len_G; j++) {
                N_0[i + j] = N_0[i + j] ^ G[j];
            }
        }
    }

    // Копирование данных для передачи (TX)
    for (int i = len_N; i < len_N_0; i++) {
        N_tx[i] = N_0[i];
    }

    printf("Исходные данные: \t ");
    print_arrey(N, 0, len_N);
    printf("CRC передачи:  ");
    print_arrey(N_0, len_N, len_N_0);
    printf("Данные для передачи: ");
    print_arrey(N_tx, 0, len_N_0);

    // Сохранение переданных данных для последующей проверки (RX)
    for (int i = 0; i < len_N_0; i++) {
        N_0[i] = N_tx[i];
    }

    // Искажение переданных данных (RX) и рассчет CRC (RX)
    for (int i = 0; i < len_N; i++) {
        if (N_tx[i] == 1) {
            for (int j = 0; j < len_G; j++) {
                N_tx[i + j] = N_tx[i + j] ^ G[j];
            }
        }
    }

    int flag = 0;

    printf("CRC приема:  ");
    print_arrey(N_tx, len_N, len_N_0);

    // Проверка наличия ошибок (RX)
    for (int i = len_N; i < len_N_0; i++) {
        if (N_tx[i] == 1) {
            printf("Ошибка в данных!\n");
            flag = 0;
            break;
        } else
            flag = 1;
    }

    if (flag == 1) {
        printf("Успешно!\n");
    }

    int temp[len_N_0];
    int count_error = 0;

    // Восстановление данных и подсчет ошибок после искажения (destructive testing)
    for (int j = 0; j < len_N + len_G - 1; j++) {
        temp[j] = N_0[j];
    }

    for (int d = 0; d < len_N + len_G - 1; d++) {
        int N_distr[len_N + len_G - 1];

        for (int j = 0; j < len_N + len_G - 1; j++) {
            N_distr[j] = temp[j];
        }

        // Искажение бита данных
        if (N_distr[d] == 1) {
            N_distr[d] = 0;
        } else
            N_distr[d] = 1;

        for (int j = 0; j < len_N + len_G - 1; j++) {
            temp[j] = N_distr[j];
        }

        // Рассчет CRC
        for (int i = 0; i < len_N; i++) {
            if (N_distr[i] == 1) {
                for (int j = 0; j < len_G; j++) {
                    N_distr[i + j] = N_distr[i + j] ^ G[j];
                }
            }
        }

        // Подсчет ошибок
        for (int i = len_N; i < len_N_0; i++) {
            if (N_distr[i] == 1) {
                count_error += 1;
                break;
            }
        }
    }

    printf("\n\nКоличество ошибок после искажения: %d", count_error);
    printf(" из %d\n", len_N_0);
    int good = len_N_0 - count_error;
    printf("Не обнаружено: %d\n", good);

    return 0;
}
