c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine print(id,lk,lunit)
c     ==================================================================
c     print object of id id(nsiz) stored at position lk in the stack
c     ==================================================================
      include 'stack.h'
      integer id(nsiz),lk,id1(nsiz),dname(nsiz)
c     
      common / ptkeep / lwk
      integer itype,itypel,gettype
      integer fl,mode,m,n,it,lr,lc,nlr,lkeep,topk,lname,siz,vol,tops
      integer namef(nlgh),under,pchar
      logical getmat,ilog,getpoly,typer,clsave,getsimat,gethmat
      logical crewimat ,islss,getilist,getbmat,eptover
      character*4 name
      character*10 form
      character*200 ligne
      integer nclas
      integer comma,left,right,rparen,lparen,equal,eol,mactop
      integer iadr, sadr
      data comma/52/
      data left/54/,right/55/,rparen/42/,lparen/41/,equal/50/
      data eol/99/,nclas/29/
      data percen/56/,under/36/,pchar/25/
      save ligne

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      islss=.false.
      lineln=lct(5)
      mode=lct(6)
      ndgt=lct(7)

      if(rstk(pt).eq.1101) goto 96
      lkeep0=lstk(lk)
      call putid(dname,id)
c     
      if (lct(1) .lt. 0) then
         lk=0
         return
      endif
c     
      if(id(1).ne.0) call prntid(dname,-1,lunit)
 01   nlist=0
C     topk : free stack zone for working areas 
      topk=top+1
      lstk(topk+1)=lstk(topk)
      li2=1
      lkeep=lstk(lk)
      itype=gettype(lk)
      call basout(io,lunit,' ')
      mactop=0
      if (abs(itype).eq.11.or.abs(itype).eq.13) mactop=1
c     


 05   goto (20,10,75,55,25,26,75,75,75,30,60,75,60,70,40,40,40)
     $     ,abs(itype)
      goto 75
c     
c     ----polynomial matrices
 10   continue
      if (lunit.eq.wte.and.intexmacs().ne.0) goto 75
      ilog=getpoly("print",lk,lk,it,m,n,name,namel,ilp,lr,lc)
C     working area (see dmpdsp)
      iwl = istk(ilp + m*n)-istk(ilp) + m*n+1
      if (.not.crewimat("print",topk,1,iwl,lw)) return
      if(it.eq.0) then 
         call dmpdsp(stk(lr+istk(ilp)),istk(ilp),m,m,n,name,
     &        namel,ndgt,mode,lineln,lunit,buf,istk(lw))
      else
         call msgs(34,lunit)
         call basout(io,lunit,' ')
         if(io.eq.-1) goto 99
         call dmpdsp(stk(lr+istk(ilp)),istk(ilp),m,m,n,name,
     &        namel,ndgt,mode, lineln,lunit,buf,istk(lw))
         call msgs(35,lunit)
         call basout(io,lunit,' ')
         if(io.eq.-1) goto 99
         call dmpdsp(stk(lc+istk(ilp)),istk(ilp),m,m,n,name,
     &        namel,ndgt,mode,lineln,lunit,buf,istk(lw))
      endif
      goto 48
c     
c     -------scalar matrices 
 20   continue
      if (lunit.eq.wte.and.intexmacs().ne.0) goto 75
      ilog=getmat("print",lk,lk,it,m,n,lr,lc)
C     working area 
      if (.not.crewimat("print",topk,1,m*n+2*n,lw)) return
      if(m*n.eq.0) then
         call basout(io,lunit,'     []')
         goto 48
      endif
      if(it.eq.0) then 
         call dmdsp(stk(lr),m,m,n,ndgt,mode,lineln,lunit,buf,istk(lw))
      else 
         call wmdsp(stk(lr),stk(lc),m,m,n,ndgt,mode,lineln,lunit,
     &        buf,istk(lw))
      endif
      goto 48 
c     -------sparse scalar matrices 
 25   il=iadr(lstk(lk))
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      ne=istk(il+4)
      irc=il+5
      lr=sadr(irc+m+ne)
      if(it.eq.0) then
         call dspdsp(ne,istk(irc),stk(lr),m,n,ndgt,mode,
     $        lineln,lunit,buf)
      else
         call wspdsp(ne,istk(irc),stk(lr),stk(lr+ne),m,n,ndgt,mode,
     $        lineln,lunit,buf)
      endif
      goto 48 
c     -------sparse boolean matrices 
 26   il=iadr(lstk(lk))
      m=istk(il+1)
      n=istk(il+2)
      ne=istk(il+4)
      irc=il+5
      call lspdsp(ne,istk(irc),m,n,ndgt,mode,
     $     lineln,lunit,buf)
      goto 48 
c     -------matrices of string 
 30   continue
      if (lunit.eq.wte.and.intexmacs().ne.0) goto 75
      ilog=getsimat("print",lk,lk,m,n,1,1,lr,nlr)
C     working area 
      if (.not.crewimat("print",topk,1,n,lw)) return
      call strdsp(istk(lr),istk(lr-m*n-1),m,n,lineln,lunit,istk(lw),buf)
      goto 48
c     -------matrices of handle 
 35   ilog=gethmat("print",lk,lk,m,n,lr)
      if (.not.crewimat("print",topk,1,m*n+2*n,lw)) return
      call dmdsp(stk(lr),m,m,n,ndgt,mode,lineln,lunit,buf,istk(lw))
      goto 48 

c     -------lists 
 40   continue
      itype=gettype(lk)
      call listtype(lk,itypel)
      if (itypel.eq.1) then
         if (lunit.eq.wte.and.intexmacs().ne.0) goto 41
         goto 50
      endif
      islss=itypel.eq.2
      if (islss) then
         if (lunit.eq.wte.and.intexmacs().ne.0) goto 41
         goto 45
      endif

 41   continue
      ilw=iadr(lstk(lk))
      if(istk(ilw).lt.0) ilw=iadr(istk(ilw+1))
      nw=istk(ilw+1)
      llw=sadr(ilw+3+nw)
c     form the requested function name
      call funnam(id1,'p',ilw)

c     look for the function 
 42   fin=0
      tops=top
      top=topk
      llk=lstk(lk)
      lstk(lk)=lkeep0
      call funs(id1)
      if(err.gt.0) return
      lstk(lk)=llk
      if(fun.gt.0) then
         top=tops
         goto 43
      endif
      if(fin.eq.0) then
         top=tops
         if(itype.eq.15) goto 45
         call setfunnam(id1,'%l_p',4)
         itype=15
         goto 42
      endif
      fin=-1
      call stackg(id1)
      if(fin.eq.0) then
         goto 45
      endif

c     copy tlist to top of stack
 43   if(lk.ne.top) then
         top=topk
         ilt1=iadr(lstk(top))
         lt1=sadr(ilt1+3+nw)
         err=lt1+istk(ilw+2+nw)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(nw+3,istk(ilw),1,istk(ilt1),1)
         call unsfdcopy(istk(ilw+2+nw)-1,stk(llw),1,stk(lt1),1)
         lstk(top+1)=lt1+istk(ilw+2+nw)-1
      endif
      goto 95
c     
c     check for typed lists 
 45   continue
      ilog=getilist("print",lk,lk,nl,1,ilt)
      illist=lstk(lk)
C     list ( we must deal with recursion ) 
      nlist=nlist+1
      if(nlist.le.1) then
         if(dname(1).ne.0) then
            call cvnamel(dname,ligne,1,li1)
         else
            li1=li2
            ligne(li1:li1)=' '
         endif
      else
         li1=li2
      endif
      ligne(li1+1:li1+1)='('  
      li1=li1+2
      kl=0
 47   continue
      if(nl.eq.0) call basout(io,lunit,'     ()')
 48   if(nlist.le.0) goto 99
      if(lct(1).lt.0) goto 99
      kl=kl+1
      if(kl.gt.nl) goto 49
C     ce qui est dessous est plus qu'etrange getilist fair un mvptr et
c     un ptrback
c     la valeur stockee dans ptkeep est ecrasee au secon appel de mvptr
c     et le secon ptrback 
c     ne retourne pas la premiere valeur sauvee mais la seconde..... et
c     pourtant ca marche ?????
      call mvptr(topk,illist)
      ilog=getilist("print",topk,topk,nl,kl,ilk)
      il=iadr(lstk(topk))
      siz=istk(il+2+kl)-istk(il+2+(kl-1))
      call ptrback(topk)
      lstk(lk)=ilk
      itype=gettype(lk)
C     if the argument is rational list we must not treat it as a list 
      if (itype.eq.15.or.itype.eq.16.or.itype.eq.17) then 
c     -compat the itype.eq.15 is retained for compatibility
         call listtype(lk,itypel)
         if (itypel.ne.1) then 
            if (.not.clsave(topk,illist,kl,li1,nl)) goto 99
         endif
      endif
      fl=int(log10(real(kl+0.1)))+1
      write(form,'(''(i'',i3,'')'')') fl
      if(li1+fl.gt.200) then
         call error(109)
         return
      endif
      write(ligne(li1:li1+fl-1),form) kl
      li2=li1+fl-1
      buf(1:nlist+6)=' '
      call basout(io,lunit,' ')
      if(io.eq.-1) goto 99
      if(islss) then
         if(kl.eq.1) then
            call basout(io,lunit,buf(1:nlist+6)//ligne(1:li2)//
     +           ')   (state-space system:)')
c     ligne(1:li2)=' '
            call basout(io,lunit,' ')
         endif
         if(kl.eq.2) then
            call basout(io,lunit,buf(1:nlist+6)//ligne(1:li2)//
     +           ') = A matrix = ')
            call basout(io,lunit,' ')
         endif
         if(kl.eq.3) then
            call basout(io,lunit,buf(1:nlist+6)//ligne(1:li2)//
     +           ') = B matrix = ')
            call basout(io,lunit,' ')
         endif
         if(kl.eq.4) then
            call basout(io,lunit,buf(1:nlist+6)//ligne(1:li2)//
     +           ') = C matrix = ')
            call basout(io,lunit,' ')
         endif
         if(kl.eq.5) then
            call basout(io,lunit,buf(1:nlist+6)//ligne(1:li2)//
     +           ') = D matrix = ')
            call basout(io,lunit,' ')
         endif
         if(kl.eq.6) then
            call basout(io,lunit,buf(1:nlist+6)//ligne(1:li2)//
     +           ') = X0 (initial state) = ')
            call basout(io,lunit,' ')
         endif
         if(kl.eq.7) then
            call basout(io,lunit,buf(1:nlist+6)//ligne(1:li2)//
     +           ') = Time domain = ')
            call basout(io,lunit,' ')
         endif
      else
         ligne(li2+1:li2+1)=')'
         li2=li2+1
         call basout(io,lunit,buf(1:nlist+6)//ligne(1:li2))
         call basout(io,lunit,' ')
      endif
      if(io.eq.-1) goto 99
      if(siz.le.0) then
         call basout(io,lunit,'    Undefined')
         goto 48
      endif
      goto 05
c     end for list 
 49   continue
      nlist=nlist-1
      if(nlist.le.0) goto 99
      call clrest(topk,illist,kl,li1,nl)
      goto 47
c     
c     -----fractions rationnelles <=> list('r',matpoly,matpoly)
 50   continue
      if (lunit.eq.wte.and.intexmacs().ne.0) goto 75
c     Numerateur ( 2ieme elt de la liste )
      ilog=getilist("print",lk,lk,nel,2,iln)
c     ---local change of lstk(topk)
      call mvptr(topk,iln)
      ivtn=gettype(topk)
      typer=.false.
      if ( ivtn.eq.1) then 
         ilog=getmat("print",topk,topk,itnum,m,n,lrn,lcn)
         lrn=lrn-1
         lcn=lcn-1
      elseif ( ivtn.eq.2) then 
         ilog=getpoly("print",topk,topk,itnum,m,n,name,
     $        namel,ilpn,lrn,lcn)
      else 
         typer=.true.
      endif 
c     ---back to proper value 
      call ptrback(topk)
c     denominateur (3ieme elt )
      ilog=getilist("print",lk,lk,nel,3,ild)
      call mvptr(topk,ild)
      ivtd=gettype(topk)
      if ( ivtd.eq.1) then 
         ilog=getmat("print",topk,topk,itden,m1,n1,lrd,lcd)
         lrd=lrd-1
         lcd=lcd-1
      elseif ( ivtd.eq.2) then 
         ilog=getpoly("print",topk,topk,itden,m1,n1,name,
     $        namel,ilpd,lrd,lcd)
      else 
         typer=.true.
      endif 
      call ptrback(topk)
      if(itnum.ne.0.or.itden.ne.0) goto 41

C     --wrong type argument or not same size 
      if (typer.or.m1.ne.m.or.n1.ne.n) then 
         call cvname(dname,buf(1:nlgh),1)
         call error(103)
         return 
      endif 
C     if num or den are scalar matrix we fill a working array 
C     with 1:m*n+1 (by creating a bmat),in which we can store integers
C     it's a trick to give a proper argument 
C     to dmrdsp which will treat the scalar matrix as a polynomial 
C     matrix of degre 0 
      if ( ivtn.eq.1.or.ivtd.eq.1) then 
         if (.not.crewimat("print",topk,1,m*n+1,idb)) return
         do 51 k=0,m*n
            istk(idb+k) =k+1
 51      continue 
         if (ivtn.eq.1) ilpn=idb
         if (ivtd.eq.1) ilpd=idb
      endif 
c     first working area in stk(lw)
C     of requested size iws 
      iws=n*(4+m)+1+istk(ilpn+n*m)+istk(ilpd+n*m)
      if ( ivtn.eq.1.or.ivtd.eq.1) then 
         if (.not.crewimat("print",topk+1,1,iws,lw)) return 
      else
         if (.not.crewimat("print",topk,1,iws,lw)) return 
      endif
      call dmrdsp(stk(lrn+istk(ilpn)),istk(ilpn),stk(lrd+istk(ilpd)),
     $     istk(ilpd),m,m,n,name,
     $     namel,ndgt,mode,lineln,lunit,buf, istk(lw))
      goto 48
c     
c     -----------boolean matrix
 55   continue
      if (lunit.eq.wte.and.intexmacs().ne.0) goto 75
      ilog= getbmat("print",lk,lk,m,n,lr)
      if(m*n.eq.0) then
         call basout(io,lunit,'     []')
      else
         call dldsp(istk(lr),m,m,n,lineln,lunit,buf)
      endif
      goto 48
c     
c     ------------macros---- a changer 
 60   continue
      il=iadr(lstk(lk))
      l=istk(il+1)
      if(istk(il).lt.0) il=iadr(lstk(l))
      ilm=il
      l=1
      is1=left
      is2=right
      do 64 i=1,2
         n=istk(il+1)
         il=il+1
         buf(l:l)=alfa(is1+1)
         l=l+1
         if (n.ne.0) then
            do 63 j=1,n
               call cvnamel(istk(il+1),buf(l:),1,lname)
               l=l+lname
               buf(l:l)=alfa(comma+1)
               l=l+1
               il=il+nsiz
 63         continue
            l=l-1
         endif	
         buf(l:l)=alfa(is2+1)
         l=l+1
         if(i.eq.2) goto 64
         buf(l:l)=alfa(equal+1)
         l=l+1
         if(mactop.eq.1) then
            call cvnamel(idstk(1,lk),buf(l:),1,lname)
            l=l+lname
         else
            buf(l:)='function'
            l=l + 8
         endif
         is1=lparen
         is2=rparen
 64   continue
      il=il+1
      l=l-1
      call basout(io,wte,buf(1:l))
      if(io.eq.-1) goto 99
      n=istk(il)
      il=il+1
C     cas ou l'object macro est au top ( pas ds une liste )
      if (mactop.eq.1) then 
         isncf=1
         if (isncf.eq.1) goto 68
         l=il
 65      if(istk(l).eq.eol) goto 66
         l=l+1
         goto 65
 66      if(istk(l+1).eq.eol) goto 68
         n=l-il
         nl=lct(5)
         do 67 i1=1,n,nl
            i2=min(n,i1+nl-1)-i1+1
            call cvstr(i2,istk(il+i1-1),buf,1)
            call basout(io,wte,buf(1:i2))
            if(io.eq.-1) goto 99
 67      continue
         il=l+1
         l=il
         goto 65
      endif
 68   il=ilm
      goto 48

c     ------------library-- a changer aussi 
C     [14,n,codagedupath(n),nombre-de-nom,nclas+1 cases,suite des noms]
 70   illib=iadr(lstk(lk))
      n=istk(illib+1)
      illib=illib+2
      call cvstr(n,istk(illib),buf,1)
      call msgs(24,n)
      illib=illib+n
      n=istk(illib)
      illib=illib+nclas+2
      call prntid(istk(illib),n,lunit)
      goto 48
c     
c     ----------- other variables type (overloaded)
 75   continue
c     form the requested function name %%%%
      namef(1)=percen
      call typ2cod(iadr(lstk(lk)),namef(2),nlt)
      if(nlt.eq.0) then
         call msgs(100,0)
         goto 48
      endif
      namef(2+nlt)=under
      namef(3+nlt)=pchar
      call namstr(id1,namef,nlt+3,0)
c     look for the function 
      fin=0
      tops=top
      top=topk
      llk=lstk(lk)
      lstk(lk)=lkeep0
      call funs(id1)
      if(err.gt.0) return

      lstk(lk)=llk
      if(fun.gt.0) then
         top=tops
         goto 76
      endif

      if(fin.eq.0) then
         top=tops
         goto 48
      endif

      fin=-1

      call stackg(id1)
      if(fin.eq.0) then
         goto 48
      endif


c     copy variable  to top of stack
 76   if(lk.ne.top) then
         top=topk
         vol=lstk(lk+1)-lstk(lk)
         call unsfdcopy(vol,stk(lstk(lk)),1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+vol
      endif
      goto 95

c     -----------end

c     overloaded print
 95   continue
c     preserve data for recursion

      if ( eptover(3,psiz)) return
      rstk(pt-2)= 0
      call putid(ids(1,pt-2),dname)
      rstk(pt-1)= 0
      ids(1,pt-1)= lhs
      ids(2,pt-1)= rhs
      ids(3,pt-1)= lstk(lk)
      ids(4,pt-1)= li1
      ids(5,pt-1)= lkeep0
      lstk(lk)   = lkeep
      pstk(pt)   = lk
      ids(1,pt)  = nlist
      ids(2,pt)  = kl
      ids(3,pt)  = nl
      ids(4,pt)  = illist
      ids(5,pt)  = topk
      ids(6,pt)  = tops
      rstk(pt)   = 1101
      lhs=1
      rhs=1
      if(fun.eq.0) then
         fin=lstk(fin)
         icall=5
c     *call* macro
         return
      else
c     *call* matfn
         return
      endif

 96   continue
c     set back preserved data
      call putid(dname,ids(1,pt-2))
      lhs     = ids(1,pt-1)
      rhs     = ids(2,pt-1)
      lk      = pstk(pt)
      lkeep   = lstk(lk)
      lstk(lk)= ids(3,pt-1)
      li1     = ids(4,pt-1)
      lkeep0  = ids(5,pt-1)
      nlist   = ids(1,pt)
      kl      = ids(2,pt)
      nl      = ids(3,pt)
      illist  = ids(4,pt)
      topk    = ids(5,pt)
      top     = ids(6,pt)
      pt=pt-3
      goto 48


 99   continue
      lstk(lk)=lkeep
      lk=0
      return
      end

      logical function clsave(topk,il,kl,li1,nl)
      include 'stack.h'   
      logical crewimat 
      integer topk,il,kl,li1
      clsave=.false.
      if (.not.crewimat("print",topk,1,4,lr)) return     
      clsave=.true.
      istk(lr)=il
      istk(lr+1)=kl
      istk(lr+2)=li1
      istk(lr+3)=nl
      topk=topk+1
      return
      end

      subroutine clrest(topk,il,kl,li1,nl)
      include 'stack.h'   
      integer topk,il,kl,li1
      logical getwimat 
      topk=topk-1
      if (.not.getwimat("print",topk,topk,m,n,lr)) return     
      il=istk(lr)
      kl=istk(lr+1)
      li1=istk(lr+2)
      nl=istk(lr+3)
      return
      end

      subroutine listtype(lk,itype)
      include 'stack.h'
c     implicit undefined (a-z)
      integer rat,gettype
      logical ilog,getilist,getsmat
c     return itype=0 for list, itype=1 for rat, itype=2 for lss
c     check for typed lists 
      integer nl,ilt,itype,topk,lk,mt,nt,ilc,nlr
      data rat/27/
c     
      itype=0
      topk=lk
      if(gettype(lk).eq.15) return
      ilog=getilist("print",lk,lk,nl,1,ilt)
      call mvptr(topk,ilt)
      if (ilt.ne.0) then
         if ( gettype(topk).eq.10) then 
            ilog=getsmat("print",topk,topk,mt,nt,1,1,ilc,nlr)
            if (nlr.eq.1.and.istk(ilc).eq.rat) then 
               itype=1
               goto  999
            endif
            if (istk(ilc).eq.21.and.istk(ilc+1).eq.28.
     +           and.istk(ilc+2).eq.28) then 
               itype=2
               goto 999
            endif
         endif
      endif
 999  call ptrback(topk)
      return
      end

