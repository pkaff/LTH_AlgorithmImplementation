/* Warning: this program contains many errors and is used for a lab on Valgrind. */

#include <stdlib.h>
#include <stdio.h>

#define fail(a)		((test == 0 || test == a) ? fail##a() : 0)

#define N		(10)

int	a[N] = { 1 };
int*	b = &a[0];

void fail1()
{
	printf("a[0] = %d\n", a[0]);
	printf("b[0] = %d\n", b[0]);
	printf("*b   = %d\n", *b);
	*b = 2;
	a[N] = 3; //overwrites b
	printf("*b = %d\n", *b); //cannot read &3 
}


void fail2()
{
	int*	a = calloc(N, sizeof(int));
	int*	b = &a[0];
	
	a[0] = 2;
	printf("a[0] = %d\n", a[0]);
	printf("b[0] = %d\n", b[0]);
	printf("*b   = %d\n", *b);
	*b = 2;
	a[N] = 3; //Cannot access a[10] since allocated only 40 bytes
	printf("*b = %d\n", *b);
	//free(a); Must free allocated memory
}

void fail3()
{
	int*		a;
	int*		b;
	long long*	c;
	int		i;

	a = calloc(N, sizeof(int));
	b = calloc(N, sizeof(int));
	c = calloc(N, sizeof(int)); //must be sizeof(long long)

	for (i = 0; i < N; ++i) {//should be i++?
		a[i] = i;
		b[i] = i;
		c[i] = i;
	}

	//free(a); Must free allocated memory
	//free(b);
	//free(c);
}

void fail4()
{
	int*		a;
	int*		b;
	int*		c;

	a  = b = calloc(N, sizeof(int));
	c = &a[N/2];

	free(NULL); //Does nothing?
	free(&a); //what does that even mean?
	free(a);
	free(b); //b has the same allocated memory as a
	free(c); //c never allocated memory for
}

void fail5()
{
	int**		a;
	int*		b;
	int		i;

	a = calloc(N, sizeof(int*));

	for (i = 0; i < N; i++)
		a[i] = calloc(N, sizeof(int));

	free(a);// must free this after freeing a[i] since memory is not allocated inbetween memory in the array a but somewhere else. the information about where that memory is allocated is lost when we free a and therefore we can't free a[i].

	for (i = 0; i < N; i++)
	  free(a[i]);
}

void fail6()
{
	int*		a;

	a = alloca(N * sizeof(int));

	a[N] = 1;

	free(a); //alloca doesn't allocate memory permanently
}

int main(int argc, char **argv)
{
	int	test = 0;

	if (argc > 1) {
		sscanf(argv[1], "%d", &test);
		printf("doing test %d\n", test);
	} else
		puts("doing all tests");
		
	fail(1);
	fail(2);
	fail(3);
	fail(4);
	fail(5);
	fail(6);

	puts("lab6 reached the end");

	exit(0);
}
