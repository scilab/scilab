c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine tradsl(ilfun,funnam,illist,nlist)
c     
c!but 
c     convert compiled macros to lst structure
c!calling sequence
c     subroutine tradsl(ilfun,id,illist,nlist)
c     kmac  : variable number of the compiled macro in the scilab stack
c!    
c
c
      include 'stack.h'
c     
      integer nops,funnam(nsiz)
      parameter (nops=32)
      character*40 strg
      character*40 form
      double precision x,xx
      integer op,ix(2),fptr
      equivalence (x,ix(1))
c

      integer id(nsiz),iadr,sadr,cmplxt,pt0
c
      integer for(3),while(5),iff(2),sel(6),try(3)
      integer ops(nops)
      external getendian
      integer getendian

      data for/15,24,27/
      data iff/18,15/
      data while/32,17,18,21,14/
      data sel/28,14,21,14,12,29/
      data try/29,27,34/
c                            +  -  * .*  *. .*.  / ./  /. ./.  
      data (ops(i),i=1,10) /45,46,47,98,200,149,48,99,201,150/
c                            \ .\   \. .\. ** =  <  >  <=  >=  <>
      data (ops(i),i=11,21)/49,100,202,151,62,50,59,60,109,110,119/
c                             : [,] ins ext '  [;]  | &   ~  .^  .'
      data (ops(i),i=22,nops)/44,01, 02 ,03,53,04, 57,58, 61,113,104/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     mode debug

      if (ddt .eq. 4) then
         write(buf(1:8),'(2i4)') pt,rstk(pt)
         call basout(io,wte,' tradsl pt:'//buf(1:4)//' rstk(pt):'
     &        //buf(5:8))
      endif
c
c
      pt0=pt
      pt=pt+1
      if(pt.gt.psiz) then
         call error(26)
         return
      endif
      rstk(pt)=0
c

      lc=ilfun
      ilk=lc

c
c     on scrute une premiere fois l'ensemble de la macro pour determiner
c     sa complexite
c
      lc = lc +1
      lc = lc + nsiz*istk(lc) + 1
      lc = lc + nsiz*istk(lc) + 1
      long = istk(lc)
      lc = lc+1
      icount=cmplxt(istk(lc),long) 
      if(icount.lt.0) then
         pt=pt0
         call error(37)
         return
      endif

c
c     premiers elements de la liste resultat
c
      ilr=illist
      istk(ilr)=15
      istk(ilr+1)=icount+3
      istk(ilr+2)=1
      lr=sadr(ilr+3+icount+3)
      ilr=ilr+3
c
c     nom de la macro
      il=iadr(lr)
      call basnms(funnam,1,istk(il),ni)
      l=sadr(il+ni)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1

c     nom des parametres de sortie
      lc=ilk+1
      nc=istk(lc)
      il=iadr(lr)
      call basnms(istk(lc+1),nc,istk(il),ni)
      l=sadr(il+ni)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1

c     nom des parametres d'entree
      lc=lc+nsiz*nc+1
      nc=istk(lc)
      il=iadr(lr)
      call basnms(istk(lc+1),nc,istk(il),ni)
      l=sadr(il+ni)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+nsiz*nc+1
c
c
      l0=0
      nc=0
C     pointeur sur le tableau de travail : pointe sur la premiere adresse libre
c     
c     debut du texte proprement dit de la macro
c
C     lc : pointeur sur l'instruction de la macro en cours de traduction
c     lr : pointeur dans stk sur l'element courant de la liste courante
c     ilr: pointeur dans istk sur la position de  l'element courant dans
c          la liste courante
C     
      Lc = lc+1
      lcf=lc+long-1
c     
c     nouvelle 'operation'
 10   continue
      if(lc.gt.lcf) then
         goto(11,11,11,46,47,11,51,54,55,11,61,63,64,65,11,
     $        121,122),rstk(pt)
 11      nlist=iadr(lr)-illist
         pt=pt0
         return
      endif
      op=istk(lc)
c     
      if(ddt.lt.-1) then
c						replaces write(6 ...) by basout bug 2598                  
c     write(6,'(i7)') op
        write(buf(1:7),'(i7)') op
        call basout(io,wte,buf(1:7))
      endif
      
      goto(20,25,40,42,30,41,45,50,50,60,
     &     120,90,90,90,90,100,90,101,102,90,
     &     103,104,105,106,107,108,110,90,115,116,
     &     117) ,op
c     
c     
c     matfns
      if(op.ge.100)   goto 80
      if(op.eq.0) then
c     "deleted op"
         lc=lc+istk(lc+1)
         goto 10
      endif
c     
 12   if(op.ne.99) then
         call error(44)
         return
      endif
c
c     return
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=1
      l=il+6
c     type 99
      istk(l)=9
      istk(l+1)=9
      istk(il+5)=3
      l=l+2
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+1
      goto 10
c     
c     
 15   continue
      call error(60)
      return
c 
c     stackp
c     
c     affectation d'une valeur a une variable
c     de par le fonctionnement de la pile , la valeur a affecter est 
c     en premiere  position
c
 20   continue
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=2
      istk(il+3)=0
      istk(il+4)=1
      l=il+7
c     type 1
      istk(l)=1
      istk(il+5)=2
      l=l+1
c     nom de la variable
      call namstr(istk(lc+1),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+1+nsiz
      goto 10
c     
c     stackg
c
 25   continue
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=4
      istk(il+3)=0
      istk(il+4)=1
      l=il+9
c     type 2
      istk(l)=2
      istk(il+5)=2
      l=l+1
c     nom de la variable
      call namstr(istk(lc+1),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c     rhs
      call intstr(istk(lc+nsiz+1),istk(l),ni,1)
      istk(il+7)=istk(il+6)+ni
      l=l+ni
c     fin
      call intstr(istk(lc+nsiz+2),istk(l),ni,1)
      istk(il+8)=istk(il+7)+ni
      l=l+ni
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+nsiz+3
      goto 10
c     
c     allops => operation sur les operandes de la pile
c     
 30   continue
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=4
      istk(il+3)=0
      istk(il+4)=1
      l=il+9
c     type 5
      istk(l)=5
      istk(il+5)=2
      l=l+1
c     op
      iop=istk(lc+1)
      ii=0
 31   ii=ii+1
      if(ii.gt.nops) then
         buf='Unmanaged operation'
         call error(999)
         return
      endif
      if(ops(ii).ne.iop) goto 31
      call intstr(ii,istk(l),ni,1)
c      call intstr(istk(lc+1),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c     rhs
      call intstr(istk(lc+2),istk(l),ni,1)
      istk(il+7)=istk(il+6)+ni
      l=l+ni
c     lhs
      call intstr(istk(lc+3),istk(l),ni,1)
      istk(il+8)=istk(il+7)+ni
      l=l+ni
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+4
      goto 10
c     
c     string
c     
 40   continue
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=2
      istk(il+3)=0
      istk(il+4)=1
      l=il+7
c     type 3
      istk(l)=3
      istk(il+5)=2
      l=l+1
c     str
      ni=istk(lc+1)
      call icopy(ni,istk(lc+2),1,istk(l),1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c      
      lc=lc+2+ni
      goto 10
c     
c     num
c     
 41   continue
      il=iadr(lr)
      if(getendian().eq.1) then
         ix(1)=istk(lc+1)
         ix(2)=istk(lc+2)
      else
         ix(2)=istk(lc+1)
         ix(1)=istk(lc+2)
      endif


      maxc=18
      isign=1
      if(x.lt.0)  isign=-1
      call fmt(abs(x),maxc,ifmt,n1,n2)
      if(ifmt.eq.-1) then
c     Inf
         ifl=3
         strg='Inf'
      elseif(ifmt.eq.-2) then
c     Nan
         ifl=3
         strg='Nan'
      elseif(ifmt.eq.1) then
         nf=1
         ifl=maxc
         n2=1
         if(abs(x).ge.1.d100.or.abs(x).lt.1.d-99) then
            ie=int(log10(abs(x)))
            if(ie.lt.0) ie=ie-1
            xx=x/(10.0d0**ie)
            nf=2
            write(form,200) maxc,15
            write(strg,form) xx
            ls=lnblnk(strg)
            if(abs(x).lt.1.d-99) then
               write(strg(ls+1:),'(''D'',i4)') ie
            else
               write(strg(ls+1:),'(''D'',i3)') ie
            endif
         else
            write(form,201) maxc,maxc-7
            write(strg,form) x
         endif
      elseif(ifmt.ge.0) then
         nf=2
         write(form,200) n1,n2
         write(strg,form) x
      endif
      i1=0
 410  i1=i1+1
      if(strg(i1:i1).eq.' ') goto 410

      i2=lnblnk(strg)+1
      if(ifmt.ge.0.and.ifmt.ne.1) then
 420     i2=i2-1
         if(i2.gt.2) then
            if(strg(i2:i2).eq.'0') goto 420
         endif
         if(strg(i2:i2).eq.'.') i2=i2-1
      else
         i2=i2-1
      endif

      istk(il)=10
      istk(il+1)=1
      istk(il+2)=2
      istk(il+3)=0
      istk(il+4)=1
      l=il+7
c     type 6
      istk(l)=6
      istk(il+5)=2
      l=l+1
c     nom de la variable
      ni=i2-i1+1
      call cvstr(ni,istk(l),strg(i1:i2),0)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+3
      goto 10
c     
c matrice vide
c
 42   continue
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=1
      l=il+6
c     type 4
      istk(l)=4
      istk(il+5)=2
      l=l+1
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+1
      goto 10

c     
c     for
c     
 45   continue
      if(ddt.le.-1) then
c      write(6,'(a10,5i5)') 'for',pt,lcf,ilr,lr,l-lr
       write(buf,'(a10,5i5)') 'for',pt,lcf,ilr,lr,l-lr
       call basout(io,wte,buf)
      endif
      il=iadr(lr)
c     on preserve les pointeurs de la liste courante
      pt=pt+1
      rstk(pt)=3
      ids(1,pt)=ilr
      ids(2,pt)=lr
      istk(ilr)=lcf
c
c     on cree les premiers elements de la liste "for"
      istk(il)=15
      istk(il+1)=3
      istk(il+2)=1
      lr=sadr(il+6)
      ilr=il+3
c
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=2
      istk(il+3)=0
      istk(il+4)=1
      l=il+7
c     type for
      call icopy(3,for,1,istk(l),1)
      istk(il+5)=istk(il+4)+3
      l=l+3
c     variable de boucle
      ni=istk(lc+1)
      li=lc+2+ni
      call namstr(istk(li+1),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l
      lc=lc+1
c
c     expression de boucle
c
      il=iadr(lr)

c     ---- on preserve les pointeurs  de la liste "for" 
      pt=pt+1
      if(pt.gt.psiz) then
         call error(26)
         return
      endif
      rstk(pt)=4
      ids(1,pt)=ilr
      ids(2,pt)=lr
      long=istk(lc)
      lcf=lc+long
      lc=lc+1
c     ----   ----   ----   ----   ----  --
c     on cree la liste "code boucle"
      ni=cmplxt(istk(lc),long)
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      l=lr
      ilr=il+3
c
      goto 10
 46   continue
c     fin code boucle
c     on rappelle les pointeurs de la liste "for"
      ilr=ids(1,pt)
      lr=ids(2,pt)
c     on complete la liste for
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l
c     code du for
c     ---- on preserve les pointeurs  de la liste "for" 
      rstk(pt)=5
      ids(1,pt)=ilr
      ids(2,pt)=lr
      long=istk(lc)
      lc=lc+1+nsiz
c     on cree la liste "for code "
      ni=cmplxt(istk(lc),long)
      lcf=lc+long-1
      il=iadr(lr)
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      ilr=il+3
c     on preserve les pointeurs de la liste "for code"
      goto 10
 47   continue
c     fin du code du for
c     fin du for
c     on recharge les pointeurs de la liste "for"
      ilr=ids(1,pt)
      lr=ids(2,pt)
      pt=pt-1
      lcf=istk(ilr)
c     on complete la liste "for"
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c     on recharge les pointeurs de la liste courante
      ilr=ids(1,pt)
      lr=ids(2,pt)
      lcf=istk(ilr)
c     on complete la liste "courante"
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
      pt=pt-1
      if(ddt.le.-1) then
c				replaces write(6 ...) by basout bug 2598                  
c       write(6,'(a10,5i5)') 'fin for',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'fin for',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
      goto 10
c
c     if - while
 50   continue
      if(istk(lc+1).lt.0) goto 60
c     ce qui suit (jusqu'a l'etiquette 60 correspond a la traduction 
c     des if/while de  la version 3 et assure la compatibilite
      if(ddt.le.-1) then
c					replaces write(6 ...) by basout bug 2598                  
c          write(6,'(a10,5i5)') 'if',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'if',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
      il=iadr(lr)
c     on preserve les pointeurs de la liste courante
      pt=pt+1
      rstk(pt)=6
      ids(1,pt)=ilr
      ids(2,pt)=lr
      istk(ilr)=lcf
      lc0=lc
c
c     on cree les premiers elements de la liste "if" ou "while"
      istk(il)=15
      istk(il+1)=4
      istk(il+2)=1
      lr=sadr(il+7)
      ilr=il+3
      istk(ilr+3)=lc0
c
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=1
      l=il+6
c     type if ou while
      if(op.eq.8) then
         call icopy(2,iff,1,istk(l),1)
         istk(il+5)=istk(il+4)+2
         l=l+2
      else
         call icopy(5,while,1,istk(l),1)
         istk(il+5)=istk(il+4)+5
         l=l+5
      endif
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l
      lc=lc+5
c
c     expression logique
c
      il=iadr(lr)

c     ---- on preserve les pointeurs  de la liste "if" ou "while"
      pt=pt+1
      if(pt.gt.psiz) then
         call error(26)
         return
      endif
      rstk(pt)=7
      ids(1,pt)=ilr
      ids(2,pt)=lr
      istk(ilr)=lc
      long=istk(lc0+2)
      lcf=lc+long-1

c     ----   ----   ----   ----   ----  --
c     on cree la liste "code expression logique"
      ni=cmplxt(istk(lc),long)+1
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      ilr=il+3
c
      goto 10
 51   continue
c     fin "code expression logique"
c     on ajoute le code de l'op de comparaison
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=4
      istk(il+3)=0
      istk(il+4)=1
      l=il+9
c     type 5
      istk(l)=5
      istk(il+5)=2
      l=l+1
c     op
      ii=0
 52   ii=ii+1
      if(ii.gt.nops) then
         buf='Unmanaged operation'
         call error(999)
         return
      endif
      if(ops(ii).ne.istk(lc0+1)) goto 52
      call intstr(ii,istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c     rhs
      istk(l)=2
      istk(il+7)=istk(il+6)+1
      l=l+1
c     lhs
      istk(l)=1
      istk(il+8)=istk(il+7)+1
      l=l+1
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
c     on rappelle les pointeurs de la liste "if" ou "while"
      ilr=ids(1,pt)
      lr=ids(2,pt)
c     on complete la liste "if" ou "while"
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l
c     code du then
c     ---- on preserve les pointeurs  de la liste "if" ou "while"
      rstk(pt)=8
      ids(1,pt)=ilr
      ids(2,pt)=lr
c     on cree la liste "then"
      long=istk(lc0+3)
      ni=cmplxt(istk(lc),long)
      lcf=lc+long-1
      il=iadr(lr)
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      l=lr
      ilr=il+3
      goto 10
 54   continue
c     fin du code du then
c     on rappele les pointeur de la liste "if" ou "while"
      ilr=ids(1,pt)
      lr=ids(2,pt)
c     on complete la liste  "if" ou "while"
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l
c     code du else
c     ---- on preserve les pointeurs  de la liste "if" ou "while"
      if(ddt.le.-1) then
c				replaces write(6 ...) by basout bug 2598                  
c        write(6,'(a10,5i5)') 'else',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'else',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
      rstk(pt)=9
      ids(1,pt)=ilr
      ids(2,pt)=lr
      lc0=istk(ilr)
      long=istk(lc0+4)
c     on cree la liste "else"
      ni=cmplxt(istk(lc),long)
      lcf=lc+long-1
      il=iadr(lr)
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      l=lr
      ilr=il+3
      goto 10
 55   continue
c     fin du else
c     on recharge les pointeurs de la liste "if" ou "while"
      ilr=ids(1,pt)
      lr=ids(2,pt)
      pt=pt-1
c     on complete la liste "if" ou "while"
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
      if(ddt.le.-1) then
c			 replaces write(6 ...) by basout bug 2598                  
c      write(6,'(a10,5i5)') 'fin else',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'fin else',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
c     on recharge les pointeurs de la liste courante
      ilr=ids(1,pt)
      lr=ids(2,pt)
      lcf=istk(ilr)
c     on complete la liste "courante"
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
      pt=pt-1
      if(ddt.le.-1) then
c			 replaces write(6 ...) by basout bug 2598                  
c      write(6,'(a10,5i5)') 'fin if',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'fin if',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
      goto 10
c
c     select - case
 60   continue
      if(ddt.le.-1) then
c					replaces write(6 ...) by basout bug 2598                  
c          write(6,'(a10,5i5)') 'sel',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'sel',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
      il=iadr(lr)
c     on preserve les pointeurs de la liste courante
      pt=pt+1
      rstk(pt)=10
      ids(1,pt)=ilr
      ids(2,pt)=lr
      istk(ilr)=lcf
      lc0=lc
c
c     on cree les premiers elements de la liste  "select case"
      ncase=istk(lc0+2)
      istk(il)=15
      if(istk(lc).eq.10) then
         istk(il+1)=2*ncase+3
      else
         istk(il+1)=2*ncase+2
      endif
      istk(il+2)=1
      ilr=il+3
      lr=sadr(ilr+istk(il+1))
c
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=1
      l=il+6
      if(istk(lc).eq.8) then
c     type if
         call icopy(2,iff,1,istk(l),1)
         istk(il+5)=istk(il+4)+2
         l=l+2
      elseif(istk(lc).eq.9) then
c     type while
         call icopy(5,while,1,istk(l),1)
         istk(il+5)=istk(il+4)+5
         l=l+5
      elseif(istk(lc).eq.10) then
c     type select
         call icopy(6,sel,1,istk(l),1)
         istk(il+5)=istk(il+4)+6
         l=l+6
      endif
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l
      lc=lc+4
c
c     premiere expression 
c
      il=iadr(lr)

c     ---- on preserve les pointeurs  de la liste  "select case"
      pt=pt+1
      if(pt.gt.psiz) then
         call error(26)
         return
      endif
      rstk(pt)=11
      ids(1,pt)=ilr
      ids(2,pt)=lr
      istk(ilr)=lc
      long=istk(lc0+3)
      lcf=lc+long-1

c     ----   ----   ----   ----   ----  --
      icase=ncase+1
      if(istk(lc0).ne.10) goto 62
c
c     on cree la liste "code premiere expression"
      ni=cmplxt(istk(lc),long)
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      ilr=il+3
c
      goto 10
 61   continue
c     fin "code premiere expression"
c     on rappelle les pointeurs de la liste  "select case"
      ilr=ids(1,pt)
      lr=ids(2,pt)
c     on complete la liste  "select case"
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l
c
c
 62   continue
c
      if(ddt.le.-1) then
c			 replaces write(6 ...) by basout bug 2598                  
c      write(6,'(a10,5i5)') 'case',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'case',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
      icase=icase-1
      istk(ilr+1)=icase
c
c     code de l'expression du case
c     ---- on preserve les pointeurs  de la liste  "select case"
      rstk(pt)=12
      ids(1,pt)=ilr
      ids(2,pt)=lr
c     on cree la liste "case"
      long=istk(lc)
      lc=lc+1
      ni=cmplxt(istk(lc),long)
      lcf=lc+long-1
      il=iadr(lr)
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      l=lr
      ilr=il+3
      goto 10
 63   continue
c     fin du code de l'expression du case
c     on rappele les pointeur de la liste  "select case"
      ilr=ids(1,pt)
      lr=ids(2,pt)
c     on complete la liste "select case"
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l


c     code du then 
c     ---- on preserve les pointeurs  de la liste  "select case"
      rstk(pt)=13
      ids(1,pt)=ilr
      ids(2,pt)=lr
      long=istk(lc)
      lc=lc+1
c     on cree la liste "then"
      ni=cmplxt(istk(lc),long)
      lcf=lc+long-1
      il=iadr(lr)
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      l=lr
      ilr=il+3
      goto 10
 64   continue
c     fin du then
c     on recharge les pointeurs de la liste  "select case"
      ilr=ids(1,pt)
      lr=ids(2,pt)
      icase=istk(ilr)
c     on complete la liste  "select case"
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
      if(ddt.le.-1) then
c			 replaces write(6 ...) by basout bug 2598                  
c      write(6,'(a10,5i5)') 'fin case',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'fin case',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
c
      if(icase.gt.1) goto 62
      lc=lc+1
c     code du else
      if(ddt.le.-1) then
c			 replaces write(6 ...) by basout bug 2598                  
c      write(6,'(a10,5i5)') 'else',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'else',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
c     ---- on preserve les pointeurs  de la liste  "select case"
      rstk(pt)=14
      ids(1,pt)=ilr
      ids(2,pt)=lr
      long=istk(lc)
      lc=lc+1
c     on cree la liste "else"
      ni=cmplxt(istk(lc),long)
      if(long.eq.0) lc=lc+1
      lcf=lc+long-1
      il=iadr(lr)
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      l=lr
      ilr=il+3
      goto 10
 65   continue
c     fin du else
c     on recharge les pointeurs de la liste  "select case"
      ilr=ids(1,pt)
      lr=ids(2,pt)
      pt=pt-1
c     on complete la liste  "select case"
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
      if(ddt.le.-1) then
c			 replaces write(6 ...) by basout bug 2598                  
c      write(6,'(a10,5i5)') 'fin else',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'fin else',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
c     on recharge les pointeurs de la liste courante
      ilr=ids(1,pt)
      lr=ids(2,pt)
      lcf=istk(ilr)
c     on complete la liste "courante"
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
      pt=pt-1
      if(ddt.le.-1) then
c			 replaces write(6 ...) by basout bug 2598                  
c      write(6,'(a10,5i5)') 'fin sel',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'fin sel',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
      goto 10
c   
 80   continue
      fptr = op * 10 + istk(lc+3)
c     SCI_HFUNCTIONS_BACKSEARCH = 2  
      call funtab(id,fptr,2,'NULL_NAME',0)

      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=4
      istk(il+3)=0
      istk(il+4)=1
      l=il+9
c     type 20
      istk(l)=2
      istk(l+1)=0
      istk(il+5)=3
      l=l+2
c     nom de la variable
      call namstr(id,istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c     rhs
      call intstr(istk(lc+1),istk(l),ni,1)
      istk(il+7)=istk(il+6)+ni
      l=l+ni
c     lhs
      call intstr(istk(lc+2),istk(l),ni,1)
      istk(il+8)=istk(il+7)+ni
      l=l+ni
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+4
      goto 10
c     
c     pause,break,abort,eol
 90   continue
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=1
      l=il+6
c     type >=12
      call intstr(op,istk(l),nw,0)
      istk(il+5)=3
      l=l+2
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+1
      goto 10
c
 100  continue
c     info sur numero de lignes
      lc=lc+2
      goto 10
c
 101  continue
c     mark named variable
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=2
      istk(il+3)=0
      istk(il+4)=1
      l=il+7
c     type 18
      istk(l)=1
      istk(l+1)=8
      istk(il+5)=3
      l=l+2
c     nom de la variable
      call namstr(istk(lc+1),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c     
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c     
      lc=lc+1+nsiz
      goto 10

 102  continue
c     mkindx
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=3
      istk(il+3)=0
      istk(il+4)=1
      l=il+8
c     type 19
      istk(l)=1
      istk(l+1)=9
      istk(il+5)=3
      l=l+2

c     n
      call intstr(istk(lc+1),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c     m
      call intstr(istk(lc+2),istk(l),ni,1)
      istk(il+7)=istk(il+6)+ni
      l=l+ni
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+3
      goto 10

 103  continue
c     begrhs
      lc=lc+1
      goto 10

 104  continue
c     printmode (ignored)
      lc=lc+2
      goto 10
c
 105  continue
c     name2var
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=2
      istk(il+3)=0
      istk(il+4)=1
      l=il+7
c     type 23
      istk(l)=2
      istk(l+1)=3
      istk(il+5)=3
      l=l+2
c     nom de la variable
      call namstr(istk(lc+1),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c     
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c     
      lc=lc+1+nsiz
      goto 10
c
106   continue
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=1
      l=il+6
c     type 24
      istk(l)=2
      istk(l+1)=4
      istk(il+5)=3
      l=l+2
c
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+1
      goto 10
c
107   continue
c     profile
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=3
      istk(il+3)=0
      istk(il+4)=1
      l=il+8
c     type 25
      istk(l)=2
      istk(l+1)=5
      istk(il+5)=3
      l=l+2
c
c     n
      call intstr(istk(lc+1),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c     m
      call intstr(istk(lc+2),istk(l),ni,1)
      istk(il+7)=istk(il+6)+ni
      l=l+ni

      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+3
      goto 10
c
 108  continue
c     vector of string
      n=istk(lc+1)*istk(lc+2)
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=3+n
      istk(il+3)=0
      istk(il+4)=1
      l=il+5+(3+n)
c     type 26
      istk(l)=2
      istk(l+1)=6
      istk(il+5)=3
      l=l+2
c
c     n
      call intstr(istk(lc+1),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c     m
      call intstr(istk(lc+2),istk(l),ni,1)
      istk(il+7)=istk(il+6)+ni
      l=l+ni

      nc=istk(lc+4+n)-1
      do 109 ii=1,n
         istk(il+7+ii)=istk(il+6+ii)+istk(lc+4+ii)-istk(lc+3+ii)
 109  continue
      call icopy(nc,istk(lc+5+n),1,istk(l),1)
      l=l+nc

      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+5+n+nc
      goto 10

 110  continue
c     varfunptr
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=4
      istk(il+3)=0
      istk(il+4)=1
      l=il+9
c     type 27
      istk(l)=2
      istk(l+1)=7
      istk(il+5)=3
      l=l+2

c     fun
      call intstr(istk(lc+1),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c     fin
      call intstr(istk(lc+2),istk(l),ni,1)
      istk(il+7)=istk(il+6)+ni
      l=l+ni
c
c     nom de la variable
      call namstr(istk(lc+3),istk(l),ni,1)
      istk(il+8)=istk(il+7)+ni
      l=l+ni

      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+3+nsiz
      goto 10
c
 115  continue
c     affectation
      nlhs=istk(lc+1)
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=2+2*nlhs
      istk(il+3)=0
      istk(il+4)=1

      l=il+5+istk(il+2)
c     type 
      call intstr(istk(lc),istk(l),ni,1)
      istk(il+5)=istk(il+4)+ni
      l=l+ni
c     sym
      call intstr(istk(lc+2),istk(l),ni,1)
      istk(il+6)=istk(il+5)+ni
      l=l+ni
c
      li=lc+3
      do i=0,nlhs-1
c     .  namei
         call namstr(istk(li),istk(l),ni,1)
         istk(il+7+2*i)=istk(il+6+2*i)+ni
         l=l+ni
c     .  rhsi
         call intstr(istk(li+nsiz),istk(l),ni,1)
         istk(il+8+2*i)=istk(il+7+2*i)+ni
         l=l+ni

         li=li+nsiz+1
      enddo

      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1

      lc=li
      goto 10
 116  continue
c     skip logical evaluation
      lc=lc+3
      goto 10
 117  continue
c     comment
      n=istk(lc+1)
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=2
      istk(il+3)=0
      istk(il+4)=1
      istk(il+5)=istk(il+4)+2
      istk(il+6)=istk(il+5)+n
      l=il+7
c     type 31
      istk(l)=3
      istk(l+1)=1
      l=l+2
      call icopy(n,istk(lc+2),1,istk(l),1)
      l=l+n
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
c
      lc=lc+2+n
      goto 10
c
 120   continue
c     try/catch
      if(ddt.le.-1) then
c			 replaces write(6 ...) by basout bug 2598                  
c      write(6,'(a10,5i5)') 'try',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'try',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
      il=iadr(lr)
c     on preserve les pointeurs de la liste courante
      pt=pt+1
      rstk(pt)=15
      ids(1,pt)=ilr
      ids(2,pt)=lr
      istk(ilr)=lcf
      lc0=lc
c
c     Initialize the   "try catch" list
c     list('try',try_instructions,catch_instructions)
      istk(il)=15
      istk(il+1)=3
      istk(il+2)=1
      lr=sadr(il+6)
      ilr=il+3

c     adding the string 'try'
      il=iadr(lr)
      istk(il)=10
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=1
      l=il+6
      call icopy(3,try,1,istk(l),1)
      istk(il+5)=istk(il+4)+3
      l=l+3
      l=sadr(l)
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l

c     adding  "try instructions"
      lc=lc+3
      il=iadr(lr)
c     preserve pointers on the   "try catch" list
      pt=pt+1
      if(pt.gt.psiz) then
         call error(26)
         return
      endif
      rstk(pt)=16
      ids(1,pt)=ilr
      ids(2,pt)=lr
      ids(3,pt)=lc0
      istk(ilr)=lc
      long=istk(lc0+1)
      lcf=lc+long-1
      ni=cmplxt(istk(lc),long)
c     Initialize the   "try_instructions" list : list(try_ops)
         
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      l=lr
      ilr=il+3
      if(long.eq.0) then
c     . no try ops
         goto 121
      endif
c     re-enter tradsl for coding the try_ops
      goto 10
 121  continue
c     end of try_ops coding
c     recall the pointers on "try catch" list
      ilr=ids(1,pt)
      lr=ids(2,pt)
      lc0=ids(3,pt)
c     add info on "try instruction" into  "try catch" list
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l
c
c     adding  "catch instructions"
      il=iadr(lr)
c     preserve pointers on the   "try catch" list

      rstk(pt)=17
      ids(1,pt)=ilr
      ids(2,pt)=lr
      ids(3,pt)=lc0
      istk(ilr)=lc
      long=istk(lc0+2) 
      lcf=lc+long-1

      ni=cmplxt(istk(lc),long)
      istk(il)=15
      istk(il+1)=ni
      istk(il+2)=1
      lr=sadr(il+3+ni)
      l=lr
      ilr=il+3
      if(long.eq.0) then
c     . no catch ops
         goto 122
      endif
c     re-enter tradsl for coding the catch_ops
      goto 10

 122  continue
c     end of catch_ops coding
c     recall the pointers on the "try catch" list
      ilr=ids(1,pt)
      lr=ids(2,pt)
      lc0=ids(3,pt)
c     add info on "catch instruction" into  "try catch" list
      istk(ilr)=istk(ilr-1)+l-lr
      ilr=ilr+1
      lr=l
      pt=pt-1
      if(ddt.le.-1) then
c			 replaces write(6 ...) by basout bug 2598                  
c      write(6,'(a10,5i5)') 'catch end',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'catch end',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
c     recall the pointers on the current list
      ilr=ids(1,pt)
      lr=ids(2,pt)
      lcf=istk(ilr)

c     add info on "try catch" into  current list
      istk(ilr)=istk(ilr-1)+l-lr
      lr=l
      ilr=ilr+1
      pt=pt-1
      if(ddt.le.-1) then
c			 replaces write(6 ...) by basout bug 2598                  
c      write(6,'(a10,5i5)') 'try end',pt,lcf,ilr,lr,l-lr
        write(buf,'(a10,5i5)') 'try end',pt,lcf,ilr,lr,l-lr
        call basout(io,wte,buf)
      endif
      goto 10
c   

c

  200 format('(f',i2,'.',i2,')')
  201 format('(1pd',i2,'.',i2,')')

c     
      end

