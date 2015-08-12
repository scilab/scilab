c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine compcl(job)
c     ======================================================================
c     compilation des structures de controle
c     ======================================================================
      include 'stack.h'
c
      parameter (nz1=nsiz-1,nz2=nsiz-2)
      parameter (iif=1,iwhile=2,iselect=3)
      logical eqid
      integer else(nsiz),r,cas(nsiz),elsif(nsiz)
      integer sadr
c     job is used for try catch
      integer job
      character tmpbuf * (bsiz)

      data else/236721422,nz1*673720360/
      data cas/236718604,nz1*673720360/
      data elsif/236721422,673713938,nz2*673720360/
c
      sadr(l)=(l/2)+1
c     
      l=comp(1)
      r = rstk(pt)-800
      if (ddt .eq. 4) then
         write(tmpbuf(1:20),'(2i4,2i6)') pt,rstk(pt),l,comp(2)
         call basout(io,wte,' compcl pt:'
     &              //tmpbuf(1:4)//' rstk(pt):'
     &              //tmpbuf(5:8)//' comp(1):'
     &              //tmpbuf(9:14)//' comp(2):'
     &              //tmpbuf(15:20))
      endif

      if(ids(1,pt).eq.iselect) goto 10
      goto(02,03,04,05,06,08,10,20),r
c
c for : <7 l.boucle boucle.ops l.ops varn(1:nsiz) for.ops>
c
c debut for
   01 continue
      err=sadr(l+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(l)=7
      l0=l+2
      istk(l0-1)=comp(2)
      comp(2)=l0
      comp(1)=l+2
      return
   02 err=sadr(l+nsiz+2)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      l0=comp(2)
      comp(2)=istk(l0-1)
      istk(l0-1)=l-l0
       l0=l
      call putid(istk(l+1),ids(1,pt))
      l=l+nsiz+1
      istk(l0)=comp(2)
      comp(2)=l0
      comp(1)=l
      return
c fin for
   03 l0=comp(2)
      comp(2)=istk(l0)
      istk(l0)=l-(l0+nsiz+1)
      call setlnb
      return
c
c while : <8 0 l.exprs l.then l.else exprs.ops then.ops else.ops>
c if    : <9 0 l.exprs l.then l.else exprs.ops then.ops else.ops>
c le "0" est present pour conserver la compatibilite avec la version precedente
c
c debut d'un if ou while
 04   err=sadr(l+2)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         if(err.gt.0) return
      endif
      istk(l)=l
      istk(l+1)=comp(2)
      lc=l+2
      istk(lc)=0
      comp(2)=l+3
      comp(1)=l+4
c
      l=comp(1)
      l0=comp(2)
      istk(l)=l0-2
      istk(l0)=l-(l0+1)
      comp(2)=l
      comp(1)=l+1
      return
c fin des expressions du if ou des elseif
   05 l0=comp(2)
      istk(l)=istk(l0)
      lc=istk(l0)+1
      istk(lc)=istk(lc)+1
      istk(l0)=l-(l0+1)
      comp(2)=l
      comp(1)=l+1
      call setlnb
      return
c fin d'un  Then
   06 l0=comp(2)
      istk(l)=istk(l0)
      istk(l0)=l-(l0+1)
      comp(2)=l
      comp(1)=l+1
      if(eqid(syn(1),elsif))return
c fin de la sequence des then
c     on debute le else par une sequence expression vide
      istk(l+1)=istk(l)
      istk(l)=0
      comp(2)=l+1
      comp(1)=l+2
      call setlnb
      if(eqid(syn(1),else)) return
c il n'y a pas de else,
c     on introduit une sequence else_instructions vide
      istk(l+2)=istk(l+1)
      istk(l+1)=0
      comp(2)=l+2
      l=l+3
      comp(1)=l
c     et on termine le if/while
      goto 09
c
c fin du else
   08 l0=comp(2)
      istk(l)=istk(l0)
      istk(l0)=l-(l0+1)
      comp(2)=l
c
c fin if/while
   09 l0=comp(2)
      l0=istk(comp(2))
      comp(2)=istk(l0)
      istk(l0)=-(l-istk(l0-1))
      istk(l0-1)=8
      if(ids(1,pt).eq.iwhile) istk(l0-1)=9
      call setlnb
      return
c
c
c select case <10,???>
   10 continue
      goto(11,13,14,15,12),r-2
      goto 99
c
c debut selec (premiere expression)
   11 err=sadr(l+2)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         if(err.gt.0) return
      endif
      istk(l)=l
      istk(l+1)=comp(2)
      lc=l+2
      istk(lc)=0
      comp(2)=l+3
      comp(1)=l+4
      return
c
c fin premiere expression
   12 l0=comp(2)
      istk(l)=l0-2
      istk(l0)=l-(l0+1)
      comp(2)=l
      comp(1)=l+1
      return
c
c fin des expressions des cases
   13 l0=comp(2)
      istk(l)=istk(l0)
      lc=istk(l0)+1
      istk(lc)=istk(lc)+1
      istk(l0)=l-(l0+1)
      comp(2)=l
      comp(1)=l+1
      call setlnb
      return
c fin d'un case (partie Then)
   14 l0=comp(2)
      istk(l)=istk(l0)
      istk(l0)=l-(l0+1)
      comp(2)=l
      comp(1)=l+1
      if(eqid(syn(1),cas))return
c fin de la sequence de cases
c     on debute le else par une sequence expression vide
      istk(l+1)=istk(l)
      istk(l)=0
      comp(2)=l+1
      comp(1)=l+2
      if(eqid(syn(1),else)) then
         call setlnb
         return
      endif
c il n'y a pas de else,
c     on introduit une sequence else_instructions vide
      istk(l+2)=istk(l+1)
      istk(l+1)=0
      comp(2)=l+2
      l=l+3
      comp(1)=l
c     et on termine le select
      goto 17
c
c fin du else
   15 l0=comp(2)
      istk(l)=istk(l0)
      istk(l0)=l-(l0+1)
      comp(2)=l
c
c fin selec
   17 l0=comp(2)
      l0=istk(comp(2))
      comp(2)=istk(l0)
      istk(l0)=l-istk(l0-1)
      istk(l0-1)=10
      call setlnb
      return
c
c     try <11 l.try l.catch try.ops catch.ops >
 20   continue
      goto (21,22,23) job
 21   continue
c     initialize the try structure
      err=sadr(l+3)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(l)=11
      istk(l+1)=-1
      istk(l+2)=comp(2)
      comp(2)=l+2
      comp(1)=l+3
      return
 22   continue
c     end of try instructions
      l0=comp(2)
      istk(l0-1)=l-(l0+1)
      call setlnb
      return
 23   continue

c     end of catch instructions
      l0=comp(2)
      comp(2)=istk(l0)
      if (istk(l0-1).eq.-1) then
c     .  no catch keyword,
         istk(l0-1)=l-(l0+1)
         istk(l0)=0
      else
         istk(l0)=l-(l0+1)-istk(l0-1)
      endif
      call setlnb
      return

   99 call error(22)
      return
      end
