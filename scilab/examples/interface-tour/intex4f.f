       subroutine intex4f(fname)
       include 'stack.h'
c      --------------------------
c      Creating a scilab variable from a pointer
c      l1 is a pointer to an integer array created 
c      by the C function intarray (in file pgmsf.c)

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
       call intarray(l1,me1,ne1,err)

       if(err .gt. 0) then 
        buf = fname // 'Internal Error' 
        call error(998)
        return
       endif
c
       if(.not.createvarfromptr(1,'i',me1,ne1,l1)) return
       call freeptr(l1)
c
       lhsvar(1)=1
       end
c
