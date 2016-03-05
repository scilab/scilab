c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.


C/MEMBR ADD NAME=DMDSPF,SSI=0
      subroutine dmdspf(x,nx,m,n,maxc,ll,lunit)
c!but
c     dmdspf ecrit une matrice  (ou un scalaire) sous la forme d'un
c     tableau s, avec gestion automatique de l'espace disponible.
c     Chaque nombre est ecrit sous la forme dmaxc.maxc-7
c!liste d'appel
c
c     subroutine dmdspf(x,nx,m,n,maxc,ll,lunit)
c
c     double precision x(*)
c     integer nx,m,n,maxc,ll,lunit
c
c
c     x : tableau contenant les coefficients de la matrice x
c     nx : entier definissant le rangement dans x
c     m : nombre de ligne de la matrice
c     n : nombre de colonnes de la matrice
c     maxc : nombre de caracteres maximum autorise pour
c            representer un nombre
c     ll : longueur de ligne maximum admissible
c     lunit : etiquette logique du support d'edition
c!
      double precision x(*)
      integer   maxc
      character buf*80,form*20,cw*20
c
      io=0
      ncol=ll/(maxc+2)
      nbloc=(n+ncol-1)/ncol
c
      write(form,130) ncol,maxc,maxc-7
c
      k1=1
      do 70 ib=1,nbloc
      k2=min(k1-1+ncol,n)
      if(nbloc.ne.1) then
         if(k1.eq.k2) then
            write(cw(1:4),'(i4)') k1
            call basout(io,lunit,' ')
            call basout(io,lunit,'         colonne '//cw(1:4))
         else
            write(cw(1:8),'(2i4)') k1,k2
            call basout(io,lunit,' ')
            call basout(io,lunit,'        colonnes '//cw(1:4)//
     &                        ' a '//cw(5:8))
            call basout(io,lunit,' ')
         endif
         call basout(io,lunit,' ')
         if (io.eq.-1) goto 99
      endif
c
      do 60 l=1,m
      write(buf,form) (x(l+(k-1)*nx),k=k1,k2)
      call basout(io,lunit,buf)
      if (io.eq.-1) goto 99
   60 continue
      k1=k2+1
   70 continue
c
   99 return
c
  130 format('(1x,',i2,'(1pd',i2,'.',i2,',2x))')
      end
