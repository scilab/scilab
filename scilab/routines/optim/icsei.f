      subroutine icsei(indi,nui,u,y0,y0u,itu,dtu,
     & t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,ny,nea,
     & itmx,nex,nob,ntob,ntobi,nitu,ndtu)
c
c     calcul de l'etat initial dans ICSE : cas standard : 
c     controle par l'etat initial 
c     Copyright INRIA
c!
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
