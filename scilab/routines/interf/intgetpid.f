      subroutine intgetpid
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1) then
         call error(42)
         return
      endif
      if(rhs.ge.1) then
         call error(39)
         return
      endif
      top=top+1
      il=iadr(lstk(top))
      l=sadr(il+4)
      err=l+1-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      call getpidc(idp)
      stk(l)=idp
      lstk(top+1)=l+1
      return
      end

