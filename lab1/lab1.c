#include <stdio.h>

int main(int argc, char* argv[])
{
	int	a;
	int sum = 0;
	FILE* fp;
	fp = fopen("data.txt", "w"); //open the file for writing

	int k = 1;
	for (k; k < argc; k++) {
	  
	  sscanf(argv[k], "%d", &a);
	  sum += a;
	  fprintf(fp, "Argument number %d is %s\n", k, argv[k]);
	}

	int i = 1;
	int temp;
	for (i; i < argc - 1; i++) {
	  sscanf(argv[i], "%d", &temp);
	  fprintf(fp, "%d + ", temp);
	}
	sscanf(argv[argc-1], "%d", &temp);
	fprintf(fp, "%d = ", temp);

	fprintf(fp, "%d\n",  sum);

	fclose(fp);

	return 0;
}
