#include <stdlib.h>
#include <string.h>

char	*replace(char const *const original, char const *const pattern,
		char const *const replacement)
{
	size_t		patcnt;
	const char	*oriptr;
	const char	*patloc;
	char		*retptr;

	size_t const replen = strlen(replacement);
	size_t const patlen = strlen(pattern);
	size_t const orilen = strlen(original);
	patcnt = 0;
	// find how many times the pattern occurs in the original string
	for ((oriptr = original); (patloc = strstr(oriptr,
				pattern)); (oriptr = patloc + patlen))
	{
		patcnt++;
	}
	{
		// allocate memory for the new string
		size_t const retlen = orilen + patcnt * (replen - patlen);
		char *const returned = (char *)malloc(sizeof(char) * (retlen + 1));
		if (returned != NULL)
		{
			// copy the original string,
			// replacing all the instances of the pattern
			retptr = returned;
			for ((oriptr = original); (patloc = strstr(oriptr,
						pattern)); (oriptr = patloc + patlen))
			{
				size_t const skplen = patloc - oriptr;
				// copy the section until the occurence of the pattern
				strncpy(retptr, oriptr, skplen);
				retptr += skplen;
				// copy the replacement
				strncpy(retptr, replacement, replen);
				retptr += replen;
			}
			// copy the rest of the string.
			strcpy(retptr, oriptr);
		}
		return (returned);
	}
}

#include <stdio.h>

int	main(void)
{
	char *const newstr = replace("echo $target hello hello youssef hello $target",
			"$target", "world");
	if (newstr)
	{
		printf("%s\n", newstr);
		free(newstr);
	}
	else
	{
		fprintf(stderr, "allocation error\n");
		exit(-2);
	}
	return (0);
}
