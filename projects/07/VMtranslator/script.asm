// push local index
@LCL
D=M
@index
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1


@SP
M=M-1
A=M
D=M
@LCL
A=M

A=A+1

M=D