      SUBROUTINE SB04MW( M, D, IPR, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve a linear algebraic system of order M whose coefficient
C     matrix is in upper Hessenberg form, stored compactly, row-wise. 
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The order of the system.  M >= 0.
C
C     D       (input/output) DOUBLE PRECISION array, dimension 
C             (M*(M+1)/2+2*M)
C             On entry, the first M*(M+1)/2 + M elements of this array
C             must contain an upper Hessenberg matrix, stored compactly,
C             row-wise, and the next M elements must contain the right
C             hand side of the linear system, as set by SLICOT Library
C             routine SB04MY.
C             On exit, the content of this array is updated, the last M
C             elements containing the solution with components 
C             interchanged (see IPR). 
C
C     IPR     (output) INTEGER array, dimension (2*M)
C             The leading M elements contain information about the
C             row interchanges performed for solving the system.
C             Specifically, the i-th component of the solution is
C             specified by IPR(i).
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             = 1:  if a singular matrix was encountered.
C
C     METHOD
C
C     Gaussian elimination with partial pivoting is used. The rows of
C     the matrix are not actually permuted, only their indices are
C     interchanged in array IPR.
C
C     REFERENCES
C
C     [1] Golub, G.H., Nash, S. and Van Loan, C.F.
C         A Hessenberg-Schur method for the problem AX + XB = C.
C         IEEE Trans. Auto. Contr., AC-24, pp. 909-913, 1979.
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTORS
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Sep. 1997.
C     Supersedes Release 2.0 routine SB04AW by G. Golub, S. Nash, and
C     C. Van Loan, Stanford University, California, United States of
C     America, January 1982.
C
C     REVISIONS
C
C     -
C
C     KEYWORDS
C
C     Hessenberg form, orthogonal transformation, real Schur form,
C     Sylvester equation.
C
C     ******************************************************************
C
      DOUBLE PRECISION  ZERO
      PARAMETER         ( ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      INTEGER           INFO, M
C     .. Array Arguments ..
      INTEGER           IPR(*)
      DOUBLE PRECISION  D(*)
C     .. Local Scalars ..
      INTEGER           I, I1, IPRM, IPRM1, K, M1, M2, MPI
      DOUBLE PRECISION  D1, D2, MULT
C     .. External Subroutines ..
      EXTERNAL          DAXPY
C     .. Intrinsic Functions ..
      INTRINSIC         ABS
C     .. Executable Statements ..
C
      INFO = 0
      M1   = ( M*( M + 3 ) )/2
      M2   = M + M
      MPI  = M
      IPRM = M1
      M1   = M
      I1   = 1
C
      DO 20 I = 1, M
         MPI = MPI + 1
         IPRM = IPRM + 1
         IPR(MPI) = I1
         IPR(I) = IPRM
         I1 = I1 + M1
         IF ( I.GT.1 ) M1 = M1 - 1
   20 CONTINUE
C
      M1  = M - 1
      MPI = M 
C
C     Reduce to upper triangular form.
C
      DO 40 I = 1, M1
         I1  = I + 1
         MPI = MPI + 1
         IPRM  = IPR(MPI)
         IPRM1 = IPR(MPI+1)
         D1 = D(IPRM)
         D2 = D(IPRM1)
         IF ( ABS( D1 ).LE.ABS( D2 ) ) THEN
C
C           Permute the row indices.
C
            K = IPRM
            IPR(MPI) = IPRM1
            IPRM = IPRM1
            IPRM1 = K
            K = IPR(I)
            IPR(I) = IPR(I1)
            IPR(I1) = K
            D1 = D2
         END IF
C
C        Check singularity.
C
         IF ( D1.EQ.ZERO ) THEN
            INFO = 1
            RETURN
         END IF
C
         MULT  = -D(IPRM1)/D1
         IPRM1 = IPRM1 + 1
         IPR(MPI+1) = IPRM1
C 
C        Annihilate the subdiagonal elements of the matrix.
C
         D(IPR(I1)) = D(IPR(I1)) + MULT*D(IPR(I))
         CALL DAXPY( M-I, MULT, D(IPRM+1), 1, D(IPRM1), 1 )
   40 CONTINUE
C
C     Check singularity.
C
      IF ( D(IPR(M2)).EQ.ZERO ) THEN
         INFO = 1
         RETURN
      END IF
C
C     Back substitution.
C
      D(IPR(M)) = D(IPR(M))/D(IPR(M2))
      MPI = M2
C
      DO 80 I = M1, 1, -1
         MPI  = MPI - 1
         IPRM = IPR(MPI)
         IPRM1 = IPRM
         MULT = ZERO
C
         DO 60 I1 = I + 1, M
            IPRM1 = IPRM1 + 1
            MULT = MULT + D(IPR(I1))*D(IPRM1)
   60    CONTINUE
C
         D(IPR(I)) = ( D(IPR(I)) - MULT )/D(IPRM)
   80 CONTINUE
C
      RETURN
C *** Last line of SB04MW ***
      END
