      subroutine i_kron()
      INCLUDE '../stack.h'
      external memused,uppertype
      integer memused,uppertype
c
      integer tops
      logical refa,refb
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      tops=top
c
      if (rhs .ne. 2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif

      il=iadr(lstk(top))
      ilb=il
      if(istk(il).lt.0) il=iadr(istk(il+1))

      refb=il.ne.ilb
      mb=istk(il+1)
      nb=istk(il+2)
      itb=istk(il+3)
      lb=il+4
      mnb=mb*nb
      top=top-1

      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))

      refa=il.ne.ilr
      ma=istk(il+1)
      na=istk(il+2)
      ita=istk(il+3)
      la=il+4
      mna=ma*na
c
      it=uppertype(ita,itb)
      l=ilr+4
      l1=l+memused(it,mnb*mna)
      lstk(top+1)=sadr(l1)
c
c move a and b if necessary
      lw=l1
      if(.not.refb.or.itb.ne.it) lw=lw+memused(it,mnb)
      if(.not.refa.or.ita.ne.it) lw=lw+memused(it,mna)
      err=sadr(lw)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      lw=l1
      if(.not.refb.or.itb.ne.it) then
         call tpconv(itb,it,mnb,istk(lb),-1,istk(lw),-1)
         lb=lw
         lw=lw+memused(itb,mnb)
      endif
      if(.not.refa.or.ita.ne.it) then
         call tpconv(ita,it,mna,istk(la),-1,istk(lw),-1)
         la=lw
      endif
      call genkron(it,istk(la),ma,ma,na,istk(lb),mb,mb,nb,istk(l),ma*mb)

 999  continue
      istk(ilr)=8
      istk(ilr+1)=mb*ma
      istk(ilr+2)=nb*na
      istk(ilr+3)=it
      return
      end
