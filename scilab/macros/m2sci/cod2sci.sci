function [txt,ilst,vnms,vtps,lcount]=cod2sci(lst,ilst,vnms,vtps)
//
//!
// Copyright INRIA
nlst=size(lst)
txt=[]
ilst=ilst-1
while ilst<nlst then
  ilst=ilst+1
  op=lst(ilst)
  if type(op)==15 then return,end
  select op(1)
  case '1' then //stackp
//    write(6,'stackp '+op(2));

    prev=lst(ilst-1)
    if size(prev,'*')==1 then prev=[prev ' '],end
    if prev(1:2)==['5','25']|prev(1)=='20' then
      lhs=evstr(prev(4))
    else
      lhs=1
    end
    if lhs==1 then
      expk=stk(1);
      opk=lst(ilst);ilst=ilst+1
      opk2=opk(2)
      if expk(2)<>'-1'& expk(2)<>'-2' then
        if opk(2)=='ans' then
	  expk(1)($)= expk(1)($)+';'
          txt=[txt;expk(1)(:)]
	else
//	  if funptr(opk2)<>0&exists('sci_'+opk(2))==0 then opk2='%'+opk(2),end
          if funptr(opk2)<>0 then opk2='%'+opk(2),end
	  w=expk(1)(:)
	  w(1)=opk2+' = '+w(1)
	  w($)= w($)+';'
	  w(2:$)=indentsci(w(2:$))
          txt=[txt;w]
	end
	nv=find(opk(2)==vnms(:,2))
        if nv==[] then 
          nv=size(vnms,1)+1,
        end
        nv=nv($)
        vnms(nv,:)=[opk2,opk(2)]
        vtps(nv)=list(expk(5),expk(3),expk(4),0)
      else
	nv=find(opk(2)==vnms(:,2))
	if nv==[] then 
	  nv=size(vnms,1)+1,
	  vnms(nv,:)=[opk2,opk(2)]
	end
	vtps(nv)=list(expk(5),expk(3),expk(4),0)
      end
    else //if size(stk)==1 then
      LHS=[]
      for k=1:lhs
	if size(stk)<lhs then
	  msg=['Unexpected problem: incompatible number of lhs arguments'
	        'between function definition ('+string(size(stk))+') '
		'and function call           ('+string(lhs)+')']
	  disp(msg)
	  pause
	end
        expk=stk(k);
        opk=lst(ilst);ilst=ilst+1
        opk2=opk(2)
//        if funptr(opk2)<>0&exists('sci_'+opk(2))==0 then opk2='%'+opk(2),end
        if funptr(opk2)<>0 then opk2='%'+opk(2),end
        LHS=[opk2,LHS]
        nv=find(opk(2)==vnms(:,2))
        if nv==[] then 
          nv=size(vnms,1)+1,
        end
        nv=nv($)
        vnms(nv,:)=[opk2,opk(2)]
	if size(expk)<5 then 
	  msg=['Unexpected problem, may be due to an incorrect '
	      'translation function']
	  if type(expk(1))==10 then msg=[msg;'context is :'+expk(1)],end
	  disp(msg)
	  pause,
	end
        vtps(nv)=list(expk(5),expk(3),expk(4),0)
      end
      if stk(1)(2)=='-1' then // variable has  not been previously stored
        txt=[txt;'['+makeargs(LHS)+'] = '+stk(1)(1)+';']
      end
    end
    if ilst<=nlst then
      if lst(ilst)(1)<>'15' then 
        ilst=ilst-1,
      else
        lcount=lcount+1
      end
    end
  case '12' then //pause
    txt=[txt;'pause']
  case '13' then //break
    txt=[txt;'break']
  case '14' then //abort
    txt=[txt;'abort']
  case '15' then ,//eol
    txt=[txt;' ']
    lcount=lcount+1
  case '18' then   
  case '99' then //return
    txt=[txt;'return']
  else
    [stk,t1,ilst,vnms,vtps]=exp2sci(lst,ilst,vnms,vtps);
    txt=[txt;t1];t1=[]
    ilst=ilst-1
  end
end
ilst=ilst+1
endfunction
