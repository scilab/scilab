      subroutine intex15f(fname)
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
