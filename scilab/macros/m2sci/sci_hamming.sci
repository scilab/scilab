function [stk,txt,top]=sci_hamming()
// Copyright INRIA
txt=[]
if rhs==1 then
 stk=list('window(''hm'','+stk(top)(1)+')''','0',stk(top)(1),'1','1')
else
  sflag=stk(top)(1)
  top=top-1
  if sflag=='symmetric' then
    stk=list('window(''hm'','+stk(top)(1)+')''','0',stk(top)(1),'1','1')
  elseif sflag=='periodic' then
    if lst(ilst+1)(1)=='1' then  //affected result
      w=lhsvarsname()
    else
      w=gettempvar()
    end
    txt=[w+' = window(''hm'','+stk(top)(1)+'+1)''';
         w+' = '+w+'(1:$-1)']

    stk=list(' ','-2',stk(top)(1),'1','1')
  else
    if lst(ilst+1)(1)=='1' then  //affected result
      w=lhsvarsname()
    else
      w=gettempvar()
    end 
    txt=['if '+sflag+'==''periodic'' then'
         '  '+w+' = window(''hm'','+stk(top)(1)+'+1)'''
         '  '+w+' = '+w+'(1:$-1)'
         'else'
         '  '+w+' = window(''hm'','+stk(top)(1)+')'''
         'end']
    stk=list(' ','-2',stk(top)(1),'1','1')
  end
end
endfunction
