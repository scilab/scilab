function [stk,txt,top]=sci_filter()
// Copyright INRIA
txt=[]
stk=list('mtlb_filter'+rhsargs([stk(top-2)(1),stk(top-1)(1),stk(top)(1)]),'0',stk(top)(3),stk(top)(4),'1')
