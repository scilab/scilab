       subroutine intex6f(fname)
       include 'stack.h'
c      --------------------------
c      Creating a scilab variables from pointers
c      l1,l2,l3 are pointer to arrays created 
c      by the C functions defined in file pgmsf.c

       character*(*) fname
       logical checkrhs,checklhs
       logical createvarfromptr

c      The pointers are stored in a double precision format.
       double precision l1,l2,l3
c     
       minrhs = 0
       maxrhs = 0
       maxlhs = 3
c     
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,1,maxlhs)) return
c     
       call crestr(l1,m,err)
       call intarray(l2,me2,ne2,err)
       call dblearray(l3,me3,ne3,err)       

       if(err .gt. 0) then 
        buf = fname // 'Internal Error' 
        call error(998)
        return
       endif
c
       if(.not.createvarfromptr(1,'c',m,1,l1)) return
       if(.not.createvarfromptr(2,'i',me2,ne2,l2)) return
       if(.not.createvarfromptr(3,'d',me3,ne3,l3)) return

       call freeptr(l1)
       call freeptr(l2)
       call freeptr(l3)
c
       lhsvar(1)=1
       lhsvar(2)=2
       lhsvar(3)=3

       end



