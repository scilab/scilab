      subroutine scifunc(mlhs,mrhs)
c     execute scilab function with mrhs input args and mlhs output
c     variables
c     input args are supposed to be stored in the top of the stack
c     at positions top-mrhs+1:top
c     function adress is given by the integer ptr in the common /scsptr/
c
c     Copyright INRIA
      include "../stack.h"
      integer ptr
      integer mlhs,mrhs
      logical allowptr
C
      common /scsptr/ ptr   
c

C     macro execution 
C     
      pt = pt + 1
      if (pt .gt. psiz) then
        call error(26)
        goto 9999
      endif
      ids(1,pt) = lhs
      ids(2,pt) = rhs
      rstk(pt) = 1001
      lhs = mlhs
      rhs = mrhs
      niv = niv + 1
      fun = 0
      fin = ptr
C     
      icall = 5
      krec = -1
      include "../callinter.h"
 200  lhs = ids(1,pt)
      rhs = ids(2,pt)
      pt = pt - 1
      niv = niv - 1
      return
C+    
 9999 continue
      if(err1.gt.0) then
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      iero=1
      niv=niv-1
      return
      end
