      SUBROUTINE SB10QD( N, M, NP, NCON, NMEAS, GAMMA, A, LDA, B, LDB, 
     $                   C, LDC, D, LDD, F, LDF, H, LDH, X, LDX, Y, LDY,
     $                   XYCOND, IWORK, DWORK, LDWORK, BWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C     
C     PURPOSE
C
C     To compute the state feedback and the output injection
C     matrices for an H-infinity (sub)optimal n-state controller,
C     using Glover's and Doyle's 1988 formulas, for the system
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
C     (A2) D12 is full column rank with D12 = | 0 | and D21 is
C                                             | I |
C          full row rank with D21 = | 0 I | as obtained by the
C          subroutine SB10PD,
C
C     (A3) | A-j*omega*I  B2  | has full column rank for all omega,
C          |    C1        D12 |
C
C
C     (A4) | A-j*omega*I  B1  |  has full row rank for all omega.
C          |    C2        D21 |  
C          
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
C             GAMMA >= 0.
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
C     F       (output) DOUBLE PRECISION array, dimension (LDF,N)
C             The leading M-by-N part of this array contains the state
C             feedback matrix F.
C
C     LDF     INTEGER
C             The leading dimension of the array F.  LDF >= max(1,M).
C
C     H       (output) DOUBLE PRECISION array, dimension (LDH,NP)
C             The leading N-by-NP part of this array contains the output
C             injection matrix H.
C
C     LDH     INTEGER
C             The leading dimension of the array H.  LDH >= max(1,N).
C
C     X       (output) DOUBLE PRECISION array, dimension (LDX,N)
C             The leading N-by-N part of this array contains the matrix
C             X, solution of the X-Riccati equation.
C
C     LDX     INTEGER
C             The leading dimension of the array X.  LDX >= max(1,N).
C
C     Y       (output) DOUBLE PRECISION array, dimension (LDY,N)
C             The leading N-by-N part of this array contains the matrix
C             Y, solution of the Y-Riccati equation.
C
C     LDY     INTEGER
C             The leading dimension of the array Y.  LDY >= max(1,N).
C  
C     XYCOND  (output) DOUBLE PRECISION array, dimension (2)
C             XYCOND(1) contains an estimate of the reciprocal condition
C                       number of the X-Riccati equation;
C             XYCOND(2) contains an estimate of the reciprocal condition
C                       number of the Y-Riccati equation.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension max(2*max(N,M-NCON,NP-NMEAS),N*N)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) contains the optimal
C             LDWORK.
C
C     LDWORK  INTEGER
C             The dimension of the array DWORK.
C             LDWORK >= max(1,M*M + max(2*M1,3*N*N +
C                                       max(N*M,10*N*N+12*N+5)),
C                           NP*NP + max(2*NP1,3*N*N +
C                                       max(N*NP,10*N*N+12*N+5))),
C             where M1 = M - M2 and NP1 = NP - NP2.
C             For good performance, LDWORK must generally be larger.
C             Denoting Q = MAX(M1,M2,NP1,NP2), an upper bound is
C             max(1,4*Q*Q+max(2*Q,3*N*N + max(2*N*Q,10*N*N+12*N+5))).
C
C     BWORK   LOGICAL array, dimension (2*N)
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  if the controller is not admissible (too small value
C                   of gamma);
C             = 2:  if the X-Riccati equation was not solved
C                   successfully (the controller is not admissible or
C                   there are numerical difficulties);
C             = 3:  if the Y-Riccati equation was not solved
C                   successfully (the controller is not admissible or
C                   there are numerical difficulties).
C
C     METHOD
C
C     The routine implements the Glover's and Doyle's formulas [1],[2]
C     modified as described in [3]. The X- and Y-Riccati equations
C     are solved with condition and accuracy estimates [4].
C
C     REFERENCES
C
C     [1] Glover, K. and Doyle, J.C.
C         State-space formulae for all stabilizing controllers that
C         satisfy an Hinf norm bound and relations to risk sensitivity.
C         Systems and Control Letters, vol. 11, pp. 167-172, 1988.
C
C     [2] Balas, G.J., Doyle, J.C., Glover, K., Packard, A., and
C         Smith, R.
C         mu-Analysis and Synthesis Toolbox.
C         The MathWorks Inc., Natick, Mass., 1995.
C
C     [3] Petkov, P.Hr., Gu, D.W., and Konstantinov, M.M.
C         Fortran 77 routines for Hinf and H2 design of continuous-time
C         linear control systems.
C         Rep. 98-14, Department of Engineering, Leicester University,
C         Leicester, U.K., 1998.
C
C     [4] Petkov, P.Hr., Konstantinov, M.M., and Mehrmann, V.
C         DGRSVX and DMSRIC: Fortan 77 subroutines for solving
C         continuous-time matrix algebraic Riccati equations with
C         condition and accuracy estimates.
C         Preprint SFB393/98-16, Fak. f. Mathematik, Tech. Univ.
C         Chemnitz, May 1998.
C
C     NUMERICAL ASPECTS
C
C     The precision of the solution of the matrix Riccati equations
C     can be controlled by the values of the condition numbers
C     XYCOND(1) and XYCOND(2) of these equations.
C
C     FURTHER COMMENTS
C
C     The Riccati equations are solved by the Schur approach 
C     implementing condition and accuracy estimates.
C
C     CONTRIBUTORS
C
C     P.Hr. Petkov, D.W. Gu and M.M. Konstantinov, October 1998.
C
C     REVISIONS
C
C     V. Sima, Research Institute for Informatics, Bucharest, May 1999,
C     Sept. 1999.
C
C     KEYWORDS
C
C     Algebraic Riccati equation, H-infinity optimal control, robust
C     control.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
C
C     .. Scalar Arguments ..
      INTEGER            INFO, LDA, LDB, LDC, LDD, LDF, LDH, LDWORK,
     $                   LDX, LDY, M, N, NCON, NMEAS, NP
      DOUBLE PRECISION   GAMMA
C     ..
C     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), B( LDB, * ), C( LDC, * ),
     $                   D( LDD, * ), DWORK( * ),  F( LDF, * ),
     $                   H( LDH, * ), X( LDX, * ), XYCOND( 2 ),
     $                   Y( LDY, * )
      LOGICAL            BWORK( * )
C      
C     ..
C     .. Local Scalars ..
      INTEGER            INFO2, IW2, IWA, IWG, IWI, IWQ, IWR, IWRK, IWS, 
     $                   IWT, IWV, LWAMAX, M1, M2, MINWRK, N2, ND1, ND2,
     $                   NN, NP1, NP2
      DOUBLE PRECISION   ANORM, EPS, FERR, RCOND, SEP
C     ..
C     .. External Functions ..
C
      DOUBLE PRECISION   DLAMCH, DLANSY
      EXTERNAL           DLAMCH, DLANSY
C     ..
C     .. External Subroutines ..
      EXTERNAL           DGEMM, DLACPY, DLASET, DSYCON, DSYMM, DSYRK,
     $                   DSYTRF, DSYTRI, MB01RU, MB01RX, SB02RD, XERBLA
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX
C     ..
C     .. Executable Statements ..      
C
C     Decode and Test input parameters.
C
      M1  = M - NCON
      M2  = NCON
      NP1 = NP - NMEAS
      NP2 = NMEAS
      NN  = N*N
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
      ELSE IF( GAMMA.LT.ZERO ) THEN
         INFO = -6
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -8
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -10  
      ELSE IF( LDC.LT.MAX( 1, NP ) ) THEN
         INFO = -12
      ELSE IF( LDD.LT.MAX( 1, NP ) ) THEN
         INFO = -14
      ELSE IF( LDF.LT.MAX( 1, M ) ) THEN
         INFO = -16
      ELSE IF( LDH.LT.MAX( 1, N ) ) THEN
         INFO = -18 
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN 
         INFO = -20
      ELSE IF( LDY.LT.MAX( 1, N ) ) THEN
         INFO = -22
      ELSE
C
C        Compute workspace.
C
         MINWRK = MAX( 1, M*M + MAX( 2*M1, 3*NN +
     $                               MAX( N*M, 10*NN + 12*N + 5 ) ),
     $                  NP*NP + MAX( 2*NP1, 3*NN +
     $                               MAX( N*NP, 10*NN + 12*N + 5 ) ) )
         IF( LDWORK.LT.MINWRK )
     $      INFO = -26
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB10QD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( N.EQ.0 .OR. M.EQ.0 .OR. NP.EQ.0 .OR. M1.EQ.0 .OR. M2.EQ.0
     $    .OR. NP1.EQ.0 .OR. NP2.EQ.0 ) THEN
         XYCOND( 1 ) = ONE
         XYCOND( 2 ) = ONE
         DWORK( 1 )  = ONE
         RETURN
      END IF
      ND1 = NP1 - M2
      ND2 = M1 - NP2
      N2  = 2*N
C
C     Get the machine precision.
C
      EPS = DLAMCH( 'Epsilon' )
C
C     Workspace usage.
C
      IWA = M*M + 1
      IWQ = IWA + NN
      IWG = IWQ + NN
      IW2 = IWG + NN
C
C     Compute |D1111'||D1111 D1112| - gamma^2*Im1 .
C             |D1112'| 
C
      CALL DLASET( 'L', M1, M1, ZERO, -GAMMA*GAMMA, DWORK, M )
      IF( ND1.GT.0 )
     $   CALL DSYRK( 'L', 'T', M1, ND1, ONE, D, LDD, ONE, DWORK, M )
C
C     Compute inv(|D1111'|*|D1111 D1112| - gamma^2*Im1) .
C                 |D1112'| 
C
      IWRK = IWA
      ANORM = DLANSY( 'I', 'L', M1, DWORK, M, DWORK( IWRK ) )
      CALL DSYTRF( 'L', M1, DWORK, M, IWORK, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 1
         RETURN
      END IF
C
      LWAMAX = INT( DWORK( IWRK ) ) + IWRK - 1
      CALL DSYCON( 'L', M1, DWORK, M, IWORK, ANORM, RCOND,
     $             DWORK( IWRK ), IWORK( M1+1 ), INFO2 )
      IF( RCOND.LT.EPS ) THEN
         INFO = 1
         RETURN
      END IF
C
C     Compute inv(R) block by block.
C
      CALL DSYTRI( 'L', M1, DWORK, M, IWORK, DWORK( IWRK ), INFO2 )
C
C     Compute -|D1121 D1122|*inv(|D1111'|*|D1111 D1112| - gamma^2*Im1) .
C                                |D1112'|
C 
      CALL DSYMM( 'R', 'L', M2, M1, -ONE, DWORK, M, D( ND1+1, 1 ), LDD,
     $            ZERO, DWORK( M1+1 ), M )
C
C     Compute |D1121 D1122|*inv(|D1111'|*|D1111 D1112| - 
C                               |D1112'|                                
C
C                  gamma^2*Im1)*|D1121'| + Im2 .
C                               |D1122'|                                
C
      CALL DLASET( 'Lower', M2, M2, ZERO, ONE, DWORK( M1*(M+1)+1 ), M )
      CALL MB01RX( 'Right', 'Lower', 'Transpose', M2, M1, ONE, -ONE,
     $             DWORK( M1*(M+1)+1 ), M, D( ND1+1, 1 ), LDD, 
     $             DWORK( M1+1 ), M, INFO2 )
C
C     Compute D11'*C1 .
C
      CALL DGEMM( 'T', 'N', M1, N, NP1, ONE, D, LDD, C, LDC, ZERO,
     $            DWORK( IW2 ), M )
C
C     Compute D1D'*C1 .
C
      CALL DLACPY( 'Full', M2, N, C( ND1+1, 1 ), LDC, DWORK( IW2+M1 ),
     $             M )
C
C     Compute inv(R)*D1D'*C1 in F .
C
      CALL DSYMM( 'L', 'L', M, N, ONE, DWORK, M, DWORK( IW2 ), M, ZERO,
     $            F, LDF )
C
C     Compute Ax = A - B*inv(R)*D1D'*C1 . 
C
      CALL DLACPY( 'Full', N, N, A, LDA, DWORK( IWA ), N )
      CALL DGEMM( 'N', 'N', N, N, M, -ONE, B, LDB, F, LDF, ONE,
     $            DWORK( IWA ), N )
C
C     Compute Cx = C1'*C1 - C1'*D1D*inv(R)*D1D'*C1 .
C
      IF( ND1.EQ.0 ) THEN
         CALL DLASET( 'L', N, N, ZERO, ZERO, DWORK( IWQ ), N )
      ELSE
         CALL DSYRK( 'L', 'T', N, NP1, ONE, C, LDC, ZERO,
     $               DWORK( IWQ ), N )
         CALL MB01RX( 'Left', 'Lower', 'Transpose', N, M, ONE, -ONE, 
     $                DWORK( IWQ ), N, DWORK( IW2 ), M, F, LDF, INFO2 )
      END IF
C
C     Compute Dx = B*inv(R)*B' .
C
      IWRK = IW2
      CALL MB01RU( 'Lower', 'NoTranspose', N, M, ZERO, ONE,
     $             DWORK( IWG ), N, B, LDB, DWORK, M, DWORK( IWRK ),
     $             M*N, INFO2 )
C
C     Solution of the Riccati equation Ax'*X + X*Ax + Cx - X*Dx*X = 0 .
C     Workspace:  need   M*M + 13*N*N + 12*N + 5;
C                 prefer larger.
C
      IWT  = IW2
      IWV  = IWT + NN
      IWR  = IWV + NN
      IWI  = IWR + N2
      IWS  = IWI + N2
      IWRK = IWS + 4*NN
C
      CALL SB02RD( 'All', 'Continuous', 'NotUsed', 'NoTranspose', 
     $             'Lower', 'GeneralScaling', 'Stable', 'NotFactored',
     $             'Original', N, DWORK( IWA ), N, DWORK( IWT ), N, 
     $             DWORK( IWV ), N, DWORK( IWG ), N, DWORK( IWQ ), N,
     $             X, LDX, SEP, XYCOND( 1 ), FERR, DWORK( IWR ),
     $             DWORK( IWI ), DWORK( IWS ), N2, IWORK, DWORK( IWRK ),
     $             LDWORK-IWRK+1, BWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 2
         RETURN
      END IF
C
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Compute F = -inv(R)*|D1D'*C1 + B'*X| .
C
      IWRK = IW2
      CALL DGEMM( 'T', 'N', M, N, N, ONE, B, LDB, X, LDX, ZERO,
     $            DWORK( IWRK ), M )
      CALL DSYMM( 'L', 'L', M, N, -ONE, DWORK, M, DWORK( IWRK ), M,
     $            -ONE, F, LDF )
C
C     Workspace usage.
C
      IWA = NP*NP + 1
      IWQ = IWA + NN
      IWG = IWQ + NN
      IW2 = IWG + NN
C
C     Compute |D1111|*|D1111' D1121'| - gamma^2*Inp1 .
C             |D1121| 
C
      CALL DLASET( 'U', NP1, NP1, ZERO, -GAMMA*GAMMA, DWORK, NP )
      IF( ND2.GT.0 )
     $   CALL DSYRK( 'U', 'N', NP1, ND2, ONE, D, LDD, ONE, DWORK, NP )
C
C     Compute inv(|D1111|*|D1111' D1121'| - gamma^2*Inp1) .
C                 |D1121| 
C
      IWRK  = IWA
      ANORM = DLANSY( 'I', 'U', NP1, DWORK, NP, DWORK( IWRK ) )
      CALL DSYTRF( 'U', NP1, DWORK, NP, IWORK, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 1
         RETURN
      END IF
C
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
      CALL DSYCON( 'U', NP1, DWORK, NP, IWORK, ANORM, RCOND,
     $             DWORK( IWRK ), IWORK( NP1+1 ), INFO2 )
      IF( RCOND.LT.EPS ) THEN
         INFO = 1
         RETURN
      END IF  
C
C     Compute inv(RT) .
C
      CALL DSYTRI( 'U', NP1, DWORK, NP, IWORK, DWORK( IWRK ), INFO2 )
C
C     Compute -inv(|D1111||D1111' D1121'| - gamma^2*Inp1)*|D1112| .
C                  |D1121|                                |D1122|
C 
      CALL DSYMM( 'L', 'U', NP1, NP2, -ONE, DWORK, NP, D( 1, ND2+1 ),
     $            LDD, ZERO, DWORK( NP1*NP+1 ), NP )
C
C     Compute [D1112' D1122']*inv(|D1111||D1111' D1121'| - 
C                                 |D1121|                              
C
C                gamma^2*Inp1)*|D1112| + Inp2 .
C                              |D1122|
C
      CALL DLASET( 'Full', NP2, NP2, ZERO, ONE, DWORK( NP1*(NP+1)+1 ), 
     $             NP )
      CALL MB01RX( 'Left', 'Upper', 'Transpose', NP2, NP1, ONE, -ONE,
     $             DWORK( NP1*(NP+1)+1 ), NP, D( 1, ND2+1 ), LDD,
     $             DWORK( NP1*NP+1 ), NP, INFO2 )
C
C     Compute B1*D11' .
C
      CALL DGEMM( 'N', 'T', N, NP1, M1, ONE, B, LDB, D, LDD, ZERO,
     $            DWORK( IW2 ), N )
C
C     Compute B1*DD1' .
C
      CALL DLACPY( 'Full', N, NP2, B( 1, ND2+1 ), LDB,
     $             DWORK( IW2+NP1*N ), N )
C
C     Compute B1*DD1'*inv(RT) in H .
C
      CALL DSYMM( 'R', 'U', N, NP, ONE, DWORK, NP, DWORK( IW2 ), N,
     $            ZERO, H, LDH )
C
C     Compute Ay = A - B1*DD1'*inv(RT)*C . 
C
      CALL DLACPY( 'Full', N, N, A, LDA, DWORK( IWA ), N )
      CALL DGEMM( 'N', 'N', N, N, NP, -ONE, H, LDH, C, LDC, ONE,
     $            DWORK( IWA ), N )
C
C     Compute Cy = B1*B1' - B1*DD1'*inv(RT)*DD1*B1' .
C
      IF( ND2.EQ.0 ) THEN
         CALL DLASET( 'U', N, N, ZERO, ZERO, DWORK( IWQ ), N )
      ELSE
         CALL DSYRK( 'U', 'N', N, M1, ONE, B, LDB, ZERO, DWORK( IWQ ),
     $               N )
         CALL MB01RX( 'Right', 'Upper', 'Transpose', N, NP, ONE, -ONE,
     $                DWORK( IWQ ), N, H, LDH, DWORK( IW2 ), N, INFO2 )
      END IF
C
C     Compute Dy = C'*inv(RT)*C .
C
      IWRK = IW2
      CALL MB01RU( 'Upper', 'Transpose', N, NP, ZERO, ONE, DWORK( IWG ),
     $             N, C, LDC, DWORK, NP, DWORK( IWRK), N*NP, INFO2 )
C
C     Solution of the Riccati equation Ay*Y + Y*Ay' + Cy - Y*Dy*Y = 0 .
C     Workspace:  need   NP*NP + 13*N*N + 12*N + 5;
C                 prefer larger.
C
      IWT  = IW2
      IWV  = IWT + NN
      IWR  = IWV + NN
      IWI  = IWR + N2
      IWS  = IWI + N2
      IWRK = IWS + 4*NN
C
      CALL SB02RD( 'All', 'Continuous', 'NotUsed', 'Transpose', 
     $             'Upper', 'GeneralScaling', 'Stable', 'NotFactored',
     $             'Original', N, DWORK( IWA ), N, DWORK( IWT ), N, 
     $             DWORK( IWV ), N, DWORK( IWG ), N, DWORK( IWQ ), N,
     $             Y, LDY, SEP, XYCOND( 2 ), FERR, DWORK( IWR ),
     $             DWORK( IWI ), DWORK( IWS ), N2, IWORK, DWORK( IWRK ),
     $             LDWORK-IWRK+1, BWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 3
         RETURN
      END IF
C
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Compute H = -|B1*DD1' + Y*C'|*inv(RT) .
C
      IWRK = IW2
      CALL DGEMM( 'N', 'T', N, NP, N, ONE, Y, LDY, C, LDC, ZERO,
     $            DWORK( IWRK ), N )
      CALL DSYMM( 'R', 'U', N, NP, -ONE, DWORK, NP, DWORK( IWRK ), N,
     $            -ONE, H, LDH )
C
      DWORK( 1 ) = DBLE( LWAMAX )
      RETURN
C *** Last line of SB10QD ***
      END
