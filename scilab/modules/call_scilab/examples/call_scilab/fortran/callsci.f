      program callsci
c!Purpose
c     Example of calling scilab from an other C or Fortran program
c!Description
c     This program demonstrate on a simple example to use scilab as a
c     procedure for an other program
c     The computations performed are:
c      -1-  computation of x such that a*x=b  (a and b real matrices)
c      -2-  computation of y=a*x-b
c     All scilab functions and instructions may be used in this context
c     for complex instruction, it may be usefull to defined it in a file
c     and use exec
c!
c     Copyright INRIA
c
      dimension a(2,2),b(2),x(2),y(2)
      double precision a,b,x,y
c
c     a and b definition
c
      a(1,1)=1.d0
      a(2,1)=2.d0
      a(1,2)=3.d0
      a(2,2)=4.d0
      b(1)=1.d0
      b(2)=0.d0

c     create temp directory
      call settmpdir

c
c     call to inisci for Scilab initializations
c     10000 is the initial stack size
c
      call inisci(-1,10000, ierr)
      if(ierr.gt.0) stop
c
c     scilab start-up execution. quit added to leave parser after
c     start-up execution
c
      call scirun('exec(''SCI/etc/scilab.start'',-1);quit')
c
c     Program's variables  a and b sent to Scilab.
c     .  2nd argument of matz not referenced here.
c     .  3rd argument is the first dimension of the transmitted
c     .  variable (here dimension a(2,.) )
c     .  arguments #3 and 4 represent the number of rows and columns
c     .  respectively
c     .  argument #5 is a character string name of the scilab variable
c     .  argument #6 (here job=1) means direction fortran--->scilab
c
      call matz(a,a,2,2,2,'a',1)
      call matz(b,b,2,2,1,'b',1)
c
c     call scirun to solve the problem
c     . \\ needed to enter a \ in fortran string
c
      call scirun('x=a\\b;quit')
c
c     Scilab variable x get in program and displayed
c     .  the numbers m and n (dimensions of x) are here given
c     .  by scilab .Don't give numerical values to arguments
c     .  4 and 5 of matz here.
c     .  last argument (job=0) means:   scilab --->program

      call matz(x,x,2,m,n,'x',0)
      write(6,100) x(1),x(2)
 100  format('x = [',2x,f10.5,2x,f10.5,']')
c
c     call scilab to check the result
c     .  note  -1- a,x,b are still in the scilab stack
c     .        -2- this computation might be done in the first call to scirun
      call scirun('y=a*x-b;quit')
c
c     Scilab variable y get in program and displayed
c
      call matz(y,y,2,m,n,'y',0)
      write(6,110) y(1),y(2)
110   format('a*x-b = [',2x,f10.5,2x,f10.5,']')
c
c     stop properly
c
c     call sciquit
      stop
      end
