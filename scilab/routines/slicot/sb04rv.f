      SUBROUTINE SB04RV( ABSCHR, UL, N, M, C, LDC, INDX, AB, LDAB, BA, 
     $                   LDBA, D, DWORK )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To construct the right-hand sides D for a system of equations in
C     quasi-Hessenberg form solved via SB04RX (case with 2 right-hand
C     sides).
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     ABSCHR  CHARACTER*1
C             Indicates whether AB contains A or B, as follows:
C             = 'A':  AB contains A;
C             = 'B':  AB contains B.
C
C     UL      CHARACTER*1
C             Indicates whether AB is upper or lower Hessenberg matrix,
C             as follows:
C             = 'U':  AB is upper Hessenberg;
C             = 'L':  AB is lower Hessenberg.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrix A.  N >= 0.
C
C     M       (input) INTEGER
C             The order of the matrix B.  M >= 0.
C
C     C       (input) DOUBLE PRECISION array, dimension (LDC,M)
C             The leading N-by-M part of this array must contain both 
C             the not yet modified part of the coefficient matrix C of
C             the Sylvester equation X + AXB = C, and both the currently
C             computed part of the solution of the Sylvester equation.
C
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,N).
C
C     INDX    (input) INTEGER
C             The position of the first column/row of C to be used in
C             the construction of the right-hand side D.
C
C     AB      (input) DOUBLE PRECISION array, dimension (LDAB,*)
C             The leading N-by-N or M-by-M part of this array must
C             contain either A or B of the Sylvester equation
C             X + AXB = C.
C
C     LDAB    INTEGER
C             The leading dimension of array AB.
C             LDAB >= MAX(1,N) or LDAB >= MAX(1,M) (depending on
C             ABSCHR = 'A' or ABSCHR = 'B', respectively).
C
C     BA      (input) DOUBLE PRECISION array, dimension (LDBA,*)
C             The leading N-by-N or M-by-M part of this array must
C             contain either A or B of the Sylvester equation
C             X + AXB = C, the matrix not contained in AB.
C
C     LDBA    INTEGER
C             The leading dimension of array BA.
C             LDBA >= MAX(1,N) or LDBA >= MAX(1,M) (depending on
C             ABSCHR = 'B' or ABSCHR = 'A', respectively).
C
C     D       (output) DOUBLE PRECISION array, dimension (*)
C             The leading 2*N or 2*M part of this array (depending on
C             ABSCHR = 'B' or ABSCHR = 'A', respectively) contains the
C             right-hand side stored as a matrix with two rows.
C
C     Workspace
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             where LDWORK is equal to 2*N or 2*M (depending on
C             ABSCHR = 'B' or ABSCHR = 'A', respectively).
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTORS
C
C     D. Sima, University of Bucharest, May 2000.
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
C     .. Parameters ..
      DOUBLE PRECISION  ONE, ZERO
      PARAMETER         ( ONE = 1.0D0, ZERO = 0.0D0 )
C     .. Scalar Arguments ..
      CHARACTER         ABSCHR, UL
      INTEGER           INDX, LDAB, LDBA, LDC, M, N
C     .. Array Arguments ..
      DOUBLE PRECISION  AB(LDAB,*), BA(LDBA,*), C(LDC,*), D(*), DWORK(*)
C     .. External Functions ..
      LOGICAL           LSAME
      EXTERNAL          LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DGEMV
C     .. Executable Statements ..
C
C     For speed, no tests on the input scalar arguments are made.
C     Quick return if possible.
C
      IF ( N.EQ.0 .OR. M.EQ.0 )
     $   RETURN
C
      IF ( LSAME( ABSCHR, 'B' ) ) THEN
C
C        Construct the 2 columns of the right-hand side.
C
         CALL DCOPY( N, C(1,INDX), 1, D(1), 2 )
         CALL DCOPY( N, C(1,INDX+1), 1, D(2), 2 )
         IF ( LSAME( UL, 'U' ) ) THEN
            IF ( INDX.GT.1 ) THEN
               CALL DGEMV( 'N', N, INDX-1, ONE, C, LDC, AB(1,INDX), 1,
     $                     ZERO, DWORK, 1 )
               CALL DGEMV( 'N', N, INDX-1, ONE, C, LDC, AB(1,INDX+1),
     $                     1, ZERO, DWORK(N+1), 1 )
               CALL DGEMV( 'N', N, N, -ONE, BA, LDBA, DWORK, 1, ONE, 
     $                     D(1), 2 )
               CALL DGEMV( 'N', N, N, -ONE, BA, LDBA, DWORK(N+1), 1,
     $                     ONE, D(2), 2 )
            END IF
         ELSE
            IF ( INDX.LT.M-1 ) THEN
               CALL DGEMV( 'N', N, M-INDX-1, ONE, C(1,INDX+2), LDC,
     $                     AB(INDX+2,INDX), 1, ZERO, DWORK, 1 )
               CALL DGEMV( 'N', N, M-INDX-1, ONE, C(1,INDX+2), LDC,
     $                     AB(INDX+2,INDX+1), 1, ZERO, DWORK(N+1), 1 )
               CALL DGEMV( 'N', N, N, -ONE, BA, LDBA, DWORK, 1, ONE, 
     $                     D(1), 2 )
               CALL DGEMV( 'N', N, N, -ONE, BA, LDBA, DWORK(N+1), 1,
     $                     ONE, D(2), 2 )
            END IF
         END IF
      ELSE
C
C        Construct the 2 rows of the right-hand side.
C
         CALL DCOPY( M, C(INDX,1), LDC, D(1), 2 )
         CALL DCOPY( M, C(INDX+1,1), LDC, D(2), 2 )
         IF ( LSAME( UL, 'U' ) ) THEN
            IF ( INDX.LT.N-1 ) THEN
               CALL DGEMV( 'T', N-INDX-1, M, ONE, C(INDX+2,1), LDC,
     $                     AB(INDX,INDX+2), LDAB, ZERO, DWORK, 1 )
               CALL DGEMV( 'T', N-INDX-1, M, ONE, C(INDX+2,1), LDC,
     $                     AB(INDX+1,INDX+2), LDAB, ZERO, DWORK(M+1), 
     $                     1 )
               CALL DGEMV( 'T', M, M, -ONE, BA, LDBA, DWORK, 1, ONE,
     $                     D(1), 2 )
               CALL DGEMV( 'T', M, M, -ONE, BA, LDBA, DWORK(M+1), 1,
     $                     ONE, D(2), 2 )
            END IF
         ELSE
            IF ( INDX.GT.1 ) THEN
               CALL DGEMV( 'T', INDX-1, M, ONE, C, LDC, AB(INDX,1),
     $                     LDAB, ZERO, DWORK, 1 )
               CALL DGEMV( 'T', INDX-1, M, ONE, C, LDC, AB(INDX+1,1),
     $                     LDAB, ZERO, DWORK(M+1), 1 )
               CALL DGEMV( 'T', M, M, -ONE, BA, LDBA, DWORK, 1, ONE,
     $                     D(1), 2 )
               CALL DGEMV( 'T', M, M, -ONE, BA, LDBA, DWORK(M+1), 1,
     $                     ONE, D(2), 2 )
            END IF
         END IF
      END IF
C
      RETURN
C *** Last line of SB04RV ***
      END
