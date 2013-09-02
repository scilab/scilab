c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine skpins(job)
c!but 
c     ce sous programme permet de sauter les instructions suivantes
c     d'une clause j'usqu'a l'un des  mots clef "end" ou
c     "else","elseif","case" de la clause courante
c     si job=1 on recherche le "end"
c     si job=0 on recherche le "end" ou "else","elseif","case", "catch"
c     
c     attention skpins modifie la valeur de lpt en particulier lpt(4)
c     et lpt(6)
c     
      include 'stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
c     
      integer for(nsiz),while(nsiz),iff(nsiz),else(nsiz),ennd(nsiz)
      integer cas(nsiz),sel(nsiz),elsif(nsiz),try(nsiz),catchkey(nsiz)
      integer clcnt,strcnt,qcount,bcount
      integer rparen,left,right,quote,dot
      integer eol,blank,name,num,cmt,psym,pchar
      logical eqid
c
      data rparen/42/,left/54/,right/55/,quote/53/,dot/51/
      data eol/99/,blank/40/
      data num/0/,name/1/,cmt/2/
      data else/236721422,nz1*673720360/
      data ennd/671946510,nz1*673720360/, for/672864271,nz1*673720360/
      data iff/673713938,nz1*673720360/
      data elsif/236721422,673713938,nz2*673720360/
      data while/353505568,673720334,nz2*673720360/
      data cas/236718604,nz1*673720360/
      data sel/236260892,673717516,nz2*673720360/
      data try/ 673323805,nz1*673720360/
      data catchkey/203229708,673720337,nz2*673720360/
c     
      l4=lpt(4)
      
      clcnt = 0
      strcnt=0
      bcount=0
 10   continue
      psym=sym
      call getsym1(1)
      if(strcnt.eq.0) then
c     
         if (sym .eq. eol) then
c     gestion des clause sur plusieurs lignes
            if(macr.gt.0.and.lin(lpt(4)+1).eq.eol) then
               call error(47)
               return
            endif
c     get the following line
            if(lpt(4).eq.lpt(6))  then
               call getlin(1,0)
               if(err.gt.0) return
            else
               lpt(4)=lpt(4)+1
               char1=blank
            endif
         else if (sym.eq.cmt) then
c     .     look for eol
            l=lpt(4)-1
 11         l=l+1
            if (lin(l).ne.eol.and.l.le.lpt(6)) goto 11
            lpt(4)=l-1
            char1=eol
         else if (sym.eq.left) then
            bcount=bcount+1
         else if (sym.eq.right) then
            bcount=bcount-1
            if(bcount.lt.0) then
               call error(46)
               return
            endif
         else if (sym.eq.quote) then
c     .  check if transpose or beginning of a string
            pchar=lin(lpt(3)-2)
            if(abs(pchar).eq.blank) then
               strcnt=1
            elseif(psym.ne.num.and.psym.ne.name.and.psym.ne.rparen.and.
     $           psym.ne.right.and.psym.ne.dot.and.psym.ne.quote) then
               strcnt=1
            endif
c            if (bcount.ne.0) then
c               pchar=lin(lpt(3)-2)
c               if(abs(pchar).eq.blank) strcnt=1
c            endif
         else if(sym .eq. name.and.bcount.eq.0) then
            if(eqid(syn,ennd)) then
               if(clcnt.eq.0) goto 20
               clcnt=clcnt-1
            elseif (eqid(syn,for) .or. eqid(syn,while) .or.
     $              eqid(syn,iff) .or. eqid(syn,sel) .or.
     $              eqid(syn,try)) then
               clcnt = clcnt+1
            elseif(clcnt.eq.0.and.job.eq.0.and.
     $              (eqid(syn,else).or. eqid(syn,catchkey).or.
     $              eqid(syn,cas).or.eqid(syn,elsif))) then
               goto 20
            endif
         endif
c
      else
         if (sym.eq.eol) then
            call error(3)
            return
         elseif (sym.eq.quote) then
            qcount=0
 14         qcount=qcount+1
            if(abs(char1).ne.quote) goto 15
            call getsym1(1)
            goto 14
 15         continue
            if(2*int(qcount/2).ne.qcount)  strcnt=0
         endif
      endif
      goto 10
c     
 20   continue
      end
