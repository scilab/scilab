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
      subroutine bfgsd(diag,n,nt,np,y,s,ys,condm,param,zero,index)
c     mise a jour de diag par la methode de bfgs diagonal
c     utiliser a la suite de la correction de powell
c     condm borne sup du conditionnement de diag
c     param borne inf rapport reduction diag(i)
c
      implicit double precision (a-h,o-z)
      dimension diag(n),y(nt,n),s(nt,n),ys(nt)
      integer index(nt)
c
      inp=index(np)
      ys1=1./ys(inp)
      sds=0.
      do 10 i=1,n
10    sds=sds + diag(i)*s(inp,i)**2
      sds1=1./sds
      dmin=1.e25
      dmax=0.
      do 20 i=1,n
      dd1=param*diag(i)
      dd1=dd1+1000.*zero
      dd=diag(i)+ys1*y(inp,i)**2-sds1*(diag(i)*s(inp,i))**2
      diag(i)=dd
c     sauvegarde positivite
      if(dd.le.dd1)diag(i)=dd1
c     calcul conditionnement
      if(diag(i).lt.dmin)dmin=diag(i)
      if(diag(i).gt.dmax)dmax=diag(i)
20    continue
c     limitation du conditionnement
      if((condm*dmin)/dmax.gt.1)return
      omeg=log(condm)/log(dmax/dmin)
      do 30 i=1,n
30    diag(i)=diag(i)**omeg
      return
      end
