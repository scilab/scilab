function [stk,txt,top]=sci_sort()
// Copyright INRIA
txt=[]
temp=gettempvar()
in=stk(top)(1)
if stk(top)(4)=='4' then
  in='bool2s('+stk(top)(1)+')'
elseif stk(top)(2)=='2'|stk(top)(2)=='3' then
  in='('+in+')'
end
if lhs==1 then  
  if stk(top)(3)=='1'|stk(top)(4)=='1' then
    stk=list('-sort(-'+in+')','2',stk(top)(3),stk(top)(4),'1')
  else
    if isname(in) then
      if lst(ilst+1)(1)=='1' then
	out=lhsvarsnames()
      else
	out=temp
      end
      temp=in
    else
      txt=[temp+' = '+in]
      out=temp
    end
    set_infos('next test may  probably be simplified',1)
    txt=[txt;
	'if min(size('+temp+'))==1 then '
	'  '+out+'=-sort(-'+temp+')'
	'else'
	'  '+out+'=-sort(-'+temp+',''r'')'
        'end']
    stk=list(out,'0',stk(top)(3),stk(top)(4),'1')
  end
else
  [y,i]=lhsvarsnames()
  if stk(top)(3)=='1'|stk(top)(4)=='1' then
    txt='['+y+','+i+'] = sort(-'+in+');'+y+' = -'+y
  else
    if isname(in) then
      temp=in
    else
      txt=[temp+' = '+in]
    end
    set_infos('next test may  probably be simplified',1)
    txt=[txt;
	  'if min(size('+temp+'))==1 then '
	  '  ['+y+','+i+']=sort(-'+temp+');'+y+' = -'+y
	  'else '
	  '  ['+y+','+i+']=sort(-'+temp+',''r'');'+y+' = -'+y
	  'end']
  end
  stk=list(list('?','-2',stk(top)(3),stk(top)(4),'1'),..
      list('?','-2',stk(top)(3),stk(top)(4),'1'))
end
