      SUBROUTINE AB13MD( FACT, N, Z, LDZ, M, NBLOCK, ITYPE, X, BOUND, D,
     $                   G, IWORK, DWORK, LDWORK, ZWORK, LZWORK, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To compute an upper bound on the structured singular value for a
C     given square complex matrix and a given block structure of the
C     uncertainty.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     FACT    CHARACTER*1
C             Specifies whether or not an information from the
C             previous call is supplied in the vector X.
C             = 'F':  On entry, X contains information from the
C                     previous call.
C             = 'N':  On entry, X does not contain an information from
C                     the previous call.
C 
C     Input/Output Parameters
C
C     N       (input) INTEGER
C             The order of the matrix Z.  N >= 0.
C
C     Z       (input) COMPLEX*16 array, dimension (LDZ,N)
C             The leading N-by-N part of this array must contain the 
C             complex matrix Z for which the upper bound on the
C             structured singular value is to be computed. 
C
C     LDZ     INTEGER
C             The leading dimension of the array Z.  LDZ >= max(1,N).
C
C     M       (input) INTEGER
C             The number of diagonal blocks in the block structure of
C             the uncertainty.  M >= 1.
C
C     NBLOCK  (input) INTEGER array, dimension (M)
C             The vector of length M containing the block structure
C             of the uncertainty. NBLOCK(I), I = 1:M, is the size of
C             each block.
C
C     ITYPE   (input) INTEGER array, dimension (M)
C             The vector of length M indicating the type of each block.
C             For I = 1:M,
C             ITYPE(I) = 1 indicates that the corresponding block is a
C                          real block, and
C             ITYPE(I) = 2 indicates that the corresponding block is a 
C                          complex block.
C             NBLOCK(I) must be equal to 1 if ITYPE(I) is equal to 1.
C
C     X       (input/output) DOUBLE PRECISION array, dimension 
C             ( M + MR - 1 ), where MR is the number of the real blocks.
C             On entry, if FACT = 'F' and NBLOCK(1) < N, this array
C             must contain information from the previous call to AB13MD.
C             If NBLOCK(1) = N, this array is not used. 
C             On exit, if NBLOCK(1) < N, this array contains information
C             that can be used in the next call to AB13MD for a matrix 
C             close to Z. 
C
C     BOUND   (output) DOUBLE PRECISION
C             The upper bound on the structured singular value.
C
C     D, G    (output) DOUBLE PRECISION arrays, dimension (N)
C             The vectors of length N containing the diagonal entries
C             of the diagonal N-by-N matrices D and G, respectively, 
C             such that the matrix
C             Z'*D^2*Z + sqrt(-1)*(G*Z-Z'*G) - BOUND^2*D^2 
C             is negative semidefinite.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension MAX(4*M-2,N)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if INFO = 0, DWORK(1) contains the optimal value
C             of LDWORK.
C
C     LDWORK  INTEGER
C             The dimension of the array DWORK.
C             LDWORK >= 2*N*N*M - N*N + 9*M*M + N*M + 11*N + 33*M - 11.
C             For best performance
C             LDWORK >= 2*N*N*M - N*N + 9*M*M + N*M + 6*N + 33*M - 11 +
C                       MAX( 5*N,2*N*NB )
C             where NB is the optimal blocksize returned by ILAENV.
C
C     ZWORK   COMPLEX*16 array, dimension (LZWORK)
C             On exit, if INFO = 0, ZWORK(1) contains the optimal value
C             of LZWORK.
C
C     LZWORK  INTEGER
C             The dimension of the array ZWORK.
C             LZWORK >= 6*N*N*M + 12*N*N + 6*M + 6*N - 3.
C             For best performance
C             LZWORK >= 6*N*N*M + 12*N*N + 6*M + 3*N - 3 +
C                       MAX( 3*N,N*NB )
C             where NB is the optimal blocksize returned by ILAENV. 
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal 
C                   value;
C             = 1:  the block sizes must be positive integers;
C             = 2:  the sum of block sizes must be equal to N;
C             = 3:  the size of a real block must be equal to 1;
C             = 4:  the block type must be either 1 or 2;
C             = 5:  errors in solving linear equations or in matrix
C                   inversion;
C             = 6:  errors in computing eigenvalues or singular values.
C
C     METHOD
C
C     The routine computes the upper bound proposed in [1].
C
C     REFERENCES 
C
C     [1] Fan, M.K.H., Tits, A.L., and Doyle, J.C.
C         Robustness in the presence of mixed parametric uncertainty
C         and unmodeled dynamics.
C         IEEE Trans. Automatic Control, vol. AC-36, 1991, pp. 25-38.
C
C     NUMERICAL ASPECTS
C
C     The accuracy and speed of computation depend on the value of
C     the internal threshold TOL.
C 
C     CONTRIBUTORS
C
C     P.Hr. Petkov, F. Delebecque, D.W. Gu, M.M. Konstantinov and
C     S. Steer with the assistance of V. Sima, September 2000.
C
C     REVISIONS
C
C     V. Sima, Katholieke Universiteit Leuven, February 2001.
C
C     KEYWORDS
C
C     H-infinity optimal control, Robust control, Structured singular
C     value.
C
C     ******************************************************************
C
C     .. Parameters ..
      COMPLEX*16         CZERO, CONE, CIMAG
      PARAMETER          ( CZERO = ( 0.0D+0, 0.0D+0 ),
     $                     CONE  = ( 1.0D+0, 0.0D+0 ), 
     $                     CIMAG = ( 0.0D+0, 1.0D+0 ) )
      DOUBLE PRECISION   ZERO, ONE, TWO, FOUR, FIVE, EIGHT, TEN, FORTY,
     $                   FIFTY
      PARAMETER          ( ZERO = 0.0D+0, ONE = 1.0D+0, TWO = 2.0D+0,
     $                     FOUR = 4.0D+0, FIVE = 5.0D+0, EIGHT = 8.0D+0,
     $                     TEN  = 1.0D+1, FORTY = 4.0D+1, FIFTY = 5.0D+1
     $                   )
      DOUBLE PRECISION   ALPHA, BETA, THETA
      PARAMETER          ( ALPHA = 100.0D+0, BETA = 1.0D-2,
     $                     THETA = 1.0D-2 )
      DOUBLE PRECISION   C1, C2, C3, C4, C5, C6, C7, C8, C9
      PARAMETER          ( C1 = 1.0D-3, C2 = 1.0D-2, C3 = 0.25D+0,
     $                     C4 = 0.9D+0, C5 = 1.5D+0, C6 = 1.0D+1,
     $                     C7 = 1.0D+2, C8 = 1.0D+3, C9 = 1.0D+4 )
C     ..
C     .. Scalar Arguments ..
      CHARACTER          FACT
      INTEGER            INFO, LDWORK, LDZ, LZWORK, M, N
      DOUBLE PRECISION   BOUND
C     ..
C     .. Array Arguments ..
      INTEGER            ITYPE( * ), IWORK( * ), NBLOCK( * )
      COMPLEX*16         Z( LDZ, * ), ZWORK( * )
      DOUBLE PRECISION   D( * ), DWORK( * ), G( * ), X( * )
C     ..
C     .. Local Scalars ..
      INTEGER            I, INFO2, ISUM, ITER, IW2, IW3, IW4, IW5, IW6, 
     $                   IW7, IW8, IW9, IW10, IW11, IW12, IW13, IW14,  
     $                   IW15, IW16, IW17, IW18, IW19, IW20, IW21, IW22,
     $                   IW23, IW24, IW25, IW26, IW27, IW28, IW29, IW30,
     $                   IW31, IW32, IW33, IWRK, IZ2, IZ3, IZ4, IZ5,
     $                   IZ6, IZ7, IZ8, IZ9, IZ10, IZ11, IZ12, IZ13, 
     $                   IZ14, IZ15, IZ16, IZ17, IZ18, IZ19, IZ20, IZ21,
     $                   IZ22, IZ23, IZ24, IZWRK, J, K, L, LWA, LWAMAX,
     $                   LZA, LZAMAX, MINWRK, MINZRK, MR, MT, NSUM, SDIM
      COMPLEX*16         DETF, TEMPIJ, TEMPJI
      DOUBLE PRECISION   C, COLSUM, DELTA, DLAMBD, E, EMAX, EMIN, EPS,
     $                   HN, HNORM, HNORM1, PHI, PP, PROD, RAT, RCOND, 
     $                   REGPAR, ROWSUM, SCALE, SNORM, STSIZE, SVLAM,
     $                   T1, T2, T3, TAU, TEMP, TOL, TOL2, TOL3, TOL4, 
     $                   TOL5, YNORM1, YNORM2, ZNORM, ZNORM2 
      LOGICAL            GTEST, POS, SELECT, XFACT
C     ..
C     .. Local Arrays ..
      LOGICAL            BWORK( 1 )
C     ..
C     .. External Functions
      DOUBLE PRECISION   DDOT, DLAMCH, DLANGE, ZLANGE
      LOGICAL            LSAME
      EXTERNAL           DDOT, DLAMCH, DLANGE, LSAME, ZLANGE
C     ..
C     .. External Subroutines ..
      EXTERNAL           DCOPY, DGEMV, DLACPY, DLASET, DSCAL, DSYCON,
     $                   DSYSV, DSYTRF, DSYTRS, XERBLA, ZCOPY, ZGEES,
     $                   ZGEMM, ZGEMV, ZGESVD, ZGETRF, ZGETRI, ZLACPY,
     $                   ZLASCL
C     ..
C     .. Intrinsic Functions ..
      INTRINSIC          ABS, DCMPLX, DCONJG, DFLOAT, DREAL, INT, LOG,
     $                   MAX, SQRT
C     ..
C     .. Executable Statements ..      
C        
C     Compute workspace.
C        
      MINWRK = 2*N*N*M - N*N + 9*M*M + N*M + 11*N + 33*M - 11
      MINZRK = 6*N*N*M + 12*N*N + 6*M + 6*N - 3     
C
C     Decode and Test input parameters.
C
      INFO = 0
      XFACT = LSAME( FACT, 'F' )
      IF( .NOT.XFACT .AND. .NOT.LSAME( FACT, 'N' ) ) THEN
         INFO = -1           
      ELSE IF( N.LT.0 ) THEN
         INFO = -2
      ELSE IF( LDZ.LT.MAX( 1, N ) ) THEN
         INFO = -4
      ELSE IF( M.LT.1 ) THEN
         INFO = -5
      ELSE IF( LDWORK.LT.MINWRK ) THEN
         INFO = -14
      ELSE IF( LZWORK.LT.MINZRK ) THEN
         INFO = -16
      END IF
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'AB13MD', -INFO )
         RETURN
      END IF
C
      NSUM = 0
      ISUM = 0
      MR = 0
      DO 10 I = 1, M
         IF( NBLOCK( I ).LT.1 ) THEN
            INFO = 1
            RETURN
         END IF
         IF( ITYPE( I ).EQ.1 .AND. NBLOCK( I ).GT.1 ) THEN
            INFO = 3
            RETURN
         END IF
         NSUM = NSUM + NBLOCK( I )
         IF( ITYPE( I ).EQ.1 ) MR = MR + 1
         IF( ITYPE( I ).EQ.1 .OR. ITYPE( I ).EQ.2 ) ISUM = ISUM + 1
   10 CONTINUE
      IF( NSUM.NE.N ) THEN
         INFO = 2
         RETURN
      END IF
      IF( ISUM.NE.M ) THEN
         INFO = 4
         RETURN
      END IF
      MT = M + MR - 1
C
      LWAMAX = 0
      LZAMAX = 0
C     
C     Set D = In, G = 0.
C     
      CALL DLASET( 'Full', N, 1, ONE, ONE, D, N )            
      CALL DLASET( 'Full', N, 1, ZERO, ZERO, G, N )
C     
C     Quick return if possible.
C        
      ZNORM = ZLANGE( 'F', N, N, Z, LDZ, DWORK )
      IF( ZNORM.EQ.ZERO ) THEN
         BOUND = ZERO
         DWORK( 1 ) = ONE
         ZWORK( 1 ) = CONE
         RETURN
      END IF
C
C     Copy Z into ZWORK.
C
      CALL ZLACPY( 'Full', N, N, Z, LDZ, ZWORK, N )
C
C     Exact bound for the case NBLOCK( 1 ) = N.
C
      IF( NBLOCK( 1 ).EQ.N ) THEN
         IF( ITYPE( 1 ).EQ.1 ) THEN
C
C           1-by-1 real block.
C
            BOUND = ZERO
            DWORK( 1 ) = ONE
            ZWORK( 1 ) = CONE
         ELSE
C
C           N-by-N complex block.
C
            CALL ZGESVD( 'N', 'N', N, N, ZWORK, N, DWORK, ZWORK, 1,
     $                   ZWORK, 1, ZWORK( N*N+1 ), LZWORK,
     $                   DWORK( N+1 ), INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 6
               RETURN
            END IF
            BOUND = DWORK( 1 )
            LZA = N*N + INT( ZWORK( N*N+1 ) )
            DWORK( 1 ) = 5*N
            ZWORK( 1 ) = DCMPLX( LZA )
         END IF
         RETURN
      END IF 
C
C     Get machine precision.
C  
      EPS = DLAMCH( 'P' )
C
C     Set tolerances.
C      
      TOL  = C7*SQRT( EPS )
      TOL2 = C9*EPS
      TOL3 = C6*EPS
      TOL4 = C1
      TOL5 = C1
      REGPAR = C8*EPS
C
C     Real workspace usage.
C
      IW2  = M*M
      IW3  = IW2  + M
      IW4  = IW3  + N
      IW5  = IW4  + M
      IW6  = IW5  + M
      IW7  = IW6  + N
      IW8  = IW7  + N
      IW9  = IW8  + N*( M - 1 )
      IW10 = IW9  + N*N*MT
      IW11 = IW10 + MT
      IW12 = IW11 + MT*MT
      IW13 = IW12 + N
      IW14 = IW13 + MT + 1
      IW15 = IW14 + MT + 1
      IW16 = IW15 + MT + 1
      IW17 = IW16 + MT + 1
      IW18 = IW17 + MT + 1
      IW19 = IW18 + MT
      IW20 = IW19 + MT
      IW21 = IW20 + MT
      IW22 = IW21 + N
      IW23 = IW22 + M - 1
      IW24 = IW23 + MR
      IW25 = IW24 + N
      IW26 = IW25 + 2*MT
      IW27 = IW26 + MT
      IW28 = IW27 + MT
      IW29 = IW28 + M - 1
      IW30 = IW29 + MR
      IW31 = IW30 + N + 2*MT
      IW32 = IW31 + MT*MT
      IW33 = IW32 + MT
      IWRK = IW33 + MT + 1
C                 
C     Double complex workspace usage.
C
      IZ2  = N*N
      IZ3  = IZ2  + N*N
      IZ4  = IZ3  + N*N
      IZ5  = IZ4  + N*N
      IZ6  = IZ5  + N*N
      IZ7  = IZ6  + N*N*MT
      IZ8  = IZ7  + N*N
      IZ9  = IZ8  + N*N
      IZ10 = IZ9  + N*N
      IZ11 = IZ10 + MT
      IZ12 = IZ11 + N*N
      IZ13 = IZ12 + N
      IZ14 = IZ13 + N*N
      IZ15 = IZ14 + N
      IZ16 = IZ15 + N*N
      IZ17 = IZ16 + N
      IZ18 = IZ17 + N*N
      IZ19 = IZ18 + N*N*MT
      IZ20 = IZ19 + MT
      IZ21 = IZ20 + N*N*MT
      IZ22 = IZ21 + N*N
      IZ23 = IZ22 + N*N
      IZ24 = IZ23 + N*N
      IZWRK = IZ24 + MT
C
C     Compute the cummulative sums of blocks dimensions.
C
      IWORK( 1 ) = 0
      DO 20 I = 2, M+1
         IWORK( I ) = IWORK( I - 1 ) + NBLOCK( I - 1 )
   20 CONTINUE
C
C     Find Osborne scaling if initial scaling is not given.
C
      IF( .NOT.XFACT ) THEN
         CALL DLASET( 'Full', M, M, ZERO, ZERO, DWORK, M )
         CALL DLASET( 'Full', M, 1, ONE, ONE, DWORK( IW2+1 ), M )
         ZNORM = ZLANGE( 'F', N, N, ZWORK, N, DWORK )
         DO 40 J = 1, M
            DO 30 I = 1, M
               IF( I.NE.J ) THEN
                  CALL ZLACPY( 'Full', IWORK( I+1 )-IWORK( I ), 
     $                         IWORK( J+1 )-IWORK( J ), 
     $                         Z( IWORK( I )+1, IWORK( J )+1 ), LDZ, 
     $                         ZWORK( IZ2+1 ), N ) 
                  CALL ZGESVD( 'N', 'N', IWORK( I+1 )-IWORK( I ), 
     $                         IWORK( J+1 )-IWORK( J ), ZWORK( IZ2+1 ), 
     $                         N, DWORK( IW3+1 ), ZWORK, 1, ZWORK, 1,
     $                         ZWORK( IZWRK+1 ), LZWORK-IZWRK,
     $                         DWORK( IWRK+1 ), INFO2 )
                  IF( INFO2.GT.0 ) THEN
                     INFO = 6
                     RETURN
                  END IF
                  LZA = INT( ZWORK( IZWRK+1 ) )
                  LZAMAX = MAX( LZA, LZAMAX )
                  ZNORM2 = DWORK( IW3+1 )
                  DWORK( I+(J-1)*M ) = ZNORM2 + ZNORM*TOL2
               END IF
   30       CONTINUE
   40    CONTINUE
         CALL  DLASET( 'Full', M, 1, ZERO, ZERO, DWORK( IW4+1 ), M ) 
   50    DO 60 I = 1, M
            DWORK( IW5+I ) = DWORK( IW4+I ) - ONE
   60    CONTINUE
         HNORM = DLANGE( 'F', M, 1, DWORK( IW5+1 ), M, DWORK )
         IF( HNORM.LE.TOL2 ) GO TO 120
            DO 110 K = 1, M
               COLSUM = ZERO
               DO 70 I = 1, M
                  COLSUM = COLSUM + DWORK( I+(K-1)*M )
   70          CONTINUE
               ROWSUM = ZERO
               DO 80 J = 1, M
                  ROWSUM = ROWSUM + DWORK( K+(J-1)*M )
   80          CONTINUE
               RAT = SQRT( COLSUM / ROWSUM )
               DWORK( IW4+K ) = RAT 
               DO 90 I = 1, M
                  DWORK( I+(K-1)*M ) = DWORK( I+(K-1)*M ) / RAT
   90          CONTINUE
               DO 100 J = 1, M
                  DWORK( K+(J-1)*M ) = DWORK( K+(J-1)*M )*RAT
  100          CONTINUE
               DWORK( IW2+K ) = DWORK( IW2+K )*RAT
  110       CONTINUE
            GO TO 50
  120    SCALE = ONE / DWORK( IW2+1 )
         CALL DSCAL( M, SCALE, DWORK( IW2+1 ), 1 )
      ELSE
         DWORK( IW2+1 ) = ONE
         DO 130 I = 2, M
            DWORK( IW2+I ) = SQRT( X( I-1 ) )
  130    CONTINUE
      END IF
      DO 150 J = 1, M
         DO 140 I = 1, M
            IF( I.NE.J ) THEN
               CALL ZLASCL( 'G', M, M, DWORK( IW2+J ), DWORK( IW2+I ),
     $                      IWORK( I+1 )-IWORK( I ), 
     $                      IWORK( J+1 )-IWORK( J ), 
     $                      ZWORK( IWORK( I )+1+IWORK( J )*N ), N,
     $                      INFO2 ) 
            END IF
  140    CONTINUE          
  150 CONTINUE
C
C     Scale Z by its 2-norm.
C
      CALL ZLACPY( 'Full', N, N, ZWORK, N, ZWORK( IZ2+1 ), N ) 
      CALL ZGESVD( 'N', 'N', N, N, ZWORK( IZ2+1 ), N, DWORK( IW3+1 ),
     $             ZWORK, 1, ZWORK, 1, ZWORK( IZWRK+1 ), LZWORK-IZWRK, 
     $             DWORK( IWRK+1 ), INFO2 )
      IF( INFO2.GT.0 ) THEN
         INFO = 6
         RETURN
      END IF
      LZA = INT( ZWORK( IZWRK+1 ) )
      LZAMAX = MAX( LZA, LZAMAX )
      ZNORM = DWORK( IW3+1 ) 
      CALL ZLASCL( 'G', M, M, ZNORM, ONE, N, N, ZWORK, N, INFO2 ) 
C
C     Set BB.
C
      CALL DLASET( 'Full', N*N, MT, ZERO, ZERO, DWORK( IW9+1 ), N*N )
C            
C     Set P.
C
      DO 160 I = 1, NBLOCK( 1 )
         DWORK( IW6+I ) = ONE
  160 CONTINUE
      DO 170 I =  NBLOCK( 1 )+1, N
         DWORK( IW6+I ) = ZERO
  170 CONTINUE
C
C     Compute P*Z.
C
      DO 190 J = 1, N
         DO 180 I = 1, N
            ZWORK( IZ3+I+(J-1)*N ) = DCMPLX( DWORK( IW6+I ) )*
     $                               ZWORK( I+(J-1)*N )
  180    CONTINUE
  190 CONTINUE
C
C     Compute Z'*P*Z.
C
      CALL ZGEMM( 'C', 'N', N, N, N, CONE, ZWORK, N, ZWORK( IZ3+1 ), N, 
     $            CZERO, ZWORK( IZ4+1 ), N )      
C     
C     Copy Z'*P*Z into A0.
C
      CALL ZLACPY( 'Full', N, N, ZWORK( IZ4+1 ), N, ZWORK( IZ5+1 ), N )
C
C     Copy diag(P) into B0d.
C
      CALL DCOPY( N, DWORK( IW6+1 ), 1, DWORK( IW7+1 ), 1 ) 
C
      DO 270 K = 2, M
C
C        Set P.
C
         DO 200 I = 1, IWORK( K )
            DWORK( IW6+I ) = ZERO
  200    CONTINUE
         DO 210 I = IWORK( K )+1, IWORK( K )+NBLOCK( K )
            DWORK( IW6+I ) = ONE
  210    CONTINUE
         IF( K.LT.M ) THEN
            DO 220 I = IWORK( K+1 )+1, N
               DWORK( IW6+I ) = ZERO
  220       CONTINUE
         END IF
C
C        Compute P*Z.
C
         DO 240 J = 1, N
            DO 230 I = 1, N
               ZWORK( IZ3+I+(J-1)*N ) = DCMPLX( DWORK( IW6+I ) )*
     $                                  ZWORK( I+(J-1)*N )
  230       CONTINUE
  240    CONTINUE
C
C        Compute t = Z'*P*Z.
C
         CALL ZGEMM( 'C', 'N', N, N, N, CONE, ZWORK, N, ZWORK( IZ3+1 ),
     $               N, CZERO, ZWORK( IZ4+1 ), N )      
C
C        Copy t(:) into the (k-1)-th column of AA.
C
         CALL ZCOPY( N*N, ZWORK( IZ4+1 ), 1, ZWORK( IZ6+1+(K-2)*N*N ), 
     $               1 )
C
C        Copy diag(P) into the (k-1)-th column of BBd.
C
         CALL DCOPY( N, DWORK( IW6+1 ), 1, DWORK( IW8+1+(K-2)*N ), 1 )
C
C        Copy P(:) into the (k-1)-th column of BB.
C
         DO 260 I = 1, N
            DWORK( IW9+I+(I-1)*N+(K-2)*N*N ) = DWORK( IW6+I )
  260    CONTINUE
  270 CONTINUE
C
      L = 0
C
      DO 350 K = 1, M
         IF( ITYPE( K ).EQ.1 ) THEN
            L = L + 1
C
C           Set P.
C
            DO 280 I = 1, IWORK( K )
               DWORK( IW6+I ) = ZERO
  280       CONTINUE
            DO 290 I =  IWORK( K )+1, IWORK( K )+NBLOCK( K )
               DWORK( IW6+I ) = ONE
  290       CONTINUE
            IF( K.LT.M ) THEN
               DO 300 I = IWORK( K+1 )+1, N
                  DWORK( IW6+I ) = ZERO
  300          CONTINUE
            END IF
C
C           Compute P*Z.
C
            DO 320 J = 1, N
               DO 310 I = 1, N
                  ZWORK( IZ3+I+(J-1)*N ) = DCMPLX( DWORK( IW6+I ) )*
     $                                     ZWORK( I+(J-1)*N )
  310          CONTINUE
  320       CONTINUE
C
C           Compute t = sqrt(-1)*( P*Z - Z'*P ).
C
            DO 340 J = 1, N
               DO 330 I = 1, J
                  TEMPIJ = ZWORK( IZ3+I+(J-1)*N ) 
                  TEMPJI = ZWORK( IZ3+J+(I-1)*N )
                  ZWORK( IZ4+I+(J-1)*N ) = CIMAG*( TEMPIJ - 
     $                                             DCONJG( TEMPJI ) )
                  ZWORK( IZ4+J+(I-1)*N ) = CIMAG*( TEMPJI - 
     $                                             DCONJG( TEMPIJ ) )
  330          CONTINUE
  340       CONTINUE
C
C           Copy t(:) into the (m-1+l)-th column of AA.
C
            CALL ZCOPY( N*N, ZWORK( IZ4+1 ), 1, 
     $                       ZWORK( IZ6+1+(M-2+L)*N*N ), 1 )
         END IF
  350 CONTINUE
C
C     Set initial X.
C
      DO 360 I = 1, M - 1
         X( I ) = ONE
  360 CONTINUE
      IF( MR.GT.0 ) THEN
         IF( .NOT.XFACT ) THEN
            DO 370 I = 1, MR
               X( M-1+I ) = ZERO
  370       CONTINUE
         ELSE
            L = 0
            DO 380 K = 1, M
               IF( ITYPE( K ).EQ.1 ) THEN
                  L = L + 1 
                  X( M-1+L ) = X( M-1+L ) / DWORK( IW2+K )**2
               END IF
  380       CONTINUE
         END IF
      END IF
C
C     Set constants.
C
      SVLAM = ONE / EPS
      C = ONE
C
C     Set H.
C
      CALL  DLASET( 'Full', MT, MT, ZERO, ONE, DWORK( IW11+1 ), MT ) 
C
      ITER = -1
C
C     Main iteration loop.
C
  390 ITER = ITER + 1
C
C        Compute A(:) = A0 + AA*x.
C
         DO 400 I = 1, MT
            ZWORK( IZ10+I ) = DCMPLX( X( I ) )
  400    CONTINUE
         CALL ZCOPY( N*N, ZWORK( IZ5+1 ), 1, ZWORK( IZ7+1 ), 1 )
         CALL ZGEMV( 'N', N*N, MT, CONE, ZWORK( IZ6+1 ), N*N, 
     $               ZWORK( IZ10+1 ), 1, CONE, ZWORK( IZ7+1 ), 1 ) 
C
C        Compute diag( Binv ).
C
         CALL DCOPY( N, DWORK( IW7+1 ), 1, DWORK( IW12+1 ), 1 )         
         CALL DGEMV( 'N', N, M-1, ONE, DWORK( IW8+1 ), N, X, 1, ONE,
     $               DWORK( IW12+1 ), 1 ) 
         DO 410 I = 1, N
            DWORK( IW12+I ) = ONE / DWORK( IW12+I )
  410    CONTINUE
C
C        Compute Binv*A.
C
         DO 430 J = 1, N
            DO 420 I = 1, N
               ZWORK( IZ11+I+(J-1)*N ) = DCMPLX( DWORK( IW12+I ) )*
     $                                   ZWORK( IZ7+I+(J-1)*N )
  420       CONTINUE
  430    CONTINUE
C
C        Compute eig( Binv*A ).
C
         CALL ZGEES( 'N', 'N', SELECT, N, ZWORK( IZ11+1 ), N, SDIM,
     $               ZWORK( IZ12+1 ), ZWORK, N, ZWORK( IZWRK+1 ),
     $               LZWORK-IZWRK, DWORK( IWRK+1 ), BWORK, INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 6
            RETURN
         END IF
         LZA = INT( ZWORK( IZWRK+1 ) )
         LZAMAX = MAX( LZA, LZAMAX )
         E = DREAL( ZWORK( IZ12+1 ) )
         IF( N.GT.1 ) THEN
            DO 440 I = 2, N
               IF( DREAL( ZWORK( IZ12+I ) ).GT.E ) 
     $                                   E = DREAL( ZWORK( IZ12+I ) )
  440       CONTINUE
         END IF
C
C        Set tau.
C
         IF( MR.GT.0 ) THEN
            SNORM = ABS( X( M ) )
            IF( MR.GT.1 ) THEN
               DO 450 I = M+1, MT
                  IF( ABS( X( I ) ).GT.SNORM ) SNORM = ABS( X( I ) )
  450          CONTINUE
            END IF
            IF( SNORM.GT.FORTY ) THEN
               TAU = C7
            ELSE IF( SNORM.GT.EIGHT ) THEN
               TAU = FIFTY
            ELSE IF( SNORM.GT.FOUR ) THEN
               TAU = TEN
            ELSE IF( SNORM.GT.ONE ) THEN
               TAU = FIVE
            ELSE
               TAU = TWO
            END IF
         END IF
         IF( ITER.EQ.0 ) THEN
            DLAMBD = E + C1
         ELSE     
            DWORK( IW13+1 ) = E
            CALL DCOPY( MT, X, 1, DWORK( IW13+2 ), 1 )
            DLAMBD = ( ONE - THETA )*DWORK( IW13+1 ) + 
     $                                         THETA*DWORK( IW14+1 )
            CALL DCOPY( MT, DWORK( IW13+2 ), 1, DWORK( IW18+1 ), 1 )
            CALL DCOPY( MT, DWORK( IW14+2 ), 1, DWORK( IW19+1 ), 1 )
            L = 0
  460       DO 470 I = 1, MT
               X( I ) = ( ONE - THETA / TWO**L )*DWORK( IW18+I ) +
     $                        ( THETA / TWO**L )*DWORK( IW19+I )
  470       CONTINUE
C            
C           Compute At(:) = A0 + AA*x.
C
            DO 480 I = 1, MT
               ZWORK( IZ10+I ) = DCMPLX( X( I ) )
  480       CONTINUE
            CALL ZCOPY( N*N, ZWORK( IZ5+1 ), 1, ZWORK( IZ9+1 ), 1 )
            CALL ZGEMV( 'N', N*N, MT, CONE, ZWORK( IZ6+1 ), N*N, 
     $                  ZWORK( IZ10+1 ), 1, CONE, ZWORK( IZ9+1 ), 1 ) 
C            
C           Compute diag(Bt).
C
            CALL DCOPY( N, DWORK( IW7+1 ), 1, DWORK( IW21+1 ), 1 ) 
            CALL DGEMV( 'N', N, M-1, ONE, DWORK( IW8+1 ), N, X, 1, ONE,
     $                  DWORK( IW21+1 ), 1 ) 
C
C           Compute W.
C
            DO 500 J = 1, N
               DO 490 I = 1, N
                  IF( I.EQ.J ) THEN
                     ZWORK( IZ13+I+(I-1)*N ) = DCMPLX( THETA*BETA*
     $                      ( DWORK( IW14+1 ) - DWORK( IW13+1 ) ) /TWO -
     $                      DLAMBD*DWORK( IW21+I ) ) +
     $                      ZWORK( IZ9+I+(I-1)*N )
                  ELSE
                     ZWORK( IZ13+I+(J-1)*N ) = ZWORK( IZ9+I+(J-1)*N )
                  END IF
  490          CONTINUE
  500       CONTINUE
C         
C           Compute eig( W ).
C
            CALL ZGEES( 'N', 'N', SELECT, N, ZWORK( IZ13+1 ), N, SDIM,
     $                  ZWORK( IZ14+1 ), ZWORK, N, ZWORK( IZWRK+1 ),
     $                  LZWORK-IZWRK, DWORK( IWRK+1 ), BWORK, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 6
               RETURN
            END IF
            LZA = INT( ZWORK( IZWRK+1 ) )
            LZAMAX = MAX( LZA, LZAMAX )
            EMAX = DREAL( ZWORK( IZ14+1 ) )
            IF( N.GT.1 ) THEN
               DO 510 I = 2, N
                  IF( DREAL( ZWORK( IZ14+I ) ).GT.EMAX ) 
     $               EMAX = DREAL( ZWORK( IZ14+I ) )
  510          CONTINUE
            END IF
            IF( EMAX.LE.ZERO ) THEN
               GO TO 515
            ELSE
               L = L + 1
               GO TO 460
            END IF
         END IF
C         
C        Set y.
C
  515    DWORK( IW13+1 ) = DLAMBD
         CALL DCOPY( MT, X, 1, DWORK( IW13+2 ), 1 )
C
         IF( ( SVLAM - DLAMBD ).LT.TOL ) THEN
            BOUND = SQRT( MAX( E, ZERO ) )*ZNORM
            DO 520 I = 1, M - 1
               X( I ) = X( I )*DWORK( IW2+I+1 )**2
  520       CONTINUE
C
C           Compute sqrt( x ).
C
            DO 530 I = 1, M-1
               DWORK( IW20+I ) = SQRT( X( I ) )
  530       CONTINUE   
C
C           Compute diag( D ).
C
            CALL DCOPY( N, DWORK( IW7+1 ), 1, D, 1 )         
            CALL DGEMV( 'N', N, M-1, ONE, DWORK( IW8+1 ), N, 
     $                  DWORK( IW20+1 ), 1, ONE, D, 1 ) 
C
C           Compute diag( G ).
C
            J = 0
            L = 0
            DO 540 K = 1, M
               J = J + NBLOCK( K )
               IF( ITYPE( K ).EQ.1 ) THEN
                  L = L + 1
                  X( M-1+L ) = X( M-1+L )*DWORK( IW2+K )**2
                  G( J ) = X( M-1+L )
               END IF
  540       CONTINUE
            CALL DSCAL( N, ZNORM, G, 1 )
            DWORK( 1 ) = DFLOAT( MINWRK - 5*N + LWAMAX )
            ZWORK( 1 ) = DCMPLX( MINZRK - 3*N + LZAMAX )
            RETURN
         END IF
         SVLAM = DLAMBD
         DO 800 K = 1, M
C
C           Store xD.
C
            CALL DCOPY( M-1, X, 1, DWORK( IW22+1 ), 1 )
            IF( MR.GT.0 ) THEN
C
C              Store xG.
C
               CALL DCOPY( MR, X( M ), 1, DWORK( IW23+1 ), 1 )
            END IF
C
C           Compute A(:) = A0 + AA*x.
C
            DO 550 I = 1, MT
               ZWORK( IZ10+I ) = DCMPLX( X( I ) )
  550       CONTINUE
            CALL ZCOPY( N*N, ZWORK( IZ5+1 ), 1, ZWORK( IZ7+1 ), 1 )
            CALL ZGEMV( 'N', N*N, MT, CONE, ZWORK( IZ6+1 ), N*N, 
     $                  ZWORK( IZ10+1 ), 1, CONE, ZWORK( IZ7+1 ), 1 ) 
C
C           Compute B = B0d + BBd*xD.
C
            CALL DCOPY( N, DWORK( IW7+1 ), 1, DWORK( IW24+1 ), 1 ) 
            CALL DGEMV( 'N', N, M-1, ONE, DWORK( IW8+1 ), N, 
     $                  DWORK( IW22+1 ), 1, ONE, DWORK( IW24+1 ), 1 ) 
C     
C           Compute F.
C
            DO 556 J = 1, N
               DO 555 I = 1, N
                  IF( I.EQ.J ) THEN
                     ZWORK( IZ15+I+(I-1)*N ) = DCMPLX( DLAMBD*
     $                      DWORK( IW24+I ) ) - ZWORK( IZ7+I+(I-1)*N )
                  ELSE
                     ZWORK( IZ15+I+(J-1)*N ) = -ZWORK( IZ7+I+(J-1)*N )
                  END IF
  555          CONTINUE
  556       CONTINUE
            CALL ZLACPY( 'Full', N, N, ZWORK( IZ15+1 ), N, 
     $                   ZWORK( IZ17+1 ), N )
C
C           Compute det( F ).
C
            CALL ZGEES( 'N', 'N', SELECT, N, ZWORK( IZ15+1 ), N, SDIM,
     $                  ZWORK( IZ16+1 ), ZWORK, N, ZWORK( IZWRK+1 ),
     $                  LZWORK-IZWRK, DWORK( IWRK+1 ), BWORK, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 6
               RETURN
            END IF
            LZA = INT( ZWORK( IZWRK+1 ) )
            LZAMAX = MAX( LZA, LZAMAX )
            DETF = CONE
            DO 560 I = 1, N
               DETF = DETF*ZWORK( IZ16+I )
  560       CONTINUE
C
C           Compute Finv.
C
            CALL ZGETRF( N, N, ZWORK( IZ17+1 ), N, IWORK, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 5
               RETURN
            END IF
            CALL ZGETRI( N, ZWORK( IZ17+1 ), N, IWORK, ZWORK( IZWRK+1 ),
     $                   LDWORK-IWRK, INFO2 )
            LZA = INT( ZWORK( IZWRK+1 ) )
            LZAMAX = MAX( LZA, LZAMAX )
C
C           Compute phi.
C
            DO 570 I = 1, M-1
               DWORK( IW25+I ) = DWORK( IW22+I ) - BETA
               DWORK( IW25+M-1+I ) = ALPHA - DWORK( IW22+I )
  570       CONTINUE
            IF( MR.GT.0 ) THEN
               DO 580 I = 1, MR
                  DWORK( IW25+2*(M-1)+I ) = DWORK( IW23+I ) + TAU
                  DWORK( IW25+2*(M-1)+MR+I ) = TAU - DWORK( IW23+I )
  580          CONTINUE
            END IF
            PROD = ONE
            DO 590 I = 1, 2*MT
               PROD = PROD*DWORK( IW25+I ) 
  590       CONTINUE
            TEMP = DREAL( DETF )
            IF( TEMP.LT.EPS ) TEMP = EPS
            PHI = -LOG( TEMP ) - LOG( PROD )
C
C           Compute g.
C
            DO 610 J = 1, MT
               DO 600 I = 1, N*N
                  ZWORK( IZ18+I+(J-1)*N*N ) = DCMPLX( DLAMBD*
     $            DWORK( IW9+I+(J-1)*N*N ) ) - ZWORK( IZ6+I+(J-1)*N*N )
  600          CONTINUE
  610       CONTINUE     
            CALL ZGEMV( 'C', N*N, MT, CONE, ZWORK( IZ18+1 ), N*N, 
     $                  ZWORK( IZ17+1 ), 1, CZERO, ZWORK( IZ19+1 ), 1 )
            DO 620 I = 1, M-1
               DWORK( IW26+I ) = ONE / ( DWORK( IW22+I ) - BETA ) -
     $                           ONE / ( ALPHA - DWORK( IW22+I ) )
  620       CONTINUE
            IF( MR.GT.0 ) THEN
               DO 630 I = 1, MR
                  DWORK( IW26+M-1+I ) = ONE / ( DWORK( IW23+I ) + TAU )
     $                                 -ONE / ( TAU - DWORK( IW23+I ) )
  630          CONTINUE
            END IF
            DO 640 I = 1, MT
               DWORK( IW26+I ) = -DREAL( ZWORK( IZ19+I ) ) - 
     $                                                   DWORK( IW26+I )
  640       CONTINUE
C
C           Compute h.
C
            CALL DLACPY( 'Full', MT, MT, DWORK( IW11+1 ), MT, 
     $                   DWORK( IW31+1 ), MT )           
            CALL DCOPY( MT, DWORK( IW26+1 ), 1, DWORK( IW27+1 ), 1 )
            CALL DSYSV( 'U', MT, 1, DWORK( IW31+1 ), MT, IWORK, 
     $                  DWORK( IW27+1 ), MT, DWORK( IWRK+1 ), 
     $                  LDWORK-IWRK, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 5
               RETURN
            END IF
            LWA = INT( DWORK( IWRK+1 ) )
            LWAMAX = MAX( LWA, LWAMAX )
            STSIZE = ONE
C
C           Store hD.
C
            CALL DCOPY( M-1, DWORK( IW27+1 ), 1, DWORK( IW28+1 ), 1 )
C
C           Determine stepsize.
C
            L = 0
            DO 650 I = 1, M-1
               IF( DWORK( IW28+I ).GT.ZERO ) THEN
                  L = L + 1
                  IF( L.EQ.1 ) THEN
                     TEMP = ( DWORK( IW22+I ) - BETA ) / DWORK( IW28+I )
                  ELSE
                     TEMP = MIN( TEMP, ( DWORK( IW22+I ) - BETA ) / 
     $                                   DWORK( IW28+I ) ) 
                  END IF
               END IF
  650       CONTINUE
            IF( L.GT.0 ) STSIZE = MIN( STSIZE, TEMP )
            L = 0
            DO 660 I = 1, M-1
               IF( DWORK( IW28+I ).LT.ZERO ) THEN
                  L = L + 1
                  IF( L.EQ.1 ) THEN
                     TEMP = ( ALPHA - DWORK( IW22+I ) ) / 
     $                      ( -DWORK( IW28+I ) )
                  ELSE
                     TEMP = MIN( TEMP, ( ALPHA - DWORK( IW22+I ) ) / 
     $                                 ( -DWORK( IW28+I ) ) ) 
                  END IF
               END IF
  660       CONTINUE
            IF( L.GT.0 ) STSIZE = MIN( STSIZE, TEMP )
            IF( MR.GT.0 ) THEN
C
C              Store hG.
C
               CALL DCOPY( MR, DWORK( IW27+M ), 1, DWORK( IW29+1 ), 1 ) 
C
C              Determine stepsize.
C
               L = 0
               DO 670 I = 1, MR
                  IF( DWORK( IW29+I ).GT.ZERO ) THEN
                     L = L + 1
                     IF( L.EQ.1 ) THEN
                        TEMP = ( DWORK( IW23+I ) + TAU ) / 
     $                           DWORK( IW29+I )
                     ELSE
                        TEMP = MIN( TEMP, ( DWORK( IW23+I ) + TAU ) / 
     $                                      DWORK( IW29+I ) ) 
                     END IF
                  END IF
  670          CONTINUE
               IF( L.GT.0 ) STSIZE = MIN( STSIZE, TEMP )
               L = 0
               DO 680 I = 1, MR
                  IF( DWORK( IW29+I ).LT.ZERO ) THEN
                     L = L + 1
                     IF( L.EQ.1 ) THEN
                        TEMP = ( TAU - DWORK( IW23+I ) ) / 
     $                         ( -DWORK( IW29+I ) )
                     ELSE
                        TEMP = MIN( TEMP, ( TAU - DWORK( IW23+I ) ) / 
     $                                    ( -DWORK( IW29+I ) ) ) 
                     END IF
                  END IF
  680          CONTINUE
            END IF
            IF( L.GT.0 ) STSIZE = MIN( STSIZE, TEMP )
            STSIZE = C4*STSIZE
            IF( STSIZE.GE.TOL4 ) THEN
C
C              Compute x_new.
C
               DO 700 I = 1, MT
                  DWORK( IW20+I ) = X( I ) - STSIZE*DWORK( IW27+I )
  700          CONTINUE
C           
C              Store xD.
C
               CALL DCOPY( M-1, DWORK( IW20+1 ), 1, DWORK( IW22+1 ), 1 )
               IF( MR.GT.0 ) THEN
C
C                 Store xG.
C
                  CALL DCOPY( MR, DWORK( IW20+M ), 1, DWORK( IW23+1 ),
     $                        1 )             
               END IF   
C
C              Compute A(:) = A0 + AA*x_new.
C
               DO 710 I = 1, MT
                  ZWORK( IZ10+I ) = DCMPLX( DWORK( IW20+I ) )
  710          CONTINUE
               CALL ZCOPY( N*N, ZWORK( IZ5+1 ), 1, ZWORK( IZ7+1 ), 1 )
               CALL ZGEMV( 'N', N*N, MT, CONE, ZWORK( IZ6+1 ), N*N, 
     $                     ZWORK( IZ10+1 ), 1, CONE, ZWORK( IZ7+1 ), 1 )
C
C              Compute B = B0d + BBd*xD.
C
               CALL DCOPY( N, DWORK( IW7+1 ), 1, DWORK( IW24+1 ), 1 ) 
               CALL DGEMV( 'N', N, M-1, ONE, DWORK( IW8+1 ), N, 
     $                     DWORK( IW22+1 ), 1, ONE, DWORK( IW24+1 ), 1 )
C
C              Compute lambda*diag(B) - A.
C
               DO 730 J = 1, N
                  DO 720 I = 1, N
                     IF( I.EQ.J ) THEN
                        ZWORK( IZ15+I+(I-1)*N ) = DCMPLX( DLAMBD*
     $                       DWORK( IW24+I ) ) - ZWORK( IZ7+I+(I-1)*N )
                     ELSE
                        ZWORK( IZ15+I+(J-1)*N ) = 
     $                                          -ZWORK( IZ7+I+(J-1)*N )
                     END IF
  720             CONTINUE
  730          CONTINUE
C
C              Compute eig( lambda*diag(B)-A ).
C
               CALL ZGEES( 'N', 'N', SELECT, N, ZWORK( IZ15+1 ), N, 
     $                     SDIM, ZWORK( IZ16+1 ), ZWORK, N, 
     $                     ZWORK( IZWRK+1 ), LZWORK-IZWRK, 
     $                     DWORK( IWRK+1 ), BWORK, INFO2 )
               IF( INFO2.GT.0 ) THEN
                  INFO = 6
                  RETURN
               END IF
               LZA = INT( ZWORK( IZWRK+1 ) )
               LZAMAX = MAX( LZA, LZAMAX )
               EMIN = DREAL( ZWORK( IZ16+1 ) )
               IF( N.GT.1 ) THEN
                  DO 740 I = 2, N
                     IF( DREAL( ZWORK( IZ16+I ) ).LT.EMIN ) 
     $                  EMIN = DREAL( ZWORK( IZ16+I ) )
  740             CONTINUE
               END IF
               DO 750 I = 1, N
                  DWORK( IW30+I ) = DREAL( ZWORK( IZ16+I ) )
  750          CONTINUE
               DO 760 I = 1, M-1
                  DWORK( IW30+N+I ) = DWORK( IW22+I ) - BETA
                  DWORK( IW30+N+M-1+I ) = ALPHA - DWORK( IW22+I )
  760          CONTINUE
               IF( MR.GT.0 ) THEN
                  DO 770 I = 1, MR
                     DWORK( IW30+N+2*(M-1)+I ) = DWORK( IW23+I ) + TAU
                     DWORK( IW30+N+2*(M-1)+MR+I ) = TAU -  
     $                      DWORK( IW23+I )
  770             CONTINUE
               END IF
               PROD = ONE
               DO 780 I = 1, N+2*MT
                  PROD = PROD*DWORK( IW30+I ) 
  780          CONTINUE
               IF( EMIN.LE.ZERO .OR. ( -LOG( PROD ) ).GE.PHI ) THEN
                  STSIZE = STSIZE / TEN
               ELSE
                  CALL DCOPY( MT, DWORK( IW20+1 ), 1, X, 1 ) 
               END IF   
            END IF   
            IF( STSIZE.LT.TOL4 ) GO TO 810
  800    CONTINUE
C
  810    CONTINUE
C
C           Store xD.
C
            CALL DCOPY( M-1, X, 1, DWORK( IW22+1 ), 1 )
            IF( MR.GT.0 ) THEN
C
C              Store xG.
C
               CALL DCOPY( MR, X( M ), 1, DWORK( IW23+1 ), 1 )
            END IF
C
C           Compute A(:) = A0 + AA*x.
C
            DO 820 I = 1, MT
               ZWORK( IZ10+I ) = DCMPLX( X( I ) )
  820       CONTINUE
            CALL ZCOPY( N*N, ZWORK( IZ5+1 ), 1, ZWORK( IZ7+1 ), 1 )
            CALL ZGEMV( 'N', N*N, MT, CONE, ZWORK( IZ6+1 ), N*N, 
     $                  ZWORK( IZ10+1 ), 1, CONE, ZWORK( IZ7+1 ), 1 ) 
C
C           Compute diag( B ) = B0d + BBd*xD.
C
            CALL DCOPY( N, DWORK( IW7+1 ), 1, DWORK( IW24+1 ), 1 ) 
            CALL DGEMV( 'N', N, M-1, ONE, DWORK( IW8+1 ), N, 
     $                  DWORK( IW22+1 ), 1, ONE, DWORK( IW24+1 ), 1 ) 
C     
C           Compute F.
C
            DO 840 J = 1, N
               DO 830 I = 1, N
                  IF( I.EQ.J ) THEN
                     ZWORK( IZ15+I+(I-1)*N ) = DCMPLX( DLAMBD*
     $                      DWORK( IW24+I ) ) - ZWORK( IZ7+I+(I-1)*N )
                  ELSE
                     ZWORK( IZ15+I+(J-1)*N ) = -ZWORK( IZ7+I+(J-1)*N )
                  END IF
  830          CONTINUE
  840       CONTINUE
            CALL ZLACPY( 'Full', N, N, ZWORK( IZ15+1 ), N, 
     $                   ZWORK( IZ17+1 ), N )
C
C           Compute det( F ).
C
            CALL ZGEES( 'N', 'N', SELECT, N, ZWORK( IZ15+1 ), N, SDIM,
     $                  ZWORK( IZ16+1 ), ZWORK, N, ZWORK( IZWRK+1 ),
     $                  LZWORK-IZWRK, DWORK( IWRK+1 ), BWORK, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 6
               RETURN
            END IF
            LZA = INT( ZWORK( IZWRK+1 ) )
            LZAMAX = MAX( LZA, LZAMAX )
            DETF = CONE
            DO 850 I = 1, N
               DETF = DETF*ZWORK( IZ16+I )
  850       CONTINUE
C
C           Compute Finv.
C
            CALL ZGETRF( N, N, ZWORK( IZ17+1 ), N, IWORK, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 5
               RETURN
            END IF
            CALL ZGETRI( N, ZWORK( IZ17+1 ), N, IWORK, ZWORK( IZWRK+1 ),
     $                   LDWORK-IWRK, INFO2 )
            LZA = INT( ZWORK( IZWRK+1 ) )
            LZAMAX = MAX( LZA, LZAMAX )
C
C           Compute the barrier function.
C
            DO 860 I = 1, M-1
               DWORK( IW25+I ) = DWORK( IW22+I ) - BETA
               DWORK( IW25+M-1+I ) = ALPHA - DWORK( IW22+I )
  860       CONTINUE
            IF( MR.GT.0 ) THEN
               DO 870 I = 1, MR
                  DWORK( IW25+2*(M-1)+I ) = DWORK( IW23+I ) + TAU
                  DWORK( IW25+2*(M-1)+MR+I ) = TAU - DWORK( IW23+I )
  870          CONTINUE
            END IF
            PROD = ONE
            DO 880 I = 1, 2*MT
               PROD = PROD*DWORK( IW25+I ) 
  880       CONTINUE
            TEMP = DREAL( DETF )
            IF( TEMP.LT.EPS ) TEMP = EPS
            PHI = -LOG( TEMP ) - LOG( PROD )
C
C           Compute the gradient of the barrier function.
C
            DO 900 J = 1, MT
               DO 890 I = 1, N*N
                  ZWORK( IZ18+I+(J-1)*N*N ) = DCMPLX( DLAMBD*
     $            DWORK(  IW9+I+(J-1)*N*N ) ) - ZWORK( IZ6+I+(J-1)*N*N )
  890          CONTINUE
  900       CONTINUE     
            CALL ZGEMV( 'C', N*N, MT, CONE, ZWORK( IZ18+1 ), N*N, 
     $                  ZWORK( IZ17+1 ), 1, CZERO, ZWORK( IZ19+1 ), 1 )
            DO 910 I = 1, M-1
               DWORK( IW26+I ) = ONE / ( DWORK( IW22+I ) - BETA ) -
     $                           ONE / ( ALPHA - DWORK( IW22+I ) )
  910       CONTINUE
            IF( MR.GT.0 ) THEN
               DO 920 I = 1, MR
                  DWORK( IW26+M-1+I ) = ONE / ( DWORK( IW23+I ) + TAU )
     $                                 -ONE / ( TAU - DWORK( IW23+I ) )
  920          CONTINUE
            END IF
            DO 925 I = 1, MT
               DWORK( IW26+I ) = -DREAL( ZWORK( IZ19+I ) ) -
     $                                                 DWORK( IW26+I )
  925       CONTINUE
C
C           Compute the Hessian of the barrier function.
C
            CALL ZGEMM( 'N', 'N', N, N*MT, N, CONE, ZWORK( IZ17+1 ), N, 
     $                  ZWORK( IZ18+1 ), N, CZERO, ZWORK( IZ20+1 ), N ) 

            CALL  DLASET( 'Full', MT, MT, ZERO, ZERO, DWORK( IW11+1 ), 
     $                    MT ) 
            DO 960 K = 1, MT
               CALL ZCOPY( N*N, ZWORK( IZ20+1+(K-1)*N*N ), 1, 
     $                          ZWORK( IZ22+1 ), 1 )
               DO 940 J = 1, N
                  DO 930 I = 1, N
                     ZWORK( IZ23+I+(J-1)*N ) = 
     $                              DCONJG( ZWORK( IZ22+J+(I-1)*N ) )
  930             CONTINUE
  940          CONTINUE                   
               CALL ZGEMV( 'C', N*N, K, CONE, ZWORK( IZ20+1 ), N*N, 
     $                     ZWORK( IZ23+1 ), 1, CZERO, ZWORK( IZ24+1 ), 
     $                     1 )
               DO 950 J = 1, K
                  DWORK( IW11+K+(J-1)*MT ) = 
     $                               DREAL( DCONJG( ZWORK( IZ24+J ) ) )
  950          CONTINUE
  960       CONTINUE
            DO 970 I = 1, M-1
               DWORK( IW10+I ) = ONE / ( DWORK( IW22+I ) - BETA )**2 +
     $                           ONE / ( ALPHA - DWORK( IW22+I ) )**2
  970       CONTINUE
            IF( MR.GT.0 ) THEN
               DO 980 I = 1, MR
                  DWORK( IW10+M-1+I ) = 
     $                           ONE / ( DWORK( IW23+I ) + TAU )**2 +
     $                           ONE / ( TAU - DWORK( IW23+I ) )**2
  980          CONTINUE
            END IF
            DO 990 I = 1, MT
               DWORK( IW11+I+(I-1)*MT ) = DWORK( IW11+I+(I-1)*MT ) +
     $                                    DWORK( IW10+I )
  990       CONTINUE
            DO 1100 J = 1, MT
               DO 1000 I = 1, J
                  IF( I.NE.J ) THEN
                     T1 = DWORK( IW11+I+(J-1)*MT ) 
                     T2 = DWORK( IW11+J+(I-1)*MT )
                     DWORK( IW11+I+(J-1)*MT ) = T1 + T2
                     DWORK( IW11+J+(I-1)*MT ) = T1 + T2
                  END IF
 1000          CONTINUE
 1100       CONTINUE
C
C           Compute norm( H ).
C
 1110       HNORM = DLANGE( 'F', MT, MT, DWORK( IW11+1 ), MT, DWORK )
C
C           Compute rcond( H ).
C
            CALL DLACPY( 'Full', MT, MT, DWORK( IW11+1 ), MT, 
     $                   DWORK( IW31+1 ), MT )           
            HNORM1 = DLANGE( '1', MT, MT, DWORK( IW31+1 ), MT, DWORK ) 
            CALL DSYTRF( 'U', MT, DWORK( IW31+1 ), MT, IWORK, 
     $                   DWORK( IWRK+1 ), LDWORK-IWRK, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 5
               RETURN
            END IF
            LWA = INT( DWORK( IWRK+1 ) )
            LWAMAX = MAX( LWA, LWAMAX )
            CALL DSYCON( 'U', MT, DWORK( IW31+1 ), MT, IWORK, HNORM1,
     $                  RCOND, DWORK( IWRK+1 ), IWORK( MT+1 ), INFO2 )
            IF( RCOND.LT.TOL3 ) THEN
               DO 1120 I = 1, MT 
                  DWORK( IW11+I+(I-1)*MT ) = DWORK( IW11+I+(I-1)*MT ) + 
     $                                       HNORM*REGPAR
 1120          CONTINUE
               GO TO 1110
            END IF
C
C           Compute the tangent line to path of center.
C
            CALL DCOPY( MT, DWORK( IW26+1 ), 1, DWORK( IW27+1 ), 1 )
            CALL DSYTRS( 'U', MT, 1, DWORK( IW31+1 ), MT, IWORK, 
     $                   DWORK( IW27+1 ), MT, INFO2 )            
C            
C           Check if x-h satisfies the Goldstein test.
C
            GTEST = .FALSE.
            DO 1130 I = 1, MT
               DWORK( IW20+I ) = X( I ) - DWORK( IW27+I )
 1130       CONTINUE
C
C           Store xD.
C
            CALL DCOPY( M-1, DWORK( IW20+1 ), 1, DWORK( IW22+1 ), 1 )
            IF( MR.GT.0 ) THEN
C
C              Store xG.
C
               CALL DCOPY( MR, DWORK( IW20+M ), 1, DWORK( IW23+1 ), 1 )
            END IF
C
C           Compute A(:) = A0 + AA*x_new.
C
            DO 1140 I = 1, MT
               ZWORK( IZ10+I ) = DCMPLX( DWORK( IW20+I ) )
 1140       CONTINUE
            CALL ZCOPY( N*N, ZWORK( IZ5+1 ), 1, ZWORK( IZ7+1 ), 1 )
            CALL ZGEMV( 'N', N*N, MT, CONE, ZWORK( IZ6+1 ), N*N, 
     $                  ZWORK( IZ10+1 ), 1, CONE, ZWORK( IZ7+1 ), 1 ) 
C
C           Compute diag( B ) = B0d + BBd*xD.
C
            CALL DCOPY( N, DWORK( IW7+1 ), 1, DWORK( IW24+1 ), 1 ) 
            CALL DGEMV( 'N', N, M-1, ONE, DWORK( IW8+1 ), N, 
     $                  DWORK( IW22+1 ), 1, ONE, DWORK( IW24+1 ), 1 ) 
C
C           Compute lambda*diag(B) - A.
C
            DO 1160 J = 1, N
               DO 1150 I = 1, N
                  IF( I.EQ.J ) THEN
                     ZWORK( IZ15+I+(I-1)*N ) = DCMPLX( DLAMBD*
     $                     DWORK( IW24+I ) ) - ZWORK( IZ7+I+(I-1)*N )
                  ELSE
                     ZWORK( IZ15+I+(J-1)*N ) = -ZWORK( IZ7+I+(J-1)*N ) 
                  END IF
 1150          CONTINUE
 1160       CONTINUE
C
C           Compute eig( lambda*diag(B)-A ).
C
            CALL ZGEES( 'N', 'N', SELECT, N, ZWORK( IZ15+1 ), N, SDIM,
     $                  ZWORK( IZ16+1 ), ZWORK, N, ZWORK( IZWRK+1 ),
     $                  LZWORK-IZWRK, DWORK( IWRK+1 ), BWORK, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 6
               RETURN
            END IF
            LZA = INT( ZWORK( IZWRK+1 ) )
            LZAMAX = MAX( LZA, LZAMAX )
            DO 1190 I = 1, N
               DWORK( IW30+I ) = DREAL( ZWORK( IZ16+I ) )
 1190       CONTINUE
            DO 1200 I = 1, M-1
               DWORK( IW30+N+I ) = DWORK( IW22+I ) - BETA
               DWORK( IW30+N+M-1+I ) = ALPHA - DWORK( IW22+I )
 1200       CONTINUE
            IF( MR.GT.0 ) THEN
               DO 1210 I = 1, MR
                  DWORK( IW30+N+2*(M-1)+I ) = DWORK( IW23+I ) + TAU
                  DWORK( IW30+N+2*(M-1)+MR+I ) = TAU - DWORK( IW23+I ) 
 1210          CONTINUE
            END IF
            EMIN = DWORK( IW30+1 )
            DO 1220 I = 1, N+2*MT
               IF( DWORK( IW30+I ).LT.EMIN ) EMIN = DWORK( IW30+I )
 1220       CONTINUE
            IF( EMIN.LE.ZERO ) THEN
               GTEST = .FALSE.
            ELSE
               PP = DDOT( MT, DWORK( IW26+1 ), 1, DWORK( IW27+1 ), 1 )
               PROD = ONE
               DO 1230 I = 1, N+2*MT
                  PROD = PROD*DWORK( IW30+I ) 
 1230          CONTINUE
               T1 = -LOG( PROD )
               T2 = PHI - C2*PP
               T3 = PHI - C4*PP
               IF( T1.GE.T3 .AND. T1.LT.T2 ) GTEST = .TRUE.
            END IF
C
C           Use x-h if Goldstein test is satisfied. Otherwise use
C           Nesterov-Nemirovsky's stepsize length.
C
            PP = DDOT( MT, DWORK( IW26+1 ), 1, DWORK( IW27+1 ), 1 )
            DELTA = SQRT( PP )
            IF( GTEST .OR. DELTA.LE.C3 ) THEN
               DO 1240 I = 1, MT
                  X( I ) = X( I ) - DWORK( IW27+I )
 1240          CONTINUE
            ELSE     
               DO 1250 I = 1, MT
                  X( I ) = X( I ) - DWORK( IW27+I ) / ( ONE + DELTA )
 1250          CONTINUE
            END IF    
C
C           Analytic center is found if delta is sufficiently small.
C
            IF( DELTA.LT.TOL5 ) GO TO 1260 
         GO TO 810
C         
C        Set yf.
C
 1260    DWORK( IW14+1 ) = DLAMBD
         CALL DCOPY( MT, X, 1, DWORK( IW14+2 ), 1 )
C    
C        Set yw.
C
         CALL DCOPY( MT+1, DWORK( IW14+1 ), 1, DWORK( IW15+1 ), 1 )
C
C        Compute Fb.
C
         DO 1280 J = 1, N         
            DO 1270 I = 1, N
               ZWORK( IZ21+I+(J-1)*N ) = DCMPLX( DWORK( IW24+I ) )*
     $                                 DCONJG( ZWORK( IZ17+J+(I-1)*N ) )
 1270       CONTINUE
 1280    CONTINUE
         CALL ZGEMV( 'C', N*N, MT, CONE, ZWORK( IZ20+1 ), N*N, 
     $               ZWORK( IZ21+1 ), 1, CZERO, ZWORK( IZ24+1 ), 1 )
         DO 1300 I = 1, MT
            DWORK( IW32+I ) = DREAL( ZWORK( IZ24+I ) )
 1300    CONTINUE
C
C        Compute h1.
C
         CALL DLACPY( 'Full', MT, MT, DWORK( IW11+1 ), MT, 
     $                DWORK( IW31+1 ), MT )           
         CALL DSYSV( 'U', MT, 1, DWORK( IW31+1 ), MT, IWORK, 
     $               DWORK( IW32+1 ), MT, DWORK( IWRK+1 ),
     $               LDWORK-IWRK, INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 5
            RETURN
         END IF
         LWA = INT( DWORK( IWRK+1 ) )
         LWAMAX = MAX( LWA, LWAMAX )
C
C        Compute hn.
C
         HN = DLANGE( 'F', MT, 1, DWORK( IW32+1 ), MT, DWORK )
C
C        Compute y.
C
         DWORK( IW13+1 ) = DLAMBD - C / HN
         DO 1310 I = 1, MT
            DWORK( IW13+1+I ) = X( I ) + C*DWORK( IW32+I ) / HN
 1310    CONTINUE
C
C        Store xD.
C
         CALL DCOPY( M-1, DWORK( IW13+2 ), 1, DWORK( IW22+1 ), 1 )
         IF( MR.GT.0 ) THEN
C
C           Store xG.
C
            CALL DCOPY( MR, DWORK( IW13+M+1 ), 1, DWORK( IW23+1 ), 1 )
         END IF
C
C        Compute A(:) = A0 + AA*y(2:mt+1).
C
         DO 1320 I = 1, MT
            ZWORK( IZ10+I ) = DCMPLX( DWORK( IW13+1+I ) )
 1320    CONTINUE
         CALL ZCOPY( N*N, ZWORK( IZ5+1 ), 1, ZWORK( IZ7+1 ), 1 )
         CALL ZGEMV( 'N', N*N, MT, CONE, ZWORK( IZ6+1 ), N*N, 
     $               ZWORK( IZ10+1 ), 1, CONE, ZWORK( IZ7+1 ), 1 )      
C
C        Compute B = B0d + BBd*xD.
C
         CALL DCOPY( N, DWORK( IW7+1 ), 1, DWORK( IW24+1 ), 1 )         
         CALL DGEMV( 'N', N, M-1, ONE, DWORK( IW8+1 ), N, 
     $               DWORK( IW22+1 ), 1, ONE, DWORK( IW24+1 ), 1 ) 
C
C        Compute y(1)*diag(B) - A.
C
         DO 1340 J = 1, N
            DO 1330 I = 1, N
               IF( I.EQ.J ) THEN
                  ZWORK( IZ15+I+(I-1)*N ) = DCMPLX( DWORK( IW13+1 )*
     $                   DWORK( IW24+I ) ) - ZWORK( IZ7+I+(I-1)*N )
               ELSE
                  ZWORK( IZ15+I+(J-1)*N ) = -ZWORK( IZ7+I+(J-1)*N )
               END IF
 1330       CONTINUE
 1340    CONTINUE
C
C        Compute eig( y(1)*diag(B)-A ).
C
         CALL ZGEES( 'N', 'N', SELECT, N, ZWORK( IZ15+1 ), N, SDIM,
     $               ZWORK( IZ16+1 ), ZWORK, N, ZWORK( IZWRK+1 ),
     $               LZWORK-IZWRK, DWORK( IWRK+1 ), BWORK, INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 6
            RETURN
         END IF
         LZA = INT( ZWORK( IZWRK+1 ) )
         LZAMAX = MAX( LZA, LZAMAX )
         EMIN = DREAL( ZWORK( IZ16+1 ) )
         IF( N.GT.1 ) THEN
            DO 1350 I = 2, N
               IF( DREAL( ZWORK( IZ16+I ) ).LT.EMIN )
     $            EMIN = DREAL( ZWORK( IZ16+I ) )
 1350       CONTINUE
         END IF
         POS = .TRUE.
         DO 1360 I = 1, M-1
            DWORK( IW25+I ) = DWORK( IW22+I ) - BETA
            DWORK( IW25+M-1+I ) = ALPHA - DWORK( IW22+I )
 1360    CONTINUE
         IF( MR.GT.0 ) THEN
            DO 1370 I = 1, MR
               DWORK( IW25+2*(M-1)+I ) = DWORK( IW23+I ) + TAU
               DWORK( IW25+2*(M-1)+MR+I ) = TAU - DWORK( IW23+I )
 1370       CONTINUE
         END IF
         TEMP = DWORK( IW25+1 )
         DO 1380 I = 2, 2*MT
            IF( DWORK( IW25+I ).LT.TEMP ) TEMP = DWORK( IW25+I )
 1380    CONTINUE
         IF( TEMP.LE.ZERO .OR. EMIN.LE.ZERO ) POS = .FALSE.
 1390    IF( POS ) THEN
C            
C           Set y2 = y.
C
            CALL DCOPY( MT+1, DWORK( IW13+1 ), 1, DWORK( IW17+1 ), 1 ) 
C            
C           Compute y = y + 1.5*( y - yw ).
C
            DO 1400 I = 1, MT+1
               DWORK( IW13+I ) = DWORK( IW13+I ) + 
     $                 C5*( DWORK( IW13+I ) - DWORK( IW15+I ) )
 1400       CONTINUE
C
C           Store xD.
C
            CALL DCOPY( M-1, DWORK( IW13+2 ), 1, DWORK( IW22+1 ), 1 )
            IF( MR.GT.0 ) THEN
C
C              Store xG.
C
               CALL DCOPY( MR, DWORK( IW13+M+1 ), 1, 
     $                                            DWORK( IW23+1 ), 1 )
            END IF
C
C           Compute A(:) = A0 + AA*y(2:mt+1).
C
            DO 1420 I = 1, MT
               ZWORK( IZ10+I ) = DCMPLX( DWORK( IW13+1+I ) )
 1420       CONTINUE
            CALL ZCOPY( N*N, ZWORK( IZ5+1 ), 1, ZWORK( IZ7+1 ), 1 )
            CALL ZGEMV( 'N', N*N, MT, CONE, ZWORK( IZ6+1 ), N*N, 
     $                  ZWORK( IZ10+1 ), 1, CONE, ZWORK( IZ7+1 ), 1 ) 
C
C           Compute diag( B ) = B0d + BBd*xD.
C
            CALL DCOPY( N, DWORK( IW7+1 ), 1, DWORK( IW24+1 ), 1 ) 
            CALL DGEMV( 'N', N, M-1, ONE, DWORK( IW8+1 ), N, 
     $                  DWORK( IW22+1 ), 1, ONE, DWORK( IW24+1 ), 1 ) 
C
C           Set yw = y2.
C
            CALL DCOPY( MT+1, DWORK( IW17+1 ), 1, DWORK( IW15+1 ), 1 )
C
C           Compute y(1)*diag(B) - A.
C
            DO 1440 J = 1, N
               DO 1430 I = 1, N
                  IF( I.EQ.J ) THEN
                     ZWORK( IZ15+I+(I-1)*N ) = DCMPLX( DWORK( IW13+1 )*
     $                      DWORK( IW24+I ) ) - ZWORK( IZ7+I+(I-1)*N )
                  ELSE
                     ZWORK( IZ15+I+(J-1)*N ) = -ZWORK( IZ7+I+(J-1)*N )
                  END IF
 1430          CONTINUE
 1440       CONTINUE
C
C           Compute eig( y(1)*diag(B)-A ).
C
            CALL ZGEES( 'N', 'N', SELECT, N, ZWORK( IZ15+1 ), N, SDIM,
     $                  ZWORK( IZ16+1 ), ZWORK, N, ZWORK( IZWRK+1 ),
     $                  LZWORK-IZWRK, DWORK( IWRK+1 ), BWORK, INFO2 )
            IF( INFO2.GT.0 ) THEN
               INFO = 6
               RETURN
            END IF
            LZA = INT( ZWORK( IZWRK+1 ) )
            LZAMAX = MAX( LZA, LZAMAX )
            EMIN = DREAL( ZWORK( IZ16+1 ) )
            IF( N.GT.1 ) THEN
               DO 1450 I = 2, N
                  IF( DREAL( ZWORK( IZ16+I ) ).LT.EMIN ) 
     $               EMIN = DREAL( ZWORK( IZ16+I ) )
 1450          CONTINUE
            END IF
            POS = .TRUE.
            DO 1460 I = 1, M-1
               DWORK( IW25+I ) = DWORK( IW22+I ) - BETA
               DWORK( IW25+M-1+I ) = ALPHA - DWORK( IW22+I )
 1460       CONTINUE
            IF( MR.GT.0 ) THEN
               DO 1470 I = 1, MR
                  DWORK( IW25+2*(M-1)+I ) = DWORK( IW23+I ) + TAU
                  DWORK( IW25+2*(M-1)+MR+I ) = TAU - DWORK( IW23+I )
 1470          CONTINUE
            END IF
            TEMP = DWORK( IW25+1 )
            DO 1480 I = 2, 2*MT
               IF( DWORK( IW25+I ).LT.TEMP ) TEMP = DWORK( IW25+I )
 1480       CONTINUE
            IF( TEMP.LE.ZERO .OR. EMIN.LE.ZERO ) POS = .FALSE.
            GO TO 1390
         END IF
 1490    CONTINUE
C
C        Set y1 = ( y + yw ) / 2.
C
         DO 1500 I = 1, MT+1
            DWORK( IW16+I ) = ( DWORK( IW13+I ) + DWORK( IW15+I ) ) 
     $                         / TWO
 1500    CONTINUE
C
C        Store xD.
C
         CALL DCOPY( M-1, DWORK( IW16+2 ), 1, DWORK( IW22+1 ), 1 )
         IF( MR.GT.0 ) THEN
C
C           Store xG.
C
            CALL DCOPY( MR, DWORK( IW16+M+1 ), 1, DWORK( IW23+1 ), 1 )
         END IF
C
C        Compute A(:) = A0 + AA*y1(2:mt+1).
C
         DO 1510 I = 1, MT
            ZWORK( IZ10+I ) = DCMPLX( DWORK( IW16+1+I ) )
 1510    CONTINUE
         CALL ZCOPY( N*N, ZWORK( IZ5+1 ), 1, ZWORK( IZ7+1 ), 1 )
         CALL ZGEMV( 'N', N*N, MT, CONE, ZWORK( IZ6+1 ), N*N, 
     $               ZWORK( IZ10+1 ), 1, CONE, ZWORK( IZ7+1 ), 1 )      
C
C        Compute diag( B ) = B0d + BBd*xD.
C
         CALL DCOPY( N, DWORK( IW7+1 ), 1, DWORK( IW24+1 ), 1 )         
         CALL DGEMV( 'N', N, M-1, ONE, DWORK( IW8+1 ), N, 
     $               DWORK( IW22+1 ), 1, ONE, DWORK( IW24+1 ), 1 ) 
C
C        Compute y1(1)*diag(B) - A.
C
         DO 1530 J = 1, N
            DO 1520 I = 1, N
               IF( I.EQ.J ) THEN
                  ZWORK( IZ15+I+(I-1)*N ) = DCMPLX( DWORK( IW16+1 )*
     $                   DWORK( IW24+I ) ) - ZWORK( IZ7+I+(I-1)*N )
               ELSE
                  ZWORK( IZ15+I+(J-1)*N ) = -ZWORK( IZ7+I+(J-1)*N )
               END IF
 1520       CONTINUE
 1530    CONTINUE
C
C        Compute eig( y1(1)*diag(B)-A ).
C
         CALL ZGEES( 'N', 'N', SELECT, N, ZWORK( IZ15+1 ), N, SDIM,
     $               ZWORK( IZ16+1 ), ZWORK, N, ZWORK( IZWRK+1 ),
     $               LZWORK-IZWRK, DWORK( IWRK+1 ), BWORK, INFO2 )
         IF( INFO2.GT.0 ) THEN
            INFO = 6
            RETURN
         END IF
         LZA = INT( ZWORK( IZWRK+1 ) )
         LZAMAX = MAX( LZA, LZAMAX )
         EMIN = DREAL( ZWORK( IZ16+1 ) )
         IF( N.GT.1 ) THEN
            DO 1540 I = 2, N
               IF( DREAL( ZWORK( IZ16+I ) ).LT.EMIN ) 
     $            EMIN = DREAL( ZWORK( IZ16+I ) )
 1540       CONTINUE
         END IF
         POS = .TRUE.
         DO 1550 I = 1, M-1
            DWORK( IW25+I ) = DWORK( IW22+I ) - BETA
            DWORK( IW25+M-1+I ) = ALPHA - DWORK( IW22+I )
 1550    CONTINUE
         IF( MR.GT.0 ) THEN
            DO 1560 I = 1, MR
               DWORK( IW25+2*(M-1)+I ) = DWORK( IW23+I ) + TAU
               DWORK( IW25+2*(M-1)+MR+I ) = TAU - DWORK( IW23+I )
 1560       CONTINUE
         END IF
         TEMP = DWORK( IW25+1 )
         DO 1570 I = 2, 2*MT
            IF( DWORK( IW25+I ).LT.TEMP ) TEMP = DWORK( IW25+I )
 1570    CONTINUE
         IF( TEMP.LE.ZERO .OR. EMIN.LE.ZERO ) POS = .FALSE.
         IF( POS ) THEN
C
C           Set yw = y1.
C
            CALL DCOPY( MT+1, DWORK( IW16+1 ), 1, DWORK( IW15+1 ), 1 )
         ELSE
C            
C           Set y = y1.
C
            CALL DCOPY( MT+1, DWORK( IW16+1 ), 1, DWORK( IW13+1 ), 1 )
         END IF
         DO 1580 I = 1, MT+1
            DWORK( IW33+I ) = DWORK( IW13+I ) - DWORK( IW15+I ) 
 1580    CONTINUE         
         YNORM1 = DLANGE( 'F', MT+1, 1, DWORK( IW33+1 ), MT+1, DWORK )
         DO 1590 I = 1, MT+1
            DWORK( IW33+I ) = DWORK( IW13+I ) - DWORK( IW14+I ) 
 1590    CONTINUE         
         YNORM2 = DLANGE( 'F', MT+1, 1, DWORK( IW33+1 ), MT+1, DWORK )
         IF( YNORM1.LT.YNORM2*THETA ) GO TO 1600
         GO TO 1490
C
C        Compute c.
C
 1600    DO 1610 I = 1, MT+1
            DWORK( IW33+I ) = DWORK( IW15+I ) - DWORK( IW14+I ) 
 1610    CONTINUE         
         C = DLANGE( 'F', MT+1, 1, DWORK( IW33+1 ), MT+1, DWORK )  
C
C        Set x = yw(2:mt+1).
C
         CALL DCOPY( MT, DWORK( IW15+2 ), 1, X, 1 )
      GO TO 390
C
C *** Last line of AB13MD ***
      END
