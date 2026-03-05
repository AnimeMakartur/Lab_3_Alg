#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
// Макрос для  який додає NULL в кінець списку аргументів
#define printSpecificWordSafe( w, ...) printSpecificWord( w, __VA_ARGS__, NULL)
#define printSpecificWordNostdargSafe( w, str, ...) printSpecificWord( w, str, __VA_ARGS__, NULL)

char* getWordByIndex(const char*, int);// прототип функциї для отримання слова по індексу
void printSpecificWord(int, ...);// прототип функциї для виведення слова з використанням stdarg
void printSpecificWordNostdarg(int, const char*, ...); // прототип функциї для виведення слова без використання stdarg, з використанням масиву аргументів

int main()
{
	char str1[] = "Hello world!";// тестовий рядок для функцій
	char str2[] = "A? B; C. D, E F G";// тестовий рядок для функцій
	printSpecificWordNostdargSafe(3, str1);
	printSpecificWordSafe(4, "Hello world!", "This is a test string.", "C programming is fun.");
	printSpecificWordSafe( 2, "One two three", str1);
	printSpecificWordSafe( 1, "First string", "Second string", "Third string string string", "Fourth string");
	printSpecificWordSafe( 0, "First string", "Second string", "Third string", "Fourth string");
    printSpecificWordSafe(3, str2);
	printf("\n");
	printSpecificWordSafe(1);
	printSpecificWordSafe(1, str1, str2); 
	printf("\n");
    printSpecificWordNostdargSafe(0, str1, str2);
	printSpecificWordNostdargSafe(4, str1, "This is a test.", "C programming is fun.");
	printSpecificWordNostdargSafe(3, str1);
}

char* getWordByIndex(const char* str, int index)
{
	char* copy = _strdup(str);// створюємо копію рядка, оскільки strtok змінює рядок
	const char* delimiters = " ,.!?;";// роздільники для токенізації
	char* nextToken = NULL;// змінна для збереження наступного токена
	char* token = strtok_s(copy, delimiters, &nextToken);// отримуємо перший токен
	int count = 1;// лічильник для відстеження індексу слова
	char* result;
	while (token != NULL) {
		if (count == index) {
			result = _strdup(token);// повертаємо слово
			free(copy);
			return result;
		}
		token = strtok_s(NULL, delimiters, &nextToken);// отримуємо наступний токен
		count++;// збільшуємо лічильник
	}
	free(copy);//звільнення пам'яті
	return NULL; 
}

void printSpecificWord(int wordNum, ...) {
    if (wordNum < 1) {
        printf("Invalid input\n");
        return;
    }

    va_list args;
    va_start(args, wordNum);
    printf("The %d-th word:\n", wordNum);

    int i = 1;
    while (1) {
        const char* str = va_arg(args, const char*);
        if (str == NULL) break; // виходимо по завершеню опрацювання речень
        char* word = getWordByIndex(str, wordNum);//функція для пошуку слова в реченні
        if (word) {
            printf("String %d: %s\n", i++, word);
            free(word); // Звільняємо пам'ять після _strdup
        }
        else {
            printf("String %d: No such word\n", i++);
        }
    }
    va_end(args);
}

void printSpecificWordNostdarg(int wordNum, const char* firstStr, ...) {
    if (wordNum < 1 || firstStr == NULL) return;
    const char** pStr = &firstStr;
    int i = 1;
    printf("Nostdarg mode, word %d:\n", wordNum);
    while (*pStr != NULL) {// виходимо по завершеню опрацювання речень
        char* word = getWordByIndex(*pStr, wordNum);//функція для пошуку слова в реченні
        if (word) {
            printf("String %d: %s\n", i++, word);
            free(word);
        }
        else {
            printf("String %d: No such word\n", i++);
        }
        pStr++; // Переходимо до наступного аргументу в стеку
    }
}