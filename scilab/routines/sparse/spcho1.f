       SUBROUTINE SPCHO1 (NEQNS,A,NNZA,INDA,XADJF,PERM,INVP,
     & colcnt,snode,nnzl,nsub,nsuper,xsuper,
     & iwork,xadj,adjncy,indlnz,lnz,diag,ierr)
C
C       ----------- 
C       PARAMETERS.  
C       -----------
C
         INTEGER    IWSIZ,NEQNS,NNZA, NSUPER,iflag
         INTEGER    I,I0,nnzb,nnzc,nnzl,nsub
C
         INTEGER ADJNCY(NNZA), COLCNT(NEQNS)   ,SNODE(NEQNS)
         INTEGER INVP(NEQNS) , IWORK(7*NEQNS+3),PERM(NEQNS) 
         INTEGER XADJ(NEQNS+1),XSUPER(NEQNS+1),xadjf(neqns+1)
         INTEGER INDA(NNZA+NEQNS),indlnz(nnza+neqns)
         DOUBLE PRECISION  A(NNZA),LNZ(NNZA),diag(neqns)
c       
         iwsiz=7*neqns+3
        xadjf(1)=1
        do 100 i=2,neqns
 100    xadjf(i)=xadjf(i-1)+inda(i-1)
        xadjf(neqns+1)=nnza+1
c
         call dspt(neqns,neqns,a,nnza,inda,xadjf,lnz,iwork,indlnz)
         ierr=5
         do 107 i=1,nnza
         if (a(i) .ne. lnz(i) ) then
            ierr=1
            return
         endif
  107    continue 
         do 108 i=1,neqns
         if (xadjf(i) .ne. iwork(i)) then
            ierr=1
            return
         endif
 108     continue
c
c                     A -DIAG(A)
c
        i0=0
        i1=i0
        i=1
        ierr=0
        idiag0=0
        nnzc=nnza-neqns
        if (inda(neqns+1) .ne. 1 .or. inda(neqns+nnza) .ne. neqns 
     &   .or. inda(neqns).eq. 0 ) then
           ierr=1
           return
        endif
        do 10 k=1,nnza
 08      i0=i0+1
         if(i0-i1.gt.inda(i)) then
            i1=i0
            i=i+1
            goto 08
         endif
         j=inda(neqns+k)
        if (i .eq. j) then
            if (idiag0+1 .ne. i) then
              ierr=1
              return
            endif
            idiag0=i
            diag(i)=-a(k)
        endif
 10    continue
c
         do 200 i=1,neqns
         iwork(i)=1
 200     iwork(i+neqns)=i
         call dspasp(neqns,neqns,a,nnza,inda,
     &           diag,neqns,iwork,lnz,nnzc,indlnz,ierr)
         xadj(1)=1
         do 210 i=2,neqns
  210      xadj(i)=xadj(i-1)+indlnz(i-1)
         xadj(neqns+1)=nnzc+1
         DO 310 i=1,nnzc
  310     adjncy(i)=indlnz(neqns+i)
C***********************************************************************
C PURPOSE - THIS ROUTINE CALLS LIU'S MULTIPLE MINIMUM DEGREE ROUTINE.
C***********************************************************************
C
          CALL ORDMMD  (  NEQNS , XADJ  , adjncy, INVP ,PERM,
     1                    IWSIZ , IWORK , NSUB, IFLAG  )
C***********************************************************************
C   PURPOSE:
C       THIS SUBROUTINE COMPUTES THE STORAGE REQUIREMENTS AND SETS UP 
C       PRELIMINARY DATA STRUCTURES FOR THE SYMBOLIC FACTORIZATION.
C   CAUTION: THE ADJACENCY VECTOR ADJNCY WILL BE DESTROYED 
         DO 410 i=1,nnza-neqns
  410     adjncy(i)=indlnz(neqns+i)


C
C   NOTE:
C       THIS VERSION PRODUCES THE MAXIMAL SUPERNODE PARTITION (I.E.,
C       THE ONE WITH THE FEWEST POSSIBLE SUPERNODES).
C 
        CALL SFINIT(NEQNS , NNZc ,  XADJ  , ADJNCY, PERM ,
     &              INVP  , COLCNT, NNZL  , NSUB  , NSUPER,
     &              SNODE , XSUPER, IWSIZ , IWORK , IFLAG   )
        return
        end
c-------------------------------------------------------------------

