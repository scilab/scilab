      SUBROUTINE SB10RD( N, M, NP, NCON, NMEAS, GAMMA, A, LDA, B, LDB, 
     $                   C, LDC, D, LDD, F, LDF, H, LDH, TU, LDTU, TY, 
     $                   LDTY, X, LDX, Y, LDY, AK, LDAK, BK, LDBK, CK,
     $                   LDCK, DK, LDDK, IWORK, DWORK, LDWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C     
C     PURPOSE
C
C     To compute the matrices of an H-infinity (sub)optimal controller
C
C              | AK | BK |
C          K = |----|----|,
C              | CK | DK |
C
C     from the state feedback matrix F and output injection matrix H as
C     determined by the SLICOT Library routine SB10QD.
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
C             The number of control inputs (M2).  M >= NCON >= 0.
C             NP-NMEAS >= NCON.
C
C     NMEAS   (input) INTEGER
C             The number of measurements (NP2).  NP >= NMEAS >= 0.
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
C     F       (input) DOUBLE PRECISION array, dimension (LDF,N)
C             The leading M-by-N part of this array must contain the
C             state feedback matrix F.
C
C     LDF     INTEGER
C             The leading dimension of the array F.  LDF >= max(1,M).
C
C     H       (input) DOUBLE PRECISION array, dimension (LDH,NP)
C             The leading N-by-NP part of this array must contain the
C             output injection matrix H.
C
C     LDH     INTEGER
C             The leading dimension of the array H.  LDH >= max(1,N).
C
C     TU      (input) DOUBLE PRECISION array, dimension (LDTU,M2)
C             The leading M2-by-M2 part of this array must contain the
C             control transformation matrix TU, as obtained by the
C             SLICOT Library routine SB10PD.
C
C     LDTU    INTEGER
C             The leading dimension of the array TU.  LDTU >= max(1,M2).
C
C     TY      (input) DOUBLE PRECISION array, dimension (LDTY,NP2)
C             The leading NP2-by-NP2 part of this array must contain the
C             measurement transformation matrix TY, as obtained by the
C             SLICOT Library routine SB10PD.
C
C     LDTY    INTEGER
C             The leading dimension of the array TY.
C             LDTY >= max(1,NP2).
C
C     X       (input) DOUBLE PRECISION array, dimension (LDX,N)
C             The leading N-by-N part of this array must contain the
C             matrix X, solution of the X-Riccati equation, as obtained
C             by the SLICOT Library routine SB10QD.
C
C     LDX     INTEGER
C             The leading dimension of the array X.  LDX >= max(1,N).
C
C     Y       (input) DOUBLE PRECISION array, dimension (LDY,N)
C             The leading N-by-N part of this array must contain the
C             matrix Y, solution of the Y-Riccati equation, as obtained
C             by the SLICOT Library routine SB10QD.
C
C     LDY     INTEGER
C             The leading dimension of the array Y.  LDY >= max(1,N).
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
C     Workspace
C
C     IWORK   INTEGER array, dimension (LIWORK), where
C             LIWORK = max(2*(max(NP,M)-M2-NP2,M2,N),NP2)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) contains the optimal
C             LDWORK.
C
C     LDWORK  INTEGER
C             The dimension of the array DWORK.
C             LDWORK >= max(1, M2*NP2 + NP2*NP2 + M2*M2 + 
C                           max(D1*D1 + max(2*D1, (D1+D2)*NP2),
C                               D2*D2 + max(2*D2, D2*M2), 3*N, 
C                               N*(2*NP2 + M2) + 
C                               max(2*N*M2, M2*NP2 + 
C                                           max(M2*M2+3*M2, NP2*(2*NP2+
C                                                  M2+max(NP2,N))))))
C             where D1 = NP1 - M2, D2 = M1 - NP2, 
C                  NP1 = NP - NP2, M1 = M - M2.
C             For good performance, LDWORK must generally be larger.
C             Denoting Q = max(M1,M2,NP1,NP2), an upper bound is
C             max( 1, Q*(3*Q + 3*N + max(2*N, 4*Q + max(Q, N)))).
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  if the controller is not admissible (too small value
C                   of gamma);
C             = 2:  if the determinant of Im2 + Tu*D11HAT*Ty*D22 is zero.
C
C     METHOD
C
C     The routine implements the Glover's and Doyle's formulas [1],[2].
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
C     NUMERICAL ASPECTS
C
C     The accuracy of the result depends on the condition numbers of the
C     input and output transformations.
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
C  *********************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0 )
C     ..
C     .. Scalar Arguments ..
      INTEGER            INFO, LDA, LDAK, LDB, LDBK, LDC, LDCK, LDD,
     $                   LDDK, LDF, LDH, LDTU, LDTY, LDWORK, LDX, LDY,
     $                   M, N, NCON, NMEAS, NP
      DOUBLE PRECISION   GAMMA
C     ..
C     .. Array Arguments ..
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), AK( LDAK, * ), B( LDB, * ),
     $                   BK( LDBK, * ), C( LDC, * ), CK( LDCK, * ),
     $                   D( LDD, * ), DK( LDDK, * ), DWORK( * ),
     $                   F( LDF, * ), H( LDH, * ), TU( LDTU, * ),
     $                   TY( LDTY, * ), X( LDX, * ), Y( LDY, * )
C     ..
C     .. Local Scalars ..
      INTEGER            I, ID11, ID12, ID21, IJ, INFO2, IW1, IW2, IW3, 
     $                   IW4, IWB, IWC, IWRK, J, LWAMAX, M1, M2, MINWRK,
     $                   ND1, ND2, NP1, NP2
      DOUBLE PRECISION   ANORM, EPS, RCOND 
C     ..
C     .. External Functions ..
      DOUBLE PRECISION   DLAMCH, DLANGE, DLANSY
      EXTERNAL           DLAMCH, DLANGE, DLANSY
C     ..
C     .. External Subroutines ..
      EXTERNAL           DGECON, DGEMM, DGETRF, DGETRI, DGETRS, DLACPY,
     $                   DLASET, DPOTRF, DSYCON, DSYRK, DSYTRF, DSYTRS,
     $                   DTRMM, MA02AD, MB01RX, XERBLA
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
      ELSE IF( LDTU.LT.MAX( 1, M2 ) ) THEN
         INFO = -20
      ELSE IF( LDTY.LT.MAX( 1, NP2 ) ) THEN
         INFO = -22
      ELSE IF( LDX.LT.MAX( 1, N ) ) THEN
         INFO = -24
      ELSE IF( LDY.LT.MAX( 1, N ) ) THEN
         INFO = -26
      ELSE IF( LDAK.LT.MAX( 1, N ) ) THEN
         INFO = -28
      ELSE IF( LDBK.LT.MAX( 1, N ) ) THEN
         INFO = -30
      ELSE IF( LDCK.LT.MAX( 1, M2 ) ) THEN
         INFO = -32
      ELSE IF( LDDK.LT.MAX( 1, M2 ) ) THEN
         INFO = -34
      ELSE
C
C        Compute workspace.
C
         ND1 = NP1 - M2
         ND2 = M1 - NP2
         MINWRK = MAX( 1, M2*NP2 + NP2*NP2 + M2*M2 + 
     $                 MAX( ND1*ND1 + MAX( 2*ND1, ( ND1 + ND2 )*NP2 ),
     $                      ND2*ND2 + MAX( 2*ND2, ND2*M2 ), 3*N, 
     $                      N*( 2*NP2 + M2 ) + 
     $                         MAX( 2*N*M2, M2*NP2 + 
     $                                MAX( M2*M2 + 3*M2, NP2*( 2*NP2 +
     $                                     M2 + MAX( NP2, N ) ) ) ) ) )
         IF( LDWORK.LT.MINWRK )
     $      INFO = -37
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB10RD', -INFO )
         RETURN
      END IF
C
C     Quick return if possible.
C
      IF( N.EQ.0 .OR. M.EQ.0 .OR. NP.EQ.0 .OR. M1.EQ.0 .OR. M2.EQ.0
     $    .OR. NP1.EQ.0 .OR. NP2.EQ.0 ) THEN
         DWORK( 1 ) = ONE
         RETURN
      END IF
C
C     Get the machine precision.
C
      EPS = DLAMCH( 'Epsilon' )
C
C     Workspace usage.
C
      ID11 = 1
      ID21 = ID11 + M2*NP2
      ID12 = ID21 + NP2*NP2         
      IW1  = ID12 + M2*M2
      IW2  = IW1  + ND1*ND1
      IW3  = IW2  + ND1*NP2
      IWRK = IW2
C
C     Set D11HAT := -D1122 .
C
      IJ = ID11
      DO 20 J = 1, NP2
         DO 10 I = 1, M2
            DWORK( IJ ) = -D( ND1+I, ND2+J )
            IJ = IJ + 1
   10    CONTINUE
   20 CONTINUE   
C     
C     Set D21HAT := Inp2 .
C
      CALL DLASET( 'Upper', NP2, NP2, ZERO, ONE, DWORK( ID21 ), NP2 )
C
C     Set D12HAT := Im2 .
C
      CALL DLASET( 'Lower', M2, M2, ZERO, ONE, DWORK( ID12 ), M2 )
C
C     Compute D11HAT, D21HAT, D12HAT .
C
      IF( ND1.GT.0 ) THEN
         IF( ND2.EQ.0 ) THEN
C
C           Compute D21HAT'*D21HAT = Inp2 - D1112'*D1112/gamma^2 .
C            
            CALL DSYRK( 'U', 'T', NP2, ND1, -ONE/GAMMA**2, D, LDD, ONE,
     $                  DWORK( ID21 ), NP2 )
         ELSE
C
C           Compute gdum = gamma^2*Ind1 - D1111*D1111' .
C
            CALL DLASET( 'U', ND1, ND1, ZERO, GAMMA**2, DWORK( IW1 ), 
     $                   ND1 )
            CALL DSYRK( 'U', 'N', ND1, ND2, -ONE, D, LDD, ONE,
     $                  DWORK( IW1 ), ND1 )
            ANORM = DLANSY( 'I', 'U', ND1, DWORK( IW1 ), ND1, 
     $                      DWORK( IWRK ) )
            CALL DSYTRF( 'U', ND1, DWORK( IW1 ), ND1, IWORK,
     $                   DWORK( IWRK ), LDWORK-IWRK+1, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 1
               RETURN
            END IF
            LWAMAX = INT( DWORK( IWRK ) ) + IWRK - 1
            CALL DSYCON( 'U', ND1, DWORK( IW1 ), ND1, IWORK, ANORM,
     $                   RCOND, DWORK( IWRK ), IWORK( ND1+1 ), INFO2 )
C
C           Return if the matrix is singular to working precision.
C
            IF( RCOND.LT.EPS ) THEN
               INFO = 1
               RETURN
            END IF
C
C           Compute inv(gdum)*D1112 .
C
            CALL DLACPY( 'Full', ND1, NP2, D( 1, ND2+1 ), LDD,
     $                   DWORK( IW2 ), ND1 )
            CALL DSYTRS( 'U', ND1, NP2, DWORK( IW1 ), ND1, IWORK,
     $                   DWORK( IW2 ), ND1, INFO2 )
C
C           Compute D11HAT = -D1121*D1111'*inv(gdum)*D1112 - D1122 .
C
            CALL DGEMM( 'T', 'N', ND2, NP2, ND1, ONE, D, LDD, 
     $                  DWORK( IW2 ), ND1, ZERO, DWORK( IW3 ), ND2 )
            CALL DGEMM( 'N', 'N', M2, NP2, ND2, -ONE, D( ND1+1, 1 ),
     $                  LDD, DWORK( IW3 ), ND2, ONE, DWORK( ID11 ), M2 )
C
C           Compute D21HAT'*D21HAT = Inp2 - D1112'*inv(gdum)*D1112 .
C
            CALL MB01RX( 'Left', 'Upper', 'Transpose', NP2, ND1, ONE, 
     $                   -ONE, DWORK( ID21 ), NP2, D( 1, ND2+1 ), LDD, 
     $                   DWORK( IW2 ), ND1, INFO2 )
C
            IW2  = IW1 + ND2*ND2
            IWRK = IW2
C
C           Compute gdum = gamma^2*Ind2 - D1111'*D1111 .
C
            CALL DLASET( 'L', ND2, ND2, ZERO, GAMMA**2, DWORK( IW1 ),
     $                   ND2 )
            CALL DSYRK( 'L', 'T', ND2, ND1, -ONE, D, LDD, ONE,
     $                  DWORK( IW1 ), ND2 )
            ANORM = DLANSY( 'I', 'L', ND2, DWORK( IW1 ), ND2,
     $                      DWORK( IWRK ) )
            CALL DSYTRF( 'L', ND2, DWORK( IW1 ), ND2, IWORK,
     $                   DWORK( IWRK ), LDWORK-IWRK+1, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 1
               RETURN
            END IF
            LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
            CALL DSYCON( 'L', ND2, DWORK( IW1 ), ND2, IWORK, ANORM,
     $                   RCOND, DWORK( IWRK ), IWORK( ND2+1 ), INFO2 )
C
C           Return if the matrix is singular to working precision.
C
            IF( RCOND.LT.EPS ) THEN
               INFO = 1
               RETURN
            END IF
C
C           Compute inv(gdum)*D1121' .
C
            CALL MA02AD( 'Full', M2, ND2, D( ND1+1, 1 ), LDD,
     $                   DWORK( IW2 ), ND2 )
            CALL DSYTRS( 'L', ND2, M2, DWORK( IW1 ), ND2, IWORK,
     $                   DWORK( IW2 ), ND2, INFO2 )
C
C           Compute D12HAT*D12HAT' = Im2 - D1121*inv(gdum)*D1121' .
C
            CALL MB01RX( 'Left', 'Lower', 'NoTranspose', M2, ND2, ONE,
     $                   -ONE, DWORK( ID12 ), M2, D( ND1+1, 1 ), LDD,
     $                   DWORK( IW2 ), ND2, INFO2 )
         END IF
      ELSE
         IF( ND2.GT.0 ) THEN
C
C           Compute D12HAT*D12HAT' = Im2 - D1121*D1121'/gamma^2 .
C            
            CALL DSYRK( 'L', 'N', M2, ND2, -ONE/GAMMA**2, D, LDD, ONE,
     $                  DWORK( ID12 ), M2 )
         END IF
      END IF
C
C     Compute D21HAT using Cholesky decomposition.
C
      CALL DPOTRF( 'U', NP2, DWORK( ID21 ), NP2, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 1
         RETURN
      END IF
C
C     Compute D12HAT using Cholesky decomposition.
C
      CALL DPOTRF( 'L', M2, DWORK( ID12 ), M2, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 1
         RETURN
      END IF
C             _
C     Compute Z = In - Y*X/gamma^2 and its LU factorization in AK .
C
      IWRK = IW1
      CALL DLASET( 'Full', N, N, ZERO, ONE, AK, LDAK )
      CALL DGEMM( 'N', 'N', N, N, N, -ONE/GAMMA**2, Y, LDY, X, LDX,
     $            ONE, AK, LDAK )
      ANORM = DLANGE( '1', N, N, AK, LDAK, DWORK( IWRK ) )
      CALL DGETRF( N, N, AK, LDAK, IWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 1
         RETURN
      END IF
      CALL DGECON( '1', N, AK, LDAK, ANORM, RCOND, DWORK( IWRK ),
     $             IWORK( N+1 ), INFO )
C
C     Return if the matrix is singular to working precision.
C
      IF( RCOND.LT.EPS ) THEN
         INFO = 1
         RETURN
      END IF     
C
      IWB = IW1
      IWC = IWB + N*NP2
      IW1 = IWC + ( M2 + NP2 )*N
      IW2 = IW1 + N*M2
C
C     Compute C2' + F12' in BK .
C
      DO 40 J = 1, N
         DO 30 I = 1, NP2
            BK( J, I ) = C( NP1 + I, J ) + F( ND2 + I, J )
   30    CONTINUE
   40 CONTINUE
C                                                          _
C     Compute the transpose of (C2 + F12)*Z , with Z = inv(Z) .
C
      CALL DGETRS( 'Transpose', N, NP2, AK, LDAK, IWORK, BK, LDBK, 
     $             INFO2 )
C
C     Compute the transpose of F2*Z .
C
      CALL MA02AD( 'Full', M2, N, F( M1+1, 1 ), LDF, DWORK( IW1 ), N )
      CALL DGETRS( 'Transpose', N, M2, AK, LDAK, IWORK, DWORK( IW1 ), N,
     $             INFO2 )
C
C     Compute the transpose of C1HAT = F2*Z - D11HAT*(C2 + F12)*Z .
C 
      CALL DGEMM( 'N', 'T', N, M2, NP2, -ONE, BK, LDBK, DWORK( ID11 ),
     $            M2, ONE, DWORK( IW1 ), N )
C
C     Compute CHAT .
C
      CALL DGEMM( 'N', 'T', M2, N, M2, ONE, TU, LDTU, DWORK( IW1 ), N,
     $            ZERO, DWORK( IWC ), M2+NP2 )
      CALL MA02AD( 'Full', N, NP2, BK, LDBK, DWORK( IWC+M2 ), M2+NP2 )
      CALL DTRMM( 'L', 'U', 'N', 'N', NP2, N, -ONE, DWORK( ID21 ), NP2,
     $            DWORK( IWC+M2 ), M2+NP2 )
C
C     Compute B2 + H12 .
C
      IJ = IW2
      DO 60 J = 1, M2
         DO 50 I = 1, N
            DWORK( IJ ) = B( I, M1 + J ) + H( I, ND1 + J )
            IJ = IJ + 1
   50    CONTINUE
   60 CONTINUE
C
C     Compute A + HC in AK .
C
      CALL DLACPY( 'Full', N, N, A, LDA, AK, LDAK )
      CALL DGEMM( 'N', 'N', N, N, NP, ONE, H, LDH, C, LDC, ONE, AK, 
     $            LDAK )
C
C     Compute AHAT = A + HC + (B2 + H12)*C1HAT in AK .
C
      CALL DGEMM( 'N', 'T', N, N, M2, ONE, DWORK( IW2 ), N,
     $            DWORK( IW1 ), N, ONE, AK, LDAK )
C
C     Compute B1HAT = -H2 + (B2 + H12)*D11HAT in BK .
C
      CALL DLACPY( 'Full', N, NP2, H( 1, NP1+1 ), LDH, BK, LDBK )
      CALL DGEMM( 'N', 'N', N, NP2, M2, ONE, DWORK( IW2 ), N,
     $            DWORK( ID11 ), M2, -ONE, BK, LDBK )
C
C     Compute the first block of BHAT, BHAT1 .
C
      CALL DGEMM( 'N', 'N', N, NP2, NP2, ONE, BK, LDBK, TY, LDTY, ZERO,
     $            DWORK( IWB ), N )
C
C     Compute Tu*D11HAT .
C
      CALL DGEMM( 'N', 'N', M2, NP2, M2, ONE, TU, LDTU, DWORK( ID11 ),
     $            M2, ZERO, DWORK( IW1 ), M2 )
C
C     Compute Tu*D11HAT*Ty in DK .
C
      CALL DGEMM( 'N', 'N', M2, NP2, NP2, ONE, DWORK( IW1 ), M2, TY,
     $            LDTY, ZERO, DK, LDDK )
C
C     Compute P = Im2 + Tu*D11HAT*Ty*D22 and its condition.
C
      IW2  = IW1 + M2*NP2
      IWRK = IW2 + M2*M2
      CALL DLASET( 'Full', M2, M2, ZERO, ONE, DWORK( IW2 ), M2 )
      CALL DGEMM( 'N', 'N', M2, M2, NP2, ONE, DK, LDDK,
     $            D( NP1+1, M1+1 ), LDD, ONE, DWORK( IW2 ), M2 )
      ANORM = DLANGE( '1', M2, M2, DWORK( IW2 ), M2, DWORK( IWRK ) )
      CALL DGETRF( M2, M2, DWORK( IW2 ), M2, IWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 2
         RETURN
      END IF
      CALL DGECON( '1', M2, DWORK( IW2 ), M2, ANORM, RCOND,
     $             DWORK( IWRK ), IWORK( M2+1 ), INFO2 )
C
C     Return if the matrix is singular to working precision.
C
      IF( RCOND.LT.EPS ) THEN
         INFO = 2
         RETURN
      END IF
C
C     Find the controller matrix CK, CK = inv(P)*CHAT(1:M2,:) .
C
      CALL DLACPY( 'Full', M2, N, DWORK( IWC ), M2+NP2, CK, LDCK )
      CALL DGETRS( 'NoTranspose', M2, N, DWORK( IW2 ), M2, IWORK, CK,
     $             LDCK, INFO2 )
C
C     Find the controller matrices AK, BK, and DK, exploiting the
C     special structure of the relations.
C
C     Compute Q = Inp2 + D22*Tu*D11HAT*Ty and its LU factorization.
C
      IW3  = IW2 + NP2*NP2
      IW4  = IW3 + NP2*M2
      IWRK = IW4 + NP2*NP2
      CALL DLASET( 'Full', NP2, NP2, ZERO, ONE, DWORK( IW2 ), NP2 )
      CALL DGEMM( 'N', 'N', NP2, NP2, M2, ONE, D( NP1+1, M1+1 ), LDD,
     $            DK, LDDK, ONE, DWORK( IW2 ), NP2 )
      CALL DGETRF( NP2, NP2, DWORK( IW2 ), NP2, IWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 2
         RETURN
      END IF
C
C     Compute A1 = inv(Q)*D22 and inv(Q) .
C
      CALL DLACPY( 'Full', NP2, M2, D( NP1+1, M1+1 ), LDD, DWORK( IW3 ),
     $             NP2 )     
      CALL DGETRS( 'NoTranspose', NP2, M2, DWORK( IW2 ), NP2, IWORK, 
     $             DWORK( IW3 ), NP2, INFO2 )
      CALL DGETRI( NP2, DWORK( IW2 ), NP2, IWORK, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Compute A2 = ( inv(Ty) - inv(Q)*inv(Ty) -
C                    A1*Tu*D11HAT )*inv(D21HAT) .
C
      CALL DLACPY( 'Full', NP2, NP2, TY, LDTY, DWORK( IW4 ), NP2 )	
      CALL DGETRF( NP2, NP2, DWORK( IW4 ), NP2, IWORK, INFO2 )
      CALL DGETRI( NP2, DWORK( IW4 ), NP2, IWORK, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
C
      CALL DLACPY( 'Full', NP2, NP2, DWORK( IW4 ), NP2, DWORK( IWRK ),
     $             NP2 )
      CALL DGEMM( 'N', 'N', NP2, NP2, NP2, -ONE, DWORK( IW2), NP2,
     $            DWORK( IWRK ), NP2, ONE, DWORK( IW4 ), NP2 )
      CALL DGEMM( 'N', 'N', NP2, NP2, M2, -ONE, DWORK( IW3), NP2,
     $            DWORK( IW1 ), M2, ONE, DWORK( IW4 ), NP2 )
      CALL DTRMM( 'R', 'U', 'N', 'N', NP2, NP2, ONE, DWORK( ID21 ), NP2,
     $            DWORK( IW4 ), NP2 )
C
C     Compute [ A1  A2 ]*CHAT .
C
      CALL DGEMM( 'N', 'N', NP2, N, M2+NP2, ONE, DWORK( IW3 ), NP2,
     $            DWORK( IWC ), M2+NP2, ZERO, DWORK( IWRK ), NP2 )
C
C     Compute AK := AHAT - BHAT1*[ A1  A2 ]*CHAT .
C
      CALL DGEMM( 'N', 'N', N, N, NP2, -ONE, DWORK( IWB ), N,
     $            DWORK( IWRK ), NP2, ONE, AK, LDAK )
C           
C     Compute BK := BHAT1*inv(Q) .
C
      CALL DGEMM( 'N', 'N', N, NP2, NP2, ONE, DWORK( IWB ), N,
     $            DWORK( IW2 ), NP2, ZERO, BK, LDBK )
C
C     Compute DK := Tu*D11HAT*Ty*inv(Q) .
C
      CALL DGEMM( 'N', 'N', M2, NP2, NP2, ONE, DK, LDDK, DWORK( IW2 ), 
     $            NP2, ZERO, DWORK( IW3 ), M2 )
      CALL DLACPY( 'Full', M2, NP2, DWORK( IW3 ), M2, DK, LDDK )
C
      DWORK( 1 ) = DBLE( LWAMAX )
      RETURN
C *** Last line of SB10RD ***
      END
