       subroutine intex9f(fname)
       include 'stack.h'
c      --------------------------
c      Passing and returning a string
       character*(*) fname
       logical checkrhs,checklhs
       logical getrhsvar
c     
       minrhs = 1
       maxrhs = 1
       minlhs = 1
       maxlhs = 1
c     
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
c     
       if(.not.getrhsvar(1,'c',m,n,l1)) return
       call as2osf(cstk(l1:l1+m*n-1))
c     
       lhsvar(1)=1
       return 
       end
c
