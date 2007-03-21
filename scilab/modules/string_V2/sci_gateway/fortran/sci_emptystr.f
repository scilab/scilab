c =========================================
c INRIA
c =========================================  
      subroutine intemptystr(id)
      INCLUDE 'stack.h'
c
      integer id(nsiz)
      logical ref
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(lhs.ne.1) then 
         call error(59)
         return
      endif


      if(rhs.eq.2) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=2
            call error(53)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         if(istk(il+3).ne.0) then
            err=2
            call error(52)
            return
         endif
         n=int(stk(sadr(il+4)))
         top=top-1
c
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=1
            call error(53)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            call error(89)
            return
         endif
         if(istk(il+3).ne.0) then
            err=1
            call error(52)
            return
         endif
         m=int(stk(sadr(il+4)))
        if (m.eq.0.or.n.eq.0) then
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
            goto 999
         endif
      elseif(rhs.eq.1) then
         il=iadr(lstk(top))
         ilr=il
         if(istk(il).lt.0) il=iadr(istk(il+1))

         if(istk(il).gt.10) then
            fun=-1
            call funnam(ids(1,pt+1),'emptystr',il)
            return
         endif
         ref=ilr.ne.il
         m=istk(il+1)
         n=istk(il+2)
         if(m*n.eq.0) then
            if(ref) call ref2val
            return
         endif
      elseif(rhs.le.0) then
         m=1
         n=1
         top=top+1
      else
         call error(42)
         return
      endif
      il=iadr(lstk(top))
      istk(il)=10
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=0
      call iset(m*n+1,1,istk(il+4),1)
      lstk(top+1)=sadr(il+6+m*n)
c     The correct assignation should be 
c     lstk(top+1)=sadr(il+5+m*n)
      goto 999 
 999  return
      end
c =========================================  
