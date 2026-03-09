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
    printSpecificWordByNumgSafe(1, str1, 2, str2, str2, 5, str1);// виклик функції для виведення слова з використанням stdarg з типами
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
    const char* pType = type;
    
	int iWordNum = 0;
	double dWordNum = 0.0;
    long long lWordNum = 0;
    const char* str;
    char* word;
    va_list args;
    va_start(args, type);


    int i = 1;
    while (*pType != '\0') {
        switch (*pType)
        {
            case 'i': {
				iWordNum = va_arg(args, int);
                str = va_arg(args, const char*);
				word = getWordByIndex(str, iWordNum);//функція для пошуку слова в реченні
				printf("String %d (i): %s\n", i++, word ? word : "No such word", iWordNum);
                break;
			}
            case 'd':
				dWordNum = va_arg(args, double);
				str = va_arg(args, const char*);
                word = getWordByIndex(str, Round(dWordNum));//функція для пошуку слова в реченні
				printf("String %d (d): %s\n", i++, word ? word : "No such word", Round(dWordNum));
				break;
            case 's': 
				str = va_arg(args, const char*);
				word = getWordByIndex(str, 1);//функція для пошуку слова в реченні
				printf("String %d (s): %s\n", i++, word ? word : "No such word");
				break;
        default:
            break;
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

void printSpecificWordByNumg(int firstWordNum, ...) {
    int wordNum;
    int* pStack = (int*)&firstWordNum;
    int slot = *pStack; 
    pStack++; 
    int i = 1;

    printf("Nostdarg mode, word:\n");

    while (slot != 0) {
        if (slot > 0xFFFF) {
            const char* str = (const char*)slot;
            if (str == NULL) break;
            char* word = getWordByIndex(str, 1);
            if (word) {
                printf("Pair %d (Index %d): %s\n", i++, 1, word);
                free(word);
            }
            else {
                printf("Pair %d: No word at index %d\n", i++, 1);
            }
            slot = *pStack++;
        }
        else {
            wordNum = (int)slot;
            if (wordNum == 0) {
				printf("Terminator reached at pair %d\n", i);
                break;
            } 
            int slot2 = *pStack++;
            if (slot2 == 0) {
                printf("Terminator reached at pair %d\n", i);
                break;
            } 
            const char* str = (const char*)slot2;
            if (str == NULL) continue;
            char* word = getWordByIndex(str,wordNum);
            if (word) {
                printf("Pair %d (Index %d): %s\n", i++, wordNum);
                free(word);
            }
            else {
                printf("Pair %d: No word at index %d\n", i++, wordNum);
            }
            slot = *pStack++;
        }
    }
}