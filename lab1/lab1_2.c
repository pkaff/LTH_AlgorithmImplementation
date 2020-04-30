#include <stdio.h>
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


int main(int argc, char* argv[])
{
  rat r;
  rat s;

  printf("Enter four numbers: ");
  scanf("%d %d %d %d", &(r.a), &(r.b), &(s.a), &(s.b));
  
  rat sum = addq(&r, &s);
  rat sub = subq(&r, &s);
  rat mul = mulq(&r, &s);
  rat div = divq(&r, &s);

  printf("Sum is: %d/%d\n", sum.a, sum.b);
  printf("Diff is: %d/%d\n", sub.a, sub.b);
  printf("Prod is: %d/%d\n", mul.a, mul.b);
  printf("Quot is: %d/%d\n", div.a, div.b);

  return 0;

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
