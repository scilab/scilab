function [ok,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18]=getvalue(desc,labels,typ,ini)
//  getvalues - xwindow dialog for data acquisition 
//%Syntax
//  [ok,x1,..,x11]=getvalue(desc,labels,typ,ini)
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
//                   'str' : stands for vector of strings
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
// getvalues macro encapsulate x_mdialog function with error checking,
// evaluation of numerical response, ...
//%Remarks
// All correct scilab syntax may be used as responses, for matrices 
// and vectors getvalues automatically adds [ ] around the given response
// before numerical evaluation
//%Example
// labels=['magnitude';'frequency';'phase    '];
// [ampl,freq,ph]=getvalue('define sine signal',labels,..
//            list('vec',1,'vec',1,'vec',1),['0.85';'10^2';'%pi/3'])
// 
//%See also
// x_mdialog, x_dialog
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
%nn=prod(size(labels))
if lhs<>%nn+2&lhs<>%nn+1 then error(41),end
if size(typ)<>2*%nn then
  error('typ : list(''type'',[sizes],...)')
end
x1=[];x2=[];x3=[];x4=[];x5=[];x6=[];x7=[];x8=[];x9=[];x10=[];x11=[];
x12=[];x13=[];x14=[];
x15=[];x16=[];x17=[];x18=[];
if rhs==3 then  ini=emptystr(%nn,1),end
ok=%t
while %t do
  str1=x_mdialog(desc,labels,ini)
  if str1==[] then ok=%f,str=[];break,end
  str=str1;
  for k=1:%nn
    cod=str2code(str(k))
    spe=find(cod==99)
    if spe<>[] then
      semi=str2code(';')
      cod(spe)=semi*ones(spe')
      str(k)=code2str(cod)
    end
  end
  nok=0
  for k=1:%nn
    select part(typ(2*k-1),1:3)
    case 'mat'
      ierr=execstr('v=['+str(k)+']','errcatch');
      if ierr<>0 then nok=-k;break,end
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
      ierr=execstr('v=['+str(k)+']','errcatch')
      if ierr<>0 then nok=-k;break,end
      if type(v)<>1 then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      ssz=string(sz(1))
      nv=prod(size(v))
      if sz(1)>=0 then if nv<>sz(1) then nok=k,break,end,end
    case 'pol'
      ierr=execstr('v=['+str(k)+']','errcatch');
      if ierr<>0 then nok=-k;break,end
      if type(v)>2 then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      ssz=string(sz(1))
      nv=prod(size(v))
      if sz(1)>=0 then if nv<>sz(1) then nok=k,break,end,end
    case 'row'
      ierr=evstr('v=['+str(k)+']','errcatch');
      if ierr<>0 then nok=-k;break,end
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
      ierr=execstr('v=['+str(k)+']','errcatch');
      if ierr<>0 then nok=-k;break,end      
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
      s=str1(k)
      spe=find(str2code(str1(k))==99)
      spe($+1)=length(s)+1
      v=[];k1=1
      for kk=1:size(spe,'*')
	v(kk,1)=part(s,k1:spe(kk)-1)
	k1=spe(kk)+1
      end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      ssz=string(sz(1))
      nv=prod(size(v))
      if sz(1)>=0 then if nv<>sz(1) then nok=k,break,end,end
    case 'lis'
      ierr=execstr('v='+str(k),'errcatch');
      if ierr<>0 then nok=-k;break,end      
      if type(v)<>15& type(v)<>16 then nok=-k,break,end
      sz=typ(2*k);if type(sz)==10 then sz=evstr(sz),end
      ssz=string(sz(1))
      nv=size(v)
      if sz(1)>=0 then if nv<>sz(1) then nok=k,break,end,end
    case 'r  '
      ierr=execstr('v=['+str(k)+']','errcatch');
      if ierr<>0 then nok=-k;break,end 
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
    x_message(['answer given for  '+labels(nok);
             'has invalid dimension: ';
             'waiting for dimension  '+ssz])
    ini=str
  elseif nok<0 then
    if ierr==0 then
      x_message(['answer given for  '+labels(-nok);
	'has incorrect type :'+ typ(-2*nok-1)])
    else
      x_message(['answer given for  '+labels(-nok);
	'is incorrect see error message in scilab window'])
    end
    ini=str
  else
    break
  end 
end
if lhs==%nn+2 then
  execstr('x'+string(lhs-1)+'=str')
end

