#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define Round(x) ((x) < 0 ? (int)((x) - 0.5) : (int)((x) + 0.5))
// Макрос для  який додає NULL в кінець списку аргументів
#define printSpecificWordSafe( w, ...) printSpecificWord( w, __VA_ARGS__, NULL)
#define printSpecificWordNostdargSafe( w, str, ...) printSpecificWord( w, str, __VA_ARGS__, NULL)
#define printSpecificWordByNumgSafe(n, ...) printSpecificWordByNumg(n, __VA_ARGS__, 0)

char* getWordByIndex(const char*, int);// прототип функциї для отримання слова по індексу
void printSpecificWord(int, ...);// прототип функциї для виведення слова з використанням stdarg
void printSpecificWordWithType(const char*, ...);
void printSpecificWordNostdarg(int, const char*, ...);  // прототип функциї для виведення слова без використання stdarg, з використанням масиву аргументів
void printSpecificWordByNumg(int, ...); // прототип функциї для виведення слова без використання stdarg, з використанням масиву аргументів, де індекс слова задається в числовому вигляді

int main()
{
	char str1[] = "Hello world!";// тестовий рядок для функцій
	char str2[] = "A? B; C. D, E F G";// тестовий рядок для функцій
	char str3[] = "This is a test string for the function.";// ще один тестовий рядок
	char str4[] = "Another example with more words to test the functionality.";// ще один тестовий рядок
    printSpecificWordByNumgSafe(9, str3, 2, str1, "str2", 5, str1);// виклик функції для виведення слова з використанням stdarg з типами
	printSpecificWordByNumgSafe(0, str3, 2, str1, "str2", 5, str1);// виклик функції для виведення слова з використанням stdarg з типами з некоректним індексом
	printSpecificWordByNumgSafe(8, str3, 2, str1, 5, str1, 6, str4, 4, str4, 5, str4 );// виклик функції для виведення слова з використанням stdarg з типами з некоректним індексом
    printSpecificWordByNumgSafe(9, str4, 0, str1, "str2", 5, str1);// виклик функції для виведення слова з використанням stdarg з типами з некоректним індексом
    printSpecificWordByNumgSafe(8, str4, 1, str1, 5, NULL, 5, str1);
    printf("\n");

	printf("\n");

}

char* getWordByIndex(const char* str, int index)
{
	char* copy = _strdup(str);// створюємо копію рядка, оскільки strtok змінює рядок
	const char* delimiters = " ,.!?;:";// роздільники для токенізації
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

void printSpecificWordWithType(const char* type, ...) {
    if (type == NULL) return;

    const char* pType = type;
    va_list args;
    va_start(args, type);

    int i = 1;
    while (*pType != '\0') {
        const char* str = NULL;
        char* word = NULL;
        int targetIndex = 1;

        switch (*pType) {
        case 'i': {
            // Читаємо індекс як int, потім рядок
            targetIndex = va_arg(args, int);
            str = va_arg(args, const char*);
            break;
        }
        case 'd': {
            // Читаємо індекс як double, округлюємо, потім рядок
            double dWordNum = va_arg(args, double);
            targetIndex = Round(dWordNum);
            str = va_arg(args, const char*);
            break;
        }
        case 's': {
            // Тільки рядок, індекс за замовчуванням 1
            str = va_arg(args, const char*);
            targetIndex = 1;
            break;
        }
        default:
            pType++; // Переходимо до наступного символу, якщо тип невідомий
            continue;
        }

        if (str != NULL) {
            // Спроба знайти слово за вказаним індексом
            word = getWordByIndex(str, targetIndex);

            // Якщо слова немає (NULL), шукаємо перше слово (індекс 1)
            if (word == NULL) {
                word = getWordByIndex(str, 1);
                if (word) {
                    printf("String %d (%c): Word %d not found, showing first: %s\n", i++, *pType, targetIndex, word);
                }
                else {
                    printf("String %d (%c): Sentence is empty\n", i++, *pType);
                }
            }
            else {
                printf("String %d (%c): %s\n", i++, *pType, word);
            }

            if (word) free(word); // Звільняємо пам'ять після _strdup
        }

        pType++; // ВАЖЛИВО: перехід до наступного символу типу, щоб уникнути зависання
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

void printSpecificWordByNumg(int firstWordNum, ...) {
    if (firstWordNum == 0) {
		printf("Invalid input: firstWordNum cannot be zero.\n");
		return;
    }
    int wordNum;
    char* word;
    const char* str;
    int* pStack = (int*)&firstWordNum;
    int slot = *pStack;
    int nextSlot;
    pStack++;
    int i = 1;

    printf("Nostdarg mode, word:\n");

    while (slot != 0) {
        if (slot > 0xFFFF) {
            str = (const char*)slot;
            // Перевірка на NULL (хоча slot > 0xFFFF вже відсікає 0)
            word = getWordByIndex(str, 1);
            printf("Pair %d (Index 1): %s\n", i++, word ? word : "No such word");
            if (word) free(word);

            slot = *pStack++; // Читаємо наступний слот
        }
        else {
            wordNum = (int)slot;
            nextSlot = *pStack++;
            if (nextSlot == 0) {
                // Якщо це не останній елемент, просто пропускаємо цю пару
                // Але як дізнатися, чи це кінець? 
                // Якщо наступний за ним слот теж 0 — це точно кінець.
                if (*(pStack + 1) == 0) break;
                printf("Pair %d: Skip (String is NULL)\n", i++);
            }
            else {
                str = (const char*)nextSlot;
                word = getWordByIndex(str, wordNum);
                printf("Pair %d (Index %d): %s\n", i++, wordNum, word ? word : "No word");
                if (word) free(word);
            }

            slot = *pStack++; // Читаємо наступний слот для нової ітерації
        }
    }
	printf("Functions come to 0(NULL) at the end of arguments list, stopping iteration.\n");
}