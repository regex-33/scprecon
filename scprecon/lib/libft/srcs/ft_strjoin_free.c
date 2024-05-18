#include "libft.h"


static char	*ft_strjoin_free_part2(char *s1, const char *s2, char **dest)
{
	int	len_s1;
	int	len_s2;

	if (!s1)
	{
		len_s2 = ft_strlen(s2);
		*dest = (char *)malloc(sizeof(char) * len_s2 + 1);
		if (!dest)
			return (NULL);
		ft_strcpy(*dest, s2);
	}
	else
	{
		len_s1 = ft_strlen(s1);
		*dest = (char *)malloc(sizeof(char) * len_s1 + 1);
		if (!*dest)
			return (NULL);
		ft_strcpy(*dest, s1);
        free(s1);
	}
	return (*dest);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*dest;

	if (!s1 && !s2)
		return (NULL);
	if (s1 && s2)
	{
		len_s1 = ft_strlen(s1);
		len_s2 = ft_strlen(s2);
		dest = (char *)malloc(sizeof(char) * len_s1 + len_s2 + 1);
		if (!dest)
			return (0);
		ft_strcpy(dest, s1);
		ft_strcat(dest, s2);
        free(s1);
	}
	else
	{
		if (!ft_strjoin_free_part2(s1, s2, &dest))
			return (NULL);
	}
	return (dest);
}
