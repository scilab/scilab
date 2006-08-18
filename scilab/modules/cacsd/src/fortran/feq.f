      subroutine feq(neq,t,tq,tqdot)
c!but
c      Etablir la valeur de l'oppose du gradient au point q
c!liste d'appel
c     subroutine feq(neq,t,tq,tqdot)
c     - neq. tableau entier de taille 3+(nq+1)*(nq+2)
c         neq(1)=nq est le degre effectif du polynome tq (ou q).
c         neq(2)=ng est le nombre de coefficient de fourier
c         neq(3)=dgmax degre maximum pour q (l'adresse des coeff de fourier dans
c               tq est neq(3)+2
c     - t  . variable parametrique necessaire a l'execution de
c         la routine lsoda .
c     - tq. tableau reel de taille au moins
c               3+dgmax+nq+2*ng
c         tq(1:nq+1) est le tableau des coefficients du polynome q.
c         tq(dgmax+2:dgmax+ng+2) est le tableau des coefficients
c                      de fourier
c         tq(dgmax+ng+3:) est un tableau de travail de taille au moins
c                         nq+ng+1
c     Sortie :
c     - tqdot . tableau contenant les opposes des coordonnees du
c              gradient de la fonction PHI au point q
c!Remarque 
c     la structure particuliere  pour neq et tq est liee au fait que feq peut
c     etre appele comme un external de lsode
c!
c     Copyright INRIA
      implicit double precision (a-h,o-y)
      dimension tq(*),tqdot(*)
      dimension neq(*)
c
      
      nq=neq(1)
      ng=neq(2)
c
c     decoupage du tableau tq
      itq=1
      itg=itq+neq(3)+1
      iw=itg+ng+1
      ifree=iw+1+nq+ng

      call feq1(nq,t,tq,tq(itg),ng,tqdot,tq(iw))
      return
      end
      subroutine feqn(neq,t,tq,tqdot)
c!but
c      Etablir la valeur  du gradient au point q
c!liste d'appel
c     subroutine feqn(neq,t,tq,tqdot)
c     - neq. tableau entier de taille 3+(nq+1)*(nq+2)
c         neq(1)=nq est le degre effectif du polynome tq (ou q).
c         neq(2)=ng est le nombre de coefficient de fourier
c         neq(3)=dgmax degre maximum pour q (l'adresse des coeff de fourier dans
c               tq est neq(3)+2
c     - t  . variable parametrique necessaire a l'execution de
c         la routine lsoda .
c     - tq. tableau reel de taille au moins
c               3+dgmax+nq+2*ng
c         tq(1:nq+1) est le tableau des coefficients du polynome q.
c         tq(dgmax+2:dgmax+ng+2) est le tableau des coefficients
c                      de fourier
c         tq(dgmax+ng+3:) est un tableau de travail de taille au moins
c                         nq+ng+1
c     Sortie :
c     - tqdot . tableau contenant les opposes des coordonnees du
c              gradient de la fonction PHI au point q
c!Remarque 
c     la structure particuliere  pour neq et tq est liee au fait que feq peut
c     etre appele comme un external de lsode
c!
      implicit double precision (a-h,o-y)
      dimension tq(*),tqdot(*)
      dimension neq(*)
c
      
      nq=neq(1)
      ng=neq(2)
c
c     decoupage du tableau tq
      itq=1
      itg=itq+neq(3)+1
      iw=itg+ng+1
      ifree=iw+1+nq+ng

      call feq1(nq,t,tq,tq(itg),ng,tqdot,tq(iw))
      do 10 i=1,nq
         tqdot(i)=-tqdot(i)
 10   continue
      return
      end
      
      subroutine feq1(nq,t,tq,tg,ng,tqdot,tr)
      implicit double precision (a-h,o-y)
      dimension tq(nq+1),tqdot(nq),tg(*)
      dimension tr(nq+ng+1)
 
      
c
      do 199 i=1,nq
c
c     -- calcul du terme general --
c
         if (i.eq.1) then
            call lq(nq,tq,tr,tg,ng)
c     .     tlq =tr(1:nq); tvq =tr(nq+1:nq+ng+1)
            ltlq=1
            ltvq=nq+1
c
c     division de tvq par q            
            call dpodiv(tr(ltvq),tq,ng,nq)
            nv=ng-nq
         else
            ichoix=1
            call mzdivq(ichoix,nv,tr(ltvq),nq,tq)
         endif
c
c     calcul de tvq~ sur place
         nr=nq-1
         call tild(nr,tr(ltvq),tr)
         call calsca(nq,tq,tr,y0,tg,ng)
c
c     -- conclusion --
c
         tqdot(i)=-2.0d+0*y0
c
 199  continue
c      write(6,'(''tqdot='',5(e10.3,2x))') (tqdot(i),i=1,nq)
c
      return
      end
