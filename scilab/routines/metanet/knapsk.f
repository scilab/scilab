c     algorithm 632 collected algorithms from acm.
c     algorithm appeared in acm-trans. math. software, vol.11, no. 2,
c     jun., 1985, p. 135-140.
      subroutine knapsk(n,m,np1,p,w,k,xstar,vstar,bck,
     + lx,lxi,bs,ps,ws,xs,bb,bl,x,xl,b,ubb,f,pbl,q,v,
     + d,min,pbar,wbar,zbar)
      integer p(n),w(n),k(m),xstar(n),vstar,bck
      integer lx(n),lxi(n),lr,lri,lubi
      integer bs(n),ps(np1),ws(np1),xs(n)
      integer bb(m,n),bl(m,np1),x(m,n),xl(m,n)
      integer b(np1),ubb(n)
      integer f(m),pbl(m),q(m),v(m),s,u,ub,vb
      integer d(n),min(n),pbar(n),wbar(n),zbar(n)
c step 0 (check on the input data)
      vstar = 0
      do 1,i=1,n
         xstar(i)=0
 1    continue
      if ( n .le. 1 ) vstar = - 1
      if ( m .le. 0 ) vstar = - 1
      if ( vstar .lt. 0 ) return
      maxw = w(1)
      minw = w(1)
      isumw = w(1)
      ap = p(1)
      aw = w(1)
      rr = ap/aw
      if ( p(1) .le. 0 ) vstar = - 2
      if ( w(1) .le. 0 ) vstar = - 2
      do 10 j=2,n
        if ( p(j) .le. 0 ) vstar = - 2
        if ( w(j) .le. 0 ) vstar = - 2
        r = rr
        if ( w(j) .gt. maxw ) maxw = w(j)
        if ( w(j) .lt. minw ) minw = w(j)
        isumw = isumw + w(j)
        ap = p(j)
        aw = w(j)
        rr = ap/aw
        if ( rr .le. r ) go to 10
        vstar = - 6
        return
   10 continue
      if ( k(1) .le. 0 ) vstar = - 2
      if ( m .eq. 1 ) go to 250
      do 20 i=2,m
        if ( k(i) .le. 0 ) vstar = - 2
        if ( k(i) .ge. k(i-1) ) go to 20
        vstar = - 7
        return
   20 continue
      if ( minw .gt. k(1) ) vstar = - 3
      if ( maxw .gt. k(m) ) vstar = - 4
      if ( isumw .le. k(m) ) vstar = - 5
      if ( vstar .lt. 0 ) return
c step 1 (initialization)
      jbck = bck
      bck = 0
      kub = 0
      n1 = n + 1
      b(n1) = 1
      m1 = m - 1
      do 40 j=1,n
        b(j) = 1
        do 30 i=1,m
          x(i,j) = 0
          bb(i,j) = 0
   30   continue
   40 continue
      do 50 i=1,m1
        q(i) = k(i)
        f(i) = -1
   50 continue
      q(m) = k(m)
      vstar = 0
      vb = 0
      i = 1
      call sigma(n,m,p,w,k,1,b,kub,ub
     +,np1,bs,ps,ws,xs,lx,lxi,lr,lri,lubi,
     + d,min,pbar,wbar,zbar)
      do 60 j=1,n
        lxi(j) = lx(j)
   60 continue
      lri = lr
      lubi = ub
      iflag = 0
c step 2 (heuristic)
   70 kub = vstar - vb
      call pi(n,m,p,w,q,i,b,bb,kub,bl,lb,pbl,v,xl,
     + np1,bs,ps,ws,xs,lx,lxi,lr,lri,lubi,
     + d,min,pbar,wbar,zbar)
      if ( lb + vb .le. vstar ) go to 140
      vstar = lb + vb
      do 90 j=1,n
        xstar(j) = 0
        do 80 s=1,i
          if ( x(s,j) .eq. 0 ) go to 80
          xstar(j) = s
          go to 90
   80   continue
   90 continue
      ip = pbl(i)
      if ( ip .eq. 0 ) go to 110
      do 100 j=1,ip
        jj = bl(i,j)
        if ( xl(i,j) .eq. 1 ) xstar(jj) = i
  100 continue
  110 i1 = i + 1
      do 130 ii=i1,m
        ip = pbl(ii)
        if ( ip .eq. 0 ) go to 130
        do 120 j=1,ip
          jj = bl(ii,j)
          if ( xl(ii,j) .eq. 1 ) xstar(jj) = ii
  120   continue
  130 continue
      if ( ub .eq. lb ) go to 200
c step 3 (updating)
  140 if ( v(i) .eq. 0 ) go to 180
      iuv = ub + vb
      u = pbl(i)
      ibv = 0
      do 170 s=1,u
        if ( xl(i,s) .eq. 0 ) go to 170
        j = bl(i,s)
        x(i,j) = 1
        q(i) = q(i) - w(j)
        vb = vb + p(j)
        b(j) = 0
        bb(i,j) = f(i)
        ubb(j) = iuv
        if ( iflag .eq. 1 ) go to 150
        lub = iuv
        lj = j
        li = i
  150   f(i) = j
        ibv = ibv + p(j)
        if ( ibv .eq. v(i) ) go to 180
        call par(i,i,ub,iflag,vb,lub,lj,li,f,bb,q,b,n
     +, m,np1,lx,lxi,lr,lri,lubi)
        if ( iflag .eq. 1 ) go to 160
        kub = vstar - vb
        call sigma(n,m,p,w,q,i,b,kub,ub
     +,np1,bs,ps,ws,xs,lx,lxi,lr,lri,lubi,
     + d,min,pbar,wbar,zbar)
        lj = n1
  160   iuv = ub + vb
        if ( iuv .le. vstar ) go to 200
  170 continue
  180 if ( i .eq. m - 1 ) go to 200
      ip1 = i + 1
      call par(ip1,i,ub,iflag,vb,lub,lj,li,f,bb,q,b,n
     +, m,np1,lx,lxi,lr,lri,lubi)
      if ( iflag .eq. 1 ) go to 190
      kub = vstar - vb
      call sigma(n,m,p,w,q,ip1,b,kub,ub
     +,np1,bs,ps,ws,xs,lx,lxi,lr,lri,lubi,
     + d,min,pbar,wbar,zbar)
      lj = n1
  190 if ( ub + vb .le. vstar ) go to 200
      i = i + 1
      go to 140
c
c step 4 (backtracking)
c
  200 if ( i .gt. 0 ) go to 210
      bck = bck - 1
      return
  210 if ( bck .eq. jbck ) return
      bck = bck + 1
      if ( f(i) .ne. (-1) ) go to 230
      do 220 j=1,n
        bb(i,j) = 0
  220 continue
      i = i - 1
      go to 200
  230 j = f(i)
      x(i,j) = 0
      b(j) = 1
      vb = vb - p(j)
      q(i) = q(i) + w(j)
      do 240 s=1,n
        if ( bb(i,s) .eq. j ) bb(i,s) = 0
  240 continue
      f(i) = bb(i,j)
      if ( ubb(j) .le. vstar ) go to 200
      ub = ubb(j) - vb
      iflag = 1
      go to 70
c particular case ( 0-1 single knapsack problem)
  250 if ( maxw .gt. k(1) ) vstar = - 4
      if ( isumw .le. k(1) ) vstar = - 5
      if ( vstar .lt. 0 ) return
      k1 = k(1)
      do 260 j=1,n
        ps(j) = p(j)
        ws(j) = w(j)
  260 continue
      call skp(n,k1,0,vstar, n,m,np1,bs,ps,ws,xs,d,min,pbar,wbar,zbar)
      do 270 j=1,n
        xstar(j) = xs(j)
  270 continue
      bck = 0
      return
      end
      subroutine sigma(n,m,p,w,q,i,b,kub,ub
     +,np1,bs,ps,ws,xs,lx,lxi,lr,lri,lubi,
     + d,min,pbar,wbar,zbar)
      integer p(n),w(n),q(m),b(np1),ub
      integer qs,sb
      integer lx(n),lxi(n),lr,lri,lubi
      integer bs(n),ps(np1),ws(np1),xs(n)
      integer d(n),min(n),pbar(n),wbar(n),zbar(n)
      ns = 0
      qs = 0
      do 10 j=i,m
        qs = qs + q(j)
   10 continue
      sb = 0
      do 20 j=1,n
        lx(j) = 0
        if ( b(j) .eq. 0 ) go to 20
        ns = ns + 1
        bs(ns) = j
        ps(ns) = p(j)
        ws(ns) = w(j)
        sb = sb + w(j)
   20 continue
      if ( sb .gt. qs ) go to 40
      lr = qs - sb
      ub = 0
      if ( ns .eq. 0 ) return
      do 30 j=1,ns
        ub = ub + ps(j)
        xs(j) = 1
   30 continue
      go to 50
   40 call skp(ns,qs,kub,ub, n,m,np1,bs,ps,ws,xs,d,min,pbar,wbar,zbar)
      lr = qs
   50 do 60 j=1,ns
        jj = bs(j)
        lx(jj) = xs(j)
   60 continue
      return
      end
      subroutine pi(n,m,p,w,q,i,b,bb,kub,bl,lb,pbl,v,xl,
     + np1,bs,ps,ws,xs,lx,lxi,lr,lri,lubi,
     + d,min,pbar,wbar,zbar)
      integer bb(m,n),bl(m,np1),xl(m,n)
      integer p(n),w(n),q(m),b(np1),pbl(m),v(m)
      integer pb,qs,sb,u
      integer lx(n),lxi(n),lr,lri,lubi
      integer bs(n),ps(np1),ws(np1),xs(n)
      integer d(n),min(n),pbar(n),wbar(n),zbar(n)
c step 1
      u = 0
      do 10 j=1,n
        if ( b(j) .eq. 0 ) go to 10
        u = u + 1
        bs(u) = j
   10 continue
      do 20 j=i,m
        pbl(j) = 0
        v(j) = 0
   20 continue
      lb = 0
      ikub = kub
      if ( u .eq. 0 ) return
      ns = 0
      sb = 0
      do 30 j=1,u
        jj = bs(j)
        if ( bb(i,jj) .ne. 0 ) go to 30
        if ( w(jj) .gt. q(i) ) go to 30
        ns = ns + 1
        sb = sb + w(jj)
        bl(i,ns) = jj
        ps(ns) = p(jj)
        ws(ns) = w(jj)
   30 continue
      ii = i
c step 2
   40 pbl(ii) = ns
      if ( sb .gt. q(ii) ) go to 60
      pb = 0
      if ( ns .eq. 0 ) go to 80
      do 50 j=1,ns
        pb = pb + ps(j)
        xl(ii,j) = 1
   50 continue
      go to 80
   60 qs = q(ii)
      kub = 0
      if ( ii .eq. m ) kub = ikub
      call skp(ns,qs,kub,pb, n,m,np1,bs,ps,ws,xs,d,min,pbar,wbar,zbar)
      do 70 j=1,ns
        xl(ii,j) = xs(j)
   70 continue
   80 lb = lb + pb
      ikub = ikub - pb
      v(ii) = pb
      bl(ii,ns+1) = n + 1
c step 3
      if ( ii .eq. m ) return
      jb = 1
      jbs = 0
      do 100 j=1,u
        if ( bs(j) .lt. bl(ii,jb) ) go to 90
        jb = jb + 1
        if ( xl(ii,jb-1) .eq. 1 ) go to 100
   90   jbs = jbs + 1
        bs(jbs) = bs(j)
  100 continue
      u = jbs
      if ( u .eq. 0 ) return
      ns = 0
      sb = 0
      ii = ii + 1
      do 110 j=1,u
        jj = bs(j)
        if( w(jj) .gt. q(ii) ) go to 110
        ns = ns + 1
        sb = sb + w(jj)
        bl(ii,ns) = jj
        ps(ns) = p(jj)
        ws(ns) =  w(jj)
  110 continue
      go to 40
      end
      subroutine par(i,ii,ub,iflag,vb,lub,lj,li,f,bb,q,b,n
     +, m,np1,lx,lxi,lr,lri,lubi)
      integer f(m),bb(m,n),q(m),b(np1),ub,vb,r,s
      integer lx(n),lxi(n),lr,lri,lubi
      iflag = 0
      if ( b(lj) .ne. 0 ) go to 60
      i1 = i - 1
      if ( i1 .lt. li ) go to 20
      iq = 0
      do 10 r=li,i1
        iq = iq + q(r)
   10 continue
      if ( iq .gt. lr ) return
   20 r = ii
      s = f(r)
   30 if ( s .ne. (-1) ) go to 40
      r = r - 1
      s = f(r)
      go to 30
   40 if ( lx(s) .eq. 0 ) return
      if ( s .eq. lj ) go to 50
      s = bb(r,s)
      go to 30
   50 ub = lub - vb
      iflag = 1
      return
   60 i1 = i - 1
      if ( i1 .lt. 1 ) go to 80
      iq = 0
      do 70 r=1,i1
        iq = iq + q(r)
   70 continue
      if ( iq .gt. lri ) return
   80 do 90 j=1,n
        if ( b(j) .eq. 1 ) go to 90
        if ( lxi(j) .eq. 0 ) return
   90 continue
      ub = lubi - vb
      iflag = 1
      return
      end
      subroutine skp(ns,qs,kub,vs,n,m,np1,bs,ps,ws,xs,
     +d,min,pbar,wbar,zbar)
      integer qs,vs,diff,pr,r,t
      integer d(n),min(n),pbar(n),wbar(n),zbar(n)
      integer bs(n),ps(np1),ws(np1),xs(n)
      vs = kub
      ip = 0
      ms = qs
      do 10 l=1,ns
        ll = l
        if ( ws(l) .gt. ms ) go to 20
        ip = ip + ps(l)
        ms = ms - ws(l)
   10 continue
   20 ll = ll - 1
      if ( ms .eq. 0 ) go to 50
      ps(ns+1) = 0
      ws(ns+1) = qs + 1
      lim = ip + ms*ps(ll+2)/ws(ll+2)
      a = ip + ps(ll+1)
      b = (ws(ll+1) - ms)*ps(ll)
      c = ws(ll)
      lim1 = a - b/c
      if ( lim1 .gt. lim ) lim = lim1
      if ( lim .le. vs ) return
      mink = qs + 1
      min(ns) = mink
      do 30 j=2,ns
        kk = ns + 2 - j
        if ( ws(kk) .lt. mink ) mink = ws(kk)
        min(kk-1) = mink
   30 continue
      do 40 j=1,ns
        d(j) = 0
   40 continue
      pr = 0
      lold = ns
      ii = 1
      go to 170
   50 if ( vs .ge. ip ) return
      vs = ip
      do 60 j=1,ll
        xs(j) = 1
   60 continue
      nn = ll + 1
      do 70 j=nn,ns
        xs(j) = 0
   70 continue
      qs = 0
      return
   80 if ( ws(ii) .le. qs ) go to 90
      ii1 = ii + 1
      if ( vs .ge. qs*ps(ii1)/ws(ii1) + pr ) go to 280
      ii = ii1
      go to 80
   90 ip = pbar(ii)
      ms = qs - wbar(ii)
      in = zbar(ii)
      ll = ns
      if ( in .gt. ns) go to 110
      do 100 l=in,ns
        ll = l
        if ( ws(l) .gt. ms ) go to 160
        ip = ip + ps(l)
        ms = ms - ws(l)
  100 continue
  110 if ( vs .ge. ip + pr ) go to 280
      vs = ip + pr
      mfirst = ms
      nn = ii - 1
      do 120 j=1,nn
        xs(j) = d(j)
  120 continue
      do 130 j=ii,ll
        xs(j) = 1
  130 continue
      if ( ll .eq. ns ) go to 150
      nn = ll + 1
      do 140 j=nn,ns
        xs(j) = 0
  140 continue
  150 if ( vs .ne. lim ) go to 280
      qs = mfirst
      return
  160 l = ll
      ll = ll - 1
      if ( ms .eq. 0 ) go to 110
      if ( vs .ge. pr + ip + ms*ps(l)/ws(l) ) go to 280
  170 wbar(ii) = qs - ms
      pbar(ii) = ip
      zbar(ii) = ll + 1
      d(ii) = 1
      nn = ll - 1
      if ( nn .lt. ii ) go to 190
      do 180 j=ii,nn
        wbar(j+1) = wbar(j) - ws(j)
        pbar(j+1) = pbar(j) - ps(j)
        zbar(j+1) = ll + 1
        d(j+1) = 1
  180 continue
  190 j1 = ll + 1
      do 200 j=j1,lold
        wbar(j) = 0
        pbar(j) = 0
        zbar(j) = j
  200 continue
      lold = ll
      qs = ms
      pr = pr + ip
      CRES=ll - (ns - 2) 
      if (CRES .lt. 0) then 
         goto 240
      elseif (CRES .eq. 0) then
         goto 220
      else
         goto 210
      endif
  210 ii = ns
      go to 250
  220 if ( qs .lt. ws(ns) ) go to 230
      qs = qs - ws(ns)
      pr = pr + ps(ns)
      d(ns) = 1
  230 ii = ns - 1
      go to 250
  240 ii = ll + 2
      if ( qs .ge. min(ii-1) ) go to 80
  250 if ( vs .ge. pr ) go to 270
      vs = pr
      do 260 j=1,ns
        xs(j) = d(j)
  260 continue
      mfirst = qs
      if ( vs .eq. lim ) return
  270 if ( d(ns) .eq. 0 ) go to 280
      d(ns) = 0
      qs = qs + ws(ns)
      pr = pr - ps(ns)
  280 nn = ii - 1
      if ( nn .eq. 0 ) go to 300
      do 290 j=1,nn
        kk = ii - j
        if ( d(kk) .eq. 1 ) go to 310
  290 continue
  300 qs = mfirst
      return
  310 r = qs
      qs = qs + ws(kk)
      pr = pr - ps(kk)
      d(kk) = 0
      if ( r .lt. min(kk) ) go to 320
      ii = kk + 1
      go to 80
  320 nn = kk + 1
      ii = kk
  330 if ( vs .ge. pr + qs*ps(nn)/ws(nn) ) go to 280
      diff = ws(nn) - ws(kk)
      if (diff .lt. 0) then 
         goto 390
      elseif (diff .eq. 0) then
         goto 340
      else
         goto 350
      endif
  340 nn = nn + 1
      go to 330
  350 if ( diff .gt. r ) go to 340
      if ( vs .ge. pr + ps(nn) ) go to 340
      vs = pr + ps(nn)
      do 360 j=1,kk
        xs(j) = d(j)
  360 continue
      jj = kk + 1
      do 370 j=jj,ns
        xs(j) = 0
  370 continue
      xs(nn) = 1
      mfirst = qs - ws(nn)
      if ( vs .ne. lim ) go to 380
      qs = mfirst
      return
  380 r = r - diff
      kk = nn
      nn = nn + 1
      go to 330
  390 t = r - diff
      if ( t .lt. min(nn) ) go to 340
      nbid = nn + 1
      if ( vs .ge. pr + ps(nn) + t*ps(nbid)/ws(nbid) ) go to 280
      qs = qs - ws(nn)
      pr = pr + ps(nn)
      d(nn) = 1
      ii = nn + 1
      wbar(nn) = ws(nn)
      pbar(nn) = ps(nn)
      zbar(nn) = ii
      n1 = nn + 1
      do 400 j=n1,lold
        wbar(j) = 0
        pbar(j) = 0
        zbar(j) = j
  400 continue
      lold = nn
      go to 80
      end
