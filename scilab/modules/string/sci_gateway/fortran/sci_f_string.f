c =========================================
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c =========================================  
      subroutine intstring
      INCLUDE 'stack.h'
c
      integer eol,nclas
      logical ref
      integer iadr,sadr
      data eol/99/,nclas/29/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
      ref=ilr.ne.il

      if(istk(il).eq.10) then
         call ref2val
         goto 999
      endif
c
      lw=iadr(lstk(top+1))
c
      if(istk(il).eq.1) then
c
c     conversion d'une matrice de scalaires
c     -------------------------------------
         m=istk(il+1)
         n=istk(il+2)
         
         if(m*n.eq.0) then
            if (ref) then
               istk(ilr)=1
               istk(ilr+1)=0
               istk(ilr+2)=0
               istk(ilr+3)=0
               lstk(top+1)=sadr(ilr+4)
            endif
            goto 999
         endif

         it=istk(il+3)
         l=sadr(il+4)
         if(ref) then
            lw=ilr+4
         else
            lw=iadr(lstk(top+1))
         endif
         err=sadr(lw+m*n*(2*lct(7)+4))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         lstr=lw+m*n+1
         if(it.eq.0) call cvdm(stk(l),m,m,n,lct(7),lct(6),
     1        istk(lstr),istk(lw))
         if(it.eq.1) call cvwm(stk(l),stk(l+m*n),m,m,n,lct(7),lct(6),
     1        istk(lstr),istk(lw))
         nstr=istk(lw+m*n)-1
         if(.not.ref) call icopy(m*n+1+nstr,istk(lw),1,istk(ilr+4),1)
         istk(ilr)=10
         istk(ilr+1)=m
         istk(ilr+2)=n
         istk(ilr+3)=0
         lstk(top+1)=sadr(ilr+5+m*n+nstr)
         goto 999
c
      elseif(abs(istk(il)).eq.11.or.abs(istk(il)).eq.13) then
c
c     conversion d'une macro non compilee
c     -----------------------------------
         ilm=il
c
c     form vectors of output and input variables
         il=il+1
         lw=lstk(top+1)
         do 37 i=1,2
            n=istk(il)
            il=il+1
            ilio=iadr(lw)
            istk(ilio)=10
            if(n.eq.0) istk(ilio)=1
            istk(ilio+1)=min(1,n)
            istk(ilio+2)=n
            istk(ilio+3)=0
            ilp=ilio+4
            istk(ilp)=1
            l=ilp+n
            if (n.eq.0) goto 35
            do 34 j=1,n
               call namstr(istk(il),istk(l+1),nn,1)
               l=l+nn
               istk(ilp+j)=l+1-(ilp+n)
               il=il+nsiz
 34         continue
 35         if(i.eq.1) then
               lout=lw
            else
               llin=lw
            endif
            lw=sadr(l+1)
 37      continue
c
         if(istk(ilm).eq.13) then
            ltxt=lw
            ilt=iadr(ltxt)
            istk(ilt)=1
            istk(ilt+2)=0
            istk(ilt+3)=0
            istk(ilt+4)=1
            ilt=ilt+4
            goto 43
         endif
         ltxt=lw
         ilt=iadr(ltxt)
         istk(ilt)=10
         istk(ilt+2)=1
         istk(ilt+3)=0
         istk(ilt+4)=1
         ilp=ilt+4
c     
c     compute number of lines of the macro
         nch=istk(il)
         nl=0
         il=il+1
         l=il-1
 39      l=l+1
         if(istk(l).eq.eol) then
            if(istk(l+1).eq.eol) goto 40
            nl=nl+1
         endif
         goto 39
c
 40      continue
         istk(ilt+1)=nl
         if(nl.eq.0) then
            istk(ilt)=1
            istk(ilt+2)=0
            ilt=ilt+3
            goto 43
         endif
         ilt=ilp+nl+1
         l=il
 41      if(istk(l).eq.eol) goto 42
         l=l+1
         goto 41
 42      if(istk(l+1).eq.eol) goto 43
         n=l-il
         ilp=ilp+1
         istk(ilp)=istk(ilp-1)+n
         call icopy(n,istk(il),1,istk(ilt),1)
         ilt=ilt+n
         il=l+1
         l=il
         goto 41
c
 43      continue
         lw=sadr(ilt+1)
c     update stack
         if(lhs.eq.3) then
            n=llin-lout
            call unsfdcopy(n,stk(lout),1,stk(lstk(top)),1)
            lstk(top+1)=lstk(top)+n+1
            top=top+1
            n=ltxt-llin
            call unsfdcopy(n,stk(llin),1,stk(lstk(top)),1)
            lstk(top+1)=lstk(top)+n+1
            top=top+1
         endif
         n=lw-ltxt
         call unsfdcopy(n,stk(ltxt),1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+n+1
         goto 999

      elseif(istk(il).eq.14) then
c     librairies
         il0=ilr
c
         n1=istk(il+1)
         l1=il+2
         il=il+n1+2
         n=istk(il)
         il=il+nclas+2

         if(.not.ref) ilr=lw
         err=sadr(ilr+6+n1+(nlgh+1)*n)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif

         istk(ilr)=10
         istk(ilr+1)=n+1
         istk(ilr+2)=1
         istk(ilr+3)=0
         istk(ilr+4)=1
         l=ilr+6+n
         call icopy(n1,istk(l1),1,istk(l),1)
         istk(ilr+5)=1+n1
         l=l+n1
         do 49  k=1,n
           call namstr(istk(il),istk(l),nn,1)
           istk(ilr+5+k)=istk(ilr+4+k)+nn
           l=l+nn
           il=il+nsiz
 49     continue
        if(.not.ref) call icopy(l-ilr,istk(ilr),1,istk(il0),1)
        lstk(top+1)=sadr(il0+l-ilr)
        goto 999
      else
         fun=-1
         call funnam(ids(1,pt+1),'string',il)
         return
      endif
 999  return
      end
c =========================================  
