      SUBROUTINE genprm(array,larray)
C**********************************************************************
C
C    SUBROUTINE GENPRM( ARRAY, LARRAY )
C               GENerate random PeRMutation of array
C
C
C                              Arguments
C
C
C     ARRAY <--> On output ARRAY is a random permutation of its
C                 value on input
C                         DOUBLE PRECISION ARRAY( LARRAY )
C
C     LARRAY <--> Length of ARRAY
C                         INTEGER LARRAY
C
C     Modification by Bruno to handle permutation of a double precision 
C     array (15/11/2001) and to handle the changes in ignuin
C
C**********************************************************************
      implicit none
C     .. Scalar Arguments ..
      INTEGER larray
C     ..
C     .. Array Arguments ..
      DOUBLE PRECISION array(larray)
C     ..
C     .. Local Scalars ..
      INTEGER i, iwhich
      DOUBLE PRECISION elt, llarray
C     ..
C     .. External Functions ..
      DOUBLE PRECISION ignuin
      EXTERNAL ignuin
C     ..
C     .. Executable Statements ..
      llarray = dble(larray)
      DO i = 1,larray
         iwhich = int(ignuin(dble(i),llarray))
         elt = array(iwhich)
         array(iwhich) = array(i)
         array(i) = elt
      END DO

      END
