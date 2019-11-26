#ifndef FUZZ_H
# define FUZZ_H

# include <stdio.h>
# include <unistd.h>
# include <stdint.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <stdarg.h>

# define MAX_AST_CHILDREN 10

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

/*
** EXPR			:=	ARG | ARG, ARG
** ARG			:=	TYPE | TYPE, CONSTR | TYPE, EQ
** TYPE			:=	"%s" | "%da" | "%c" | "%d"
** CONSTR		:=	BLOCK_START, CMP, BLOCK_END
** NOT			:=	"!"
** CMP			:=	CMP_OP, VAR | CMP_OP, ARG_NO |
**					NOT, CMP_OP, VAR | NOT, CMP_OP, ARG_NO |
**					CMP_OP, VAR, SEP, CMP | CMP_OP, ARG_NO, SEP, CMP |
**					NOT, CMP_OP, VAR, SEP, CMP | NOT, CMP_OP, ARG_NO, SEP, CMP
** VAR			:=	NUMBER | STR
** NUMBER		:=	0-9 | 0-9, NUMBER
** STR			:=	string (\0 terminated char array)
** CMP_OP		:=	">" | "<" | ">=" | "<=" | "==" | "%%"
** ARG_NO		:=	"$", NUMBER
** MATH_OP		:=	"+" | "-" | "*" | "/"
** EQUALITY		:=	"="
** BLOCK_START	:=	"{" | "("
** BLOCK_END	:= 	"}" | ")"
** SEP			:=	";"
*/

typedef enum
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
	ARG_NO,
	EQUALITY,
	BLOCK_START,
	BLOCK_END,
	SEP,
	MATH_OP,
	EMPTY
} e_ast_node;

typedef struct	s_lexeme	t_lexeme;
typedef struct	s_ast_node	t_ast_node;

struct s_lexeme
{
	char			*data;
	int				len;
	e_lexeme		type;
	struct s_lexeme	*next;
	e_ast_node		dec;
};

struct s_ast_node
{
	struct s_lexeme		*lexeme;
	e_ast_node			type;
	struct s_ast_node	*parent;
	struct s_ast_node	*children[MAX_AST_CHILDREN];
};

void		*fuzz(void (*f)(void), const char *fmt, ...);
t_lexeme	*fuzzy_lexer(char *format);
void		print_enum_type(t_lexeme *lexeme);
t_ast_node	*fuzzy_parser(t_lexeme *lex);
void		fuzzy_ast_insert(t_ast_node **head, e_ast_node type, t_lexeme *lex);
void		fuzzy_ast_print(t_ast_node *head);

#endif
