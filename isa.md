Instruction Set Architecture
============================

Clocking
--------

     +---+   +---+
     |   |   |   |
     |   |   |   |
c ---+   +---+   +---
r  0   1   1   1   0
w  0   0   1   0   0
c  0   1   1   0   0


ALU
---

Operations:
add
sub
mov
inc
dec


Registers
---------

ACC |....| -> FR[z]
PC  |....|
IR  |....|
TMP |....|
RES |....|
MAR |....|


Instructions
------------

        |7 6 5 4 3 2 1 0|
Format: | op  |m|  imm  |

0000 imm  -  add imm    -  ACC = ACC + imm
0001 imm  -  add [imm]  -  ACC = ACC + [imm]
0010 imm  -  sub imm    -  ACC = ACC - imm
0011 imm  -  sub [imm]  -  ACC = ACC - [imm]
0100 imm  -  mov imm    -  ACC = imm
0101 imm  -  ld  [imm]  -  ACC = [imm]
011x imm  -  str [imm]  -  [imm] = ACC
1000 imm  -  bz  imm    -  if ACC == 0: NPC = imm
1001 imm  -  bz  [imm]  -  if ACC == 0: NPC = [imm]
1010 imm  -  bnz imm    -  if ACC != 0: NPC = imm
1011 imm  -  bnz [imm]  -  if ACC != 0: NPC = [imm]
1100 imm  -  jmp imm    -  NPC = imm
1101 imm  -  jmp [imm]  -  NPC = [imm]
111x imm  -  halt       -  ignore clock


Microcode
---------

ifetch:
s1: pc_r=1, rommar_w=1, temp_w=1
s2: muxB=0, alu=add, cin=1, res_w=1, irom_r=1, ir_w=1
s3: res_r=1, pc_w=1

000x: add
s4: drom_r=1, mar_w=1
s5: acc_r=1, temp_w=1
s6: drom_r=~op[0], ram_r=op[0], muxB=1, alu=add, cin=0, res_w=1
s7: res_r=1, acc_w=1

001x: sub
s4: drom_r=1, mar_w=1
s5: acc_r=1, temp_w=1
s6: drom_r=~op[0], ram_r=op[0], muxB=1, alu=sub, cin=0, res_w=1
s7: res_r=1, acc_w=1

010x: mov/ld
s4: drom_r=1, mar_w=1
s5: acc_r=1
s6: drom_r=~op[0], ram_r=op[0], acc_w=1
s7: 

011x: str
s4: drom_r=1, mar_w=1
s5: acc_r=1, ram_w=1
s6: drom_r=~op[0], ram_r=op[0]
s7: 

100x: bz
s4: drom_r=1, mar_w=1
s5: 
s6: drom_r=~op[0], ram_r=op[0], pc_w=fr[z]
s7: 

101x: bnz
s4: drom_r=1, mar_w=1
s5: 
s6: drom_r=~op[0], ram_r=op[0], pc_w=~fr[z]
s7: 

110x: jmp
s4: drom_r=1, mar_w=1
s5: 
s6: drom_r=~op[0], ram_r=op[0], pc_w=1
s7: 

111x: halt
s4: drom_r=1, mar_w=1
s5: 
s6: drom_r=~op[0], ram_r=op[0]
s7: halt=1


+halt           s7 & (op==111x)

+rommar_w  s1
+irom_r    s2
+drom_r    s4 | s6 & ~op[0]

+pc_r      s1
+pc_w      s3 | s6 & (op==100x & fr[z] | op==101x & ~fr[z] | op==110x)
+ir_w      s2

+mar_w     s4
+ram_r          s6 & op[0]
+ram_w          s5 & (op==011x)

+acc_r          s5 & (op==0xxx)
+acc_w          s6 & (op==010x) | s7 & (op==00xx)

+temp_w    s1 | s5 & (op==00xx)
+muxB           s6 & (op==00xx)
+res_r     s3 | s7 & (op==00xx)
+res_w     s2 | s6 & (op==00xx)
+aluop          s6 & (op==xx1x)
+cin       s2
