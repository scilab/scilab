      SUBROUTINE AB01ND( JOBZ, N, M, A, LDA, B, LDB, NCONT, INDCON, 
     $                   NBLK, Z, LDZ, TAU, TOL, IWORK, DWORK, LDWORK, 
     $                   INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 1999.
C
C     PURPOSE
C
C     To find a controllable realization for the linear time-invariant
C     multi-input system
C
C             dX/dt = A * X + B * U,
C
C     where A and B are N-by-N and N-by-M matrices, respectively,
C     which are reduced by this routine to orthogonal canonical form 
C     using (and optionally accumulating) orthogonal similarity
C     transformations.  Specifically, the pair (A, B) is reduced to
C     the pair (Ac, Bc),  Ac = Z' * A * Z,  Bc = Z' * B,  given by
C
C             [ Acont     *    ]         [ Bcont ] 
C        Ac = [                ],   Bc = [       ], 
C             [   0    Auncont ]         [   0   ] 
C
C        and
C
C                [ A11 A12  . . .  A1,p-1 A1p ]         [ B1 ] 
C                [ A21 A22  . . .  A2,p-1 A2p ]         [ 0  ] 
C                [  0  A32  . . .  A3,p-1 A3p ]         [ 0  ] 
C        Acont = [  .   .   . . .    .     .  ],   Bc = [ .  ], 
C                [  .   .     . .    .     .  ]         [ .  ] 
C                [  .   .       .    .     .  ]         [ .  ] 
C                [  0   0   . . .  Ap,p-1 App ]         [ 0  ] 
C
C     where the blocks  B1, A21, ..., Ap,p-1  have full row ranks and 
C     p is the controllability index of the pair.  The size of the
C     block  Auncont is equal to the dimension of the uncontrollable
C     subspace of the pair (A, B). 
C
C     ARGUMENTS 
C
C     Mode Parameters
C
C     JOBZ    CHARACTER*1
C             Indicates whether the user wishes to accumulate in a
C             matrix Z the orthogonal similarity transformations for
C             reducing the system, as follows:
C             = 'N':  Do not form Z and do not store the orthogonal 
C                     transformations; 
C             = 'F':  Do not form Z, but store the orthogonal 
C                     transformations in the factored form; 
C             = 'I':  Z is initialized to the unit matrix and the 
C                     orthogonal transformation matrix Z is returned.
C
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the original state-space representation,
C             i.e. the order of the matrix A.  N >= 0.
C
C     M       (input) INTEGER
C             The number of system inputs, or of columns of B.  M >= 0.
C             
C     A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
C             On entry, the leading N-by-N part of this array must  
C             contain the original state dynamics matrix A.
C             On exit, the leading NCONT-by-NCONT part contains the 
C             upper block Hessenberg state dynamics matrix Acont in Ac, 
C             given by Z' * A * Z, of a controllable realization for 
C             the original system. The elements below the first block-
C             subdiagonal are set to zero.  
C     
C     LDA     INTEGER
C             The leading dimension of array A.  LDA >= MAX(1,N).
C
C     B       (input/output) DOUBLE PRECISION array, dimension (LDB,M)
C             On entry, the leading N-by-M part of this array must 
C             contain the input matrix B.
C             On exit, the leading NCONT-by-M part of this array 
C             contains the transformed input matrix Bcont in Bc, given
C             by Z' * B, with all elements but the first block set to 
C             zero.
C              
C     LDB     INTEGER
C             The leading dimension of array B.  LDB >= MAX(1,N).
C
C     NCONT   (output) INTEGER
C             The order of the controllable state-space representation.
C
C     INDCON  (output) INTEGER
C             The controllability index of the controllable part of the
C             system representation.
C
C     NBLK    (output) INTEGER array, dimension (N)
C             The leading INDCON elements of this array contain the
C             the orders of the diagonal blocks of Acont. 
C
C     Z       (output) DOUBLE PRECISION array, dimension (LDZ,N)
C             If JOBZ = 'I', then the leading N-by-N part of this
C             array contains the matrix of accumulated orthogonal
C             similarity transformations which reduces the given system
C             to orthogonal canonical form.
C             If JOBZ = 'F', the elements below the diagonal, with the
C             array TAU, represent the orthogonal transformation matrix
C             as a product of elementary reflectors. The transformation 
C             matrix can then be obtained by calling the LAPACK Library 
C             routine DORGQR.
C             If JOBZ = 'N', the array Z is not referenced and can be 
C             supplied as a dummy array (i.e. set parameter LDZ = 1 and
C             declare this array to be Z(1,1) in the calling program).
C
C     LDZ     INTEGER
C             The leading dimension of array Z. If JOBZ = 'I' or 
C             JOBZ = 'F', LDZ >= MAX(1,N); if JOBZ = 'N', LDZ >= 1.
C
C     TAU     (output) DOUBLE PRECISION array, dimension (N)  
C             The elements of TAU contain the scalar factors of the
C             elementary reflectors used in the reduction of B and A.
C
C     Tolerances
C
C     TOL     DOUBLE PRECISION
C             The tolerance to be used in rank determination when 
C             transforming (A, B). If the user sets TOL > 0, then
C             the given value of TOL is used as a lower bound for the
C             reciprocal condition number (see the description of the
C             argument RCOND in the SLICOT routine MB03OD);  a 
C             (sub)matrix whose estimated condition number is less than
C             1/TOL is considered to be of full rank.  If the user sets
C             TOL <= 0, then an implicitly computed, default tolerance,
C             defined by  TOLDEF = N*N*EPS,  is used instead, where EPS 
C             is the machine precision (see LAPACK Library routine 
C             DLAMCH).
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (M) 
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) returns the optimal value
C             of LDWORK.
C
C     LDWORK  INTEGER
C             The length of the array DWORK. 
C             LDWORK >= MAX(1, N, 3*M).
C             For optimum performance LDWORK should be larger.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value.
C
C     METHOD
C
C     Matrix B is first QR-decomposed and the appropriate orthogonal
C     similarity transformation applied to the matrix A. Leaving the
C     first rank(B) states unchanged, the remaining lower left block
C     of A is then QR-decomposed and the new orthogonal matrix, Q1,
C     is also applied to the right of A to complete the similarity
C     transformation. By continuing in this manner, a completely
C     controllable state-space pair (Acont, Bcont) is found for the 
C     given (A, B), where Acont is upper block Hessenberg with each 
C     subdiagonal block of full row rank, and Bcont is zero apart from
C     its (independent) first rank(B) rows.
C     NOTE that the system controllability indices are easily
C     calculated from the dimensions of the blocks of Acont.
C
C     REFERENCES
C
C     [1] Konstantinov, M.M., Petkov, P.Hr. and Christov, N.D.
C         Orthogonal Invariants and Canonical Forms for Linear
C         Controllable Systems.
C         Proc. 8th IFAC World Congress, Kyoto, 1, pp. 49-54, 1981.
C
C     [2] Paige, C.C.
C         Properties of numerical algorithms related to computing
C         controllablity.
C         IEEE Trans. Auto. Contr., AC-26, pp. 130-138, 1981.
C
C     [3] Petkov, P.Hr., Konstantinov, M.M., Gu, D.W. and 
C         Postlethwaite, I.
C         Optimal Pole Assignment Design of Linear Multi-Input Systems.
C         Leicester University, Report 99-11, May 1996.
C
C     NUMERICAL ASPECTS
C                               3
C     The algorithm requires 0(N ) operations and is backward stable.
C
C     FURTHER COMMENTS
C
C     If the system matrices A and B are badly scaled, it would be
C     useful to scale them with SLICOT routine TB01ID, before calling 
C     the routine.
C 
C     CONTRIBUTOR
C
C     Release 3.0: V. Sima, Katholieke Univ. Leuven, Belgium, Nov. 1996.
C     Supersedes Release 2.0 routine AB01BD by P.Hr. Petkov.
C
C     REVISIONS
C
C     January 14, 1997, June 4, 1997, February 13, 1998.   
C
C     KEYWORDS
C
C     Controllability, minimal realization, orthogonal canonical form,
C     orthogonal transformation.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
C     .. Scalar Arguments ..
      CHARACTER*1       JOBZ
      INTEGER           INDCON, INFO, LDA, LDB, LDWORK, LDZ, M, N, NCONT
      DOUBLE PRECISION  TOL
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), DWORK(*), TAU(*), Z(LDZ,*)
      INTEGER           IWORK(*), NBLK(*)
C     .. Local Scalars ..
      LOGICAL           LJOBF, LJOBI, LJOBZ
      INTEGER           IQR, ITAU, J, MCRT, NBL, NCRT, NI, NJ, RANK,
     $                  WRKOPT
      DOUBLE PRECISION  ANORM, BNORM, FNRM, TOLDEF
C     .. Local Arrays ..
      DOUBLE PRECISION  SVAL(3)
C     .. External Functions ..
      LOGICAL           LSAME
      DOUBLE PRECISION  DLAMCH, DLANGE, DLAPY2
      EXTERNAL          DLAMCH, DLANGE, DLAPY2, LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DLACPY, DLAPMT, DLASET, DORGQR, DORMQR, 
     $                  MB01PD, MB03OY, XERBLA 
C     .. Intrinsic Functions ..
      INTRINSIC         DBLE, INT, MAX, MIN
C     ..
C     .. Executable Statements ..
C
      INFO = 0
      LJOBF = LSAME( JOBZ, 'F' ) 
      LJOBI = LSAME( JOBZ, 'I' ) 
      LJOBZ = LJOBF.OR.LJOBI
C
C     Test the input scalar arguments.
C
      IF( .NOT.LJOBZ .AND. .NOT.LSAME( JOBZ, 'N' ) ) THEN
         INFO = -1
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( M.LT.0 ) THEN
         INFO = -3
      ELSE IF( LDA.LT.MAX( 1, N ) ) THEN
         INFO = -5
      ELSE IF( LDB.LT.MAX( 1, N ) ) THEN
         INFO = -7
      ELSE IF( .NOT.LJOBZ .AND. LDZ.LT.1 .OR.
     $              LJOBZ .AND. LDZ.LT.MAX( 1, N ) ) THEN
         INFO = -12
      ELSE IF(TOL.LT.ZERO .OR.  TOL.GT.ONE ) THEN
C added by S. STEER (see mb03oy)
         INFO = -14
      ELSE IF( LDWORK.LT.MAX( 1, N, 3*M ) ) THEN
         INFO = -17
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'AB01ND', -INFO )
         RETURN
      END IF
C
      NCONT  = 0
      INDCON = 0
C
C     Quick return if possible.
C
      IF ( MIN( N, M ).EQ.0 ) 
     $   RETURN
C
C     Calculate the absolute norms of A and B (used for scaling).
C     
      ANORM = DLANGE( 'M', N, N, A, LDA, DWORK )
      BNORM = DLANGE( 'M', N, M, B, LDB, DWORK )
C
C     Return if matrix B is zero. 
C     
      IF( BNORM.EQ.ZERO ) THEN
         IF ( LJOBI ) THEN
            CALL DLASET( 'Full', N, N, ZERO, ONE, Z, LDZ )
         ELSE IF ( LJOBF ) THEN
            CALL DLASET( 'Full', N, N, ZERO, ZERO, Z, LDZ )
            CALL DLASET( 'Full', N, 1, ZERO, ZERO, TAU, N )
         END IF
         RETURN
      END IF
C
C     Scale (if needed) the matrices A and B.
C
      CALL MB01PD( 'Scale', 'G', N, N, 0, 0, ANORM, 0, NBLK, A, LDA,
     $             INFO )
      CALL MB01PD( 'Scale', 'G', N, M, 0, 0, BNORM, 0, NBLK, B, LDB,
     $             INFO )
C
C     Compute the Frobenius norm of [ B  A ] (used for rank estimation).
C     
      FNRM = DLAPY2( DLANGE( 'F', N, M, B, LDB, DWORK ), 
     $               DLANGE( 'F', N, N, A, LDA, DWORK ) )
C
      TOLDEF = TOL
      IF ( TOLDEF.LE.ZERO ) THEN
C
C        Use the default tolerance in controllability determination.
C
         TOLDEF = DBLE( N*N )*DLAMCH( 'EPSILON' )
      END IF
C 
      WRKOPT = 1
      NI = 0
      ITAU = 1
      NCRT = N
      MCRT = M
      IQR  = 1
C
C     (Note: Comments in the code beginning "Workspace:" describe the
C     minimal amount of real workspace needed at that point in the
C     code, as well as the preferred amount for good performance.
C     NB refers to the optimal block size for the immediately
C     following subroutine, as returned by ILAENV.)
C
   10 CONTINUE
C     
C        Rank-revealing QR decomposition with column pivoting. 
C        The calculation is performed in NCRT rows of B starting from 
C        the row IQR (initialized to 1 and then set to rank(B)+1).
C        Workspace: 3*MCRT.
C     
         CALL MB03OY( NCRT, MCRT, B(IQR,1), LDB, TOLDEF, FNRM, RANK,
     $                SVAL, IWORK, TAU(ITAU), DWORK, INFO )
C
         IF ( RANK.NE.0 ) THEN
            NJ = NI
            NI = NCONT
            NCONT = NCONT + RANK
            INDCON = INDCON + 1
            NBLK(INDCON) = RANK
C
C           Premultiply and postmultiply the appropriate block row 
C           and block column of A by Q' and Q, respectively.
C           Workspace: need   NCRT;  
C                      prefer NCRT*NB.
C           
            CALL DORMQR( 'Left', 'Transpose', NCRT, NCRT, RANK, 
     $                   B(IQR,1), LDB, TAU(ITAU), A(NI+1,NI+1), LDA,
     $                   DWORK, LDWORK, INFO )
            WRKOPT = MAX( WRKOPT, INT( DWORK(1) ) )
C
C           Workspace: need   N;
C                      prefer N*NB.
C
            CALL DORMQR( 'Right', 'No transpose', N, NCRT, RANK,
     $                   B(IQR,1), LDB, TAU(ITAU), A(1,NI+1), LDA,
     $                   DWORK, LDWORK, INFO )
            WRKOPT = MAX( WRKOPT, INT( DWORK(1) ) )
C
C           If required, save transformations.
C
            IF ( LJOBZ.AND.NCRT.GT.1 ) THEN
               CALL DLACPY( 'L', NCRT-1, MIN( RANK, NCRT-1 ), 
     $                      B(IQR+1,1), LDB, Z(NI+2,ITAU), LDZ )
            END IF 
C
C           Zero the subdiagonal elements of the current matrix.
C
            IF ( RANK.GT.1 ) 
     $         CALL DLASET( 'L', RANK-1, RANK-1, ZERO, ZERO, B(IQR+1,1),
     $                      LDB ) 
C     
C           Backward permutation of the columns of B or A.
C     
            IF ( INDCON.EQ.1 ) THEN
               CALL DLAPMT( .FALSE., RANK, M, B(IQR,1), LDB, IWORK )
               IQR = RANK + 1
            ELSE
               DO 20 J = 1, MCRT
                  CALL DCOPY( RANK, B(IQR,J), 1, A(NI+1,NJ+IWORK(J)),  
     $                        1 )
   20          CONTINUE
            END IF
C
            ITAU = ITAU + RANK
            IF ( RANK.NE.NCRT ) THEN
               MCRT = RANK
               NCRT = NCRT - RANK
               CALL DLACPY( 'G', NCRT, MCRT, A(NCONT+1,NI+1), LDA, 
     $                      B(IQR,1), LDB )
               CALL DLASET( 'G', NCRT, MCRT, ZERO, ZERO, 
     $                      A(NCONT+1,NI+1), LDA )
               GO TO 10
            END IF
         END IF
C
C     If required, accumulate transformations.
C     Workspace: need N;  prefer N*NB.
C     
      IF ( LJOBI ) THEN
         CALL DORGQR( N, N, MAX( 1, ITAU-1 ), Z, LDZ, TAU, DWORK, 
     $                LDWORK, INFO )
         WRKOPT = MAX( WRKOPT, INT( DWORK(1) ) )
      END IF
C
C     Annihilate the trailing blocks of B.
C     
      CALL DLASET( 'G', N-IQR+1, M, ZERO, ZERO, B(IQR,1), LDB )
C
C     Annihilate the trailing elements of TAU, if JOBZ = 'F'.
C     
      IF ( LJOBF ) THEN
         DO 30 J = ITAU, N
            TAU(J) = ZERO
 30      CONTINUE
      END IF
C     
C     Undo scaling of A and B.
C     
      IF ( INDCON.LT.N ) THEN
         NBL = INDCON + 1
         NBLK(NBL) = N - NCONT
      ELSE
         NBL = 0
      END IF
      CALL MB01PD( 'Undo', 'H', N, N, 0, 0, ANORM, NBL, NBLK, A,
     $             LDA, INFO )
      CALL MB01PD( 'Undo', 'G', NBLK(1), M, 0, 0, BNORM, 0, NBLK, B,
     $             LDB, INFO )
C
C     Set optimal workspace dimension.
C
      DWORK(1)  = WRKOPT
      RETURN
C *** Last line of AB01ND ***
      END
