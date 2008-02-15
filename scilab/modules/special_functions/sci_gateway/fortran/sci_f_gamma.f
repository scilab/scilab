
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     SCILAB function : lgamma, fin = 5
      subroutine intsgamma(fname)
c     
      character*(*) fname
      include 'stack.h'
c     
      integer iadr, sadr
      integer topk,rhsk,topl
      logical checkrhs,checklhs,getmat,getscalar,cremat
      double precision dgammacody
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c     
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable x (number 1)
      if(.not.getmat(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
      if(m1*n1.eq.0) then
         return
      endif
      if(it1.ne.0) then
         err=1
         call error(52)
         return
      endif

*** modif bruno : reused Cody 's gamma (waiting final decision...)
*      So I have commented the following line (needed when using the
*      gamma func from Slatec)
c$$$      if (setslatecjmp().ne.0) then
c$$$         buf='Slatec fatal error'
c$$$         call error(999)
c$$$         return
c$$$      endif
      
      do 10 i=0,n1*m1-1
         stk(lr1+i)=dgammacody(stk(lr1+i))
 10   continue
***end bruno 's modif
c     
      return
      end
      