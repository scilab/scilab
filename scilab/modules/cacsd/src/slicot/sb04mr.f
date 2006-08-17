      SUBROUTINE SB04MR( M, D, IPR, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve a linear algebraic system of order M whose coefficient
C     matrix has zeros below the second subdiagonal. The matrix is 
C     stored compactly, row-wise. 
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The order of the system.  M >= 0.
C             Note that parameter M should have twice the value in the
C             original problem (see SLICOT Library routine SB04MU).
C
C     D       (input/output) DOUBLE PRECISION array, dimension 
C             (M*(M+1)/2+3*M)
C             On entry, the first M*(M+1)/2 + 2*M elements of this array
C             must contain the coefficient matrix, stored compactly,
C             row-wise, and the next M elements must contain the right
C             hand side of the linear system, as set by SLICOT Library
C             routine SB04MU.
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
C     Supersedes Release 2.0 routine SB04AR by G. Golub, S. Nash, and
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
      INTEGER           I, I1, I2, IPRM, IPRM1, J, K, L, M1, MPI, MPI1,
     $                  MPI2
      DOUBLE PRECISION  D1, D2, D3, DMAX
C     .. External Subroutines ..
      EXTERNAL          DAXPY
C     .. Intrinsic Functions ..
      INTRINSIC         ABS
C     .. Executable Statements ..
C
      INFO = 0
      I2   = ( M*( M + 5 ) )/2
      MPI  = M
      IPRM = I2
      M1 = M
      I1 = 1
C
      DO 20 I = 1, M
         MPI = MPI + 1
         IPRM = IPRM + 1
         IPR(MPI) = I1
         IPR(I) = IPRM
         I1 = I1 + M1
         IF ( I.GE.3 ) M1 = M1 - 1 
   20 CONTINUE
C
      M1   = M - 1
      MPI1 = M + 1 
C
C     Reduce to upper triangular form.
C
      DO 80 I = 1, M1
         MPI  = MPI1
         MPI1 = MPI1 + 1
         IPRM = IPR(MPI)
         D1 = D(IPRM)
         I1 = 2
         IF ( I.EQ.M1 ) I1 = 1
         MPI2 = MPI + I1
         L = 0
         DMAX = ABS( D1 )
C
         DO 40 J = MPI1, MPI2
            D2 = D(IPR(J))
            D3 = ABS( D2 )
            IF ( D3.GT.DMAX ) THEN
               DMAX = D3
               D1 = D2
               L = J - MPI
            END IF 
   40    CONTINUE
C
C        Check singularity.
C
         IF ( DMAX.EQ.ZERO ) THEN
            INFO = 1
            RETURN
         END IF
C
         IF ( L.GT.0 ) THEN
C
C           Permute the row indices.
C
            K = IPRM
            J = MPI + L
            IPRM   = IPR(J)
            IPR(J) = K
            IPR(MPI) = IPRM
            K  = IPR(I)
            I2 = I + L
            IPR(I)  = IPR(I2)
            IPR(I2) = K
         END IF
         IPRM = IPRM + 1
C 
C        Annihilate the subdiagonal elements of the matrix.
C
         I2 = I
         D3 = D(IPR(I))
C
         DO 60 J = MPI1, MPI2
            I2 = I2 + 1
            IPRM1 = IPR(J)
            DMAX = -D(IPRM1)/D1
            D(IPR(I2)) = D(IPR(I2)) + DMAX*D3
            CALL DAXPY( M-I, DMAX, D(IPRM), 1, D(IPRM1+1), 1 )
   60    CONTINUE
C
         IPR(MPI1) = IPR(MPI1) + 1
         IF ( I.NE.M1 ) IPR(MPI2) = IPR(MPI2) + 1
   80 CONTINUE
C
      MPI  = M + M
      IPRM = IPR(MPI)
C
C     Check singularity.
C
      IF ( D(IPRM).EQ.ZERO ) THEN
         INFO = 1
         RETURN
      END IF
C
C     Back substitution.
C
      D(IPR(M)) = D(IPR(M))/D(IPRM)
C
      DO 120 I = M1, 1, -1
         MPI   = MPI - 1
         IPRM  = IPR(MPI)
         IPRM1 = IPRM
         DMAX  = ZERO
C
         DO 100 K = I+1, M
            IPRM1 = IPRM1 + 1
            DMAX  = DMAX + D(IPR(K))*D(IPRM1)
  100    CONTINUE
C
         D(IPR(I)) = ( D(IPR(I)) - DMAX )/D(IPRM)
  120 CONTINUE
C
      RETURN
C *** Last line of SB04MR ***
      END
