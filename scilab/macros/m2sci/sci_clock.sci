function [stk,txt,top]=sci_clock()
// Copyright INRIA
txt=[]
w=gettempvar()
txt=w+'=getdate()'
stk=list(w+'([1:2 6:9])','0','1','6','1')

