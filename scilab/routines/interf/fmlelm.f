      subroutine fmlelm
c ================================== ( Inria    ) =============
c     
c     evaluate formal operations
c =============================================================
c     
c     Copyright INRIA
      include '../stack.h'
      logical iseye,isnum
      integer lparen,rparen,star,plus,minus,blanc,slash,bslash,symb
      integer iadr,sadr
      data lparen/41/,rparen/42/,star/47/,plus/45/,minus/46/,blanc/40/
      data slash/48/  ,bslash/49/
c     
c     fonctions/fin
c     addf       mulf  ldivf  rdivf
c 
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c    
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' fmlelm '//buf(1:4))
      endif
      if(rhs.ne.2) then
         call error(39)
         return
      endif
      
      goto(1001,1001,2000,2001,2002) fin
c     
c     addition soustraction formelle
c     
      
 1001 continue
      isg=1
      if(fin.eq.2) isg=-1
      ilptr1=iadr(lstk(top+1))+1
c     le "+1" est du a expsum qui peut renvoyer une chaine 1 carac plus long 
c     que la chaine donnee
      il1=iadr(lstk(top-1))
      il=il1
      if(istk(il1).ne.10) then
         err=1
         call error(55)
         return
      endif
      il2=iadr(lstk(top))
      if(istk(il2).ne.10) then
         err=2
         call error(55)
         return
      endif
      is=plus
c     
      if(istk(il1+1)*istk(il1+2).ne.1) then
         err=1
         call error(36)
         return
      endif
      n1=istk(il1+5)-1
      il1=il1+6
      il0=il1
      maxnp=iadr(lstk(bot))-ilptr1
      call expsum(1,istk(il1),n1,istk(ilptr1),np1,maxnp,err)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
      ilptr2=ilptr1+np1+1
      if(istk(il2+1)*istk(il2+2).ne.1) then
         err=2
         call error(36)
         return
      endif
      n2=istk(il2+5)-1
      il2=il2+6
      top=top-1
      maxnp=iadr(lstk(bot))-ilptr2
      call expsum(isg,istk(il2),n2,istk(ilptr2),np2,maxnp,err)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     
      inum=0
      do 1005 i1=1,np1
         it1=il1-1+istk(ilptr1-1+i1)
         nt1=istk(ilptr1+i1)-istk(ilptr1-1+i1)
         if(istk(it1+1).eq.0.and.n1.eq.2) then
            call icopy(n2,istk(il2),1,istk(il1),1)
            il1=il1+n2
            goto 1010
         endif
         if(isnum(istk(it1+1),nt1-1,inum1)) then
            if(istk(it1).eq.minus) inum1=-inum1  
            inum=inum+inum1
            istk(it1)=0
         endif
         i2=0
 1004    i2=i2+1
         if(i2.gt.np2) goto 1005
         it2=il2-1+istk(ilptr2-1+i2)
         if(istk(it2).eq.0) goto 1004
         nt2=istk(ilptr2+i2)-istk(ilptr2-1+i2)
         if(istk(it2+1).eq.0.and.n2.eq.2) then
            if (istk(it1).ne.0) then
               il1=il1+n1
               goto 1010
            else
               ilw=il1
               goto 1008
            endif
         endif
         if(isnum(istk(it2+1),nt2-1,inum2)) then
            if(istk(it2).eq.minus) inum2=-inum2
            inum=inum+inum2
            istk(it2)=0
            goto 1004
         endif
         if (istk(it1).ne.istk(it2).and.nt1.eq.nt2) then
c     on regarde si les termes sont egaux
            do 1002 k=2,nt1
               if(istk(it1-1+k).ne.istk(it2-1+k)) goto 1004
 1002       continue
            istk(it2)=0
            istk(it1)=0
            goto 1005
         endif
         goto 1004
 1005 continue
c     
      ilw=il1
      do 1006 i1=1,np1
         it1=il1-1+istk(ilptr1-1+i1)
         nt1=istk(ilptr1+i1)-istk(ilptr1-1+i1)
         if(istk(it1).ne.0) then
            call icopy(nt1,istk(it1),1,istk(ilw),1)
            ilw=ilw+nt1
         endif
 1006 continue
      do 1007 i2=1,np2
         it2=il2-1+istk(ilptr2-1+i2)
         nt2=istk(ilptr2+i2)-istk(ilptr2-1+i2)
         if(istk(it2).ne.0) then
            call icopy(nt2,istk(it2),1,istk(ilw),1)
            ilw=ilw+nt2
         endif
 1007 continue
c
 1008 if(inum.ne.0) then
         if(inum.lt.0) then
            istk(ilw)=minus
            ilw=ilw+1
            inum=-inum
         elseif(ilw.ne.il0) then
            istk(ilw)=plus
            ilw=ilw+1
         endif
         call intstr(inum,istk(ilw),ni,0)
         ilw=ilw+ni
      endif
c     
      il1=ilw
      if(il1.eq.il0) then
         istk(il1)=0
         il1=il1+1
      endif
c     
 1010 n1=il1-il0
      if(istk(il0).eq.plus) then
         call icopy(n1-1,istk(il0+1),1,istk(il0),1)
         n1=n1-1
         il1=il1-1
      endif
      istk(il+5)=n1+1
      lstk(top+1)=sadr(il1+1)
      goto 9999
c     
c     multiplication et divisions formelle
 2000 symb=star
      goto 2005
 2001 symb=bslash
      goto 2005
 2002 symb=slash
      goto 2005
 2005 il1=iadr(lstk(top-1))
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
