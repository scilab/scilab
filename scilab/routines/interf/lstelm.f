      subroutine lstelm
c ================================== ( Inria    ) =============
c
c     evaluate utility list's functions
c
c =============================================================
c     

c     Copyright INRIA
c     
      include '../stack.h'
      integer ltyp
      integer iadr,sadr
c
      external rattyp,lsstyp,gratyp
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' lstelm '//buf(1:4))
      endif
c
c     functions/fin
c     1        2       3       4        5     6     7     8
c     list     tlist   rlist   lsslist glist lstcat mlist definedfields
c     9
c     lstsize

c
      rhs=max(0,rhs)
      if(top-rhs+lhs+1.ge.bot) then
         call error(18)
         return
      endif
c
      goto(01,01,20,30,40,50,01,60,70) fin
c
c
 01   continue
      call scilist('list')
      goto 99
 
 20   continue
c     rlist
      call mtlist(rattyp)
      if(err.gt.0) return
      goto 99
c
 30   continue
c     lsslist
      call mtlist(lsstyp)
      if(err.gt.0) return
      goto 99
c
 40   continue
c     glist
      call mtlist(gratyp)
      if(err.gt.0) return
      goto 99

 50   continue
c     lstcat
      call intlstcat()
      if(err.gt.0) return
      goto 99

 60   continue
c     definedfields
      call definedfields()
      if(err.gt.0) return
      goto 99

 70   continue
c     lstsize
      call intlstsize()
      if(err.gt.0) return
      goto 99


 99   return
      end
