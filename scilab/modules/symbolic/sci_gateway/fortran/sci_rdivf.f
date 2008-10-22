c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine scirdivf(fname)
c     ----------------------------
      character*(*) fname
      include 'stack.h'
      
      logical iseye,isnum
      integer lparen,rparen,star,plus,minus,blanc,slash,bslash,symb
      integer iadr,sadr
      data lparen/41/,rparen/42/,star/47/,plus/45/,minus/46/,blanc/40/
      data slash/48/  ,bslash/49/
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      
      symb=slash
            
			il1=iadr(lstk(top-1))
      il=il1
      if(istk(il1).ne.10) then
         err=1
         call error(55)
         return
      endif
      if(istk(il1+1)*istk(il1+2).ne.1) then
         err=1
         call error(36)
         return
      endif
      n1=istk(il1+5)-1
      il1=il1+6
      il0=il1
      call atome(istk(il1),n1,it1,is1)
      if(symb.ne.bslash) call termf(istk(il1),n1,it1)
c     
      il2=iadr(lstk(top))
      if(istk(il2).ne.10) then
         err=2
         call error(55)
         return
      endif
      if(istk(il2+1)*istk(il2+2).ne.1) then
         err=2
         call error(36)
         return
      endif
      n2=istk(il2+5)-1
      il2=il2+6
      top=top-1
c     
      call atome(istk(il2),n2,it2,is2)
      if(symb.ne.slash) call termf(istk(il2),n2,it2)
      if (it2.ne.0) call factf(istk(il2),n2,it2)

c     
      goto (2010,2020,2030,2040) it1+2*it2+1
c     
c     2 expressions
 2010 continue
      idec=0
      if(is1.ne.1) then
         call icopy(n1,istk(il1),-1,istk(il1+1),-1)
         idec=1
      endif
      il0=il1
      istk(il1)=lparen
      istk(il1+n1+idec)=rparen
      il1=il1+n1+idec+1
      istk(il1)=symb
      il1=il1+1
      istk(il1)=lparen
      il1=il1+1
      if(is2.eq.1) then
         il2=il2+1
         n2=n2-1
      endif
      call icopy(n2,istk(il2),1,istk(il1),1)
      il1=il1+n2
      istk(il1)=rparen
      il1=il1+1
      istk(il+5)=il1-il0+1
      lstk(top+1)=sadr(il1)
      goto 9999
 2020 continue
c     un atome et une expression
      idec=abs(is1)
      ipar=0
      if(n1-idec.eq.1 .and.istk(il1+idec).eq.0) then
         if(symb.eq.bslash) then
            call error(27)
            return
         endif
         goto 9999
      endif
      if(symb.ne.slash.and.
     &     ((n1-idec.eq.1 .and. istk(il1+idec).eq.1).or.
     &     (n1-idec.eq.5.and.iseye(istk(il1+idec))))) then
c     n1=n1-1
         n1=idec
         if(is1.eq.-1) then
            istk(il1+n1)=lparen
            n1=n1+1
            ipar=1
         endif
         goto 2021
      else
         istk(il1+n1)=symb
         istk(il1+n1+1)=lparen
         n1=n1+2
         ipar=1
      endif
 2021 if(is2.eq.1) then
         il2=il2+1
         n2=n2-1
      endif
      call icopy(n2,istk(il2),1,istk(il1+n1),1)
      n1=n1+n2
      if(ipar.eq.1) then
         istk(il1+n1)=rparen
         n1=n1+1
      endif
      istk(il+5)=1+n1
      lstk(top+1)=sadr(il1+n1)
      goto 9999
c     expr et atome
 2030 continue
      idec=abs(is2)
      if(n2-idec.eq.1.and.istk(il2+idec).eq.0) then
         if(symb.eq.slash) then
            call error(27)
            return
         endif
         istk(il1)=0
         istk(il+5)=2
         lstk(top+1)=sadr(il1+1)
         goto 9999
      endif
      if(symb.ne.bslash.and.is2.ge.0
     &     .and.((n2-idec.eq.1.and.istk(il2+idec).eq.1)
     &     .or.(n2-idec.eq.5.and.iseye(istk(il2+idec))))) then
         istk(il+5)=n1+1
         lstk(top+1)=sadr(il1+n1)
         goto 9999
      endif
      idec=0
      if(is1.ne.1) idec=idec+1
      if(is2.lt.0) idec=idec+1
      call icopy(n1,istk(il1),-1,istk(il1+idec),-1)
      il0=il1
      ill=il1
      if(is2.lt.0) then
         istk(ill)=minus
         ill=ill+1
      endif
      istk(ill)=lparen
      istk(il1+n1+idec)=rparen
      il1=il1+n1+idec+1
      if(symb.ne.bslash.and.is2.eq.-1.and.
     &     ((n2.eq.2.and.istk(il2+1).eq.1).or.
     &     (n2.eq.6.and.iseye(istk(il2+1))))) goto 2031
      istk(il1)=symb
      il1=il1+1
      if(is2.ne.0) then
         il2=il2+1
         n2=n2-1
      endif
      call icopy(n2,istk(il2),1,istk(il1),1)
      il1=il1+n2
 2031 istk(il+5)=il1-il0+1
      lstk(top+1)=sadr(il1)
      goto 9999
c     atome et atome
 2040 continue

      idec1=abs(is1)
      if(n1-idec1.eq.1.and.istk(il1+idec1).eq.0) then
         if(symb.eq.bslash) then
            call error(27)
            return
         endif
         goto 9999
      endif
      idec2=abs(is2)
      if(n2-idec2.eq.1.and.istk(il2+idec2).eq.0) then
         if(symb.eq.slash) then
            call error(27)
            return
         endif
         istk(il1)=0
         istk(il+5)=2
         lstk(top+1)=sadr(il1+1)
         goto 9999
      endif
      if(symb.eq.star) then
         if(isnum(istk(il1+idec1),n1-idec1,inum1).and.
     &        isnum(istk(il2+idec2),n2-idec2,inum2)) then
            if(is1.eq.-1) inum1=-inum1
            if(is2.eq.-1) inum2=-inum2
            inum=inum1*inum2
            call intstr(inum,istk(il1),ni,0)
            il1=il1+ni
            istk(il+5)=ni+1
            lstk(top+1)=sadr(il1)
            goto 9999
         endif
      endif
      is=blanc
      if(is1.eq.-1.and.is2.ge.0 .or.
     +     is2.eq.-1.and.is1.ge.0) is=minus
      if(symb.ne.slash.and.
     &     ((n1-abs(is1).eq.1.and.istk(il1+abs(is1)).eq.1).or.
     &     (n1-abs(is1).eq.5.and.iseye(istk(il1+abs(is1))))) ) then
         if(is.eq.minus) then
            istk(il1)=is
            il1=il1+1
         endif
         call icopy(n2-abs(is2),istk(il2+abs(is2)),1,istk(il1),1)
         il1=il1+n2-abs(is2)
         istk(il+5)=il1-il0+1
         lstk(top+1)=sadr(il1+1)
         goto 9999
      endif
      idec1=0
      idec=0
      if(is1.eq.-1.and.is2.eq.-1) idec1=1
      if(is.eq.minus.and.is1.eq.0) then
         idec=1
         call icopy(n1,istk(il1),-1,istk(il1+idec),-1)
         n1=n1+1
      endif
      if(idec1.ne.0) then
         call icopy(n1-1,istk(il1+idec+1),1,istk(il1+idec),1)
         n1=n1-1
      endif
      il0=il1
      ill=il1
      if(is.eq.minus) then
         istk(ill)=minus
         ill=ill+1
      endif
      il1=il1+n1
      if(symb.ne.bslash.and.
     &     ((n2-abs(is2).eq.1.and.istk(il2+abs(is2)).eq.1).or.
     &     (n2-abs(is2).eq.5.and.iseye(istk(il2+abs(is2)))))) goto 2041
      istk(il1)=symb
      il1=il1+1
      if(is2.ne.0) then
         il2=il2+1
         n2=n2-1
      endif
      call icopy(n2,istk(il2),1,istk(il1),1)
      il1=il1+n2
 2041 istk(il+5)=il1-il0+1
      lstk(top+1)=sadr(il1)
      goto 9999
c     
 9999 return
      end
