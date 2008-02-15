
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     SCILAB function : calerf, fin = 7
      subroutine intscalerf(fname)
c     
      character*(*) fname
      include 'stack.h'
c     
      integer iadr, sadr
      integer topk,rhsk,topl
      logical checkrhs,checklhs,getmat,getscalar,cremat
      double precision alpha
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c     
      if(.not.checkrhs(fname,2,2)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable x (number 1)
      if(.not.getmat(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
      if(m1*n1.eq.0) then
         top=top-rhs+1
         return
      endif
      if(it1.ne.0) then
         err=1
         call error(52)
         return
      endif
c     checking variable jint (number 2)
      if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
      jint=stk(lr2)
      if(jint.lt.0.or.jint.gt.2) then 
         err=2
         call error(44)
         return
      endif

      do 10 i=0,n1*m1-1
         call calerf(stk(lr1+i),stk(lr1+i),jint)
 10   continue
      top=top-1
c     
      return
      end

