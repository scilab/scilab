// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [sl]=syslin(domain,a,b,c,d,x0)

[lhs,rhs]=argn(0)
//
// check domain 
select type(domain)
case 1 then  //sampled system
  if size(domain,'*')<=2 then
    tp=domain
  else
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"),"syslin",1))
  end
  z='z'
case 10 //continuous or discrete
  if size(domain,'*')<>1 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"syslin",1))
  end
  domain=part(domain,1)
  select domain
  case 'c' then 
    z='s'
  case 'd' then 
    z='z'
  else 
    error(msprintf(gettext("%s: %s: Unknown time domain.\n"), "syslin",domain))
  end;
else 
  error(msprintf(gettext("%s: Wrong type for input argument #%d: Single, Scalar or [] matrix expected.\n"),"syslin",1))
end;
//============================================================================
if rhs==2 then //syslin(domaine,sys)

  if type(a)<>16 then 
    error()
  else
    if a(1)(1)=='r'|a(1)(1)=='lss' then
      sl=a;
      sl('dt')=domain
	else
	  error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"syslin",2))
    end
  end
//============================================================================
elseif rhs==3 then // syslin(domaine,num,den)
  num=a;den=b
  if type(num)>2 | type(den)>2 then
    error(msprintf(gettext("%s: %s and %s must be matrix of numbers or of polynomials.\n"),"syslin","N","D"))
  end
  if or(size(num)<>size(den)) then
    error(msprintf(gettext("%s: %s and %s have inconsistent dimensions.\n"),"syslin","N","D"))
  end

  if type(num)==2 & type(den)==2 then
    if varn(num)<>varn(den) then 
	  error(msprintf(gettext("%s: %s and %s have inconsistent formal variable names.\n"),"syslin","N","D"))
    end
  end
  if type(num)==1 then
    num=num*poly(1,z,'c')
  end
   if type(den)==1 then
    den=den*poly(1,z,'c')
  end
  
  sl=rlist(varn(num,z),varn(den,z),domain)
//============================================================================
elseif rhs>3 then // syslin(domaine,A,B,C [,D [X0]])
  if type(a)<>1 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of real expected.\n"),"syslin",2))
  end
  [ma,na]=size(a);
  if ma<>na then 
	error(msprintf(gettext("%s: Wrong type for input argument #%d: Square Matrix of real expected.\n"),"syslin",2))
  end
  if type(b)<>1 then
	error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of real expected.\n"),"syslin",3))
  end
  [mb,nb]=size(b);
  if na<>mb&mb<>0 then 
    error(msprintf(gettext("%s: Wrong size for input argument #%d and #%d.\n"),"syslin",2,3));
  end
  if type(c)<>1 then
	error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of real expected.\n"),"syslin",4))
  end
  [mc,nc]=size(c);
  if na<>nc&nc<>0 then 
	error(msprintf(gettext("%s: Wrong size for input argument #%d and #%d.\n"),"syslin",2,4));
  end
  if rhs<6 then
    x0=0*ones(na,1)
  else
    if type(x0)>1 then
	  error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of real expected.\n"),"syslin",6))
    end
    [mx,nx]=size(x0);
    if mx<>na|nx<>min(na,1) then 
	  error(msprintf(gettext("%s: Wrong size for input argument #%d.\n"),"syslin",6));
    end
  end
  if rhs<5  then
    d=0*ones(mc,nb)
  else
    if type(d)>2 then
	  error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of real and polynomials expected.\n"),"syslin",5))
    end
    [md,nd]=size(d);
    if c*b<>[] then
      if mc<>md|nb<>nd then 
		error(msprintf(gettext("%s: Wrong size for input argument #%d, #%d and #%d.\n"),"syslin",3,4,5));

      end
    end
  end
  sl=lsslist(a,b,c,d,x0,domain)
end
 
endfunction
