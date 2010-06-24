c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intspchol(id)
c     
      include 'stack.h'
      integer id(nsiz)
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)

      lw = lstk(top+1)
      l0=  lstk(top+1-rhs)
      if (rhs .ne.1) then
         call error(39)
         return
      endif
      if (lhs .ne. 2) then
         call error(41)
         return
      endif 
      il=iadr(lstk(top))
      ityp=istk(il)
      if(ityp.ne.5.and.ityp.ne.6) then
         err=1 
         call error(217)
         return
      endif
      nel=istk(il+4)
      m=istk(il+1)
      n=istk(il+2)
      if (m.ne.n) then
         call error(20)
         return
      endif
      it=istk(il+3)
      if (it.ne.0) then
         err=1
         call error(52)
         return
      endif
      iinda=il+5
      ia=sadr(iinda+nel+n)
      ixadjf=iadr(ia+nel)
      iperm=ixadjf+n+1
      iinvp=iperm+n
      icolcnt=iinvp+n
      isnode=icolcnt+n
      ixadj=isnode+n
      iadjncy=ixadj+n+1
      ixsuper=iadjncy+nel-n
      iiwork=ixsuper+n+1
      iindlnz=iiwork+7*n+3
      ilnz=sadr(iindlnz+n+nel)
      idiag=ilnz+nel
      err = idiag+n-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      call spcho1(n,stk(ia),nel,istk(iinda),istk(ixadjf),istk(iperm),
     &     istk(iinvp),istk(icolcnt),istk(isnode),nnzl,nsub,nsuper,
     &     istk(ixsuper),istk(iiwork),istk(ixadj),istk(iadjncy),
     &     istk(iindlnz), stk(ilnz),stk(idiag),ierr)
C     
      if (ierr .eq. 1) then
         call error(29)
         if(n.le.0) then
            call blkslv(0,0,0,0,0,0.d0,0.d0)
         endif
         return
      endif
      iwsiz=nsuper+2*n+1
C************************************************************************
      iiwork=ixsuper+nsuper+1
      ixlindx=iiwork+2*nsuper+2*n+1
      ilindx=ixlindx+nsuper+1
      ixlnz=ilindx+nsub
      ilnz=sadr(ixlnz+n+1)
      isplit=iadr(ilnz+nnzl)
      itmpvec=sadr(isplit+n)
      CALL SYMFCT (N,nel-n,istk(iXADJ),istk(iADJNCY), istk(iPERM), 
     &     istk(iINVP),istk(iCOLCNT),NSUPER,istk(iXSUPER),istk(iSNODE),
     &     nsub , istk(iXLINDX), istk(iLINDX),istk(iXLNZ),nsuper+2*n+1,
     &     istk(iiwork) , IFLAG    )
C     
      CALL  BFINIT ( n , NSUPER,istk(iXSUPER),istk(iSNODE) , 
     &     istk(iXLINDX),istk(iLINDX), 16, iTMPSIZ, istk(iSPLIT) )
c     
      itmpsiz=max(itmpsiz,n)
      iindlnz=iadr(itmpvec+itmpsiz)
      err = sadr(iindlnz+n+nnzl)-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
C     
      CALL INPNV (n,istk(iXADJF),istk(iinda+n),stk(ia),istk(iPERM),
     &     istk(iINVP),NSUPER, istk(iXSUPER),istk(iXLINDX),istk(iLINDX),
     &     istk(iXLNZ),stk(iLNZ),istk(iIWORK)     )
C     
      CALL  SPCHO2 ( n , NSUPER, istk(iXSUPER),istk(iSNODE) ,
     &     istk(iSPLIT) , istk(iXLINDX), istk(iLINDX) ,istk(iXLNZ)  , 
     &     stk(iLNZ)  , istk(iIWORK) ,iTMPSIZ,stk(iTMPVEC),
     &     istk(iindlnz),nnzl,nsub)
      nel=istk(ixlnz+n)-1
      ia=sadr(iinda+nel+n)
      ierr=5
      do 32 i=1,nel
         if (stk(iLNZ+i-1) .gt. 5.D+63) then
            ierr=1
         endif
 32   continue
      if (ierr .eq. 1) then
         call basout(io,wte,
     &        'WARNING: Possible not positive definite matrix!')
      endif
      call icopy(n,istk(iinvp),1,istk(iindlnz-n),1)
      if (ia+nel .GE. sadr(ixlnz)) then
         ixln=iadr(ia+nel)
         iln=sadr(ixln+n+1)
         isplit=iadr(iln+nnzl)
         itmpvec=sadr(isplit+n)
         iindn=iadr(itmpvec+itmpsiz)
         do 33 i=1,n+nnzl
            istk(iindn+n+nnzl-i)=istk(iindlnz+n+nnzl-i)
 33      continue
         iindlnz=iindn
         do 34 i=1,nnzl
            stk(iln+nnzl-i)=stk(ilnz+nnzl-i)
 34      continue
         ilnz=iln
         do 35 i=1,n+1
            istk(ixln+n+1-i)=istk(ixlnz+n+1-i)      
 35      continue
         ixlnz=ixln
         call icopy(n,istk(iinvp),1,istk(iindlnz-n),1)
         err = sadr(iindlnz+n+nnzl)-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
      endif
C     
      call dspt(n,n,stk(ilnz),nel,istk(iindlnz),istk(ixlnz),
     &     stk(ia),istk(isplit),istk(iinda))
      istk(il+4)=nel
      top=top+1 

      lstk(top)=ia+nel
      lnl=iadr(ia+nel)
      istk(lnl)=ityp
      istk(lnl+1)=n
      istk(lnl+2)=n
      istk(lnl+3)=0
      istk(lnl+4)=n        
      call icopy(n,istk(iindlnz-n),1,istk(lnl+5+n),1)
      ilp=sadr(lnl+5+2*n)
      do 70 i=1,n
         istk(lnl+4+i)=1
         stk(ilp+i-1)=1.0d0
 70   continue

      lstk(top+1)=ilp+n
c     
      return
      end

c			======================================
