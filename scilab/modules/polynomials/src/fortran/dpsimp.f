c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1990-2008 - INRIA - Serge STEER
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
      subroutine dpsimp(a,na,b,nb,a1,na1,b1,nb1,w,ierr)
c!    but
c     Etant donnes une fraction rationnelle donnee par ses polynomes
c     numerateur et denominateurs, ce sous programme retourne le numerateur
c     et le denominateur de sa representation simplifiee.
c!    liste d'appel
c     subroutine dpsimp(a,na,b,nb,as,nas,bs,nbs,w,ierr)
c
c     double precision a(na+1),b(nb+1),as(*),bs(*),w,er
c     integer na,nb,nas,nbs,ierr
c
c     a    :  tableau contenant les coefficients du numerateur range
c             par puissance croissante.(entree)
c     na   :  degre du numerateur (entree)
c     b    :  tableau contenant les coefficients du denominateur range
c             par puissance croissante. (entree)
c     nb   :  degre du denominateur (entree)
c
c     a1   :  tableau contenant les coefficients du numerateur range
c             par puissance croissante.(sortie)
c     na1  :  degre+1 du numerateur (sortie)
c     b1   :  tableau contenant les coefficients du denominateur range
c             par puissance croissante. (sortie)
c     nb1  :  degre+1 du denominateur (sortie)
c
c     les implantations de a et a1, b et b1 peuvent etre confondues.
c     Dans les cas ou les zones memoires de a (resp b) et a1 (resp b1) se
c     chevauchent, l'adresse de a1 (resp b1) doit etre au moins egale a
c     l'adresse de a  (resp b)
c
c     w    :  tableau de travail de taille:
c             n0 = max(na,nb) + 1
c             2*(na+nb)+min(na,nb)+10*n0+3*n0**2+4
c     ierr :
c             en entree ierr specifie l'espace memoire disponible dans w
c             en sortie:
c     ierr=0 : ok
c     ierr=1 : denominateur nul
c     ierr=2 : espace memoire insuffisant on retourne les polynomes
c!
      double precision a(na+1),b(nb+1),w(*),a1(*),b1(*),t,er,t1,t2
      integer na,nb,ierr,ipb(6)
c
      lw=1+2*(na+nb)+min(na,nb)+3
c     n0=max(na,nb)+1
c     lfree=lw+10*n0+3*n0*n0

c
      maxw=ierr
      ierr=0
c
c degre reel des polynomes
c

c
      nnb=nb+1
 08   nnb=nnb-1
      if(nnb.lt.0) then
         ierr=1
         return
      endif
      if(b(nnb+1).eq.0.0d+0) goto 08

      nna=na+1
 09   nna=nna-1
      if(nna.lt.0) goto 20
      if(a(nna+1).eq.0.0d+0) goto 09
c
c     elimination des racines en zero
      la0=0
 10   la0=la0+1
      if(a(la0).eq.0.0d+0) goto 10
      na1=nna-(la0-1)
      nz=la0-1
c
      lb0=0
 11   lb0=lb0+1
      if(b(lb0).eq.0.0d+0) goto 11
      nb1=nnb-(lb0-1)
      nz=nz-(lb0-1)
c

      n0=max(na1,nb1)+1
      lfree=lw+10*n0+3*n0*n0
      if(lfree.gt.maxw.and.na1.gt.0.and.nb1.gt.0) ierr=2
      if(lfree.gt.maxw.or.na1.eq.0.or.nb1.eq.0) then
         if(nz.eq.0) then
            call dcopy(na1+1,a(la0),1,a1,1)
            call dcopy(nb1+1,b(lb0),1,b1,1)
         elseif(nz.gt.0) then
            call dset(nz,0.0d0,a1,1)
            call dcopy(na1+1,a(la0),1,a1(nz+1),1)
            call dcopy(nb1+1,b(lb0),1,b1,1)
            na1=na1+nz
         else
            call dcopy(na1+1,a(la0),1,a1,1)
            call dset(-nz,0.0d0,b1,1)
            call dcopy(nb1+1,b(lb0),1,b1(-nz+1),1)
            nb1=nb1-nz
         endif
         na1=na1+1
         nb1=nb1+1
         return
      endif
c     normalize highest degree coefficients of num and den
      t1=a(nna+1)
      t2=b(nnb+1)
      call dscal(na1+1,1.0d0/t1,a(la0),1)
      call dscal(nb1+1,1.0d0/t2,b(lb0),1)
c
      call recbez(a(la0),na1,b(lb0),nb1,w,ipb,w(lw),er)
      if(er.gt.1d-3) goto 30
      nden=ipb(5)-ipb(4)
      nnum=ipb(6)-ipb(5)
      if(na1.ne.nnum-1) then
         t=w(ipb(5)-1)
         t=(1.0d0)/t
         if(nz.eq.0) then
            call dcopy(nnum,w(ipb(5)),1,a1,1)
            call dcopy(nden,w(ipb(4)),1,b1,1)
            call dscal(nden,t,b1,1)
         else if(nz.gt.0) then
            call dcopy(nnum,w(ipb(5)),1,a1(1+nz),1)
            call dset(nz,0.0d0,a1,1)
            nnum=nnum+nz
            call dcopy(nden,w(ipb(4)),1,b1,1)
            call dscal(nden,t,b1,1)
         else if(nz.lt.0) then
            nz=-nz
            call dcopy(nnum,w(ipb(5)),1,a1,1)
            call dcopy(nden,w(ipb(4)),1,b1(1+nz),1)
            call dset(nz,0.0d+0,b1,1)
            call dscal(nden,t,b1(1+nz),1)
            nden=nden+nz
         endif
         call dscal(nnum,-t*t1/t2,a1,1)
      else
         if(nz.eq.0) then
            call dcopy(nnum,a(la0),1,a1,1)
            call dcopy(nden,b(lb0),1,b1,1)
         else if(nz.gt.0) then
            call dcopy(nnum,a(la0),1,a1(1+nz),1)
            call dset(nz,0.0d+0,a1,1)
            nnum=nnum+nz
            call dcopy(nden,b(lb0),1,b1,1)
         else
            nz=-nz
            call dcopy(nnum,a(la0),1,a1,1)
            call dcopy(nden,b(lb0),1,b1(1+nz),1)
            call dset(nz,0.0d+0,b1,1)
            nden=nden+nz
         endif
         call dscal(nnum,t1,a1,1)
         call dscal(nden,t2,b1,1)

      endif
      na1=nnum
      nb1=nden
      return
 20   a1(1)=0.0d+0
      b1(1)=1.0d+0
      na1=1
      nb1=1
      return
 30   if(nz.eq.0) then
         call dcopy(na1+1,a(la0),1,a1,1)
         call dcopy(nb1+1,b(lb0),1,b1,1)
      elseif(nz.gt.0) then
         call dset(nz,0.0d0,a1,1)
         call dcopy(na1+1,a(la0),1,a1(nz+1),1)
         call dcopy(nb1+1,b(lb0),1,b1,1)
         na1=na1+nz
      else
         call dcopy(na1+1,a(la0),1,a1,1)
         call dset(-nz,0.0d0,b1,1)
         call dcopy(nb1+1,b(lb0),1,b1(-nz+1),1)
         nb1=nb1-nz
      endif
      na1=na1+1
      nb1=nb1+1
      call dscal(na1,t1,a1,1)
      call dscal(nb1,t2,b1,1)
      return

      end
