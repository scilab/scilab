       subroutine intex3f(fname)
       include 'stack.h'

c      --------------------------
c      Creating a scilab variable from a pointer
c      l1 is a pointer to a double array created 
c      by the C function dblearray (in file pgmsf.c)

       character*(*) fname
       logical checkrhs,checklhs
       logical createvarfromptr
c      The pointer is stored in a double precision format.
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
       call dblearray(l1,me1,ne1,err)
c
       if(err .gt. 0) then 
        buf = fname // 'Internal Error' 
        call error(998)
        return
       endif
c
c      Creating variable (matrix) #1 of type double and dimensions me1 x ne1
       if(.not.createvarfromptr(1,'d',me1,ne1,l1)) return
       call freeptr(l1)
c
c      Returning to Scilab variable #1
       lhsvar(1)=1
       end
c

