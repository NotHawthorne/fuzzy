NAME = tester
CC := gcc

all: $(NAME)

MODULES := src/lex src/fuzz src/exec includes
LIBDIRS :=
LIBS :=
CFLAGS += -Iincludes -Wall -Werror -Wextra #-g -fsanitize=address
MODNAME := module.mk
SRC :=

include $(patsubst %,%/$(MODNAME),$(MODULES))

OBJ :=  $(patsubst %.c,%.o,$(filter %.c,$(SRC)))
DEP :=	$(patsubst %.c,%.d,$(filter %.c,$(SRC)))

-include $(DEP)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $@

%.d : %.c
	@./depend.sh $*.o $(CFLAGS) $< > $@
	@printf '\t%s' "$(CC) $(CFLAGS) -c -o $*.o $<" >> $@
	@echo $@ >> all.log

clean:
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

clean_nolib:
	rm -f $(OBJ)
	rm -f $(shell cat all.log)
	@rm -f all.log

fclean: clean_nolib
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
