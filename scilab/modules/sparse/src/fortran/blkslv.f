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
C*********     BLKSLV ... BLOCK TRIANGULAR SOLUTIONS          **********
C***********************************************************************
C***********************************************************************
C
C   PURPOSE:
C       GIVEN THE CHOLESKY FACTORIZATION OF A SPARSE SYMMETRIC
C       POSITIVE DEFINITE MATRIX, THIS SUBROUTINE PERFORMS THE
C       TRIANGULAR SOLUTION.  IT USES OUTPUT FROM BLKFCT.
C
C   INPUT PARAMETERS:
C       NSUPER          -   NUMBER OF SUPERNODES.
C       XSUPER          -   SUPERNODE PARTITION.
C       (XLINDX,LINDX)  -   ROW INDICES FOR EACH SUPERNODE.
C       (XLNZ,LNZ)      -   CHOLESKY FACTOR.
C
C   UPDATED PARAMETERS:
C       RHS             -   ON INPUT, CONTAINS THE RIGHT HAND SIDE.  ON
C                           OUTPUT, CONTAINS THE SOLUTION.
C
C***********************************************************************
C
      SUBROUTINE  BLKSLV (  NSUPER, XSUPER, XLINDX, LINDX , XLNZ  ,
     &                      LNZ   , RHS                             )
C
C***********************************************************************
C
        INTEGER             NSUPER
        INTEGER             LINDX(*)      , XSUPER(*)
        INTEGER             XLINDX(*)     , XLNZ(*)
        DOUBLE PRECISION    LNZ(*)        , RHS(*)
C
C***********************************************************************
C
        INTEGER             FJCOL , I     , IPNT  , IX    , IXSTOP,
     &                      IXSTRT, JCOL  , JPNT  , JSUP  , LJCOL
        DOUBLE PRECISION    T
C
C***********************************************************************
C
        IF  ( NSUPER .LE. 0 )  RETURN
C
C       ------------------------
C       FORWARD SUBSTITUTION ...
C       ------------------------
        FJCOL = XSUPER(1)
        DO  300  JSUP = 1, NSUPER
            LJCOL  = XSUPER(JSUP+1) - 1
            IXSTRT = XLNZ(FJCOL)
            JPNT   = XLINDX(JSUP)
            DO  200  JCOL = FJCOL, LJCOL
                IXSTOP    = XLNZ(JCOL+1) - 1
                T         = RHS(JCOL)/LNZ(IXSTRT)
                RHS(JCOL) = T
                IPNT      = JPNT + 1
CDIR$           IVDEP
                DO  100  IX = IXSTRT+1, IXSTOP
                    I      = LINDX(IPNT)
                    RHS(I) = RHS(I) - T*LNZ(IX)
                    IPNT   = IPNT + 1
  100           CONTINUE
                IXSTRT = IXSTOP + 1
                JPNT   = JPNT + 1
  200       CONTINUE
            FJCOL = LJCOL + 1
  300   CONTINUE
C
C       -------------------------
C       BACKWARD SUBSTITUTION ...
C       -------------------------
        LJCOL = XSUPER(NSUPER+1) - 1
        DO  600  JSUP = NSUPER, 1, -1
            FJCOL  = XSUPER(JSUP)
            IXSTOP = XLNZ(LJCOL+1) - 1
            JPNT   = XLINDX(JSUP) + (LJCOL - FJCOL)
            DO  500  JCOL = LJCOL, FJCOL, -1
                IXSTRT = XLNZ(JCOL)
                IPNT   = JPNT + 1
                T      = RHS(JCOL)
CDIR$           IVDEP
                DO  400  IX = IXSTRT+1, IXSTOP
                    I    = LINDX(IPNT)
                    T    = T - LNZ(IX)*RHS(I)
                    IPNT = IPNT + 1
  400           CONTINUE
                RHS(JCOL) = T/LNZ(IXSTRT)
                IXSTOP    = IXSTRT - 1
                JPNT      = JPNT - 1
  500       CONTINUE
            LJCOL = FJCOL - 1
  600   CONTINUE
C
        RETURN
      END
