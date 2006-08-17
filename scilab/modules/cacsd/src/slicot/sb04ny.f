      SUBROUTINE SB04NY( RC, UL, M, A, LDA, LAMBDA, D, TOL, IWORK,
     $                   DWORK, LDDWOR, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve a system of equations in Hessenberg form with one
C     offdiagonal and one right-hand side.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     RC      CHARACTER*1
C             Indicates processing by columns or rows, as follows:
C             = 'R':  Row transformations are applied;
C             = 'C':  Column transformations are applied.
C
C     UL      CHARACTER*1
C             Indicates whether AB is upper or lower Hessenberg matrix,
C             as follows:
C             = 'U':  AB is upper Hessenberg;
C             = 'L':  AB is lower Hessenberg.
C
C     Input/Output Parameters
C
C     M       (input) INTEGER
C             The order of the matrix A.  M >= 0.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,M)
C             The leading M-by-M part of this array must contain a
C             matrix A in Hessenberg form.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,M).
C
C     LAMBDA  (input) DOUBLE PRECISION
C             This variable must contain the value to be added to the 
C             diagonal elements of A.
C
C     D       (input/output) DOUBLE PRECISION array, dimension (M)
C             On entry, this array must contain the right-hand side
C             vector of the Hessenberg system.
C             On exit, if INFO = 0, this array contains the solution
C             vector of the Hessenberg system.
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             The tolerance to be used to test for near singularity of
C             the triangular factor R of the Hessenberg matrix. A matrix
C             whose estimated condition number is less than 1/TOL is
C             considered to be nonsingular.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (M)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDDWOR,M+3)
C             The leading M-by-M part of this array is used for
C             computing the triangular factor of the QR decomposition 
C             of the Hessenberg matrix. The remaining 3*M elements are
C             used as workspace for the computation of the reciprocal
C             condition estimate.
C
C     LDDWOR  INTEGER
C             The leading dimension of array DWORK.  LDDWOR >= MAX(1,M).
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             = 1:  if the Hessenberg matrix is (numerically) singular.
C                   That is, its estimated reciprocal condition number
C                   is less than or equal to TOL.
C
C     NUMERICAL ASPECTS
C
C     None.
C
C     CONTRIBUTORS
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Aug. 1997.
C     Supersedes Release 2.0 routine SB04BY by M. Vanbegin, and
C     P. Van Dooren, Philips Research Laboratory, Brussels, Belgium.
C
C     REVISIONS
C
C     -
C
C     Note that RC, UL, M and LDA must be such that the value of the
C     LOGICAL variable OK in the following statement is true.
C
C      OK = ( ( UL.EQ.'U' ) .OR. ( UL.EQ.'u' ) .OR.
C             ( UL.EQ.'L' ) .OR. ( UL.EQ.'l' ) )
C           .AND.
C           ( ( RC.EQ.'R' ) .OR. ( RC.EQ.'r' ) .OR.
C             ( RC.EQ.'C' ) .OR. ( RC.EQ.'c' ) )
C           .AND.
C           ( M.GE.0 )
C           .AND.
C           ( LDA.GE.MAX( 1, M ) )
C           .AND.
C           ( LDDWOR.GE.MAX( 1, M ) )
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
      CHARACTER         RC, UL
      INTEGER           INFO, LDA, LDDWOR, M
      DOUBLE PRECISION  LAMBDA, TOL
C     .. Array Arguments ..
      INTEGER           IWORK(*)
      DOUBLE PRECISION  A(LDA,*), D(*), DWORK(LDDWOR,*)
C     .. Local Scalars ..
      CHARACTER         TRANS
      INTEGER           J, J1, MJ
      DOUBLE PRECISION  C, R, RCOND, S
C     .. External Functions ..
      LOGICAL           LSAME
      EXTERNAL          LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DLARTG, DROT, DTRCON, DTRSV
C     .. Intrinsic Functions ..
      INTRINSIC         ABS, MAX, MIN
C     .. Executable Statements ..
C
      INFO = 0
C
C     For speed, no tests on the input scalar arguments are made.
C     Quick return if possible.
C
      IF ( M.EQ.0 )
     $   RETURN
C
      IF ( LSAME( UL, 'U' ) ) THEN
C
         DO 20 J = 1, M
            CALL DCOPY( MIN( J+1, M ), A(1,J), 1, DWORK(1,J), 1 )
            DWORK(J,J) = DWORK(J,J) + LAMBDA
   20    CONTINUE
C
         IF ( LSAME( RC, 'R' ) ) THEN
            TRANS = 'N'
C
C           A is an upper Hessenberg matrix, row transformations.
C
            DO 40 J = 1, M - 1
               MJ = M - J
               IF ( DWORK(J+1,J).NE.ZERO ) THEN
                  CALL DLARTG( DWORK(J,J), DWORK(J+1,J), C, S, R )
                  DWORK(J,J)   = R
                  DWORK(J+1,J) = ZERO
                  CALL DROT( MJ, DWORK(J,J+1), LDDWOR, DWORK(J+1,J+1),
     $                       LDDWOR, C, S )
                  CALL DROT( 1, D(J), 1, D(J+1), 1, C, S )
               END IF
   40       CONTINUE
C
         ELSE
            TRANS = 'T'
C
C           A is an upper Hessenberg matrix, column transformations.
C
            DO 60 J = 1, M - 1
               MJ = M - J
               IF ( DWORK(MJ+1,MJ).NE.ZERO ) THEN
                  CALL DLARTG( DWORK(MJ+1,MJ+1), DWORK(MJ+1,MJ), C, S,
     $                         R )
                  DWORK(MJ+1,MJ+1) = R
                  DWORK(MJ+1,MJ)   = ZERO
                  CALL DROT( MJ, DWORK(1,MJ+1), 1, DWORK(1,MJ), 1, C,
     $                       S )
                  CALL DROT( 1, D(MJ+1), 1, D(MJ), 1, C, S )
               END IF
   60       CONTINUE
C
         END IF
      ELSE
C
         DO 80 J = 1, M
            J1 = MAX( J - 1, 1 )
            CALL DCOPY( M-J1+1, A(J1,J), 1, DWORK(J1,J), 1 )
            DWORK(J,J) = DWORK(J,J) + LAMBDA
   80    CONTINUE
C
         IF ( LSAME( RC, 'R' ) ) THEN
            TRANS = 'N'
C
C           A is a lower Hessenberg matrix, row transformations.
C
            DO 100 J = 1, M - 1
               MJ = M - J
               IF ( DWORK(MJ,MJ+1).NE.ZERO ) THEN
                  CALL DLARTG( DWORK(MJ+1,MJ+1), DWORK(MJ,MJ+1), C, S,
     $                         R )
                  DWORK(MJ+1,MJ+1) = R
                  DWORK(MJ,MJ+1)   = ZERO
                  CALL DROT( MJ, DWORK(MJ+1,1), LDDWOR, DWORK(MJ,1),
     $                       LDDWOR, C, S )
                  CALL DROT( 1, D(MJ+1), 1, D(MJ), 1, C, S )
               END IF
  100       CONTINUE
C
         ELSE
            TRANS = 'T'
C
C           A is a lower Hessenberg matrix, column transformations.
C
            DO 120 J = 1, M - 1
               MJ = M - J
               IF ( DWORK(J,J+1).NE.ZERO ) THEN
                  CALL DLARTG( DWORK(J,J), DWORK(J,J+1), C, S, R )
                  DWORK(J,J)   = R
                  DWORK(J,J+1) = ZERO
                  CALL DROT( MJ, DWORK(J+1,J), 1, DWORK(J+1,J+1), 1, C,
     $                       S )
C
                  CALL DROT( 1, D(J), 1, D(J+1), 1, C, S )
               END IF
  120       CONTINUE
C
         END IF
      END IF
C
      CALL DTRCON( '1-norm', UL, 'Non-unit', M, DWORK, LDDWOR, RCOND,
     $             DWORK(1,M+1), IWORK, INFO )
      IF ( RCOND.LE.TOL ) THEN
         INFO = 1
      ELSE
         CALL DTRSV( UL, TRANS, 'Non-unit', M, DWORK, LDDWOR, D, 1 )
      END IF
C
      RETURN
C *** Last line of SB04NY ***
      END
