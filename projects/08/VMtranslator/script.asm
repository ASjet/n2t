@10
D=A
@R13
M=D

@R13
A=M-1
D=M

@R13
D=M
D=D-1
A=D
D=M


// Stack
// return_address
(PUSH_TRUE)
@SP
A=M
M=-1
@SP
M=M+1
@R15
A=M
0;JMP

@Sys.init
0;JMP