      subroutine permuto(n,n3,n4,c,f,d, crit,loc3n,work4n)
      implicit doubleprecision (a-h,o-z)
      dimension c(n,n),f(n,n),d(n,n),loc3n(n3),work4n(n4)
      dimension bp(7),bpn(7),bpb(7)
c      common /hgwc/ bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3
      crit=0.d0
      do 1,i=1,n3
         loc3n(i)=0
 1    continue
      nc=n
      nf=n                                                
      nd=n      
      call hgw(n, c, f, d, nc, nf, nd, loc3n, work4n, 
     +bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3)
      crit=work4n(1)
      return
      end   
      subroutine hgw(n,c,f,d,nc,nf,nd,loc3n,work4n, 
     +bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3)
      implicit doubleprecision (a-h,o-z)
      dimension c(nc,1),f(nf,1),d(nd,1),loc3n(1),work4n(1)
      dimension bp(7),bpn(7),bpb(7)
      ibest=1
      jbest=1
c
      if (n.le.1) return
      n1 = n
      n2 = n + n
      n3 = 3*n
c     initialize recurrence data
      call initrd(c,f,d,nc,nf,nd,pnorm,loc3n(n1+1),loc3n(n2+1),
     * work4n(1), work4n(n1+1), work4n(n2+1), work4n(n3+1), 
     +bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3)
      obj = (bp(3)*rfnk1+bp(5))*rfnk
      nm1 = n - 1
c
      do 30 kp1=1,nm1
        k = kp1 - 1
c        select candidates for position kp1 in s' and l' to give a
c        maximal reduction in the average objective function.
        delbst = pnorm
c        delbst need only be larger than the rounding error in delta
        do 20 i=kp1,n
          is = n1 + i
          ibar = loc3n(is)
c
          do 10 j=kp1,n
            jl = n2 + j
c              pre-evaluate effect on obj of assignment ibar<->loc3n(jl)
            temp = delta(ibar,loc3n(jl),c,f,d,nc,nf,nd,loc3n(n1+1),
     *       loc3n(n2+1),work4n(1),work4n(n1+1),work4n(n2+1),
     *       work4n(n3+1),
     +bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3)
            if (temp.ge.delbst) go to 10
c              got an improvement,so save it
            ibest = i
            jbest = j
            delbst = temp
            bpb(1) = bpn(1)
            bpb(2) = bpn(2)
            bpb(3) = bpn(3)
            bpb(4) = bpn(4)
            bpb(5) = bpn(5)
            bpb(6) = bpn(6)
            bpb(7) = bpn(7)
   10     continue
c
   20   continue
c        exchange best items into position kp1
        kp1s = n1 + kp1
        kp1l = n2 + kp1
        ibests = n1 + ibest
        jbestl = n2 + jbest
        itemp = loc3n(kp1s)
        loc3n(kp1s) = loc3n(ibests)
        loc3n(ibests) = itemp
        jtemp = loc3n(kp1l)
        loc3n(kp1l) = loc3n(jbestl)
        loc3n(jbestl) = jtemp
        obj = obj + delbst
c        update recurrence data
        if (kp1.ne.nm1) call updrd(f, d, nf, nd, loc3n(n1+1),
     *   loc3n(n2+1), work4n(1), work4n(n1+1), work4n(n2+1),
     *   work4n(n3+1), 
     +bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3)
   30 continue
c
c the final iteration, with k=n-1, is not performed since there is only one
c candidate pair and it is already in place. now unscramble the permutation.
c
      do 40 i=1,n
        il = n2 + i
        is = n1 + i
        ls = loc3n(is)
        loc3n(ls) = loc3n(il)
   40 continue
c the construction phase is now complete.  the remaining code tries to 
c improve the current approximate solution using pair exchanges.if the 
c user wishes to trade solution quality for speed of computation, the
c following statements up to and including label 90 may be omitted.
      do 70 m=1,n
c        at most n times
        delbst = 0.d0
        do 60 i=2,n
          im1 = i - 1
c
          do 50 j=1,im1
            temp = deltx(i,j,c,f,d,nc,nf,nd,loc3n,
     +bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3)
            if (temp.ge.delbst) go to 50
c              got an improvement,so save it
            ibest = i
            jbest = j
            delbst = temp
   50     continue
   60   continue
        if (delbst.ge.0.d0) go to 80
c        perform exchange
        li = loc3n(ibest)
        lj = loc3n(jbest)
        loc3n(ibest) = lj
        loc3n(jbest) = li
        obj = obj + delbst
   70 continue
c
   80 continue
      work4n(1) = obj
      return
      end
      doubleprecision function delta(ibar,jbar,c,f,d,nc,nf,nd,ls,ll,w1,
     + w2,w3,w4,bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3)
      implicit doubleprecision (a-h,o-z)
      dimension c(nc,1),f(nf,1),d(nd,1),ls(1),ll(1),w1(1),w2(1),
     * w3(1), w4(1)
      dimension bp(7),bpn(7),bpb(7)
c  called by the quadratic assignment heuristic hgw. returns the
c  change in obj  (new - old) that would be produced by transferring
c  ibar from s to s' and jbar from l to l'. these transfers are only 
c  virtual.  on exit bpn contains the corresponding new values of bp. f 
c  and d are not assumed symmetric. assert:k.le.n1-2
      db1 = 0.d0
      dbp2 = w1(ibar)*w2(jbar) + w3(ibar)*w4(jbar)
      if (k.eq.0) go to 20
      do 10 m=1,k
        m1 = ls(m)
        lm1 = ll(m)
        f1 = f(ibar,m1)
        f2 = f(m1,ibar)
        d1 = d(jbar,lm1)
        d2 = d(lm1,jbar)
        db1 = db1 + f1*d1 + f2*d2
        dbp2 = dbp2 - f2*w2(lm1) - f1*w4(lm1) - w1(m1)*d2 - w3(m1)*d1
   10 continue
      dbp2 = dbp2 + db1
   20 continue
c     the average objective function, obj, for the current partial
c     permutation is calculable from the contents of the array bp,
c     although we do not do so explicitly.  we now construct delta, the
c     anticipated change in obj, from the corresponding change in bp.
      bpn(1) = bp(1) + db1
      bpn(2) = bp(2) + dbp2
      bpn(6) = bp(6) - w1(ibar) - w3(ibar)
      bpn(7) = bp(7) - w2(jbar) - w4(jbar)
      bpn(3) = bpn(6)*bpn(7)
      if (k.ge.n1-2) bpn(3) = 0.d0
      bpn(4) = bp(4) + c(ibar,jbar)
      dbp5 = c(ibar,jbar)
      kp1 = k + 1
c
      do 30 m=kp1,n1
        m1 = ls(m)
        lm1 = ll(m)
        dbp5 = dbp5 - c(ibar,lm1) - c(m1,jbar)
   30 continue
c
      bpn(5) = bp(5) + dbp5
      delta = db1 + c(ibar,jbar) - rfnk*(bp(2)+bp(5)) +
     * rfnk1*((bpn(3)*rfnk2-bp(3)*rfnk)+(bpn(2)+bpn(5)))
      return
      end
      subroutine initrd(c,f,d,nc,nf,nd,pnorm,ls,ll,w1,w2,w3,w4, 
     +bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3)
      implicit doubleprecision (a-h,o-z)
      dimension c(nc,1),f(nf,1),d(nd,1),ls(1),ll(1),w1(1),w2(1),
     * w3(1), w4(1)
      dimension bp(7),bpn(7),bpb(7)
c     initialize recurrence data
c     called by the quadratic assignment heuristic hgw.
      scsl = 0.d0
      sfs = 0.d0
      sdl = 0.d0
      cmx = 0.d0
      fmx = 0.d0
      dmx = 0.d0
      rfnk = 1.0d0/dble(n1)
      rfnk1 = 0.d0
      rfnk1 = 1.0d0/dble(n1-1)
      rfnk2 = 0.d0
      if (n1.gt.2) rfnk2 = 1.0d0/dble(n1-2)
c
      do 20 i=1,n1
        ls(i) = i
        ll(i) = i
c        initialize row and col sums
        fr = 0.d0
        fc = 0.d0
        dr = 0.d0
        dc = 0.d0
c
        do 10 j=1,n1
          scsl = scsl + c(i,j)
          cmx = max(cmx,abs(c(i,j)))
          fr = fr + f(i,j)
          fmx = max(fmx,abs(f(i,j)))
          fc = fc + f(j,i)
          dr = dr + d(i,j)
          dmx = max(dmx,abs(d(i,j)))
          dc = dc + d(j,i)
   10   continue
c
        sfs = sfs + fr
        sdl = sdl + dr
        w1(i) = fr
        w3(i) = fc
        w2(i) = dr
        w4(i) = dc
   20 continue
c
      bp(1) = 0.d0
      bp(2) = 0.d0
      bp(3) = sfs*sdl
      bp(4) = 0.d0
      bp(5) = scsl
      bp(6) = sfs
      bp(7) = sdl
      pnorm = cmx + fmx*dmx
c     pnorm is an upper bound on the error in evaluating obj if n1 is
c     less than the reciprocal of the machine precision
      return
      end
      subroutine updrd(f,d,nf,nd,ls,ll,w1,w2,w3,w4, 
     +bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3)
      implicit doubleprecision (a-h,o-z)      
      dimension f(nf,1),d(nd,1),ls(1),ll(1),w1(1),w2(1),w3(1),w4(1)
      dimension bp(7),bpn(7),bpb(7)
c  update recurrence data just before k is incremented called by  
c  the quadratic assignment heuristic hgw.   assert: k<=n1-2
      kp1 = k + 1
      kp11 = ls(kp1)
      lkp1 = ll(kp1)
      do 10 i=1,n1
        w1(i) = w1(i) - f(i,kp11)
        w3(i) = w3(i) - f(kp11,i)
        w2(i) = w2(i) - d(i,lkp1)
        w4(i) = w4(i) - d(lkp1,i)
   10 continue
c
      bp(1) = bpb(1)
      bp(2) = bpb(2)
      bp(3) = bpb(3)
      bp(4) = bpb(4)
      bp(5) = bpb(5)
      bp(6) = bpb(6)
      bp(7) = bpb(7)
      rfnk = rfnk1
      rfnk1 = rfnk2
      rfnk2 = 0.d0
      if (kp1.lt.n1-2) rfnk2 = 1.0d0/dble(n1-kp1-2)
c     next value of k is kp1
      return
      end
      doubleprecision function deltx(i, j, c, f, d, nc, nf, nd, loc3n,
     +bp,bpn,bpb,rfnk,rfnk1,rfnk2,k,n1,n2,n3)  
      implicit doubleprecision (a-h,o-z)      
      dimension c(nc,1), f(nf,1), d(nd,1), loc3n(1)
      dimension bp(7),bpn(7),bpb(7)
c  called by the quadratic assignment heuristic hgw.  returns the change 
c  (new - old) in the hgw objective function that would be produced if the
c  existing assignment pairs (i<->li) and (j<->lj) were replaced by (i<->lj)
c  and (j<->li). i and j are in s',li and lj are in l'. f and d are not 
c  assumed symmetric.
      li = loc3n(i)
      lj = loc3n(j)
      deltx = c(i,lj) - c(i,li) + c(j,li) - c(j,lj) +
     * (f(i,j)-f(j,i))*(d(lj,li)-d(li,lj))
c
      do 10 m=1,n1
        if (m.eq.i) go to 10
        if (m.eq.j) go to 10
        lm = loc3n(m)
        deltx = deltx + (f(i,m)-f(j,m))*(d(lj,lm)-d(li,lm)) +
     *   (f(m,i)-f(m,j))*(d(lm,lj)-d(lm,li))
   10 continue
c
      return
      end
