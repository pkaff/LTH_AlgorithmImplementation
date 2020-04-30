#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

static unsigned long long	fm_count;
static volatile bool		proceed = false;

static void done(int unused)
{
	proceed = false;
	unused = unused;
}

typedef struct rat rat;
  struct rat {
    int a;
    int b;
  };

void reduce(int* a, int* b);
int gcd(int a, int b);
rat addq(rat* r1, rat* r2);
rat subq(rat* r1, rat* r2);
rat mulq(rat* r1, rat* r2);
rat divq(rat* r1, rat* r2);
int sign(rat* r1);

unsigned long long make_solution(int n1, int n2, int rows, rat* c);
unsigned long long fm_elim(int rows, int cols, int** a, int* c);
	
unsigned long long danielUlricaNils_fm(char* aname, char* cname, int seconds)
{
	FILE*		afile = fopen(aname, "r");
	FILE*		cfile = fopen(cname, "r");

	fm_count = 0;

	if (afile == NULL) {
		fprintf(stderr, "could not open file A\n");
		exit(1);
	}

	if (cfile == NULL) {
		fprintf(stderr, "could not open file c\n");
		exit(1);
	}
	
	  int rows;
	  int cRows;
	  int cols;

	  fscanf(afile, "%d", &rows);
	  fscanf(afile, "%d", &cols);
	  fscanf(cfile, "%d", &cRows);
	  int k;
	  int** a = malloc(rows*sizeof(int*));
	  for (k = 0; k < rows; k++) {
	    a[k] = malloc(cols*sizeof(int));
	  }

	  int* c = malloc(cRows*sizeof(int));

	  int j;
	  for (k = 0; k < rows; k++) {
	    for (j = 0; j < cols; j++) {
	      fscanf(afile, "%d", &a[k][j]);	      
	    }
	    fscanf(cfile, "%d", &c[k]);
	  }
			
	  if (seconds == 0) {
	    unsigned long long elim = fm_elim(rows, cols, a, c);
	    for (k = 0; k < rows; k++) {
	      free(a[k]);
	    }
	    free(a);
	    free(c);
	    fclose(afile);
	    fclose(cfile);
	    return elim;
	  }

	/* Tell operating system to call function DONE when an ALARM comes. */
	signal(SIGALRM, done);
	alarm(seconds);

	/* Now loop until the alarm comes... */
	proceed = true;
	while (proceed) {
		fm_elim(rows, cols, a, c);

		fm_count++;
	}
	for (k = 0; k < rows; k++) {
	  free(a[k]);
	}
	free(a);
	free(c);
	fclose(afile);
	fclose(cfile);
	return fm_count;
}

unsigned long long fm_elim(int rows, int cols, int** a, int* c) {

  int r = rows;
  int s = cols;
  int k = 0;
  int i = 0;

  rat** T = malloc(rows*sizeof(rat*));
  for (i = 0; i < rows; i++) {
    T[i] = malloc(cols*sizeof(rat));
  }

  rat* q = malloc(rows*sizeof(rat));

  for (k = 0; k < r; k++) {
    for (i = 0; i < s; i++) {
      T[k][i].a = a[k][i];
      T[k][i].b = 1;
    }
    q[k].a = c[k];
    q[k].b = 1;
  }


  while (1) {
    
    int n1 = 0;
    int n2 = 0;

    int k = 0;
    int i = 0;
    int j = 0;

    int sig = 0;
    for (k = 0; k < r; k++) {
      sig = sign(&(T[k][s-1]));
      if (sig == 1) {
	n1++;
      } else if (sig == -1) {
	n2++;
      }
    }
    n2 += n1;

    int currentPosRow = 0;
    int currentNegRow = n1;
    int currentZerRow = n2;
    rat** temp = malloc(r*sizeof(rat*));
    for (i = 0; i < r; i++) {
      temp[i] = malloc(s*sizeof(rat));
    }
    rat* qTemp = malloc(r*sizeof(rat));

    for (k = 0; k < r; k++) {
      sig = sign(&T[k][s-1]);
      if (sig > 0) {
	for (i = 0; i < s; i++) {
	  temp[currentPosRow][i] = T[k][i];
	}
	qTemp[currentPosRow] = q[k];
	currentPosRow++;
      } else if (sig < 0) {
	for (i = 0; i < s; i++) {
	  temp[currentNegRow][i] = T[k][i];
	}
	qTemp[currentNegRow] = q[k];
	currentNegRow++;
      } else {
	for (i = 0; i < s; i ++) {
	  temp[currentZerRow][i] = T[k][i];
	}
	qTemp[currentZerRow] = q[k];
	currentZerRow++;
      }
    }

    for (k = 0; k < r; k++) {
      for (i = 0; i < s; i++) {
	T[k][i] = temp[k][i];
      }
      q[k] = qTemp[k];
    }

    for (i = 0; i < r; i++) {
      free(temp[i]);
    }
    free(temp);
    free(qTemp);

    for (i = 0; i < n2; i++) {
      q[i] = divq(&q[i], &T[i][s-1]);
      for (j = 0; j < s; j++) {
	T[i][j] = divq(&T[i][j], &T[i][s-1]);
      }
    }

 
    if (s == 1) {
      unsigned long long sol = make_solution(n1, n2, r, q);
      for (i = 0; i < r; i++) {
	free(T[i]);
      }
      free(T);
      free(q);
      return sol;
    }

    /* Elimination */
    int oldR = r;
    r = r - n2 + n1*(n2 - n1);
    if (r == 0) {
      for (i = 0; i < r; i++) {
	free(T[i]);
      }
      free(T);
      free(q);
      return 1;
    }
    s = s - 1;
    rat(** elim) = malloc(r*sizeof(rat*));
    for (i = 0; i < r; i++) {
      elim[i] = malloc(s*sizeof(rat));
    }
    rat* cTemp = malloc(r*sizeof(rat));
    int curRow = 0;

    for (k = 0; k < n1; k++) {
      for (j = n1; j < n2; j++) {
	for (i = 0; i < s; i++) {
	  elim[curRow][i] = subq(&T[k][i], &T[j][i]);
	}
	cTemp[curRow] = subq(&q[k], &q[j]);
	curRow++;
      }
    }

    for (k = n2; k < oldR; k++) {
      for (i = 0; i < s; i++) {
	elim[curRow][i] = T[k][i];
      }
      cTemp[curRow] = q[k];
      curRow++;
    }
    
    for (i = 0; i < oldR; i++) {
      free(T[i]);
    }
    T = realloc(T, r*sizeof(rat*));
    for (i = 0; i < r; i++) {
      T[i] = malloc(s*sizeof(rat));
    }
    
    q = realloc(q, r*sizeof(rat));
    if (T == NULL) {
      fprintf(stderr, "T out of memory \n");
      exit(1);
    }
    if (q == NULL) {
      fprintf(stderr, "q out of memory \n");
      exit(1);
    }

    for (k = 0; k < r; k++) {
      for (i = 0; i < s; i++) {
	T[k][i] = elim[k][i];
      }
      q[k] = cTemp[k];
    }

    for (i = 0; i < r; i++) {
      free(elim[i]);
    }
    free(elim);
    free(cTemp);
  }
}

unsigned long long  make_solution(int n1, int n2, int rows, rat* c) {
  int k;
  int j;

  for (k = n2; k < rows; k++) {
    if (sign(&c[k]) < 0) {
      return (unsigned long long)0;
    }
  }
 
  rat rat;
  for (k = 0; k < n1; k++) {
    for (j = n1; j < n2; j++) {
      rat = subq(&c[k], &c[j]);
      if (sign(&rat) < 0) {
	return (unsigned long long)0;
      }
    }
  }

  return (unsigned long long)1;
}


void reduce(int* a, int* b) {
  int g = gcd(*a, *b);
  *a = *a/g;
  *b = *b/g;
}

int gcd(int a, int b) {
  if (b == 0) {
    return a;
  } else {
    int temp = a%b;
    return gcd(b, temp);
  }
}

rat addq(rat* r1, rat* r2) {
  rat r3;
  r3.a = (*r1).a*(*r2).b + (*r2).a*(*r1).b;
  r3.b = (*r1).b*(*r2).b;
  reduce(&(r3.a), &(r3.b));
  return r3;
}

rat subq(rat* r1, rat* r2) {
  rat r3;
  r3.a = (*r1).a*(*r2).b - (*r2).a*(*r1).b;
  r3.b = (*r1).b*(*r2).b;
  reduce(&(r3.a), &(r3.b));
  return r3;
}

rat mulq(rat* r1, rat* r2) {
  rat r3;
  r3.a = (*r1).a*(*r2).a;
  r3.b = (*r1).b*(*r2).b;
  reduce(&(r3.a), &(r3.b));
  return r3;
}

rat divq(rat* r1, rat* r2) {
  rat r3;
  r3.a = (*r1).a*(*r2).b;
  r3.b = (*r1).b*(*r2).a;
  reduce(&(r3.a), &(r3.b));
  return r3;
}

int sign(rat* r1) {
  if (r1->a == 0) {
    return 0;
  }
  if (!(r1->a < 0 ^ r1->b < 0)) {
    return 1;
  }
  return -1;
}
