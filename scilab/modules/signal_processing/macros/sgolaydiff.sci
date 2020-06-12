//This file is part of the Cardiovascular Wawes Analysis toolbox
//Copyright (C) 2014 - INRIA - Serge Steer
//Copyright (C) 2020 - Stéphane Mottelet (improvement of computation at boudaries)
//This file must be used under the terms of the CeCILL.
//This source file is licensed as described in the file COPYING, which
//you should have received as part of this distribution.  The terms
//are also available at
//http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function D=sgolaydiff(X,order,varargin)
  if type(X)<>1|~isreal(X) then
    error(msprintf(_("%s: Wrong type for argument %d: Real matrix expected.\n"),"sgolaydiff",1))
  end
  sz=size(X)
  if size(find(sz==1),'*')==1 then X = X(:);end
  i1=find(sz>1,1)
  if i1<>[] then X=matrix(X,sz(i1),-1);end

  if or(size(varargin)==[2 3]) then
    k=varargin(1);
    nf=varargin(2);
    if type(k)<>1|int(k)<>k|size(k,'*')<>1 then
      error(msprintf(_("%s: Wrong value for input argument #%d: An integer value expected.\n"),"sgolay",2))
    end
    if type(nf)<>1|int(nf)<>nf|size(nf,'*')<>1 then
      error(msprintf(_("%s: Wrong type for input argument #%d: a scalar with integer value expected.\n"),...
                     "sgolay",2))
    end
    if modulo(nf,2)==0 then nf=nf+1;varargin(2)=nf;end
    [B,C] = sgolay(varargin(:))
  else
    C=varargin(1)
    if type(C)<>1|~isreal(C) then
      error(msprintf(_("%s: Wrong type for argument %d: Real matrix expected.\n"),"sgolaydiff",2))
    end
    [nf,n]=size(C);
    k=n-1;
  end
   if size(X,1) < nf then
    error(msprintf(_("%s: Wrong size for argument %d: At least %d expected.\n"),"sgolaydiff",1,nf))
  end
  if order>k then
    error(msprintf(_("%s: Wrong value for argument %d: At most %d expected.\n"),"sgolaydiff",2,n-1))
  end
  nf2=floor(nf/2);
  //extend X on its boundaries by adding, in reverse order and vertically
  //symmetrized, copies of the first (nf−1)/2 points at the beginning and 
  //copies of the last (nf−1)/2 points at the end.
  X = [X(1)-X(nf2+1:-1:2,:);X;2*X($)-X($-1:-1:$-nf2,:)];
  for k=1:size(X,2)
    D(:,k)=((-1)^order*order)*conv(X(:,k),C(:,order+1)',"valid");
  end

  D=matrix(D,sz)
endfunction
