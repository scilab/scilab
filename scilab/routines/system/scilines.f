      subroutine scilines(nl,nc)
c     Copyright INRIA
      include '../stack.h'
      integer nl,nc
c     set  number of lines and columns 
      lct(5)=nc 
      if(lct(2).ne.0) lct(2) = max(0,nl)
      return
      end
