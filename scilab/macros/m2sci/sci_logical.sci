function [stk,txt,top]=sci_logical()
// Copyright INRIA
txt=[]
set_infos(['logical('+stk(top)(1)+') replaced by '+stk(top)(1);
           'computation may be wrong if '+stk(top)(1)+' is used as an index';
	   'in that case replace '+stk(top)(1)+' by '+stk(top)(1)+'==1'],2)
stk=list('stk(top)(1)','0',stk(top)(3),stk(top)(4),stk(top)(5))
//ceci pourrait etre ameliore avec une propriete supplementaire qui serait le 
//type Matlab, type que l'on interrogerait dans l'extration et insertion..
