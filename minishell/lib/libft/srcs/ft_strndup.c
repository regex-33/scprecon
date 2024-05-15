#include "libft.h"

char    *ft_strndup(const char *s, size_t n)
{
    char    *str;
    size_t  len;

    len = ft_strnlen(s, n);
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
    ft_memcpy(str, s, len);
    str[len] = '\0';
    return (str);
}