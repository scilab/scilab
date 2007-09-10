      subroutine mkindx(n,m)
c     form the list which represents recursive index 
c     (i1)(i2)...(in)(k1,...,km)
c     i1,..in,k1,..km are supposed to be stored recursively in the stack
c!
c     Copyright INRIA
      include '../stack.h'
      integer n,m
      
      if(err1.gt.0) return
      lhss=lhs
      if(m.gt.1) then
c     .  last is (i,j)
         rhs=m
         fin=1
         lhs=1
         call lstelm
      endif
      if(n.eq.0) goto 10
      rhs=n
      fin=1
      lhs=1
      call lstelm
      rhs=1
 10   continue
      lhs=lhss 
      return
      end
