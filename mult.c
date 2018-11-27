T#include <stdio.h>

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
[0x00]: res
[0x01]: a
[0x02]: b

mult:
0x00: nop         0000 0000
0x01: mov 0       0100 0000
0x02: str [0x00]  0111 0000
0x03: mov a       0100 aaaa
0x04: str [0x01]  0111 0001
0x05: mov b       0100 bbbb
0x06: str [0x02]  0111 0010
0x07: nop         0000 0000

loop:
0x08: ld  [0x00]  0101 0000
0x09: add [0x01]  0001 0001
0x0A: str [0x00]  0111 0000
0x0B: ld  [0x02]  0101 0010
0x0C: sub 1       0010 0001
0x0D: str [0x02]  0111 0010
0x0E: bnz 0x08    1010 1000

end:
0x0F: halt        1110 0000
*/
