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
c     Copyright ENPC/INRIA
      include '../stack.h'
      integer n,il1,dl1 
      integer iadr,sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
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

      subroutine stackr2d(n,sl1,dl1) 
c     =============================
c     same as stacki2d but for reals 
c     to double 
c     ===============================
      include '../stack.h'
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

      subroutine stackc2i(n,sl1,il1) 
c     =============================
c     same as stacki2d but for characters
c     to integer and overlapping region 
c     ===============================
      include '../stack.h'
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

