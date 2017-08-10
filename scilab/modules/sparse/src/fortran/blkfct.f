C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  March 6, 1995
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C*********     BLKFCT .....  BLOCK GENERAL SPARSE CHOLESKY     *********
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       THIS SUBROUTINE CALLS THE BLOCK GENERAL SPARSE CHOLESKY ROUTINE,
C       BLKFC2.
C
C   INPUT PARAMETERS:
C       NSUPER          -   NUMBER OF SUPERNODES.
C       XSUPER          -   SUPERNODE PARTITION.
C       SNODE           -   MAPS EACH COLUMN TO THE SUPERNODE CONTAINING
C                           IT.
C       SPLIT           -   SPLITTING OF SUPERNODES SO THAT THEY FIT
C                           INTO CACHE.
C       (XLINDX,LINDX)  -   ROW INDICES FOR EACH SUPERNODE (INCLUDING
C                           THE DIAGONAL ELEMENTS).
C       (XLNZ,LNZ)      -   ON INPUT, CONTAINS MATRIX TO BE FACTORED.
C       IWSIZ           -   SIZE OF INTEGER WORKING STORAGE
C       TMPSIZ          -   SIZE OF FLOATING POINT WORKING STORAGE.
C       MMPYN           -   EXTERNAL ROUTINE: MATRIX-MATRIX MULTIPLY.
C       SMXPY           -   EXTERNAL ROUTINE: MATRIX-VECTOR MULTIPLY.
C
C   OUTPUT PARAMETERS:
C       LNZ             -   ON OUTPUT, CONTAINS CHOLESKY FACTOR.
C       IFLAG           -   ERROR FLAG.
C                               0: SUCCESSFUL FACTORIZATION.
C                              -1: NONPOSITIVE DIAGONAL ENCOUNTERED,
C                                  MATRIX IS NOT POSITIVE DEFINITE.
C                              -2: INSUFFICIENT WORKING STORAGE 
C                                  [TEMP(*)].
C                              -3: INSUFFICIENT WORKING STORAGE 
C                                  [IWORK(*)].
C
C   WORKING PARAMETERS:
C       IWORK           -   INTEGER WORKING STORAGE OF LENGTH 
C                           2*NEQNS + 2*NSUPER.
C       TMPVEC          -   DOUBLE PRECISION WORKING STORAGE OF LENGTH
C                           NEQNS.
C       
C***********************************************************************
C
      SUBROUTINE  BLKFCT (  NEQNS , NSUPER, XSUPER, SNODE , SPLIT , 
     &                      XLINDX, LINDX , XLNZ  , LNZ   , IWSIZ ,
     &                      IWORK , TMPSIZ, TMPVEC, IFLAG , MMPYN , 
     &                      SMXPY                                   )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        EXTERNAL            MMPYN , SMXPY
        INTEGER             XLINDX(*)     , XLNZ(*)
        INTEGER             IWORK(*)      , LINDX(*)      , 
     &                      SNODE(*)      , SPLIT(*)      , 
     &                      XSUPER(*)
        INTEGER             IFLAG , IWSIZ , NEQNS , NSUPER, TMPSIZ
        DOUBLE PRECISION    LNZ(*)        , TMPVEC(*)
C
C*********************************************************************
C
        IFLAG = 0
        IF  ( IWSIZ .LT. 2*NEQNS+2*NSUPER )  THEN
            IFLAG = -3
            RETURN
        ENDIF
        CALL  BLKFC2 (  NSUPER, XSUPER, SNODE , SPLIT , XLINDX,
     &                  LINDX , XLNZ  , LNZ   , 
     &                  IWORK(1)                      ,
     &                  IWORK(NSUPER+1)               ,
     &                  IWORK(2*NSUPER+1)             ,
     &                  IWORK(2*NSUPER+NEQNS+1)       ,
     &                  TMPSIZ, TMPVEC, IFLAG , MMPYN , SMXPY   )
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  March 6, 1995
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C*********     BLKFC2 .....  BLOCK GENERAL SPARSE CHOLESKY     *********
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       THIS SUBROUTINE FACTORS A SPARSE POSITIVE DEFINITE MATRIX.
C       THE COMPUTATION IS ORGANIZED AROUND KERNELS THAT PERFORM
C       SUPERNODE-TO-SUPERNODE UPDATES, I.E., BLOCK-TO-BLOCK UPDATES.
C
C   INPUT PARAMETERS:
C       NSUPER          -   NUMBER OF SUPERNODES.
C       XSUPER          -   SUPERNODE PARTITION.
C       SNODE           -   MAPS EACH COLUMN TO THE SUPERNODE CONTAINING
C                           IT.
C       SPLIT           -   SPLITTING OF SUPERNODES SO THAT THEY FIT
C                           INTO CACHE.
C       (XLINDX,LINDX)  -   ROW INDICES FOR EACH SUPERNODE (INCLUDING
C                           THE DIAGONAL ELEMENTS).
C       (XLNZ,LNZ)      -   ON INPUT, CONTAINS MATRIX TO BE FACTORED.
C       TMPSIZ          -   SIZE OF TEMPORARY WORKING STORAGE.
C       MMPYN           -   EXTERNAL ROUTINE: MATRIX-MATRIX MULTIPLY.
C       SMXPY           -   EXTERNAL ROUTINE: MATRIX-VECTOR MULTIPLY.
C
C   OUTPUT PARAMETERS:
C       LNZ             -   ON OUTPUT, CONTAINS CHOLESKY FACTOR.
C       IFLAG           -   ERROR FLAG.
C                               0: SUCCESSFUL FACTORIZATION.
C                              -1: NONPOSITIVE DIAGONAL ENCOUNTERED,
C                                  MATRIX IS NOT POSITIVE DEFINITE.
C                              -2: INSUFFICIENT WORKING STORAGE 
C                                  [TEMP(*)].
C
C   WORKING PARAMETERS:
C       LINK            -   LINKS TOGETHER THE SUPERNODES IN A SUPERNODE
C                           ROW.
C       LENGTH          -   LENGTH OF THE ACTIVE PORTION OF EACH 
C                           SUPERNODE.
C       INDMAP          -   VECTOR OF SIZE NEQNS INTO WHICH THE GLOBAL
C                           INDICES ARE SCATTERED.
C       RELIND          -   MAPS LOCATIONS IN THE UPDATING COLUMNS TO 
C                           THE CORRESPONDING LOCATIONS IN THE UPDATED 
C                           COLUMNS.  (RELIND IS GATHERED FROM INDMAP).
C       TEMP            -   REAL VECTOR FOR ACCUMULATING UPDATES.  MUST
C                           ACCOMODATE ALL COLUMNS OF A SUPERNODE. 
C       
C***********************************************************************
C
      SUBROUTINE  BLKFC2 (  NSUPER, XSUPER, SNODE , SPLIT , XLINDX,
     &                      LINDX , XLNZ  , LNZ   , LINK  , LENGTH,
     &                      INDMAP, RELIND, TMPSIZ, TEMP  , IFLAG ,
     &                      MMPYN , SMXPY                           )
C
C*********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        EXTERNAL            MMPYN , SMXPY
        INTEGER             XLINDX(*)     , XLNZ(*)
        INTEGER             INDMAP(*)     , LENGTH(*)     ,
     &                      LINDX(*)      , LINK(*)       ,
     &                      RELIND(*)     , SNODE(*)      ,
     &                      SPLIT(*)      , XSUPER(*)
        INTEGER             IFLAG , NSUPER, TMPSIZ
        DOUBLE PRECISION    LNZ(*)        , TEMP(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
C
        INTEGER             FJCOL , FKCOL , I     , ILEN  , ILPNT ,
     &                      INDDIF, JLEN  , JLPNT , JSUP  , JXPNT ,
     &                      KFIRST, KLAST , KLEN  , KLPNT , KSUP  ,
     &                      KXPNT , LJCOL , NCOLUP, NJCOLS, NKCOLS,
     &                      NXKSUP, NXTCOL, NXTSUP, STORE
C
C*********************************************************************
C
        IFLAG = 0
C
C       -----------------------------------------------------------
C       INITIALIZE EMPTY ROW LISTS IN LINK(*) AND ZERO OUT TEMP(*).
C       -----------------------------------------------------------
        DO  100  JSUP = 1, NSUPER
            LINK(JSUP) = 0
  100   CONTINUE
        DO  200  I = 1, TMPSIZ
            TEMP(I) = 0.0D+00
  200   CONTINUE
C
C       ---------------------------
C       FOR EACH SUPERNODE JSUP ...
C       ---------------------------
        DO  600  JSUP = 1, NSUPER
C
C           ------------------------------------------------
C           FJCOL  ...  FIRST COLUMN OF SUPERNODE JSUP.
C           LJCOL  ...  LAST COLUMN OF SUPERNODE JSUP.
C           NJCOLS ...  NUMBER OF COLUMNS IN SUPERNODE JSUP.
C           JLEN   ...  LENGTH OF COLUMN FJCOL.
C           JXPNT  ...  POINTER TO INDEX OF FIRST
C                       NONZERO IN COLUMN FJCOL.
C           ------------------------------------------------
            FJCOL  = XSUPER(JSUP)
            NJCOLS = XSUPER(JSUP+1) - FJCOL
            LJCOL  = FJCOL + NJCOLS - 1
            JLEN   = XLNZ(FJCOL+1) - XLNZ(FJCOL)
            JXPNT  = XLINDX(JSUP)
C
C           -----------------------------------------------------
C           SET UP INDMAP(*) TO MAP THE ENTRIES IN UPDATE COLUMNS
C           TO THEIR CORRESPONDING POSITIONS IN UPDATED COLUMNS, 
C           RELATIVE THE BOTTOM OF EACH UPDATED COLUMN.
C           -----------------------------------------------------
            CALL  LDINDX ( JLEN, LINDX(JXPNT), INDMAP )
C
C           -----------------------------------------
C           FOR EVERY SUPERNODE KSUP IN ROW(JSUP) ...
C           -----------------------------------------
            KSUP = LINK(JSUP)
  300       IF  ( KSUP .GT. 0 )  THEN
                NXKSUP = LINK(KSUP)
C
C               -------------------------------------------------------
C               GET INFO ABOUT THE CMOD(JSUP,KSUP) UPDATE.
C
C               FKCOL  ...  FIRST COLUMN OF SUPERNODE KSUP.
C               NKCOLS ...  NUMBER OF COLUMNS IN SUPERNODE KSUP.
C               KLEN   ...  LENGTH OF ACTIVE PORTION OF COLUMN FKCOL.
C               KXPNT  ...  POINTER TO INDEX OF FIRST NONZERO IN ACTIVE
C                           PORTION OF COLUMN FJCOL.
C               -------------------------------------------------------
                FKCOL = XSUPER(KSUP)
                NKCOLS = XSUPER(KSUP+1) - FKCOL
                KLEN = LENGTH(KSUP)
                KXPNT = XLINDX(KSUP+1) - KLEN
C
C               -------------------------------------------
C               PERFORM CMOD(JSUP,KSUP), WITH SPECIAL CASES
C               HANDLED DIFFERENTLY.
C               -------------------------------------------
C
                IF  ( KLEN .NE. JLEN )  THEN
C
C                   -------------------------------------------
C                   SPARSE CMOD(JSUP,KSUP).
C
C                   NCOLUP ... NUMBER OF COLUMNS TO BE UPDATED.
C                   -------------------------------------------
C
                    DO  400  I = 0, KLEN-1
                        NXTCOL = LINDX(KXPNT+I)
                        IF  ( NXTCOL .GT. LJCOL )  GO TO 500
  400               CONTINUE
                    I = KLEN
  500               CONTINUE
                    NCOLUP = I
C
                    IF  ( NKCOLS .EQ. 1 )  THEN
C
C                       ----------------------------------------------
C                       UPDATING TARGET SUPERNODE BY TRIVIAL
C                       SUPERNODE (WITH ONE COLUMN).
C
C                       KLPNT  ...  POINTER TO FIRST NONZERO IN ACTIVE
C                                   PORTION OF COLUMN FKCOL.
C                       ----------------------------------------------
                        KLPNT = XLNZ(FKCOL+1) - KLEN
                        CALL  MMPYI ( KLEN, NCOLUP, LINDX(KXPNT),
     &                                LNZ(KLPNT), XLNZ, LNZ, INDMAP )
C
                    ELSE
C
C                       --------------------------------------------
C                       KFIRST ...  FIRST INDEX OF ACTIVE PORTION OF
C                                   SUPERNODE KSUP (FIRST COLUMN TO
C                                   BE UPDATED).
C                       KLAST  ...  LAST INDEX OF ACTIVE PORTION OF
C                                   SUPERNODE KSUP.
C                       --------------------------------------------
C
                        KFIRST = LINDX(KXPNT)
                        KLAST  = LINDX(KXPNT+KLEN-1)
                        INDDIF = INDMAP(KFIRST) - INDMAP(KLAST)
C
                        IF  ( INDDIF .LT. KLEN )  THEN
C
C                           ---------------------------------------
C                           DENSE CMOD(JSUP,KSUP).
C
C                           ILPNT  ...  POINTER TO FIRST NONZERO IN
C                                       COLUMN KFIRST.
C                           ILEN   ...  LENGTH OF COLUMN KFIRST.
C                           ---------------------------------------
                            ILPNT = XLNZ(KFIRST)
                            ILEN = XLNZ(KFIRST+1) - ILPNT
                            CALL  MMPY ( KLEN, NKCOLS, NCOLUP,
     &                                   SPLIT(FKCOL), XLNZ(FKCOL),
     &                                   LNZ, LNZ(ILPNT), ILEN, MMPYN  )
C
                        ELSE
C
C                           -------------------------------
C                           GENERAL SPARSE CMOD(JSUP,KSUP).
C                           COMPUTE CMOD(JSUP,KSUP) UPDATE
C                           IN WORK STORAGE.
C                           -------------------------------
                            STORE = KLEN * NCOLUP - NCOLUP * 
     &                              (NCOLUP-1) / 2
                            IF  ( STORE .GT. TMPSIZ )  THEN
                                IFLAG = -2
                                RETURN
                            ENDIF
                            CALL  MMPY ( KLEN, NKCOLS, NCOLUP,
     &                                   SPLIT(FKCOL), XLNZ(FKCOL),
     &                                   LNZ, TEMP, KLEN, MMPYN  )
C                           ----------------------------------------
C                           GATHER INDICES OF KSUP RELATIVE TO JSUP.
C                           ----------------------------------------
                            CALL  IGATHR ( KLEN, LINDX(KXPNT),
     &                                     INDMAP, RELIND )
C                           --------------------------------------
C                           INCORPORATE THE CMOD(JSUP,KSUP) BLOCK
C                           UPDATE INTO THE TO APPROPRIATE COLUMNS
C                           OF L.
C                           --------------------------------------
                            CALL  ASSMB ( KLEN, NCOLUP, TEMP, RELIND,
     &                                    XLNZ(FJCOL), LNZ, JLEN )
C
                        ENDIF
C
                    ENDIF
C
                ELSE
C
C                   ----------------------------------------------
C                   DENSE CMOD(JSUP,KSUP).
C                   JSUP AND KSUP HAVE IDENTICAL STRUCTURE.
C
C                   JLPNT  ...  POINTER TO FIRST NONZERO IN COLUMN
C                               FJCOL.
C                   ----------------------------------------------
                    JLPNT = XLNZ(FJCOL)
                    CALL  MMPY ( KLEN, NKCOLS, NJCOLS, SPLIT(FKCOL),
     &                           XLNZ(FKCOL), LNZ, LNZ(JLPNT), JLEN,
     &                           MMPYN )
                    NCOLUP = NJCOLS
                    IF  ( KLEN .GT. NJCOLS )  THEN
                        NXTCOL = LINDX(JXPNT+NJCOLS)
                    ENDIF
C
                ENDIF
C
C               ------------------------------------------------
C               LINK KSUP INTO LINKED LIST OF THE NEXT SUPERNODE
C               IT WILL UPDATE AND DECREMENT KSUP'S ACTIVE
C               LENGTH.
C               ------------------------------------------------
                IF  ( KLEN .GT. NCOLUP )  THEN
                    NXTSUP = SNODE(NXTCOL)
                    LINK(KSUP) = LINK(NXTSUP)
                    LINK(NXTSUP) = KSUP
                    LENGTH(KSUP) = KLEN - NCOLUP
                ELSE
                    LENGTH(KSUP) = 0
                ENDIF
C
C               -------------------------------
C               NEXT UPDATING SUPERNODE (KSUP).
C               -------------------------------
                KSUP = NXKSUP
                GO TO 300
C
            ENDIF
C
C           ----------------------------------------------
C           APPLY PARTIAL CHOLESKY TO THE COLUMNS OF JSUP.
C           ----------------------------------------------
            CALL CHLSUP ( JLEN, NJCOLS, SPLIT(FJCOL), XLNZ(FJCOL), LNZ,
     &                    IFLAG, MMPYN, SMXPY )
            IF  ( IFLAG .NE. 0 )  THEN
                IFLAG = -1
                RETURN
            ENDIF
C
C           -----------------------------------------------
C           INSERT JSUP INTO LINKED LIST OF FIRST SUPERNODE
C           IT WILL UPDATE.
C           -----------------------------------------------
            IF  ( JLEN .GT. NJCOLS )  THEN
                NXTCOL = LINDX(JXPNT+NJCOLS)
                NXTSUP = SNODE(NXTCOL)
                LINK(JSUP) = LINK(NXTSUP)
                LINK(NXTSUP) = JSUP
                LENGTH(JSUP) = JLEN - NJCOLS
            ELSE
                LENGTH(JSUP) = 0
            ENDIF
C
  600   CONTINUE
C
        RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******         LDINDX .... LOAD INDEX VECTOR             **************
C***********************************************************************
C***********************************************************************
C
C     PURPOSE - THIS ROUTINE COMPUTES THE SECOND INDEX VECTOR
C               USED TO IMPLEMENT THE DOUBLY-INDIRECT SAXPY-LIKE
C               LOOPS THAT ALLOW US TO ACCUMULATE UPDATE 
C               COLUMNS DIRECTLY INTO FACTOR STORAGE.
C
C     INPUT PARAMETERS -
C        JLEN   - LENGTH OF THE FIRST COLUMN OF THE SUPERNODE,
C                 INCLUDING THE DIAGONAL ENTRY.
C        LINDX  - THE OFF-DIAGONAL ROW INDICES OF THE SUPERNODE, 
C                 I.E., THE ROW INDICES OF THE NONZERO ENTRIES
C                 LYING BELOW THE DIAGONAL ENTRY OF THE FIRST
C                 COLUMN OF THE SUPERNODE.
C
C     OUTPUT PARAMETERS - 
C        INDMAP - THIS INDEX VECTOR MAPS EVERY GLOBAL ROW INDEX
C                 OF NONZERO ENTRIES IN THE FIRST COLUMN OF THE 
C                 SUPERNODE TO ITS POSITION IN THE INDEX LIST 
C                 RELATIVE TO THE LAST INDEX IN THE LIST.  MORE
C                 PRECISELY, IT GIVES THE DISTANCE OF EACH INDEX
C                 FROM THE LAST INDEX IN THE LIST.
C
C***********************************************************************
C
      SUBROUTINE  LDINDX ( JLEN, LINDX, INDMAP )
C
C***********************************************************************
C
C     -----------
C     PARAMETERS.
C     -----------
      INTEGER             JLEN
      INTEGER             LINDX(*), INDMAP(*)
C
C     ----------------
C     LOCAL VARIABLES.
C     ----------------
      INTEGER             CURLEN, J     , JSUB
C
C***********************************************************************
C
      CURLEN = JLEN
      DO  200  J = 1, JLEN
          JSUB = LINDX(J)
          CURLEN = CURLEN - 1
          INDMAP(JSUB) = CURLEN
  200 CONTINUE
      RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C*************     MMPYI  .... MATRIX-MATRIX MULTIPLY     **************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE -
C       THIS ROUTINE PERFORMS A MATRIX-MATRIX MULTIPLY, Y = Y + XA,
C       ASSUMING DATA STRUCTURES USED IN SOME OF OUR SPARSE CHOLESKY
C       CODES.
C
C       MATRIX X HAS ONLY 1 COLUMN.
C
C   INPUT PARAMETERS -
C       M               -   NUMBER OF ROWS IN X AND IN Y.
C       Q               -   NUMBER OF COLUMNS IN A AND Y.
C       XPNT(*)         -   XPNT(J+1) POINTS ONE LOCATION BEYOND THE
C                           END OF THE J-TH COLUMN OF X.  XPNT IS ALSO
C                           USED TO ACCESS THE ROWS OF A.
C       X(*)            -   CONTAINS THE COLUMNS OF X AND THE ROWS OF A.
C       IY(*)           -   IY(COL) POINTS TO THE BEGINNING OF COLUMN
C       RELIND(*)       -   RELATIVE INDICES.
C
C   UPDATED PARAMETERS -
C       Y(*)            -   ON OUTPUT, Y = Y + AX.
C
C***********************************************************************
C
      SUBROUTINE  MMPYI  (  M     , Q     , XPNT  , X     , IY    ,
     &                      Y     , RELIND                          )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        INTEGER             M     , Q
        INTEGER             IY(*)         , RELIND(*)     ,
     &                      XPNT(*)
        DOUBLE PRECISION    X(*)      , Y(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
C
        INTEGER             COL   , I     , ISUB  , K     , YLAST
        DOUBLE PRECISION    A
C
C***********************************************************************
C
        DO  200  K = 1, Q
            COL = XPNT(K)
            YLAST = IY(COL+1) - 1
            A = - X(K)
CDIR$   IVDEP
            DO  100  I = K, M
                ISUB = XPNT(I)
                ISUB = YLAST - RELIND(ISUB)
                Y(ISUB) = Y(ISUB) + A*X(I)
  100       CONTINUE
  200   CONTINUE
        RETURN
C
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C**************     MMPY  .... MATRIX-MATRIX MULTIPLY     **************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE -
C       THIS ROUTINE PERFORMS A MATRIX-MATRIX MULTIPLY, Y = Y + XA,
C       ASSUMING DATA STRUCTURES USED IN SOME OF OUR SPARSE CHOLESKY
C       CODES.
C
C   INPUT PARAMETERS -
C       M               -   NUMBER OF ROWS IN X AND IN Y.
C       N               -   NUMBER OF COLUMNS IN X AND NUMBER OF ROWS
C                           IN A.
C       Q               -   NUMBER OF COLUMNS IN A AND Y.
C       SPLIT(*)        -   BLOCK PARTITIONING OF X.
C       XPNT(*)         -   XPNT(J+1) POINTS ONE LOCATION BEYOND THE
C                           END OF THE J-TH COLUMN OF X.  XPNT IS ALSO
C                           USED TO ACCESS THE ROWS OF A.
C       X(*)            -   CONTAINS THE COLUMNS OF X AND THE ROWS OF A.
C       LDY             -   LENGTH OF FIRST COLUMN OF Y.
C       MMPYN           -   EXTERNAL ROUTINE: MATRIX-MATRIX MULTIPLY,
C                           WITH LEVEL N LOOP UNROLLING.
C
C   UPDATED PARAMETERS -
C       Y(*)            -   ON OUTPUT, Y = Y + AX.
C
C***********************************************************************
C
      SUBROUTINE  MMPY   (  M     , N     , Q     , SPLIT , XPNT  ,
     &                      X     , Y     , LDY   , MMPYN           )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        EXTERNAL            MMPYN
        INTEGER             LDY   , M     , N     , Q
        INTEGER             SPLIT(*)      , XPNT(*)
        DOUBLE PRECISION    X(*)          , Y(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
C
        INTEGER             BLK   , FSTCOL, NN
C
C***********************************************************************
C
        BLK = 1
        FSTCOL = 1
  100   CONTINUE
        IF  ( FSTCOL .LE. N )  THEN
            NN = SPLIT(BLK)
            CALL  MMPYN ( M, NN, Q, XPNT(FSTCOL), X, Y, LDY )
            FSTCOL = FSTCOL + NN
            BLK = BLK + 1
            GO TO 100
        ENDIF
        RETURN
C
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C*************     MMPY1  .... MATRIX-MATRIX MULTIPLY     **************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE -
C       THIS ROUTINE PERFORMS A MATRIX-MATRIX MULTIPLY, Y = Y + XA,
C       ASSUMING DATA STRUCTURES USED IN SOME OF OUR SPARSE CHOLESKY
C       CODES.
C
C       LOOP UNROLLING: LEVEL 1
C
C   INPUT PARAMETERS -
C       M               -   NUMBER OF ROWS IN X AND IN Y.
C       N               -   NUMBER OF COLUMNS IN X AND NUMBER OF ROWS
C                           IN A.
C       Q               -   NUMBER OF COLUMNS IN A AND Y.
C       XPNT(*)         -   XPNT(J+1) POINTS ONE LOCATION BEYOND THE
C                           END OF THE J-TH COLUMN OF X.  XPNT IS ALSO
C                           USED TO ACCESS THE ROWS OF A.
C       X(*)            -   CONTAINS THE COLUMNS OF X AND THE ROWS OF A.
C       LDY             -   LENGTH OF FIRST COLUMN OF Y.
C
C   UPDATED PARAMETERS -
C       Y(*)            -   ON OUTPUT, Y = Y + AX.
C
C***********************************************************************
C
      SUBROUTINE  MMPY1  (  M     , N     , Q     , XPNT  , X     ,
     &                      Y     , LDY                             )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        INTEGER             LDY   , M     , N     , Q
        INTEGER             XPNT(*)
        DOUBLE PRECISION    X(*)          , Y(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
C
        INTEGER             I1
        INTEGER             IY    , IYLAST, IYSTRT, IYSTOP, LENY  ,
     &                      MM    , XCOL  , YCOL
        DOUBLE PRECISION    A1
C
C***********************************************************************
C
        MM = M
        IYLAST = 0
        LENY = LDY
C       ------------------------------------
C       TO COMPUTE EACH COLUMN YCOL OF Y ...
C       ------------------------------------
        DO  300  YCOL = 1, Q
            IYSTRT = IYLAST + 1
            IYSTOP = IYSTRT + MM - 1
            IYLAST = IYLAST + LENY
C           --------------------------------------------------
C           ... PERFORM THE APPROPRATE MATRIX VECTOR MULTIPLY:
C               X * A(*,YCOL).
C           --------------------------------------------------
            DO  200  XCOL = 1, N
                I1 = XPNT(XCOL+1) - MM
                A1  = - X(I1)
                DO  100  IY = IYSTRT, IYSTOP
                    Y(IY) = Y(IY) + A1 * X(I1)
                    I1 = I1 + 1
  100           CONTINUE
  200       CONTINUE
            MM = MM - 1
            LENY = LENY - 1
  300   CONTINUE
C
        RETURN
        END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C*************     MMPY2  .... MATRIX-MATRIX MULTIPLY     **************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE -
C       THIS ROUTINE PERFORMS A MATRIX-MATRIX MULTIPLY, Y = Y + XA,
C       ASSUMING DATA STRUCTURES USED IN SOME OF OUR SPARSE CHOLESKY
C       CODES.
C
C       LOOP UNROLLING: LEVEL 2
C
C   INPUT PARAMETERS -
C       M               -   NUMBER OF ROWS IN X AND IN Y.
C       N               -   NUMBER OF COLUMNS IN X AND NUMBER OF ROWS
C                           IN A.
C       Q               -   NUMBER OF COLUMNS IN A AND Y.
C       XPNT(*)         -   XPNT(J+1) POINTS ONE LOCATION BEYOND THE
C                           END OF THE J-TH COLUMN OF X.  XPNT IS ALSO
C                           USED TO ACCESS THE ROWS OF A.
C       X(*)            -   CONTAINS THE COLUMNS OF X AND THE ROWS OF A.
C       LDY             -   LENGTH OF FIRST COLUMN OF Y.
C
C   UPDATED PARAMETERS -
C       Y(*)            -   ON OUTPUT, Y = Y + AX.
C
C***********************************************************************
C
      SUBROUTINE  MMPY2  (  M     , N     , Q     , XPNT  , X     ,
     &                      Y     , LDY                             )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        INTEGER             LDY   , M     , N     , Q
        INTEGER             XPNT(*)
        DOUBLE PRECISION    X(*)          , Y(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
C
        INTEGER             I1    , I2
        INTEGER             IY    , IYLAST, IYSTRT, IYSTOP, LENY  ,
     &                      MM    , REMAIN, XCOL  , YCOL
        DOUBLE PRECISION    A1    , A2
C
        INTEGER             LEVEL
        PARAMETER       (   LEVEL = 2 )
C
C***********************************************************************
C
C       -----------------------------------------------------------
C       INITIAL OFFSETS, COLUMN LENGTHS, AND INDEX RANGE VARIABLES.
C       -----------------------------------------------------------
        REMAIN = MOD ( N, LEVEL ) + 1
        MM = M
        IYLAST = 0
        LENY = LDY
C
C       ------------------------------------
C       TO COMPUTE EACH COLUMN YCOL OF Y ...
C       ------------------------------------
C
        DO  500  YCOL = 1, Q
C
            IYSTRT = IYLAST + 1
            IYSTOP = IYSTRT + MM - 1
            IYLAST = IYLAST + LENY
C
C           --------------------------------------------------
C           ... PERFORM THE APPROPRATE MATRIX VECTOR MULTIPLY:
C               X * A(*,YCOL) WITH LEVEL 2 LOOP-UNROLLING.
C           --------------------------------------------------
            GO TO ( 200, 100 ), REMAIN
C
  100           CONTINUE
                I1 = XPNT(1+1) - MM
                A1  = - X(I1)
                DO  150  IY = IYSTRT, IYSTOP
                    Y(IY) = Y(IY) + A1*X(I1)
                    I1 = I1 + 1
  150           CONTINUE
                GO TO 200
C
  200           CONTINUE
                DO  400  XCOL = REMAIN, N, LEVEL
                    I1 = XPNT(XCOL+1) - MM
                    I2 = XPNT(XCOL+2) - MM
                    A1  = - X(I1)
                    A2  = - X(I2)
                    DO  300  IY = IYSTRT, IYSTOP
                        Y(IY) = ( (Y(IY))
     &                          + A1*X(I1)) + A2*X(I2)
                        I1 = I1 + 1
                        I2 = I2 + 1
  300               CONTINUE
  400           CONTINUE
C
            MM = MM - 1
            LENY = LENY - 1
C
  500   CONTINUE
C
        RETURN
        END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C*************     MMPY4  .... MATRIX-MATRIX MULTIPLY     **************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE -
C       THIS ROUTINE PERFORMS A MATRIX-MATRIX MULTIPLY, Y = Y + XA,
C       ASSUMING DATA STRUCTURES USED IN SOME OF OUR SPARSE CHOLESKY
C       CODES.
C
C       LOOP UNROLLING: LEVEL 4
C
C   INPUT PARAMETERS -
C       M               -   NUMBER OF ROWS IN X AND IN Y.
C       N               -   NUMBER OF COLUMNS IN X AND NUMBER OF ROWS
C                           IN A.
C       Q               -   NUMBER OF COLUMNS IN A AND Y.
C       XPNT(*)         -   XPNT(J+1) POINTS ONE LOCATION BEYOND THE
C                           END OF THE J-TH COLUMN OF X.  XPNT IS ALSO
C                           USED TO ACCESS THE ROWS OF A.
C       X(*)            -   CONTAINS THE COLUMNS OF X AND THE ROWS OF A.
C       LDY             -   LENGTH OF FIRST COLUMN OF Y.
C
C   UPDATED PARAMETERS -
C       Y(*)            -   ON OUTPUT, Y = Y + AX.
C
C***********************************************************************
C
      SUBROUTINE  MMPY4  (  M     , N     , Q     , XPNT  , X     ,
     &                      Y     , LDY                             )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        INTEGER             LDY   , M     , N     , Q
        INTEGER             XPNT(*)
        DOUBLE PRECISION    X(*)          , Y(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
C
        INTEGER             I1    , I2    , I3    , I4
        INTEGER             IY    , IYLAST, IYSTRT, IYSTOP, LENY  ,
     &                      MM    , REMAIN, XCOL  , YCOL
        DOUBLE PRECISION    A1    , A2    , A3    , A4
C
        INTEGER             LEVEL
        PARAMETER       (   LEVEL = 4 )
C
C***********************************************************************
C
C       -----------------------------------------------------------
C       INITIAL OFFSETS, COLUMN LENGTHS, AND INDEX RANGE VARIABLES.
C       -----------------------------------------------------------
        REMAIN = MOD ( N, LEVEL ) + 1
        MM = M
        IYLAST = 0
        LENY = LDY
C
C       ------------------------------------
C       TO COMPUTE EACH COLUMN YCOL OF Y ...
C       ------------------------------------
C
        DO  700  YCOL = 1, Q
C
            IYSTRT = IYLAST + 1
            IYSTOP = IYSTRT + MM - 1
            IYLAST = IYLAST + LENY
C
C           --------------------------------------------------
C           ... PERFORM THE APPROPRATE MATRIX VECTOR MULTIPLY:
C               X * A(*,YCOL) WITH LEVEL 4 LOOP-UNROLLING.
C           --------------------------------------------------
C
            GO TO ( 400, 100, 200, 300 ), REMAIN
C
  100           CONTINUE
                I1 = XPNT(1+1) - MM
                A1 = - X(I1)
                DO  150  IY = IYSTRT, IYSTOP
                    Y(IY) = Y(IY) + A1*X(I1)
                    I1 = I1 + 1
  150           CONTINUE
                GO TO 400
C
  200           CONTINUE
                I1 = XPNT(1+1) - MM
                I2 = XPNT(1+2) - MM
                A1 = - X(I1)
                A2 = - X(I2)
                DO  250  IY = IYSTRT, IYSTOP
                    Y(IY) = ( (Y(IY))
     &                      + A1*X(I1)) + A2*X(I2)
                    I1 = I1 + 1
                    I2 = I2 + 1
  250           CONTINUE
                GO TO 400
C
  300           CONTINUE
                I1 = XPNT(1+1) - MM
                I2 = XPNT(1+2) - MM
                I3 = XPNT(1+3) - MM
                A1 = - X(I1)
                A2 = - X(I2)
                A3 = - X(I3)
                DO  350  IY = IYSTRT, IYSTOP
                    Y(IY) = (( (Y(IY))
     &                      + A1*X(I1)) + A2*X(I2))
     &                      + A3*X(I3)
                    I1 = I1 + 1
                    I2 = I2 + 1
                    I3 = I3 + 1
  350           CONTINUE
                GO TO 400
C
  400           CONTINUE
                DO  600  XCOL = REMAIN, N, LEVEL
                    I1 = XPNT(XCOL+1) - MM
                    I2 = XPNT(XCOL+2) - MM
                    I3 = XPNT(XCOL+3) - MM
                    I4 = XPNT(XCOL+4) - MM
                    A1 = - X(I1)
                    A2 = - X(I2)
                    A3 = - X(I3)
                    A4 = - X(I4)
                    DO  500  IY = IYSTRT, IYSTOP
                        Y(IY) = ((( (Y(IY))
     &                          + A1*X(I1)) + A2*X(I2))
     &                          + A3*X(I3)) + A4*X(I4)
                        I1 = I1 + 1
                        I2 = I2 + 1
                        I3 = I3 + 1
                        I4 = I4 + 1
  500               CONTINUE
  600           CONTINUE
C
            MM = MM - 1
            LENY = LENY - 1
C
  700   CONTINUE
C
        RETURN
        END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C*************     MMPY8  .... MATRIX-MATRIX MULTIPLY     **************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE -
C       THIS ROUTINE PERFORMS A MATRIX-MATRIX MULTIPLY, Y = Y + XA,
C       ASSUMING DATA STRUCTURES USED IN SOME OF OUR SPARSE CHOLESKY
C       CODES.
C
C       LOOP UNROLLING: LEVEL 8
C
C   INPUT PARAMETERS -
C       M               -   NUMBER OF ROWS IN X AND IN Y.
C       N               -   NUMBER OF COLUMNS IN X AND NUMBER OF ROWS
C                           IN A.
C       Q               -   NUMBER OF COLUMNS IN A AND Y.
C       XPNT(*)         -   XPNT(J+1) POINTS ONE LOCATION BEYOND THE
C                           END OF THE J-TH COLUMN OF X.  XPNT IS ALSO
C                           USED TO ACCESS THE ROWS OF A.
C       X(*)            -   CONTAINS THE COLUMNS OF X AND THE ROWS OF A.
C       LDY             -   LENGTH OF FIRST COLUMN OF Y.
C
C   UPDATED PARAMETERS -
C       Y(*)            -   ON OUTPUT, Y = Y + AX.
C
C***********************************************************************
C
      SUBROUTINE  MMPY8  (  M     , N     , Q     , XPNT  , X     ,
     &                      Y     , LDY                             )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        INTEGER             LDY   , M     , N     , Q
        INTEGER             XPNT(*)
        DOUBLE PRECISION    X(*)          , Y(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
C
        INTEGER             I1    , I2    , I3    , I4    , I5    ,
     &                      I6    , I7    , I8
        INTEGER             IY    , IYLAST, IYSTRT, IYSTOP, LENY  ,
     &                      MM    , REMAIN, XCOL  , YCOL
        DOUBLE PRECISION    A1    , A2    , A3    , A4    , A5    ,
     &                      A6    , A7    , A8
C
        INTEGER             LEVEL
        PARAMETER       (   LEVEL = 8 )
C
C***********************************************************************
C
C       -----------------------------------------------------------
C       INITIAL OFFSETS, COLUMN LENGTHS, AND INDEX RANGE VARIABLES.
C       -----------------------------------------------------------
        REMAIN = MOD ( N, LEVEL ) + 1
        MM = M
        IYLAST = 0
        LENY = LDY
C
C       ------------------------------------
C       TO COMPUTE EACH COLUMN YCOL OF Y ...
C       ------------------------------------
C
        DO  1100  YCOL = 1, Q
C
            IYSTRT = IYLAST + 1
            IYSTOP = IYSTRT + MM - 1
            IYLAST = IYLAST + LENY
C
C           --------------------------------------------------
C           ... PERFORM THE APPROPRATE MATRIX VECTOR MULTIPLY:
C               X * A(*,YCOL) WITH LEVEL 8 LOOP-UNROLLING.
C           --------------------------------------------------
C
            GO TO ( 800, 100, 200, 300, 400, 500, 600, 700 ), REMAIN
C
  100           CONTINUE
                I1 = XPNT(1+1) - MM
                A1  = - X(I1)
                DO  150  IY = IYSTRT, IYSTOP
                    Y(IY) = Y(IY) + A1*X(I1)
                    I1 = I1 + 1
  150           CONTINUE
                GO TO 800
C
  200           CONTINUE
                I1 = XPNT(1+1) - MM
                I2 = XPNT(1+2) - MM
                A1  = - X(I1)
                A2  = - X(I2)
                DO  250  IY = IYSTRT, IYSTOP
                    Y(IY) = ( (Y(IY))
     &                      + A1*X(I1)) + A2*X(I2)
                    I1 = I1 + 1
                    I2 = I2 + 1
  250           CONTINUE
                GO TO 800
C
  300           CONTINUE
                I1 = XPNT(1+1) - MM
                I2 = XPNT(1+2) - MM
                I3 = XPNT(1+3) - MM
                A1  = - X(I1)
                A2  = - X(I2)
                A3  = - X(I3)
                DO  350  IY = IYSTRT, IYSTOP
                    Y(IY) = (( (Y(IY))
     &                      + A1*X(I1)) + A2*X(I2))
     &                      + A3*X(I3)
                    I1 = I1 + 1
                    I2 = I2 + 1
                    I3 = I3 + 1
  350           CONTINUE
                GO TO 800
C
  400           CONTINUE
                I1 = XPNT(1+1) - MM
                I2 = XPNT(1+2) - MM
                I3 = XPNT(1+3) - MM
                I4 = XPNT(1+4) - MM
                A1  = - X(I1)
                A2  = - X(I2)
                A3  = - X(I3)
                A4  = - X(I4)
                DO  450  IY = IYSTRT, IYSTOP
                    Y(IY) = ((( (Y(IY))
     &                      + A1*X(I1)) + A2*X(I2))
     &                      + A3*X(I3)) + A4*X(I4)
                    I1 = I1 + 1
                    I2 = I2 + 1
                    I3 = I3 + 1
                    I4 = I4 + 1
  450           CONTINUE
                GO TO 800
C
  500           CONTINUE
                I1 = XPNT(1+1) - MM
                I2 = XPNT(1+2) - MM
                I3 = XPNT(1+3) - MM
                I4 = XPNT(1+4) - MM
                I5 = XPNT(1+5) - MM
                A1  = - X(I1)
                A2  = - X(I2)
                A3  = - X(I3)
                A4  = - X(I4)
                A5  = - X(I5)
                DO  550  IY = IYSTRT, IYSTOP
                    Y(IY) = (((( (Y(IY))
     &                      + A1*X(I1)) + A2*X(I2))
     &                      + A3*X(I3)) + A4*X(I4))
     &                      + A5*X(I5)
                    I1 = I1 + 1
                    I2 = I2 + 1
                    I3 = I3 + 1
                    I4 = I4 + 1
                    I5 = I5 + 1
  550           CONTINUE
                GO TO 800
C
  600           CONTINUE
                I1 = XPNT(1+1) - MM
                I2 = XPNT(1+2) - MM
                I3 = XPNT(1+3) - MM
                I4 = XPNT(1+4) - MM
                I5 = XPNT(1+5) - MM
                I6 = XPNT(1+6) - MM
                A1  = - X(I1)
                A2  = - X(I2)
                A3  = - X(I3)
                A4  = - X(I4)
                A5  = - X(I5)
                A6  = - X(I6)
                DO  650  IY = IYSTRT, IYSTOP
                    Y(IY) = ((((( (Y(IY))
     &                      + A1*X(I1)) + A2*X(I2))
     &                      + A3*X(I3)) + A4*X(I4))
     &                      + A5*X(I5)) + A6*X(I6)
                    I1 = I1 + 1
                    I2 = I2 + 1
                    I3 = I3 + 1
                    I4 = I4 + 1
                    I5 = I5 + 1
                    I6 = I6 + 1
  650           CONTINUE
                GO TO 800
C
  700           CONTINUE
                I1 = XPNT(1+1) - MM
                I2 = XPNT(1+2) - MM
                I3 = XPNT(1+3) - MM
                I4 = XPNT(1+4) - MM
                I5 = XPNT(1+5) - MM
                I6 = XPNT(1+6) - MM
                I7 = XPNT(1+7) - MM
                A1  = - X(I1)
                A2  = - X(I2)
                A3  = - X(I3)
                A4  = - X(I4)
                A5  = - X(I5)
                A6  = - X(I6)
                A7  = - X(I7)
                DO  750  IY = IYSTRT, IYSTOP
                    Y(IY) = (((((( (Y(IY))
     &                      + A1*X(I1)) + A2*X(I2))
     &                      + A3*X(I3)) + A4*X(I4))
     &                      + A5*X(I5)) + A6*X(I6))
     &                      + A7*X(I7)
                    I1 = I1 + 1
                    I2 = I2 + 1
                    I3 = I3 + 1
                    I4 = I4 + 1
                    I5 = I5 + 1
                    I6 = I6 + 1
                    I7 = I7 + 1
  750           CONTINUE
                GO TO 800
C
  800           CONTINUE
                DO  1000  XCOL = REMAIN, N, LEVEL
                    I1 = XPNT(XCOL+1) - MM
                    I2 = XPNT(XCOL+2) - MM
                    I3 = XPNT(XCOL+3) - MM
                    I4 = XPNT(XCOL+4) - MM
                    I5 = XPNT(XCOL+5) - MM
                    I6 = XPNT(XCOL+6) - MM
                    I7 = XPNT(XCOL+7) - MM
                    I8 = XPNT(XCOL+8) - MM
                    A1  = - X(I1)
                    A2  = - X(I2)
                    A3  = - X(I3)
                    A4  = - X(I4)
                    A5  = - X(I5)
                    A6  = - X(I6)
                    A7  = - X(I7)
                    A8  = - X(I8)
                    DO  900  IY = IYSTRT, IYSTOP
                        Y(IY) = ((((((( (Y(IY))
     &                          + A1*X(I1)) + A2*X(I2))
     &                          + A3*X(I3)) + A4*X(I4))
     &                          + A5*X(I5)) + A6*X(I6))
     &                          + A7*X(I7)) + A8*X(I8)
                        I1 = I1 + 1
                        I2 = I2 + 1
                        I3 = I3 + 1
                        I4 = I4 + 1
                        I5 = I5 + 1
                        I6 = I6 + 1
                        I7 = I7 + 1
                        I8 = I8 + 1
  900               CONTINUE
 1000           CONTINUE
C
            MM = MM - 1
            LENY = LENY - 1
C
 1100   CONTINUE
C
        RETURN
        END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******         IGATHR .... INTEGER GATHER OPERATION      **************
C***********************************************************************
C***********************************************************************
C
C     PURPOSE - THIS ROUTINE PERFORMS A STANDARD INTEGER GATHER
C               OPERATION.
C
C     INPUT PARAMETERS -
C        KLEN   - LENGTH OF THE LIST OF GLOBAL INDICES.
C        LINDX  - LIST OF GLOBAL INDICES.
C        INDMAP - INDEXED BY GLOBAL INDICES, IT CONTAINS THE
C                 REQUIRED RELATIVE INDICES.
C
C     OUTPUT PARAMETERS - 
C        RELIND - LIST RELATIVE INDICES.
C
C***********************************************************************
C
      SUBROUTINE  IGATHR ( KLEN  , LINDX, INDMAP, RELIND )
C
C***********************************************************************
C
C     -----------
C     PARAMETERS.
C     -----------
      INTEGER             KLEN  
      INTEGER             INDMAP(*), LINDX (*), RELIND(*)
C
C     ----------------
C     LOCAL VARIABLES.
C     ----------------
      INTEGER             I
C
C***********************************************************************
C
CDIR$ IVDEP
      DO  100  I = 1, KLEN  
          RELIND(I) = INDMAP(LINDX(I))
  100 CONTINUE
      RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C************     ASSMB .... INDEXED ASSEMBLY OPERATION     ************
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       THIS ROUTINE PERFORMS AN INDEXED ASSEMBLY (I.E., SCATTER-ADD)
C       OPERATION, ASSUMING DATA STRUCTURES USED IN SOME OF OUR SPARSE
C       CHOLESKY CODES.
C
C   INPUT PARAMETERS:
C       M               -   NUMBER OF ROWS IN Y.
C       Q               -   NUMBER OF COLUMNS IN Y.
C       Y               -   BLOCK UPDATE TO BE INCORPORATED INTO FACTOR
C                           STORAGE.
C       RELIND          -   RELATIVE INDICES FOR MAPPING THE UPDATES
C                           ONTO THE TARGET COLUMNS.
C       XLNZ            -   POINTERS TO THE START OF EACH COLUMN IN THE
C                           TARGET MATRIX.
C
C   OUTPUT PARAMETERS:
C       LNZ             -   CONTAINS COLUMNS MODIFIED BY THE UPDATE
C                           MATRIX.
C
C***********************************************************************
C
      SUBROUTINE  ASSMB  (  M     , Q     , Y     , RELIND, XLNZ  ,
     &                      LNZ   , LDA                             )
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        INTEGER             LDA   , M     , Q
        INTEGER             XLNZ(*)
        INTEGER             RELIND(*)
        DOUBLE PRECISION    LNZ(*)        , Y(*)
C
C       ----------------
C       LOCAL VARIABLES.
C       ----------------
C
        INTEGER             ICOL  , IL1   , IR    , IY1   , LBOT1 ,
     &                      YCOL  , YOFF1
C
C***********************************************************************
C
C
        YOFF1 = 0
        DO  200  ICOL = 1, Q
            YCOL = LDA - RELIND(ICOL)
            LBOT1 = XLNZ(YCOL+1) - 1
CDIR$ IVDEP
            DO  100  IR = ICOL, M
                IL1 = LBOT1 - RELIND(IR)
                IY1 = YOFF1 + IR
                LNZ(IL1) = LNZ(IL1) + Y(IY1)
                Y(IY1) = 0.0D0
  100       CONTINUE
            YOFF1 = IY1 - ICOL
  200   CONTINUE
C
      RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******     CHLSUP .... DENSE CHOLESKY WITHIN SUPERNODE   **************
C***********************************************************************
C***********************************************************************
C
C     PURPOSE - THIS ROUTINE PERFORMS CHOLESKY
C               FACTORIZATION ON THE COLUMNS OF A SUPERNODE
C               THAT HAVE RECEIVED ALL UPDATES FROM COLUMNS
C               EXTERNAL TO THE SUPERNODE.
C
C     INPUT PARAMETERS -
C        M      - NUMBER OF ROWS (LENGTH OF THE FIRST COLUMN).
C        N      - NUMBER OF COLUMNS IN THE SUPERNODE.
C        XPNT   - XPNT(J+1) POINTS ONE LOCATION BEYOND THE END
C                 OF THE J-TH COLUMN OF THE SUPERNODE.
C        X(*)   - CONTAINS THE COLUMNS OF OF THE SUPERNODE TO
C                 BE FACTORED.
C        SMXPY  - EXTERNAL ROUTINE: MATRIX-VECTOR MULTIPLY.
C
C     OUTPUT PARAMETERS -
C        X(*)   - ON OUTPUT, CONTAINS THE FACTORED COLUMNS OF
C                 THE SUPERNODE.
C        IFLAG  - UNCHANGED IF THERE IS NO ERROR.
C                 =1 IF NONPOSITIVE DIAGONAL ENTRY IS ENCOUNTERED.
C
C***********************************************************************
C
      SUBROUTINE  CHLSUP  ( M, N, SPLIT, XPNT, X, IFLAG, MMPYN, 
     &                      SMXPY )
C
C***********************************************************************
C
C     -----------
C     PARAMETERS.
C     -----------
C
      EXTERNAL            MMPYN, SMXPY
C
      INTEGER             M, N, IFLAG
C
      INTEGER             XPNT(*), SPLIT(*)
C
      DOUBLE PRECISION    X(*)
C
C     ----------------
C     LOCAL VARIABLES.
C     ----------------
C
      INTEGER             FSTCOL, JBLK  , JPNT  , MM    , NN    ,
     &                    NXTCOL, Q
C
C***********************************************************************
C
        JBLK = 0
        FSTCOL = 1
        MM = M
        JPNT = XPNT(FSTCOL)
C
C       ----------------------------------------
C       FOR EACH BLOCK JBLK IN THE SUPERNODE ...
C       ----------------------------------------
  100   CONTINUE
        IF  ( FSTCOL .LE. N )  THEN
            JBLK = JBLK + 1
            NN = SPLIT(JBLK)
C           ------------------------------------------
C           ... PERFORM PARTIAL CHOLESKY FACTORIZATION
C               ON THE BLOCK.
C           ------------------------------------------
            CALL PCHOL ( MM, NN, XPNT(FSTCOL), X, IFLAG, SMXPY )
            IF  ( IFLAG .EQ. 1 )  RETURN
C           ----------------------------------------------
C           ... APPLY THE COLUMNS IN JBLK TO ANY COLUMNS
C               OF THE SUPERNODE REMAINING TO BE COMPUTED.
C           ----------------------------------------------
            NXTCOL = FSTCOL + NN
            Q = N - NXTCOL + 1
            MM = MM - NN
            JPNT = XPNT(NXTCOL)
            IF  ( Q .GT. 0 )  THEN
                CALL  MMPYN ( MM, NN, Q, XPNT(FSTCOL), X, X(JPNT), MM )
            ENDIF
            FSTCOL = NXTCOL
            GO TO 100
        ENDIF
C
        RETURN
        END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******     PCHOL .... DENSE PARTIAL CHOLESKY             **************
C***********************************************************************
C***********************************************************************
C
C     PURPOSE - THIS ROUTINE PERFORMS CHOLESKY
C               FACTORIZATION ON THE COLUMNS OF A SUPERNODE
C               THAT HAVE RECEIVED ALL UPDATES FROM COLUMNS
C               EXTERNAL TO THE SUPERNODE.
C
C     INPUT PARAMETERS -
C        M      - NUMBER OF ROWS (LENGTH OF THE FIRST COLUMN).
C        N      - NUMBER OF COLUMNS IN THE SUPERNODE.
C        XPNT   - XPNT(J+1) POINTS ONE LOCATION BEYOND THE END
C                 OF THE J-TH COLUMN OF THE SUPERNODE.
C        X(*)   - CONTAINS THE COLUMNS OF OF THE SUPERNODE TO
C                 BE FACTORED.
C        SMXPY  - EXTERNAL ROUTINE: MATRIX-VECTOR MULTIPLY.
C
C     OUTPUT PARAMETERS -
C        X(*)   - ON OUTPUT, CONTAINS THE FACTORED COLUMNS OF
C                 THE SUPERNODE.
C        IFLAG  - UNCHANGED IF THERE IS NO ERROR.
C                 =1 IF NONPOSITIVE DIAGONAL ENTRY IS ENCOUNTERED.
C
C***********************************************************************
C
      SUBROUTINE  PCHOL  ( M, N, XPNT, X, IFLAG, SMXPY )
C
C***********************************************************************
C
C     -----------
C     PARAMETERS.
C     -----------
C
      EXTERNAL            SMXPY
C
      INTEGER             M, N, IFLAG
C
      INTEGER             XPNT(*)
C
      DOUBLE PRECISION    X(*)
C
C     ----------------
C     LOCAL VARIABLES.
C     ----------------
C
      INTEGER             JPNT  , JCOL  , MM
C
      DOUBLE PRECISION    DIAG
      DOUBLE PRECISION    MXDIAG
C
C***********************************************************************
C
C       ------------------------------------------
C       FOR EVERY COLUMN JCOL IN THE SUPERNODE ...
C       ------------------------------------------
        MM     = M
        JPNT = XPNT(1)
        MXDIAG = 1.D+0
        DO  100  JCOL = 1, N
C
C           ----------------------------------
C           UPDATE JCOL WITH PREVIOUS COLUMNS.
C           ----------------------------------
            IF  ( JCOL .GT. 1 )  THEN
                CALL SMXPY ( MM, JCOL-1, X(JPNT), XPNT, X )
            ENDIF
C
C           ---------------------------
C           COMPUTE THE DIAGONAL ENTRY.
C           ---------------------------
            DIAG = X(JPNT)
            MXDIAG = MAX ( MXDIAG ,  DIAG )
            IF ( DIAG .LE. MIN(1.0D-15*MXDIAG, 1.0D-10) ) THEN
                DIAG = 1.0D+128
            ENDIF
            DIAG = SQRT ( DIAG )
            X(JPNT) = DIAG
            DIAG = 1.0D+00 / DIAG
C
C           ----------------------------------------------------
C           SCALE COLUMN JCOL WITH RECIPROCAL OF DIAGONAL ENTRY.
C           ----------------------------------------------------
            MM = MM - 1
            JPNT = JPNT + 1
            CALL DSCAL ( MM, DIAG, X(JPNT),1)
            JPNT = JPNT + MM
C
  100   CONTINUE
C
      RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******     SMXPY1 .... MATRIX-VECTOR MULTIPLY            **************
C***********************************************************************
C***********************************************************************
C
C     PURPOSE - THIS ROUTINE PERFORMS A MATRIX-VECTOR MULTIPLY,
C               Y = Y + AX, ASSUMING DATA STRUCTURES USED IN
C               RECENTLY DEVELOPED SPARSE CHOLESKY CODES.  THE 
C               '1' SIGNIFIES NO LOOP UNROLLING, I.E., 
C               LOOP-UNROLLING TO LEVEL 1.
C
C     INPUT PARAMETERS -
C        M      - NUMBER OF ROWS.
C        N      - NUMBER OF COLUMNS.
C        Y      - M-VECTOR TO WHICH AX WILL BE ADDED.
C        APNT   - INDEX VECTOR FOR A.  XA(I) POINTS TO THE
C                 FIRST NONZERO IN COLUMN I OF A.
C        Y      - ON OUTPUT, CONTAINS Y = Y + AX.
C
C***********************************************************************
C
      SUBROUTINE  SMXPY1 ( M, N, Y, APNT, A )
C
C***********************************************************************
C
C     -----------
C     PARAMETERS.
C     -----------
C
      INTEGER             M, N
C
      INTEGER             APNT(N)
C
      DOUBLE PRECISION    Y(M), A(*)
C
C     ----------------
C     LOCAL VARIABLES.
C     ----------------
C
      INTEGER             I, II, J
C
      DOUBLE PRECISION    AMULT
C
C***********************************************************************
C
      DO  200  J = 1, N
          II = APNT(J+1) - M
          AMULT  = - A(II)
          DO  100  I = 1, M
              Y(I) = Y(I) + AMULT * A(II)
              II = II + 1
  100     CONTINUE
  200 CONTINUE
      RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******     SMXPY2 .... MATRIX-VECTOR MULTIPLY            **************
C***********************************************************************
C***********************************************************************
C
C     PURPOSE - THIS ROUTINE PERFORMS A MATRIX-VECTOR MULTIPLY,
C               Y = Y + AX, ASSUMING DATA STRUCTURES USED IN
C               RECENTLY DEVELOPED SPARSE CHOLESKY CODES.  THE 
C               '2' SIGNIFIES LEVEL 2 LOOP UNROLLING.
C
C     INPUT PARAMETERS -
C        M      - NUMBER OF ROWS.
C        N      - NUMBER OF COLUMNS.
C        Y      - M-VECTOR TO WHICH AX WILL BE ADDED.
C        APNT   - INDEX VECTOR FOR A.  XA(I) POINTS TO THE
C                 FIRST NONZERO IN COLUMN I OF A.
C        Y      - ON OUTPUT, CONTAINS Y = Y + AX.
C
C***********************************************************************
C
      SUBROUTINE  SMXPY2 ( M, N, Y, APNT, A )
C
C***********************************************************************
C
C     -----------
C     PARAMETERS.
C     -----------
C
      INTEGER             M, N, LEVEL
C
      INTEGER             APNT(*)
C
      DOUBLE PRECISION    Y(*), A(*)
C
      PARAMETER           ( LEVEL = 2 )
C
C     ----------------
C     LOCAL VARIABLES.
C     ----------------
C
      INTEGER             I, I1, I2,
     &                    J, REMAIN
C
      DOUBLE PRECISION    A1, A2
C
C***********************************************************************
C
      REMAIN = MOD ( N, LEVEL )
C
      GO TO ( 2000, 100 ), REMAIN+1
C
  100 CONTINUE
      I1 = APNT(1+1) - M
      A1 = - A(I1)
      DO  150  I = 1, M
          Y(I) = Y(I) + A1*A(I1)
          I1 = I1 + 1
  150 CONTINUE
      GO TO 2000
C
 2000 CONTINUE
      DO  4000  J = REMAIN+1, N, LEVEL
          I1 = APNT(J+1) - M
          I2 = APNT(J+2) - M
          A1 = - A(I1)
          A2 = - A(I2)
          DO  3000  I = 1, M
              Y(I) = ( (Y(I)) +
     &               A1*A(I1)) + A2*A(I2)
              I1 = I1 + 1
              I2 = I2 + 1
 3000     CONTINUE
 4000 CONTINUE
C
      RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******     SMXPY4 .... MATRIX-VECTOR MULTIPLY            **************
C***********************************************************************
C***********************************************************************
C
C     PURPOSE - THIS ROUTINE PERFORMS A MATRIX-VECTOR MULTIPLY,
C               Y = Y + AX, ASSUMING DATA STRUCTURES USED IN
C               RECENTLY DEVELOPED SPARSE CHOLESKY CODES.  THE 
C               '4' SIGNIFIES LEVEL 4 LOOP UNROLLING.
C
C     INPUT PARAMETERS -
C        M      - NUMBER OF ROWS.
C        N      - NUMBER OF COLUMNS.
C        Y      - M-VECTOR TO WHICH AX WILL BE ADDED.
C        APNT   - INDEX VECTOR FOR A.  XA(I) POINTS TO THE
C                 FIRST NONZERO IN COLUMN I OF A.
C        Y      - ON OUTPUT, CONTAINS Y = Y + AX.
C
C***********************************************************************
C
      SUBROUTINE  SMXPY4 ( M, N, Y, APNT, A )
C
C***********************************************************************
C
C     -----------
C     PARAMETERS.
C     -----------
C
      INTEGER             M, N, LEVEL
C
      INTEGER             APNT(*)
C
      DOUBLE PRECISION    Y(*), A(*)
C
      PARAMETER           ( LEVEL = 4 )
C
C     ----------------
C     LOCAL VARIABLES.
C     ----------------
C
      INTEGER             I, I1, I2, I3, I4,
     &                    J, REMAIN
C
      DOUBLE PRECISION    A1, A2, A3, A4
C
C***********************************************************************
C
      REMAIN = MOD ( N, LEVEL )
C
      GO TO ( 2000, 100, 200, 300 ), REMAIN+1
C
  100 CONTINUE
      I1 = APNT(1+1) - M
      A1 = - A(I1)
      DO  150  I = 1, M
          Y(I) = Y(I) + A1*A(I1)
          I1 = I1 + 1
  150 CONTINUE
      GO TO 2000
C
  200 CONTINUE
      I1 = APNT(1+1) - M
      I2 = APNT(1+2) - M
      A1 = - A(I1)
      A2 = - A(I2)
      DO  250  I = 1, M
          Y(I) = ( (Y(I))
     &           + A1*A(I1)) + A2*A(I2)
          I1 = I1 + 1
          I2 = I2 + 1
  250 CONTINUE
      GO TO 2000
C
  300 CONTINUE
      I1 = APNT(1+1) - M
      I2 = APNT(1+2) - M
      I3 = APNT(1+3) - M
      A1 = - A(I1)
      A2 = - A(I2)
      A3 = - A(I3)
      DO  350  I = 1, M
          Y(I) = (( (Y(I))
     &           + A1*A(I1)) + A2*A(I2))
     &           + A3*A(I3)
          I1 = I1 + 1
          I2 = I2 + 1
          I3 = I3 + 1
  350 CONTINUE
      GO TO 2000
C
 2000 CONTINUE
      DO  4000  J = REMAIN+1, N, LEVEL
          I1 = APNT(J+1) - M
          I2 = APNT(J+2) - M
          I3 = APNT(J+3) - M
          I4 = APNT(J+4) - M
          A1 = - A(I1)
          A2 = - A(I2)
          A3 = - A(I3)
          A4 = - A(I4)
          DO  3000  I = 1, M
              Y(I) = ((( (Y(I))
     &               + A1*A(I1)) + A2*A(I2))
     &               + A3*A(I3)) + A4*A(I4)
              I1 = I1 + 1
              I2 = I2 + 1
              I3 = I3 + 1
              I4 = I4 + 1
 3000     CONTINUE
 4000 CONTINUE
C
      RETURN
      END
C***********************************************************************
C***********************************************************************
C
C   Version:        0.3
C   Last modified:  December 27, 1994
C   Authors:        Esmond G. Ng and Barry W. Peyton
C
C   Mathematical Sciences Section, Oak Ridge National Laboratory
C
C***********************************************************************
C***********************************************************************
C******     SMXPY8 .... MATRIX-VECTOR MULTIPLY            **************
C***********************************************************************
C***********************************************************************
C
C     PURPOSE - THIS ROUTINE PERFORMS A MATRIX-VECTOR MULTIPLY,
C               Y = Y + AX, ASSUMING DATA STRUCTURES USED IN
C               RECENTLY DEVELOPED SPARSE CHOLESKY CODES.  THE 
C               '8' SIGNIFIES LEVEL 8 LOOP UNROLLING.
C
C     INPUT PARAMETERS -
C        M      - NUMBER OF ROWS.
C        N      - NUMBER OF COLUMNS.
C        Y      - M-VECTOR TO WHICH AX WILL BE ADDED.
C        APNT   - INDEX VECTOR FOR A.  APNT(I) POINTS TO THE
C                 FIRST NONZERO IN COLUMN I OF A.
C        Y      - ON OUTPUT, CONTAINS Y = Y + AX.
C
C***********************************************************************
C
      SUBROUTINE  SMXPY8 ( M, N, Y, APNT, A )
C
C***********************************************************************
C
C     -----------
C     PARAMETERS.
C     -----------
C
      INTEGER             M, N, LEVEL
C
      INTEGER             APNT(*)
C
      DOUBLE PRECISION    Y(*), A(*)
C
      PARAMETER           ( LEVEL = 8 )
C
C     ----------------
C     LOCAL VARIABLES.
C     ----------------
C
      INTEGER             I, I1, I2, I3, I4, I5, I6, I7, I8,
     &                    J, REMAIN
C
      DOUBLE PRECISION    A1, A2, A3, A4, A5, A6, A7, A8
C
C***********************************************************************
C
      REMAIN = MOD ( N, LEVEL )
C
      GO TO ( 2000, 100, 200, 300,
     &         400, 500, 600, 700  ), REMAIN+1
C
  100 CONTINUE
      I1 = APNT(1+1) - M
      A1 = - A(I1)
      DO  150  I = 1, M
          Y(I) = Y(I) + A1*A(I1)
          I1 = I1 + 1
  150 CONTINUE
      GO TO 2000
C
  200 CONTINUE
      I1 = APNT(1+1) - M
      I2 = APNT(1+2) - M
      A1 = - A(I1)
      A2 = - A(I2)
      DO  250  I = 1, M
          Y(I) = ( (Y(I))
     &           + A1*A(I1)) + A2*A(I2)
          I1 = I1 + 1
          I2 = I2 + 1
  250 CONTINUE
      GO TO 2000
C
  300 CONTINUE
      I1 = APNT(1+1) - M
      I2 = APNT(1+2) - M
      I3 = APNT(1+3) - M
      A1 = - A(I1)
      A2 = - A(I2)
      A3 = - A(I3)
      DO  350  I = 1, M
          Y(I) = (( (Y(I))
     &           + A1*A(I1)) + A2*A(I2))
     &           + A3*A(I3)
          I1 = I1 + 1
          I2 = I2 + 1
          I3 = I3 + 1
  350 CONTINUE
      GO TO 2000
C
  400 CONTINUE
      I1 = APNT(1+1) - M
      I2 = APNT(1+2) - M
      I3 = APNT(1+3) - M
      I4 = APNT(1+4) - M
      A1 = - A(I1)
      A2 = - A(I2)
      A3 = - A(I3)
      A4 = - A(I4)
      DO  450  I = 1, M
          Y(I) = ((( (Y(I))
     &           + A1*A(I1)) + A2*A(I2))
     &           + A3*A(I3)) + A4*A(I4)
          I1 = I1 + 1
          I2 = I2 + 1
          I3 = I3 + 1
          I4 = I4 + 1
  450 CONTINUE
      GO TO 2000
C
  500 CONTINUE
      I1 = APNT(1+1) - M
      I2 = APNT(1+2) - M
      I3 = APNT(1+3) - M
      I4 = APNT(1+4) - M
      I5 = APNT(1+5) - M
      A1 = - A(I1)
      A2 = - A(I2)
      A3 = - A(I3)
      A4 = - A(I4)
      A5 = - A(I5)
      DO  550  I = 1, M
          Y(I) = (((( (Y(I))
     &           + A1*A(I1)) + A2*A(I2))
     &           + A3*A(I3)) + A4*A(I4))
     &           + A5*A(I5)
          I1 = I1 + 1
          I2 = I2 + 1
          I3 = I3 + 1
          I4 = I4 + 1
          I5 = I5 + 1
  550 CONTINUE
      GO TO 2000
C
  600 CONTINUE
      I1 = APNT(1+1) - M
      I2 = APNT(1+2) - M
      I3 = APNT(1+3) - M
      I4 = APNT(1+4) - M
      I5 = APNT(1+5) - M
      I6 = APNT(1+6) - M
      A1 = - A(I1)
      A2 = - A(I2)
      A3 = - A(I3)
      A4 = - A(I4)
      A5 = - A(I5)
      A6 = - A(I6)
      DO  650  I = 1, M
          Y(I) = ((((( (Y(I))
     &           + A1*A(I1)) + A2*A(I2))
     &           + A3*A(I3)) + A4*A(I4))
     &           + A5*A(I5)) + A6*A(I6)
          I1 = I1 + 1
          I2 = I2 + 1
          I3 = I3 + 1
          I4 = I4 + 1
          I5 = I5 + 1
          I6 = I6 + 1
  650 CONTINUE
      GO TO 2000
C
  700 CONTINUE
      I1 = APNT(1+1) - M
      I2 = APNT(1+2) - M
      I3 = APNT(1+3) - M
      I4 = APNT(1+4) - M
      I5 = APNT(1+5) - M
      I6 = APNT(1+6) - M
      I7 = APNT(1+7) - M
      A1 = - A(I1)
      A2 = - A(I2)
      A3 = - A(I3)
      A4 = - A(I4)
      A5 = - A(I5)
      A6 = - A(I6)
      A7 = - A(I7)
      DO  750  I = 1, M
          Y(I) = (((((( (Y(I))
     &           + A1*A(I1)) + A2*A(I2))
     &           + A3*A(I3)) + A4*A(I4))
     &           + A5*A(I5)) + A6*A(I6))
     &           + A7*A(I7)
          I1 = I1 + 1
          I2 = I2 + 1
          I3 = I3 + 1
          I4 = I4 + 1
          I5 = I5 + 1
          I6 = I6 + 1
          I7 = I7 + 1
  750 CONTINUE
      GO TO 2000
C
 2000 CONTINUE
      DO  4000  J = REMAIN+1, N, LEVEL
          I1 = APNT(J+1) - M
          I2 = APNT(J+2) - M
          I3 = APNT(J+3) - M
          I4 = APNT(J+4) - M
          I5 = APNT(J+5) - M
          I6 = APNT(J+6) - M
          I7 = APNT(J+7) - M
          I8 = APNT(J+8) - M
          A1 = - A(I1)
          A2 = - A(I2)
          A3 = - A(I3)
          A4 = - A(I4)
          A5 = - A(I5)
          A6 = - A(I6)
          A7 = - A(I7)
          A8 = - A(I8)
          DO  3000  I = 1, M
              Y(I) = ((((((( (Y(I))
     &               + A1*A(I1)) + A2*A(I2))
     &               + A3*A(I3)) + A4*A(I4))
     &               + A5*A(I5)) + A6*A(I6))
     &               + A7*A(I7)) + A8*A(I8)
              I1 = I1 + 1
              I2 = I2 + 1
              I3 = I3 + 1
              I4 = I4 + 1
              I5 = I5 + 1
              I6 = I6 + 1
              I7 = I7 + 1
              I8 = I8 + 1
 3000     CONTINUE
 4000 CONTINUE
C
      RETURN
      END
