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
Done with 13 errors:
---------------------------------------
at line: 0 and col: 0, invalid token: 0x29.3p3lf
at line: 0 and col: 0, invalid token: 0x29.3p+3lf
at line: 0 and col: 0, invalid token: 0x29.3p-3lf
at line: 0 and col: 0, invalid token: 0x.3p3lf
at line: 0 and col: 0, invalid token: 0x.3p+3lf
at line: 0 and col: 0, invalid token: 0x.3p-3lf
at line: 0 and col: 0, invalid token: 0x.p3
at line: 0 and col: 0, invalid token: 0x.p+3
at line: 0 and col: 0, invalid token: 0x.p-3
at line: 0 and col: 0, invalid token: 0x23p
at line: 0 and col: 0, invalid token: 0x
at line: 0 and col: 0, invalid token: 0x23.
at line: 0 and col: 0, invalid token: 0x23.p
---------------------------------------
*/