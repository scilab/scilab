C**********************************************************************
       SUBROUTINE SPCOMPACK ( NEQNS , NSUPER, NSUB, NNZ, XLINDX, LINDX , 
     &                        XLNZ  ,  ADJNCY)
C***********************************************************************
C
C       -----------
C       PARAMETERS.
C       -----------
C
        INTEGER         NEQNS, NSUPER, NSUB, NZL
c
        INTEGER         XLINDX(nsuper+1), XLNZ(neqns+1)
        INTEGER         LINDX(nsub)     , ADJNCY(nnz)
C***********************************************************************
C   PURPOSE:
C       THIS SUBROUTINE Convert a compact adjacency representation into 
C       a standard adjacency form
C*******************************************************************

       call icopy (nsub,lindx,1,adjncy,1)
       i=1
       do 1140 j=1,neqns
       if (i .eq. nsuper+1) go to 1141
       if ((xlnz(j+1)-xlnz(j) .eq. xlindx(i+1)-xlindx(i)) 
     &    .and. (adjncy(xlnz(j)) .eq. j))          go to 1120
       l=xlindx(nsuper+1)-xlindx(i)+(xlnz(j+1)-xlnz(j))
       call icopy(l,lindx(xlindx(i)-(xlnz(j+1)-xlnz(j))),
     &     1,adjncy(xlnz(j)),1)
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
       adjncy(xlnz(neqns+1)-i) = neqns-j+1
 1130  i=i+1
        ii=ii+1
        go to 1125
 1135  continue
       return
       end
C**********************************************************************
