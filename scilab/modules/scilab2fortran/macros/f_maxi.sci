function [stk,nwrk,txt,top]=f_maxi(nwrk)
//!purpose
//  Scilab maxi function translation
//!
txt=[]
select rhs
case 1 then
  s2=stk(top)
  if lhs==1 then
    if s2(3)=='0' then fun='ivmax',else fun='dvmax',end
    nwrk=dclfun(nwrk,fun,s2(3))
    out=callfun([fun,mulf(s2(4),s2(5)),s2(1),'1'],s2(3))
    stk=list(out,'0',s2(3),'1','1')
  else
    [out,nwrk,t1]=outname(nwrk,[s2(3);'0'],['1';'1'],['1';'1'])
    w=getentry1(s2(1),'loop')
    txt=[t1;
	' '+out(2)+'='+getentry1(s2(1),'1')
	' do loop=2,'+mulf(s2(4),s2(5))
	indentfor(genif(w+'.gt.'+out(2),..
	    [' '+out(2)+'='+w;
	     ' '+out(1)+'=loop']))
       ' enddo']
    stk=list(list(out(1),'-1','1','1','1'),list(out(2),'-1','0','1','1'))
  end
case 2 then
  s2=stk(top);s1=stk(top-1);top=top-1
  if lhs==1 then
    if s2(4)=='1'&s2(5)=='1'&s1(4)=='1'&s1(5)=='1' then
      stk=list('max('+s1(1)+','+s2(1)+')','0',s2(3),'1','1')
    else
      warning('max with 2 matrix args not translated')
    end
  else
    warning('max with 2 lhs arguments not translated')
  end
else
  warning('max with more than 2 args not translated')
end

endfunction
function var=getentry1(var,i)
if part(var,1:5)=='work(' then
  pti=part(var,6:length(var)-1)
  
  var='work('+addf(pti,addf(i,'-1'))+')'
elseif  part(var,1:6)=='iwork(' then
  pti=part(var,7:length(var)-1)
  var='iwork('+addf(pti,addf(i,'-1'))+')'
else
  var=var+'('+i+')'
end
endfunction
