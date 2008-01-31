      subroutine skipvars(n)
c     decrease top by n . So n latest variables at the top of stack are
c     skipped 
c     Copyright INRIA
      include '../stack.h'
      integer n
c
      top=top-n
      return
      end
