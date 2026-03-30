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
0x23.23
0x23.p

#endif

/*

Done with 14 errors:
---------------------------------------
at line: 31 and col: 1, invalid token: 0x29.3p3lf
at line: 32 and col: 1, invalid token: 0x29.3p+3lf
at line: 33 and col: 1, invalid token: 0x29.3p-3lf
at line: 34 and col: 1, invalid token: 0x.3p3lf
at line: 35 and col: 1, invalid token: 0x.3p+3lf
at line: 36 and col: 1, invalid token: 0x.3p-3lf
at line: 38 and col: 1, invalid token: 0x.p3
at line: 39 and col: 1, invalid token: 0x.p+3
at line: 40 and col: 1, invalid token: 0x.p-3
at line: 42 and col: 1, invalid token: 0x23p
at line: 43 and col: 1, invalid token: 0x
at line: 44 and col: 1, invalid token: 0x23.
at line: 45 and col: 1, invalid token: 0x23.23
at line: 46 and col: 1, invalid token: 0x23.p

*/