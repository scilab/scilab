      subroutine getfun(lunit,nlines)
c
c ======================================================================     
c     get a user defined function
c ======================================================================    
c     
c     Copyright INRIA
      include '../stack.h'
c     
      integer lrecl,id(nsiz),retu(6),icount
      integer slash,dot,blank,equal,lparen,rparen
      integer comma,semi,less,great,left,right
      integer name,eol
      integer ssym,schar
      integer first,ierr
      integer iadr,sadr
      logical maj,isinstring
c     
      data slash/48/,dot/51/,blank/40/,equal/50/,lparen/41/,rparen/42/
      data comma/52/,semi/43/,less/59/,great/60/,left/54/,right/55/
      data name/1/,eol/99/,lrecl/512/
      data retu/27,14,29,30,27,23/
c     ennd/14,23,13/
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lmax=iadr(lstk(bot)-1)
c     
      if(top-rhs+lhs+1.ge.bot) then
         call error(18)
         return
      endif
c     
      job=0
      l1=lpt(1)
      l2=lpt(2)
      l4=lpt(3)
      l6=lpt(6)
      lpt(1)=l6+1
      lpt(5)=lpt(3)
c     
      ssym=sym
      schar=char1
c
      n=1
      first=1
      l = lpt(1)
      if(lunit.eq.0) goto 30
c     
c     get macro deff from file
c     ------------------------
c     acquisition d'une ligne du fichier
      call getfiletype(lunit,ltype,info)
      if(info.ne.0) goto 90 
      icount=0

 11   buf=' '
      if(ltype.eq.1) then
         call basin(ierr,lunit,buf(1:lrecl),'*')
         if(ierr.eq.1) goto 60
         if(ierr.eq.2) goto 90
         n=lnblnk(buf(1:lrecl))
      else
         call readnextline(lunit,buf,bsiz,n,nr,info)
         if(info.eq.-1) goto 60
         n=n-1
         if (n.ge.1) n=lnblnk(buf(1:n))
      endif

      l0=l
      nlines=nlines+1
      
      if(n.le.0) then
         if(first.eq.1) goto 11
         goto 28
      endif
c     strip blanks at the beginning of the line
      m=0
 16   m=m+1
      if(buf(m:m).eq.' ') goto 16
c
      if(buf(m:m+10).eq.'endfunction') goto 61
      if(buf(m:m+7).eq.'function'.or.buf(m:m+7).eq.'FUNCTION') then
         if(first.eq.1) then
            j=m+7
            goto 25            
         else
            if( ltype.eq.1) then
C     .       jpc : cygwin32 b17.1 bug 
c            backspace(lunit)
               call myback(lunit)
            else
               call mseek(lunit,-nr,'cur',ierr)
            endif
            nlines=max(0,nlines-1)
            goto 61
         endif
      endif
c     
c     boucle de conversion des caracteres de la ligne
      j=m-1
 17   j=j+1
      if(j.gt.n) goto 27
c     
 18   do 20 k = 1, csiz
         if (buf(j:j).eq.alfa(k)) then
            maj=.false.
            goto 21
         elseif(buf(j:j).eq.alfb(k)) then
            maj=.true.
            go to 21
         endif
 20   continue
      k = eol+1
      call xchar(buf(j:j),k)
      if (k .eq. eol) go to 11
      if (k .eq. 0) then
         call basout(io,wte,
     +        buf(j:j)//' is not a scilab character')
         go to 24
      endif
      maj=.false.
c     
 21   k = k-1

      if(buf(j+1:j+1).ne.buf(j:j)) goto 23
      if(k.eq.slash) then
c     .    check if // occurs in a string
         if(first.eq.0.and.isinstring(istk(l0),l-l0+1)) then
c     .     // is part of a string
            if(l+1.gt.lmax) then
               ierr=5
               goto 90
            endif
            istk(l)=slash
            istk(l+1)=slash
            j=j+1
            l=l+2
            goto 17
         else
c     .     // marks beginning of a comment
            if(first.eq.1) then
c     .     // comments before declaration line
               if(j.eq.1) goto 11
               if(buf(1:j-1).eq.' ') goto 11
c     .     // comments at the end of declaration line
               goto 40
            endif
            goto 28
         endif
      endif
c
      if (k.eq.dot) then
c     on a trouve ..
c     c'est une ligne suite si on a que des . ou //
         jj=j+1
 22      continue
         if(jj.ge.n) then
            icount=icount+1
            goto 11
         endif
         jj=jj+1
         if(buf(jj:jj).eq.buf(j:j))goto 22
         if(buf(jj:jj).eq.' ') goto 22
         if(buf(jj:jj).eq.'/'.and.buf(jj+1:jj+1).eq.'/') then
            icount=icount+1
            goto 11
         endif
      endif
 23   continue
c     ce n'est pas une carte suite            
      if(first.eq.1) goto 24
      istk(l) = k
c     
      if(maj) istk(l)=-k
      l = l + 1
      if(l.gt.lmax) then
         ierr=5
         goto 90
      endif
      goto 17

c     premiere ligne
 24   if(l.gt.lpt(1)) goto 26
      if(buf(m:m+7).eq.'function'.or.buf(m:m+7).eq.'FUNCTION') then
         j=m+6
      elseif(k.ne.slash .or. buf(m+1:m+1).ne.buf(m:m)) then
         ierr=4
         goto 90
      else
         goto 11
      endif
      j=j+1
 25   lin(l)=blank
      l=l+1
      goto 17
 26   lin(l)=k
      if(maj) lin(l)=-k
      l=l+1
      if(l.gt.lsiz) then
        ierr=3
        goto 90
      endif
      goto 17
c     
c     fin de conversion de la ligne
 27   if(first.eq.1) goto 40

 28   l=l-1
      if(istk(l).eq.blank) goto 28
      l=l+1
      if(l-1.le.l0) then
         istk(l)=comma
         l=l+1
      endif
      do 29 i=0,icount
c     la gestion de icount a ete ajoute pour maintenir un compteur de ligne
c     correct malgre les lignes suite
         istk(l)=eol
         istk(l+1)=blank
         l=l+2
 29   continue
      l=l-1
      icount=0
c
      goto 11
      
c     
c     get macro deff from stk
c     -----------------------
 30   if(rhs.ne.2) then
         call error(39)
         return
      endif
c     
      ilt=iadr(lstk(top))
      if(istk(ilt).ne.10) then
         err=2
         call error(55)
         return
      endif
c
      ild=iadr(lstk(top-1))
      if(istk(ild).ne.10) then
         err=1
         call error(55)
         return
      endif
      if(istk(ild+1)*istk(ild+2).ne.1) then
         err=1
         call error(89)
         return
      endif
c
      il=ild+5
      n=istk(il)-1
      do 31 j=1,n
         lin(l)=istk(il+j)
         l=l+1
 31   continue
      goto 40
c     
 33   mn=istk(ilt+1)*istk(ilt+2)
      ili=ilt+4+mn
      ilt=ilt+4
      do 35 i=1,mn
         n=istk(ilt+i)-istk(ilt+i-1)
         if(n.gt.0) then
            do 34 j=1,n
               istk(l)=istk(ili+j)
               l=l+1
 34         continue
         else
           istk(l)=blank
           l=l+1
         endif
         istk(l)=eol
         l=l+1
         ili=ili+n
 35   continue
      goto 61
c     
c     analyse de la ligne de declaration
 40   continue
      if(ddt.ge.2) call basout(io,wte,buf(1:n))
      if(l.eq.lpt(1)) then
         ierr=6
         goto 90
      endif
      lin(l) = eol
      lpt(6) = l
      lpt(4) = lpt(1)
      lpt(3) = lpt(1)
      lpt(2) = lpt(1)
      lct(1) = 0
cMAJ  
      fin=0
      call getch
c     
      if(top+2.ge.bot) then
         call error(18)
         return
      endif
      top=top+1
      il=iadr(lstk(top))
      istk(il)=11
      l=il+2
      if(l.gt.lmax) then
         ierr=5
         goto 90
      endif
c     
      call getsym
      mlhs=0
      if(sym.eq.name) then
c     a=func(..) ou func(..)
         if(char1.eq.equal) then
c     a=func(..) 
            mlhs=mlhs+1
            l=l+nsiz
            if(l.gt.lmax) then
               ierr=5
               goto 90
            endif
            call putid(istk(l-nsiz),syn(1))
            call getsym
            call getsym
         endif
      elseif(sym.eq.less.or.sym.eq.left) then
c     [..]=func()
 41      call getsym
         if(sym.ne.name) goto  42
         mlhs=mlhs+1
         l=l+nsiz
         if(l.gt.lmax) then
            ierr=5
            goto 90
         endif
         call putid(istk(l-nsiz),syn(1))
         call getsym
         if(sym.eq.comma) goto  41
 42      if(sym.ne.great.and.sym.ne.right) then
            ierr=4
            goto  90
         endif
c     
         call getsym
         if(sym.ne.equal) then
            ierr=4
            goto  90
         endif
         call getsym
      else
         ierr=4
         goto 90
      endif
c
      if(sym.ne.name) then
         ierr=4
         goto  90
      endif
      istk(il+1)=mlhs
      call putid(id,syn(1))
c     
      mrhs=0
      il=l
      l=l+1
      if(l.gt.lmax) then
         ierr=5
         goto 90
      endif
      if(char1.eq.semi.or.char1.eq.comma) goto 46
      call getsym
      if(sym.eq.eol) goto 46
      if(sym.ne.lparen) then
         ierr=4
         goto  90
      endif
 44   call getsym
      if(sym.ne.name) goto  45
      mrhs=mrhs+1
      l=l+nsiz
      if(l.gt.lmax) then
         ierr=5
         goto 90
      endif
      call putid(istk(l-nsiz),syn(1))
      call getsym
      if(sym.eq.comma) goto  44
 45   if(sym.ne.rparen) then
         ierr=4
         goto  90
      endif
      call getsym
      if(sym.ne.eol.and.sym.ne.semi.and.sym.ne.comma) then
         ierr=4
         goto  90
      endif 
 46   istk(il)=mrhs
c     
      il=l
      l=l+1
      if(lunit.eq.0) goto 33
      first=0
      goto 11
c     
c     fin
 60   if(first.eq.1) then
         job=-1
         goto 62
      else
         job=1
      endif
 61   call icopy(6,retu,1,istk(l),1)
      l=l+6
      istk(l)=eol
      l=l+1
      istk(l)=eol
      l=l+1
      istk(il)=l-(il+1)
      lstk(top+1)=sadr(l)
c     
      lpt(1)=l1
      call putid(idstk(1,top),id)
c
 62   lpt(1)=l1
      lpt(6)=l6
      lpt(4)=l4
      lpt(3)=l4
      lpt(2)=l2
      char1=semi
      sym=semi
c      char1=schar
c      sym=ssym
      fin=job
      return

c     
 90   continue
c gestion des erreurs
c
c     on retablit les pointeurs de ligne pour le gestionnaire d'erreur
      lpt(1)=l1
      lpt(6)=l6
      lpt(4)=l4
      lpt(3)=l4
      lpt(2)=l2
      goto(91,92,93,94,95),ierr-1
c
 91   continue
c     erreur de lecture
      call error(49)
      return
 92   continue
c     buffer limit
      call error(26)
      return
 93   continue
c     invalid syntaxe
      err=nlines
      call error(37)
      return
 94   err=lstk(bot)-sadr(l)
      call error(17)
      return
 95   call error(28)
      return
c     
 
      end
