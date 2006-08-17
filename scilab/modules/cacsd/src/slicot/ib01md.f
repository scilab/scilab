      SUBROUTINE IB01MD( METH, ALG, BATCH, CONCT, NOBR, M, L, NSMP, U,
     $                   LDU, Y, LDY, R, LDR, IWORK, DWORK, LDWORK,
     $                   IWARN, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To construct an upper triangular factor  R  of the concatenated
C     block Hankel matrices using input-output data.  The input-output
C     data can, optionally, be processed sequentially.
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
C     ALG     CHARACTER*1
C             Specifies the algorithm for computing the triangular
C             factor R, as follows:
C             = 'C':  Cholesky algorithm applied to the correlation
C                     matrix of the input-output data;
C             = 'F':  Fast QR algorithm;
C             = 'Q':  QR algorithm applied to the concatenated block
C                     Hankel matrices.
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
C             (In the MOESP theory,  NOBR  should be larger than  n,
C             the estimated dimension of state vector.)
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
C     R       (output or input/output) DOUBLE PRECISION array, dimension
C             ( LDR,2*(M+L)*NOBR )
C             On exit, if INFO = 0 and ALG = 'Q', or (ALG = 'C' or 'F',
C             and BATCH = 'L' or 'O'), the leading
C             2*(M+L)*NOBR-by-2*(M+L)*NOBR upper triangular part of
C             this array contains the (current) upper triangular factor
C             R from the QR factorization of the concatenated block
C             Hankel matrices. The diagonal elements of R are positive
C             when the Cholesky algorithm was successfully used.
C             On exit, if ALG = 'C' and BATCH = 'F' or 'I', the leading
C             2*(M+L)*NOBR-by-2*(M+L)*NOBR upper triangular part of this
C             array contains the current upper triangular part of the
C             correlation matrix in sequential data processing.
C             If ALG = 'F' and BATCH = 'F' or 'I', the array R is not
C             referenced.
C             On entry, if ALG = 'C', or ALG = 'Q', and BATCH = 'I' or 
C             'L', the leading  2*(M+L)*NOBR-by-2*(M+L)*NOBR  upper
C             triangular part of this array must contain the upper
C             triangular matrix R computed at the previous call of this
C             routine in sequential data processing. The array R need
C             not be set on entry if ALG = 'F' or if BATCH = 'F' or 'O'.
C
C     LDR     INTEGER
C             The leading dimension of the array  R.
C             LDR >= 2*(M+L)*NOBR.
C
C     Workspace
C
C     IWORK   INTEGER array, dimension (LIWORK)
C             LIWORK >= M+L, if ALG = 'F';
C             LIWORK >= 0,   if ALG = 'C' or 'Q'.
C
C     DWORK   DOUBLE PRECISION array, dimension (LDWORK)
C             On exit, if  INFO = 0,  DWORK(1)  returns the optimal
C             value of LDWORK.
C             On exit, if  INFO = -17,  DWORK(1)  returns the minimum
C             value of LDWORK.
C             Let
C             k = 0,               if CONCT = 'N' and ALG = 'C' or 'Q';
C             k = 2*NOBR-1,        if CONCT = 'C' and ALG = 'C' or 'Q';
C             k = 2*NOBR*(M+L+1),  if CONCT = 'N' and ALG = 'F';
C             k = 2*NOBR*(M+L+2),  if CONCT = 'C' and ALG = 'F'.
C             The first (M+L)*k elements of  DWORK  should be preserved
C             during successive calls of the routine with  BATCH = 'F'
C             or  'I',  till the final call with  BATCH = 'L'.
C
C     LDWORK  INTEGER
C             The length of the array DWORK.
C             LDWORK >= (4*NOBR-2)*(M+L), if ALG = 'C', BATCH <> 'O' and
C                                     CONCT = 'C';
C             LDWORK >= 1,            if ALG = 'C', BATCH = 'O' or
C                                     CONCT = 'N';
C             LDWORK >= (M+L)*2*NOBR*(M+L+3), if ALG = 'F',
C                                     BATCH <> 'O' and CONCT = 'C';
C             LDWORK >= (M+L)*2*NOBR*(M+L+1), if ALG = 'F',
C                                     BATCH = 'F', 'I' and CONCT = 'N';
C             LDWORK >= (M+L)*4*NOBR*(M+L+1)+(M+L)*2*NOBR, if ALG = 'F',
C                                     BATCH = 'L' and CONCT = 'N', or
C                                     BATCH = 'O';
C             LDWORK >= 4*(M+L)*NOBR, if ALG = 'Q', BATCH = 'F' or 'O',
C                                     and LDR >= NS = NSMP - 2*NOBR + 1;
C             LDWORK >= 6*(M+L)*NOBR, if ALG = 'Q', BATCH = 'F' or 'O',
C                                     and LDR < NS, or BATCH = 'I' or 
C                                     'L' and CONCT = 'N';
C             LDWORK >= 4*(NOBR+1)*(M+L)*NOBR, if ALG = 'Q', BATCH = 'I'
C                                     or 'L' and CONCT = 'C'.
C             The workspace used for ALG = 'Q' is 
C                       LDRWRK*2*(M+L)*NOBR + 4*(M+L)*NOBR, 
C             where LDRWRK = LDWORK/(2*(M+L)*NOBR) - 2; recommended 
C             value LDRWRK = NS, assuming a large enough cache size.
C             For good performance,  LDWORK  should be larger.
C
C     Warning Indicator
C
C     IWARN   INTEGER
C             = 0:  no warning;
C             = 1:  the number of 100 cycles in sequential data
C                   processing has been exhausted without signaling
C                   that the last block of data was get; the cycle
C                   counter was reinitialized;
C             = 2:  a fast algorithm was requested (ALG = 'C' or 'F'),
C                   but it failed, and the QR algorithm was then used
C                   (non-sequential data processing).
C
C     Error Indicator
C
C     INFO    INTEGER
C             = 0:  successful exit;
C             < 0:  if INFO = -i, the i-th argument had an illegal
C                   value;
C             = 1:  a fast algorithm was requested (ALG = 'C', or 'F')
C                   in sequential data processing, but it failed. The
C                   routine can be repeatedly called again using the
C                   standard QR algorithm.
C
C     METHOD
C
C     1) For non-sequential data processing using QR algorithm, a
C     t x 2(m+l)s  matrix H is constructed, where  
C
C          H = [ Uf'         Up'      Y'      ],  for METH = 'M',
C                  s+1,2s,t    1,s,t   1,2s,t
C
C          H = [ U'       Y'      ],              for METH = 'N',
C                 1,2s,t   1,2s,t
C
C     and  Up     , Uf        , U      , and  Y        are block Hankel
C            1,s,t    s+1,2s,t   1,2s,t        1,2s,t
C     matrices defined in terms of the input and output data [3].
C     A QR factorization is used to compress the data.
C     The fast QR algorithm uses a QR factorization which exploits 
C     the block-Hankel structure. Actually, the Cholesky factor of H'*H
C     is computed.
C
C     2) For sequential data processing using QR algorithm, the QR
C     decomposition is done sequentially, by updating the upper
C     triangular factor  R.  This is also performed internally if the
C     workspace is not large enough to accommodate an entire batch.
C
C     3) For non-sequential or sequential data processing using
C     Cholesky algorithm, the correlation matrix of input-output data is
C     computed (sequentially, if requested), taking advantage of the
C     block Hankel structure [7].  Then, the Cholesky factor of the
C     correlation matrix is found, if possible.
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
C     [5] Peternell, K., Scherrer, W. and Deistler, M.
C         Statistical Analysis of Novel Subspace Identification Methods.
C         Signal Processing, 52, pp. 161-177, 1996.
C
C     [6] Sima, V.
C         Subspace-based Algorithms for Multivariable System 
C         Identification.
C         Studies in Informatics and Control, 5, pp. 335-344, 1996.
C
C     [7] Sima, V.
C         Cholesky or QR Factorization for Data Compression in 
C         Subspace-based Identification ?
C         Proceedings of the Second NICONET Workshop on ``Numerical 
C         Control Software: SLICOT, a Useful Tool in Industry'', 
C         December 3, 1999, INRIA Rocquencourt, France, pp. 75-80, 1999.
C
C     NUMERICAL ASPECTS
C
C     The implemented method is numerically stable (when QR algorithm is
C     used), reliable and efficient. The fast Cholesky or QR algorithms
C     are more efficient, but the accuracy could diminish by forming the
C     correlation matrix.
C                                        2 
C     The QR algorithm needs 0(t(2(m+l)s) ) floating point operations.
C                                           2              3
C     The Cholesky algorithm needs 0(2t(m+l) s)+0((2(m+l)s) ) floating 
C     point operations.
C                                          2           3 2 
C     The fast QR algorithm needs 0(2t(m+l) s)+0(4(m+l) s ) floating 
C     point operations.
C
C     FURTHER COMMENTS
C
C     For ALG = 'Q', BATCH = 'O' and LDR < NS, or BATCH <> 'O', the  
C     calculations could be rather inefficient if only minimal workspace
C     (see argument LDWORK) is provided. It is advisable to provide as
C     much workspace as possible. Almost optimal efficiency can be 
C     obtained for  LDWORK = (NS+2)*(2*(M+L)*NOBR),  assuming that the 
C     cache size is large enough to accommodate R, U, Y, and DWORK.
C
C     CONTRIBUTOR
C
C     V. Sima, Research Institute for Informatics, Bucharest, Aug. 1999.
C
C     REVISIONS
C
C     Feb. 2000, Aug. 2000.
C
C     KEYWORDS
C
C     Cholesky decomposition, Hankel matrix, identification methods, 
C     multivariable systems, QR decomposition.
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
      CHARACTER          ALG, BATCH, CONCT, METH
C     .. Array Arguments ..
      INTEGER            IWORK(*)
      DOUBLE PRECISION   DWORK(*), R(LDR, *), U(LDU, *), Y(LDY, *)
C     .. Local Scalars ..
      DOUBLE PRECISION   UPD, TEMP
      INTEGER            I, ICOL, ICYCLE, ID, IERR, II, INICYC, INIT,
     $                   INITI, INU, INY, IREV, ISHFT2, ISHFTU, ISHFTY,
     $                   ITAU, J, JD, JWORK, LDRWMX, LDRWRK, LLDRW,
     $                   LMNOBR, LNOBR, MAXWRK, MINWRK, MLDRW, MMNOBR,
     $                   MNOBR, NCYCLE, NICYCL, NOBR2, NOBR21, NOBRM1,
     $                   NR, NS, NSF, NSL, NSLAST, NSMPSM
      LOGICAL            CHALG, CONNEC, FIRST, FQRALG, INTERM, LAST,
     $                   LINR, MOESP, N4SID, ONEBCH, QRALG
C     .. Local Arrays ..
      DOUBLE PRECISION   DUM( 1 )
C     .. External Functions ..
      LOGICAL            LSAME 
      INTEGER            ILAENV
      EXTERNAL           ILAENV, LSAME
C     .. External Subroutines ..
      EXTERNAL           DAXPY, DCOPY, DGEMM, DGEQRF, DGER, DLACPY,
     $                   DLASET, DPOTRF, DSWAP, DSYRK, IB01MY, MB04OD, 
     $                   XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC          INT, MAX, MIN
C     .. Save Statement ..
C        ICYCLE  is used to count the cycles for  BATCH = 'I'. It is
C                reinitialized at each MAXCYC cycles.
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
      FQRALG = LSAME( ALG,   'F' )
      QRALG  = LSAME( ALG,   'Q' )
      CHALG  = LSAME( ALG,   'C' )
      ONEBCH = LSAME( BATCH, 'O' )
      FIRST  = LSAME( BATCH, 'F' ) .OR. ONEBCH
      INTERM = LSAME( BATCH, 'I' )
      LAST   = LSAME( BATCH, 'L' ) .OR. ONEBCH
      IF( .NOT.ONEBCH ) THEN
         CONNEC = LSAME( CONCT, 'C' )
      ELSE
         CONNEC = .FALSE.
      END IF
C
      MNOBR  = M*NOBR
      LNOBR  = L*NOBR
      LMNOBR = LNOBR + MNOBR
      MMNOBR = MNOBR + MNOBR
      NOBRM1 = NOBR - 1
      NOBR21 = NOBR + NOBRM1
      NOBR2  = NOBR21 + 1
      IWARN  = 0
      INFO   = 0
      IERR   = 0
      IF( FIRST ) THEN
         ICYCLE = 1
         MAXWRK = 1
         NSMPSM = 0
      END IF
      NSMPSM = NSMPSM + NSMP
      NR = LMNOBR + LMNOBR
C
C     Check the scalar input parameters.
C
      IF( .NOT.( MOESP .OR. N4SID ) ) THEN
         INFO = -1
      ELSE IF( .NOT.( FQRALG .OR. QRALG .OR. CHALG ) ) THEN
         INFO = -2
      ELSE IF( .NOT.( FIRST .OR. INTERM .OR. LAST ) ) THEN
         INFO = -3
      ELSE IF( .NOT. ONEBCH ) THEN
         IF( .NOT.( CONNEC .OR. LSAME( CONCT, 'N' ) ) )
     $      INFO = -4
      END IF
      IF( INFO.EQ.0 ) THEN
         IF( NOBR.LE.0 ) THEN
            INFO = -5
         ELSE IF( M.LT.0 ) THEN
            INFO = -6
         ELSE IF( L.LE.0 ) THEN
            INFO = -7
         ELSE IF( NSMP.LT.NOBR2 .OR.
     $          ( LAST .AND. NSMPSM.LT.NR+NOBR21 ) ) THEN
            INFO = -8
         ELSE IF( LDU.LT.1 .OR. ( M.GT.0 .AND. LDU.LT.NSMP ) ) THEN
            INFO = -10
         ELSE IF( LDY.LT.NSMP ) THEN
            INFO = -12
         ELSE IF( LDR.LT.NR ) THEN
            INFO = -14
         ELSE
C
C           Compute workspace.
C           (Note: Comments in the code beginning "Workspace:" describe
C           the minimal amount of workspace needed at that point in the
C           code, as well as the preferred amount for good performance.
C           NB refers to the optimal block size for the immediately
C           following subroutine, as returned by ILAENV.)
C
            NS = NSMP - NOBR21
            IF ( CHALG ) THEN
               IF ( .NOT.ONEBCH .AND. CONNEC ) THEN
                  MINWRK = 2*( NR - M - L )
               ELSE
                  MINWRK = 1
               END IF
            ELSE IF ( FQRALG ) THEN
               IF ( .NOT.ONEBCH .AND. CONNEC ) THEN
                  MINWRK = NR*( M + L + 3 )
               ELSE IF ( FIRST .OR. INTERM ) THEN
                  MINWRK = NR*( M + L + 1 )
               ELSE 
                  MINWRK = 2*NR*( M + L + 1 ) + NR
               END IF
            ELSE 
               MINWRK = 2*NR
               MAXWRK = NR + NR*ILAENV( 1, 'DGEQRF', ' ', NS, NR, -1,
     $                                  -1 )
               IF ( FIRST ) THEN
                  IF ( LDR.LT.NS ) THEN
                     MINWRK = MINWRK + NR
                     MAXWRK = NS*NR + MAXWRK
                  END IF
               ELSE
                  IF ( CONNEC ) THEN
                     MINWRK = MINWRK*( NOBR + 1 )
                  ELSE
                     MINWRK = MINWRK + NR
                  END IF
                  MAXWRK = NS*NR + MAXWRK
               END IF
            END IF
            MAXWRK = MAX( MINWRK, MAXWRK )
C
            IF( LDWORK.LT.MINWRK ) THEN
               INFO = -17
               DWORK( 1 ) = MINWRK
            END IF
         END IF
      END IF       
C
C     Return if there are illegal arguments.
C
      IF( INFO.NE.0 ) THEN
         CALL XERBLA( 'IB01MD', -INFO )
         RETURN
      END IF
C
      IF ( CHALG ) THEN
C
C        Compute the  R  factor from a Cholesky factorization of the
C        input-output data correlation matrix.
C
C        Set the parameters for constructing the correlations of the 
C        current block.
C
         LDRWRK = 2*NOBR2 - 2
         IF( FIRST ) THEN
            UPD = ZERO
         ELSE
            UPD = ONE
         END IF
C
         IF( .NOT.FIRST .AND. CONNEC ) THEN
C
C           Restore the saved (M+L)*(2*NOBR-1) "connection" elements of
C           U  and  Y  into their appropriate position in sequential
C           processing. The process is performed column-wise, in
C           reverse order, first for  Y  and then for  U.
C           Workspace: need   (4*NOBR-2)*(M+L).
C
            IREV =     NR - M - L   - NOBR21 + 1
            ICOL = 2*( NR - M - L ) - LDRWRK + 1
C
            DO 10 I = 1, M + L
               CALL DCOPY( NOBR21, DWORK(IREV), -1, DWORK(ICOL), -1 )
               IREV = IREV - NOBR21
               ICOL = ICOL - LDRWRK
   10       CONTINUE
C
            IF ( M.GT.0 ) 
     $         CALL DLACPY( 'Full', NOBR21, M, U, LDU, DWORK(NOBR2), 
     $                      LDRWRK )
            CALL DLACPY( 'Full', NOBR21, L, Y, LDY, 
     $                   DWORK(LDRWRK*M+NOBR2), LDRWRK )
         END IF
C
         IF ( M.GT.0 ) THEN
C
C           Let  Guu(i,j) = Guu0(i,j) + u_i*u_j' + u_(i+1)*u_(j+1)' +
C                                 ... + u_(i+NS-1)*u_(j+NS-1)',
C           where  u_i'  is the i-th row of  U,  j = 1 : 2s,  i = 1 : j,
C           NS = NSMP - 2s + 1,  and  Guu0(i,j)  is a zero matrix for 
C           BATCH = 'O' or 'F', and it is the matrix Guu(i,j) computed
C           till the current block for BATCH = 'I' or 'L'. The matrix
C           Guu(i,j)  is  m-by-m,  and  Guu(j,j)  is symmetric. The
C           upper triangle of the U-U correlations,  Guu,  is computed
C           (or updated) column-wise in the array  R,  that is, in the
C           order  Guu(1,1),  Guu(1,2),  Guu(2,2),  ...,  Guu(2s,2s). 
C           Only the submatrices of the first block-row are fully
C           computed (or updated). The remaining ones are determined
C           exploiting the block-Hankel structure, using the updating
C           formula
C
C           Guu(i+1,j+1) = Guu0(i+1,j+1) - Guu0(i,j) + Guu(i,j) + 
C                                 u_(i+NS)*u_(j+NS)' - u_i*u_j'.
C
            IF( .NOT.FIRST ) THEN
C
C              Subtract the contribution of the previous block of data
C              in sequential processing. The columns must be processed
C              in backward order.
C
               DO 20 I = NOBR21*M, 1, -1
                  CALL DAXPY( I, -ONE, R(1,I), 1, R(M+1,M+I), 1 )
   20          CONTINUE
C
            END IF
C
C           Compute/update  Guu(1,1). 
C
            IF( .NOT.FIRST .AND. CONNEC )
     $         CALL DSYRK( 'Upper', 'Transpose', M, NOBR21, ONE, DWORK,
     $                     LDRWRK, UPD, R, LDR )
            CALL DSYRK( 'Upper', 'Transpose', M, NS, ONE, U, LDU, UPD,
     $                  R, LDR )
C
            JD = 1
C
            IF( FIRST .OR. .NOT.CONNEC ) THEN
C
               DO 70 J = 2, NOBR2
                  JD = JD + M
                  ID = M  + 1
C
C                 Compute/update  Guu(1,j). 
C
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, M, NS, ONE,
     $                        U, LDU, U(J,1), LDU, UPD, R(1,JD), LDR )
C
C                 Compute/update  Guu(2:j,j), exploiting the
C                 block-Hankel structure. 
C
                  IF( FIRST ) THEN
C
                     DO 30 I = JD - M, JD - 1
                        CALL DCOPY( I, R(1,I), 1, R(M+1,M+I), 1 )
   30                CONTINUE
C
                  ELSE
C
                     DO 40 I = JD - M, JD - 1
                        CALL DAXPY( I, ONE, R(1,I), 1, R(M+1,M+I), 1 )
   40                CONTINUE
C
                  END IF
C
                  DO 50 I = 2, J - 1
                     CALL DGER( M, M, ONE, U(NS+I-1,1), LDU,
     $                          U(NS+J-1,1), LDU, R(ID,JD), LDR )
                     CALL DGER( M, M, -ONE, U(I-1,1), LDU, U(J-1,1),
     $                          LDU, R(ID,JD), LDR )
                     ID = ID + M
   50             CONTINUE
C
                  DO 60 I = 1, M
                     CALL DAXPY( I, U(NS+J-1,I), U(NS+J-1,1), LDU, 
     $                           R(JD,JD+I-1), 1 )
                     CALL DAXPY( I, -U(J-1,I), U(J-1,1), LDU,
     $                           R(JD,JD+I-1), 1 )
   60             CONTINUE
C
   70          CONTINUE
C
            ELSE
C
               DO 120 J = 2, NOBR2
                  JD = JD + M
                  ID = M  + 1
C
C                 Compute/update  Guu(1,j)  for sequential processing
C                 with connected blocks. 
C
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, M, NOBR21,
     $                        ONE, DWORK, LDRWRK, DWORK(J), LDRWRK, UPD,
     $                        R(1,JD), LDR )
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, M, NS, ONE,
     $                        U, LDU, U(J,1), LDU, ONE, R(1,JD), LDR )
C
C                 Compute/update  Guu(2:j,j)  for sequential processing
C                 with connected blocks, exploiting the block-Hankel
C                 structure. 
C
                  IF( FIRST ) THEN
C
                     DO 80 I = JD - M, JD - 1
                        CALL DCOPY( I, R(1,I), 1, R(M+1,M+I), 1 )
   80                CONTINUE
C
                  ELSE
C
                     DO 90 I = JD - M, JD - 1
                        CALL DAXPY( I, ONE, R(1,I), 1, R(M+1,M+I), 1 )
   90                CONTINUE
C
                  END IF
C
                  DO 100 I = 2, J - 1
                     CALL DGER( M, M, ONE, U(NS+I-1,1), LDU,
     $                          U(NS+J-1,1), LDU, R(ID,JD), LDR )
                     CALL DGER( M, M, -ONE, DWORK(I-1), LDRWRK,
     $                          DWORK(J-1), LDRWRK, R(ID,JD), LDR )
                     ID = ID + M
  100             CONTINUE
C
                  DO 110 I = 1, M
                     CALL DAXPY( I, U(NS+J-1,I), U(NS+J-1,1), LDU, 
     $                           R(JD,JD+I-1), 1 )
                     CALL DAXPY( I, -DWORK((I-1)*LDRWRK+J-1),
     $                           DWORK(J-1), LDRWRK, R(JD,JD+I-1), 1 )
  110             CONTINUE
C
  120          CONTINUE
C
            END IF
C
            IF ( LAST .AND. MOESP ) THEN
C
C              Interchange past and future parts for MOESP algorithm.
C              (Only the upper triangular parts are interchanged, and 
C              the (1,2) part is transposed in-situ.)
C
               TEMP = R(1,1)
               R(1,1) = R(MNOBR+1,MNOBR+1)
               R(MNOBR+1,MNOBR+1) = TEMP
C
               DO 130 J = 2, MNOBR
                  CALL DSWAP( J, R(1,J), 1, R(MNOBR+1,MNOBR+J), 1 )
                  CALL DSWAP( J-1, R(1,MNOBR+J), 1, R(J,MNOBR+1), LDR )
  130          CONTINUE
C
            END IF
C
C           Let  Guy(i,j) = Guy0(i,j) + u_i*y_j' + u_(i+1)*y_(j+1)' +
C                                 ... + u_(i+NS-1)*y_(j+NS-1)',
C           where  u_i'  is the i-th row of  U,  y_j'  is the j-th row 
C           of  Y,  j = 1 : 2s,  i = 1 : 2s,  NS = NSMP - 2s + 1,  and
C           Guy0(i,j)  is a zero matrix for  BATCH = 'O' or 'F', and it
C           is the matrix Guy(i,j) computed till the current block for 
C           BATCH = 'I' or 'L'.  Guy(i,j) is m-by-L. The U-Y
C           correlations,  Guy,  are computed (or updated) column-wise
C           in the array  R. Only the submatrices of the first block-
C           column and block-row are fully computed (or updated). The 
C           remaining ones are determined exploiting the block-Hankel
C           structure, using the updating formula
C
C           Guy(i+1,j+1) = Guy0(i+1,j+1) - Guy0(i,j) + Guy(i,j) + 
C                                 u_(i+NS)*y(j+NS)' - u_i*y_j'.
C
            II = MMNOBR - M
            IF( .NOT.FIRST ) THEN
C
C              Subtract the contribution of the previous block of data
C              in sequential processing. The columns must be processed
C              in backward order. 
C
               DO 140 I = NR - L, MMNOBR + 1, -1
                  CALL DAXPY( II, -ONE, R(1,I), 1, R(M+1,L+I), 1 )
  140          CONTINUE
C
            END IF
C
C           Compute/update the first block-column of  Guy,  Guy(i,1). 
C
            IF( FIRST .OR. .NOT.CONNEC ) THEN
C
               DO 150 I = 1, NOBR2
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, L, NS, ONE,
     $                        U(I,1), LDU, Y, LDY, UPD, 
     $                        R((I-1)*M+1,MMNOBR+1), LDR )
  150          CONTINUE
C
            ELSE
C
               DO 160 I = 1, NOBR2
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, L, NOBR21,
     $                        ONE, DWORK(I), LDRWRK, DWORK(LDRWRK*M+1), 
     $                        LDRWRK, UPD, R((I-1)*M+1,MMNOBR+1), LDR )
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, L, NS, ONE,
     $                        U(I,1), LDU, Y, LDY, ONE,
     $                        R((I-1)*M+1,MMNOBR+1), LDR )
  160          CONTINUE
C
            END IF
C
            JD = MMNOBR + 1
C
            IF( FIRST .OR. .NOT.CONNEC ) THEN
C
               DO 200 J = 2, NOBR2
                  JD = JD + L
                  ID = M  + 1
C
C                 Compute/update  Guy(1,j). 
C
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, L, NS, ONE,
     $                        U, LDU, Y(J,1), LDY, UPD, R(1,JD), LDR )
C
C                 Compute/update  Guy(2:2*s,j), exploiting the
C                 block-Hankel structure. 
C
                  IF( FIRST ) THEN
C
                     DO 170 I = JD - L, JD - 1
                        CALL DCOPY( II, R(1,I), 1, R(M+1,L+I), 1 )
  170                CONTINUE
C
                  ELSE
C
                     DO 180 I = JD - L, JD - 1
                        CALL DAXPY( II, ONE, R(1,I), 1, R(M+1,L+I), 1 )
  180                CONTINUE
C
                  END IF
C
                  DO 190 I = 2, NOBR2
                     CALL DGER( M, L, ONE, U(NS+I-1,1), LDU,
     $                          Y(NS+J-1,1), LDY, R(ID,JD), LDR )
                     CALL DGER( M, L, -ONE, U(I-1,1), LDU, Y(J-1,1),
     $                          LDY, R(ID,JD), LDR )
                     ID = ID + M
  190             CONTINUE
C
  200          CONTINUE
C
            ELSE
C
               DO 240 J = 2, NOBR2
                  JD = JD + L
                  ID = M  + 1
C
C                 Compute/update  Guy(1,j)  for sequential processing
C                 with connected blocks. 
C
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, L, NOBR21,
     $                        ONE, DWORK, LDRWRK, DWORK(LDRWRK*M+J), 
     $                        LDRWRK, UPD, R(1,JD), LDR )
                  CALL DGEMM( 'Transpose', 'NoTranspose', M, L, NS, ONE,
     $                        U, LDU, Y(J,1), LDY, ONE, R(1,JD), LDR )
C
C                 Compute/update  Guy(2:2*s,j)  for sequential
C                 processing with connected blocks, exploiting the 
C                 block-Hankel structure. 
C
                  IF( FIRST ) THEN
C
                     DO 210 I = JD - L, JD - 1
                        CALL DCOPY( II, R(1,I), 1, R(M+1,L+I), 1 )
  210                CONTINUE
C
                  ELSE
C
                     DO 220 I = JD - L, JD - 1
                        CALL DAXPY( II, ONE, R(1,I), 1, R(M+1,L+I), 1 )
  220                CONTINUE
C
                  END IF
C
                  DO 230 I = 2, NOBR2
                     CALL DGER( M, L, ONE, U(NS+I-1,1), LDU,
     $                          Y(NS+J-1,1), LDY, R(ID,JD), LDR )
                     CALL DGER( M, L, -ONE, DWORK(I-1), LDRWRK,
     $                          DWORK(LDRWRK*M+J-1), LDRWRK, R(ID,JD), 
     $                          LDR )
                     ID = ID + M
  230             CONTINUE
C
  240          CONTINUE
C
            END IF
C
            IF ( LAST .AND. MOESP ) THEN
C
C              Interchange past and future parts of U-Y correlations
C              for MOESP algorithm.
C
               DO 250 J = MMNOBR + 1, NR
                  CALL DSWAP( MNOBR, R(1,J), 1, R(MNOBR+1,J), 1 )
  250          CONTINUE
C
            END IF
         END IF
C
C        Let  Gyy(i,j) = Gyy0(i,j) + y_i*y_i' + y_(i+1)*y_(i+1)' + ... +
C                                    y_(i+NS-1)*y_(i+NS-1)',
C        where  y_i'  is the i-th row of  Y,  j = 1 : 2s,  i = 1 : j,  
C        NS = NSMP - 2s + 1,  and  Gyy0(i,j)  is a zero matrix for 
C        BATCH = 'O' or 'F', and it is the matrix Gyy(i,j) computed till
C        the current block for BATCH = 'I' or 'L'.  Gyy(i,j) is L-by-L, 
C        and  Gyy(j,j)  is symmetric. The upper triangle of the Y-Y
C        correlations,  Gyy,  is computed (or updated) column-wise in
C        the corresponding part of the array  R,  that is, in the order 
C        Gyy(1,1),  Gyy(1,2),  Gyy(2,2),  ...,  Gyy(2s,2s).  Only the
C        submatrices of the first block-row are fully computed (or
C        updated). The remaining ones are determined exploiting the
C        block-Hankel structure, using the updating formula
C
C        Gyy(i+1,j+1) = Gyy0(i+1,j+1) - Gyy0(i,j) + Gyy(i,j) + 
C                              y_(i+NS)*y_(j+NS)' - y_i*y_j'.
C
         JD = MMNOBR + 1
C
         IF( .NOT.FIRST ) THEN
C
C           Subtract the contribution of the previous block of data
C           in sequential processing. The columns must be processed in
C           backward order.  
C
            DO 260 I = NR - L, MMNOBR + 1, -1
               CALL DAXPY( I-MMNOBR, -ONE, R(JD,I), 1, R(JD+L,L+I), 1 )
  260       CONTINUE
C
         END IF
C
C        Compute/update  Gyy(1,1). 
C
         IF( .NOT.FIRST .AND. CONNEC )
     $      CALL DSYRK( 'Upper', 'Transpose', L, NOBR21, ONE,
     $                  DWORK(LDRWRK*M+1), LDRWRK, UPD, R(JD,JD), LDR ) 
         CALL DSYRK( 'Upper', 'Transpose', L, NS, ONE, Y, LDY, UPD,
     $               R(JD,JD), LDR )
C
         IF( FIRST .OR. .NOT.CONNEC ) THEN
C
            DO 310 J = 2, NOBR2
               JD = JD + L
               ID = MMNOBR + L + 1
C
C              Compute/update  Gyy(1,j). 
C
               CALL DGEMM( 'Transpose', 'NoTranspose', L, L, NS, ONE, Y,
     $                     LDY, Y(J,1), LDY, UPD, R(MMNOBR+1,JD), LDR )
C
C              Compute/update  Gyy(2:j,j), exploiting the block-Hankel
C              structure. 
C
               IF( FIRST ) THEN
C
                  DO 270 I = JD - L, JD - 1
                     CALL DCOPY( I-MMNOBR, R(MMNOBR+1,I), 1,
     $                           R(MMNOBR+L+1,L+I), 1 )
  270             CONTINUE
C
               ELSE
C
                  DO 280 I = JD - L, JD - 1
                     CALL DAXPY( I-MMNOBR, ONE, R(MMNOBR+1,I), 1,
     $                           R(MMNOBR+L+1,L+I), 1 )
  280             CONTINUE
C
               END IF
C
               DO 290 I = 2, J - 1
                  CALL DGER( L, L, ONE, Y(NS+I-1,1), LDY, Y(NS+J-1,1),
     $                       LDY, R(ID,JD), LDR )
                  CALL DGER( L, L, -ONE, Y(I-1,1), LDY, Y(J-1,1), LDY,
     $                       R(ID,JD), LDR )
                  ID = ID + L
  290          CONTINUE
C
               DO 300 I = 1, L
                  CALL DAXPY( I, Y(NS+J-1,I), Y(NS+J-1,1), LDY, 
     $                        R(JD,JD+I-1), 1 )
                  CALL DAXPY( I, -Y(J-1,I), Y(J-1,1), LDY, R(JD,JD+I-1),
     $                        1 )
  300          CONTINUE
C
  310       CONTINUE
C
         ELSE
C
            DO 360 J = 2, NOBR2
               JD = JD + L
               ID = MMNOBR + L + 1
C
C              Compute/update  Gyy(1,j)  for sequential processing with
C              connected blocks. 
C
               CALL DGEMM( 'Transpose', 'NoTranspose', L, L, NOBR21,
     $                     ONE, DWORK(LDRWRK*M+1), LDRWRK,
     $                     DWORK(LDRWRK*M+J), LDRWRK, UPD,
     $                     R(MMNOBR+1,JD), LDR )
               CALL DGEMM( 'Transpose', 'NoTranspose', L, L, NS, ONE, Y,
     $                     LDY, Y(J,1), LDY, ONE, R(MMNOBR+1,JD), LDR )
C
C              Compute/update  Gyy(2:j,j)  for sequential processing
C              with connected blocks, exploiting the block-Hankel
C              structure. 
C
               IF( FIRST ) THEN
C
                  DO 320 I = JD - L, JD - 1
                     CALL DCOPY( I-MMNOBR, R(MMNOBR+1,I), 1,
     $                           R(MMNOBR+L+1,L+I), 1 )
  320             CONTINUE
C
               ELSE
C
                  DO 330 I = JD - L, JD - 1
                     CALL DAXPY( I-MMNOBR, ONE, R(MMNOBR+1,I), 1,
     $                           R(MMNOBR+L+1,L+I), 1 )
  330             CONTINUE
C
               END IF
C
               DO 340 I = 2, J - 1
                  CALL DGER( L, L, ONE, Y(NS+I-1,1), LDY, Y(NS+J-1,1),
     $                       LDY, R(ID,JD), LDR )
                  CALL DGER( L, L, -ONE, DWORK(LDRWRK*M+I-1), LDRWRK,
     $                       DWORK(LDRWRK*M+J-1), LDRWRK, R(ID,JD), 
     $                       LDR )
                  ID = ID + L
  340          CONTINUE
C
               DO 350 I = 1, L
                  CALL DAXPY( I, Y(NS+J-1,I), Y(NS+J-1,1), LDY, 
     $                        R(JD,JD+I-1), 1 )
                  CALL DAXPY( I, -DWORK(LDRWRK*(M+I-1)+J-1), 
     $                        DWORK(LDRWRK*M+J-1), LDRWRK, R(JD,JD+I-1),
     $                        1 )
  350          CONTINUE
C
  360       CONTINUE
C
         END IF
C
         IF ( .NOT.LAST ) THEN
            IF ( CONNEC ) THEN
C
C              For sequential processing with connected data blocks, 
C              save the remaining ("connection") elements of  U  and  Y
C              in the first  (M+L)*(2*NOBR-1)  locations of  DWORK.
C
               IF ( M.GT.0 )
     $            CALL DLACPY( 'Full', NOBR21, M, U(NS+1,1), LDU, DWORK,
     $                         NOBR21 )
               CALL DLACPY( 'Full', NOBR21, L, Y(NS+1,1), LDY,
     $                      DWORK(MMNOBR-M+1), NOBR21 )
            END IF
C
C           Return to get new data.
C
            ICYCLE = ICYCLE + 1
            IF ( ICYCLE.GT.MAXCYC )
     $         IWARN = 1
            RETURN
C
         ELSE
C
C           Try to compute the Cholesky factor of the correlation
C           matrix. 
C
            CALL DPOTRF( 'Upper', NR, R, LDR, IERR )
            GO TO 370
         END IF
      ELSE IF ( FQRALG ) THEN
C
C        Compute the  R  factor from a fast QR factorization of the
C        input-output data correlation matrix.
C
         CALL IB01MY( METH, BATCH, CONCT, NOBR, M, L, NSMP, U, LDU,
     $                Y, LDY, R, LDR, IWORK, DWORK, LDWORK, IWARN, 
     $                IERR )
         IF( .NOT.LAST ) 
     $      RETURN
         MAXWRK = MAX( MAXWRK, INT( DWORK(1) ) )
      END IF
C
  370 CONTINUE
C
      IF( IERR.NE.0 ) THEN
C
C        Error return from a fast factorization algorithm of the
C        input-output data correlation matrix.
C
         IF( ONEBCH ) THEN
            QRALG  = .TRUE.
            IWARN  = 2
            MINWRK = 2*NR
            MAXWRK = NR + NR*ILAENV( 1, 'DGEQRF', ' ', NS, NR, -1,
     $                               -1 )
            IF ( LDR.LT.NS ) THEN
               MINWRK = MINWRK + NR
               MAXWRK = NS*NR + MAXWRK
            END IF
            MAXWRK = MAX( MINWRK, MAXWRK )
C     
            IF( LDWORK.LT.MINWRK ) THEN
               INFO = -17
C     
C              Return: Not enough workspace.
C     
               DWORK( 1 ) = MINWRK
               CALL XERBLA( 'IB01MD', -INFO )
               RETURN
            END IF
         ELSE
            INFO = 1
            RETURN
         END IF
      END IF
C
      IF ( QRALG ) THEN
C
C        Compute the  R  factor from a QR factorization of the matrix  H
C        of concatenated block Hankel matrices.
C
C        Construct the matrix  H.
C
C        Set the parameters for constructing the current segment of the
C        Hankel matrix, taking the available memory space into account.
C        INITI+1 points to the beginning rows of  U  and  Y  from which
C                data are taken when NCYCLE > 1 inner cycles are needed,
C                or for sequential processing with connected blocks.
C        LDRWMX is the number of rows that can fit in the working space.
C        LDRWRK is the actual number of rows processed in this space.
C        NSLAST is the number of samples to be processed at the last
C               inner cycle.
C
         INITI  = 0
         LDRWMX = LDWORK / NR - 2
         NCYCLE = 1
         NSLAST = NSMP
         LINR   = .FALSE.
         IF ( FIRST ) THEN
            LINR   = LDR.GE.NS
            LDRWRK = NS
         ELSE IF ( CONNEC ) THEN
            LDRWRK = NSMP
         ELSE
            LDRWRK = NS
         END IF
         INICYC = 1
C
         IF ( .NOT.LINR ) THEN
            IF ( LDRWMX.LT.LDRWRK ) THEN
C
C              Not enough working space for doing a single inner cycle.
C              NCYCLE inner cycles are to be performed for the current
C              data block using the working space.
C
               NCYCLE = LDRWRK / LDRWMX
               NSLAST = MOD( LDRWRK, LDRWMX )
               IF ( NSLAST.NE.0 ) THEN
                  NCYCLE = NCYCLE + 1
               ELSE
                  NSLAST = LDRWMX
               END IF
               LDRWRK = LDRWMX
               NS = LDRWRK
               IF ( FIRST ) INICYC = 2
            END IF
            MLDRW = M*LDRWRK
            LLDRW = L*LDRWRK
            INU = MLDRW*NOBR  + 1
            INY = MLDRW*NOBR2 + 1
         END IF
C
C        Process the data given at the current call.
C
         IF ( .NOT.FIRST .AND. CONNEC ) THEN
C
C           Restore the saved (M+L)*(2*NOBR-1) "connection" elements of
C           U  and  Y  into their appropriate position in sequential
C           processing. The process is performed column-wise, in
C           reverse order, first for  Y  and then for  U.
C
            IREV = NR - M - L - NOBR21 + 1
            ICOL = INY + LLDRW - LDRWRK
C
            DO 380 I = 1, L
               CALL DCOPY( NOBR21, DWORK(IREV), -1, DWORK(ICOL), -1 )
               IREV = IREV - NOBR21
               ICOL = ICOL - LDRWRK
  380       CONTINUE
C
            IF( MOESP ) THEN
               ICOL = INU + MLDRW - LDRWRK
            ELSE
               ICOL = MLDRW - LDRWRK + 1
            END IF
C
            DO 390 I = 1, M
               CALL DCOPY( NOBR21, DWORK(IREV), -1, DWORK(ICOL), -1 )
               IREV = IREV - NOBR21
               ICOL = ICOL - LDRWRK
  390       CONTINUE
C
            IF( MOESP )
     $         CALL DLACPY( 'Full', NOBRM1, M, DWORK(INU+NOBR), LDRWRK,
     $                      DWORK, LDRWRK )
         END IF
C
C        Data compression using QR factorization.
C
         IF ( FIRST ) THEN
C
C           Non-sequential data processing or first block in
C           sequential data processing:
C           Use the general QR factorization algorithm.
C
            IF ( LINR ) THEN
C
C              Put the input-output data in the array  R.
C
               IF( M.GT.0 ) THEN
                  IF( MOESP ) THEN
C
                     DO 400 I = 1, NOBR
                        CALL DLACPY( 'Full', NS, M, U(NOBR+I,1), LDU,
     $                               R(1,M*(I-1)+1), LDR )
  400                CONTINUE
C
                     DO 410 I = 1, NOBR
                        CALL DLACPY( 'Full', NS, M, U(I,1), LDU,
     $                               R(1,MNOBR+M*(I-1)+1), LDR )
  410                CONTINUE
C
                  ELSE
C
                     DO 420 I = 1, NOBR2
                        CALL DLACPY( 'Full', NS, M, U(I,1), LDU,
     $                               R(1,M*(I-1)+1), LDR )
  420                CONTINUE
C
                  END IF
               END IF
C
               DO 430 I = 1, NOBR2
                  CALL DLACPY( 'Full', NS, L, Y(I,1), LDY,
     $                         R(1,MMNOBR+L*(I-1)+1), LDR )
  430          CONTINUE
C
C              Workspace: need   4*(M+L)*NOBR,
C                         prefer 2*(M+L)*NOBR+2*(M+L)*NOBR*NB.
C
               ITAU  = 1
               JWORK = ITAU + NR
               CALL DGEQRF( NS, NR, R, LDR, DWORK(ITAU), DWORK(JWORK),
     $                      LDWORK-JWORK+1, IERR )
            ELSE
C
C              Put the input-output data in the array  DWORK.
C
               IF( M.GT.0 ) THEN
                  ISHFTU = 1
                  IF( MOESP ) THEN
                     ISHFT2 = INU
C
                     DO 440 I = 1, NOBR
                        CALL DLACPY( 'Full', NS, M, U(NOBR+I,1), LDU,
     $                               DWORK(ISHFTU), LDRWRK )
                        ISHFTU = ISHFTU + MLDRW
  440                CONTINUE
C
                     DO 450 I = 1, NOBR
                        CALL DLACPY( 'Full', NS, M, U(I,1), LDU,
     $                               DWORK(ISHFT2), LDRWRK )
                        ISHFT2 = ISHFT2 + MLDRW
  450                CONTINUE
C
                  ELSE
C
                     DO 460 I = 1, NOBR2
                        CALL DLACPY( 'Full', NS, M, U(I,1), LDU,
     $                               DWORK(ISHFTU), LDRWRK )
                        ISHFTU = ISHFTU + MLDRW
  460                CONTINUE
C
                  END IF
               END IF
C
               ISHFTY = INY
C
               DO 470 I = 1, NOBR2
                  CALL DLACPY( 'Full', NS, L, Y(I,1), LDY,
     $                         DWORK(ISHFTY), LDRWRK )
                  ISHFTY = ISHFTY + LLDRW
  470          CONTINUE
C
C              Workspace: need   2*(M+L)*NOBR + 4*(M+L)*NOBR,
C                         prefer NS*2*(M+L)*NOBR + 2*(M+L)*NOBR
C                                                + 2*(M+L)*NOBR*NB,
C                         used LDRWRK*2*(M+L)*NOBR + 4*(M+L)*NOBR,
C                         where  NS = NSMP - 2*NOBR + 1,
C                            LDRWRK = min(NS, LDWORK/(2*(M+L)*NOBR)-2).
C
               ITAU  = LDRWRK*NR + 1
               JWORK = ITAU + NR
               CALL DGEQRF( NS, NR, DWORK, LDRWRK, DWORK(ITAU),
     $                      DWORK(JWORK), LDWORK-JWORK+1, IERR )
               CALL DLACPY( 'Upper ', MIN(NS,NR), NR, DWORK, LDRWRK, R,
     $                      LDR )
            END IF
C
            IF ( NS.LT.NR )
     $         CALL DLASET( 'Upper ', NR - NS, NR - NS, ZERO, ZERO,
     $                      R(NS+1,NS+1), LDR )
            INITI = INITI + NS
         END IF
C
         IF ( NCYCLE.GT.1 .OR. .NOT.FIRST ) THEN
C
C           Remaining segments of the first data block or
C           remaining segments/blocks in sequential data processing:
C           Use a structure-exploiting QR factorization algorithm.
C
            NSL = LDRWRK
            IF ( .NOT.CONNEC ) NSL = NS
            ITAU  = LDRWRK*NR + 1
            JWORK = ITAU + NR
C
            DO 560 NICYCL = INICYC, NCYCLE
C
C              INIT  denotes the beginning row where new data are put.
C
               IF ( CONNEC .AND. NICYCL.EQ.1 ) THEN
                  INIT = NOBR2
               ELSE
                  INIT = 1
               END IF
               IF ( NCYCLE.GT.1 .AND. NICYCL.EQ.NCYCLE ) THEN
C
C                 Last samples in the last data segment of a block.
C
                  NS  = NSLAST
                  NSL = NSLAST
               END IF
C
C              Put the input-output data in the array  DWORK.
C
               NSF = NS
               IF ( INIT.GT.1 .AND. NCYCLE.GT.1 ) NSF = NSF - NOBR21
               IF ( M.GT.0 ) THEN
                  ISHFTU = INIT
C
                  IF( MOESP ) THEN
                     ISHFT2 = INIT + INU - 1
C
                     DO 480 I = 1, NOBR
                        CALL DLACPY( 'Full', NSF, M, U(INITI+NOBR+I,1),
     $                               LDU, DWORK(ISHFTU), LDRWRK )
                        ISHFTU = ISHFTU + MLDRW
  480                CONTINUE
C
                     DO 490 I = 1, NOBR
                        CALL DLACPY( 'Full', NSF, M, U(INITI+I,1), LDU,
     $                               DWORK(ISHFT2), LDRWRK )
                        ISHFT2 = ISHFT2 + MLDRW
  490                CONTINUE
C
                  ELSE
C
                     DO 500 I = 1, NOBR2
                        CALL DLACPY( 'Full', NSF, M, U(INITI+I,1), LDU,
     $                               DWORK(ISHFTU), LDRWRK )
                        ISHFTU = ISHFTU + MLDRW
  500                CONTINUE
C
                  END IF
               END IF
C
               ISHFTY = INIT + INY - 1
C
               DO 510 I = 1, NOBR2
                  CALL DLACPY( 'Full', NSF, L, Y(INITI+I,1), LDY,
     $                         DWORK(ISHFTY), LDRWRK )
                  ISHFTY = ISHFTY + LLDRW
  510          CONTINUE
C
               IF ( INIT.GT.1 ) THEN
C
C                 Prepare the connection to the previous block of data
C                 in sequential processing.
C
                  IF( MOESP .AND. M.GT.0 )
     $               CALL DLACPY( 'Full', NOBR, M, U, LDU, DWORK(NOBR),
     $                            LDRWRK )
C
C                 Shift the elements from the connection to the previous
C                 block of data in sequential processing.
C
                  IF ( M.GT.0 ) THEN
                     ISHFTU = MLDRW + 1
C
                     IF( MOESP ) THEN
                        ISHFT2 = MLDRW + INU
C
                        DO 520 I = 1, NOBRM1
                           CALL DLACPY( 'Full', NOBR21, M,
     $                                  DWORK(ISHFTU-MLDRW+1), LDRWRK, 
     $                                  DWORK(ISHFTU), LDRWRK )
                           ISHFTU = ISHFTU + MLDRW
  520                  CONTINUE
C
                        DO 530 I = 1, NOBRM1
                           CALL DLACPY( 'Full', NOBR21, M,
     $                                  DWORK(ISHFT2-MLDRW+1), LDRWRK, 
     $                                  DWORK(ISHFT2), LDRWRK )
                          ISHFT2 = ISHFT2 + MLDRW
  530                  CONTINUE
C
                     ELSE
C
                        DO 540 I = 1, NOBR21
                           CALL DLACPY( 'Full', NOBR21, M,
     $                                  DWORK(ISHFTU-MLDRW+1), LDRWRK, 
     $                                  DWORK(ISHFTU), LDRWRK )
                           ISHFTU = ISHFTU + MLDRW
  540                   CONTINUE
C
                     END IF
                  END IF
C
                  ISHFTY = LLDRW + INY
C
                  DO 550 I = 1, NOBR21
                     CALL DLACPY( 'Full', NOBR21, L, 
     $                            DWORK(ISHFTY-LLDRW+1), LDRWRK,
     $                            DWORK(ISHFTY), LDRWRK )
                     ISHFTY = ISHFTY + LLDRW
  550             CONTINUE
C
               END IF
C
C              Workspace: need LDRWRK*2*(M+L)*NOBR + 4*(M+L)*NOBR.
C
               CALL MB04OD( 'Full', NR, 0, NSL, R, LDR, DWORK, LDRWRK,
     $                      DUM, NR, DUM, NR, DWORK(ITAU), DWORK(JWORK)
     $                    )
               INITI = INITI + NSF
  560       CONTINUE
C
         END IF
C
         IF ( .NOT.LAST ) THEN
            IF ( CONNEC ) THEN
C
C              For sequential processing with connected data blocks,
C              save the remaining ("connection") elements of  U  and  Y
C              in the first  (M+L)*(2*NOBR-1)  locations of  DWORK.
C
               IF ( M.GT.0 ) 
     $            CALL DLACPY( 'Full', NOBR21, M, U(INITI+1,1), LDU,
     $                         DWORK, NOBR21 )
               CALL DLACPY( 'Full', NOBR21, L, Y(INITI+1,1), LDY,
     $                      DWORK(MMNOBR-M+1), NOBR21 )
            END IF
C
C           Return to get new data.
C
            ICYCLE = ICYCLE + 1
            IF ( ICYCLE.LE.MAXCYC ) 
     $         RETURN
            IWARN  = 1
            ICYCLE = 1
C
         END IF
C
      END IF
C
C     Return optimal workspace in  DWORK(1).
C
      DWORK( 1 ) = MAXWRK
      IF ( LAST ) THEN
         ICYCLE = 1
         MAXWRK = 1
         NSMPSM = 0
      END IF
      RETURN
C
C *** Last line of IB01MD ***
      END
