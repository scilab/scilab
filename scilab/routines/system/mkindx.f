      subroutine mkindx(n,m)
c     form the list which represents recursive index 
c     (i1)(i2)...(in)(k1,...,km)
c     i1,..in,k1,..km are supposed to be stored recursively in the stack
c!
c     Copyright INRIA
      include '../stack.h'
      integer n,m

      if(m.gt.1) then
c     .  last is (i,j)
         rhs=m
         fin=1
         lhs=1
         call lstelm
      endif
      if(n.eq.0) return
      rhs=n
      fin=1
      lhs=1
      call lstelm
      rhs=1
      return
      end
