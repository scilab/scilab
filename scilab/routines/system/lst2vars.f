      subroutine lst2vars(il,m)
c =============================================================
c     extract all variables contained in a list and store them
c     at the top of the stack
c =============================================================
c
c     Copyright INRIA
      include '../stack.h'
c
c     il : pointer on the beginning of the list
c     on return
c     m  : number of variable created. top is incremented
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(istk(il).lt.0) il=iadr(istk(il+1))
      m=istk(il+1)

      ll=sadr(il+m+3)
      top=top+1
      err=lstk(top)+istk(il+m+2)-1-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(top+m.ge.bot) then
         call error(18)
         return
      endif
      call unsfdcopy(istk(il+m+2)-1,stk(ll),1,stk(lstk(top)),1)
      do 41 k1=1,m
         lstk(top+1)=lstk(top)+istk(il+2+k1)-istk(il+1+k1)
         top=top+1
 41   continue
      top=top-1
      return
      end
