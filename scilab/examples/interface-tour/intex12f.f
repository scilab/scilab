      subroutine intex12f(fname)
      character*(*) fname
      include 'stack.h'
C     --------------------------------------------
      logical getrhsvar,scistring
      logical checklhs,checkrhs
      common/  ierfeval / iero

      if(.not.checkrhs(fname,2,2)) return
      if(.not.checklhs(fname,1,1)) return
c
c     this interface is called by the command: ex12f(x1,x2) 

c     get adress of x1 (multiplicative factor (scalar))
      if (.not.getrhsvar(1,'d',m1,n1,l1))  return
c     get adress of x2 (half roots of the polynomial)
      if (.not.getrhsvar(2,'d',m2,n2,l2))  return

c     multiply the roots by the scaling factor in place.
      call dscal(m2*n2,stk(l1),stk(l2),1)

c     Call mypoly function to create the polynomial from its roots

c     To call a function it is required that its input arguments are
c     stored in the last positions of the variables stack (it is the
c     ase here. NOTE that when 
c     called, the function destroys its input variables and replaces them by 
c     the output variables. 

c     Here  function  mypoly takes  variables 2  as input and generates output
c     variable at positions 2.

c     ibegin must be the index of the first input variable of mypoly 
      ibegin=2

c     execute the mypoly function
      mlhs=1
      mrhs=1
      if(.not.scistring(ibegin,'mypoly',mlhs,mrhs)) return

c     check if an error has occured while running mypoly
      if(err.gt.0) return
c
c     output variable: 

c     select index of variables to return
      lhsvar(1)=2
      return
      end
       


