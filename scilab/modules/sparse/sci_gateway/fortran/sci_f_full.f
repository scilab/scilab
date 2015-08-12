c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine intfull(id)
      include 'stack.h'
      integer id(nsiz),top0
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif 
      il=iadr(lstk(top))
      if(istk(il).eq.1.or.istk(il).eq.2.or.istk(il).eq.4) return
      if(istk(il).ne.5.and.istk(il).ne.6) then
         call funnam(ids(1,pt+1),'full',iadr(lstk(top)))
         fun=-1
         return
      endif
      nel=istk(il+4)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)

*     take care of the fact that m*n*(1+it) may be too big for
*     integer arithmetic (and then can lead to stack corruption)
*     (added by Bruno trapping the m x n problems ...)
*     A solution is to do the following test:
*      if ( dble( m*n*(1+it) ) .ne. dble(m)*dble(n)*dble(1+it) ) then
*          raise an error
*
*      but further computations may also be corrupted for instance
*      sadr(il+4)+m*n*(it+1) ...  (m*n*(1+it) may be just good in
*      integer arithmetic but then adding with  sadr(il+4) may
*      produce an integer overflow.
*
*     So a safer test is to limit to m*n <= 30000 x 30000 which
*     represent in full format a very big matrix (7.2 Go in double
*     precision) together with enough security for the other computations
*      30000 x 30000 = 900 000 000 < 2 147 483 647 = 2^31-1 
*
      if ( dble(m)*dble(n) .gt. 900000000 ) then
         buf='not enough memory'
         call error(9999)
         return
      endif

      ilr=il+5
      ilc=ilr+m
      if(istk(il).eq.5) then
c     matrix of scalar
         l=sadr(ilc+nel)
         ils=iadr(max(sadr(il+4)+m*n*(it+1),lw))
         ls=sadr(ils+m+nel)
         lw=ls+nel*(it+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(m+nel,istk(ilr),1,istk(ils),1)
         call unsfdcopy(nel*(it+1),stk(l),1,stk(ls),1)
         istk(il)=1
         l=sadr(il+4)
         if(it.eq.0) then
            call dspful(m,n,stk(ls),nel,istk(ils),stk(l))
         else
            call wspful(m,n,stk(ls),stk(ls+nel),nel,istk(ils),
     $           stk(l),stk(l+m*n))
         endif
         lstk(top+1)=l+m*n*(it+1)
      else
c     matrix of boolean
         ils=max(il+3+m*n,iadr(lw))
         lw=sadr(ils+m+nel)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(m+nel,istk(ilr),1,istk(ils),1)
         istk(il)=4
         l=il+3
         call lspful(m,n,nel,istk(ils),istk(l))
         lstk(top+1)=sadr(l+m*n)
      endif
      return
      end
c			======================================
