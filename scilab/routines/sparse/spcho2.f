C********************************************************************
       SUBROUTINE SPCHO2 (  NEQNS , NSUPER, XSUPER, SNODE , SPLIT , 
     &                  XLINDX, LINDX , XLNZ  , LNZ  ,
     &                  IWORK , TMPSIZ, TMPVEC,indlnz,nnzl,nsub) 
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        INTEGER             level
c
        INTEGER         XLINDX(nsuper+1)     , XLNZ(neqns+1)
        INTEGER         IWORK(2*neqns+2*nsuper), LINDX(nsub), 
     &                  SNODE(neqns)      , SPLIT(neqns)    , 
     &                  XSUPER(nsuper+1),indlnz(neqns+nnzl)
        INTEGER         IFLAG , IWSIZ , NEQNS , NSUPER, TMPSIZ
        DOUBLE PRECISION  LNZ(nnzl)        , TMPVEC(tmpsiz)
C***********************************************************************
C   PURPOSE:
C       THIS SUBROUTINE CALLS THE BLOCK GENERAL SPARSE CHOLESKY ROUTINE,
C       BLKFC2.
C
        iwsiz=2*neqns+2*nsuper
        level = 8
         CALL BLKFC1 (  NEQNS , NSUPER, XSUPER, SNODE , SPLIT , 
     &                  XLINDX, LINDX , XLNZ  , LNZ  , IWSIZ ,
     &                  IWORK , TMPSIZ, TMPVEC, IFLAG ,level)
c********************************************************************
       do 1110 i=2,neqns+1
 1110  indlnz(i-1)=xlnz(i)-xlnz(i-1)
       call icopy (xlindx(nsuper+1)-1,lindx,1,indlnz(neqns+1),1)
       i=1
       do 1140 j=1,neqns
       if (i .eq. nsuper+1) go to 1141
       if ((xlnz(j+1)-xlnz(j) .eq. xlindx(i+1)-xlindx(i)) 
     &  .and. (indlnz(neqns+xlnz(j)) .eq. j))     go to 1120
       l=xlindx(nsuper+1)-xlindx(i)+(xlnz(j+1)-xlnz(j))
       call icopy(l,lindx(xlindx(i)-(xlnz(j+1)-xlnz(j))),
     &     1,indlnz(neqns+xlnz(j)),1)
       i=i-1
 1120  continue
       i=i+1
 1140  continue
       go to 1135
 1141  k=xlnz(neqns+1)-xlnz(j)
       i=1
         ii=1
 1125  if (i.gt.k) go to 1135 
        do 1130 j=1,ii 
       indlnz(neqns+xlnz(neqns+1)-i)=neqns-j+1
 1130  i=i+1
        ii=ii+1
        go to 1125
 1135  continue
       return
       end
C**********************************************************************
