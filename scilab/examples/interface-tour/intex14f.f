      subroutine intex14f(fname)
      include 'stack.h'
c  -------------------------------
c
c   Accessing the Scilab Stack :
c   Creation of a Scilab Matrix "C"
c   from a name and an array of data
c
      character*(*) fname
      logical cwritemat
      logical checklhs,checkrhs

      double precision C(3)

      minlhs=1
      maxlhs=1
      minrhs=0
      maxrhs=0

      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      nrows=1
      ncols=3
      C(1)=10.0d0
      C(2)=20.0d0
      C(3)=30.0d0

c  Sending array C to Scilab as variable "C" (size [1,3]) 
      if(.not.cwritemat("C"//char(0), nrows, ncols, C)) return

c  No output
      lhsvar(1) = 0
      return
      end


