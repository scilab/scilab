      subroutine lstelmi
c ================================== ( Inria    ) =============
c
c     evaluate utility list's functions with possible indirect args
c
c =============================================================
c     

c     Copyright INRIA
c     
      include '../stack.h'
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' lstelmi '//buf(1:4))
      endif
c
c     functions/fin
c     1        2       
c     getfield setfield
c
c
      rhs=max(0,rhs)
      if(top-rhs+lhs+1.ge.bot) then
         call error(18)
         return
      endif
c
      goto(10,20) fin
 10   continue
c     getfield
      call intgetfield()
      if(err.gt.0) return
      goto 99

 20   continue
c     setfield
      call intsetfield()
      if(err.gt.0) return
      goto 99

 99   return
      end
