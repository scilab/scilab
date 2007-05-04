      subroutine vvtosci(x,nx)
c     fortran var2vec  to scilab
c     x is supposed to be a fortran image of var2vec result
c 
c     Copyright INRIA
      double precision x(*)
c 
      integer  l
      integer inull(4)
c 
      external scidcopy, error
      include "../stack.h"
      data inull/1,1,0,0/
c 
      if (top .ge. bot) then
        call error(18)
      else
        top = top + 1
        l=lstk(top)

       if (nx.eq.0) then
        err = l + 4 - lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
       endif
          call scidcopy(2,inull,1,stk(l),1)
          lstk(top+1) = l + 4
       else
        err = l + nx - lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
       endif
          call scidcopy(nx,x,1,stk(l),1)
          lstk(top+1) = l + nx
       endif
      endif
      end 
