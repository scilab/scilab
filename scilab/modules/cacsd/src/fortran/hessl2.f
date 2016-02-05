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

      subroutine hessl2(neq,tq,pd,nrowpd)
c!but
c     Elle etablit la valeur de la Hessienne, derivee
c       seconde de la fonction phi au point q .
c!liste d'appel
c     subroutine hessl2(neq,tq,pd,nrowpd)
c     Entree :
c     - neq. tableau entier de taille 3+(nq+1)*(nq+2)
c         neq(1)=nq est le degre effectif du polynome tq (ou q).
c         neq(2)=ng est le nombre de coefficient de fourier
c         neq(3)=dgmax degre maximum pour q (l'adresse des coeff de fourier dans
c               tq est neq(3)+2
c         neq(4:(nq+1)*(nq+2)) tableau de travail entier
c     - tq. tableau reel de taille au moins
c               6+dgmax+5*nq+6*ng+nq*ng+nq**2*(ng+1)
c         tq(1:nq+1) est le tableau des coefficients du polynome.
c         tq(dgmax+2:dgmax+ng+2) est le tableau des coefficients
c                      de fourier
c         tq(dgmax+ng+3:) est un tableau de travail de taille au moins
c                         5+5*nq+5*ng+nq*ng+nq**2*(ng+1)
c     Sortie :
c     - pd matrice hessienne
c!

      implicit double precision (a-h,o-y)
      dimension tq(*),pd(nrowpd,*)
      dimension neq(*)
c
      nq=neq(1)
      ng=neq(2)
c
c     decoupage du tableau neq
      jmxnv=4
      jmxnw=jmxnv+(nq+1)
      jw=jmxnw+(nq+1)**2
c
c     decoupage du tableau tq
      itq=1
      itg=itq+neq(3)+1
      itr=itg+ng+1
      itp=itr+nq+ng+1
      itv=itp+nq+ng+1
      itw=itv+nq+ng+1
      itij=itw+nq+ng+1
      id1aux=itij+ng+1
      id2aux=id1aux+(ng+1)*nq
      iw=id2aux+nq*nq*(ng+1)

      call hl2(nq,tq,tq(itg),ng,pd,nrowpd,tq(itr),
     $     tq(itp),tq(itv),tq(itw),tq(itij),tq(id1aux),tq(id2aux),
     $     neq(jmxnv),neq(jmxnw))
      return
      end



      subroutine hl2(nq,tq,tg,ng,pd,nrowpd,tr,tp,tv,tw,tij,d1aux,d2aux,
     &     maxnv,maxnw)
c!

      implicit double precision (a-h,o-y)
      dimension tq(nq+1),tg(ng+1),pd(nrowpd,*)
c
      dimension tr(nq+ng+1),tv(nq+ng+1),tp(nq+ng+1),tw(nq+ng+1),
     &     tij(ng+1),d1aux(ng+1,nq),d2aux(nq,nq,ng+1)
      integer maxnv(nq),maxnw(nq,nq)
c
c     --- Calcul des derivees premieres de 'vq' ---
c
      do 20 i=1,nq
         if (i.eq.1) then
c     .     division euclidienne de z^nq*g par q
            call dset(nq,0.0d0,tp,1)
            call dcopy(ng+1,tg,1,tp(nq+1),1)
            call dpodiv(tp,tq,nq+ng,nq)
            nv1=ng
c     .     calcul de Lq et Vq
            call lq(nq,tq,tr,tg,ng)
            ltvq=nq+1
c     .     division euclidienne de Vq par q
            call dcopy(ng+1,tr(ltvq),1,tv,1)
            call dset(nq,0.0d0,tv(ng+2),1)
            call dpodiv(tv,tq,ng,nq)
            nv2=ng-nq
         else
            ichoi1=1
            call dzdivq(ichoi1,nv1,tp,nq,tq)
            ichoi2=2
            call mzdivq(ichoi2,nv2,tv,nq,tq)
         endif
         maxnv(i)=max(nv1,nv2)
         do 10 j=1,maxnv(i)+1
            d1aux(j,i)= tp(nq+j)-tv(nq+j)
 10      continue
 20   continue
c
c     --- Calcul des derivees secondes de 'vq' ---
c
      do 50 i=1,nq
         call dset(ng+nq+1,0.0d0,tw,1)
         do 40 j=nq,1,-1
            if (j.eq.nq) then
               call dcopy(maxnv(i)+1,d1aux(1,i),1,tw(nq),1)
               nw=maxnv(i)+nq-1
               call dpodiv(tw,tq,nw,nq)
               nw=nw-nq
            else
               ichoix=1
               call dzdivq(ichoix,nw,tw,nq,tq)
            endif
            do 30 k=1,nw+1
               d2aux(i,j,k)=tw(nq+k)
 30         continue
            maxnw(i,j)=nw
 40      continue
 50   continue
c
c     --- Conclusion des calculs sur la hessienne ---
c
      do 100 i=1,nq
         do 90 j=1,i
            call scapol(maxnv(i),d1aux(1,i),maxnv(j),
     &           d1aux(1,j),y1)
c     
            if (maxnw(i,j).gt.maxnw(j,i)) then
               maxij=maxnw(i,j)
               minij=maxnw(j,i)
               do 60 k=minij+2,maxij+1
                  tij(k)= -d2aux(i,j,k)
 60            continue
            else if (maxnw(i,j).lt.maxnw(j,i)) then
               maxij=maxnw(j,i)
               minij=maxnw(i,j)
               do 70 k=minij+2,maxij+1
                  tij(k)= -d2aux(j,i,k)
 70            continue
            else
               maxij=maxnw(i,j)
               minij=maxij
            endif
c     
            do 80 k=1,minij+1
               tij(k)= -d2aux(i,j,k) -d2aux(j,i,k)
 80         continue
c     
            call scapol(maxij,tij,ng,tr(ltvq),y2)

            if (i.eq.j) then
               pd(i,i)=-2.0d+0 * (y1+y2)
            else
               pd(i,j)=-2.0d+0 * (y1+y2)
               pd(j,i)=-2.0d+0 * (y1+y2)
            endif
 90      continue
 100  continue
      return
      end
