#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
// Макрос для безпечного виклику функції printSpecificWord, який додає NULL в кінець списку аргументів
#define printSpecificWordSafe(n, w, ...) printSpecificWord(n, w, __VA_ARGS__, NULL)

char* getWordByIndex(const char*, int);// прототип функциї для отримання слова по індексу
void printSpecificWord(int, int, ...);// прототип функциї для виведення слова з використанням stdarg
void printSpecificWordNostdarg(int, int, const char*, ...); // прототип функциї для виведення слова без використання stdarg, з використанням масиву аргументів

int main()
{
	char str1[] = "Hello world!";// тестовий рядок для функцій
	char str2[] = "A? B; C. D, E F G";// тестовий рядок для функцій
	printSpecificWordSafe(3, 4, "Hello world!", "This is a test.", "C programming is fun.");//тест 1
	printSpecificWordSafe(2, 2, "One two three", str1);//тест 2
	printSpecificWordSafe(0, 1, "First string", "Second string", "Third string", "Fourth string");//тест 3, перевірка на некоректне введення
	printSpecificWordSafe(1, 0, "First string", "Second string", "Third string", "Fourth string");//тест 4, перевірка на некоректне введення
	
	printf("\n");
	printSpecificWordSafe(1, 1);//тест 5, перевірка на відсутність рядків
	printSpecificWordSafe(5, 1, str1, str2); //тест 6, перевірка на невідповідність кількості рядків і переданих аргументів

	printf("\n");
	printSpecificWordNostdarg(3, 4, str1, "This is a test.", "C programming is fun.");//тест 7, перевірка функції без використання stdarg
	printSpecificWordNostdarg(5, 3, str1);//тест 8, перевірка функції без використання stdarg на невідповідність кількості рядків і переданих аргументів
}

char* getWordByIndex(const char* str, int index)
{
	char* copy = _strdup(str);// створюємо копію рядка, оскільки strtok змінює рядок
	const char* delimiters = " ,.!?;";// роздільники для токенізації
	char* nextToken = NULL;// змінна для збереження наступного токена
	char* token = strtok_s(copy, delimiters, &nextToken);// отримуємо перший токен
	int count = 1;// лічильник для відстеження індексу слова
	while (token != NULL) {
		if (count == index) {
			return _strdup(token);// повертаємо слово
		}
		token = strtok_s(NULL, delimiters, &nextToken);// отримуємо наступний токен
		count++;// збільшуємо лічильник
	}
	return NULL; 
}

void printSpecificWordNostdarg(int numStr, int wordNum, const char* str1, ...) {
	if (numStr < 1 || wordNum < 1) {// перевірка на некоректне введення
		printf("Invalid input: numStr and wordNum must be greater than 0.\n");
		return;
	}
	char* word;
	const char** args = &str1;// створюємо масив вказівників на рядки, починаючи з str1
	printf("The %d-th word of the following strings:\n", wordNum);
	for (int i = 0; i < numStr; i++, args++) {
		if (*args == NULL) {// перевірка на NULL, якщо кількість рядків менша за numStr
			printf("[Error]: You said there are %d strings, but only found %d before NULL!\n", numStr, i);
			break;
		}
		word = getWordByIndex(*args, wordNum);// отримуємо слово по індексу з поточного рядка
		if (word) {// якщо слово існує, виводимо його
			printf("String %d: %s\n", i + 1, word);
		}
		else {
			printf("String %d: No such word\n", i + 1);
		}
	}

}

void printSpecificWord(int numStr , int wordNum, ...) {
	if (numStr < 1 || wordNum < 1) {
		printf("Invalid input: numStr and wordNum must be greater than 0.\n");
		return;
	}
	va_list args;// створюємо змінну для збереження аргументів
	va_start(args, wordNum); // ініціалізуємо змінну для збереження аргументів, вказуючи останній обов'язковий параметр	
	printf("The %d-th word of the following strings:\n", wordNum);

	for (int i = 0; i < numStr; i++) {
		const char* str = va_arg(args, const char*); // отримуємо поточний рядок з аргументів
		if (str == NULL) {// перевірка на NULL, якщо кількість рядків менша за numStr
			printf("[Error]: You said there are %d strings, but only found %d before NULL!\n", numStr, i);
			break;
		}
		char* word = getWordByIndex(str, wordNum);
		if (word) {
			printf("String %d: %s\n", i + 1, word);
		} else {
			printf("String %d: No such word\n", i + 1);
		}
	}
	va_end(args);	// звільняємо змінну для збереження аргументів
}