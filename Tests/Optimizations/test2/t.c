int main()
{
int i;
char C[2];
char A[10];
/* ... */
for (i = 0; i != 10; ++i) {
  C[0] = A[i];          /* One byte store */
  C[1] = A[9-i];        /* One byte store */
}
}
