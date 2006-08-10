      SUBROUTINE  BLKFC1 ( NEQNS , NSUPER, XSUPER, SNODE , SPLIT , 
     &                      XLINDX, LINDX , XLNZ  , LNZ   , IWSIZ ,
     &                      IWORK , TMPSIZ, TMPVEC, IFLAG , level)
C
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        INTEGER             level
c
        INTEGER             XLINDX(*)     , XLNZ(*)
        INTEGER             IWORK(*)      , LINDX(*)      , 
     &                      SNODE(*)      , SPLIT(*)      , 
     &                      XSUPER(*)
        INTEGER             IFLAG , IWSIZ , NEQNS , NSUPER, TMPSIZ
        DOUBLE PRECISION    LNZ(*)        , TMPVEC(*)
c
        EXTERNAL            MMPY1, MMPY2 , MMPY4 , MMPY8
        EXTERNAL            SMXPY1, SMXPY2, SMXPY4, SMXPY8
C
C*********************************************************************
C
      if (level .eq. 1) then
      call  BLKFCT (  NEQNS , NSUPER, XSUPER, SNODE , SPLIT , 
     &               XLINDX, LINDX , XLNZ  , LNZ   , IWSIZ ,
     &               IWORK , TMPSIZ, TMPVEC, IFLAG , MMPY1 , 
     &               SMXPY1                                  )
      endif
      if (level .eq. 2) then
      call  BLKFCT (  NEQNS , NSUPER, XSUPER, SNODE , SPLIT , 
     &                XLINDX, LINDX , XLNZ  , LNZ   , IWSIZ ,
     &                IWORK , TMPSIZ, TMPVEC, IFLAG , MMPY2 , 
     &                SMXPY2                                  )
      endif
      if (level .eq. 4) then
      call  BLKFCT (  NEQNS , NSUPER, XSUPER, SNODE , SPLIT , 
     &                XLINDX, LINDX , XLNZ  , LNZ   , IWSIZ ,
     &                IWORK , TMPSIZ, TMPVEC, IFLAG , MMPY4 , 
     &                SMXPY4                                  )
      endif
      if (level .eq. 8) then
      call BLKFCT (  NEQNS , NSUPER, XSUPER, SNODE , SPLIT , 
     &               XLINDX, LINDX , XLNZ  , LNZ   , IWSIZ ,
     &               IWORK , TMPSIZ, TMPVEC, IFLAG , MMPY8 , 
     &               SMXPY8                                  )
      endif
      return
      end
