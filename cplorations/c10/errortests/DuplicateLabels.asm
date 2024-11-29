// DuplicateLabels.asm
// This file tests duplicate label detection.

(LOOP)  // First definition of LOOP.
@R0
D=M
(LOOP)  // Duplicate label definition.
@END
0;JMP
(END)

