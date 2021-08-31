# file: sum.asm
# Test program used as input to parser unit tests.
# Computes sum of 1 to N=10 (hardcoded input)

# R0=10
@10
D=A
@R0
M=D

# i=1
@i
M=  1

# sum=0
@sum
M=0

(LOOP)
# if(i>R0) goto STOP
@i
   D =M
@  R0
D=D-M
@STOP
D;JGT

# sum=sum+i
@sum
D=M
@i
D=D+M
@sum
M=D

# i=i+1
@i
M=M+1
# goto LOOP
@LOOP
0;JMP

(STOP)
# R1=sum
@sum
D=M
@R1
M=D

# inifinite loop to end
(END)
@END
0;JMP

# erronoeous instruction that should throw
x+=2


