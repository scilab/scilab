function [stk,txt,top]=sci_str2num()
txt=[]

if lst(ilst+1)(1)=='1' then
  out=lhsvarsnames()
  ierr=gettempvar()
  txt='['+out+','+ierr+']=evstr('+stk(top)(1)+')'
  stk=list(out,'-1','?','?','1')
else
  out=gettempvar(1)
  ierr=gettempvar()
  txt='['+out+','+ierr+']=evstr('+stk(top)(1)+')'
  stk=list(out,'0','?','?','1')
end

