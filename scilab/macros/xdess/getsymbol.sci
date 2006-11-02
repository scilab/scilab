function rep=getsymbol(Title)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<1 then Title="",end
marksIds=['.','+','x','*','diamond fill.','diamond',..
	'triangle up','triangle down','trefle','circle'];
fontsSiz=['08' ,'10','12','14','18','24'];
mm=xget('mark');
lmid=list('markId',mm(1)+1,marksIds);
lmsiz=list('marksize',mm(2)+1,fontsSiz);
xch_l = list(lmid,lmsiz);
entval=[lmid(2),lmsiz(2)];
rep=x_choices(Title,xch_l)-[1 1];
endfunction
