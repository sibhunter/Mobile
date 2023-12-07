#include <stdio.h>
#include <math.h>

#define LENGTH 5

void shift_register_x(int *register_state_x) {
    int feedback = (register_state_x[2] + register_state_x[3]) % 2;
    for (int i = LENGTH - 1; i > 0; i--) {
        register_state_x[i] = register_state_x[i - 1];
    }
    register_state_x[0] = feedback;
}

void shift_register_y(int *register_state_y) {
    int feedback = (register_state_y[1] + register_state_y[2]) % 2;
    for (int i = LENGTH - 1; i > 0; i--) {
        register_state_y[i] = register_state_y[i - 1];
    }
    register_state_y[0] = feedback;
}

// Функция для Последовательности Голда
void generate_pseudo_random_sequence(int *sequence, int *register_state_x, int *register_state_y, int length) {
    printf("\n");
    printf("Последовательность Голда равняется: ");
    for (int i = 0; i < length; i++) {
        sequence[i] = (register_state_x[4] + register_state_y[4]) % 2;
        printf("%d", sequence[i]);
        shift_register_x(register_state_x);
        shift_register_y(register_state_y);
    }
    printf("\n\n");
}

// Функция для Второй Последовательности Голда
void second_generate_pseudo_random_sequence(int *sequence, int *register_state_x, int *register_state_y, int length) {
    printf("Измененная последовательность Голда равняется: ");
    for (int i = 0; i < length; i++) {
        sequence[i] = (register_state_x[4] + register_state_y[4]) % 2;
        printf("%d", sequence[i]);
        shift_register_x(register_state_x);
        shift_register_y(register_state_y);
    }
    printf("\n");
}

// Функция для сдвига
void cyclic_shift(int *sequence, int size) {
    int temp = sequence[size - 1];
    for (int i = size - 1; i > 0; i--) {
        sequence[i] = sequence[i - 1];
    }
    sequence[0] = temp;
}

// Функция для автокорреляции
double calculate_autocorrelation(int *sequence1, int *sequence2, int length) {
    int matches = 0;
    for (int i = 0; i < length; i++) {
        matches += sequence1[i] == sequence2[i] ? 1 : -1;
    }
    return matches;
}

//Функция для взаимно корреляции
double calculate_crosscorrelation(int *sequence1, int *sequence2, int length) {
    double crosscorrelation = 0;
    for (int i = 0; i < length; i++) {
        crosscorrelation += sequence1[i] * sequence2[i];
    }
    return crosscorrelation;
}

int main() {
    int cons = pow(2, LENGTH) - 1;
    int pseudo_random_sequence[cons];
    int modified_sequence[cons];
    
    int register_state_x[LENGTH] = {0, 0, 1, 1, 0};
    int register_state_y[LENGTH] = {0, 1, 1, 0, 1};

    int register_state_x1[LENGTH] = {0, 0, 1, 1, 1};
    int register_state_y1[LENGTH] = {0, 0, 0, 0, 1};
    
    generate_pseudo_random_sequence(pseudo_random_sequence, register_state_x, register_state_y, cons);
    second_generate_pseudo_random_sequence(modified_sequence, register_state_x1, register_state_y1, cons);
    
    printf("\n");
    
    int shifted_sequence[cons];
    printf("Сдвиг|1 |2 |3 |4 |5 |6 |7 |8 |9 |10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|Автокорреляция\n");
    for (int shift = 0; shift < cons; shift++) {
        printf("%5d|", shift);
        for (int i = 0; i < cons; i++) {
            shifted_sequence[i] = pseudo_random_sequence[(i + shift) % cons];
            printf("%2d|", shifted_sequence[i]);
        }
        double autocorrelation = calculate_autocorrelation(pseudo_random_sequence, shifted_sequence, cons);
        printf("%.0f/31", autocorrelation);
        printf("\n");
    }
    
    printf("\n");
    double crosscorrelation = 0;
    printf("Взаимная корреляция двух последовательностей голда равняется: ");
    for (int shift = 0; shift < cons; shift++) {
        crosscorrelation = calculate_crosscorrelation(pseudo_random_sequence, modified_sequence, cons);
    }
    printf("%.0f", crosscorrelation);
    printf("\n");
    return 0;
}

