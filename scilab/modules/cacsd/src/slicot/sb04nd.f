      SUBROUTINE SB04ND( ABSCHU, ULA, ULB, N, M, A, LDA, B, LDB, C,
     $                   LDC, TOL, IWORK, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To solve for X the continuous-time Sylvester equation
C
C        AX + XB = C,
C
C     with at least one of the matrices A or B in Schur form and the
C     other in Hessenberg or Schur form (both either upper or lower);
C     A, B, C and X are N-by-N, M-by-M, N-by-M, and N-by-M matrices,
C     respectively.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     ABSCHU  CHARACTER*1
C             Indicates whether A and/or B is/are in Schur or
C             Hessenberg form as follows:
C             = 'A':  A is in Schur form, B is in Hessenberg form;
C             = 'B':  B is in Schur form, A is in Hessenberg form;
C             = 'S':  Both A and B are in Schur form.
C
C     ULA     CHARACTER*1
C             Indicates whether A is in upper or lower Schur form or
C             upper or lower Hessenberg form as follows:
C             = 'U':  A is in upper Hessenberg form if ABSCHU = 'B' and
C                     upper Schur form otherwise;
C             = 'L':  A is in lower Hessenberg form if ABSCHU = 'B' and
C                     lower Schur form otherwise.
C
C     ULB     CHARACTER*1
C             Indicates whether B is in upper or lower Schur form or
C             upper or lower Hessenberg form as follows:
C             = 'U':  B is in upper Hessenberg form if ABSCHU = 'A' and
C                     upper Schur form otherwise;
C             = 'L':  B is in lower Hessenberg form if ABSCHU = 'A' and
C                     lower Schur form otherwise.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrix A.  N >= 0.
C
C     M       (input) INTEGER
C             The order of the matrix B.  M >= 0.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             The leading N-by-N part of this array must contain the
C             coefficient matrix A of the equation.
C
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,N).
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,M)
C             The leading M-by-M part of this array must contain the
C             coefficient matrix B of the equation.
C
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,M).
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,M)
C             On entry, the leading N-by-M part of this array must
C             contain the coefficient matrix C of the equation.
C             On exit, if INFO = 0, the leading N-by-M part of this
C             array contains the solution matrix X of the problem.
C
C     LDC     INTEGER
C             The leading dimension of array C.  LDC >= MAX(1,N).
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             The tolerance to be used to test for near singularity in
C             the Sylvester equation. If the user sets TOL > 0, then the
C             given value of TOL is used as a lower bound for the
C             reciprocal condition number; a matrix whose estimated
C             condition number is less than 1/TOL is considered to be
C             nonsingular. If the user sets TOL <= 0, then a default
C             tolerance, defined by TOLDEF = EPS, is used instead, where
C             EPS is the machine precision (see LAPACK Library routine
C             DLAMCH).
C             This parameter is not referenced if ABSCHU = 'S', 
C             ULA = 'U', and ULB = 'U'.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (2*MAX(M,N))
C             This parameter is not referenced if ABSCHU = 'S', 
C             ULA = 'U', and ULB = 'U'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             This parameter is not referenced if ABSCHU = 'S', 
C             ULA = 'U', and ULB = 'U'.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK = 0, if ABSCHU = 'S', ULA = 'U', and ULB = 'U';
C             LDWORK = 2*MAX(M,N)*(4 + 2*MAX(M,N)), otherwise.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  if a (numerically) singular matrix T was encountered
C                   during the computation of the solution matrix X.
C                   That is, the estimated reciprocal condition number
C                   of T is less than or equal to TOL.
C
C     METHOD
C
C     Matrices A and B are assumed to be in (upper or lower) Hessenberg
C     or Schur form (with at least one of them in Schur form). The
C     solution matrix X is then computed by rows or columns via the back
C     substitution scheme proposed by Golub, Nash and Van Loan (see
C     [1]), which involves the solution of triangular systems of
C     equations that are constructed recursively and which may be nearly
C     singular if A and -B have close eigenvalues. If near singularity
C     is detected, then the routine returns with the Error Indicator
C     (INFO) set to 1.
C
C     REFERENCES
C
C     [1] Golub, G.H., Nash, S. and Van Loan, C.F.
C         A Hessenberg-Schur method for the problem AX + XB = C.
C         IEEE Trans. Auto. Contr., AC-24, pp. 909-913, 1979.
C
C     NUMERICAL ASPECTS
C                                            2         2
C     The algorithm requires approximately 5M N + 0.5MN  operations in
C                            2         2
C     the worst case and 2.5M N + 0.5MN  operations in the best case
C     (where M is the order of the matrix in Hessenberg form and N is
C     the order of the matrix in Schur form) and is mixed stable (see
C     [1]).
C
C     CONTRIBUTORS
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Aug. 1997.
C     Supersedes Release 2.0 routine SB04BD by M. Vanbegin, and
C     P. Van Dooren, Philips Research Laboratory, Brussels, Belgium.
C
C     REVISIONS
C
C     V. Sima, Research Institute for Informatics, Bucharest, Apr. 2000.
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
      CHARACTER         ABSCHU, ULA, ULB
      INTEGER           INFO, LDA, LDB, LDC, LDWORK, M, N
      DOUBLE PRECISION  TOL
C     .. Array Arguments ..
      INTEGER           IWORK(*)
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), C(LDC,*), DWORK(*)
C     .. Local Scalars ..
      CHARACTER         ABSCHR
      LOGICAL           LABSCB, LABSCS, LULA, LULB
      INTEGER           FWD, I, IBEG, IEND, INCR, IPINCR, ISTEP, JWORK,
     $                  LDW, MAXMN
      DOUBLE PRECISION  SCALE, TOL1
C     .. External Functions ..
      LOGICAL           LSAME
      DOUBLE PRECISION  DLAMCH
      EXTERNAL          DLAMCH, LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DTRSYL, SB04NV, SB04NW, SB04NX, SB04NY, 
     $                  XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         MAX
C     .. Executable Statements ..
C
      INFO = 0
      MAXMN = MAX( M, N )
      LABSCB = LSAME( ABSCHU, 'B' )
      LABSCS = LSAME( ABSCHU, 'S' )
      LULA   = LSAME( ULA, 'U' )
      LULB   = LSAME( ULB, 'U' )
C
C     Test the input scalar arguments.
C
      IF( .NOT.LABSCB .AND. .NOT.LABSCS .AND.
     $    .NOT.LSAME( ABSCHU, 'A' ) ) THEN
         INFO = -1
      ELSE IF( .NOT.LULA .AND. .NOT.LSAME( ULA, 'L' ) ) THEN
         INFO = -2
      ELSE IF( .NOT.LULB .AND. .NOT.LSAME( ULB, 'L' ) ) THEN
         INFO = -3
      ELSE IF( N.LT.0 ) THEN
         INFO = -4
      ELSE IF( M.LT.0 ) THEN
         INFO = -5
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( LDB.LT.MAX( 1, M ) ) THEN
         INFO = -9
      ELSE IF( LDC.LT.MAX( 1, N ) ) THEN
         INFO = -11
      ELSE IF( LDWORK.LT.0 .OR. ( .NOT.( LABSCS .AND. LULA .AND. LULB )
     $   .AND. LDWORK.LT.2*MAXMN*( 4 + 2*MAXMN ) ) ) THEN
         INFO = -15
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'SB04ND', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF ( MAXMN.EQ.0 )
     $   RETURN
C
      IF ( LABSCS .AND. LULA .AND. LULB ) THEN
C
C        If both matrices are in a real Schur form, use DTRSYL.
C
         CALL DTRSYL( 'NoTranspose', 'NoTranspose', 1, N, M, A, LDA, B,
     $                LDB, C, LDC, SCALE, INFO )
         IF ( SCALE.NE.ONE ) 
     $      INFO = 1 
         RETURN
      END IF
C
      LDW   = 2*MAXMN
      JWORK = LDW*LDW + 3*LDW + 1
      TOL1 = TOL
      IF ( TOL1.LE.ZERO )
     $   TOL1 = DLAMCH( 'Epsilon' )
C
C     Choose the smallest of both matrices as the one in Hessenberg
C     form when possible.
C
      ABSCHR = ABSCHU
      IF ( LABSCS ) THEN
         IF ( N.GT.M ) THEN
            ABSCHR = 'A'
         ELSE
            ABSCHR = 'B'
         END IF
      END IF
      IF ( LSAME( ABSCHR, 'B' ) ) THEN
C
C        B is in Schur form: recursion on the columns of B.
C
         IF ( LULB ) THEN
C
C           B is upper: forward recursion.
C
            IBEG = 1
            IEND = M
            FWD  = 1
            INCR = 0
         ELSE
C
C           B is lower: backward recursion.
C
            IBEG = M
            IEND = 1
            FWD  = -1
            INCR = -1
         END IF
         I = IBEG
C        WHILE ( ( IEND - I ) * FWD .GE. 0 ) DO
   20    IF ( ( IEND - I )*FWD.GE.0 ) THEN
C
C           Test for 1-by-1 or 2-by-2 diagonal block in the Schur
C           form.
C
            IF ( I.EQ.IEND ) THEN
               ISTEP = 1
            ELSE
               IF ( B(I+FWD,I).EQ.ZERO ) THEN
                  ISTEP = 1
               ELSE
                  ISTEP = 2
               END IF
            END IF
C
            IF ( ISTEP.EQ.1 ) THEN
               CALL SB04NW( ABSCHR, ULB, N, M, C, LDC, I, B, LDB,
     $                      DWORK(JWORK) )
               CALL SB04NY( 'R', ULA, N, A, LDA, B(I,I), DWORK(JWORK),
     $                      TOL1, IWORK, DWORK, LDW, INFO )
               IF ( INFO.EQ.1 )
     $            RETURN
               CALL DCOPY( N, DWORK(JWORK), 1, C(1,I), 1 )
            ELSE
               IPINCR = I + INCR
               CALL SB04NV( ABSCHR, ULB, N, M, C, LDC, IPINCR, B, LDB,
     $                      DWORK(JWORK) )
               CALL SB04NX( 'R', ULA, N, A, LDA, B(IPINCR,IPINCR),
     $                      B(IPINCR+1,IPINCR), B(IPINCR,IPINCR+1),
     $                      B(IPINCR+1,IPINCR+1), DWORK(JWORK), TOL1,
     $                      IWORK, DWORK, LDW, INFO )
               IF ( INFO.EQ.1 )
     $            RETURN
               CALL DCOPY( N, DWORK(JWORK), 2, C(1,IPINCR), 1 )
               CALL DCOPY( N, DWORK(JWORK+1), 2, C(1,IPINCR+1), 1 )
            END IF
            I = I + FWD*ISTEP
            GO TO 20
         END IF
C        END WHILE 20
      ELSE
C
C        A is in Schur form: recursion on the rows of A.
C
         IF ( LULA ) THEN
C
C           A is upper: backward recursion.
C
            IBEG = N
            IEND = 1
            FWD  = -1
            INCR = -1
         ELSE
C
C           A is lower: forward recursion.
C
            IBEG = 1
            IEND = N
            FWD  = 1
            INCR = 0
         END IF
         I = IBEG
C        WHILE ( ( IEND - I ) * FWD .GE. 0 ) DO
   40    IF ( ( IEND - I )*FWD.GE.0 ) THEN
C
C           Test for 1-by-1 or 2-by-2 diagonal block in the Schur
C           form.
C
            IF ( I.EQ.IEND ) THEN
               ISTEP = 1
            ELSE
               IF ( A(I,I+FWD).EQ.ZERO ) THEN
                  ISTEP = 1
               ELSE
                  ISTEP = 2
               END IF
            END IF
C
            IF ( ISTEP.EQ.1 ) THEN
               CALL SB04NW( ABSCHR, ULA, N, M, C, LDC, I, A, LDA, 
     $                      DWORK(JWORK) )
               CALL SB04NY( 'C', ULB, M, B, LDB, A(I,I), DWORK(JWORK),
     $                      TOL1, IWORK, DWORK, LDW, INFO )
               IF ( INFO.EQ.1 )
     $            RETURN
               CALL DCOPY( M, DWORK(JWORK), 1, C(I,1), LDC )
            ELSE
               IPINCR = I + INCR
               CALL SB04NV( ABSCHR, ULA, N, M, C, LDC, IPINCR, A, LDA,
     $                      DWORK(JWORK) )
               CALL SB04NX( 'C', ULB, M, B, LDB, A(IPINCR,IPINCR),
     $                      A(IPINCR+1,IPINCR), A(IPINCR,IPINCR+1),
     $                      A(IPINCR+1,IPINCR+1), DWORK(JWORK), TOL1,
     $                      IWORK, DWORK, LDW, INFO )
               IF ( INFO.EQ.1 )
     $            RETURN
               CALL DCOPY( M, DWORK(JWORK), 2, C(IPINCR,1), LDC )
               CALL DCOPY( M, DWORK(JWORK+1), 2, C(IPINCR+1,1), LDC )
            END IF
            I = I + FWD*ISTEP
            GO TO 40
         END IF
C        END WHILE 40
      END IF
C
      RETURN
C *** Last line of SB04ND ***
      END
