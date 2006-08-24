      integer function idmin(n, x, incx)
*
*     PURPOSE
*        finds the index of the first element having minimum value
*        without taking into account the nan(s)
*
*     NOTE
*        - original version modified by Bruno (for the nan problem...)
*          (01/01/2003)
*        - this function return 1 if x has only nan components : may be
*          this is not a good behavior
*        - this function doesn't test if n<1 or incx<1 : this is done
*          by the scilab interface
*
      implicit none
      integer n, incx
      double precision x(incx,*)

      double precision xmin
      integer i, j
      external isanan
      integer  isanan

      idmin = 1

*     initialize the min with the first component being not a nan
      j = 1      
      do while ( isanan(x(1,j)) .eq. 1 )
         j = j + 1
         if ( j .gt. n ) return
      enddo
      xmin = x(1,j)
      idmin = j

*     the usual loop
      do i = j+1, n
         if ( x(1,i) .lt. xmin) then  ! a test with a nan must always return false
            xmin = x(1,i)
            idmin = i
         endif
      enddo
 
      end
