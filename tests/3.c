#ifndef __INTELLISENSE__

// Lexer only
// Valid
0x29.3p3f;
0x29.3p+3f;
0x29.3p-3f;
0x.3p3f;
0x.3p+3f;
0x.3p-3f;

0x29.3p3;
0x29.3p+3;
0x29.3p-3;
0x.3p3;
0x.3p+3;
0x.3p-3;

0x29.3p3l;
0x29.3p+3l;
0x29.3p-3l;
0x.3p3l;
0x.3p+3l;
0x.3p-3l;

0x29.p3;
0x29.p+3;
0x29.p-3;

// Invalid
0x29.3p3lf;
0x29.3p+3lf;
0x29.3p-3lf;
0x.3p3lf;
0x.3p+3lf;
0x.3p-3lf;

0x.p3;
0x.p+3;
0x.p-3;

0x23p
0x
0x23.
0x23.p

#endif

/*
Current result:

Done with 9 errors:
---------------------------------------
at line: 18 and col: 9, invalid token: 0x29.3p3l
at line: 19 and col: 10, invalid token: 0x29.3p+3l
at line: 20 and col: 10, invalid token: 0x29.3p-3l
at line: 21 and col: 7, invalid token: 0x.3p3l
at line: 22 and col: 8, invalid token: 0x.3p+3l
at line: 23 and col: 8, invalid token: 0x.3p-3l
at line: 41 and col: 5, invalid token: 0x23p
at line: 43 and col: 5, invalid token: 0x23.
at line: 44 and col: 6, invalid token: 0x23.p
---------------------------------------

a lot to work on
*/