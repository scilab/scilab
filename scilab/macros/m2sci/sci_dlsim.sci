function [stk,txt,top]=sci_dlsim()
// Copyright INRIA
txt=[]
if rhs>3 then // DLSIM(A,B,C,D,U [,X0])
  A=stk(top-rhs+1)(1);na=stk(top-rhs+1)(4)
  B=stk(top-rhs+2)(1)
  C=stk(top-rhs+3)(1)
  D=stk(top-rhs+4)(1)
  nin=stk(top-rhs+4)(4)
  nt=stk(top-rhs+4)(3)
  U=stk(top-rhs+5)(1);U='('+U+').'''
  nu=stk(top-rhs+5)(3)
  if lhs==1 then
    if rhs==5 then
      Y='('+C+'* ltitr'+rhsargs([A,B,U])+' + '+D+' * '+U+').'''
    else
      X0=stk(top)(1)
      Y='('+C+'* ltitr'+rhsargs([A,B,U,X0])+' + '+D+' * '+U+').'''
    end
    stk=list(Y,'0',nt,nu,'1')
  else
    [Y,X]=lhsvarsnames()
    if rhs==5 then
      txt=X+' = ltitr'+rhsargs([A,B,U])
    else
      X0=stk(top)(1)
      txt=X+' = ltitr'+rhsargs([A,B,U,X0])
    end
    txt=[txt;Y+' = ('+C+' * '+X+' + '+D+' * '+U+').'';'
	X+' = '+X+'.'';']
    stk=list(list('?','-2',nt,nu,'?'),list('?','-2',nt,na,'?'))
  end
else //DLSIM(NUM,DEN,U) 
  NUM=stk(top-rhs+1)(1)
  DEN=stk(top-rhs+2)(1)
  U=stk(top-rhs+3)(1);nt=stk(top-rhs+2)(3)
  if lhs>1 then 
    set_infos(['Dlsim with polynomial input and 2 output not translated';
	'matlab syntax used'] ,2)
    [Y,X]=lhsvarsnames()
    txt=[txt;'['+Y+','+X+'] = dlsim'+rhsargs([NUM,DEN,U])]
    out=list('?','-2','1',nt,'1')
    stk=list(out,out)
  else 
    U='('+U+').'''
    stk=list('rtitr'+rhsargs([NUM,DEN,U])+'.''','0',nt,'1','1')
  end
end
endfunction
