# fuzzy
Fuzzy is a currently work-in-progress fuzzing library for C language functions. The current function prototype is as follows.
```
void  *fuzz(void (*f)(void), const char *fmt, ...);
```
The format string language is as follows:
```
"%s" - STRING
"%d" - INT
"%d={>15;<212;%%2}" - INT that will be between 16 and 211, and odd.
"%dp={>5152}" - INT array where elements will be bigger than 5152
```
