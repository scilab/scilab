      SUBROUTINE SB10DD( N, M, NP, NCON, NMEAS, GAMMA, A, LDA, B, LDB, 
     $                   C, LDC, D, LDD, AK, LDAK, BK, LDBK, CK, LDCK,
     $                   DK, LDDK, X, LDX, Z, LDZ, RCOND, TOL, IWORK,
     $                   DWORK, LDWORK, BWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute the matrices of an H-infinity (sub)optimal n-state
C     controller
C
C                           | AK | BK |
C                       K = |----|----|,
C                           | CK | DK |
C
C     for the discrete-time system
C
C                   | A  | B1  B2  |   | A | B |
C               P = |----|---------| = |---|---|
C                   | C1 | D11 D12 |   | C | D | 
C                   | C2 | D21 D22 |
C
C     and for a given value of gamma, where B2 has as column size the
C     number of control inputs (NCON) and C2 has as row size the number
C     of measurements (NMEAS) being provided to the controller.
C
C     It is assumed that 
C
C     (A1) (A,B2) is stabilizable and (C2,A) is detectable,
C
C     (A2) D12 is full column rank and D21 is full row rank,
C
C               j*Theta 
C     (A3) | A-e       *I  B2  | has full column rank for all 
C          |    C1         D12 |
C
C          0 <= Theta < 2*Pi ,
C
C               j*Theta
C     (A4) | A-e       *I  B1  |  has full row rank for all
C          |    C2         D21 |  
C          
C          0 <= Theta < 2*Pi .
C
C     ARGUMENTS
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the system.  N >= 0.
C
C     M       (input) INTEGER
C             The column size of the matrix B.  M >= 0.
C
C     NP      (input) INTEGER
C             The row size of the matrix C.  NP >= 0.
C
C     NCON    (input) INTEGER
C             The number of control inputs (M2).  M >= NCON >= 0,
C             NP-NMEAS >= NCON.
C
C     NMEAS   (input) INTEGER
C             The number of measurements (NP2).  NP >= NMEAS >= 0,
C             M-NCON >= NMEAS.
C
C     GAMMA   (input) DOUBLE PRECISION
C             The value of gamma. It is assumed that gamma is
C             sufficiently large so that the controller is admissible.
C             GAMMA > 0.
C
C     A       (input) DOUBLE PRECISION array, dimension (LDA,N)
C             The leading N-by-N part of this array must contain the
C             system state matrix A.
C
C     LDA     INTEGER
C             The leading dimension of the array A.  LDA >= max(1,N).
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,M)
C             The leading N-by-M part of this array must contain the
C             system input matrix B.
C
C     LDB     INTEGER
C             The leading dimension of the array B.  LDB >= max(1,N).
C
C     C       (input) DOUBLE PRECISION array, dimension (LDC,N)
C             The leading NP-by-N part of this array must contain the
C             system output matrix C.
C
C     LDC     INTEGER
C             The leading dimension of the array C.  LDC >= max(1,NP).
C
C     D       (input) DOUBLE PRECISION array, dimension (LDD,M)
C             The leading NP-by-M part of this array must contain the
C             system input/output matrix D.
C           
C     LDD     INTEGER
C             The leading dimension of the array D.  LDD >= max(1,NP).
C
C     AK      (output) DOUBLE PRECISION array, dimension (LDAK,N)
C             The leading N-by-N part of this array contains the
C             controller state matrix AK.
C
C     LDAK    INTEGER
C             The leading dimension of the array AK.  LDAK >= max(1,N).
C
C     BK      (output) DOUBLE PRECISION array, dimension (LDBK,NMEAS)
C             The leading N-by-NMEAS part of this array contains the
C             controller input matrix BK.
C
C     LDBK    INTEGER
C             The leading dimension of the array BK.  LDBK >= max(1,N).
C
C     CK      (output) DOUBLE PRECISION array, dimension (LDCK,N)
C             The leading NCON-by-N part of this array contains the
C             controller output matrix CK.
C
C     LDCK    INTEGER
C             The leading dimension of the array CK.
C             LDCK >= max(1,NCON).
C
C     DK      (output) DOUBLE PRECISION array, dimension (LDDK,NMEAS)
C             The leading NCON-by-NMEAS part of this array contains the
C             controller input/output matrix DK.
C
C     LDDK    INTEGER
C             The leading dimension of the array DK.
C             LDDK >= max(1,NCON).
C
C     X       (output) DOUBLE PRECISION array, dimension (LDX,N)
C             The leading N-by-N part of this array contains the matrix
C             X, solution of the X-Riccati equation.
C
C     LDX     INTEGER
C             The leading dimension of the array X.  LDX >= max(1,N).
C
C     Z       (output) DOUBLE PRECISION array, dimension (LDZ,N)
C             The leading N-by-N part of this array contains the matrix
C             Z, solution of the Z-Riccati equation.
C
C     LDZ     INTEGER
C             The leading dimension of the array Z.  LDZ >= max(1,N).
C  
C     RCOND   (output) DOUBLE PRECISION array, dimension (8)
C             RCOND contains estimates of the reciprocal condition
C             numbers of the matrices which are to be inverted and 
C             estimates of the reciprocal condition numbers of the
C             Riccati equations which have to be solved during the
C             computation of the controller. (See the description of
C             the algorithm in [2].)
C             RCOND(1) contains the reciprocal condition number of the 
C                      matrix R3;
C             RCOND(2) contains the reciprocal condition number of the 
C                      matrix R1 - R2'*inv(R3)*R2;
C             RCOND(3) contains the reciprocal condition number of the 
C                      matrix V21;
C             RCOND(4) contains the reciprocal condition number of the 
C                      matrix St3;
C             RCOND(5) contains the reciprocal condition number of the 
C                      matrix V12;
C             RCOND(6) contains the reciprocal condition number of the 
C                      matrix Im2 + DKHAT*D22
C             RCOND(7) contains the reciprocal condition number of the 
C                      X-Riccati equation;
C             RCOND(8) contains the reciprocal condition number of the 
C                      Z-Riccati equation.
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             Tolerance used in neglecting the small singular values
C             in rank determination. If TOL <= 0, then a default value
C             equal to 1000*EPS is used, where EPS is the relative
C             machine precision.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension max(2*max(M2,N),M,M2+NP2,N*N)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) contains the optimal
C             LDWORK.
C
C     LDWORK  INTEGER
C             The dimension of the array DWORK.
C             LDWORK >= max(LW1,LW2,LW3,LW4), where
C             LW1 = (N+NP1+1)*(N+M2) + max(3*(N+M2)+N+NP1,5*(N+M2));
C             LW2 = (N+NP2)*(N+M1+1) + max(3*(N+NP2)+N+M1,5*(N+NP2));
C             LW3 = 13*N*N + 2*M*M + N*(8*M+NP2) + M1*(M2+NP2) + 6*N + 
C                   max(14*N+23,16*N,2*N+M,3*M);
C             LW4 = 13*N*N + M*M + (8*N+M+M2+2*NP2)*(M2+NP2) + 6*N +
C                   N*(M+NP2) + max(14*N+23,16*N,2*N+M2+NP2,3*(M2+NP2));
C             For good performance, LDWORK must generally be larger.
C             Denoting Q = max(M1,M2,NP1,NP2), an upper bound is
C             max((N+Q)*(N+Q+6),13*N*N + M*M + 2*Q*Q + N*(M+Q) +
C                 max(M*(M+7*N),2*Q*(8*N+M+2*Q)) + 6*N + 
C                 max(14*N+23,16*N,2*N+max(M,2*Q),3*max(M,2*Q)).
C
C     BWORK   LOGICAL array, dimension (2*N)
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C                                      j*Theta
C             = 1:  if the matrix | A-e       *I  B2  | had not full
C                                 |      C1       D12 |
C                   column rank;
C                                      j*Theta
C             = 2:  if the matrix | A-e       *I  B1  | had not full
C                                 |      C2       D21 |
C                   row rank;
C             = 3:  if the matrix D12 had not full column rank;
C             = 4:  if the matrix D21 had not full row rank;
C             = 5:  if the controller is not admissible (too small value
C                   of gamma);
C             = 6:  if the X-Riccati equation was not solved
C                   successfully (the controller is not admissible or
C                   there are numerical difficulties);
C             = 7:  if the Z-Riccati equation was not solved
C                   successfully (the controller is not admissible or
C                   there are numerical difficulties);
C             = 8:  if the matrix Im2 + DKHAT*D22 is singular.
C             = 9:  if the singular value decomposition (SVD) algorithm
C                   did not converge (when computing the SVD of one of
C                   the matrices |A   B2 |, |A   B1 |, D12 or D21).
C                                |C1  D12|  |C2  D21|
C
C     METHOD
C
C     The routine implements the method presented in [1].
C
C     REFERENCES 
C
C     [1] Green, M. and Limebeer, D.J.N.
C         Linear Robust Control.
C         Prentice-Hall, Englewood Cliffs, NJ, 1995.
C
C     [2] Petkov, P.Hr., Gu, D.W., and Konstantinov, M.M.
C         Fortran 77 routines for Hinf and H2 design of linear 
C         discrete-time control systems.
C         Report 99-8, Department of Engineering, Leicester University,
C         April 1999.
C
C     NUMERICAL ASPECTS
C
C     With approaching the minimum value of gamma some of the matrices
C     which are to be inverted tend to become ill-conditioned and 
C     the X- or Z-Riccati equation may also become ill-conditioned
C     which may deteriorate the accuracy of the result. (The
C     corresponding reciprocal condition numbers are given in 
C     the output array RCOND.) 
C
C     CONTRIBUTORS
C
C     P.Hr. Petkov, D.W. Gu and M.M. Konstantinov, April 1999.
C
C     REVISIONS
C
C     V. Sima, Research Institute for Informatics, Bucharest, Sep. 1999.
C     V. Sima, Research Institute for Informatics, Bucharest, Feb. 2000.
C
C     KEYWORDS
C
C     Algebraic Riccati equation, discrete-time H-infinity optimal
C     control, robust control.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE, THOUSN
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0,
     $                     THOUSN = 1.0D+3 )
C     ..
C     .. Scalar Arguments ..
      INTEGER            INFO, LDA, LDAK, LDB, LDBK, LDC, LDCK, LDD,
     $                   LDDK, LDWORK, LDX, LDZ, M, N, NCON, NMEAS, NP
      DOUBLE PRECISION   GAMMA, TOL
C     ..
C     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), AK( LDAK, * ), B( LDB, * ),
     $                   BK( LDBK, * ), C( LDC, * ), CK( LDCK, * ),
     $                   D( LDD, * ), DK( LDDK, * ), DWORK( * ),
     $                   RCOND( * ), X( LDX, * ), Z( LDZ, * )
      LOGICAL            BWORK( * )
C     ..
C     .. Local Scalars ..
      INTEGER            INFO2, IR2, IR3, IS2, IS3, IWB, IWC, IWD, IWG,
     $                   IWH, IWI, IWL, IWQ, IWR, IWRK, IWS, IWT, IWU,
     $                   IWV, IWW, J, LWAMAX, M1, M2, MINWRK, NP1, NP2
      DOUBLE PRECISION   ANORM, FERR, RCOND2, SEPD, TOLL
C
C     .. External Functions
      DOUBLE PRECISION   DLAMCH, DLANGE, DLANSY
      EXTERNAL           DLAMCH, DLANGE, DLANSY
C     ..
C     .. External Subroutines ..
      EXTERNAL           DGECON, DGEMM, DGESVD, DGETRF, DGETRS, DLACPY,
     $                   DLASET, DPOCON, DPOTRF, DSCAL, DSWAP, DSYRK,
     $                   DSYTRF, DSYTRS, DTRCON, DTRSM, MA02AD, MB01RU,
     $                   MB01RX, SB02OD, SB02SD, XERBLA
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX
C     ..
C     .. Executable Statements ..      
C
C     Decode and Test input parameters.
C
      M1 = M - NCON
      M2 = NCON
      NP1 = NP - NMEAS
      NP2 = NMEAS
C
      INFO = 0
      IF( N.LT.0 ) THEN
         INFO = -1
      ELSE IF( M.LT.0 ) THEN
         INFO = -2
      ELSE IF( NP.LT.0 ) THEN
         INFO = -3
      ELSE IF( NCON.LT.0 .OR. M1.LT.0 .OR. M2.GT.NP1 ) THEN
         INFO = -4
      ELSE IF( NMEAS.LT.0 .OR. NP1.LT.0 .OR. NP2.GT.M1 ) THEN
         INFO = -5
      ELSE IF( GAMMA.LE.ZERO ) THEN
         INFO = -6
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -8
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -10  
      ELSE IF( LDC.LT.MAX( 1, NP ) ) THEN
         INFO = -12
      ELSE IF( LDD.LT.MAX( 1, NP ) ) THEN
         INFO = -14
      ELSE IF( LDAK.LT.MAX( 1, N ) ) THEN
         INFO = -16
      ELSE IF( LDBK.LT.MAX( 1, N ) ) THEN
         INFO = -18
      ELSE IF( LDCK.LT.MAX( 1, M2 ) ) THEN
         INFO = -20
      ELSE IF( LDDK.LT.MAX( 1, M2 ) ) THEN
         INFO = -22
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -24
      ELSE IF( LDZ.LT.MAX( 1, N ) ) THEN
         INFO = -26 
      ELSE
C
C        Compute workspace.
C
         IWB = ( N + NP1 + 1 )*( N + M2 ) + 
     $         MAX( 3*( N + M2 ) + N + NP1, 5*( N + M2 ) )
         IWC = ( N + NP2 )*( N + M1 + 1 ) +  
     $         MAX( 3*( N + NP2 ) + N + M1, 5*( N + NP2 ) )
         IWD = 13*N*N + 2*M*M + N*( 8*M + NP2 ) + M1*( M2 + NP2 ) + 
     $         6*N + MAX( 14*N + 23, 16*N, 2*N + M, 3*M )
         IWG = 13*N*N + M*M + ( 8*N + M + M2 + 2*NP2 )*( M2 + NP2 ) +
     $         6*N + N*( M + NP2 ) + 
     $         MAX( 14*N + 23, 16*N, 2*N + M2 + NP2, 3*( M2 + NP2 ) )
         MINWRK = MAX( IWB, IWC, IWD, IWG )
         IF( LDWORK.LT.MINWRK )
     $      INFO = -31
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB10DD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( N.EQ.0 .OR. M.EQ.0 .OR. NP.EQ.0 .OR. M1.EQ.0 .OR. M2.EQ.0
     $    .OR. NP1.EQ.0 .OR. NP2.EQ.0 ) THEN
          RCOND( 1 ) = ONE
          RCOND( 2 ) = ONE
          RCOND( 3 ) = ONE
          RCOND( 4 ) = ONE
          RCOND( 5 ) = ONE
          RCOND( 6 ) = ONE
          RCOND( 7 ) = ONE
          RCOND( 8 ) = ONE
          DWORK( 1 ) = ONE
          RETURN
      END IF
C
      TOLL = TOL
      IF( TOLL.LE.ZERO ) THEN
C
C        Set the default value of the tolerance in rank determination.
C
         TOLL = THOUSN*DLAMCH( 'Epsilon' )
      END IF
C
C     Workspace usage.
C
      IWS  = (N+NP1)*(N+M2) + 1
      IWRK = IWS + (N+M2)
C
C                      jTheta
C     Determine if |A-e      I  B2 | has full column rank at
C                  |     C1     D12|
C     Theta = Pi/2 .
C     Workspace: need (N+NP1+1)*(N+M2) + MAX(3*(N+M2)+N+NP1,5*(N+M2));
C                prefer larger.
C
      CALL DLACPY( 'Full', N, N, A, LDA, DWORK, N+NP1 )
      CALL DLACPY( 'Full', NP1, N, C, LDC, DWORK( N+1 ), N+NP1 )
      CALL DLACPY( 'Full', N, M2, B( 1, M1+1 ), LDB,
     $             DWORK( (N+NP1)*N+1 ), N+NP1 )
      CALL DLACPY( 'Full', NP1, M2, D( 1, M1+1 ), LDD,
     $             DWORK( (N+NP1)*N+N+1 ), N+NP1 )
      CALL DGESVD( 'N', 'N', N+NP1, N+M2, DWORK, N+NP1, DWORK( IWS ),
     $             DWORK, N+NP1, DWORK, N+M2, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 9
         RETURN
      END IF
      IF( DWORK( IWS+N+M2 ) / DWORK( IWS ).LE.TOLL ) THEN
         INFO = 1
         RETURN
      END IF
      LWAMAX = INT( DWORK( IWRK ) ) + IWRK - 1
C
C     Workspace usage.
C
      IWS  = (N+NP2)*(N+M1) + 1
      IWRK = IWS + (N+NP2)
C
C                      jTheta
C     Determine if |A-e      I   B1 | has full row rank at
C                  |     C2      D21|
C     Theta = Pi/2 .
C     Workspace: need   (N+NP2)*(N+M1+1) + 
C                       MAX(3*(N+NP2)+N+M1,5*(N+NP2));
C                prefer larger.
C
      CALL DLACPY( 'Full', N, N, A, LDA, DWORK, N+NP2 )
      CALL DLACPY( 'Full', NP2, N, C( NP1+1, 1), LDC, DWORK( N+1 ),
     $             N+NP2 )     
      CALL DLACPY( 'Full', N, M1, B, LDB, DWORK( (N+NP2)*N+1 ),
     $             N+NP2 )      
      CALL DLACPY( 'Full', NP2, M1, D( NP1+1, 1 ), LDD,
     $             DWORK( (N+NP2)*N+N+1 ), N+NP2 )
      CALL DGESVD( 'N', 'N', N+NP2, N+M1, DWORK, N+NP2, DWORK( IWS ),
     $             DWORK, N+NP2, DWORK, N+M1, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 9
         RETURN
      END IF
      IF( DWORK( IWS+N+NP2 ) / DWORK( IWS ).LE.TOLL ) THEN
         INFO = 2
         RETURN
      END IF
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Workspace usage.
C
      IWS  = NP1*M2 + 1
      IWRK = IWS + M2
C
C     Determine if D12 has full column rank.
C     Workspace: need   (NP1+1)*M2 + MAX(3*M2+NP1,5*M2);
C                prefer larger.
C
      CALL DLACPY( 'Full', NP1, M2, D( 1, M1+1 ), LDD, DWORK, NP1 )
      CALL DGESVD( 'N', 'N', NP1, M2, DWORK, NP1, DWORK( IWS ), DWORK,
     $             NP1, DWORK, M2, DWORK( IWRK ), LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 9
         RETURN
      END IF
      IF( DWORK( IWS+M2 ) / DWORK( IWS ).LE.TOLL ) THEN
         INFO = 3
         RETURN
      END IF
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Workspace usage.
C
      IWS  = NP2*M1 + 1
      IWRK = IWS + NP2
C
C     Determine if D21 has full row rank.
C     Workspace: need   NP2*(M1+1) + MAX(3*NP2+M1,5*NP2);
C                prefer larger.
C
      CALL DLACPY( 'Full', NP2, M1, D( NP1+1, 1 ), LDD, DWORK, NP2 )
      CALL DGESVD( 'N', 'N', NP2, M1, DWORK, NP2, DWORK( IWS ), DWORK,
     $             NP2, DWORK, M1, DWORK( IWRK ), LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 9
         RETURN
      END IF
      IF( DWORK( IWS+NP2 ) / DWORK( IWS ).LE.TOLL ) THEN
         INFO = 4
         RETURN
      END IF
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Workspace usage.
C
      IWV  = 1
      IWB  = IWV + M*M
      IWC  = IWB + N*M1
      IWD  = IWC + ( M2 + NP2 )*N
      IWQ  = IWD + ( M2 + NP2 )*M1 
      IWL  = IWQ + N*N
      IWR  = IWL + N*M
      IWI  = IWR + 2*N
      IWH  = IWI + 2*N
      IWS  = IWH + 2*N
      IWT  = IWS + ( 2*N + M )*( 2*N + M )
      IWU  = IWT + ( 2*N + M )*2*N 
      IWRK = IWU + 4*N*N
      IR2  = IWV + M1
      IR3  = IR2 + M*M1
C
C     Compute R0 = |D11'||D11 D12| -|gamma^2*Im1 0| .
C                  |D12'|           |   0        0|
C
      CALL DSYRK( 'Lower', 'Transpose', M, NP1, ONE, D, LDD, ZERO,
     $            DWORK, M )
      DO 10 J = 1, M*M1, M + 1
         DWORK( J ) = DWORK( J ) - GAMMA*GAMMA
   10 CONTINUE
C
C     Compute C1'*C1 .
C
      CALL DSYRK( 'Lower', 'Transpose', N, NP1, ONE, C, LDC, ZERO, 
     $            DWORK( IWQ ), N )
C
C     Compute C1'*|D11 D12| .
C
      CALL DGEMM( 'Transpose', 'NoTranspose', N, M, NP1, ONE, C, LDC, 
     $            D, LDD, ZERO, DWORK( IWL ), N )
C
C     Solution of the X-Riccati equation.
C     Workspace: need   13*N*N + 2*M*M + N*(8*M+NP2) + M1*(M2+NP2) + 
C                       6*N + max(14*N+23,16*N,2*N+M,3*M);
C                prefer larger.
C
      CALL SB02OD( 'D', 'B', 'N', 'L', 'N', 'S', N, M, NP, A, LDA, B,
     $             LDB, DWORK( IWQ ), N, DWORK, M, DWORK( IWL ), N,
     $             RCOND2, X, LDX, DWORK( IWR ), DWORK( IWI ),
     $             DWORK( IWH ), DWORK( IWS ), 2*N+M, DWORK( IWT ),
     $             2*N+M, DWORK( IWU ), 2*N, TOLL, IWORK,
     $             DWORK( IWRK ), LDWORK-IWRK+1, BWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 6
         RETURN
      END IF
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Condition estimation.
C     Workspace: need   4*N*N + 2*M*M + N*(3*M+NP2) + M1*(M2+NP2) +
C                       max(5*N,max(3,2*N*N)+N*N);
C                prefer larger.
C
      IWS  = IWR
      IWH  = IWS + M*M
      IWT  = IWH + N*M
      IWU  = IWT + N*N 
      IWG  = IWU + N*N 
      IWRK = IWG + N*N
      CALL DLACPY( 'Lower', M, M, DWORK, M, DWORK( IWS ), M )
      CALL DSYTRF( 'Lower', M, DWORK( IWS ), M, IWORK, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 5
         RETURN
      END IF
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
      CALL MA02AD( 'Full', N, M, B, LDB, DWORK( IWH ), M )
      CALL DSYTRS( 'Lower', M, N, DWORK( IWS ), M, IWORK, DWORK( IWH ), 
     $             M, INFO2 )
      CALL MB01RX( 'Left', 'Lower', 'NoTranspose', N, M, ZERO, ONE,
     $             DWORK( IWG ), N, B, LDB, DWORK( IWH ), M, INFO2 )
      CALL SB02SD( 'C', 'N', 'N', 'L', 'O', N, A, LDA, DWORK( IWT ), N,
     $             DWORK( IWU ), N, DWORK( IWG ), N, DWORK( IWQ ), N, X,
     $             LDX, SEPD, RCOND( 7 ), FERR, IWORK, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) RCOND( 7 ) = ZERO
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Workspace usage.
C
      IWRK = IWR
C
C     Compute the lower triangle of |R1  R2'| = R0 + B'*X*B .
C                                   |R2  R3 |
C
      CALL MB01RU( 'Lower', 'Transpose', M, N, ONE, ONE, DWORK, M,
     $             B, LDB, X, LDX, DWORK( IWRK ), M*N, INFO2 )
C
C     Compute the Cholesky factorization of R3, R3 = V12'*V12 .
C     Note that V12' is stored.
C
      ANORM = DLANSY( '1', 'Lower', M2, DWORK( IR3 ), M, DWORK( IWRK ) )
      CALL DPOTRF( 'Lower', M2, DWORK( IR3 ), M, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 5
         RETURN
      END IF
      CALL DPOCON( 'Lower', M2, DWORK( IR3 ), M, ANORM, RCOND( 1 ),
     $             DWORK( IWRK ), IWORK, INFO2 )
C
C     Return if the matrix is singular to working precision.
C
      IF( RCOND( 1 ).LT.TOLL ) THEN
         INFO = 5
         RETURN
      END IF     
C
      CALL DTRCON( '1', 'Lower', 'NonUnit', M2, DWORK( IR3 ), M,
     $             RCOND( 5 ), DWORK( IWRK ), IWORK, INFO2 )
C
C     Return if the matrix is singular to working precision.
C
      IF( RCOND( 5 ).LT.TOLL ) THEN
         INFO = 5
         RETURN
      END IF     
C
C     Compute R2 <- inv(V12')*R2 .
C
      CALL DTRSM( 'Left', 'Lower', 'NoTranspose', 'NonUnit', M2, M1,
     $            ONE, DWORK( IR3 ), M, DWORK( IR2 ), M )
C                   
C     Compute -Nabla = R2'*inv(R3)*R2 - R1 .
C
      CALL DSYRK( 'Lower', 'Transpose', M1, M2, ONE, DWORK( IR2 ), M,
     $            -ONE, DWORK, M )
C
C     Compute the Cholesky factorization of -Nabla, -Nabla = V21t'*V21t.
C     Note that V21t' is stored.
C
      ANORM = DLANSY( '1', 'Lower', M1, DWORK, M, DWORK( IWRK ) )
      CALL DPOTRF( 'Lower', M1, DWORK, M, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 5
         RETURN
      END IF
      CALL DPOCON( 'Lower', M1, DWORK, M, ANORM, RCOND( 2 ),
     $             DWORK( IWRK ), IWORK, INFO2 )
C
C     Return if the matrix is singular to working precision.
C
      IF( RCOND( 2 ).LT.TOLL ) THEN
         INFO = 5
         RETURN
      END IF     
C
      CALL DTRCON( '1', 'Lower', 'NonUnit', M1, DWORK, M, RCOND( 3 ),
     $             DWORK( IWRK ), IWORK, INFO2 )
C
C     Return if the matrix is singular to working precision.
C
      IF( RCOND( 3 ).LT.TOLL ) THEN
         INFO = 5
         RETURN
      END IF     
C
C     Compute X*A .
C
      CALL DGEMM( 'NoTranspose', 'NoTranspose', N, N, N, ONE, X, LDX,
     $            A, LDA, ZERO, DWORK( IWQ ), N )
C
C     Compute |L1| = |D11'|*C1 + B'*X*A .
C             |L2| = |D12'| 
C 
      CALL MA02AD( 'Full', N, M, DWORK( IWL ), N, DWORK( IWRK ), M )
      CALL DLACPY( 'Full', M, N, DWORK( IWRK ), M, DWORK( IWL ), M )
      CALL DGEMM( 'Transpose', 'NoTranspose', M, N, N, ONE, B, LDB, 
     $            DWORK( IWQ ), N, ONE, DWORK( IWL ), M )
C
C     Compute L2 <- inv(V12')*L2 .
C
      CALL DTRSM( 'Left', 'Lower', 'NoTranspose', 'NonUnit', M2, N, ONE,
     $            DWORK( IR3 ), M, DWORK( IWL+M1 ), M )
C
C     Compute L_Nabla = L1 - R2'*inv(R3)*L2 .
C
      CALL DGEMM( 'Transpose', 'NoTranspose', M1, N, M2, -ONE,
     $            DWORK( IR2 ), M, DWORK( IWL+M1 ), M, ONE, 
     $            DWORK( IWL ), M )
C
C     Compute L_Nabla <- inv(V21t')*L_Nabla .
C
      CALL DTRSM( 'Left', 'Lower', 'NoTranspose', 'NonUnit', M1, N, ONE,
     $            DWORK, M, DWORK( IWL ), M )
C
C     Compute Bt1 = B1*inv(V21t) .
C
      CALL DLACPY( 'Full', N, M1, B, LDB, DWORK( IWB ), N )
      CALL DTRSM( 'Right', 'Lower', 'Transpose', 'NonUnit', N, M1, ONE,
     $            DWORK, M, DWORK( IWB ), N )
C
C     Compute At .
C
      CALL DLACPY( 'Full', N, N, A, LDA, AK, LDAK )
      CALL DGEMM( 'NoTranspose', 'NoTranspose', N, N, M1, ONE,
     $            DWORK( IWB ), N, DWORK( IWL ), M, ONE, AK, LDAK )
C
C     Scale Bt1 .
C
      CALL DSCAL( N*M1, GAMMA, DWORK( IWB ), 1 )
C
C     Compute |Dt11| = |R2 |*inv(V21t) .
C             |Dt21|   |D21|
C
      CALL DLACPY( 'Full', M2, M1, DWORK( IR2 ), M, DWORK( IWD ), 
     $             M2+NP2 )
      CALL DLACPY( 'Full', NP2, M1, D( NP1+1, 1 ), LDD, DWORK( IWD+M2 ),
     $             M2+NP2 )     
      CALL DTRSM( 'Right', 'Lower', 'Transpose', 'NonUnit', M2+NP2,
     $            M1, ONE, DWORK, M, DWORK( IWD ), M2+NP2 )
C
C     Compute Ct = |Ct1| = |L2| + |Dt11|*inv(V21t')*L_Nabla .
C                  |Ct2| = |C2| + |Dt21|
C
      CALL DLACPY( 'Full', M2, N, DWORK( IWL+M1 ), M, DWORK( IWC ), 
     $             M2+NP2 )
      CALL DLACPY( 'Full', NP2, N, C( NP1+1, 1 ), LDC, DWORK( IWC+M2 ),
     $             M2+NP2 )     
      CALL DGEMM( 'NoTranspose', 'NoTranspose', M2+NP2, N, M1, ONE,
     $            DWORK( IWD ), M2+NP2, DWORK( IWL ), M, ONE, 
     $            DWORK( IWC ), M2+NP2 )
C
C     Scale |Dt11| .
C           |Dt21|
C
      CALL DSCAL( ( M2+NP2 )*M1, GAMMA, DWORK( IWD ), 1 )
C
C     Workspace usage.
C
      IWW  = IWD + ( M2 + NP2 )*M1  
      IWQ  = IWW + ( M2 + NP2 )*( M2 + NP2 )
      IWL  = IWQ + N*N
      IWR  = IWL + N*( M2 + NP2 )
      IWI  = IWR + 2*N
      IWH  = IWI + 2*N
      IWS  = IWH + 2*N
      IWT  = IWS + ( 2*N + M2 + NP2 )*( 2*N + M2 + NP2 )
      IWU  = IWT + ( 2*N + M2 + NP2 )*2*N 
      IWG  = IWU + 4*N*N
      IWRK = IWG + ( M2 + NP2 )*N
      IS2  = IWW + ( M2 + NP2 )*M2
      IS3  = IS2 + M2
C
C     Compute S0 = |Dt11||Dt11' Dt21'| -|gamma^2*Im2 0| .
C                  |Dt21|               |   0        0|
C
      CALL DSYRK( 'Upper', 'NoTranspose', M2+NP2, M1, ONE, DWORK( IWD ),
     $            M2+NP2, ZERO, DWORK( IWW ), M2+NP2 )
      DO 20 J = IWW, IWW - 1 + ( M2 + NP2 )*M2, M2 + NP2 + 1
         DWORK( J ) = DWORK( J ) - GAMMA*GAMMA
   20 CONTINUE
C
C     Compute Bt1*Bt1' .
C
      CALL DSYRK( 'Upper', 'NoTranspose', N, M1, ONE, DWORK( IWB ), N,
     $            ZERO, DWORK( IWQ ), N )
C
C     Compute Bt1*|Dt11' Dt21'| .
C
      CALL DGEMM( 'NoTranspose', 'Transpose', N, M2+NP2, M1, ONE,
     $            DWORK( IWB ), N, DWORK( IWD ), M2+NP2, ZERO,
     $            DWORK( IWL ), N )
C
C     Transpose At in situ (in AK) .
C
      DO 30 J = 2, N
         CALL DSWAP( J-1, AK( J, 1 ), LDAK, AK( 1, J ), 1 )
   30 CONTINUE 
C
C     Transpose Ct .
C
      CALL MA02AD( 'Full', M2+NP2, N, DWORK( IWC ), M2+NP2, 
     $             DWORK( IWG ), N )
C
C     Solution of the Z-Riccati equation.
C     Workspace: need   13*N*N + M*M + (8*N+M+M2+2*NP2)*(M2+NP2) +
C                       N*(M+NP2) + 6*N + 
C                       max(14*N+23,16*N,2*N+M2+NP2,3*(M2+NP2));
C                prefer larger.
C
      CALL SB02OD( 'D', 'B', 'N', 'U', 'N', 'S', N, M2+NP2, NP, AK, 
     $             LDAK, DWORK( IWG ), N, DWORK( IWQ ), N, DWORK( IWW ),
     $             M2+NP2, DWORK( IWL ), N, RCOND2, Z, LDZ, DWORK( IWR),
     $             DWORK( IWI ), DWORK( IWH ), DWORK( IWS ), 2*N+M2+NP2,
     $             DWORK( IWT ), 2*N+M2+NP2, DWORK( IWU ), 2*N, TOLL,
     $             IWORK, DWORK( IWRK ), LDWORK-IWRK+1, BWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 7
         RETURN
      END IF
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Condition estimation.
C     Workspace: need   4*N*N + M*M + 2*(M2+NP2)*(M2+NP2)+
C                       N*(M+2*M2+3*NP2) + (M2+NP2)*M1 +
C                       max(5*N,max(3,2*N*N)+N*N);
C                prefer larger.
C
      IWS  = IWR
      IWH  = IWS + ( M2 + NP2 )*( M2 + NP2 )
      IWT  = IWH + N*( M2 + NP2 )
      IWU  = IWT + N*N 
      IWG  = IWU + N*N 
      IWRK = IWG + N*N
      CALL DLACPY( 'Upper', M2+NP2, M2+NP2, DWORK( IWW ), M2+NP2,
     $             DWORK( IWS ), M2+NP2 )
      CALL DSYTRF( 'Upper', M2+NP2, DWORK( IWS ), M2+NP2, IWORK,
     $             DWORK( IWRK ), LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 5
         RETURN
      END IF
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
      CALL DLACPY( 'Full', M2+NP2, N, DWORK( IWC ), M2+NP2, 
     $             DWORK( IWH ), M2+NP2 )
      CALL DSYTRS( 'Upper', M2+NP2, N, DWORK( IWS ), M2+NP2, IWORK,
     $             DWORK( IWH ), M2+NP2, INFO2 )
      CALL MB01RX( 'Left', 'Upper', 'Transpose', N, M2+NP2, ZERO, ONE,
     $             DWORK( IWG ), N, DWORK( IWC ), M2+NP2, DWORK( IWH ),
     $             M2+NP2, INFO2 )
      CALL SB02SD( 'C', 'N', 'N', 'U', 'O', N, AK, LDAK, DWORK( IWT ),
     $             N, DWORK( IWU ), N, DWORK( IWG ), N, DWORK( IWQ ), N,
     $             Z, LDZ, SEPD, RCOND( 8 ), FERR, IWORK, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) RCOND( 8 ) = ZERO
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Workspace usage.
C
      IWRK = IWR
C
C     Compute the upper triangle of
C             |St1   St2| = S0 + |Ct1|*Z*|Ct1' Ct2'| .
C             |St2'  St3|        |Ct2|
C
      CALL MB01RU( 'Upper', 'NoTranspose', M2+NP2, N, ONE, ONE,
     $             DWORK( IWW ), M2+NP2, DWORK( IWC ), M2+NP2, Z, LDZ, 
     $             DWORK( IWRK ), (M2+NP2)*N, INFO2 )
C
C     Compute the Cholesky factorization of St3, St3 = U12'*U12 .
C
      ANORM = DLANSY( '1', 'Upper', NP2, DWORK( IS3 ), M2+NP2, 
     $                DWORK( IWRK ) )
      CALL DPOTRF( 'Upper', NP2, DWORK( IS3 ), M2+NP2, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 5
         RETURN
      END IF
      CALL DPOCON( 'Upper', NP2, DWORK( IS3 ), M2+NP2, ANORM,
     $             RCOND( 4 ), DWORK( IWRK ), IWORK, INFO2 )
C
C     Return if the matrix is singular to working precision.
C
      IF( RCOND( 4 ).LT.TOLL ) THEN
         INFO = 5
         RETURN
      END IF
C
C     Compute St2 <- St2*inv(U12) .
C 
      CALL DTRSM( 'Right', 'Upper', 'NoTranspose', 'NonUnit', M2, NP2,
     $            ONE, DWORK( IS3 ), M2+NP2, DWORK( IS2 ), M2+NP2 )
C
C     Check the negative definiteness of St1 - St2*inv(St3)*St2' .
C
      CALL DSYRK( 'Upper', 'NoTranspose', M2, NP2, ONE, DWORK( IS2 ), 
     $            M2+NP2, -ONE, DWORK( IWW ), M2+NP2 )
      CALL DPOTRF( 'Upper', M2, DWORK( IWW ), M2+NP2, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 5
         RETURN
      END IF     
C
C     Restore At in situ .
C
      DO 40 J = 2, N
         CALL DSWAP( J-1, AK( J, 1 ), LDAK, AK( 1, J ), 1 )
   40 CONTINUE 
C
C     Compute At*Z .
C
      CALL DGEMM( 'NoTranspose', 'NoTranspose', N, N, N, ONE, AK, LDAK,
     $            Z, LDZ, ZERO, DWORK( IWRK ), N )
C
C     Compute Mt2 = Bt1*Dt21' + At*Z*Ct2' in BK .
C
      CALL DLACPY( 'Full', N, NP2, DWORK( IWL+N*M2 ), N, BK, LDBK )
      CALL DGEMM( 'NoTranspose', 'Transpose', N, NP2, N, ONE,
     $            DWORK( IWRK ), N, DWORK( IWC+M2 ), M2+NP2, ONE,
     $            BK, LDBK )
C
C     Compute St2 <- St2*inv(U12') .
C
      CALL DTRSM( 'Right', 'Upper', 'Transpose', 'NonUnit', M2, NP2,
     $            ONE, DWORK( IS3 ), M2+NP2, DWORK( IS2 ), M2+NP2 )
C
C     Compute DKHAT = -inv(V12)*St2 in DK .
C
      CALL DLACPY( 'Full', M2, NP2, DWORK( IS2 ), M2+NP2, DK, LDDK ) 
      CALL DTRSM( 'Left', 'Lower', 'Transpose', 'NonUnit', M2, NP2,
     $            -ONE, DWORK( IR3 ), M, DK, LDDK )
C
C     Compute CKHAT = -inv(V12)*(Ct1 - St2*inv(St3)*Ct2) in CK .
C
      CALL DLACPY( 'Full', M2, N, DWORK( IWC ), M2+NP2, CK, LDCK ) 
      CALL DGEMM( 'NoTranspose', 'NoTranspose', M2, N, NP2, -ONE,
     $            DWORK( IS2 ), M2+NP2, DWORK( IWC+M2 ), M2+NP2, ONE, 
     $            CK, LDCK )
      CALL DTRSM( 'Left', 'Lower', 'Transpose', 'NonUnit', M2, N, -ONE,
     $            DWORK( IR3 ), M, CK, LDCK )
C
C     Compute Mt2*inv(St3) in BK .
C
      CALL DTRSM( 'Right', 'Upper', 'NoTranspose', 'NonUnit', N, NP2,
     $            ONE, DWORK( IS3 ), M2+NP2, BK, LDBK )
      CALL DTRSM( 'Right', 'Upper', 'Transpose', 'NonUnit', N, NP2,
     $            ONE, DWORK( IS3 ), M2+NP2, BK, LDBK )
C
C     Compute AKHAT in AK .
C
      CALL DGEMM( 'NoTranspose', 'NoTranspose', N, N, M2, ONE,
     $            B( 1, M1+1 ), LDB, CK, LDCK, ONE, AK, LDAK )
      CALL DGEMM( 'NoTranspose', 'NoTranspose', N, N, NP2, -ONE, BK, 
     $            LDBK, DWORK( IWC+M2 ), M2+NP2, ONE, AK, LDAK )
C
C     Compute BKHAT in BK .
C
      CALL DGEMM( 'NoTranspose', 'NoTranspose', N, NP2, M2, ONE, 
     $            B( 1, M1+1 ), LDB, DK, LDDK, ONE, BK, LDBK )
C
C     Compute Im2 + DKHAT*D22 .
C
      IWRK = M2*M2 + 1
      CALL DLASET( 'Full', M2, M2, ZERO, ONE, DWORK, M2 )
      CALL DGEMM( 'NoTranspose', 'NoTranspose', M2, M2, NP2, ONE, DK,
     $            LDDK, D( NP1+1, M1+1 ), LDD, ONE, DWORK, M2 )
      ANORM = DLANGE( '1', M2, M2, DWORK, M2, DWORK( IWRK ) )
      CALL DGETRF( M2, M2, DWORK, M2, IWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 8
         RETURN
      END IF
      CALL DGECON( '1', M2, DWORK, M2, ANORM, RCOND( 6 ), DWORK( IWRK ),
     $             IWORK( M2+1 ), INFO2 )
C
C     Return if the matrix is singular to working precision.
C
      IF( RCOND( 6 ).LT.TOLL ) THEN
         INFO = 8
         RETURN
      END IF
C
C     Compute CK .
C
      CALL DGETRS( 'NoTranspose', M2, N, DWORK, M2, IWORK, CK, LDCK, 
     $             INFO2 )
C
C     Compute DK .
C
      CALL DGETRS( 'NoTranspose', M2, NP2, DWORK, M2, IWORK, DK, LDDK,
     $             INFO2 )
C
C     Compute AK .
C
      CALL DGEMM( 'NoTranspose', 'NoTranspose', N, M2, NP2, ONE, BK, 
     $            LDBK, D( NP1+1, M1+1 ), LDD, ZERO, DWORK, N )
      CALL DGEMM( 'NoTranspose', 'NoTranspose', N, N, M2, -ONE, DWORK, 
     $            N, CK, LDCK, ONE, AK, LDAK )
C
C     Compute BK .
C 
      CALL DGEMM( 'NoTranspose', 'NoTranspose', N, NP2, M2, -ONE, DWORK,
     $            N, DK, LDDK, ONE, BK, LDBK )
C
      DWORK( 1 ) = DBLE( LWAMAX )
      RETURN
C *** Last line of SB10DD ***
      END
