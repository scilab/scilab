      subroutine icse0(nu,t0,tf,dti,dtf,ermx,iu,nuc,nuv,ilin,nti,ntf,
     & ny,nea,itmx,nex,nob,ntob,ntobi,nitu,ndtu,nitv,nrtv,ndtv)
c
c     programme d'initialisation appele par icse.bas
c     initialisation des commons icsez icsez0 et nird
c     Copyright INRIA
c
      implicit double precision (a-h,o-z)
      dimension iu(5),iu0(5)
      common/icsez/t00,tf0,dti0,dtf0,ermx0,iu0,nuc0,nuv0,ilin0,nti0,
     & ntf0,ny0,nea0,itmx0,nex0,nob0,ntob0,ntobi0,nitu0,ndtu0
      common/nird/nitv0,nrtv0,ndtv0
c
      t00=t0
      tf0=tf
      dti0=dti
      dtf0=dtf
      ermx0=ermx
      do 10 i=1,5
10    iu0(i)=iu(i)
      nuc0=nuc
      nuv0=nuv
      ilin0=ilin
      nti0=nti
      ntf0=ntf
      ny0=ny
      nea0=nea
      itmx0=itmx
      nex0=nex
      nob0=nob
      ntob0=ntob
      ntobi0=ntobi
      nitu0=nitu
      ndtu0=ndtu
      nitv0=0
      nrtv0=0
      ndtv0=0
      ind=0
      call icse(ind,nu,zz,zz,zz,zz,zz,zz,zz,zz,zz)
      nitv=max(1,nitv0)
      nrtv=max(1,nrtv0)
      ndtv=max(1,ndtv0)
      return
      end
