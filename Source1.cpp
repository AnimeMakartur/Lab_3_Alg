#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdarg.h>
#include <string.h>

double Forms(int count, ...) {
    double* number = &count + 1;
    double result = 0.0;
    if ((count & 1) != 0) {
        return DBL_MIN;
    }

    for (int i = 0; i < count; i += 2) {
        result += sin(*number);
        number++;
        result -= cos(*number);
        number++;
    }
    return result;
}

double Forms4(char* type, ...)
{
    double result = 0.0; double current_number;
    char* pt = type;
    char* pmess; int i_numb; long l_numb; double d_numb;
    int sign = 1; int count = strlen(type);

    void* number;
    number = (void*)(&type + 1);;

    for (int i = 0; i < count; i++) {
        if (type[i] == 's') {
            count--;
        }
    }

    if (count % 2 != 0) {
        return DBL_MIN;
    }

    while (*pt != '\0') {
        switch (*pt) {
        case 'i': i_numb = *(int*)number;
            current_number = (double)i_numb;
            number = (int*)number + 1;
            break;
        case 'l': l_numb = *(long int*)number;
            current_number = (double)l_numb; \
                number = (long int*)number + 1;
            break;
        case 'd': d_numb = *(double*)number;
            current_number = d_numb;
            number = (double*)number + 1;
            break;
        case 's': pmess = *(char**)number;
            printf("%s\n", pmess);
            number = (char**)number + 1;
        }

        if (*pt != 's') {
            result += sign * (sign == 1 ? sin(current_number) : cos(current_number));
        }
        pt++; sign = -sign;
    }
    return result;
}

double Forms2(char* type, ...)
{
    double result = 0.0; double current_number;
    char* pt = type;
    char* pmess; int i_numb; long l_numb; double d_numb;
    int sign = 1; int count = strlen(type);

    for (int i = 0; i < count; i++) {
        if (type[i] == 's') {
            count--;
        }
    }

    if (count % 2 != 0) {
        return DBL_MIN;
    }

    va_list args;
    va_start(args, &type + count);
    while (*pt != '\0') {
        switch (*pt) {
        case 'i': i_numb = va_arg(args, int);
            current_number = (double)i_numb;
            break;
        case 'l': l_numb = va_arg(args, long);
            current_number = (double)l_numb;
            break;
        case 'd': d_numb = va_arg(args, double);
            current_number = d_numb;
            break;
        case 's': pmess = va_arg(args, char*);
            printf("%s\n", pmess);
        }

        if (*pt != 's') {
            result += sign * (sign == 1 ? sin(current_number) : cos(current_number));
        }
        pt++; sign = -sign;
    }
    va_end(args);
    return result;
}

double Forms3(int count, ...) {
    char* number = &count + 1;
    double result = 0.0;

    if (count % 2 != 0) {
        return DBL_MIN;
    }

    for (int i = 0; i < count; i += 2) {
        result += sin(*(double*)number);
        number += sizeof(double);
        result -= cos(*(double*)number);
        number += sizeof(double);
    }
    return result;
}

int main() {
    system("chcp 65001");

    // printf("Func 1: \n");
    // printf("Test1: %lf\n", Forms(4, 3.15*4, 14.4/6.76, 18.19-12, 16.0));
    // printf("Test2: %lf\n", Forms(6, 3.15*4, 2.0*123.123, 3.0*1234, 4.0+1723.123, 5.0/100, 6.0));
    // printf("Test3: %lf\n", Forms(5, 3.15*4, 0*3.123, 3.0*1234, 4.0+13.123, 5.0/100));


    // printf("\n\n\n");
    //? Second func
    printf("Func 2: \n");
    printf("Test1: %lf\n", Forms2("dddd", 3.15 * 4, 14.4 / 6.76, 18.19 - 12, 16.0));
    printf("Test2: %lf\n", Forms2("idlds", 3 * 4, 14.4 / 6.76, 180 * 12, 16.0, "Hello world"));
    printf("Test3: %lf\n", Forms2("ddildd", 1.0, 2.0 * 123.123, 1234, 1723, 5.0 / 100, 6.0));
    printf("Test4: %lf\n", Forms2("idldsi", 145, 0 * 3.123, 3 * 1234, 4.0 + 13.123, "Hello", 12809));
    printf("Test5: %lf\n", Forms2("idldsis", 145, 0 * 3.123, 3 * 1234, 4.0 + 13.123, "Hello", 12809, "END"));

    printf("\n\n\n");
    printf("Func4:\n");
    printf("Test1: %lf\n", Forms4("dddd", 3.15 * 4, 14.4 / 6.76, 18.19 - 12, 16.0));
    printf("Test2: %lf\n", Forms4("idlds", 3 * 4, 14.4 / 6.76, 180 * 12, 16.0, "Hello world"));
    printf("Test3: %lf\n", Forms4("ddildd", 1.0, 2.0 * 123.123, 1234, 1723, 5.0 / 100, 6.0));
    printf("Test4: %lf\n", Forms4("idldsi", 145, 0 * 3.123, 3 * 1234, 4.0 + 13.123, "Hello", 12809));
    printf("Test5: %lf\n", Forms4("idldsis", 145, 0 * 3.123, 3 * 1234, 4.0 + 13.123, "Hello", 12809, "END"));
    // printf("\n\n\n");
    // //?Experemental func
    // printf("Func 3: \n");
    // printf("Test1: %lf\n", Forms3(4, 3.15*4, 14.4/6.76, 18.19-12, 16.0));
    // printf("Test2: %lf\n", Forms3(6, 1.0, 2.0*123.123, 3.0*1234, 4.0+1723.123, 5.0/100, 6.0));
    // printf("Test3: %lf\n", Forms3(5, 1.0, 0*3.123, 3.0*1234, 4.0+13.123, 5.0/100));

    return 0;
}



