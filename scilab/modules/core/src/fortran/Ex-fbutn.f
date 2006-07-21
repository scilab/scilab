C     Examples for fbutn 
c     Copyright INRIA
      subroutine haltscicos(name,win,entry)
      integer win,entry
      character*(*) name
      integer halt
      common /coshlt/ halt
      halt=1
      return
      end
