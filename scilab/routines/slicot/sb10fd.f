      SUBROUTINE SB10FD( N, M, NP, NCON, NMEAS, GAMMA, A, LDA, B, LDB, 
     $                   C, LDC, D, LDD, AK, LDAK, BK, LDBK, CK, LDCK,
     $                   DK, LDDK, RCOND, TOL, IWORK, DWORK, LDWORK,
     $                   BWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To compute the matrices of an H-infinity (sub)optimal n-state
C     controller
C
C              | AK | BK |
C          K = |----|----|,
C              | CK | DK |
C
C     using modified Glover's and Doyle's 1988 formulas, for the system
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
C     (A3) | A-j*omega*I  B2  | has full column rank for all omega,
C          |    C1        D12 |
C
C     (A4) | A-j*omega*I  B1  |  has full row rank for all omega.
C          |    C2        D21 |  
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
C     RCOND   (output) DOUBLE PRECISION array, dimension (4)
C             RCOND(1) contains the reciprocal condition number of the 
C                      control transformation matrix;
C             RCOND(2) contains the reciprocal condition number of the 
C                      measurement transformation matrix;
C             RCOND(3) contains an estimate of the reciprocal condition
C                      number of the X-Riccati equation;
C             RCOND(4) contains an estimate of the reciprocal condition
C                      number of the Y-Riccati equation.
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             Tolerance used for controlling the accuracy of the applied
C             transformations for computing the normalized form in
C             SLICOT Library routine SB10PD. Transformation matrices
C             whose reciprocal condition numbers are less than TOL are
C             not allowed. If TOL <= 0, then a default value equal to
C             sqrt(EPS) is used, where EPS is the relative machine
C             precision.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (LIWORK), where
C             LIWORK = max(2*max(N,M-NCON,NP-NMEAS,NCON),N*N)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) contains the optimal
C             LDWORK.
C
C     LDWORK  INTEGER
C             The dimension of the array DWORK.
C             LDWORK >= N*M + NP*(N+M) + M2*M2 + NP2*NP2 +
C                       max(1,LW1,LW2,LW3,LW4,LW5,LW6), where
C             LW1 = (N+NP1+1)*(N+M2) + max(3*(N+M2)+N+NP1,5*(N+M2)),
C             LW2 = (N+NP2)*(N+M1+1) + max(3*(N+NP2)+N+M1,5*(N+NP2)),
C             LW3 = M2 + NP1*NP1 + max(NP1*max(N,M1),3*M2+NP1,5*M2),
C             LW4 = NP2 + M1*M1 + max(max(N,NP1)*M1,3*NP2+M1,5*NP2),
C             LW5 = 2*N*N + N*(M+NP) + 
C                   max(1,M*M + max(2*M1,3*N*N+max(N*M,10*N*N+12*N+5)),
C                       NP*NP + max(2*NP1,3*N*N +
C                                   max(N*NP,10*N*N+12*N+5))),
C             LW6 = 2*N*N + N*(M+NP) +
C                   max(1, M2*NP2 + NP2*NP2 + M2*M2 + 
C                       max(D1*D1 + max(2*D1, (D1+D2)*NP2),
C                           D2*D2 + max(2*D2, D2*M2), 3*N, 
C                           N*(2*NP2 + M2) + 
C                           max(2*N*M2, M2*NP2 + 
C                                       max(M2*M2+3*M2, NP2*(2*NP2+
C                                              M2+max(NP2,N)))))),
C             with D1 = NP1 - M2, D2 = M1 - NP2, 
C                 NP1 = NP - NP2, M1 = M - M2.
C             For good performance, LDWORK must generally be larger.
C             Denoting Q = max(M1,M2,NP1,NP2), an upper bound is
C             2*Q*(3*Q+2*N)+max(1,(N+Q)*(N+Q+6),Q*(Q+max(N,Q,5)+1),
C               2*N*(N+2*Q)+max(1,4*Q*Q+
C                               max(2*Q,3*N*N+max(2*N*Q,10*N*N+12*N+5)),
C                                 Q*(3*N+3*Q+max(2*N,4*Q+max(N,Q))))). 
C
C     BWORK   LOGICAL array, dimension (2*N)
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  if the matrix | A-j*omega*I  B2  | had not full
C                                 |    C1        D12 |
C                   column rank in respect to the tolerance EPS;
C             = 2:  if the matrix | A-j*omega*I  B1  |  had not full row
C                                 |    C2        D21 |
C                   rank in respect to the tolerance EPS;
C             = 3:  if the matrix D12 had not full column rank in 
C                   respect to the tolerance TOL;
C             = 4:  if the matrix D21 had not full row rank in respect 
C                   to the tolerance TOL;
C             = 5:  if the singular value decomposition (SVD) algorithm
C                   did not converge (when computing the SVD of one of
C                   the matrices |A   B2 |, |A   B1 |, D12 or D21).
C                                |C1  D12|  |C2  D21|
C             = 6:  if the controller is not admissible (too small value
C                   of gamma);
C             = 7:  if the X-Riccati equation was not solved
C                   successfully (the controller is not admissible or
C                   there are numerical difficulties);
C             = 8:  if the Y-Riccati equation was not solved
C                   successfully (the controller is not admissible or
C                   there are numerical difficulties);
C             = 9:  if the determinant of Im2 + Tu*D11HAT*Ty*D22 is 
C                   zero [3].
C
C     METHOD
C
C     The routine implements the Glover's and Doyle's 1988 formulas [1],
C     [2] modified to improve the efficiency as described in [3].
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
C     NUMERICAL ASPECTS
C
C     The accuracy of the result depends on the condition numbers of the
C     input and output transformations and on the condition numbers of 
C     the two Riccati equations, as given by the values of RCOND(1),
C     RCOND(2), RCOND(3) and RCOND(4), respectively.
C
C     CONTRIBUTORS
C
C     P.Hr. Petkov, D.W. Gu and M.M. Konstantinov, October 1998.
C
C     REVISIONS
C
C     V. Sima, Research Institute for Informatics, Bucharest, May 1999,
C     Sept. 1999, Feb. 2000.
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
C     ..
C     .. Scalar Arguments ..
      INTEGER            INFO, LDA, LDAK, LDB, LDBK, LDC, LDCK, LDD,
     $                   LDDK, LDWORK, M, N, NCON, NMEAS, NP
      DOUBLE PRECISION   GAMMA, TOL
C     ..
C     .. Array Arguments ..
      LOGICAL            BWORK( * )
      INTEGER            IWORK( * )
      DOUBLE PRECISION   A( LDA, * ), AK( LDAK, * ), B( LDB, * ),
     $                   BK( LDBK, * ), C( LDC, * ), CK( LDCK, * ),
     $                   D( LDD, * ), DK( LDDK, * ), DWORK( * ),
     $                   RCOND( 4 )
C     ..
C     .. Local Scalars ..
      INTEGER            INFO2, IWC, IWD, IWF, IWH, IWRK, IWTU, IWTY,
     $                   IWX, IWY, LW1, LW2, LW3, LW4, LW5, LW6,
     $                   LWAMAX, M1, M2, MINWRK, ND1, ND2, NP1, NP2
      DOUBLE PRECISION   TOLL
C     ..
C     .. External Functions ..
      DOUBLE PRECISION   DLAMCH
      EXTERNAL           DLAMCH
C     ..
C     .. External Subroutines ..
      EXTERNAL           DLACPY, SB10PD, SB10QD, SB10RD, XERBLA
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          DBLE, INT, MAX, SQRT
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
      ELSE IF( LDAK.LT.MAX( 1, N ) ) THEN
         INFO = -16
      ELSE IF( LDBK.LT.MAX( 1, N ) ) THEN
         INFO = -18
      ELSE IF( LDCK.LT.MAX( 1, M2 ) ) THEN
         INFO = -20
      ELSE IF( LDDK.LT.MAX( 1, M2 ) ) THEN
         INFO = -22
      ELSE
C
C        Compute workspace.
C
         ND1 = NP1 - M2
         ND2 = M1 - NP2
         LW1 = ( N + NP1 + 1 )*( N + M2 ) + MAX( 3*( N + M2 ) + N + NP1,
     $                                           5*( N + M2 ) )
         LW2 = ( N + NP2 )*( N + M1 + 1 ) + MAX( 3*( N + NP2 ) + N + 
     $                                           M1, 5*( N + NP2 ) )
         LW3 = M2 + NP1*NP1 + MAX( NP1*MAX( N, M1 ), 3*M2 + NP1, 5*M2 )
         LW4 = NP2 + M1*M1  + MAX( MAX( N, NP1 )*M1, 3*NP2 + M1, 5*NP2 )
         LW5 = 2*N*N + N*( M + NP ) + 
     $         MAX( 1, M*M + MAX( 2*M1, 3*N*N + 
     $                            MAX( N*M, 10*N*N + 12*N + 5 ) ),
     $              NP*NP + MAX( 2*NP1, 3*N*N +
     $                           MAX( N*NP, 10*N*N + 12*N + 5 ) ) )
         LW6 = 2*N*N + N*( M + NP ) +
     $         MAX( 1, M2*NP2 + NP2*NP2 + M2*M2 + 
     $                 MAX( ND1*ND1 + MAX( 2*ND1, ( ND1 + ND2 )*NP2 ),
     $                      ND2*ND2 + MAX( 2*ND2, ND2*M2 ), 3*N, 
     $                      N*( 2*NP2 + M2 ) + 
     $                      MAX( 2*N*M2, M2*NP2 +
     $                              MAX( M2*M2 + 3*M2, NP2*( 2*NP2 +
     $                                   M2 + MAX( NP2, N ) ) ) ) ) )
         MINWRK = N*M + NP*( N + M ) + M2*M2 + NP2*NP2 +
     $            MAX( 1, LW1, LW2, LW3, LW4, LW5, LW6 )
         IF( LDWORK.LT.MINWRK )
     $      INFO = -27
      END IF       
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'SB10FD', -INFO )
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
         DWORK( 1 ) = ONE
         RETURN
      END IF
C
      TOLL = TOL
      IF( TOLL.LE.ZERO ) THEN
C
C        Set the default value of the tolerance.
C
         TOLL = SQRT( DLAMCH( 'Epsilon' )  )
      END IF      
C
C     Workspace usage.
C
      IWC  = 1 + N*M
      IWD  = IWC + NP*N
      IWTU = IWD + NP*M
      IWTY = IWTU + M2*M2
      IWRK = IWTY + NP2*NP2
C
      CALL DLACPY( 'Full', N, M, B, LDB, DWORK, N )
      CALL DLACPY( 'Full', NP, N, C, LDC, DWORK( IWC ), NP )
      CALL DLACPY( 'Full', NP, M, D, LDD, DWORK( IWD ), NP )
C
C     Transform the system so that D12 and D21 satisfy the formulas
C     in the computation of the Hinf (sub)optimal controller.
C
      CALL SB10PD( N, M, NP, NCON, NMEAS, A, LDA, DWORK, N,
     $             DWORK( IWC ), NP, DWORK( IWD ), NP, DWORK( IWTU ),
     $             M2, DWORK( IWTY ), NP2, RCOND, TOLL, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = INFO2
         RETURN
      END IF
      LWAMAX = INT( DWORK( IWRK ) ) + IWRK - 1
C
      IWX  = IWRK
      IWY  = IWX + N*N
      IWF  = IWY + N*N
      IWH  = IWF + M*N
      IWRK = IWH + N*NP
C
C     Compute the (sub)optimal state feedback and output injection
C     matrices.
C
      CALL SB10QD( N, M, NP, NCON, NMEAS, GAMMA, A, LDA, DWORK, N,
     $             DWORK( IWC ), NP, DWORK( IWD ), NP, DWORK( IWF ),
     $             M, DWORK( IWH ), N, DWORK( IWX ), N, DWORK( IWY ),
     $             N, RCOND(3), IWORK, DWORK( IWRK ), LDWORK-IWRK+1,
     $             BWORK, INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = INFO2 + 5
         RETURN
      END IF
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
C     Compute the Hinf (sub)optimal controller.
C
      CALL SB10RD( N, M, NP, NCON, NMEAS, GAMMA, A, LDA, DWORK, N,
     $             DWORK( IWC ), NP, DWORK( IWD ), NP, DWORK( IWF ),
     $             M, DWORK( IWH ), N, DWORK( IWTU ), M2, DWORK( IWTY ),
     $             NP2, DWORK( IWX ), N, DWORK( IWY ), N, AK, LDAK, BK,
     $             LDBK, CK, LDCK, DK, LDDK, IWORK, DWORK( IWRK ),
     $             LDWORK-IWRK+1, INFO2 )
      IF( INFO2.EQ.1 ) THEN
         INFO = 6
         RETURN
      ELSE IF( INFO2.EQ.2 ) THEN
         INFO = 9
         RETURN
      END IF
      LWAMAX = MAX( INT( DWORK( IWRK ) ) + IWRK - 1, LWAMAX )
C
      DWORK( 1 ) = DBLE( LWAMAX )
      RETURN
C *** Last line of SB10FD ***
      END
