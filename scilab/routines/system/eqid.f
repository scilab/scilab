      logical function eqid(x,y)
c
c     check for equality of two names
c
c     Copyright INRIA
      include '../stack.h'
c
      integer x(nsiz),y(nsiz)
c
      eqid = .false.
      do 10 i = 1, nsiz
   10 if(x(i).ne.y(i)) return
      eqid=.true.
      return
      end
