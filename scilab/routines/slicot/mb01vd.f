      SUBROUTINE MB01VD( TRANA, TRANB, MA, NA, MB, NB, ALPHA, BETA, 
     $                   A, LDA, B, LDB, C, LDC, MC, NC, INFO )
C
C     RELEASE 4.0, WGS COPYRIGHT 2000.
C
C     PURPOSE
C
C     To perform the following matrix operation
C
C        C = alpha*kron( op(A), op(B) ) + beta*C, 
C
C     where alpha and beta are real scalars, op(M) is either matrix M or
C     its transpose, M', and kron( X, Y ) denotes the Kronecker product 
C     of the matrices X and Y.
C
C     ARGUMENTS
C
C     Mode Parameters
C
C     TRANA   CHARACTER*1
C             Specifies the form of op(A) to be used as follows:
C             = 'N':  op(A) = A;
C             = 'T':  op(A) = A';
C             = 'C':  op(A) = A'.
C            
C     TRANB   CHARACTER*1
C             Specifies the form of op(B) to be used as follows:
C             = 'N':  op(B) = B;
C             = 'T':  op(B) = B';
C             = 'C':  op(B) = B'.
C            
C     Input/Output Parameters
C
C     MA      (input) INTEGER
C             The number of rows of the matrix op(A).  MA >= 0.
C
C     NA      (input) INTEGER
C             The number of columns of the matrix op(A).  NA >= 0.
C
C     MB      (input) INTEGER
C             The number of rows of the matrix op(B).  MB >= 0.
C
C     NB      (input) INTEGER
C             The number of columns of the matrix op(B).  NB >= 0.
C
C     ALPHA   (input) DOUBLE PRECISION
C             The scalar alpha. When alpha is zero then A and B need not
C             be set before entry.
C            
C     BETA    (input) DOUBLE PRECISION
C             The scalar beta. When beta is zero then C need not be
C             set before entry.
C            
C     A       (input) DOUBLE PRECISION array, dimension (LDA,ka),
C             where ka is NA when TRANA = 'N', and is MA otherwise.
C             If TRANA = 'N', the leading MA-by-NA part of this array
C             must contain the matrix A; otherwise, the leading NA-by-MA
C             part of this array must contain the matrix A. 
C
C     LDA     INTEGER
C             The leading dimension of the array A.
C             LDA >= max(1,MA), if TRANA = 'N';
C             LDA >= max(1,NA), if TRANA = 'T' or 'C'.
C
C     B       (input) DOUBLE PRECISION array, dimension (LDB,kb)
C             where kb is NB when TRANB = 'N', and is MB otherwise.
C             If TRANB = 'N', the leading MB-by-NB part of this array
C             must contain the matrix B; otherwise, the leading NB-by-MB
C             part of this array must contain the matrix B. 
C
C     LDB     INTEGER
C             The leading dimension of the array B.
C             LDB >= max(1,MB), if TRANB = 'N';
C             LDB >= max(1,NB), if TRANB = 'T' or 'C'.
C
C     C       (input/output) DOUBLE PRECISION array, dimension (LDC,NC)
C             On entry, if beta is nonzero, the leading MC-by-NC part of
C             this array must contain the given matric C, where
C             MC = MA*MB and NC = NA*NB.
C             On exit, the leading MC-by-NC part of this array contains
C             the computed matrix expression
C             C = alpha*kron( op(A), op(B) ) + beta*C.
C
C     LDC     INTEGER
C             The leading dimension of the array C.  
C             LDC >= max(1,MC).
C
C     MC      (output) INTEGER
C             The number of rows of the matrix C.  MC = MA*MB.
C
C     NC      (output) INTEGER
C             The number of columns of the matrix C.  NC = NA*NB.
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
C     The Kronecker product of the matrices op(A) and op(B) is computed 
C     column by column.
C
C     FURTHER COMMENTS
C
C     The multiplications by zero elements in A are avoided, if the 
C     matrix A is considered to be sparse, i.e., if 
C     (number of zeros in A)/(MA*NA) >= SPARST = 0.8. The code makes
C     NB+1 passes through the matrix A, and MA*NA passes through the
C     matrix B. If LDA and/or LDB are very large, and op(A) = A' and/or
C     op(B) = B', it could be more efficient to transpose A and/or B
C     before calling this routine, and use the 'N' values for TRANA
C     and/or TRANB.
C
C     CONTRIBUTOR
C
C     V. Sima, Katholieke Univ. Leuven, Belgium, February 2000.
C
C     REVISIONS
C
C     -   
C
C     KEYWORDS
C
C     Elementary matrix operations, matrix operations.
C
C     ******************************************************************
C
C     .. Parameters ..
      DOUBLE PRECISION  ZERO, ONE
      PARAMETER         ( ZERO = 0.0D0, ONE = 1.0D0 )
      DOUBLE PRECISION  SPARST
      PARAMETER         ( SPARST = 0.8D0 )
C     .. Scalar Arguments ..
      CHARACTER         TRANA, TRANB
      INTEGER           INFO, LDA, LDB, LDC, MA, MB, MC, NA, NB, NC
      DOUBLE PRECISION  ALPHA, BETA
C     .. Array Arguments ..
      DOUBLE PRECISION  A(LDA,*), B(LDB,*), C(LDC,*)
C     .. Local Scalars ..
      LOGICAL           SPARSE, TRANSA, TRANSB
      INTEGER           I, IC, J, JC, K, L, LC, NZ
      DOUBLE PRECISION  AIJ
C     .. Local Arrays ..
      DOUBLE PRECISION  DUM(1)
C     .. External Functions ..
      LOGICAL           LSAME
      EXTERNAL          LSAME
C     .. External Subroutines ..
      EXTERNAL          DCOPY, DLASET, DSCAL, XERBLA
C     .. Intrinsic Functions ..
      INTRINSIC         DBLE, MAX
C
C     .. Executable Statements ..
C
C     Test the input scalar arguments.
C
      TRANSA = LSAME( TRANA, 'T' ) .OR. LSAME( TRANA, 'C' )
      TRANSB = LSAME( TRANB, 'T' ) .OR. LSAME( TRANB, 'C' )
      MC     = MA*MB
      INFO   = 0
      IF( .NOT.( TRANSA .OR. LSAME( TRANA, 'N' ) ) ) THEN
         INFO = -1
      ELSE IF( .NOT.( TRANSB .OR. LSAME( TRANB, 'N' ) ) ) THEN
         INFO = -2
      ELSE IF( MA.LT.0 ) THEN
         INFO = -3
      ELSE IF( NA.LT.0 ) THEN
         INFO = -4
      ELSE IF( MB.LT.0 ) THEN
         INFO = -5
      ELSE IF( NB.LT.0 ) THEN
         INFO = -6
      ELSE IF( ( TRANSA .AND. LDA.LT.NA ) .OR. LDA.LT.1 .OR. 
     $    ( .NOT.TRANSA .AND. LDA.LT.MA ) ) THEN
         INFO = -10
      ELSE IF( ( TRANSB .AND. LDB.LT.NB ) .OR. LDB.LT.1 .OR. 
     $    ( .NOT.TRANSB .AND. LDB.LT.MB ) ) THEN
         INFO = -12
      ELSE IF( LDC.LT.MAX( 1, MC ) ) THEN
         INFO = -14
      END IF
C
      IF ( INFO.NE.0 ) THEN
C
C        Error return.
C
         CALL XERBLA( 'MB01VD', -INFO )
         RETURN
      END IF
C
C     Quick return, if possible.
C
      NC = NA*NB
      IF ( MC.EQ.0 .OR. NC.EQ.0 ) 
     $   RETURN
C
      IF ( ALPHA.EQ.ZERO ) THEN
         IF ( BETA.EQ.ZERO ) THEN
            CALL DLASET( 'Full', MC, NC, ZERO, ZERO, C, LDC )
         ELSE IF ( BETA.NE.ONE ) THEN
C     
            DO 10 J = 1, NC
               CALL DSCAL( MC, BETA, C(1,J), 1 )
   10       CONTINUE
C     
         END IF
         RETURN
      END IF
C
      DUM(1) = ZERO
      JC = 1
      NZ = 0
C
C     Compute the Kronecker product of the matrices op(A) and op(B),
C        C = alpha*kron( op(A), op(B) ) + beta*C.
C     First, check if A is sparse. Here, A is considered as being sparse
C     if (number of zeros in A)/(MA*NA) >= SPARST.
C         
      DO 30 J = 1, NA
C
         DO 20 I = 1, MA
            IF ( TRANSA ) THEN
               IF ( A(J,I).EQ.ZERO ) 
     $            NZ = NZ + 1
            ELSE
               IF ( A(I,J).EQ.ZERO ) 
     $            NZ = NZ + 1
            END IF
   20    CONTINUE
C
   30 CONTINUE
C
      SPARSE = DBLE( NZ )/DBLE( MA*NA ).GE.SPARST
C
      IF ( .NOT.TRANSA .AND. .NOT.TRANSB ) THEN
C
C        Case op(A) = A and op(B) = B.
C
         IF ( BETA.EQ.ZERO ) THEN     
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 0, alpha = 1, A sparse.
C
                  DO 80 J = 1, NA
C        
                     DO 70 K = 1, NB
                        IC = 1
C        
                        DO 60 I = 1, MA
                           AIJ = A(I,J)
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DCOPY( MB, DUM(1), 0, C(IC,JC), 1 )
                           ELSE IF ( AIJ.EQ.ONE ) THEN
                              CALL DCOPY( MB, B(1,K), 1, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 50 L = 1, MB
                                 C(LC,JC) = AIJ*B(L,K)
                                 LC = LC + 1
   50                         CONTINUE
C        
                           END IF
                           IC = IC + MB
   60                   CONTINUE
C        
                        JC = JC + 1
   70                CONTINUE
C        
   80             CONTINUE
C
               ELSE     
C
C                 Case beta = 0, alpha = 1, A not sparse.
C
                  DO 120 J = 1, NA
C        
                     DO 110 K = 1, NB
                        IC = 1
C        
                        DO 100 I = 1, MA
                           AIJ = A(I,J)
                           LC = IC
C        
                           DO 90 L = 1, MB
                              C(LC,JC) = AIJ*B(L,K)
                              LC = LC + 1
   90                      CONTINUE
C        
                           IC = IC + MB
  100                   CONTINUE
C        
                        JC = JC + 1
  110                CONTINUE
C        
  120             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 0, alpha <> 1, A sparse.
C
                  DO 160 J = 1, NA
C        
                     DO 150 K = 1, NB
                        IC = 1
C        
                        DO 140 I = 1, MA
                           AIJ = ALPHA*A(I,J)
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DCOPY( MB, DUM(1), 0, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 130 L = 1, MB
                                 C(LC,JC) = AIJ*B(L,K)
                                 LC = LC + 1
  130                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  140                   CONTINUE
C        
                        JC = JC + 1
  150                CONTINUE
C        
  160             CONTINUE
C
               ELSE     
C
C                 Case beta = 0, alpha <> 1, A not sparse.
C
                  DO 200 J = 1, NA
C        
                     DO 190 K = 1, NB
                        IC = 1
C        
                        DO 180 I = 1, MA
                           AIJ = ALPHA*A(I,J)
                           LC = IC
C        
                           DO 170 L = 1, MB
                              C(LC,JC) = AIJ*B(L,K)
                              LC = LC + 1
  170                      CONTINUE
C        
                           IC = IC + MB
  180                   CONTINUE
C        
                        JC = JC + 1
  190                CONTINUE
C        
  200             CONTINUE
C
               END IF     
            END IF
         ELSE IF ( BETA.EQ.ONE ) THEN 
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 1, alpha = 1, A sparse.
C
                  DO 240 J = 1, NA
C        
                     DO 230 K = 1, NB
                        IC = 1
C        
                        DO 220 I = 1, MA
                           AIJ = A(I,J)
                           IF ( AIJ.NE.ZERO ) THEN
                              LC = IC
C        
                              DO 210 L = 1, MB
                                 C(LC,JC) = C(LC,JC) + AIJ*B(L,K)
                                 LC = LC + 1
  210                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  220                   CONTINUE
C        
                        JC = JC + 1
  230                CONTINUE
C        
  240             CONTINUE
C
               ELSE     
C
C                 Case beta = 1, alpha = 1, A not sparse.
C
                  DO 280 J = 1, NA
C        
                     DO 270 K = 1, NB
                        IC = 1
C        
                        DO 260 I = 1, MA
                           AIJ = A(I,J)
                           LC = IC
C        
                           DO 250 L = 1, MB
                              C(LC,JC) = C(LC,JC) + AIJ*B(L,K)
                              LC = LC + 1
  250                      CONTINUE
C        
                           IC = IC + MB
  260                   CONTINUE
C        
                        JC = JC + 1
  270                CONTINUE
C        
  280             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 1, alpha <> 1, A sparse.
C
                  DO 320 J = 1, NA
C        
                     DO 310 K = 1, NB
                        IC = 1
C        
                        DO 300 I = 1, MA
                           AIJ = ALPHA*A(I,J)
                           IF ( AIJ.NE.ZERO ) THEN
                              LC = IC
C        
                              DO 290 L = 1, MB
                                 C(LC,JC) = C(LC,JC) + AIJ*B(L,K)
                                 LC = LC + 1
  290                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  300                   CONTINUE
C        
                        JC = JC + 1
  310                CONTINUE
C        
  320             CONTINUE
C
               ELSE     
C
C                 Case beta = 1, alpha <> 1, A not sparse.
C
                  DO 360 J = 1, NA
C        
                     DO 350 K = 1, NB
                        IC = 1
C        
                        DO 340 I = 1, MA
                           AIJ = ALPHA*A(I,J)
                           LC = IC
C        
                           DO 330 L = 1, MB
                              C(LC,JC) = C(LC,JC) + AIJ*B(L,K)
                              LC = LC + 1
  330                      CONTINUE
C        
                           IC = IC + MB
  340                   CONTINUE
C        
                        JC = JC + 1
  350                CONTINUE
C        
  360             CONTINUE
C
               END IF     
            END IF
         ELSE 
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta <> 0 or 1, alpha = 1, A sparse.
C
                  DO 400 J = 1, NA
C        
                     DO 390 K = 1, NB
                        IC = 1
C        
                        DO 380 I = 1, MA
                           AIJ = A(I,J)
C
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DSCAL( MB, BETA, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 370 L = 1, MB
                                 C(LC,JC) = BETA*C(LC,JC) + AIJ*B(L,K)
                                 LC = LC + 1
  370                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  380                   CONTINUE
C        
                        JC = JC + 1
  390                CONTINUE
C        
  400             CONTINUE
C
               ELSE     
C
C                 Case beta <> 0 or 1, alpha = 1, A not sparse.
C
                  DO 440 J = 1, NA
C        
                     DO 430 K = 1, NB
                        IC = 1
C        
                        DO 420 I = 1, MA
                           AIJ = A(I,J)
                           LC = IC
C        
                           DO 410 L = 1, MB
                              C(LC,JC) = BETA*C(LC,JC) + AIJ*B(L,K)
                              LC = LC + 1
  410                      CONTINUE
C        
                           IC = IC + MB
  420                   CONTINUE
C        
                        JC = JC + 1
  430                CONTINUE
C        
  440             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta <> 0 or 1, alpha <> 1, A sparse.
C
                  DO 480 J = 1, NA
C        
                     DO 470 K = 1, NB
                        IC = 1
C        
                        DO 460 I = 1, MA
                           AIJ = ALPHA*A(I,J)
C
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DSCAL( MB, BETA, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 450 L = 1, MB
                                 C(LC,JC) = BETA*C(LC,JC) + AIJ*B(L,K)
                                 LC = LC + 1
  450                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  460                   CONTINUE
C        
                        JC = JC + 1
  470                CONTINUE
C        
  480             CONTINUE
C
               ELSE     
C
C                 Case beta <> 0 or 1, alpha <> 1, A not sparse.
C
                  DO 520 J = 1, NA
C        
                     DO 510 K = 1, NB
                        IC = 1
C        
                        DO 500 I = 1, MA
                           AIJ = ALPHA*A(I,J)
                           LC = IC
C        
                           DO 490 L = 1, MB
                              C(LC,JC) = BETA*C(LC,JC) + AIJ*B(L,K)
                              LC = LC + 1
  490                      CONTINUE
C        
                           IC = IC + MB
  500                   CONTINUE
C        
                        JC = JC + 1
  510                CONTINUE
C        
  520             CONTINUE
C
               END IF     
            END IF
         END IF 
      ELSE IF ( TRANSA .AND. .NOT.TRANSB ) THEN
C
C        Case op(A) = A' and op(B) = B.
C     
         IF ( BETA.EQ.ZERO ) THEN     
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 0, alpha = 1, A sparse.
C
                  DO 560 J = 1, NA
C        
                     DO 550 K = 1, NB
                        IC = 1
C        
                        DO 540 I = 1, MA
                           AIJ = A(J,I)
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DCOPY( MB, DUM(1), 0, C(IC,JC), 1 )
                           ELSE IF ( AIJ.EQ.ONE ) THEN
                              CALL DCOPY( MB, B(1,K), 1, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 530 L = 1, MB
                                 C(LC,JC) = AIJ*B(L,K)
                                 LC = LC + 1
  530                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  540                   CONTINUE
C        
                        JC = JC + 1
  550                CONTINUE
C        
  560             CONTINUE
C
               ELSE     
C
C                 Case beta = 0, alpha = 1, A not sparse.
C
                  DO 600 J = 1, NA
C        
                     DO 590 K = 1, NB
                        IC = 1
C        
                        DO 580 I = 1, MA
                           AIJ = A(J,I)
                           LC = IC
C        
                           DO 570 L = 1, MB
                              C(LC,JC) = AIJ*B(L,K)
                              LC = LC + 1
  570                      CONTINUE
C        
                           IC = IC + MB
  580                   CONTINUE
C        
                        JC = JC + 1
  590                CONTINUE
C        
  600             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 0, alpha <> 1, A sparse.
C
                  DO 640 J = 1, NA
C        
                     DO 630 K = 1, NB
                        IC = 1
C        
                        DO 620 I = 1, MA
                           AIJ = ALPHA*A(J,I)
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DCOPY( MB, DUM(1), 0, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 610 L = 1, MB
                                 C(LC,JC) = AIJ*B(L,K)
                                 LC = LC + 1
  610                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  620                   CONTINUE
C        
                        JC = JC + 1
  630                CONTINUE
C        
  640             CONTINUE
C
               ELSE     
C
C                 Case beta = 0, alpha <> 1, A not sparse.
C
                  DO 680 J = 1, NA
C        
                     DO 670 K = 1, NB
                        IC = 1
C        
                        DO 660 I = 1, MA
                           AIJ = ALPHA*A(J,I)
                           LC = IC
C        
                           DO 650 L = 1, MB
                              C(LC,JC) = AIJ*B(L,K)
                              LC = LC + 1
  650                      CONTINUE
C        
                           IC = IC + MB
  660                   CONTINUE
C        
                        JC = JC + 1
  670                CONTINUE
C        
  680             CONTINUE
C
               END IF     
            END IF
         ELSE IF ( BETA.EQ.ONE ) THEN 
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 1, alpha = 1, A sparse.
C
                  DO 720 J = 1, NA
C        
                     DO 710 K = 1, NB
                        IC = 1
C        
                        DO 700 I = 1, MA
                           AIJ = A(J,I)
                           IF ( AIJ.NE.ZERO ) THEN
                              LC = IC
C        
                              DO 690 L = 1, MB
                                 C(LC,JC) = C(LC,JC) + AIJ*B(L,K)
                                 LC = LC + 1
  690                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  700                   CONTINUE
C        
                        JC = JC + 1
  710                CONTINUE
C        
  720             CONTINUE
C
               ELSE     
C
C                 Case beta = 1, alpha = 1, A not sparse.
C
                  DO 760 J = 1, NA
C        
                     DO 750 K = 1, NB
                        IC = 1
C        
                        DO 740 I = 1, MA
                           AIJ = A(J,I)
                           LC = IC
C        
                           DO 730 L = 1, MB
                              C(LC,JC) = C(LC,JC) + AIJ*B(L,K)
                              LC = LC + 1
  730                      CONTINUE
C        
                           IC = IC + MB
  740                   CONTINUE
C        
                        JC = JC + 1
  750                CONTINUE
C        
  760             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 1, alpha <> 1, A sparse.
C
                  DO 800 J = 1, NA
C        
                     DO 790 K = 1, NB
                        IC = 1
C        
                        DO 780 I = 1, MA
                           AIJ = ALPHA*A(J,I)
                           IF ( AIJ.NE.ZERO ) THEN
                              LC = IC
C        
                              DO 770 L = 1, MB
                                 C(LC,JC) = C(LC,JC) + AIJ*B(L,K)
                                 LC = LC + 1
  770                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  780                   CONTINUE
C        
                        JC = JC + 1
  790                CONTINUE
C        
  800             CONTINUE
C
               ELSE     
C
C                 Case beta = 1, alpha <> 1, A not sparse.
C
                  DO 840 J = 1, NA
C        
                     DO 830 K = 1, NB
                        IC = 1
C        
                        DO 820 I = 1, MA
                           AIJ = ALPHA*A(J,I)
                           LC = IC
C        
                           DO 810 L = 1, MB
                              C(LC,JC) = C(LC,JC) + AIJ*B(L,K)
                              LC = LC + 1
  810                      CONTINUE
C        
                           IC = IC + MB
  820                   CONTINUE
C        
                        JC = JC + 1
  830                CONTINUE
C        
  840             CONTINUE
C
               END IF     
            END IF
         ELSE 
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta <> 0 or 1, alpha = 1, A sparse.
C
                  DO 880 J = 1, NA
C        
                     DO 870 K = 1, NB
                        IC = 1
C        
                        DO 860 I = 1, MA
                           AIJ = A(J,I)
C
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DSCAL( MB, BETA, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 850 L = 1, MB
                                 C(LC,JC) = BETA*C(LC,JC) + AIJ*B(L,K)
                                 LC = LC + 1
  850                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  860                   CONTINUE
C        
                        JC = JC + 1
  870                CONTINUE
C        
  880             CONTINUE
C
               ELSE     
C
C                 Case beta <> 0 or 1, alpha = 1, A not sparse.
C
                  DO 920 J = 1, NA
C        
                     DO 910 K = 1, NB
                        IC = 1
C        
                        DO 900 I = 1, MA
                           AIJ = A(J,I)
                           LC = IC
C        
                           DO 890 L = 1, MB
                              C(LC,JC) = BETA*C(LC,JC) + AIJ*B(L,K)
                              LC = LC + 1
  890                      CONTINUE
C        
                           IC = IC + MB
  900                   CONTINUE
C        
                        JC = JC + 1
  910                CONTINUE
C        
  920             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta <> 0 or 1, alpha <> 1, A sparse.
C
                  DO 960 J = 1, NA
C        
                     DO 950 K = 1, NB
                        IC = 1
C        
                        DO 940 I = 1, MA
                           AIJ = ALPHA*A(J,I)
C
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DSCAL( MB, BETA, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 930 L = 1, MB
                                 C(LC,JC) = BETA*C(LC,JC) + AIJ*B(L,K)
                                 LC = LC + 1
  930                         CONTINUE
C        
                           END IF
                           IC = IC + MB
  940                   CONTINUE
C        
                        JC = JC + 1
  950                CONTINUE
C        
  960             CONTINUE
C
               ELSE     
C
C                 Case beta <> 0 or 1, alpha <> 1, A not sparse.
C
                  DO 1000 J = 1, NA
C        
                     DO 990 K = 1, NB
                        IC = 1
C        
                        DO 980 I = 1, MA
                           AIJ = ALPHA*A(J,I)
                           LC = IC
C        
                           DO 970 L = 1, MB
                              C(LC,JC) = BETA*C(LC,JC) + AIJ*B(L,K)
                              LC = LC + 1
  970                      CONTINUE
C        
                           IC = IC + MB
  980                   CONTINUE
C        
                        JC = JC + 1
  990                CONTINUE
C        
 1000             CONTINUE
C
               END IF     
            END IF
         END IF 
      ELSE IF ( TRANSB .AND. .NOT.TRANSA ) THEN
C
C        Case op(A) = A and op(B) = B'.
C     
         IF ( BETA.EQ.ZERO ) THEN     
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 0, alpha = 1, A sparse.
C
                  DO 1080 J = 1, NA
C        
                     DO 1070 K = 1, NB
                        IC = 1
C        
                        DO 1060 I = 1, MA
                           AIJ = A(I,J)
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DCOPY( MB, DUM(1), 0, C(IC,JC), 1 )
                           ELSE IF ( AIJ.EQ.ONE ) THEN
                              CALL DCOPY( MB, B(K,1), LDB, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 1050 L = 1, MB
                                 C(LC,JC) = AIJ*B(K,L)
                                 LC = LC + 1
 1050                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1060                   CONTINUE
C        
                        JC = JC + 1
 1070                CONTINUE
C        
 1080             CONTINUE
C
               ELSE     
C
C                 Case beta = 0, alpha = 1, A not sparse.
C
                  DO 1120 J = 1, NA
C        
                     DO 1110 K = 1, NB
                        IC = 1
C        
                        DO 1100 I = 1, MA
                           AIJ = A(I,J)
                           LC = IC
C        
                           DO 1090 L = 1, MB
                              C(LC,JC) = AIJ*B(K,L)
                              LC = LC + 1
 1090                      CONTINUE
C        
                           IC = IC + MB
 1100                   CONTINUE
C        
                        JC = JC + 1
 1110                CONTINUE
C        
 1120             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 0, alpha <> 1, A sparse.
C
                  DO 1160 J = 1, NA
C        
                     DO 1150 K = 1, NB
                        IC = 1
C        
                        DO 1140 I = 1, MA
                           AIJ = ALPHA*A(I,J)
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DCOPY( MB, DUM(1), 0, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 1130 L = 1, MB
                                 C(LC,JC) = AIJ*B(K,L)
                                 LC = LC + 1
 1130                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1140                   CONTINUE
C        
                        JC = JC + 1
 1150                CONTINUE
C        
 1160             CONTINUE
C
               ELSE     
C
C                 Case beta = 0, alpha <> 1, A not sparse.
C
                  DO 1200 J = 1, NA
C        
                     DO 1190 K = 1, NB
                        IC = 1
C        
                        DO 1180 I = 1, MA
                           AIJ = ALPHA*A(I,J)
                           LC = IC
C        
                           DO 1170 L = 1, MB
                              C(LC,JC) = AIJ*B(K,L)
                              LC = LC + 1
 1170                      CONTINUE
C        
                           IC = IC + MB
 1180                   CONTINUE
C        
                        JC = JC + 1
 1190                CONTINUE
C        
 1200             CONTINUE
C
               END IF     
            END IF
         ELSE IF ( BETA.EQ.ONE ) THEN 
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 1, alpha = 1, A sparse.
C
                  DO 1240 J = 1, NA
C        
                     DO 1230 K = 1, NB
                        IC = 1
C        
                        DO 1220 I = 1, MA
                           AIJ = A(I,J)
                           IF ( AIJ.NE.ZERO ) THEN
                              LC = IC
C        
                              DO 1210 L = 1, MB
                                 C(LC,JC) = C(LC,JC) + AIJ*B(K,L)
                                 LC = LC + 1
 1210                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1220                   CONTINUE
C        
                        JC = JC + 1
 1230                CONTINUE
C        
 1240             CONTINUE
C
               ELSE     
C
C                 Case beta = 1, alpha = 1, A not sparse.
C
                  DO 1280 J = 1, NA
C        
                     DO 1270 K = 1, NB
                        IC = 1
C        
                        DO 1260 I = 1, MA
                           AIJ = A(I,J)
                           LC = IC
C        
                           DO 1250 L = 1, MB
                              C(LC,JC) = C(LC,JC) + AIJ*B(K,L)
                              LC = LC + 1
 1250                      CONTINUE
C        
                           IC = IC + MB
 1260                   CONTINUE
C        
                        JC = JC + 1
 1270                CONTINUE
C        
 1280             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 1, alpha <> 1, A sparse.
C
                  DO 1320 J = 1, NA
C        
                     DO 1310 K = 1, NB
                        IC = 1
C        
                        DO 1300 I = 1, MA
                           AIJ = ALPHA*A(I,J)
                           IF ( AIJ.NE.ZERO ) THEN
                              LC = IC
C        
                              DO 1290 L = 1, MB
                                 C(LC,JC) = C(LC,JC) + AIJ*B(K,L)
                                 LC = LC + 1
 1290                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1300                   CONTINUE
C        
                        JC = JC + 1
 1310                CONTINUE
C        
 1320             CONTINUE
C
               ELSE     
C
C                 Case beta = 1, alpha <> 1, A not sparse.
C
                  DO 1360 J = 1, NA
C        
                     DO 1350 K = 1, NB
                        IC = 1
C        
                        DO 1340 I = 1, MA
                           AIJ = ALPHA*A(I,J)
                           LC = IC
C        
                           DO 1330 L = 1, MB
                              C(LC,JC) = C(LC,JC) + AIJ*B(K,L)
                              LC = LC + 1
 1330                      CONTINUE
C        
                           IC = IC + MB
 1340                   CONTINUE
C        
                        JC = JC + 1
 1350                CONTINUE
C        
 1360             CONTINUE
C
               END IF     
            END IF
         ELSE 
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta <> 0 or 1, alpha = 1, A sparse.
C
                  DO 1400 J = 1, NA
C        
                     DO 1390 K = 1, NB
                        IC = 1
C        
                        DO 1380 I = 1, MA
                           AIJ = A(I,J)
C
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DSCAL( MB, BETA, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 1370 L = 1, MB
                                 C(LC,JC) = BETA*C(LC,JC) + AIJ*B(K,L)
                                 LC = LC + 1
 1370                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1380                   CONTINUE
C        
                        JC = JC + 1
 1390                CONTINUE
C        
 1400             CONTINUE
C
               ELSE     
C
C                 Case beta <> 0 or 1, alpha = 1, A not sparse.
C
                  DO 1440 J = 1, NA
C        
                     DO 1430 K = 1, NB
                        IC = 1
C        
                        DO 1420 I = 1, MA
                           AIJ = A(I,J)
                           LC = IC
C        
                           DO 1410 L = 1, MB
                              C(LC,JC) = BETA*C(LC,JC) + AIJ*B(K,L)
                              LC = LC + 1
 1410                      CONTINUE
C        
                           IC = IC + MB
 1420                   CONTINUE
C        
                        JC = JC + 1
 1430                CONTINUE
C        
 1440             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta <> 0 or 1, alpha <> 1, A sparse.
C
                  DO 1480 J = 1, NA
C        
                     DO 1470 K = 1, NB
                        IC = 1
C        
                        DO 1460 I = 1, MA
                           AIJ = ALPHA*A(I,J)
C
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DSCAL( MB, BETA, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 1450 L = 1, MB
                                 C(LC,JC) = BETA*C(LC,JC) + AIJ*B(K,L)
                                 LC = LC + 1
 1450                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1460                   CONTINUE
C        
                        JC = JC + 1
 1470                CONTINUE
C        
 1480             CONTINUE
C
               ELSE     
C
C                 Case beta <> 0 or 1, alpha <> 1, A not sparse.
C
                  DO 1520 J = 1, NA
C        
                     DO 1510 K = 1, NB
                        IC = 1
C        
                        DO 1500 I = 1, MA
                           AIJ = ALPHA*A(I,J)
                           LC = IC
C        
                           DO 1490 L = 1, MB
                              C(LC,JC) = BETA*C(LC,JC) + AIJ*B(K,L)
                              LC = LC + 1
 1490                      CONTINUE
C        
                           IC = IC + MB
 1500                   CONTINUE
C        
                        JC = JC + 1
 1510                CONTINUE
C        
 1520             CONTINUE
C
               END IF     
            END IF
         END IF 
      ELSE 
C
C        Case op(A) = A' and op(B) = B'.
C     
         IF ( BETA.EQ.ZERO ) THEN     
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 0, alpha = 1, A sparse.
C
                  DO 1580 J = 1, NA
C        
                     DO 1570 K = 1, NB
                        IC = 1
C        
                        DO 1560 I = 1, MA
                           AIJ = A(J,I)
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DCOPY( MB, DUM(1), 0, C(IC,JC), 1 )
                           ELSE IF ( AIJ.EQ.ONE ) THEN
                              CALL DCOPY( MB, B(K,1), LDB, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 1550 L = 1, MB
                                 C(LC,JC) = AIJ*B(K,L)
                                 LC = LC + 1
 1550                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1560                   CONTINUE
C        
                        JC = JC + 1
 1570                CONTINUE
C        
 1580             CONTINUE
C
               ELSE     
C
C                 Case beta = 0, alpha = 1, A not sparse.
C
                  DO 1620 J = 1, NA
C        
                     DO 1610 K = 1, NB
                        IC = 1
C        
                        DO 1600 I = 1, MA
                           AIJ = A(J,I)
                           LC = IC
C        
                           DO 1590 L = 1, MB
                              C(LC,JC) = AIJ*B(K,L)
                              LC = LC + 1
 1590                      CONTINUE
C        
                           IC = IC + MB
 1600                   CONTINUE
C        
                        JC = JC + 1
 1610                CONTINUE
C        
 1620             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 0, alpha <> 1, A sparse.
C
                  DO 1660 J = 1, NA
C        
                     DO 1650 K = 1, NB
                        IC = 1
C        
                        DO 1640 I = 1, MA
                           AIJ = ALPHA*A(J,I)
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DCOPY( MB, DUM(1), 0, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 1630 L = 1, MB
                                 C(LC,JC) = AIJ*B(K,L)
                                 LC = LC + 1
 1630                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1640                   CONTINUE
C        
                        JC = JC + 1
 1650                CONTINUE
C        
 1660             CONTINUE
C
               ELSE     
C
C                 Case beta = 0, alpha <> 1, A not sparse.
C
                  DO 1700 J = 1, NA
C        
                     DO 1690 K = 1, NB
                        IC = 1
C        
                        DO 1680 I = 1, MA
                           AIJ = ALPHA*A(J,I)
                           LC = IC
C        
                           DO 1670 L = 1, MB
                              C(LC,JC) = AIJ*B(K,L)
                              LC = LC + 1
 1670                      CONTINUE
C        
                           IC = IC + MB
 1680                   CONTINUE
C        
                        JC = JC + 1
 1690                CONTINUE
C       
 1700             CONTINUE
C
               END IF     
            END IF
         ELSE IF ( BETA.EQ.ONE ) THEN 
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 1, alpha = 1, A sparse.
C
                  DO 1740 J = 1, NA
C        
                     DO 1730 K = 1, NB
                        IC = 1
C        
                        DO 1720 I = 1, MA
                           AIJ = A(J,I)
                           IF ( AIJ.NE.ZERO ) THEN
                              LC = IC
C        
                              DO 1710 L = 1, MB
                                 C(LC,JC) = C(LC,JC) + AIJ*B(K,L)
                                 LC = LC + 1
 1710                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1720                   CONTINUE
C        
                        JC = JC + 1
 1730                CONTINUE
C        
 1740             CONTINUE
C
               ELSE     
C
C                 Case beta = 1, alpha = 1, A not sparse.
C
                  DO 1780 J = 1, NA
C        
                     DO 1770 K = 1, NB
                        IC = 1
C        
                        DO 1760 I = 1, MA
                           AIJ = A(J,I)
                           LC = IC
C        
                           DO 1750 L = 1, MB
                              C(LC,JC) = C(LC,JC) + AIJ*B(K,L)
                              LC = LC + 1
 1750                      CONTINUE
C        
                           IC = IC + MB
 1760                   CONTINUE
C        
                        JC = JC + 1
 1770                CONTINUE
C        
 1780             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta = 1, alpha <> 1, A sparse.
C
                  DO 1820 J = 1, NA
C        
                     DO 1810 K = 1, NB
                        IC = 1
C        
                        DO 1800 I = 1, MA
                           AIJ = ALPHA*A(J,I)
                           IF ( AIJ.NE.ZERO ) THEN
                              LC = IC
C        
                              DO 1790 L = 1, MB
                                 C(LC,JC) = C(LC,JC) + AIJ*B(K,L)
                                 LC = LC + 1
 1790                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1800                   CONTINUE
C        
                        JC = JC + 1
 1810                CONTINUE
C        
 1820             CONTINUE
C
               ELSE     
C
C                 Case beta = 1, alpha <> 1, A not sparse.
C
                  DO 1860 J = 1, NA
C        
                     DO 1850 K = 1, NB
                        IC = 1
C        
                        DO 1840 I = 1, MA
                           AIJ = ALPHA*A(J,I)
                           LC = IC
C        
                           DO 1830 L = 1, MB
                              C(LC,JC) = C(LC,JC) + AIJ*B(K,L)
                              LC = LC + 1
 1830                      CONTINUE
C        
                           IC = IC + MB
 1840                   CONTINUE
C        
                        JC = JC + 1
 1850                CONTINUE
C        
 1860             CONTINUE
C
               END IF     
            END IF
         ELSE 
            IF ( ALPHA.EQ.ONE ) THEN     
               IF ( SPARSE ) THEN     
C
C                 Case beta <> 0 or 1, alpha = 1, A sparse.
C
                  DO 1900 J = 1, NA
C        
                     DO 1890 K = 1, NB
                        IC = 1
C        
                        DO 1880 I = 1, MA
                           AIJ = A(J,I)
C
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DSCAL( MB, BETA, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 1870 L = 1, MB
                                 C(LC,JC) = BETA*C(LC,JC) + AIJ*B(K,L)
                                 LC = LC + 1
 1870                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1880                   CONTINUE
C        
                        JC = JC + 1
 1890                CONTINUE
C        
 1900             CONTINUE
C
               ELSE     
C
C                 Case beta <> 0 or 1, alpha = 1, A not sparse.
C
                  DO 1940 J = 1, NA
C        
                     DO 1930 K = 1, NB
                        IC = 1
C        
                        DO 1920 I = 1, MA
                           AIJ = A(J,I)
                           LC = IC
C        
                           DO 1910 L = 1, MB
                              C(LC,JC) = BETA*C(LC,JC) + AIJ*B(K,L)
                              LC = LC + 1
 1910                      CONTINUE
C        
                           IC = IC + MB
 1920                   CONTINUE
C        
                        JC = JC + 1
 1930                CONTINUE
C        
 1940             CONTINUE
C
               END IF     
            ELSE     
               IF ( SPARSE ) THEN     
C
C                 Case beta <> 0 or 1, alpha <> 1, A sparse.
C
                  DO 1980 J = 1, NA
C        
                     DO 1970 K = 1, NB
                        IC = 1
C        
                        DO 1960 I = 1, MA
                           AIJ = ALPHA*A(J,I)
C
                           IF ( AIJ.EQ.ZERO ) THEN
                              CALL DSCAL( MB, BETA, C(IC,JC), 1 )
                           ELSE 
                              LC = IC
C        
                              DO 1950 L = 1, MB
                                 C(LC,JC) = BETA*C(LC,JC) + AIJ*B(K,L)
                                 LC = LC + 1
 1950                         CONTINUE
C        
                           END IF
                           IC = IC + MB
 1960                   CONTINUE
C        
                        JC = JC + 1
 1970                CONTINUE
C        
 1980             CONTINUE
C
               ELSE     
C
C                 Case beta <> 0 or 1, alpha <> 1, A not sparse.
C
                  DO 2020 J = 1, NA
C        
                     DO 2010 K = 1, NB
                        IC = 1
C        
                        DO 2000 I = 1, MA
                           AIJ = ALPHA*A(J,I)
                           LC = IC
C        
                           DO 1990 L = 1, MB
                              C(LC,JC) = BETA*C(LC,JC) + AIJ*B(K,L)
                              LC = LC + 1
 1990                      CONTINUE
C        
                           IC = IC + MB
 2000                   CONTINUE
C        
                        JC = JC + 1
 2010                CONTINUE
C        
 2020             CONTINUE
C
               END IF     
            END IF
         END IF 
      END IF
      RETURN
C *** Last line of MB01VD ***
      END
