/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -t -N lookup_keyword lib/include/keywords.gperf  */
/* Computed positions: -k'1,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "lib/include/keywords.gperf"

#include <string.h>
#include "token.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wpointer-sign"

typedef struct KeywordMap{
    const char *name;
    TokenType type;
} KeywordMap;
#line 15 "lib/include/keywords.gperf"
struct KeywordMap;

#define TOTAL_KEYWORDS 36
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 8
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 55
/* maximum key range = 52, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56,  0, 56, 10, 10, 20,
      25,  5, 15,  0,  0, 30, 56,  5,  0, 25,
      40,  5, 56, 56,  0,  0,  5, 10, 10,  5,
      20, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56
    };
  return len + asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]];
}

struct KeywordMap *
lookup_keyword (register const char *str, register size_t len)
{
  static struct KeywordMap wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 21 "lib/include/keywords.gperf"
      {"long", TOK_KW_LONG},
#line 51 "lib/include/keywords.gperf"
      {"_Bool", TOK_KW_BOOL},
#line 35 "lib/include/keywords.gperf"
      {"switch", TOK_KW_SWITCH},
      {""},
#line 31 "lib/include/keywords.gperf"
      {"register", TOK_KW_REGISTER},
#line 32 "lib/include/keywords.gperf"
      {"goto", TOK_KW_GOTO},
#line 20 "lib/include/keywords.gperf"
      {"short", TOK_KW_SHORT},
#line 45 "lib/include/keywords.gperf"
      {"struct", TOK_KW_STRUCT},
      {""},
#line 50 "lib/include/keywords.gperf"
      {"restrict", TOK_KW_RESTRICT},
#line 34 "lib/include/keywords.gperf"
      {"else", TOK_KW_ELSE},
#line 41 "lib/include/keywords.gperf"
      {"while", TOK_KW_WHILE},
      {""},
#line 53 "lib/include/keywords.gperf"
      {"_Pragma", TOK_KW_PRAGMA},
#line 42 "lib/include/keywords.gperf"
      {"for", TOK_KW_FOR},
#line 29 "lib/include/keywords.gperf"
      {"auto", TOK_KW_AUTO},
#line 38 "lib/include/keywords.gperf"
      {"break", TOK_KW_BREAK},
#line 48 "lib/include/keywords.gperf"
      {"sizeof", TOK_KW_SIZEOF},
      {""},
#line 30 "lib/include/keywords.gperf"
      {"volatile", TOK_KW_VOLATILE},
#line 18 "lib/include/keywords.gperf"
      {"char", TOK_KW_CHAR},
#line 22 "lib/include/keywords.gperf"
      {"float", TOK_KW_FLOAT},
#line 27 "lib/include/keywords.gperf"
      {"static", TOK_KW_STATIC},
#line 47 "lib/include/keywords.gperf"
      {"typedef", TOK_KW_TYPEDEF},
#line 52 "lib/include/keywords.gperf"
      {"_Complex", TOK_KW_COMPLEX},
#line 36 "lib/include/keywords.gperf"
      {"case", TOK_KW_CASE},
      {""},
#line 25 "lib/include/keywords.gperf"
      {"signed", TOK_KW_SIGNED},
#line 40 "lib/include/keywords.gperf"
      {"do", TOK_KW_DO},
#line 39 "lib/include/keywords.gperf"
      {"continue", TOK_KW_CONTINUE},
#line 44 "lib/include/keywords.gperf"
      {"enum", TOK_KW_ENUM},
      {""},
#line 23 "lib/include/keywords.gperf"
      {"double", TOK_KW_DOUBLE},
#line 37 "lib/include/keywords.gperf"
      {"default", TOK_KW_DEFAULT},
#line 19 "lib/include/keywords.gperf"
      {"int", TOK_KW_INT},
#line 24 "lib/include/keywords.gperf"
      {"void", TOK_KW_VOID},
      {""},
#line 49 "lib/include/keywords.gperf"
      {"inline", TOK_KW_INLINE},
      {""},
#line 26 "lib/include/keywords.gperf"
      {"unsigned", TOK_KW_UNSIGNED},
      {""}, {""},
#line 43 "lib/include/keywords.gperf"
      {"return", TOK_KW_RETURN},
#line 33 "lib/include/keywords.gperf"
      {"if", TOK_KW_IF},
      {""}, {""}, {""},
#line 28 "lib/include/keywords.gperf"
      {"extern", TOK_KW_EXTERN},
      {""}, {""}, {""},
#line 46 "lib/include/keywords.gperf"
      {"union", TOK_KW_UNION}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
