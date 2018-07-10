#include <stdio.h>

int main(int argc, char** argv) {
  int a = 2;
  int b = 2;
  int product = 0;
  printf("%d x %d ", a, b);

  do {
    product = product + a;
    b = b - 1;
  } while (b != 0);

  printf("= %d\n", product);
  return 0;
}


/*
# r0: product
# r1: a
# r2: b
# r3: constant 1

mult:
0x00: data r1          0001 0100
0x01: uint #2          0000 0010
0x02: data r2          0001 1000
0x03: uint #2          0000 0010
0x04: data r3          0001 1100
0x05: uint #1          0000 0001

loop:
0x06: add  r0, r1      1000 0001
0x07: sub  r2, r3      1001 1011
0x08: bnz  loop        0101 0010
0x09: uint 0x06        0000 0110

end:
0x0a: halt             0110 0000
*/
