      subroutine n1qn3 (simul,prosca,ctonb,ctcab,n,x,f,g,dxmin,df1,
     /                  epsg,impres,io,mode,niter,nsim,dz,ndz,
     /                  izs,rzs,dzs)
c----
c
c     n1qn3, version 1.0, septembre 1988.
c     Jean Charles Gilbert, Claude Lemarechal, INRIA.
c     Copyright INRIA
c     email: Jean-Charles.Gilbert@inria.fr
c
c     Utilise les sous-routines:
c         n1qn3a
c         ddd2
c         nlis0 + dcube (XII/88)
c
c     La sous-routine n1qn3 est une interface entre le programme
c     appelant et la sous-routine n1qn3a, le minimiseur proprement dit.
c
c     Le module prosca est sense realiser le produit scalaire de deux
c     vecteurs de Rn; le module ctonb est sense realiser le changement
c     bases: base euclidienne -> base orthonormale (pour le produit
c     scalaire prosca); le module ctbas fait la transformation inverse:
c     base orthonormale -> base euclidienne.
c
c     dz est la zone de travail pour n1qn3a, de dimension ndz.
c     Elle est subdivisee en
c         4 vecteurs de dimension n: d,gg,diag,aux
c         m scalaires: alpha
c         m vecteurs de dimension n: ybar
c         m vecteurs de dimension n: sbar
c
c     m est alors le plus grand entier tel que
c         m*(2*n+1)+4*n .le. ndz,
c     soit m := (ndz-4*n) / (2*n+1)
c     Il faut avoir m >= 1, donc ndz >= 6n+1.
c
c     A chaque iteration la metrique est formee a partir d'une matrice
c     diagonale D qui est mise a jour m fois par la formule de BFGS en
c     utilisant les m couples {y,s} les plus recents. La matrice
c     diagonale est egale apres la premiere iteration a
c         (y,s)/|y|**2 * identite (facteur d'Oren-Spedicato)
c     et est elle-meme mise a jour a chaque iteration en utilisant la
c     formule de BFGS directe diagonalisee adaptee a l'ellipsoide de
c     Rayleigh. Si on note
c         D[i]:=(De[i],e[i]), y[i]:=(y,e[i]), s[i]:=(s,e[i]),
c     ou les e[i] forment une base orthonormale pour le produit scalaire
c     (.,.) que realise prosca, la formule de mise a jour de D s'ecrit:
c         D[i] := 1 / ( (Dy,y)/(y,s)/D[i] + y[i]**2/(y,s)
c                        - (Dy,y)*(s[i]/D[i])**2/(y,s)/(D**(-1)s,s) )
c
c----
c
c         arguments
c
      integer n,impres,io,mode,niter,nsim,ndz,izs(1)
      real rzs(1)
      double precision x(1),f,g(1),dxmin,df1,epsg,dz(1),dzs(1)
      external simul,prosca,ctonb,ctcab
c
c         variables locales
c
      integer ntravu,l1memo,id,igg,iprec,iaux,ialpha,iybar,isbar,m
      double precision r1,r2
      double precision ps
c
c---- impressions initiales et controle des arguments
c
      if (impres.ge.1)
     /    write (io,900) n,dxmin,df1,epsg,niter,nsim,impres
900   format (/" n1qn3: entry point"/
     /    5x,"dimension of the problem (n):",i6/
     /    5x,"absolute precision on x (dxmin):",d9.2/
     /    5x,"expected decrease for f (df1):",d9.2/
     /    5x,"relative precision on g (epsg):",d9.2/
     /    5x,"maximal number of iterations (niter):",i6/
     /    5x,"maximal number of simulations (nsim):",i6/
     /    5x,"printing level (impres):",i4)
      if (n.le.0.or.niter.le.0.or.nsim.le.0.or.dxmin.le.0.d0
     /    .or.epsg.le.0.d0.or.epsg.gt.1.d0) then
          mode=2
          if (impres.ge.1) write (io,901)
901       format (/" >>> n1qn3: inconsistent call")
          return
      endif
      if (ndz.lt.6*n+1) then
          mode=2
          if (impres.ge.1) write (io,902)
902       format (/" >>> n1qn3: not enough memory allocated")
          goto 904
      endif
c
c---- calcul de m et des pointeurs subdivisant la zone de travail dz
c
      ntravu=ndz-4*n
      l1memo=2*n+1
      m=ntravu/l1memo
      ntravu=m*l1memo+4*n
      if (impres.ge.1) write (io,903) ndz,ntravu,m
903   format (/,5x,"allocated memory (nrz) :",i7,/,
     /         5x,"used memory :           ",i7,/,
     /         5x,"number of updates :     ",i7,/,1x)
      id=1
      igg=id+n
      iprec=igg+n
      iaux=iprec+n
      ialpha=iaux+n
      iybar=ialpha+m
      isbar=iybar+n*m
c
c---- appel du code d"optimisation
c
      call n1qn3a (simul,prosca,ctonb,ctcab,n,x,f,g,dxmin,df1,epsg,
     /             impres,io,mode,niter,nsim,m,dz(id),dz(igg),dz(iprec),
     /             dz(iaux),dz(ialpha),dz(iybar),dz(isbar),izs,rzs,dzs)
c
c---- impressions finales
c
904   continue
      if (impres.ge.1) write (io,905) mode,niter,nsim,epsg
905   format (/,1x,79("-"),/,
     /        /," n1qn3: output mode is ",i2,
     /        /,5x,"number of iterations: ",i4,
     /        /,5x,"number of simulations: ",i6,
     /        /,5x,"realized relative precision on g: ",d9.2)
      call prosca (n,x,x,ps,izs,rzs,dzs)
      r1=sqrt(ps)
      call prosca (n,g,g,ps,izs,rzs,dzs)
      r2=sqrt(ps)
      if (impres.ge.1) write (io,906) r1,f,r2
906   format (5x,"norm of x = ",d15.8,
     /       /,5x,"f         = ",d15.8,
     /       /,5x,"norm of g = ",d15.8)
      return
      end
