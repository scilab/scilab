      subroutine   spind(i,xadj,nb,nza)
c converts adjacency into sparse matrix
      integer*4 i(*),xadj(*)
      integer*4 nb,ii,io,nza,k
      io=0
      do 100 k=1,nb
      ii=xadj(k+1)-xadj(k)
      do 200 j=1,ii
      io = io + 1
      i(io) = k
 200  continue
 100  continue
      return
      end
