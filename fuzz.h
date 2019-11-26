#ifndef FUZZ_H
# define FUZZ_H

# include <stdio.h>
# include <unistd.h>
# include <stdint.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <stdarg.h>

enum	e_lexme
{
	PERCENT,
	DATATYPE,
	OPENCURLY,
	CLOSECURLY,
	OPENPAR,
	CLOSEPAR,
	PLUS,
	MINUS,
	MULT,
	DIV,
	MOD,
	EQ,
	DOLLAR,
	VARIABLE,
	EXCL,
	GREAT,
	LESS,
	GREATEQ,
	LESSEQ,
	QUOTE
};

enum	e_ast_node
{
	EXPR,
	ARG,
	TYPE,
	CONSTR,
	NOT,
	CMP,
	VAR,
	NUMBER,
	STRING,
	CMP_OP,
	ARG_NO
};

void	*fuzz(void (*f)(void), const char *fmt, ...);

#endif
