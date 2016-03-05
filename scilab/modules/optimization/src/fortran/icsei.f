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
      subroutine icsei(indi,nui,u,y0,y0u,itu,dtu,
     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     & itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c
c     calcul de l'etat initial dans ICSE : cas standard : 
c     controle par l'etat initial 
c
      implicit double precision (a-h,o-z)
      dimension u(nui),y0(ny),y0u(ny,nui),itu(nitu),dtu(ndtu),iu(5)
c
      if (indi.eq.1) then
        do 10 i=1,ny
           y0(i)=u(i)
 10     continue
      endif
c
      if (indi.eq.2) then
c       cas ou y0u est l identite
        call  dset(ny*nui,0.0d+0,y0u,1)
        do 20 i=1,ny
           y0u(i,i)=1.0d+0
 20     continue
      endif
      end
