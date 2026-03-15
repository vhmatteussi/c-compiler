/*  
    Autor:  ChatGPT 
    Chat:   https://chatgpt.com/share/69b6cd50-9874-8013-a51a-d691e2334bdb
    Notas:  Ele esqueceu de botar digrafos, mas acho que ta funcionando no meu código.
            Tem alguns outros esquecimentos, mais tarde vou fazer um melhor só pro lexer, mas acho
            que da pra considerar q meu lexer ta funcionando no momento. Esse código vai ser bom para
            testar o parser e o restante do compilador no futuro, então vou manter. Foi bom pra saber
            q os hexadecimais estão funcionando, é muito chato as regras de float hex do C99.
*/

// Teste de lexer C99 sem trigrafos

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <complex.h>
#include <stdbool.h>

#define SQR(x) ((x) * (x))
#define PRINTF(fmt, ...) printf("LOG: " fmt "\n", __VA_ARGS__)
#define STR(x) #x
#define CONCAT(a,b) a##b

_Pragma("GCC diagnostic ignored \"-Wunused-variable\"")

static inline int add_inline(int a, int b) {
    return a + b;
}

void variadic_example(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

int sum_array(int n, int arr[restrict static 1]) {
    int s = 0;
    for (int i = 0; i < n; i++)
        s += arr[i];
    return s;
}

int main(void) {

    // Tipos incomuns
    _Bool flag = 1;
    long long big = 9223372036854775807LL;
    unsigned long long ubig = 18446744073709551615ULL;

    // Float hexadecimal (C99)
    double hx = 0x1.8p3;     // 1.5 * 2^3 = 12
    float hf = 0x1.2p2f;

    // Complex numbers
    double _Complex z = 1.0 + 2.0*I;

    // Literais de caractere
    char c1 = 'a';
    char c2 = '\n';
    char c3 = '\x41';
    char c4 = '\101';

    // String escapes
    const char *str = "linha1\nlinha2\t\"quoted\"";

    // Designated initializers
    int table[10] = {
        [0] = 1,
        [5] = 42,
        [9] = 7
    };

    struct Point {
        int x;
        int y;
    };

    struct Point p = {.y = 5, .x = 3};

    // Compound literal
    struct Point *pp = &(struct Point){ .x = 10, .y = 20 };

    // VLA
    int n = 5;
    int vla[n];

    for (int i = 0; i < n; i++) {
        vla[i] = i * i;
    }

    int total = sum_array(n, vla);

    // Macro variádica
    PRINTF("total = %d", total);

    // Macro stringification
    printf("macro text: %s\n", STR(hello_world));

    // Token concatenation
    int CONCAT(my,Var) = 123;
    printf("concat var = %d\n", myVar);

    // Compound literal como argumento
    int result = sum_array(3, (int[]){1,2,3});

    // Operadores incomuns
    int a = 5, b = 2;
    int r = (a > b) ? a : b;

    // Operadores bitwise
    r ^= 0xFF;
    r <<= 2;
    r |= 0b1010; // extensão comum suportada por alguns compiladores
    // vhm: 0b não é padrão de C99 (só foi adicionado no C23 a 2 anos atrás se n me engano), não vou adicionar.
    //      Se algum dia isso virar um compilador de C23 eu fui atropelado por um trem.

    // Inline function
    printf("inline add: %d\n", add_inline(3,4));

    // Variadic function
    variadic_example("variadic %d %f\n", 10, 3.14);

    // sizeof com expressão
    printf("sizeof expr: %zu\n", sizeof(a + b));

    // ponteiros e casts
    void *ptr = &p;
    struct Point *casted = (struct Point *)ptr;

    printf("point: %d %d\n", casted->x, casted->y);

    // _Bool comportamento
    if (flag && !0) {
        puts("bool works");
    }

    return EXIT_SUCCESS;
}