c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc     -------------------------------
c
      subroutine intmatrix(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      integer tops,top2
      integer iadr,sadr
      logical ref
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      tops=top

      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.lt.2) then
         call error(39)
         return
      endif
      if(rhs.gt.3) then
         top=tops
         call ref2val
         call setfunnam(ids(1,pt+1),'%hm_matrix',10)
         fun=-1
         return
      endif

      top2=top-rhs+1
      il2=iadr(lstk(top2))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))


c changement de dimension d'une matrice
      il=iadr(lstk(top+1-rhs))
 
***** part of code commented by bruno (see some explanations after)
c$$$      if(abs(istk(il)).eq.5.or.abs(istk(il)).eq.6) then
c$$$         top=tops
c$$$         call ref2val
c$$$         fin=12
c$$$         fun=27
c$$$c        *call* spelm
c$$$         return
c$$$      endif

      ityp=istk(il2)

* next line modified by bruno (added  sparse and sparse boolean)
      if(ityp.lt.1.or.ityp.gt.10) then
         top=tops
         call ref2val
         call funnam(ids(1,pt+1),'matrix',iadr(lstk(top-rhs+1)))
         fun=-1
         return
      endif

      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).ne.1) then
         err=3
         call error(53)
         return
      endif
      if(rhs.eq.2) then
         if(istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).eq.1) then
            m=int(stk(sadr(il+4)))
            n=1
         elseif(istk(il+1)*istk(il+2).eq.2) then
            m=int(stk(sadr(il+4)))
            n=int(stk(sadr(il+4)+1))
         else 
            top=tops
            call ref2val
            call setfunnam(ids(1,pt+1),'%hm_matrix',10)
            fun=-1
            return
         endif
      else
         if(istk(il+1)*istk(il+2).ne.1) then
            err=3
            call error(89)
            return
         endif
         if(istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         n=int(stk(sadr(il+4)))
         if(n.lt.-1) then
            err=3
            call error(116)
            return
         endif

c     
         top=top-1
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=2
            call error(53)
            return
         endif

         if(istk(il+1)*istk(il+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         if(istk(il+3).ne.0) then
            err=2
            call error(52)
            return
         endif
         m=int(stk(sadr(il+4)))
         if(m.lt.-1) then
            err=2
            call error(116)
            return
         endif

      endif
c
      top=top-1
      il=iadr(lstk(top))
      ilr=il

****** sparse case (added here by bruno dec 2004) **************************
      if (ityp.eq.5 .or. ityp.eq.6) then

*   NOTE: 
*     This part of the code duplicate some lines which are used
*     after for the others cases...  But if this added part
*     should bring some problems it will be enought to delete
*     it and to uncomment the "commented" lines at the beginning
*     of this subroutine. If no problems occurs I will clean
*     the code.
*
         ref = istk(il) .lt. 0
         if (ref) then
            il = iadr(lstk(istk(il+2)))
         endif
         mA = istk(il+1)
         nA = istk(il+2)
*        take care of the fact that mA x nA may be too big for 
*        integer arithmetic (mA x nA > 2^31-1)
         if ( dble(mA)*dble(nA) .gt. 2147483647.d0 ) then
            buf='integer overflow'
            call error(9999)
            return
         endif
         mn = mA*nA                   
         if(m.eq.-1.and.n.eq.-1) then 
            call error(42)            
            return                    
         endif                        
         if(m.eq.-1) m=mn/n           
         if(n.eq.-1) n=mn/m           
         if(mA*nA.ne.m*n) then
            call error(60)
            return
         endif
         it = istk(il+3)
         nel = istk(il+4)
         iAmnel = il+5
         iAcol = iAmnel + mA
         iAR = sadr(iAcol+nel)
         iAI = iAR + nel
         if (ref) then
            ilB = ilr
         else
            ilB = iadr(lstk(top+1))
         endif
         iBmnel = ilB + 5
         iBcol = iBmnel + m
         iBR = sadr(iBcol+nel)
         iBI = iBR + nel
         if (ityp .eq. 6) then  ! boolean sparse
            ifin = iBR
         else ! real or cmplx sparse
            if (it .eq. 0) then
               ifin = iBI
            else
               ifin = iBI + nel
            endif
         endif
*        memory for work arrays
         ij = iadr(ifin)
         ip = ij + 2*nel
         iptr = ip + nel
         ilast = sadr( iptr + max(nA,m) + 1 )
         err = ilast - lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
 
         if (ityp .eq. 6) then
            ite = -1
         else
            ite = it
         endif

         call spreshape(mA,nA,istk(iAmnel),istk(iAcol),stk(iAR),stk(iAI)
     $                  ,m, n,istk(iBmnel),istk(iBcol),stk(iBR),stk(iBI)
     $                  ,nel, ite, istk(ij),istk(iptr),istk(ip))

         istk(ilr) = ityp  ! a voir
         istk(ilr+1) = m
         istk(ilr+2) = n
         istk(ilr+3) = it
         istk(ilr+4) = nel
         if (ref) then
            lstk(top+1) = ifin
         else
            call icopy(m+nel,istk(iBmnel),1,istk(ilr+5),1)
            lr = sadr(ilr+5+m+nel)
            if (ityp .eq. 6) then
               lstk(top+1) = lr
            else
               call unsfdcopy(nel*(it+1),stk(iBR),1,stk(lr),1)
               lstk(top+1) = lr+nel*(it+1)
            endif
         endif
         return
      endif 
*** end of the sparse case ***********************************************


      if(istk(il).lt.0) then
         k=istk(il+2)
         err=lstk(top)+lstk(k+1)-lstk(k)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(lstk(k+1)-lstk(k),stk(lstk(k)),1,
     $        stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(k+1)-lstk(k)
      endif

      mn=istk(il+1)*istk(il+2)
      if(m.eq.-1.and.n.eq.-1) then
         call error(42)
         return
      endif
      if(m.eq.0.or.n.eq.0) then
         if(mn.ne.0) then
            call error(60)
            return
         endif
         return
      endif
      if(m.eq.-1) m=mn/n
      if(n.eq.-1) n=mn/m

      if(m*n.ne.istk(il+1)*istk(il+2)) then
         call error(60)
         return
      endif
      if(m*n.eq.0) then
         istk(il+1)=0
         istk(il+2)=0
      else
         istk(il+1)=m
         istk(il+2)=n
      endif
 999  return
      end
c     -------------------------------
