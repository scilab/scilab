      subroutine i_matrix()
      INCLUDE '../stack.h'

      integer tops,top2
      integer iadr,sadr
      logical ref
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      tops=top

      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.lt.2) then
         call error(39)
         return
      endif
      if(rhs.gt.3) then
        top=tops
         call ref2val
         call setfunnam(ids(1,pt+1),'%hm_matrix',10)
         fun=-1
         return
      endif

      top2=top-rhs+1
      il2=iadr(lstk(top2))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))


c changement de dimension d'une matrice

      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).ne.1) then
         err=3
         call error(53)
         return
      endif
      if(rhs.eq.2) then
         if(istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).eq.1) then
            m=int(stk(sadr(il+4)))
            n=1
         elseif(istk(il+1)*istk(il+2).eq.2) then
            m=int(stk(sadr(il+4)))
            n=int(stk(sadr(il+4)+1))
         else
            top=tops
            call ref2val
            call funnam(ids(1,pt+1),'matrix',iadr(lstk(top-rhs+1)))
            fun=-1
            return
         endif
      else
         if(istk(il+1)*istk(il+2).ne.1) then
            err=3
            call error(89)
            return
         endif
         if(istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         n=int(stk(sadr(il+4)))
         if(n.lt.-1) then
            err=3
            call error(116)
            return
         endif

c     
         top=top-1
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
         m=int(stk(sadr(il+4)))
         if(m.lt.-1) then
            err=2
            call error(116)
            return
         endif

      endif
c
      top=top-1
      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) then
         k=istk(il+2)
         err=lstk(top)+lstk(k+1)-lstk(k)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(lstk(k+1)-lstk(k),stk(lstk(k)),1,
     $        stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(k+1)-lstk(k)
      endif

      mn=istk(il+1)*istk(il+2)
      if(m.eq.-1.and.n.eq.-1) then
         call error(42)
         return
      endif
      if(m.eq.-1) m=mn/n
      if(n.eq.-1) n=mn/m

      if(m*n.ne.istk(il+1)*istk(il+2)) then
         call error(60)
         return
      endif
      if(m*n.eq.0) then
         istk(il+1)=0
         istk(il+2)=0
      else
         istk(il+1)=m
         istk(il+2)=n
      endif
 999  return
      end
