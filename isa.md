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

states:
S0: pc->alu.a, 0->alu.b, alu, alu.d_w
S1: alu.d->pc, alu.d->mar
S2: mem->ir

S3: src->alu.a
S4: src->alu.b, alu, alu.d_w, (fr_w)
S5: alu.d->dest
S6: src->mar
S7: src->mem, mem->dest, return


ifetch
	S0: pc_r=1; a_w=1; b_en=0; op=0; cin=1; d_w=1;
	S1: d_r=1; pc_w=1; mar_w=1;
	S2: mem_en=1; mem_rw=0; ir_w=1;

0000 ra rb		mov  ra, rb
	S3: ra_r=1; a_w=1;
	S4: rb_r=1; b_en=1; op=0; cin=0; d_w=1;
	S5: d_r=1; ra_w=1;
	S6:
	S7: return;

0001 ra xx		data ra, imm8
	S3: pc_r=1; a_w=1;
	S4: b_en=0; op=0; cin=1; d_w=1;
	S5: d_r=1; pc_w=1;
	S6: pc_r=1; mar_w=1;
	S7: mem_en=1; mem_rw=0; ra_w=1; return;
		
0010 ra rb		ldr  ra, [rb]
	S3:
	S4:
	S5:
	S6: rb_r=1; mar_w=1;
	S7: mem_en=1; mem_rw=0; ra_w=1; return;

0011 ra rb		str  ra, [rb]
	S3:
	S4:
	S5:
	S6: rb_r=1; mar_w=1;
	S7: ra_r=1; mem_en=1; mem_rw=1; return;

0101 00 00		br   imm8
0101 00 01		brz  imm8
0101 00 10		brnz imm8
	S3: pc_r=1; a_w=1;
	S4: b_en=0; op=0; cin=1; d_w=1;
	S5: d_r=1; pc_w=1;
	S6: pc_r=1; mar_w=1;
	S7: if cond==flags: mem_en=1; mem_rw=0; pc_w=1;

0110 xx xx		halt
	S3:
	S4:
	S5:
	S6:
	S7: halt=1; return;

1000 ra rb		add  ra, rb
	S3: ra_r=1; a_w=1;
	S4: rb_r=1; b_en=1; op=0; cin=0; fr_w=1; d_w=1;
	S5: d_r=1; ra_w=1;
	S6:
	S7: return;

1001 ra rb		sub  ra, rb
	S3: ra_r=1; a_w=1;
	S4: rb_r=1; b_en=1; op=1; cin=0; fr_w=1; d_w=1;
	S5: d_r=1; ra_w=1;
	S6:
	S7: return;

1111 ra rb		cmp  ra, rb
	S3: ra_r=1; a_w=1;
	S4: rb_r=1; b_en=1; op=1; cin=0; fr_w=1; (d_w=1;)
	S5:
	S6:
	S7: return;


Microprogramming
----------------

- all microprograms executed in 8 steps
- all instructions perform one of two programs
		- increment pc OR perform operation on ra and rb
		- load from memory at address in pc/rb and store in ra
		- pc->alu
			ra->alu
		- rb->alu; b_en=0; op=0; cin=1; d_w=1; fr_w=0
			rb->alu; b_en=1; op=<op>; cin=0; d_w=1; if opcode[3]==1: fr_w=1 else: fr_w=0
		- alu->pc
			alu->ra
		- pc->mar
			rb->mar
		- if cond=flags: mem->pc
			mem->ra
			ra->mem
- level 0: PLA instruction & flags
- level 1: PLA µROM
- level 2: PLA steps
- level 3: AND clock 


r0_r			(~instr[3] & ~instr[2] & ra_r) | (~instr[1] & ~instr[0] & rb_r)
r0_w			(~instr[3] & ~instr[2] & ra_w)
r1_r			(~instr[3] &  instr[2] & ra_r) | (~instr[1] &  instr[0] & rb_r)
r1_w			(~instr[3] &  instr[2] & ra_w)
r2_r			( instr[3] & ~instr[2] & ra_r) | ( instr[1] & ~instr[0] & rb_r)
r2_w			( instr[3] & ~instr[2] & ra_w)
r3_r			( instr[3] &  instr[2] & ra_r) | ( instr[1] &  instr[0] & rb_r)
r3_w			( instr[3] &  instr[2] & ra_w)
br				br_en & ((~instr[1] & ~instr[0]) | (~instr[1] & instr[0] & fr[z]) | (instr[1] & ~instr[0] & ~fr[z]))

ra_r			(S3 & insr[7]) | (S3 & opcode==0000)
ra_w			S5 | S7
rb_r
rb_w
br_en

a_w
b_en
op
cin
d_r
d_w
fr_w

pc_r
pc_w
ir_w
mar_w
mem_en
mem_rw

internal:
halt
return
