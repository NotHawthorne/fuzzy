#include "fuzz.h"

int		seed_random(void)
{
	int		fd;
	char	buf[8];
	int		i;
	int		ret;

	i = 0;
	ret = 5381;
	fd = open("/dev/random", O_RDONLY);
	read(fd, &buf, 7);
	buf[7] = 0;
	while (i < 7)
		ret = ((ret << 5) + buf[i++]) + ret;
	return (ret);
}

t_arg	*gen_int(t_constraint *constraints)
{
	int				ret;
	t_constraint	*tmp;
	t_arg			*new;

	tmp = constraints;
	ret = seed_random();
	new = malloc(sizeof(*new));
	new->next = NULL;
	new->type = INT;
	new->data.d = ret;
	while (tmp)
	{
		new->data.d = ret;
		if (tmp->cmp(new, tmp->cmp_against) < 0)
		{
			printf("loopback\n");
			ret = ((ret << 5) + ret);
			tmp = constraints;
		}
		else
			tmp = tmp->next;
	}
	return (new);
}

char	*create_str(int size)
{
	char	*new;
	int		seed;
	int		i;

	if (size < 0)
		size = -size;
	new = malloc(size);
	i = 0;
	seed = seed_random();
	while (i < size)
	{
		new[i++] = (seed % 127) + 1;
		seed = ((seed << 5) + seed);
	}
	new[i - 1] = 0;
	return (new);
}

t_arg	*gen_str(t_constraint *constraints)
{
	t_constraint	*tmp;
	t_arg			*new;

	tmp = constraints;
	new = malloc(sizeof(*new));
	new->next = NULL;
	new->type = STR;
	new->data.s = create_str(seed_random() % MAX_DEFAULT_RAND_STR_LEN);
	while (tmp)
	{
		if (tmp->cmp(new, tmp->cmp_against) < 0)
		{
			printf("loopback\n");
			free(new->data.s);
			new->data.s = create_str(seed_random() % MAX_DEFAULT_RAND_STR_LEN);
			tmp = constraints;
		}
		else
			tmp = tmp->next;
	}
	return (new);

}
