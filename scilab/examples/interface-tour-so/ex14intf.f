      subroutine intex14f(fname)
      character*(*) fname
C     --------------------------------------------
      include 'stack.h'
c
c   An example of an hand written interface 
c   passing a Scilab function as input of function ex14f

c    call in Scilab:-->ex14f(x1,x2,a_function)
c     x1<->1    (double array)
c     x2<->2    (double array) 
c     a_function <-> 3    (a scilab function).
c     a_function is the function "myfunction" defined 
c     in ex14f.sce. It has mlhs=2 inputs and mrhs=3 outputs.

      logical getrhsvar,createvar,scifunction
      logical checklhs,checkrhs
      common/  ierfeval / iero

      if(.not.checkrhs(fname,3,3)) return
      if(.not.checklhs(fname,1,3)) return
c
c     get adress of x1
      if (.not.getrhsvar(1,'d',m1,n1,l1))  return
c     get adress of x2
      if (.not.getrhsvar(2,'d',m2,n2,l2))  return
c     lf is the adress of a_function 
c     mlhs (resp. mrhs) is its number of outputs (resp. inputs)
c     3 and 'f' are inputs of getrhsvar
c     mlhs,mrhs,lf are outputs of getrhsvar

      if (.not.getrhsvar(3,'f',mlhs,mrhs,lf))  return

      if(mrhs.ne.2) then
         buf='invalid rhs for Scilab function'
         call error(998)
         return
      endif
c     To call a_function it is required that its input arguments are
c     stored in the last positions of the variables stack. NOTE that when 
c     called, the function destroys its input variables and replaces them by 
c     the output variables. so in this  case we need to make a copy of
c     them.
c     Remark: if the calling sequence of geval had been geval(a_function,x1,x2)
c     the following two copies would be un-necessary.

c     make a copy of x1
      if(.not.createvar(3+1,'d',m1,n1,l4)) return
      call dcopy(m1*n1,stk(l1),1,stk(l4),1)
c      ....
c     make a copy of x2
      if(.not.createvar(3+mrhs,'d',m2,n2,l5)) return
      call dcopy(m2*n2,stk(l2),1,stk(l5),1)
c
c     Here a_function  takes  variables 4 and 5 as inputs and generates output
c     variables at positions 4 to 4-1+mlhs

c     ibegin must be the index of the first input variable of a_function
      ibegin=3+1

c     execute a_function
      if(.not.scifunction(ibegin,lf,mlhs,mrhs)) return

c     check if an error has occured while running a_function
      if(err.gt.0) return

c     output variables: 4 and 5 (created by a_function) and possibly 6
c                       if a_function has 3 output parameters

c     select index of variables to return
      lhsvar(1)=4
      lhsvar(2)=5
      if(mlhs.eq.3) lhsvar(3)=6
      return
      end
       


