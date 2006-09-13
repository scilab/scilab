c     -------------------------------
c	INRIA
c     -------------------------------
      subroutine intchinesehat(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      
      fun=-1
      il=iadr(lstk(top-rhs+1))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      call funnam(ids(1,pt+1),'pow',il)
      
      return
      end
c     -------------------------------
