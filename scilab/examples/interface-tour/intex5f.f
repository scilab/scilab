       subroutine intex5f(fname)
       include 'stack.h'
c      --------------------------
c      Creating a scilab variable from a pointer
c      l1 is a pointer to an char array created 
c      by the C function crestr (in file pgmsf.c)

       character*(*) fname
       logical checkrhs,checklhs
       logical createvarfromptr
c      The pointer is stored in double precision format
       double precision l1
c     
       minrhs = 0
       maxrhs = 0
       minlhs = 1
       maxlhs = 1
c     
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
c     
       call crestr(l1,m,err)

       if(err .gt. 0) then 
         buf = fname // 'Internal Error' 
         call error(998)
         return
       endif
c
       if(.not.createvarfromptr(maxrhs+1,'c',m,1,l1)) return
       call freeptr(l1)
c
       lhsvar(1)=1
       end

