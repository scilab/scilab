      subroutine defint
c     defined integrals computation
c     Copyright INRIA
      include '../stack.h'
c
      if(fin.eq.1) then
         call intg
      elseif(fin.eq.2) then
         call int2d
      elseif(fin.eq.3) then
         call int3d
      endif
      return
      end

