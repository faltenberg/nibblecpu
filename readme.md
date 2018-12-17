Nibble CPU
==========

The Nibble CPU is a simple 4bit processor for teaching purposes. Version 0 is based on a shared-bus
architecture and uses latches. Version 1 has a datapath and uses flip-flops, thus being faster.
Both CPUs have the following characteristics:

- 4bit ALU for addtion and subtraction
- 4bit instruction code + 4bit of immediate value
- 2 addressing modes (immediate, absolute)
- 16 half-bytes of RAM
- 16 bytes of ROM for code
- 2 clock ticks per cycle
- v0: 7 cylces per instruction, v1: 2 cycles per instruction


Usage
-----

The CPU is implemented with the [Logisim](http://www.cburch.com/logisim) software. Open the *.circ*
files with that tool and read the documentation on the website or watch videos on Youtube for more
information regarding the usage of Logisim.


Registers
---------

```
ACC |....|
PC  |....|
IR  |....|
```


Instructions
------------

```
        |7 6 5 4 3 2 1 0|
Format: | op  |m|  imm  |
```

```
0000 imm  -  add imm    -  ACC = ACC + imm
0001 imm  -  add [imm]  -  ACC = ACC + RAM[imm]
0010 imm  -  sub imm    -  ACC = ACC - imm
0011 imm  -  sub [imm]  -  ACC = ACC - RAM[imm]
0100 imm  -  mov imm    -  ACC = imm
0101 imm  -  ld  [imm]  -  ACC = RAM[imm]
011x imm  -  str [imm]  -  RAM[imm] = ACC
1000 imm  -  bz  imm    -  if ACC == 0: PC = imm
1001 imm  -  bz  [imm]  -  if ACC == 0: PC = RAM[imm]
1010 imm  -  bnz imm    -  if ACC != 0: PC = imm
1011 imm  -  bnz [imm]  -  if ACC != 0: PC = RAM[imm]
1100 imm  -  jmp imm    -  PC = imm
1101 imm  -  jmp [imm]  -  PC = RAM[imm]
111x imm  -  halt       -  ignore clock
```
