      subroutine polaut
c ====================================================================
c      polynomial stuff
c ====================================================================
c
c     Copyright INRIA
      include '../stack.h'
      integer iadr,sadr
c
      integer vol1,vol2,vol3,var(4)
      double precision v,eps,errl2,phi,gnrm
      logical all
      common/no2f/gnrm
      common/arl2c/info,ierr
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) write(wte,1000) fin
 1000 format(1x,'polaut',i4)
c
c     functions/fin
c     1    2   3    4    5
c   arl2 resi ldiv     
c
      if(top+lhs-rhs.ge.bot) then
         call error(18)
         return
      endif
      if(rhs.le.0) then
         call error(39)
         return
      endif
c
      eps=stk(leps)
c
      lw=lstk(top+1)
c
      il1=iadr(lstk(top+1-rhs))
      if(istk(il1).gt.2) then
         err=1
         call error(44)
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      mn1=m1*n1
      if(istk(il1).eq.1) goto 01
      id1=il1+8
      vol1=istk(id1+mn1)-1
      l1r=sadr(id1+mn1+1)
      l1i=l1r+vol1
      goto 05
   01 id1=iadr(lw)
      l1r=sadr(il1+4)
      l1i=l1r+mn1
      lw=sadr(id1+mn1+1)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      do 02 i=1,mn1
   02 istk(id1+i-1)=i
      istk(id1+mn1)=mn1+1
      vol1=mn1
c
   05 goto (80,20,40,99,99) fin
c
c residu
  20  continue
      call sciresidu('residu')
      return
c
c
c ldiv
  40  continue
      call scildiv('ldiv')
      return
c
c arl2
   80 continue
      call sciarl2('arl2')
      return

   99 return
      end
