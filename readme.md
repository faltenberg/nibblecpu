Nibble CPU
==========

The Nibble CPU is a simple 4bit processor for teaching purposes. Version 0 is based on a shared-bus
architecture and uses latches. Version 1 has a datapath and uses flip-flops, thus being faster.
Both CPUs have the following characteristics:

- 4bit ALU for addition and subtraction
- 4bit instruction code + 4bit of immediate value
- 2 addressing modes (immediate, absolute)
- 16 half-bytes of RAM
- 16 bytes of ROM for code
- 2 clock ticks per cycle
- v0: 7 cycles per instruction, v1: 2 cycles per instruction


Usage
-----

The CPU is implemented with the [Logisim](http://www.cburch.com/logisim) software. Open the *.circ*
files with that tool and read the documentation on the website or watch videos on Youtube for more
information regarding the usage of Logisim.

The simulator can be operated with a couple of buttons. The processor has a manual mode, where the
clock will be ignored, and the regular automatic mode. The manual mode can be used to store code to
the program ROM with the provided interface. The manual mode should only be switched on once the
processor has stopped by the `halt` instruction. Otherwise the behavior is undefined and should be
restored by resetting the processor. Resuming the processor after a halt doesn't produce undefined
behavior and is the recommended way to go.

| Button         | Action                                                                           |
| -------------- | -------------------------------------------------------------------------------- |
| **ON**         | powers on and off the processor                                                  |
| **CLOCK**      | the clock signal that drives the processor                                       |
| **RESUME**     | resumes the processor after it was halted                                        |
| **RESET**      | resets the processor and the RAM                                                 |
| **MANUAL**     | switches the processor to manual mode (after it was halted)                      |
| **WRITE ROM**  | stores the inserted instruction to the selected address in ROM if in manual mode |
| **RESET ROM**  | resets the program ROM                                                           |
| **ROM ADDR**   | selects the ROM address to write a new instruction to                            |
| **ROM OPCODE** | the new instruction's opcode                                                     |
| **ROM IMM**    | the new instruction's immediate value                                            |

For demonstration purposes there is a multiplication program inside the *mult.c* file that can be
inserted into the ROM. Since Nibble is a very limited processor, don't expect it to run Doom.


Registers
---------

The processor has two accessible registers, namely the accumulator `ACC` which is used to store
temporary data, and the program counter `PC`. Both registers are 4bit wide. Additional hidden
registers do exist as part of the microarchitecture.

```
ACC |....|
PC  |....|
```


Instructions
------------

Since the Nibble CPU is a simple processor for teaching, it has only one instruction format and
due to the 3bit opcode only 8 instructions. The `m` bit of the instruction specifies whether the
immediate value is used as such or used as absolute address to grab the operand from the RAM.
This drastically improves the processor's capability and flexibility. Without absolute addressing
it would be hard (if not even impossible) to write even a simple multiplication program for
arbitrary numbers.

```
        |7 6 5 4 3 2 1 0|
Format: | op  |m|  imm  |
```

| Instruction | Assembly    | Semantics                    |
| ----------- | ----------- | ---------------------------- |
| `0000 imm`  | `add  imm`  | `ACC = ACC + imm`            |
| `0001 imm`  | `add [imm]` | `ACC = ACC + RAM[imm]`       |
| `0010 imm`  | `sub  imm`  | `ACC = ACC - imm`            |
| `0011 imm`  | `sub [imm]` | `ACC = ACC - RAM[imm]`       |
| `0100 imm`  | `mov  imm`  | `ACC = imm`                  |
| `0101 imm`  | `ld  [imm]` | `ACC = RAM[imm]`             |
| `011x imm`  | `str [imm]` | `RAM[imm] = ACC`             |
| `1000 imm`  | `bz   imm`  | `if ACC == 0: PC = imm`      |
| `1001 imm`  | `bz  [imm]` | `if ACC == 0: PC = RAM[imm]` |
| `1010 imm`  | `bnz  imm`  | `if ACC != 0: PC = imm`      |
| `1011 imm`  | `bnz [imm]` | `if ACC != 0: PC = RAM[imm]` |
| `1100 imm`  | `jmp  imm`  | `PC = imm`                   |
| `1101 imm`  | `jmp [imm]` | `PC = RAM[imm]`              |
| `111x imm`  | `halt`      | `ignore clock`               |


V0 - Clocking
-------------

Each instruction is broken up into up to seven micro-operations which are executed within one
single read-write cycle. The overlapping read and write signals are generated from the clock
impulse and ensure that data can flow properly through the shared databus.

```
       1  2  3  4
       +--+  +--+  +-
       |  |  |  |  |
clk   -+  +--+  +--+

       +--+--+--+  +-
       |        |  |
read  -+        +--+

          +--+
          |  |
write -+--+  +--+--+
```

V0 - Microinstructions
----------------------

| Signal     | Action                                                                             |
| ---------- | ---------------------------------------------------------------------------------- |
| **rmar_w** | writes new address to the ROM memory address register `ROM MAR`                    |
| **irom_r** | reads instruction opcode from ROM onto the bus                                     |
| **drom_r** | reads immediate value from ROM onto the bus                                        |
| **pc_r**   | reads program counter `PC` onto the bus                                            |
| **pc_w**   | writes new address to the program counter `PC`                                     |
| **ir_w**   | writes new instruction to the instruction register `IR`                            |
| **mar_w**  | writes new address to the RAM memory address register `RAM MAR`                    |
| **ram_r**  | reads RAM onto the bus                                                             |
| **ram_w**  | writes new value to selected RAM address                                           |
| **acc_r**  | reads accumulator `ACC` onto the bus                                               |
| **acc_w**  | writes new value to the accumulator `ACC`                                          |
| **tmp_w**  | writes new value to the ALU temporary register `TMP`                               |
| **muxB**   | passes value to the ALU if 1, otherwise the constant zero                          |
| **aluop**  | ALU performs an addition if 0, otherwise a subtraction                             |
| **cin**    | allows to increment a value by one if 1                                            |
| **res_r**  | reads ALU result onto the bus                                                      |
| **res_w**  | writes ALU result into the temporary result register `RES`                         |
| **halt**   | stops processor                                                                    |

Each instruction starts with a 3-step instruction fetch and is executed within four steps. Each
step is dedicated to a specific databus transfer to simplify instruction decoding, although not all
of the instructions perform meaningful tasks in every step.

- **Step 1:**  `PC -> ROM_MAR, PC ->TMP`
- **Step 2:**  `ROM -> IR, TMP+1 -> RES`
- **Step 3:**  `RES -> PC`
- **Step 4:**  `IMM -> RAM_MAR`
- **Step 5a:** `ACC -> RAM`
- **Step 5b:** `ACC -> TMP`
- **Step 6a:** `IMM or RAM -> ACC`
- **Step 6b:** `IMM or RAM -> ALU.B, TMP<op>ALU.B -> RES`
- **Step 6c:** `IMM or RAM -> PC`
- **Step 7a:** `halt`
- **Step 7b:** `RES -> ACC`


### Microprograms

**IFETCH:**
- **Step 1:** `pc_r=1, rmar_w=1, tmp_w=1`
- **Step 2:** `irom_r=1, ir_w=1, muxB=1, aluop=0, cin=1, res_w=1`
- **Step 3:** `res_r=1, pc_w=1`

**ADD:**
- **Step 4:** `drom_r=1, mar_w=1`
- **Step 5:** `acc_r=1, tmp_w=1`
- **Step 6:** `if m==0: irom_r=1 else: ram_r=1, muxB=1, aluop=0, cin=0, res_w=1`
- **Step 7:** `res_r=1, acc_w=1`

**SUB:**
- **Step 4:** `drom_r=1, mar_w=1`
- **Step 5:** `acc_r=1, tmp_w=1`
- **Step 6:** `if m==0: drom_r=1 else: ram_r=1, muxB=1, aluop=1, cin=0, res_w=1`
- **Step 7:** `res_r=1, acc_w=1`

**MOV / LD:**
- **Step 4:** `drom_r=1, mar_w=1`
- **Step 5:** `---`
- **Step 6:** `if m==0: drom_r=1 else: ram_r=1, acc_w=1`
- **Step 7:** `---`

**STR:**
- **Step 4:** `drom_r=1, mar_w=1`
- **Step 5:** `acc_r=1, ram_w=1`
- **Step 6:** `---`
- **Step 7:** `---`

**BZ:**
- **Step 4:** `drom_r=1, mar_w=1`
- **Step 5:** `---`
- **Step 6:** `if m==0: drom_r=1 else: ram_r=1, if zero==0: pc_w=1`
- **Step 7:** `---`

**BNZ:**
- **Step 4:** `drom_r=1, mar_w=1`
- **Step 5:** `---`
- **Step 6:** `if m==0: drom_r=1 else: ram_r=1, if zero==1: pc_w=1`
- **Step 7:** `---`

**JMP:**
- **Step 4:** `drom_r=1, mar_w=1`
- **Step 5:** `---`
- **Step 6:** `if m==0: drom_r=1 else: ram_r=1, pc_w=1`
- **Step 7:** `---`

**HALT:**
- **Step 4:** `---`
- **Step 5:** `---`
- **Step 6:** `---`
- **Step 7:** `halt=1`


V1 - Clocking
-------------

```
 1  2  3  4
 +--+  +--+  +-
 |  |  |  |  |
-+  +--+  +--+
```

Each cycle consists of two clock ticks. The first tick is used for instruction fetching and
instruction decoding. The second tick is used for instruction execution. The cycle starts at `(1)`
with the instruction register `IR` being written to the new instruction from the ROM. Immediately
the decoding is performed and all non-edge-sensitive signals are set, which remain set until the
next cycle when a new instruction was loaded. At `(2)` nothing particular happens. At `(3)` all
write signals are fired, thus updating the program counter `PC` and the accumulator `ACC` on the
rising edge. Once `PC` is updated the new instruction in the ROM is selected. The RAM gets its
write signal at `(3)`, too, but since the RAM is built from latches the final data is fixed at the
falling edge `(4)`. The `halt` signal is fired at `4` so the processor starts with a new cycle
after resume.


V1 - Microinstructions
----------------------

| Signal    | Action                                                                               |
| --------- | ------------------------------------------------------------------------------------ |
| **ir_w**  | writes new instruction from ROM to the instruction register `IR` on rising edge      |
| **muxPC** | passes new address to the program counter `PC` if 1, otherwise the incremented value |
| **pc_w**  | writes new address to the program counter `PC` on rising edge                        |
| **acc_w** | writes new value to the accumulator `ACC` on rising edge                             |
| **ram_w** | writes `ACC` to RAM at the address given in the instruction on the high phase        |
| **op**    | ALU performs an addition if 0, otherwise a subtraction                               |
| **muxA**  | passes accumulator `ACC` to the ALU if 1, otherwise the constant zero                |
| **muxB**  | passes the immediate value to the ALU if 0, otherwise the contents of the RAM        |
| **halt**  | stops processor on the falling edge                                                  |
