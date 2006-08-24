      subroutine isova0(a,lda,m,n,path,kpath,ir,ic,dir,pend,
c     Copyright INRIA
     $     h,v,c)
c% but
c     Sous programme appele par le sous programme isoval
c%
      double precision a(lda,*),c,path(2,*)
      integer lda,m,n,h(m,*),v(m-1,*)
c     
      logical pend
      integer north,south,east,west,dir
      data north/0/,south/1/,east/2/,west/3/
      
c     extend the path at this level by one edge element
      
      if(dir.eq.north) then
         if(v(ir,ic).lt.0) then
            if(kpath.gt.1) h(ir,ic)=0
c     path to east
            goto 30
         else if(v(ir,ic+1).lt.0) then
            if(kpath.gt.1) h(ir,ic)=0
c     path to west
            goto 40
         else if(h(ir+1,ic).lt.0) then
            if(kpath.gt.1) h(ir,ic)=0
c     path to north
            goto 10
         else
            pend=.true.
         endif 
      else if(dir.eq.west) then
         if(h(ir+1,ic).lt.0) then
            if(kpath.gt.1) v(ir,ic)=0
c     path to north
            goto 10
         else if(h(ir,ic).lt.0) then
            if(kpath.gt.1) v(ir,ic)=0
c     path to south
            goto 20
         else if(v(ir,ic+1).lt.0) then
            if(kpath.gt.1) v(ir,ic)=0
c     path to west
            goto 40
         else
            pend=.true.
         endif
      else if( dir.eq.south) then
         if(v(ir,ic+1).lt.0) then
            if(kpath.gt.1) h(ir+1,ic)=0
c     path to west
            goto 40
         else if(v(ir,ic).lt.0) then
            if(kpath.gt.1) h(ir+1,ic)=0
c     path to east
            goto 30
         else if(h(ir,ic).lt.0) then
            if(kpath.gt.1) h(ir+1,ic)=0
c     path to south
            goto 20
         else
            pend=.true.
         endif
      else if(dir.eq.east) then
         if(h(ir,ic).lt.0) then
            if(kpath.gt.1) v(ir,ic+1)=0
c     path to south
            goto 20
         else if(h(ir+1,ic).lt.0) then
            if(kpath.gt.1) v(ir,ic+1)=0
c     path to north
            goto 10
         else if(v(ir,ic).lt.0) then
            if(kpath.gt.1) v(ir,ic+1)=0
c     path to east
            goto 30
         else
            pend=.true.
         endif
      endif
      return
c     
 10   continue
c     
c     NORTH
c     
      kpath=kpath+1
      path(2,kpath)=ir+1
      path(1,kpath)=ic+(c-a(ir+1,ic))/(a(ir+1,ic+1)-a(ir+1,ic))
      if(ir+1.lt.m) then
         ir=ir+1
         dir=north
      else
         pend=.true.
      endif
      return
 20   continue
c     
c     SOUTH
c     
      kpath=kpath+1
      path(2,kpath)=ir
      path(1,kpath)=ic+(c-a(ir,ic))/(a(ir,ic+1)-a(ir,ic))
      if(ir.gt.1) then
         ir=ir-1
         dir=south
      else
         pend=.true.
      endif
      return
c     
 30   continue
c     
c     EAST
c     
      kpath=kpath+1
      path(2,kpath)=ir+(c-a(ir,ic))/(a(ir+1,ic)-a(ir,ic))
      path(1,kpath)=ic
      if(ic.gt.1) then
         ic=ic-1
         dir=east
      else
         pend=.true.
      endif
      return
c     
 40   continue
c     
c     WEST
c     
      kpath=kpath+1
      path(2,kpath)=ir+(c-a(ir,ic+1))/(a(ir+1,ic+1)-a(ir,ic+1))
      path(1,kpath)=ic+1
      if(ic+1.lt.n) then
         ic=ic+1
         dir=west
      else
         pend=.true.
      endif
      return
c     
      end

