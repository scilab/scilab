      subroutine putid(x,y)
c
c     store a name
c
c     Copyright INRIA
      include '../stack.h'
c
      integer x(nsiz),y(nsiz)
      do 10 i = 1, nsiz
   10 x(i) = y(i)
      return
      end
