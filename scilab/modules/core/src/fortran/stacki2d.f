c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


      subroutine stacki2d(n,il1,dl1) 
c     =============================
c     performs stk(dl1+j) = istk(il1+j)
c     but checks for overlapping areas
c     there's an equivalence between stk and istk 
c     to perform stk(dl1+j)=istk(il1+j) j=0,N-1 
c     we have three possible cases considering 
c     the 2 curves iadr(dl1+j) and il1+j 
c     -if ( iadr(dl1+j) < il1+j ) with 
c     possible equality for the last term 
c     the we copy from 0 to n-1
c     -if ( iadr(dl1+j) > il1+j ) with 
c     possible equality for the first term 
c     then we copy from n-1 to 0 (step= -1)
c     -if the two curves intersect 
c     ==> 2(dl1+p)-1 = il1+p 
c     p= il1- iadr(dl1) 
c     we copy from 0 to p-1 ( then istk(il1+k) k>=p
c     are not scratched and then from n-1 to p 
c     ===============================
      include 'stack.h'
      integer n,il1,dl1 
      integer iadr
      iadr(l)=l+l-1
      if((il1+n-1).ge.iadr(dl1+n-1)) then
         call int2db(n,istk(il1),1,stk(dl1),1)
      elseif(il1.le.iadr(dl1)) then
         call int2db(n,istk(il1),-1,stk(dl1),-1)
      else
         imont=il1-iadr(dl1)
         ides=n-imont
         call int2db(imont,istk(il1),1,stk(dl1),1)
         call int2db(ides,istk(il1+imont),-1,stk(dl1+imont),-1)
      endif
      return
      end
c     ===============================
