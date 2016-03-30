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
c
      subroutine icsec2(indc,nu,tob,obs,cof,ytob,ob,u,
     & c,cy,g,yob,d,itu,dtu,
     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     & itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c
c
c     critere standard des moindres carres
c
c
c      Cout ponctuel :
c       Parametres d'entree :
c        indc     : 1 si on desire calculer c2,2 si on desire
c                   calculer c2y,c2u
c        tob      : instants de mesure
c        obs      : matrice d'observation
c        cof      : coefficients de ponderation du cout
c        ytob     : valeur de l'etat aux instants d'observation
c        ob       : mesures
c        u(nu)    : controle.Le controle variable est stocke a la
c                   suite du controle suite du constant.
c       Parametres de sortie :
c        indc     : comme pour icsec1
c        c2       : cout
c        c2y(ny,ntob) : derivee de c2 par rapport a y
c        g(nu)  : derivee de c2 par rapport a u
c
      implicit double precision (a-h,o-z)
      dimension tob(ntob),obs(nob,ny),cof(nob,ntob),ytob(ny,ntob),
     &ob(nex,ntob,nob),u(nu),cy(ny,ntob),g(nu),yob(nob,ntob),
     &d(nob),itu(nitu),dtu(ndtu),iu(5)
c
c     critere standard des moindres carres
c
      call dmmul(obs,nob,ytob,ny,yob,nob,nob,ny,ntob)
      if (indc.eq.1) then
         c=0.0d+0
         do 12 i=1,nob
            do 11 j=1,ntob
               do 10 k=1,nex
                  c=c+0.50d+0*cof(i,j)*(yob(i,j)-ob(k,j,i))**2
 10            continue
 11         continue
 12      continue
      else
         do 20 j=1,ntob
            do 25 i=1,nob
               d(i)=0.0d+0
               do 24 k=1,nex
                  d(i)=d(i)+cof(i,j)*(yob(i,j)-ob(k,j,i))
 24            continue
 25         continue
            call dmmul(d,1,obs,nob,cy(1,j),1,1,nob,ny)
 20      continue
      endif

      end
