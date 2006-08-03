c     ********************
c     Copyright INRIA
c     ********************
      subroutine intvec2var 
      
      include 'stack.h'
      integer iadr,sadr
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      il1=iadr(lstk(top))
      if(istk(il1).ne.1) then
         err=1
         call error(44)
         return
      endif
      l1=sadr(il1+4)

      n=lstk(top+1)-l1
      call unsfdcopy(n,stk(l1),1,stk(lstk(top)),1)
      lstk(top+1)=lstk(top)+n
      
      end
c     ********************
