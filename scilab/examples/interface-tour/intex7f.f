      subroutine intex7f(fname)
      character*(*) fname
C     --------------------------------------------
      include 'stack.h'
      logical getrhsvar,scistring
      logical checklhs,checkrhs
      common/  ierfeval / iero

      if(.not.checkrhs(fname,2,2)) return
      if(.not.checklhs(fname,1,1)) return
c
c     get adress of x1
      if (.not.getrhsvar(1,'d',m1,n1,l1))  return
c     get adress of x2
      if (.not.getrhsvar(2,'d',m2,n2,l2))  return

c     To call a function it is required that its input arguments are
c     stored in the last positions of the variables stack (it is the
c     ase here. NOTE that when 
c     called, the function destroys its input variables and replaces them by 
c     the output variables. 

c     Here  function  takes  variables 1 and 2 as inputs and generates output
c     variables at positions 1.

c     ibegin must be the index of the first input variable of a_function
      ibegin=1

c     execute the disp function
      mlhs=1
      mrhs=2
      if(.not.scistring(ibegin,'disp',mlhs,mrhs)) return

c     check if an error has occured while running a_function
      if(err.gt.0) return

c     Note that disp, as every function which has nothing to return,
c     creates as output a variable with special type 0.

c     output variable: 

c     select index of variables to return
      lhsvar(1)=1
      return
      end
       

