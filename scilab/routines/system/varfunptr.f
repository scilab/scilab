      subroutine varfunptr(id,ffun,ffin)
c     ---------------------------------
c     creates a variable with is a pointer on a primitive function 
C     ---------------------------------
c     Copyright INRIA
      include '../stack.h'

      logical compil,cremat
      integer bl(nsiz),id(*),ffun,ffin,iadr,sadr
      data bl/nsiz*673720360/
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(err1.gt.0) return
c     compilation  [27]
      if(compil(27,id,ffun,ffin,0)) return 
      if(top+2 .ge. bot) then
         call error(18)
         return
      endif
      top = top+1
      call putid(idstk(1,top),bl)
      infstk(top)=0
      il=iadr(lstk(top))
      istk(il)=130
      istk(il+1)=ffun
      istk(il+2)=ffin
      call putid(istk(il+3),id)
      lstk(top+1)=sadr(il+3+nsiz)
      return
      end

      subroutine isafunptr(k,id,ffun,ffin)
      include '../stack.h'
      integer iadr,sadr
      integer id(*),ffun,ffin

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      ffun=0
      ffin=0
      il=iadr(lstk(k))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).eq.130) then
         ffun=istk(il+1)
         ffin=istk(il+2)
         call putid(id,istk(il+3))
      endif
      return
      end
