       subroutine intex9f1(fname)
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

      subroutine intex9f2(fname)
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


      subroutine intex9f3(fname)
      include 'stack.h'
c ------------------------------------
      character*(*) fname
      logical creadchain
      logical checklhs,checkrhs
      character*(30) str
      character*(40) message
      integer strl
      strl=30

      minlhs=1
      minrhs=0
      maxlhs=1
      maxrhs=0

      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return
c   We search a Scilab Object named Mystr, check that 
c   it is a string and store the string in str. 
c    strl is used on entry to give the maximum number 
c    of characters which can be stored in str 
c   After the call strl contains the number of 
c    copied characters
      if(.not.creadchain("Mystr"//char(0), strl, str)) return
c
      message="Mystr="//str(1:strl)//"  Its length is ..."
      call basout(io, wte, message)
c     No output
      lhsvar(1) = 0
      return
      end

      subroutine intex9f4(fname)
      include 'stack.h'
c ------------------------------------
      character*(*) fname
      logical checklhs, checkrhs, cwritechain
      character*(54) str
      integer strl
c
c   Creation of a Scilab variable Str of type string
c   from a name and an a Fortran string 

      minrhs=0
      maxrhs=0
      minlhs=1
      maxlhs=1
c
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      str='Pilgrim said he, where can it be this land of Eldorado'
      strl=len(str)
      if(.not.cwritechain('Str', strl, str)) return

c   No output
      lhsvar(1)=0
      return
      end






