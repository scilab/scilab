function [stk,txt,top]=sci_ftell()
// Copyright INRIA
txt=[]
fid=stk(top)
stk=list('mtell('+fid(1)+')','0','1','1','1')

