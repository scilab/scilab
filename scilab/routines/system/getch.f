      subroutine fortrangetch
c
c     get next character
c
c
c     Copyright INRIA
      include '../stack.h'
c
      integer eol
      data eol/99/
c
      l = lpt(4)
      char1 = lin(l)
      if (char1 .ne. eol) lpt(4) = l + 1
      return
      end
