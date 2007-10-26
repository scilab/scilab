      integer function idmax(n, x, incx)
*
*     PURPOSE
*        finds the index of element having maximum value
*        without taking into account the nan(s)...
*
*     NOTES
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

      double precision xmax
      integer i, j
      external isanan
      integer  isanan

      idmax = 1

*     initialize the max with the first component being not a nan
      j = 1      
      do while ( isanan(x(1,j)) .eq. 1 )
         j = j + 1
         if ( j .gt. n ) return
      enddo
      xmax = x(1,j)
      idmax = j

*     the usual loop
      do i = j+1, n
         if ( x(1,i) .gt. xmax) then  ! a test with a nan must always return false
            xmax = x(1,i)
            idmax = i
         endif
      enddo
 
      end
