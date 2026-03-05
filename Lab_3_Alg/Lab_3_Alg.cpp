#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
// Макрос для безпечного виклику функції printSpecificWord, який додає NULL в кінець списку аргументів
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
	printSpecificWordSafe(4, "Hello world!", "This is a test.", "C programming is fun.");//тест 1
	printSpecificWordSafe( 2, "One two three", str1);//тест 2
	printSpecificWordSafe( 1, "First string", "Second string", "Third string", "Fourth string");//тест 3, перевірка на некоректне введення
	printSpecificWordSafe( 0, "First string", "Second string", "Third string", "Fourth string");//тест 4, перевірка на некоректне введення
	
	printf("\n");
	printSpecificWordSafe( 1);//тест 5, перевірка на відсутність рядків
	printSpecificWordSafe( 1, str1, str2); //тест 6, перевірка на невідповідність кількості рядків і переданих аргументів
	printf("\n");
	printSpecificWordNostdargSafe(4, str1, "This is a test.", "C programming is fun.");//тест 7, перевірка функції без використання stdarg
	printSpecificWordNostdargSafe(3, str1);//тест 8, перевірка функції без використання stdarg на невідповідність кількості рядків і переданих аргументів
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
	free(copy);
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
        if (str == NULL) break; // ОСЬ ПРАВИЛЬНА ПЕРЕВІРКА НА КІНЕЦЬ

        char* word = getWordByIndex(str, wordNum);
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

    // Використання вказівників на стек — це bad practice, 
    // але якщо вчитель вимагає "без stdarg", то:
    const char** p = &firstStr;
    int i = 1;

    printf("Nostdarg mode, word %d:\n", wordNum);
    while (*p != NULL) {
        char* word = getWordByIndex(*p, wordNum);
        if (word) {
            printf("String %d: %s\n", i++, word);
            free(word);
        }
        else {
            printf("String %d: No such word\n", i++);
        }
        p++; // Переходимо до наступного аргументу в стеку
    }
}