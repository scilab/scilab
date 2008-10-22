      SUBROUTINE IB01MY( METH, BATCH, CONCT, NOBR, M, L, NSMP, U, LDU,
     $                   Y, LDY, R, LDR, IWORK, DWORK, LDWORK, IWARN, 
     $                   INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To construct an upper triangular factor  R  of the concatenated
C     block Hankel matrices using input-output data, via a fast QR 
C     algorithm based on displacement rank.  The input-output data can,
C     optionally, be processed sequentially.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     METH    CHARACTER*1
C             Specifies the subspace identification method to be used,
C             as follows:
C             = 'M':  MOESP  algorithm with past inputs and outputs;
C             = 'N':  N4SID  algorithm.
C
C     BATCH   CHARACTER*1
C             Specifies whether or not sequential data processing is to
C             be used, and, for sequential processing, whether or not
C             the current data block is the first block, an intermediate
C             block, or the last block, as follows:
C             = 'F':  the first block in sequential data processing;
C             = 'I':  an intermediate block in sequential data
C                     processing;
C             = 'L':  the last block in sequential data processing;
C             = 'O':  one block only (non-sequential data processing).
C             NOTE that when  100  cycles of sequential data processing
C                  are completed for  BATCH = 'I',  a warning is
C                  issued, to prevent for an infinite loop.
C
C     CONCT   CHARACTER*1
C             Specifies whether or not the successive data blocks in
C             sequential data processing belong to a single experiment,
C             as follows:
C             = 'C':  the current data block is a continuation of the
C                     previous data block and/or it will be continued
C                     by the next data block;
C             = 'N':  there is no connection between the current data
C                     block and the previous and/or the next ones.
C             This parameter is not used if BATCH = 'O'.
C
C     Input/Output Parameters
C
C     NOBR    (input) INTEGER
C             The number of block rows,  s,  in the input and output
C             block Hankel matrices to be processed.  NOBR > 0.
C             (In the MOESP theory,  NOBR  should be larger than  n, the
C             estimated dimension of state vector.)
C
C     M       (input) INTEGER
C             The number of system inputs.  M >= 0.
C             When M = 0, no system inputs are processed.
C
C     L       (input) INTEGER
C             The number of system outputs.  L > 0.
C
C     NSMP    (input) INTEGER
C             The number of rows of matrices  U  and  Y  (number of
C             samples,  t). (When sequential data processing is used,
C             NSMP  is the number of samples of the current data
C             block.)
C             NSMP >= 2*(M+L+1)*NOBR - 1,  for non-sequential
C                                          processing;
C             NSMP >= 2*NOBR,  for sequential processing.
C             The total number of samples when calling the routine with
C             BATCH = 'L'  should be at least  2*(M+L+1)*NOBR - 1.
C             The  NSMP  argument may vary from a cycle to another in
C             sequential data processing, but  NOBR, M,  and  L  should
C             be kept constant. For efficiency, it is advisable to use
C             NSMP  as large as possible.
C
C     U       (input) DOUBLE PRECISION array, dimension (LDU,M)
C             The leading NSMP-by-M part of this array must contain the
C             t-by-m input-data sequence matrix  U,
C             U = [u_1 u_2 ... u_m].  Column  j  of  U  contains the
C             NSMP  values of the j-th input component for consecutive
C             time increments.
C             If M = 0, this array is not referenced.
C
C     LDU     INTEGER
C             The leading dimension of the array U.
C             LDU >= NSMP, if M > 0;
C             LDU >= 1,    if M = 0.
C
C     Y       (input) DOUBLE PRECISION array, dimension (LDY,L)
C             The leading NSMP-by-L part of this array must contain the
C             t-by-l output-data sequence matrix  Y,
C             Y = [y_1 y_2 ... y_l].  Column  j  of  Y  contains the
C             NSMP  values of the j-th output component for consecutive
C             time increments.
C
C     LDY     INTEGER
C             The leading dimension of the array Y.  LDY >= NSMP.
C
C     R       (output) DOUBLE PRECISION array, dimension
C             ( LDR,2*(M+L)*NOBR )
C             If INFO = 0 and BATCH = 'L' or 'O', the leading 
C             2*(M+L)*NOBR-by-2*(M+L)*NOBR upper triangular part of this
C             array contains the upper triangular factor R from the
C             QR factorization of the concatenated block Hankel
C             matrices.
C
C     LDR     INTEGER
C             The leading dimension of the array  R.
C             LDR >= 2*(M+L)*NOBR.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (M+L)
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if  INFO = 0,  DWORK(1)  returns the optimal
C             value of LDWORK.
C             On exit, if  INFO = -16,  DWORK(1)  returns the minimum
C             value of LDWORK.
C             The first (M+L)*2*NOBR*(M+L+c) elements of  DWORK  should
C             be preserved during successive calls of the routine
C             with  BATCH = 'F'  or  'I',  till the final call with 
C             BATCH = 'L',  where 
C             c = 1,  if the successive data blocks do not belong to a
C                     single experiment  (CONCT = 'N');
C             c = 2,  if the successive data blocks belong to a single
C                     experiment  (CONCT = 'C').
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= (M+L)*2*NOBR*(M+L+3),
C                              if BATCH <> 'O' and CONCT = 'C';
C             LDWORK >= (M+L)*2*NOBR*(M+L+1),
C                              if BATCH = 'F' or 'I' and CONCT = 'N';
C             LDWORK >= (M+L)*4*NOBR*(M+L+1)+(M+L)*2*NOBR,  
C                              if BATCH = 'L' and CONCT = 'N',
C                              or BATCH = 'O'.
C
C     Warning Indicator
C
C     IWARN   INTEGER
C             = 0:  no warning;
C             = 1:  the number of 100 cycles in sequential data
C                   processing has been exhausted without signaling
C                   that the last block of data was get.
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  the fast QR factorization algorithm failed. The
C                   matrix H'*H is not (numerically) positive definite.
C                   
C     METHOD
C
C     Consider the  t x 2(m+l)s  matrix H of concatenated block Hankel
C     matrices 
C
C          H = [ Uf'         Up'      Y'      ],  for METH = 'M',
C                  s+1,2s,t    1,s,t   1,2s,t
C
C          H = [ U'       Y'      ],              for METH = 'N',
C                 1,2s,t   1,2s,t
C
C     where  Up     , Uf        , U      , and  Y        are block
C              1,s,t    s+1,2s,t   1,2s,t        1,2s,t
C     Hankel matrices defined in terms of the input and output data [3].
C     The fast QR algorithm uses a factorization of H'*H which exploits 
C     the block-Hankel structure, via a displacement rank technique [5].
C
C     REFERENCES
C
C     [1] Verhaegen M., and Dewilde, P.
C         Subspace Model Identification. Part 1: The output-error 
C         state-space model identification class of algorithms.
C         Int. J. Control, 56, pp. 1187-1210, 1992.
C
C     [2] Verhaegen M.
C         Subspace Model Identification. Part 3: Analysis of the
C         ordinary output-error state-space model identification
C         algorithm.
C         Int. J. Control, 58, pp. 555-586, 1993.
C
C     [3] Verhaegen M.
C         Identification of the deterministic part of MIMO state space
C         models given in innovations form from input-output data.
C         Automatica, Vol.30, No.1, pp.61-74, 1994.
C
C     [4] Van Overschee, P., and De Moor, B.
C         N4SID: Subspace Algorithms for the Identification of
C         Combined Deterministic-Stochastic Systems.
C         Automatica, Vol.30, No.1, pp. 75-93, 1994.
C
C     [5] Kressner, D., Mastronardi, N., Sima, V., Van Dooren, P., and
C         Van Huffel, S.
C         A Fast Algorithm for Subspace State-space System
C         Identification via Exploitation of the Displacement Structure.
C         J. Comput. Appl. Math., 2000 (submitted).
C
C     NUMERICAL ASPECTS
C
C     The implemented method is reliable and efficient. Numerical
C     difficulties are possible when the matrix H'*H is nearly rank 
C     defficient. The method cannot be used if the matrix H'*H is not
C     numerically positive definite.
C                                     2           3 2 
C     The algorithm requires 0(2t(m+l) s)+0(4(m+l) s ) floating point
C     operations.
C
C     CONTRIBUTORS
C
C     V. Sima, Katholieke Universiteit Leuven, June 2000.
C     Partly based on Matlab codes developed by N. Mastronardi, 
C     Katholieke Universiteit Leuven, February 2000.
C
C     REVISIONS
C
C     V. Sima, July 2000, August 2000.
C
C     KEYWORDS
C
C     Displacement rank, Hankel matrix, Householder transformation, 
C     identification methods, multivariable systems.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION   ZERO, ONE
      PARAMETER          ( ZERO = 0.0D0, ONE = 1.0D0 )
      INTEGER            MAXCYC
      PARAMETER          ( MAXCYC = 100 )
C     .. Scalar Arguments ..
      INTEGER            INFO, IWARN, L, LDR, LDU, LDWORK, LDY, M, NOBR,
     $                   NSMP
      CHARACTER          BATCH, CONCT, METH
C     .. Array Arguments ..
      INTEGER            IWORK(*)
      DOUBLE PRECISION   DWORK(*), R(LDR, *), U(LDU, *), Y(LDY, *)
C     .. Local Scalars ..
      DOUBLE PRECISION   BETA, CS, SN, UPD, TAU
      INTEGER            I, ICJ, ICOL, ICONN, ICYCLE, IERR, IMAX, ING, 
     $                   INGC, INGP, IPG, IPGC, IPY, IREV, ITAU, J, JD,
     $                   JDS, JWORK, K, LDRWRK, LLNOBR, LNOBR, LNRG,
     $                   MAXWRK, MINWRK, MMNOBR, MNOBR, MNRG, NOBR2,
     $                   NOBR21, NR, NRG, NS, NSM, NSMPSM
      LOGICAL            CONNEC, FIRST, INTERM, LAST, MOESP, N4SID,
     $                   ONEBCH
C     .. Local Arrays ..
      DOUBLE PRECISION   DUM(1)
C     .. External Functions ..
      LOGICAL            LSAME 
      INTEGER            IDAMAX
      EXTERNAL           IDAMAX, LSAME
C     .. External Subroutines ..
      EXTERNAL           DCOPY, DGEMM, DGEQRF, DLACPY, DLARF, DLARFG,
     $                   DLASET, DORMQR, DSCAL, DSWAP, DSYRK, MA02ED, 
     $                   MA02FD, MB04ID, MB04OD, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          ABS, INT, MAX, SQRT
C     .. Save Statement ..
C        ICYCLE  is used to count the cycles for  BATCH = 'I'.
C        MAXWRK  is used to store the optimal workspace.
C        NSMPSM  is used to sum up the  NSMP  values for  BATCH <> 'O'.
      SAVE               ICYCLE, MAXWRK, NSMPSM
C     ..
C     .. Executable Statements ..
C
C     Decode the scalar input parameters.
C
      MOESP  = LSAME( METH,  'M' )
      N4SID  = LSAME( METH,  'N' )
      ONEBCH = LSAME( BATCH, 'O' )
      FIRST  = LSAME( BATCH, 'F' ) .OR. ONEBCH
      INTERM = LSAME( BATCH, 'I' )
      LAST   = LSAME( BATCH, 'L' ) .OR. ONEBCH
      IF( .NOT.ONEBCH ) THEN
         CONNEC = LSAME( CONCT, 'C' )
      ELSE
         CONNEC = .FALSE.
      END IF
      MNOBR  = M*NOBR
      LNOBR  = L*NOBR
      MMNOBR = MNOBR + MNOBR
      LLNOBR = LNOBR + LNOBR
      NOBR2  = 2*NOBR 
      NOBR21 = NOBR2 - 1
      IWARN  = 0
      INFO   = 0
      IF( FIRST ) THEN
         ICYCLE = 1
         MAXWRK = 1
         NSMPSM = 0
      END IF
      NSMPSM = NSMPSM + NSMP
      NR     = MMNOBR + LLNOBR
C
C     Check the scalar input parameters.
C
      IF( .NOT.( MOESP .OR. N4SID ) ) THEN
         INFO = -1
      ELSE IF( .NOT.( FIRST .OR. INTERM .OR. LAST ) ) THEN
         INFO = -2
      ELSE IF( .NOT. ONEBCH ) THEN
         IF( .NOT.( CONNEC .OR. LSAME( CONCT, 'N' ) ) )
     $      INFO = -3
      END IF
      IF( INFO.EQ.0 ) THEN
         IF( NOBR.LE.0 ) THEN
            INFO = -4
         ELSE IF( M.LT.0 ) THEN
            INFO = -5
         ELSE IF( L.LE.0 ) THEN
            INFO = -6
         ELSE IF( NSMP.LT.NOBR2 .OR.
     $            ( LAST .AND. NSMPSM.LT.NR+NOBR21 ) ) THEN
            INFO = -7
         ELSE IF( LDU.LT.1 .OR. ( M.GT.0 .AND. LDU.LT.NSMP ) ) THEN
            INFO = -9
         ELSE IF( LDY.LT.NSMP ) THEN
            INFO = -11
         ELSE IF( LDR.LT.NR ) THEN
            INFO = -13
         ELSE
C
C           Compute workspace.
C           NRG is the number of positive (or negative) generators.
C
            NRG = M + L + 1
            IF ( .NOT.ONEBCH .AND. CONNEC ) THEN
               MINWRK = NR*( NRG + 2 )
            ELSE IF ( FIRST .OR. INTERM ) THEN
               MINWRK = NR*NRG
            ELSE
               MINWRK = 2*NR*NRG + NR
            END IF
            MAXWRK = MAX( MINWRK, MAXWRK )
C
            IF( LDWORK.LT.MINWRK ) 
     $         INFO = -16
         END IF
      END IF       
C
C     Return if there are illegal arguments.
C
      IF( INFO.NE.0 ) THEN
         NSMPSM = 0
         IF ( INFO.EQ.-16 ) 
     $      DWORK( 1 ) = MINWRK
         CALL XERBLA( 'IB01MY', -INFO )
         RETURN
      END IF
C
C     Compute the  R  factor from a fast QR factorization of the
C     matrix  H,  a concatenation of two block Hankel matrices.
C     Specifically, a displacement rank technique is applied to
C     the block Toeplitz matrix,  G = (P*H)'*(P*H),  where  P  is a 
C     2-by-2 block diagonal matrix, having as diagonal blocks identity
C     matrices with columns taken in the reverse order.
C     The technique builds and processes the generators of  G.  The 
C     matrices  G  and  G1 = H'*H  have the same  R  factor.
C     
C     Set the parameters for constructing the correlations of the 
C     current block. 
C     NSM is the number of processed samples in U and Y, t - 2s.
C     IPG and ING are pointers to the "positive" and "negative" 
C     generators, stored row-wise in the workspace. All "positive" 
C     generators are stored before any "negative" generators.
C     If BATCH <> 'O' and CONCT = 'C', the "connection" elements of
C     two successive batches are stored in the same workspace as the
C     "negative" generators (which will be computed later on).
C     IPY is a pointer to the Y part of the "positive" generators. 
C     LDRWRK is used as a leading dimension for the workspace part used
C     to store the "connection" elements. 
C     
      NS   = NSMP - NOBR21
      NSM  = NS - 1
      MNRG = M*NRG
      LNRG = L*NRG
C
      LDRWRK = 2*NOBR2
      IF( FIRST ) THEN
         UPD = ZERO
      ELSE
         UPD = ONE
      END IF
      DUM(1) = ZERO
C     
      IPG   = 1
      IPY   = IPG + M
      ING   = IPG + NRG*NR
      ICONN = ING
C
      IF( .NOT.FIRST .AND. CONNEC ) THEN
C     
C        Restore the saved (M+L)*2*NOBR "connection" elements of
C        U  and  Y  into their appropriate position in sequential
C        processing. The process is performed column-wise, in
C        reverse order, first for  Y  and then for  U.
C        ICONN is a pointer to the first saved "connection" element. 
C        Workspace: need   (M+L)*2*NOBR*(M+L+3).
C     
         IREV = ICONN +   NR
         ICOL = ICONN + 2*NR
C     
         DO 10 I = 1, M + L
            IREV = IREV - NOBR2
            ICOL = ICOL - LDRWRK
            CALL DCOPY( NOBR2, DWORK(IREV), -1, DWORK(ICOL), -1 )
   10    CONTINUE
C     
         IF ( M.GT.0 ) 
     $      CALL DLACPY( 'Full', NOBR2, M, U, LDU, DWORK(ICONN+NOBR2),
     $                   LDRWRK )
         CALL DLACPY( 'Full', NOBR2, L, Y, LDY, 
     $                DWORK(ICONN+LDRWRK*M+NOBR2), LDRWRK )
      END IF
C     
      IF ( M.GT.0 ) THEN
C     
C        Let  Guu(i,j) = Guu0(i,j) + u_i*u_j' + u_(i+1)*u_(j+1)' +
C                              ... + u_(i+NSM-1)*u_(j+NSM-1)',
C        where  u_i'  is the i-th row of  U,  j = 1 : 2s,  i = 1 : j,  
C        NSM = NSMP - 2s,  and  Guu0(i,j)  is a zero matrix for 
C        BATCH = 'O' or 'F', and it is the matrix Guu(i,j) computed
C        till the current block for BATCH = 'I' or 'L'. The matrix
C        Guu(i,j)  is  m-by-m,  and  Guu(j,j)  is symmetric. The 
C        submatrices of the first block-row, Guu(1,j), are needed only.
C     
C        Compute/update  Guu(1,1). 
C     
         IF( .NOT.FIRST .AND. CONNEC )
     $      CALL DSYRK( 'Upper', 'Transpose', M, NOBR2, ONE,
     $                  DWORK(ICONN), LDRWRK, UPD, DWORK(IPG), NRG )
         CALL DSYRK( 'Upper', 'Transpose', M, NSM, ONE, U, LDU, UPD,
     $               DWORK(IPG), NRG )
         CALL MA02ED( 'Upper', M, DWORK(IPG), NRG )
C     
         JD = 1
C     
         IF( FIRST .OR. .NOT.CONNEC ) THEN
C     
            DO 20 J = 2, NOBR2
               JD = JD + M
C     
C              Compute/update  Guu(1,j). 
C     
               CALL DGEMM( 'Transpose', 'NoTranspose', M, M, NSM, ONE,
     $                     U, LDU, U(J,1), LDU, UPD,
     $                     DWORK(IPG+(JD-1)*NRG), NRG )
   20       CONTINUE
C     
         ELSE
C     
            DO 30 J = 2, NOBR2
               JD = JD + M
C     
C              Compute/update  Guu(1,j)  for sequential processing
C              with connected blocks. 
C     
               CALL DGEMM( 'Transpose', 'NoTranspose', M, M, NOBR2,
     $                     ONE, DWORK(ICONN), LDRWRK, DWORK(ICONN+J-1),
     $                     LDRWRK, UPD, DWORK(IPG+(JD-1)*NRG), NRG )
               CALL DGEMM( 'Transpose', 'NoTranspose', M, M, NSM, ONE,
     $                     U, LDU, U(J,1), LDU, ONE,
     $                     DWORK(IPG+(JD-1)*NRG), NRG )
   30       CONTINUE
C     
         END IF
C     
C        Let  Guy(i,j) = Guy0(i,j) + u_i*y_j' + u_(i+1)*y_(j+1)' +
C                              ... + u_(i+NSM-1)*y_(j+NSM-1)',
C        where  u_i'  is the i-th row of  U,  y_j'  is the j-th row 
C        of  Y,  j = 1 : 2s,  i = 1 : 2s,  NSM = NSMP - 2s,  and
C        Guy0(i,j)  is a zero matrix for  BATCH = 'O' or 'F', and it
C        is the matrix Guy(i,j) computed till the current block for 
C        BATCH = 'I' or 'L'.  Guy(i,j) is m-by-L. The submatrices 
C        of the first block-row, Guy(1,j), as well as the transposes 
C        of the submatrices of the first block-column, i.e., Gyu(1,j),
C        are needed only.
C     
         JD = MMNOBR + 1
C
         IF( FIRST .OR. .NOT.CONNEC ) THEN
C     
            DO 40 J = 1, NOBR2
C     
C              Compute/update  Guy(1,j). 
C     
               CALL DGEMM( 'Transpose', 'NoTranspose', M, L, NSM, ONE,
     $                     U, LDU, Y(J,1), LDY, UPD, 
     $                     DWORK(IPG+(JD-1)*NRG), NRG )
               JD = JD + L
   40       CONTINUE
C     
         ELSE
C     
            DO 50 J = 1, NOBR2
C     
C              Compute/update  Guy(1,j)  for sequential processing
C              with connected blocks. 
C     
               CALL DGEMM( 'Transpose', 'NoTranspose', M, L, NOBR2,
     $                     ONE, DWORK(ICONN), LDRWRK, 
     $                     DWORK(ICONN+LDRWRK*M+J-1), LDRWRK, UPD, 
     $                     DWORK(IPG+(JD-1)*NRG), NRG )
               CALL DGEMM( 'Transpose', 'NoTranspose', M, L, NSM, ONE,
     $                     U, LDU, Y(J,1), LDY, ONE, 
     $                     DWORK(IPG+(JD-1)*NRG), NRG )
               JD = JD + L
   50       CONTINUE
C     
         END IF
C        
C        Now, the first M "positive" generators have been built. 
C        Transpose  Guy(1,1)  in the first block of the  Y  part of the
C        "positive" generators.
C
         DO 60 J = 1, L
            CALL DCOPY( M, DWORK(IPG+(MMNOBR+J-1)*NRG), 1, 
     $                  DWORK(IPY+J-1), NRG )
   60    CONTINUE
C
         JD = 1
C     
         IF( FIRST .OR. .NOT.CONNEC ) THEN
C     
            DO 70 J = 2, NOBR2
               JD = JD + M
C     
C              Compute/update  Gyu(1,j). 
C     
               CALL DGEMM( 'Transpose', 'NoTranspose', L, M, NSM, ONE,
     $                     Y, LDY, U(J,1), LDU, UPD, 
     $                     DWORK(IPY+(JD-1)*NRG), NRG )
   70       CONTINUE
C     
         ELSE
C     
            DO 80 J = 2, NOBR2
               JD = JD + M
C     
C              Compute/update  Gyu(1,j)  for sequential processing
C              with connected blocks. 
C     
               CALL DGEMM( 'Transpose', 'NoTranspose', L, M, NOBR2,
     $                     ONE, DWORK(ICONN+LDRWRK*M), LDRWRK, 
     $                     DWORK(ICONN+J-1), LDRWRK, UPD, 
     $                     DWORK(IPY+(JD-1)*NRG), NRG )
               CALL DGEMM( 'Transpose', 'NoTranspose', L, M, NSM, ONE,
     $                     Y, LDY, U(J,1), LDU, ONE, 
     $                     DWORK(IPY+(JD-1)*NRG), NRG )
   80       CONTINUE
C     
         END IF
C     
      END IF
C     
C     Let  Gyy(i,j) = Gyy0(i,j) + y_i*y_i' + y_(i+1)*y_(i+1)' + ... +
C                                 y_(i+NSM-1)*y_(i+NSM-1)',
C     where  y_i'  is the i-th row of  Y,  j = 1 : 2s,  i = 1 : j,  
C     NSM = NSMP - 2s,  and  Gyy0(i,j)  is a zero matrix for 
C     BATCH = 'O' or 'F', and it is the matrix Gyy(i,j) computed till
C     the current block for BATCH = 'I' or 'L'.  Gyy(i,j) is L-by-L, 
C     and  Gyy(j,j)  is symmetric. The submatrices of the first
C     block-row, Gyy(1,j), are needed only. 
C     
      JD = MMNOBR + 1
C
C     Compute/update  Gyy(1,1). 
C     
      IF( .NOT.FIRST .AND. CONNEC )
     $   CALL DSYRK( 'Upper', 'Transpose', L, NOBR2, ONE,
     $               DWORK(ICONN+LDRWRK*M), LDRWRK, UPD, 
     $               DWORK(IPY+MMNOBR*NRG), NRG )
      CALL DSYRK( 'Upper', 'Transpose', L, NSM, ONE, Y, LDY, UPD,
     $            DWORK(IPY+MMNOBR*NRG), NRG )
      CALL MA02ED( 'Upper', L, DWORK(IPY+MMNOBR*NRG), NRG )
C     
      IF( FIRST .OR. .NOT.CONNEC ) THEN
C     
         DO 90 J = 2, NOBR2
            JD = JD + L
C     
C           Compute/update  Gyy(1,j). 
C     
            CALL DGEMM( 'Transpose', 'NoTranspose', L, L, NSM, ONE, Y, 
     $                  LDY, Y(J,1), LDY, UPD, DWORK(IPY+(JD-1)*NRG), 
     $                  NRG )
   90    CONTINUE
C     
      ELSE
C     
         DO 100 J = 2, NOBR2
            JD = JD + L
C     
C           Compute/update  Gyy(1,j)  for sequential processing with
C           connected blocks. 
C     
            CALL DGEMM( 'Transpose', 'NoTranspose', L, L, NOBR2, ONE,
     $                  DWORK(ICONN+LDRWRK*M), LDRWRK, 
     $                  DWORK(ICONN+LDRWRK*M+J-1), LDRWRK, UPD,
     $                  DWORK(IPY+(JD-1)*NRG), NRG )
            CALL DGEMM( 'Transpose', 'NoTranspose', L, L, NSM, ONE, Y,
     $                  LDY, Y(J,1), LDY, ONE, DWORK(IPY+(JD-1)*NRG), 
     $                  NRG )
  100    CONTINUE
C     
      END IF
C     
      IF ( .NOT.LAST ) THEN
         IF ( FIRST ) THEN
C     
C           For sequential processing, save the first 2*NOBR-1 rows of
C           the first block of  U  and  Y  in the appropriate
C           (M+L)*(2*NOBR-1) locations of  DWORK  starting at (1+M)*NRG.
C           These will be used to construct the last negative generator.
C
            JD = NRG
            IF ( M.GT.0 ) THEN
               CALL DCOPY( M, DUM, 0, DWORK(JD), NRG )
C
               DO 110 J = 1, NOBR21
                  JD = JD + MNRG
                  CALL DCOPY( M, U(J,1), LDU, DWORK(JD), NRG )
  110          CONTINUE
C
               JD = JD + MNRG
            END IF
            CALL DCOPY( L, DUM, 0, DWORK(JD), NRG )
C
            DO 120 J = 1, NOBR21
               JD = JD + LNRG
               CALL DCOPY( L, Y(J,1), LDY, DWORK(JD), NRG )
  120       CONTINUE
C      
         END IF
C
         IF ( CONNEC ) THEN
C     
C           For sequential processing with connected data blocks, 
C           save the remaining ("connection") elements of  U  and  Y
C           in (M+L)*2*NOBR locations of  DWORK  starting at ICONN.
C     
            IF ( M.GT.0 )
     $         CALL DLACPY( 'Full', NOBR2, M, U(NS,1), LDU,
     $                      DWORK(ICONN), NOBR2 )
            CALL DLACPY( 'Full', NOBR2, L, Y(NS,1), LDY,
     $                   DWORK(ICONN+MMNOBR), NOBR2 )
         END IF
C     
C        Return to get new data.
C     
         ICYCLE = ICYCLE + 1
         IF ( ICYCLE.GT.MAXCYC )
     $      IWARN = 1
         RETURN
      END IF
C     
      IF ( LAST ) THEN
C
C        Try to compute the R factor. 
C 
C        Scale the first M+L positive generators and set the first 
C        M+L negative generators.
C        Workspace: need   (M+L)*4*NOBR*(M+L+1)+M+L.
C
         JWORK = NRG*2*NR + 1
         CALL DCOPY( M, DWORK(IPG), NRG+1, DWORK(JWORK), 1 )
         CALL DCOPY( L, DWORK(IPY+MMNOBR*NRG), NRG+1, DWORK(JWORK+M), 
     $               1 )
C
         DO 130 I = 1, M + L
            IWORK(I) = IDAMAX( M+L, DWORK(JWORK), 1 )
            DWORK(JWORK+IWORK(I)-1) = ZERO
  130    CONTINUE
C
         DO 150 I = 1, M + L
            IMAX = IWORK(I)
            IF ( IMAX.LE.M ) THEN
               ICOL = IMAX
            ELSE
               ICOL = MMNOBR - M + IMAX
            END IF
            BETA = SQRT( ABS( DWORK(IPG+IMAX-1+(ICOL-1)*NRG) ) )
            IF ( BETA.EQ.ZERO ) THEN
C
C              Error exit.
C 
               INFO = 1
               RETURN
            END IF  
            CALL DSCAL( NR, ONE / BETA, DWORK(IPG+IMAX-1), NRG )
            CALL DCOPY( NR, DWORK(IPG+IMAX-1), NRG, DWORK(ING+IMAX-1), 
     $                  NRG )
            DWORK(IPG+IMAX-1+(ICOL-1)*NRG) = BETA
            DWORK(ING+IMAX-1+(ICOL-1)*NRG) = ZERO
C
            DO 140 J = I + 1, M + L
               DWORK(IPG+IWORK(J)-1+(ICOL-1)*NRG) = ZERO
  140       CONTINUE
C 
  150    CONTINUE
C 
C        Compute the last two generators.
C
         IF ( .NOT.FIRST ) THEN
C 
C           For sequential processing, move the stored last negative 
C           generator.
C
            CALL DCOPY( NR, DWORK(NRG), NRG, DWORK(ING+NRG-1), NRG )
         END IF
C 
         JD = NRG
         IF ( M.GT.0 ) THEN
C
            DO 160 J = NS, NSMP
               CALL DCOPY( M, U(J,1), LDU, DWORK(JD), NRG )
               JD = JD + MNRG
  160       CONTINUE
C
         END IF
C
         DO 170 J = NS, NSMP
            CALL DCOPY( L, Y(J,1), LDY, DWORK(JD), NRG )
            JD = JD + LNRG
  170    CONTINUE
C
         IF ( FIRST ) THEN
            IF ( M.GT.0 ) THEN
               CALL DCOPY( M, DUM, 0, DWORK(JD), NRG )
C
               DO 180 J = 1, NOBR21
                  JD = JD + MNRG
                  CALL DCOPY( M, U(J,1), LDU, DWORK(JD), NRG )
  180          CONTINUE
C
               JD = JD + MNRG
            END IF
            CALL DCOPY( L, DUM, 0, DWORK(JD), NRG )
C
            DO 190 J = 1, NOBR21
               JD = JD + LNRG
               CALL DCOPY( L, Y(J,1), LDY, DWORK(JD), NRG )
  190       CONTINUE
C     
         END IF
C 
         ITAU = JWORK
         IPGC = IPG + MMNOBR*NRG
C
         IF ( M.GT.0 ) THEN
C
C           Process the input part of the generators.     
C 
            JWORK = ITAU + M
C
C           Reduce the first M columns of the matrix G1 of positive 
C           generators to an upper triangular form.
C           Workspace: need   (M+L)*4*NOBR*(M+L+1)+2*M;
C                   prefer (M+L)*4*NOBR*(M+L+1)+M+M*NB.
C
            INGC = ING
            CALL DGEQRF( NRG, M, DWORK(IPG), NRG, DWORK(ITAU),
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
C           Workspace: need   (M+L)*4*NOBR*(M+L+1)+(M+L)*2*NOBR;
C                      prefer (M+L)*4*NOBR*(M+L+1)+M+
C                                                 ((M+L)*2*NOBR-M)*NB.
C
            CALL DORMQR( 'Left', 'Transpose', NRG, NR-M, M, DWORK(IPG),
     $                   NRG, DWORK(ITAU), DWORK(IPG+MNRG), NRG,
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
C           Annihilate, column by column, the first M columns of the 
C           matrix G2 of negative generators, using Householder 
C           transformations and modified hyperbolic plane rotations.
C           In the DLARF calls, ITAU is a pointer to the workspace 
C           array.
C
            DO 210 J = 1, M
               CALL DLARFG( NRG, DWORK(INGC), DWORK(INGC+1), 1, TAU )
               BETA = DWORK(INGC)
               DWORK(INGC) = ONE
               INGP = INGC + NRG
               CALL DLARF( 'Left', NRG, NR-J, DWORK(INGC), 1, TAU,
     $                     DWORK(INGP), NRG, DWORK(ITAU) )
               DWORK(INGC) = BETA
C
C              Compute the coefficients of the modified hyperbolic
C              rotation.
C 
               CALL MA02FD( DWORK(IPG+(J-1)*(NRG+1)), DWORK(INGC), CS, 
     $                      SN, IERR )
               IF( IERR.NE.0 ) THEN
C      
C                 Error return: the matrix H'*H is not (numerically) 
C                 positive definite.     
C    
                  INFO = 1
                  RETURN
               END IF
C
               DO 200 I = J*NRG, ( NR - 1 )*NRG, NRG
                  DWORK(IPG+J-1+I) =     ( DWORK(IPG+J-1+I) - 
     $                                SN * DWORK(ING+I) ) / CS
                  DWORK(ING+I)     = -SN * DWORK(IPG+J-1+I) +
     $                                CS * DWORK(ING+I)
  200          CONTINUE
C
               INGC = INGP
  210       CONTINUE
C
C           Save one block row of R, and shift the generators for the
C           calculation of the following row.     
C 
            CALL DLACPY( 'Upper', M, NR, DWORK(IPG), NRG, R, LDR )
C
            DO 220 I = ( MMNOBR - M )*NRG, MNRG, -MNRG
               CALL DLACPY( 'Full', M, M, DWORK(IPG+I-MNRG), NRG, 
     $                      DWORK(IPG+I), NRG )
  220       CONTINUE
C
            DO 230 I = ( NR - L )*NRG, ( MMNOBR + L )*NRG, -LNRG
               CALL DLACPY( 'Full', M, L, DWORK(IPG+I-LNRG), NRG,
     $                      DWORK(IPG+I), NRG )
  230       CONTINUE
C
            CALL DLASET( 'Full', M, L, ZERO, ZERO, DWORK(IPGC), NRG )
C
C           Update the input part of generators using Schur algorithm.
C           Workspace: need   (M+L)*4*NOBR*(M+L+1)+2*NOBR*(M+L)-M.
C     
            JDS  = MNRG
            ICOL = M
C
            DO 280 K = 2, NOBR2
               CALL MB04OD( 'Full', M, NR-ICOL-M, L+1, DWORK(IPG+JDS),
     $                      NRG, DWORK(IPY+JDS), NRG, 
     $                      DWORK(IPG+JDS+MNRG), NRG, 
     $                      DWORK(IPY+JDS+MNRG), NRG, DWORK(ITAU),
     $                      DWORK(JWORK) )
C
               DO 250 J = 1, M
                  ICJ = ICOL + J
                  CALL DLARFG( NRG, DWORK(INGC), DWORK(INGC+1), 1, TAU )
                  BETA = DWORK(INGC)
                  DWORK(INGC) = ONE
                  INGP = INGC + NRG
                  CALL DLARF( 'Left', NRG, NR-ICJ, DWORK(INGC), 1, TAU,
     $                        DWORK(INGP), NRG, DWORK(ITAU) )
                  DWORK(INGC) = BETA
C
C                 Compute the coefficients of the modified hyperbolic
C                 rotation.
C 
                  CALL MA02FD( DWORK(IPG+J-1+(ICJ-1)*NRG), DWORK(INGC), 
     $                         CS, SN, IERR )
                  IF( IERR.NE.0 ) THEN
C      
C                    Error return: the matrix H'*H is not (numerically) 
C                    positive definite.     
C 
                     INFO = 1
                     RETURN
                  END IF
C
                  DO 240 I = ICJ*NRG, ( NR - 1 )*NRG, NRG
                     DWORK(IPG+J-1+I) =     ( DWORK(IPG+J-1+I) - 
     $                                   SN * DWORK(ING+I) ) / CS
                     DWORK(ING+I)     = -SN * DWORK(IPG+J-1+I) +
     $                                   CS * DWORK(ING+I)
  240             CONTINUE
C
                  INGC = INGP
  250          CONTINUE
C
C              Save one block row of R, and shift the generators for the
C              calculation of the following row.     
C 
               CALL DLACPY( 'Upper', M, NR-ICOL, DWORK(IPG+JDS), NRG,
     $                      R(ICOL+1,ICOL+1), LDR )
               ICOL = ICOL + M
C
               DO 260 I = ( MMNOBR - M )*NRG, ICOL*NRG, -MNRG
                  CALL DLACPY( 'Full', M, M, DWORK(IPG+I-MNRG), NRG, 
     $                         DWORK(IPG+I), NRG )
  260          CONTINUE
C
               DO 270 I = ( NR - L )*NRG, ( MMNOBR + L )*NRG, -LNRG
                  CALL DLACPY( 'Full', M, L, DWORK(IPG+I-LNRG), NRG,
     $                         DWORK(IPG+I), NRG )
  270          CONTINUE
C
               CALL DLASET( 'Full', M, L, ZERO, ZERO, DWORK(IPGC), NRG )
               JDS = JDS + MNRG
  280       CONTINUE
C
         END IF
C     
C        Process the output part of the generators.     
C
         JWORK = ITAU + L
C
C        Reduce the first L columns of the submatrix
C        G1(1:M+L+1,2*M*NOBR+1:2*(M+L)*NOBR) to upper triangular form.
C        Workspace: need   (M+L)*4*NOBR*(M+L+1)+2*L;
C                   prefer (M+L)*4*NOBR*(M+L+1)+L+L*NB.
C
         INGC = ING + MMNOBR*NRG
         CALL DGEQRF( NRG, L, DWORK(IPGC), NRG, DWORK(ITAU),
     $                DWORK(JWORK), LDWORK-JWORK+1, IERR )
         MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
C        Workspace: need   (M+L)*4*NOBR*(M+L+1)+L*2*NOBR;
C                   prefer (M+L)*4*NOBR*(M+L+1)+L+(L*2*NOBR-L)*NB.
C
         CALL DORMQR( 'Left', 'Transpose', NRG, LLNOBR-L, L, 
     $                DWORK(IPGC), NRG, DWORK(ITAU), DWORK(IPGC+LNRG), 
     $                NRG, DWORK(JWORK), LDWORK-JWORK+1, IERR )
         MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
C
C        Annihilate, column by column, the first L columns of the 
C        output part of the matrix G2 of negative generators, using 
C        Householder transformations and modified hyperbolic rotations.
C
         DO 300 J = 1, L
            CALL DLARFG( NRG, DWORK(INGC), DWORK(INGC+1), 1, TAU )
            BETA = DWORK(INGC)
            DWORK(INGC) = ONE
            INGP = INGC + NRG
            CALL DLARF( 'Left', NRG, LLNOBR-J, DWORK(INGC), 1, TAU,
     $                  DWORK(INGP), NRG, DWORK(ITAU) )
            DWORK(INGC) = BETA
C
C           Compute the coefficients of the modified hyperbolic
C           rotation.
C 
            CALL MA02FD( DWORK(IPGC+(J-1)*(NRG+1)), DWORK(INGC), CS, SN,
     $                   IERR ) 
            IF( IERR.NE.0 ) THEN
C      
C              Error return: the matrix H'*H is not (numerically) 
C              positive definite.     
C 
               INFO = 1
               RETURN
            END IF
C
            DO 290 I = ( J + MMNOBR )*NRG, ( NR - 1 )*NRG, NRG
               DWORK(IPG+J-1+I) =     ( DWORK(IPG+J-1+I) - 
     $                             SN * DWORK(ING+I) ) / CS
               DWORK(ING+I)     = -SN * DWORK(IPG+J-1+I) +
     $                             CS * DWORK(ING+I)
  290       CONTINUE
C
            INGC = INGP
  300    CONTINUE
C
C        Save one block row of R, and shift the generators for the
C        calculation of the following row.     
C 
         CALL DLACPY( 'Upper', L, LLNOBR, DWORK(IPGC), NRG,
     $                 R(MMNOBR+1,MMNOBR+1), LDR )
C
         DO 310 I = ( NR - L )*NRG, ( MMNOBR + L )*NRG, -LNRG
            CALL DLACPY( 'Full', L, L, DWORK(IPG+I-LNRG), NRG,
     $                   DWORK(IPG+I), NRG )
  310    CONTINUE
C
C        Update the output part of generators using the Schur algorithm.
C        Workspace: need   (M+L)*4*NOBR*(M+L+1)+2*NOBR*L-L.
C     
         JDS  = LNRG
         ICOL = L
C
         DO 350 K = 2, NOBR2
            CALL MB04OD( 'Full', L, LLNOBR-ICOL-L, M+1, DWORK(IPGC+JDS),
     $                   NRG, DWORK(IPGC+L+JDS), NRG,
     $                   DWORK(IPGC+JDS+LNRG),   NRG, 
     $                   DWORK(IPGC+L+JDS+LNRG), NRG, DWORK(ITAU), 
     $                   DWORK(JWORK) )
C
            DO 330 J = 1, L
               ICJ = ICOL + J
               CALL DLARFG( NRG, DWORK(INGC), DWORK(INGC+1), 1, TAU )
               BETA = DWORK(INGC)
               DWORK(INGC) = ONE
               INGP = INGC + NRG
               CALL DLARF( 'Left', NRG, LLNOBR-ICJ, DWORK(INGC), 1,
     $                     TAU, DWORK(INGP), NRG, DWORK(ITAU) )
               DWORK(INGC) = BETA
C
C              Compute the coefficients of the modified hyperbolic
C              rotation.
C 
               CALL MA02FD( DWORK(IPGC+J-1+(ICJ-1)*NRG), DWORK(INGC), 
     $                      CS, SN, IERR )
               IF( IERR.NE.0 ) THEN
C      
C                 Error return: the matrix H'*H is not (numerically) 
C                 positive definite.     
C 
                  INFO = 1
                  RETURN
               END IF
C
               DO 320 I = ( ICJ + MMNOBR )*NRG, ( NR - 1 )*NRG, NRG
                  DWORK(IPG+J-1+I) =     ( DWORK(IPG+J-1+I) - 
     $                                SN * DWORK(ING+I) ) / CS
                  DWORK(ING+I)     = -SN * DWORK(IPG+J-1+I) +
     $                                CS * DWORK(ING+I)
  320          CONTINUE
C
               INGC = INGP
  330       CONTINUE
C
C           Save one block row of R, and shift the generators for the
C           calculation of the following row.     
C 
            CALL DLACPY( 'Upper', L, LLNOBR-ICOL, DWORK(IPGC+JDS), NRG,
     $                    R(MMNOBR+ICOL+1,MMNOBR+ICOL+1), LDR )
C
            DO 340 I = ( NR - L )*NRG, ( MMNOBR + ICOL )*NRG, -LNRG
               CALL DLACPY( 'Full', L, L, DWORK(IPG+I-LNRG), NRG,
     $                      DWORK(IPG+I), NRG )
  340       CONTINUE
C
            ICOL = ICOL + L
            JDS  = JDS + LNRG
  350    CONTINUE
C
         IF ( MOESP .AND. M.GT.0 ) THEN
C
C           For the MOESP algorithm, interchange the past and future 
C           input parts of the R factor, and compute the new R factor
C           using a specialized QR factorization.  A tailored fast
C           QR factorization for the MOESP algorithm could be slightly
C           more efficient.
C     
            DO 360 J = 1, MNOBR
               CALL DSWAP( J, R(1,J), 1, R(1,MNOBR+J), 1 )
               CALL DCOPY( MNOBR, R(J+1,MNOBR+J), 1, R(J+1,J), 1 )
               CALL DCOPY( MMNOBR-J, DUM, 0, R(J+1,MNOBR+J), 1 )
  360       CONTINUE
C
C           Triangularize the first two block columns (using structure),
C           and apply the transformation to the corresponding part of
C           the remaining block columns.
C           Workspace: need 2*(M+L)*NOBR.
C
            ITAU  = 1
            JWORK = ITAU + MMNOBR
            CALL MB04ID( MMNOBR, MMNOBR, MNOBR-1, LLNOBR, R, LDR,
     $                   R(1,MMNOBR+1), LDR, DWORK(ITAU),
     $                   DWORK(JWORK), LDWORK-JWORK+1, IERR )
            MAXWRK = MAX( MAXWRK, INT( DWORK(JWORK) ) + JWORK - 1 )
         END IF
      END IF
C
      NSMPSM = 0
      ICYCLE = 1
C
C     Return optimal workspace in  DWORK(1).
C
      DWORK( 1 ) = MAXWRK
      MAXWRK = 1
      RETURN
C
C *** Last line of IB01MY ***
      END
