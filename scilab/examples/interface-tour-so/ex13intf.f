      subroutine intex13f1(fname)
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
       

      subroutine intex13f2(fname)
      include 'stack.h'
c     -----------------------------------
c     Executing the Scilab function "myfct" defined in ex11f.sce
      character*(*) fname
      logical getrhsvar, scistring
      logical checklhs,checkrhs
c
       minrhs=6
       maxrhs=6
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
c
      if(.not.getrhsvar(1,'d',m1,n1,l1)) return
      if(.not.getrhsvar(2,'d',m2,n2,l2)) return
      if(.not.getrhsvar(3,'d',m2,n2,l2)) return
      if(.not.getrhsvar(4,'d',m2,n2,l2)) return
      if(.not.getrhsvar(5,'d',m2,n2,l2)) return
      if(.not.getrhsvar(6,'d',m2,n2,l2)) return

c     We receive 6 input variables indexed by (1,2,...6)
c     when the command ex11f(x1,x2,x3,x4,x5,x6) is issued.
 
c     We have a Scilab function "myfct" with mrhs=2 inputs 
c     and mlhs=3 outputs:

c     function [u,v,w]=myfct(x,y)','u=7+x,v=8+y,w=9+y')
c     To run myfct with input variables x5 and x6,
c     we must set ifirst=5. Variables passed to the function must
c     appear consecutively with index ifirst, ifirst+1,..., ifirst+mrhs.
      mlhs=3
      mrhs=2
      ifirst=5
c     Variables #5 (x5) and #6 (x6) are the two inputs (x,y above) of "myfct",
c     i.e. myfct(x5,x6) is executed now:
      if(.not.scistring(ifirst,'myfct',mlhs,mrhs)) return

c     Output variables u, v, and w of myfct 
c     are now indexed by ifirst, ifirst+1, ifirst+mlhs i.e.
c     u and v are indexed by 5 and 6 resp. and w (created by myfct)
c     is indexed by 7.
c     We return u v and w:
c     Caution: Variables with index larger than ifirst+mrhs cannot be 
c     returned to Scilab.

       lhsvar(1)=5
       lhsvar(2)=6
       lhsvar(3)=7
c
       end


      subroutine intex13f3(fname)
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

