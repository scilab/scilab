C      ------------------------------------------------------
C      * Creating a scilab variable from a pointer
C      *-------------------------------------------------------*/

       subroutine intex4f1(fname)
       include 'stack.h'

c      --------------------------
c      Creating a scilab variable from a pointer
c      l1 is a pointer to a double array created 

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

       subroutine intex4f2(fname)
       include 'stack.h'
c      --------------------------
c      Creating a scilab variable from a pointer
c      l1 is a pointer to an integer array created 

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
       subroutine intex4f3(fname)
       include 'stack.h'
c      --------------------------
c      Creating a scilab variable from a pointer
c      l1 is a pointer to an char array created 

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


       subroutine intex4f4(fname)
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






