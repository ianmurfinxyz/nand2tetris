# file: sum.asm
# Test program used as input to tokenizer unit tests.
# Computes sum of 1 to N=10 (hardcoded input)

# R0=10
@R0
M=10

# i=1
@i
M=  1

# sum=0
@s u  m
M=0

(LOOP)
# if(i>R0) goto STOP
@i
   D =M
@  R  0
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


