c			INRIA
      subroutine msgout(io,lunit,str)
      character*(*) str
      integer io,lunit
c
      integer ierr
      integer msgstore
      external msgstore

c
      call basout(io,lunit,str)
      ierr=msgstore(str,len(str))
      return
      end
