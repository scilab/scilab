// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function elts=pfss(S,rmax,cord)
//Syntax : elts=pfss(S)
//Partial fraction decomposition of the linear system S (in state-space form):
// elts is the list of linear systems which add up to S
// i.e. elts=list(S1,S2,S3,...,Sn) with S1 + S2 +... +Sn = S
// Each Si contains some poles of S according to the block-diagonalization
// of the A matrix of S.
// If S is given in transfer form, it is first converted into state-space
// and each subsystem is then converted in transfer form.
//!
  select argn(2)
  case 1 then
    rmax=[];cord=[]
  case 2 then
    if type(rmax)==10 then cord=rmax;end
    if type(rmax)==1 then cord=[];end
  end
  
  if and(typeof(S)<>['rational','state-space']) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"pfss",1))
  end
  if typeof(S)=='rational' then 
    S=tf2ss(S),flag=%T
  else
    flag=%F
  end

  [f,g,h,dd,dom]=S([2:5,7]);
   [n,n]=size(f);
  if rmax==[] then
    [f,x,bs]=bdiag(f);
  else
    [f,x,bs]=bdiag(f,rmax);
  end
  h=h*x;g=x\g;
  k=1;ll=0;
  elts=list();
  for l=bs',
    ind=k:k+l-1;
    f1l=f(ind,ind);
    gl=g(ind,:);
    hl=h(:,ind);
    elts(ll+1)=syslin('c',f1l,gl,hl)
    ll=ll+1;k=k+l;
  end;
  if argn(2)==2  then
    select cord
    case 'c'
      nb=size(bs,'*');
      class=[];
      for k=1:nb
	oneortwo=bs(k);ss=elts(k);A=ss(2);
	if oneortwo==1 then 
	  class=[class,real(spec(A))];
	end
	if oneortwo>1 then 
	  class=[class,mini(real(spec(A)))];
	end
      end;
      [cl,indi] = gsort(-class);
      elts1=elts;
      for k=1:size(elts);
        elts(k)=elts1(indi(k));
      end
    case 'd'
      nb=size(bs,'*');
      class=[];
      for k=1:nb
	oneortwo=bs(k);ss=elts(k);A=ss(2);
	if oneortwo==1 then 
	  class=[class,abs(spec(A))];
	end
	if oneortwo>1 then 
	  class=[class,maxi(abs(spec(A)))];
	end
      end;
      [cl,indi] = gsort(-class);
      elts1=elts;
      for k=1:size(elts);
        elts(k)=elts1(indi(k));
      end
    end
  end
  if type(dd)==1 then
    if norm(dd,'fro')<>0 then elts(ll+1)=dd,end
  end
  if type(dd)==2 then 
    if norm(coeff(dd),1) > %eps then elts(ll+1)=dd,end
  end
  if flag then
    k=size(elts);
    for kk=1:k,elts(kk)=ss2tf(elts(kk));end
  end
endfunction
