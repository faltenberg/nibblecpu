Instruction Set Architecture
============================


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

0000 ra rb		mov  ra, rb
0001 ra xx		data ra, imm8
0010 ra rb		ldr  ra, [rb]
0011 ra rb		str  ra, [rb]
0101 00 00		br   imm8
0101 00 01		brz  imm8
0101 00 10		brnz imm8
0110 xx xx		halt

1000 ra rb		add  ra, rb
1001 ra rb		sub  ra, rb
1111 ra rb		cmp  ra, rb
