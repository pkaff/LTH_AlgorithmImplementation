#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bp() {};

struct {
	double				zero;
	int				i;
        char				s[6];
	char				t[6];
	unsigned long long		x;
	unsigned long long		y;
} a = { 0, 1756, "mozart", "???", 8243124871055238688ull };
//s should contain 7 bytes as a string is supposed to be ended with a 0-byte, e.g. mozart\0 which is 7 chars. Same for t if we want to copy s into t. the forever comes from the unsigned long long x which actually spells forever if you interpret the number as a string.

size_t strlen(const char* s)
{
	int		i;
	
	i = 0;

	while (s[i] != 0) {
		if (s[i] == 't') {
			bp();
		}	
	i++;
	}
	return i;
}
	
int main()
{
	int	length;

	strncpy(a.t, a.s, 6);
	
	length = strlen(a.t);

	printf("a.t is %s with length %d\n", a.t, length);

	return 0;
}
