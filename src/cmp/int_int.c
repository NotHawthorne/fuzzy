#include "fuzz.h"

int		fuzzy_int_cmp_g(t_arg *arg, t_arg *against)
{
	return (arg ? (arg->data->d > data) : 0);
}

int		fuzzy_int_cmp_ge(t_arg *arg, t_arg *against)
{
	return (arg ? (arg->data->d >= data) : 0);
}

int		fuzzy_int_cmp_l(t_arg *arg, t_arg *against)
{
	return (arg ? (arg->data->d < data) : 0);
}

int		fuzzy_int_cmp_le(t_arg *arg, t_arg *against)
{
	return (arg ? (arg->data->d <= data) : 0);
}

int		fuzzy_int_cmp(t_arg *arg, t_arg *against, char *cmp)
{
	if (arg->type != INT)
		return (0);
	if (!strncmp(cmp, ">=", 2))
		return (fuzzy_int_cmp_ge(arg, against));
	else if (!strncmp(cmp, "<=", 2))
		return (fuzzy_int_cmp_le(arg, against));
	else if (!strncmp(cmp, "<", 1))
		return (fuzzy_int_cmp_l(arg, against));
	else if (!strncmp(cmp, ">", 1))
		return (fuzzy_int_cmp_g(arg, against));
	return (0);
}
