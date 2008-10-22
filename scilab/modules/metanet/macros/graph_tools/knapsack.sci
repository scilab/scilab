
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [earn,ind]=knapsack(profit,weight,capa,bck)
  [lhs,rhs]=argn(0)
  if (rhs<3|rhs>4) then 
    error(39)
  elseif (rhs==3) then 
    bck=-1;
  end;
  if (bck<0) then bck=-1;end
  n=size(profit,'*');
  // check input argument validity
  if min(size(profit))<>1|n<2 then
    error(msprintf(_("%s: First argument must be a vector of size greater or equal to 2.\n"),"knapsack"))
  end
  if min(size(weight))<>1|size(weight,'*')<>n then
    error(msprintf(_("%s: Second argument must be a vector with same dimension as the first one.\n"),"knapsack"))
  end
  if min(size(capa))<>1|size(profit,'*')<1 then
    error(msprintf(_("%s: Third argument must be a vector of size greater or equal to 1.\n"),"knapsack"))
  end

  if (min(profit) <= 0)|or(round(profit)<>profit) then
    error(msprintf(_("%s: First argument must be a vector with positive integer values.\n"),"knapsack"))
  end;
  if (min(weight) <= 0)|or(round(weight)<>weight) then
    error(msprintf(_("%s: Second argument must be a vector with positive integer values.\n"),"knapsack"))
  end;
  if (min(capa) <= 0)|or(round(capa)<>capa) then
    error(msprintf(_("%s: Third argument must be a vector with positive integer values.\n"),"knapsack"))
  end;
  //transform into row vector
  profit = matrix(profit,1,-1)
  weight = matrix(weight,1,-1)
  capa   = matrix(capa,1,-1)
  //preprocess the data
  [s,k]=sort(profit./weight);
  [ss,kk]=sort(-k);
  p=profit(k);w=weight(k);
  np1=n+1;
  m=size(capa,2);
  [s1,k1]=sort(capa);
  [s2,k2]=sort(k1);
  kap=s1($:-1:1);
  mn=m*n;mnp1=m*np1;
  [xstar,vstar]=m6knapsk(n,m,np1,mn,mnp1,p,w,kap,bck);

  earn=vstar;
  //if(vstar==-3) then print(%io(2),'A knapsack cannot contain any item');end;
  //if(vstar==-4) then print(%io(2),'An item cannot fit into any knapsack');end;
  //if(vstar==-5) then print(%io(2),'A knapsack contains all the items'); end;
  xstar=xstar(kk);
  ij=find(xstar<>0);
  ind=zeros(xstar);
  ind(ij)=k2(xstar(ij));
endfunction
