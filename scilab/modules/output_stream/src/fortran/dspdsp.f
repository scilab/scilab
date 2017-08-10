c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) - INRIA - Serge Steer
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine dspdsp(ne,ind,x,m,n,maxc,mode,ll,lunit,cw)
c!but
c     dspdsp visualise une matrice  creuse
c!liste d'appel
c
c     subroutine dspdsp(ne,ind,x,m,n,maxc,mode,ll,lunit,cw)
c
c     double precision x(*)
c     integer ind(*)
c     integer nx,m,n,maxc,mode,ll,lunit
c     character cw*(*)
c
c     c : nombre d'elements nons nuls de la matrice
c     ind : indices specifiant la position des elements non nuls
c     x : tableau contenant les elements non nuls
c     m : nombre de ligne de la matrice
c     n : nombre de colonnes de la matrice
c     maxc : nombre de caracteres maximum autorise pour
c            representer un nombre
c     mode : si mode=1 representation variable
c            si mode=0 representation d(maxc).(maxc-7)
c     ll : longueur de ligne maximum admissible
c     lunit : etiquette logique du support d'edition
c     cw : chaine de caracteres de travail de longueur au moins ll
c!
      double precision x(*),a,a1,a2,fact,eps,dlamch
      integer maxc,mode,fl,typ
      integer ind(*)
      character cw*(*),sgn*1,dl*1
      character*10 form(2)
c
      if(ne.eq.0) then
         write(cw,'(''('',i5,'','',i5,'') zero sparse matrix'')') m,n
         call basout(io,lunit,cw(1:32))
         call basout(io,lunit,' ')
         goto 99
      else
         write(cw,'(''('',i5,'','',i5,'') sparse matrix'')') m,n
         call basout(io,lunit,cw(1:27))
         call basout(io,lunit,' ')
         if(io.eq.-1) goto 99
      endif
      ilr=1
      ilc=m+1
      nx=1
      eps=dlamch('p')
      cw=' '
      write(form(1),130) maxc,maxc-7
      dl=' '
      if(m*n.gt.1) dl=' '
      
c
c facteur d'echelle
c
      fact=1.0d+0
      a1=0.0d+0
      if(ne.eq.1) goto 10
      a2=abs(x(1))
      do 05 i=1,ne
         a=abs(x(i))
         if(a.eq.0.0d+0.or.a.gt.dlamch('o')) goto 05
         a1=max(a1,a)
         a2=min(a2,a)
   05 continue
      imax=0
      imin=0
      if(a1.gt.0.0d+0) imax=int(log10(a1))
      if(a2.gt.0.0d+0) imin=int(log10(a2))
      if(imax*imin.le.0) goto 10
      imax=(imax+imin)/2
      if(abs(imax).ge.maxc-2)  fact=10.0d+0**(-imax)
   10 continue
      eps=a1*fact*eps
c
      if(fact.ne.1.0d+0) then
         write(cw(1:12),'(1x,1pd9.1,'' *'')')  1.0d+0/fact
         call basout(io,lunit,cw(1:12))
         call basout(io,lunit,' ')
         if(io.eq.-1) goto 99
      endif
      i0=0
      i1=i0
      l=1
      do 20 k=1,ne
         cw=' '
 11      i0=i0+1
         if(i0-i1.gt.ind(l)) then
            i1=i0
            l=l+1
            goto 11
         endif
         i=l
         j=ind(ilc-1+k)
         write(cw,'(''('',i5,'','',i5,'')'')') i,j
         l1=14
          a=x(k)*fact
c         if(abs(a).lt.eps.and.mode.ne.0) a=0.0d+0
         sgn=' '
         if(a.lt.0.0d+0) sgn='-'
         a=abs(a)

c     determination du format devant representer a
         cw(l1:l1+6)='      '//sgn
         l1=l1+7

         typ=1
         if(mode.eq.1) then
            call fmt(a,maxc,typ,n1,n2)
            if(typ.eq.2) typ=n2+32*n1
         else
            if (isanan(a).eq.1) then
               typ=-2
            elseif (a.gt.dlamch('o')) then
               typ=-1
            endif
         endif

         call formatnumber(a,typ,maxc,cw(l1:),fl)
         l1=l1+fl
         call basout(io,lunit,cw(1:l1) )
         if (io.eq.-1) goto 99

   20 continue
 99   continue
c
  130 format('(1pd',i2,'.',i2,')')
      end
