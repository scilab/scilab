      subroutine stackr2d(n,sl1,dl1) 
c     =============================
c     same as stacki2d but for reals 
c     to double 
c     ===============================
      include 'stack.h'
      integer n,sl1,dl1 
      integer iadr,sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if((sl1+n-1).ge.iadr(dl1+n-1)) then
         call rea2db(n,istk(sl1),1,stk(dl1),1)
      elseif(sl1.le.iadr(dl1)) then
         call rea2db(n,istk(sl1),-1,stk(dl1),-1)
      else
         imont=sl1-iadr(dl1)
         ides=n-imont
         call rea2db(imont,istk(sl1),1,stk(dl1),1)
         call rea2db(ides,istk(sl1+imont),-1,stk(dl1+imont),-1)
      endif
      return
      end
c     ===============================

