
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - François DELEBECQUE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [X,dim,Y]=im_inv(A,B,tol)
//[X,dim]=im_inv(A,B [,tol]) computes (A^-1)(B) i.e vectors whose
// image through A are in range(B).
// The dim first columns de X span (A^-1) (B)
// tol is a threshold to test if a  subspace is included in an other
// default value tol = 100*%eps;

  [lhs,rhs]=argn(0);
  [nA,mA]=size(A);[nB,mB]=size(B);
  if rhs==2 then tol=100*%eps*mA*nA*nB*mB,end;
  if nA<>nB then 
    error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),'im_inv',1,2))
    return
  end
  // basis for im(B)
  [Y,rB]=rowcomp(B);//u=Y'
  
  //Trivial cases
  if rB >= nA then X=eye(mA,mA);dim=mA,return;end;
  if rB ==0 then [X,k1]=colcomp(A);dim=mA-k1,return,end
  //
  up=1:rB;low=rB+1:nA;
  A=Y*A;   //update 
  
  //vectors with image in B
  [X,r1]=colcomp(A(low,:))
  A1=A*X;    //update
  Aup=A1(up,:);
  Alow=A1(low,:);    //Alow(:,1:ma-r1)=0 by construction
  if norm(Alow,1) <= tol*norm(Aup,1) then dim=mA,return,end
  dim=mA-r1;
endfunction



