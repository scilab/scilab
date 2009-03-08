c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     SCILAB function : besseli, fin = 1
      subroutine intsbesseli(fname)
c     
      character*(*) fname
      include 'stack.h'
c     
      logical checkrhs,checklhs,getmat,getscalar,cremat
      double precision infinity
      double precision alpha
c
      rhs = max(0,rhs)
c     
      if(.not.checkrhs(fname,2,3)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable alpha (number 1)
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
      nb1=int(stk(lr1))
      alpha=stk(lr1)-nb1
      if(alpha.lt.0.0d0.or.alpha.gt.1.0d0.or.nb1.lt.0) then
         err=1
         call error(116)
         return
      endif
      if(m1*n1.gt.1) then
         do 01 i=2,m1*n1
            if(stk(lr1+i-1)-stk(lr1+i-2).ne.1.0d0) then
               err=1
               call error(116)
               return
            endif
 01      continue
      endif
      nb=m1*n1+nb1
c     
c     checking variable x (number 2)
      if(.not.getmat(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
      if(m2*n2.eq.0) then
         top=top-rhs+1
         if(.not.cremat(fname,top,0,0,0,lrs,lcs)) return
         return
      endif
      if(it2.ne.0) then
         err=2
         call error(52)
         return
      endif
      do 02 i=0,m2*n2-1
         if(stk(lr2+i).lt.0.0d0) then
            err=2
            call error(116)
            return
         endif
 02      continue
         
c
      if(rhs.eq.3) then
c     checking variable ize (number 3)
         if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
         ice=stk(lr3)
         if(ice.ne.1.and.ice.ne.2) then 
            err=3
            call error(44)
            return
         endif
      else
         ice=1
      endif
c     cross variable size checking
c     

      if(.not.cremat(fname,top+1,0,n2*m2,n1*m1,lw4,lwc4)) return

      if(.not.cremat(fname,top+2,0,1,nb,lw5,lwc5)) return
      nn5=1
      
      do 10 i=0,n2*m2-1
            call ribesl(stk(lr2+i),alpha,nb,ice,stk(lw5),ncalc)
            if(ncalc.ne.nb) then
               if(ncalc.lt.0.and.ice.eq.1) then
                  call dset(m1*n1,infinity(0.0d0),stk(lw4+i),n2*m2)
                  goto 10
               endif
               call error(24)
               return
            endif
            call unsfdcopy(m1*n1,stk(lw5+nb1),1,stk(lw4+i),n2*m2)
 10   continue
c     
      if(lhs .ge. 1) then
c     --------------output variable: b
         top=top-rhs+1
         if(.not.cremat(fname,top,0,n2*m2,n1*m1,lrs,lcs)) return
         call unsfdcopy(n2*m2*n1*m1,stk(lw4),1,stk(lrs),1)
      endif
      return
      end
      
