function [stk,txt,ilst,vnms,vtps,lcount]=exp2sci(lst,ilst,vnms,vtps)
//
//!
// Copyright INRIA
nlst=size(lst)
top=0
stk=list()
txt=[]
ilst=ilst-1
cmod=0;
//
ok=%t
while ilst<nlst&ok then
  lhs=1
  ilst=ilst+1
  op=lst(ilst)
  if type(op)==10 then
    if prod(size(op))==1 then op=[op ' '],end
    //disp('exp2sci '+op(1)+'   '+op(2)+'  '+string(top))
    select op(1)
    case '0' then
 
    case '2' then //stackg
      if or(op(2)==vnms(:,2))|(op(3)=='-3'&op(4)<>'0') then
        [stk,top,vnms,vtps,t1]=get2sci(op(2),stk,top,vnms,vtps)
	txt=[txt;t1]
      elseif op(4)=='0' then
	if funptr(op(2))<>0&exists('sci_'+op(2))==0 then op(2)='%'+op(2),end
        [stk,top,vnms,vtps,t1]=get2sci(op(2),stk,top,vnms,vtps)
	//stsz=stacksize();if stsz(2)>500000 then pause,end
	txt=[txt;t1]
        if stk(top)==list() then
	  // m-file without input args
	  // compute lhs
	  lhs=0
	  il=ilst+1
	  nlst=size(lst)
	  while il<=nlst
	    if lst(il)(1)<>'1' then break,end
	    lhs=lhs+1
	    il=il+1
	  end
	  top=top-1
	  op(1)='20'
          op(3)=op(4)
          op(4)=string(lhs)
	  lst;lst(ilst)=op;
          [stk,t1,top]=func2sci(op,stk)
          txt=[txt;t1]
          if ilst<nlst then
            if lst(ilst+1)(1)=='1'& lst(ilst+1)(2)==op(2) then
              ilst=ilst+1
	      t1=stk(top)(1);t1($)=t1($)+';'
              txt=[txt;t1]
              top=top-1
            end
	  end
        end
      else
        //appel des macros
        op(3)=op(4)
        op1=lst(ilst+1)
        if op1(1)=='5'&op1(2)=='25' then
          ilst=ilst+1
          op(4)=op1(4)
        else
          op(4)='1'
        end
        [stk,t1,top]=func2sci(op,stk)
        txt=[txt;t1]
      end
      t1=[]
    case '3' then //string
      quote=''''
      dqote='""'
      top=top+1
      st=strsubst(strsubst(op(2),quote,quote+quote),dquote,dquote+dquote)
      stk(top)=list(quote+st+quote,'0','1','1','10')
    case '4' then //matrice vide
      top=top+1
      stk(top)=list('[]','0','0','0','1')
    case '5' then //allops
      t1=[]
      iop=evstr(op(2))
      top1=top
      execstr('[stkr,t1,top]=%'+ops(iop,2)+'2sci()')
      stk(top)=stkr
      txt=[txt;t1]
      t1=[]
    case '6' then //num
      [stk,top]=num2sci(op(2),stk)
    case '20' then //functions
      [stk,t1,top]=func2sci(op,stk)
      txt=[txt;t1]
      t1=[]
    case '15' then 
      lcount=lcount+1
    case '19' then  // mkindx
      // replace all variables describing path by a single list
      n=evstr(op(2))
      m=evstr(op(3))
      l=list([n,m])
      if m>0 then n=n-1,end
      nv=(n+m)
      pos=top-nv
      for k=1:nv
	l($+1)=stk(pos+k)
      end
      top=pos+1
      stk(top)=l
    case '23' then   
      top=top+1
      stk(top)=list(quote+op(2)+quote,'0','1','1','10')
    else
      ok=%f
    end
  else
    ok=%f
  end

end
lst=resume(lst)



