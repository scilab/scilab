function [ok,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18]=setvalue(desc,labels,typ,ini)
//  setvalues -  data acquisition, getvalue equivalent without dialog
//%Syntax
//  [ok,x1,..,x18]=setvalue(desc,labels,typ,ini)
//%Parameters
//  desc    : column vector of strings, dialog general comment 
//  labels  : n column vector of strings, labels(i) is the label of 
//            the ith required value
//  typ     : list(typ1,dim1,..,typn,dimn)
//            typi : defines the type of the ith required value
//                   if may have the following values:
//                   'mat' : stands for matrix of scalars 
//                   'col' : stands for column vector of scalars
//                   'row' : stands for row vector of scalars
//                   'vec' : stands for  vector of scalars
//                   'str' : stands for string
//                   'lis' : stands for list
//                   'pol' : stands for polynomials
//                   'r'   : stands for rational
//            dimi : defines the size of the ith required value
//                   it must be 
//                    - an integer or a 2-vector of integers (-1 stands for 
//                      arbitrary dimension)
//                    - an evaluatable character string 
//  ini     : n column vector of strings, ini(i) gives the suggested
//            response for the ith required value
//  ok      : boolean ,%t if ok button pressed, %f if cancel button pressed
//  xi      : contains the ith required value if ok==%t
//%Description
// getvalues function uses ini strings to evaluate required args 
// with error checking,
//%Remarks
// All correct scilab syntax may be used as responses, for matrices 
// and vectors getvalues automatically adds [ ] around the given response
// before numerical evaluation
//%Example
// labels=['magnitude';'frequency';'phase    '];
// [ampl,freq,ph]=setvalue('define sine signal',labels,..
//            list('vec',1,'vec',1,'vec',1),['0.85';'10^2';'%pi/3'])
// 
//%See also
// x_mdialog, dialog
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
n=prod(size(labels))
if lhs<>n+2&lhs<>n+1 then error(41),end
if size(typ)<>2*n then
  error('typ : list(''type'',[sizes],...)')
end
x1=[];x2=[];x3=[];x4=[];x5=[];x6=[];x7=[];x8=[];x9=[];x10=[];x11=[];x12=[];x13=[];x14=[]
if rhs==3 then  ini=emptystr(n,1),end
ok=%t
while %t do
  str=ini;
  if str==[] then ok=%f,break,end
  for k=1:n
    cod=str2code(str(k))
    spe=find(cod==99)
    if spe<>[] then
      semi=str2code(';')
      cod(spe)=semi*ones(spe')
      str(k)=code2str(cod)
    end
  end
  nok=0
  for k=1:n
    select part(typ(2*k-1),1:3)
    case 'mat'
      errcatch(-1,'continue','nomessage')
      v=evstr('['+str(k)+']')
      errcatch(-1)
      if iserror(-1) then nok=-k,errclear(-1);break,end
      if type(v)<>1 then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      [mv,nv]=size(v)
      ssz=string(sz(1))+' x '+string(sz(2))
      if mv*nv==0 then
	if  sz(1)>=0&sz(2)>=0&sz(1)*sz(2)<>0 then nok=k,break,end
      else
	if sz(1)>=0 then if mv<>sz(1) then nok=k,break,end,end
	if sz(2)>=0 then if nv<>sz(2) then nok=k,break,end,end
      end
    case 'vec'
      errcatch(-1,'continue','nomessage')
      v=evstr('['+str(k)+']')
      errcatch(-1)
      if iserror(-1) then nok=-k,errclear(-1);break,end
      if type(v)<>1 then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      ssz=string(sz(1))
      nv=prod(size(v))
      if sz(1)>=0 then if nv<>sz(1) then nok=k,break,end,end
    case 'pol'
      errcatch(-1,'continue','nomessage')
      v=evstr('['+str(k)+']')
      errcatch(-1)
      if iserror(-1) then nok=-k,errclear(-1);break,end
      if type(v)>2 then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      ssz=string(sz(1))
      nv=prod(size(v))
      if sz(1)>=0 then if nv<>sz(1) then nok=k,break,end,end
    case 'row'
      errcatch(-1,'continue','nomessage')
      v=evstr('['+str(k)+']')
      errcatch(-1)
      if iserror(-1) then nok=-k,errclear(-1);break,end
      if type(v)<>1 then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      if sz(1)<0 then
	ssz='1 x *'
      else
	ssz='1 x '+string(sz(1))
      end
      [mv,nv]=size(v)
      if mv<>1 then nok=k,break,end,
      if sz(1)>=0 then if nv<>sz(1) then nok=k,break,end,end
    case 'col'
      errcatch(-1,'continue','nomessage')
      v=evstr('['+str(k)+']')
      errcatch(-1)
      if iserror(-1) then nok=-k,errclear(-1);break,end
      if type(v)<>1 then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      if sz(1)<0 then
	ssz='* x 1'
      else
	ssz=string(sz(1))+' x 1'
      end
      [mv,nv]=size(v)
      if nv<>1 then nok=k,break,end,
      if sz(1)>=0 then if nv<>sz(1) then nok=k,break,end,end
    case 'str'
      v=str(k)
      if type(v)<>10 then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      ssz=string(sz(1))
      nv=prod(size(v))
      if sz(1)>=0 then if nv<>1 then nok=k,break,end,end
    case 'lis'
      errcatch(-1,'continue','nomessage')
      v=evstr(str(k))
      errcatch(-1)
      if iserror(-1) then nok=-k,errclear(-1);break,end
      if type(v)<>15& type(v)<>16 then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      ssz=string(sz(1))
      nv=size(v)
      if sz(1)>=0 then if nv<>sz(1) then nok=k,break,end,end
    case 'r  '
      errcatch(-1,'continue','nomessage')
      v=evstr('['+str(k)+']')
      errcatch(-1)
      if iserror(-1) then nok=-k,errclear(-1);break,end
      if type(v)<>16 then nok=-k,break,end
      if h(1)<>'r' then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      [mv,nv]=size(v(2))
      ssz=string(sz(1))+' x '+string(sz(2))
      if mv*nv==0 then
	if  sz(1)>=0&sz(2)>=0&sz(1)*sz(2)<>0 then nok=k,break,end
      else
	if sz(1)>=0 then if mv<>sz(1) then nok=k,break,end,end
	if sz(2)>=0 then if nv<>sz(2) then nok=k,break,end,end
      end
    else
      error('type non gere :'+typ(2*k-1))
    end
    execstr('x'+string(k)+'=v')
  end
  if nok>0 then 
    message(['answer given for  '+labels(nok);
             'has invalid dimension: ';
             'waiting for dimension  '+ssz])
    ini=str
    ok=%f;break
  elseif nok<0 then
    message(['answer given for  '+labels(-nok);
             'has incorrect type :'+ typ(-2*nok-1)])
    ini=str
    ok=%f;break
  else
    break
  end 
end
if lhs==n+2 then
  execstr('x'+string(lhs-1)+'=str')
end

