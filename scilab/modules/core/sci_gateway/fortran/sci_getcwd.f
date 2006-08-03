
      subroutine intsgetdir(fname)
c     --------------------------
      character*(*) fname
      logical checkrhs,checklhs
      include 'stack.h'
      logical putlhsvar, createvarfromptr
      double precision l1
      nbvars = 0
      rhs = max(0,rhs)
      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return
      call scigetcwd(l1,m,err)
      if(err .gt. 0) then 
         buf = fname // ': Internal Error' 
         call error(998)
         return
      endif
      if(.not.createvarfromptr(1,'c',m,1,l1)) return
      lhsvar(1)=1
      if(.not.putlhsvar()) return
      end
