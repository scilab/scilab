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
      subroutine frdf1(prosca,n,ntot,ninf,kgrad,
     & al,q,s,poids,aps,anc,mm1,r,e,ic,izs,rzs,dzs)
c
      implicit double precision (a-h,o-z)
      dimension al(ntot),q(*),poids(ntot),aps(ntot),anc(ntot),
     & ic(mm1),s(n),izs(*),dzs(*),e(mm1),r(*)
      external prosca
      real rzs(*)
c
c              this subroutine reduces a nonconvex bundle
c              of size ntot in rn
c              to a size no greater than ninf
c
      if(ntot.le.ninf) go to 900
      if (ninf.gt.0) go to 100
c
c           pure gradient method
c
      ntot=0
      kgrad=0
      go to 900
c
c          reduction to the corral
  100 nt1=0
      do 150 j=1,ntot
      if(al(j).eq.0.d0 .and. poids(j).ne.0.d0) go to 150
      nt1=nt1+1
      ic(nt1)=j
      if(j.eq.nt1) go to 130
      nj=n*(j-1)
      nn=n*(nt1-1)
      do 110 i=1,n
      nn=nn+1
      nj=nj+1
  110 q(nn)=q(nj)
      al(nt1)=al(j)
      poids(nt1)=poids(j)
      aps(nt1)=aps(j)
      anc(nt1)=anc(j)
      e(nt1+1)=e(j+1)
  130 if (poids(j).eq.0.) kgrad=nt1
      nn=nt1*mm1+1
      nj=j*mm1+1
      do 140 k=1,nt1
      njk=nj+ic(k)
      nn=nn+1
  140 r(nn)=r(njk)
  150 continue
      ntot=nt1
      if(ntot.le.ninf) go to 900
c
c          corral still too large
c              save the near
c
      call prosca (n,s,s,ps,izs,rzs,dzs)
      e(2)=1.d0
      z=0.d0
      z1=0.d0
      z2=0.d0
      do 370 k=1,ntot
      z1=z1+al(k)*aps(k)
      z2=z2+al(k)*anc(k)
  370 z=z+al(k)*poids(k)
      aps(1)=z1
      anc(1)=z2
      poids(1)=z
      r(mm1+2)=ps
      if (ninf.gt.1) go to 400
      ntot=1
      kgrad=0
      do 380 i=1,n
  380 q(i)=s(i)
      go to 900
c                save the gradient
  400 nn=(kgrad-1)*n
      do 470 i=1,n
      nj=n+i
      nn=nn+1
      q(nj)=q(nn)
  470 q(i)=s(i)
      call prosca(n,q(n+1),s,ps,izs,rzs,dzs)
      e(3)=1.d0
      r(2*mm1+2)=ps
      call prosca (n,q(n+1),q(n+1),ps,izs,rzs,dzs)
      r(2*mm1+3)=ps
      aps(2)=0.d0
      anc(2)=0.d0
      poids(2)=0.d0
      kgrad=2
      ntot=2
  900 return
      end
