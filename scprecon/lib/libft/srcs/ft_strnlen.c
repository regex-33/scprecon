#include "libft.h"

int ft_strnlen(const char *s, size_t maxlen)
{
    size_t i;

    i = 0;
    while (s[i] && i < maxlen)
        i++;
    return (i);
}