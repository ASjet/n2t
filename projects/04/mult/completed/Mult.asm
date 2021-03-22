// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

@R2
M=0
// if R0 < R1 then GOTO R0LR1 else GOTO R0GR1
@R0
D=M
@R1
D=D-M
@R0LR1
D;JLT
@R0GR1
0;JMP

(R0LR1)
@R0
D=M
@times
M=D
@R1
D=M
@adder
M=D
@COMP
0;JMP

(R0GR1)
@R1
D=M
@times
M=D
@R0
D=M
@adder
M=D
@COMP
0;JMP

(COMP)
@i
M=0
@R2
M=0

(LOOP)
@i
D=M
@times
D=D-M
@END
D;JEQ

@adder
D=M
@R2
M=M+D
@i
M=M+1
@LOOP
0;JMP
(END)