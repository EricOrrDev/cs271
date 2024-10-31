//The keyboard outputs numbers greater than 0 when a key is pressed
//SCREEN starts at 0x4000 and ends at 0x5FFF or 24575
(INITALIZE)
@24575
D=A         // D now holds 24575 (the end screen address)
            // Load the screen start address and subtract
@SCREEN
D=D-A       // D = 24575 - SCREEN
// Store the result in @range
@range
M=D         // Memory[@range] now holds the value of 24575 - SCREEN

@range
D=M
@i
M=D         //initalize i to range

(LOOK_FOR_KEYPRESS)
@KBD
D=M
@BLACK_SCREEN
D;JGT           //escape infinite loop if key is pressed
@LOOK_FOR_KEYPRESS
0;JMP           //if no keys pressed then loop

(LOOK_FOR_KEY_RELEASE)
@KBD
D=M
@WHITE_SCREEN
D;JEQ           //escape infinite loop if key is released
@LOOK_FOR_KEY_RELEASE
0;JMP           //if key is still held then repeat loop

(BLACK_SCREEN)  //Value of -1 for black in all 16 bits
@range
D=M             //load range value into D           
@i
M=D             //set i to range value

(BLACK_LOOP)    
@i
D=M             //D=i
@SCREEN         //A=SCREEN address
A=D+A           //SCREEN[i]
M=-1            //setting SCREEN[i] to -1
@i
M=M-1           //decrement i
@i
D=M
//Falling out of black loop if statement
@BLACK_LOOP
D;JGE           //Repeat loop if i less than screen range. Might be off by one error here 
@LOOK_FOR_KEY_RELEASE
0;JMP           //Once we've blackened all the screen memory, we go to looking for key release


(WHITE_SCREEN)  //Value of -1 for black in all 16 bits
@range
D=M             //load range value into D           
@i
M=D             //set i to range value

(WHITE_LOOP)    
@i
D=M             //D=i
@SCREEN         //A=SCREEN address
A=D+A           //SCREEN[i]
M=0            //setting SCREEN[i] to -1
@i
M=M-1           //decrement i
@i
D=M
//Falling out of black loop if statement
@WHITE_LOOP
D;JGE           //Repeat loop if i less than screen range. Might be off by one error here 
@LOOK_FOR_KEYPRESS

0;JMP           //Once we've blackened all the screen memory, we go to looking for key release
@LOOK_FOR_KEYPRESS  //Fall through prevention
0;JMP               //go back to looking for keyboard input
