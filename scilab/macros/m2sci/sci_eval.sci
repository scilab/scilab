function [stk,txt,top]=sci_eval()
// Copyright INRIA
ARGS=[]
for k=1:rhs
  ARGS=[stk(top)(1),ARGS]
  top=top-1
end
if lhs==1 then
  LHS=lhsargs(lhsvarsnames())
  if rhs==1 then
    if LHS<>[] then
      if LHS<>'ans' then
	set_infos(['mtlb_eval can be replaced by evstr if '+ARGS
	    'is a valid scilab instruction'],1)
      else
	set_infos(['mtlb_eval can be replaced by '
	    'evstr if '+ARGS+' is a valid scilab expression'
	    'execstr if '+ARGS+' is a valid scilab instruction'  ],1)
      end
    else
      set_infos(['mtlb_eval can be replaced by '
	  '  evstr if '+ARGS+ 'is a valid scilab expression'
	  '  execstr if '+ARGS+ 'is a valid scilab instruction'],1)
    end
    stk=list('mtlb_eval'+rhsargs(ARGS),'0','?','?','?')
  else
    if LHS<>[] then
      ARGS=sci2exp(LHS+'=')+'+'+ARGS
      opt='-2'
      w=' '
    else
      w=gettempvar()
      ARGS=sci2exp(w+'=')+'+'+ARGS
      opt='0'
    end
    set_infos(['mtlb_eval'+rhsargs(ARGS)+' can be replaced by'
	'if '+'execstr('+ARGS(1)+',''errcatch'')'+'<>0 then'
	'  execstr('+ARGS(2)+')'
	'end'
	'If '+ARGS(1)+' and '+ARGS(2)+' are valid scilab instructions'],1)
    txt=[txt;'mtlb_eval'+rhsargs(ARGS)+';']
    stk=list(w,opt,'?','?','?')
  end
else
  LHS=lhsargs(lhsvarsnames())
  ARGS=sci2exp(LHS+'=')+'+'+ARGS
  if rhs==1 then
    set_infos(['mtlb_eval can be replaced by execstr if '+ARGS
	'is a valid scilab instruction'],1)
  else
    set_infos(['mtlb_eval'+rhsargs(ARGS)+' can be replaced by'
	'if '+'execstr('+ARGS(1)+',''errcatch'')'+'<>0 then'
        '  execstr('+ARGS(2)+')'
      'end'
      'If '+ARGS(1)+' and '+ARGS(2)+' are valid scilab instructions'],1)
  end
  txt=[txt;'mtlb_eval'+rhsargs(ARGS)+';']
  s=list(' ','-2','?','?','?')
  stk=list()
  for k=1:lhs,stk(k)=s,end
end

