      logical function ptover(inc,icheck)
c     Copyright INRIA
      integer inc,icheck
C     increment pt with inc and 
C     checks overflow in pt counter 
C     returns true if overflow
      include '../stack.h'
      pt=pt+inc 
      if(pt.gt.icheck) then
         call error(26)
         ptover=.true.
      else
         ptover=.false.
      endif
      return
      end


      logical function eptover(inc,icheck)
      integer inc,icheck
C     like ptover but returns true 
C     if both pt overflows and err is present 
      include '../stack.h'
      pt=pt+inc 
      if(pt.gt.icheck) then
         call error(26)
         if ( err.gt.0) eptover=.true.
      else
         eptover=.false.
      endif
      return
      end
