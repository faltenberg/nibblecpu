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
s  0   1   1   0   0


ALU
---

Operations:
add
sub
inc
dec
cmp


Instructions
------------

ifetch
  S1: pc_r=1; a_w=1; b_en=0; op=0; cin=1; d_w=1; mar_w=1;
  S2: d_r=1; pc_w=1;
  S3: mem_r=1; ir_w=1;

0000 ra rb    mov  ra, rb
  S4: rb_r=1; a_w=1;
  S5: b_en=0; op=0; cin=0; d_w=1;
  S6: d_r=1; ra_w=1;

0001 ra xx    data ra, imm8
  S4: pc_r=1; a_w=1; mar_w=1; b_en=0; op=0; cin=1; d_w=1;
  S5: d_r=1; pc_w=1;
  S6: mem_r=1; ra_w=1;
    
0010 ra rb    ldr  ra, [rb]
  S4: rb_r=1; mar_w=1;
  S5: mem_r=1; ra_w=1;
  S6:

0011 ra rb    str  ra, [rb]
  S4: rb_r=1; mar_w=1;
  S5: ra_r=1; mem_w=1;
  S6:

0101 00 00    br   imm8
0101 00 01    brz  imm8
0101 00 10    brnz imm8
  S4: pc_r=1; a_w=1; mar_w=1; b_en=0; op=0; cin=1; d_w=1;
  S5: d_r=1; pc_w=1;
  S6: br_en=1; if cond==flags: mem_r=1; pc_w=1;

0110 xx xx    halt
  S4:
  S5:
  S6: halt=1;

1000 ra rb    add  ra, rb
  S4: ra_r=1; a_w=1;
  S5: rb_r=1; b_en=1; op=0; cin=0; fr_w=1; d_w=1;
  S6: d_r=1; ra_w=1;

1001 ra rb    sub  ra, rb
  S4: ra_r=1; a_w=1;
  S5: rb_r=1; b_en=1; op=1; cin=0; fr_w=1; d_w=1;
  S6: d_r=1; ra_w=1;


Microinstruction
----------------

µInstr[ 0]: r0_w
µInstr[ 1]: r0_r
µInstr[ 2]: r1_w
µInstr[ 3]: r1_r
µInstr[ 4]: r2_w
µInstr[ 5]: r2_r
µInstr[ 6]: r3_w
µInstr[ 7]: r3_r

µInstr[ 8]: b_en
µInstr[ 9]: ra_w
µInstr[10]: rd_w
µInstr[11]: rd_r
µInstr[12]: cin
µInstr[13]: sub
µInstr[14]: fr_w
µInstr[15]:

µInstr[16]: pc_w
µInstr[17]: pc_r
µInstr[18]: ir_w
µInstr[19]:

µInstr[20]: mar_w
µInstr[21]: mem_w
µInstr[22]: mem_r
µInstr[23]: 
