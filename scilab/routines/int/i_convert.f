      subroutine i_convert
      include '../stack.h'
c
      logical checkrhs,checklhs
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(.not.checkrhs('iconvert',2,2)) return
      if(.not.checklhs('iconvert',1,1)) return

      top0=top
      il=iadr(lstk(top))
      if(istk(il).lt.0) il=istk(il+1)
      if(istk(il).ne.1) then
         err=2
         call error(53)
         return
      endif
      is=stk(sadr(il+4))
      top=top-1
      rhs=rhs-1
      call stack_convert(is)
      return
      end

