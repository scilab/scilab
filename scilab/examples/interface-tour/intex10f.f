       subroutine intex10f(fname)
c      --------------------------
c      Get a pointer to a Scilab variable with name "param"
       character*(*) fname
       logical checkrhs,checklhs
       include 'stack.h'
       logical cmatptr,createvar
c     
       minrhs = 0
       maxrhs = 0
       maxlhs = 1
       minlhs = 1
c     
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
c     
c      get matrix ptr
c      param is m x n and param(1), param(2) ...
c      is equal to        stk(lp),  stk(lp+1), ...
       if(.not.cmatptr('param'//char(0),m,n,lp)) return 

c      Creating a Scilab variable (#1) of type double (matrix)
c      with m rows and n columns (l1 is the output of createvar).
       if(.not.createvar(1,'d',m,n,l1)) return
c 
c      Copy m*n entries from lp to l1:
c      stk(l1)=stk(lp), stk(l1+1)=stk(lp+1), ...    
       call dcopy(m*n,stk(lp),1,stk(l1),1)
       lhsvar(1)=1
       return
       end
c





