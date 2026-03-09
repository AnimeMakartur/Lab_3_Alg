#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
// Макрос для округлення числа до найближчого цілого
#define Round(x) ((x) < 0 ? (int)((x) - 0.5) : (int)((x) + 0.5))

// Макрос для  який додає NULL в кінець списку аргументів
#define printSpecificWordSafe( w, ...) printSpecificWord( w, __VA_ARGS__, NULL)
#define printSpecificWordNostdargSafe( w, str, ...) printSpecificWord( w, str, __VA_ARGS__, NULL)
#define printSpecificWordByNumgSafe(n, ...) printSpecificWordByNumg(n, __VA_ARGS__, NULL)

// Прототипи функцій
char* getWordByIndex(const char*, int);// прототип функциї для отримання слова по індексу
void printSpecificWord(int, ...);// прототип функциї для виведення слова з використанням stdarg
void printSpecificWordWithType(const char*, ...);// прототип функциї для виведення слова з використанням stdarg з типами
void printSpecificWordNostdarg(int, const char*, ...);  // прототип функциї для виведення слова без використання stdarg, з використанням масиву аргументів
void printSpecificWordByNumg(int, ...); // прототип функциї для виведення слова без використання stdarg, з використанням масиву аргументів, де індекс слова задається в числовому вигляді

int main()
{
	char str1[] = "Hello world!";// тестовий рядок для функцій
	char str2[] = "A? B; C. D, E F G";// тестовий рядок для функцій
	char str3[] = "This is a test string for the function.";// ще один тестовий рядок
	char str4[] = "Another example with more words to test the functionality.";// ще один тестовий рядок
	long long ll = 1234567890123456789LL;// тестове число для функції з типами
    printSpecificWordNostdargSafe(3, str1);
    printSpecificWordSafe(4, "Hello world!", "This is a test.", "C programming is fun.");//тест 1
    printSpecificWordSafe(2, "One two three", str1);//тест 2
    printSpecificWordSafe(1, "First string", "Second string", "Third string", "Fourth string");//тест 3, перевірка на некоректне введення
    printSpecificWordSafe(0, "First string", "Second string", "Third string", "Fourth string");//тест 4, перевірка на некоректне введення
    printf("\n");
    printSpecificWordSafe(1);//тест 5, перевірка на відсутність рядків
    printSpecificWordSafe(1, str1, str2); //тест 6, перевірка на невід-повідність кількості рядків і переданих аргументів
    printf("\n");
    printSpecificWordNostdargSafe(4, str1, "This is a test.", "C programming is fun.");//тест 7, перевірка функції без використання stdarg
    printSpecificWordNostdargSafe(3, str1);//тест 8, перевірка функції без використання stdarg на невідповідність кількості рядків і переданих ар-гументів
    printSpecificWordByNumgSafe(9, str3, 2, str1, "str2", 5, str1);// виклик функції для виведення слова з використанням stdarg з типами
	printSpecificWordByNumgSafe(0, str3, 2, str1, "str2", 5, str1);// виклик функції для виведення слова з використанням stdarg з типами з некоректним індексом
	printSpecificWordByNumgSafe(8, str3, 2, str1, 5, str1, 6, str4, 4, str4, 5, str4 );// виклик функції для виведення слова з використанням stdarg з типами з некоректним індексом
    printSpecificWordByNumgSafe(9, str4, 0, str1, "str2", 5, str1);// виклик функції для виведення слова з використанням stdarg з типами з некоректним індексом
    printSpecificWordByNumgSafe(8, str4, 1, str1, 5, NULL, 5, str1);
    printf("\n");
	printSpecificWordWithType("ids", 3, str1, 2.5, str2, str3);// виклик функції для виведення слова з використанням stdarg з типами
	printSpecificWordWithType("ids", 10, str1, 2.5, str2, str3);// виклик функції для виведення слова з використанням stdarg з типами з некоректним індексом
    printSpecificWordWithType("idds", 10, str1, 2.7, str2, str3);
	printSpecificWordWithType("ids", 10, str1, 2.0, NULL, str3);
	printSpecificWordWithType("ids", 1, str1, 2.7, NULL, NULL);
	printSpecificWordWithType("ids", -1, str1, -7.7, str2, str3);
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
	printf("Print specific word with type:\n");
    int i = 1;
    while (*pType != '\0') {
		const char* str = NULL;// Рядок для пошуку слова
		char* word = NULL;// Індекс слова за замовчуванням
        int targetIndex = 1;

        switch (*pType) {
        case 'i': {
            // Читаємо індекс як int, потім рядок
            targetIndex = va_arg(args, int);
            if(targetIndex < 1) {
                printf("Invalid index %d, search first word\n", targetIndex);
                targetIndex = 1;
			}
            str = va_arg(args, const char*);
            break;
        }
        case 'd': {
            // Читаємо індекс як double, округлюємо, потім рядок
            double dWordNum = va_arg(args, double);
            targetIndex = Round(dWordNum);
            if (targetIndex < 1) {
                printf("Invalid index %d (rounded from %.2f), search first word\n", targetIndex, dWordNum);
                targetIndex = 1;
            }
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
		if (slot > 0xFFFF) {// Якщо слот більше 0xFFFF, вважаємо його рядком
            str = (const char*)slot;
            word = getWordByIndex(str, 1);
            printf("Pair %d (Index 1): %s\n", i++, word ? word : "No such word");
            if (word) free(word);
            slot = *pStack++; // Читаємо наступний слот
        }
        else {
			wordNum = (int)slot;// Якщо слот менше або рівний 0xFFFF, вважаємо його індексом
			nextSlot = *pStack++;// Читаємо наступний слот, який має бути рядком
            if (nextSlot == 0) {
                // Якщо це не останній елемент, просто пропускаємо цю пару
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