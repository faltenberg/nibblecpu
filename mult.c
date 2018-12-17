#include <stdio.h>


int main(int argc, char** argv) {
  int a = 4;
  int b = 3;
  int product = 0;
  printf("%d x %d ", a, b);

  while (b != 0) {
    product = product + a;
    b = b - 1;
  }

  printf("= %d\n", product);
  return 0;
}


/*
[0x00]: res
[0x01]: a
[0x02]: b

mult:
0x00: mov 0       0100 0000
0x01: str [0x00]  0111 0000
0x02: mov a       0100 aaaa
0x03: str [0x01]  0111 0001
0x04: mov b       0100 bbbb
0x05: str [0x02]  0111 0010

loop:
0x06: ld  [0x02]  0101 0010
0x07: bz  0x0E    1000 1110
0x08: sub 1       0010 0001
0x09: str [0x02]  0111 0010
0x0A: ld  [0x00]  0101 0000
0x0B: add [0x01]  0001 0001
0x0C: str [0x00]  0111 0000
0x0D: jmp 0x06    1100 0110

end:
0x0E: halt        1110 0000
*/
