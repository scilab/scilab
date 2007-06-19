      subroutine stackc2i(n,sl1,il1) 
c     =============================
c     same as stacki2d but for characters
c     to integer and overlapping region 
c     ===============================
      include 'stack.h'
      integer n,sl1,il1
      integer iadr,cadr
      iadr(l)=l+l-1
      cadr(l)=l+l+l+l-3      
      if((sl1+n-1).ge.cadr(il1+n-1)) then
         call cvstr(n,istk(il1),cstk(sl1:sl1+n),0)
      elseif( sl1.le.cadr(il1)) then
         call cvstr1(n,istk(il1),cstk(sl1:sl1+n),0)
      else
         kh=(sl1-cadr(il1))/3
         kb=n-kh
         call cvstr(kh,istk(il1),cstk(sl1:sl1+kh),0)
         lsuite=sl1+kh
         call cvstr1(kb,istk(il1+kh),cstk(lsuite:lsuite+kb),0)
      endif
      return
      end
c     ===============================

