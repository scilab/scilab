      subroutine intex16f(fname)
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

