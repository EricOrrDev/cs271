//the test file loads the operands into R0 an R1
//R0 is first operand
//R1 is second operand
//R2 is the product
//R3 is i

//initalize r2 and i
@0
D=A
@2
M=D //sets r2 = 0 (product)
@3
M=D //sets r3 = 0 (i)

//check if either operand == zero
@0
D=M
@INFINITE_LOOP
D;JEQ
@1
D=M
@INFINITE_LOOP
D;JEQ

(Add_r0)
// Load the first operand (R0) into D
@0
D=M
// Load the current sum (R2) and add D to it
@2
D=D+M
// Store the result back in R2
M=D

(Increment_I)
//Load i into D
@3
D=M //D = i
//increment D
@1
D=D+A //D = i + 1
@3
M=D
//Now that i is incremented, we check if i >= second operand

(Check_I)
//Load i into D
@3
D=M //D = i
//load R1 into A
@1
D=D-M   //D = i - R!
//check if i is less than the second operand
@Add_r0
D;JLT

(INFINITE_LOOP)
@INFINITE_LOOP
0;JMP
