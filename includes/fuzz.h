#ifndef FUZZ_H
# define FUZZ_H

# include <stdio.h>
# include <unistd.h>
# include <stdint.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <stdarg.h>

typedef enum
{
	NONE,
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
	QUOTE,
	LITQUOTE,
	SEMI,
	EQUALS,
	WHITESPACE,
	ARG_REF
} e_lexeme;

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

typedef struct	s_lexeme	t_lexeme;

struct s_lexeme
{
	char			*data;
	int				len;
	e_lexeme		type;
	struct s_lexeme	*next;
};

void		*fuzz(void (*f)(void), const char *fmt, ...);
t_lexeme	*fuzzy_lexer(char *format);
void		print_enum_type(t_lexeme *lexeme);

#endif
