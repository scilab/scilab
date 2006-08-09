c			=======================================
c			INRIA
c			=======================================
      subroutine intsimp(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol,v2,blank
      double precision er
      integer simpmd
      common/csimp/ simpmd
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(simpmd.eq.0) then
         call ref2val()
         return
      endif
      if(rhs.eq.1) then
         call intrsimp(id)
      elseif(rhs.eq.2) then
         call intpsimp(id)
      else
         call error(39)
      endif
c
      return
      end
c			=======================================
