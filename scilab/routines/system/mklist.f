       subroutine mklist(n)
c     form the list built with last n variables stored in top-n+1:top
c      positions of the stack
c!
c     Copyright INRIA
      call mklistt(n,15)
      return
      end
      subroutine mktlist(n)
c     form the list built with last n variables stored in top-n+1:top
c      positions of the stack
c!
c     Copyright INRIA
      call mklistt(n,16)
      return
      end
      subroutine mkmlist(n)
c     form the list built with last n variables stored in top-n+1:top
c      positions of the stack
c!
c     Copyright INRIA
      call mklistt(n,17)
      return
      end

      subroutine mklistt(n,typ)
c     form the list built with last n variables stored in top-n+1:top
c      positions of the stack
c!
c     Copyright INRIA
      include '../stack.h'
      integer n,typ,srhs,slhs,sfin
c
      srhs=rhs
      slhs=lhs
      sfin=fin
c
      rhs=n
      if(typ.eq.15) then
         fin=1
      elseif(typ.eq.16) then
         fin=2
      elseif(typ.eq.17) then
         fin=7
      endif

      lhs=1
      call ref2val
      call lstelm
      rhs=srhs
      lhs=slhs
      fin=sfin
      return
      end
