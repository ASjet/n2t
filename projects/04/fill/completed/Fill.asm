// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Kerboard at RAM[24576] KBD
// Screen start at RAM[16384] SCREEN with 256 rows which per row with 32 16bits-words
@256
D=A
@height
M=D
@32
D=A
@width
M=D
@8192
D=A
@SCREEN
D=D+A
@max
M=D
@SCREEN
D=A
@pointer
M=D

(MAIN_LOOP)
// if KBD = 0 then GOTO CLEAR else GOTO FILL
@KBD
D=M
@CLEAR
D;JEQ
// set write = 0xFFFF
D=0
D=D-1
@write
M=D
@WRITE
0;JMP
(CLEAR)
// set write = 0x0000
@write
M=D

(WRITE)
// if preKBD = curKBD then continue write else reset pointer
@written
D=M
@write
D=D-M
@CONTINUE
D;JEQ
(RESET)
@SCREEN
D=A
@pointer
M=D

(CONTINUE)
@write
D=M
@pointer
A=M
M=D
@pointer
M=M+1
@written
M=D
@MAIN_LOOP
0;JMP