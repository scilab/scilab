c     algorithm 595, collected algorithms from acm.
c     algorithm appeared in acm-trans. math. software, vol.9, no. 1,
c     mar., 1983, p. 131-138.
c this program finds one or more hamiltonian circuits in a              
c directed graph of  n  vertices and  m  arcs.                          
c input : n number of vertices, ar=ls, pr=lp-1
      subroutine hamil(n,m,np1,pr,ar,s,nc,nb,
     * pc,ac,vr,vc,p,subr,rbus,tor)
      integer pr(np1),pc(np1),ar(m),ac(m),s(n),vr(n),    
     * vc(n),p(n),subr(n),rbus(n),tor(n)                  
c exact procedure to find a single hamiltonian circuit, if one exists. 
      do 1,i=1,n
         s(i)=0
 1    continue
      nc = 1                                                            
      nb = -1                                                           
      call hproc(n,pr,ar,nout,nc,nb,s,n+1,pr(n+1),pc,ac,vr, vc, 
     *p,subr,rbus,tor) 
c exact procedure to find all the hamiltonian circuits.
c      nc = -1                                                           
c       nb = -1                                                           
c       call hproc(n,pr,ar,nout,nc,nb,s,n+1,pr(n+1),pc,ac,vr,vc, 
c      * p, subr, rbus, tor)                                              
c heuristic procedure to find a single hamiltonian 
c circuit, if one exists, without performing more than 2 backtrackings.
c       nc = 1                                                            
c       nb = 2                                                            
c       call hproc(n,pr,ar,nout,nc,nb,s,n+1, pr(n+1),pc,ac,vr,vc, 
c      *p,subr,rbus,tor)                                              
c heuristic procedure to find a single hamiltonian
c circuit, if one exists, without performing more than 4 backtrackings.
c       nc = 1                                                            
c       nb = 4                                                            
c       call hproc(n,pr,ar,nout,nc,nb,s,n+1,pr(n+1),pc,ac,vr,vc, 
c      *p,subr,rbus,tor)                                              
c heuristic procedure to find  at most  2
c hamiltonian circuits, without performing more than  5  backtrackings. 
c       nc = 2                                                            
c       nb = 5                                                            
c       call hproc(n,pr,ar,-1,nc,nb,s,n+1,pr(n+1),pc,ac,vr,vc,   
c      *p,subr,rbus,tor)                                              
c       if (nc.eq.0) return
      return
      end                                                               
      subroutine hproc(n,pr,ar,kw,nc,nb,s,np1,m,pc,ac,vr,vc,
     *p,subr,rbus,tor)
c
c subroutine to find one or more hamiltonian circuits in a
c directed graph of  n  vertices ( n .gt. 1 ) represented
c by the integers  1, 2, ..., n  and  m  arcs.
c
c hproc is based on an enumerative algorithm and can be used
c either as an exact procedure or as a heuristic procedure
c (by limiting the number of backtrackings allowed).
c
c entrance to hproc is achieved by using the statement
c     call hproc(n,pr,ar,kw,nc,nb,s,n+1,pr(n+1),pc,ac,vr,vc,
c    *        p,subr,rbus,tor)
c
c the values of the first six parameters must be defined
c by the user prior to calling hproc. hproc needs  2  arrays ( pr
c and  pc ) of length  n + 1 ,  2  arrays ( ar  and  ac )
c of length  m  and  7  arrays ( s ,  vr ,  vc ,  subr ,
c rbus  and  tor ) of length  n . these arrays must be
c dimensioned by the user in the calling program.
c
c hproc calls  5  subroutines: pathp, fupd, bupd, iupd, rarc.
c meaning of the input parameters:
c n     = number of vertices.
c pr(i) = sum of the out-degrees of vertices  1, ..., i-1
c         ( pr(1) = 0 ,  pr(n+1) = m ).
c ar    = adjacency list. the elements from  ar(pr(i)+1)  to
c         ar(pr(i+1))  are a record containing,in any order,
c         all the vertices  j  such that arc  (i,j)  exists.
c         the graph should not contain arcs starting and
c         ending at the same vertex.
c kw    = unit tape on which to write the hamiltonian cir-
c         cuits found, according to format  20i5 .  kw = - 1
c         if no writing is desired. the circuits are written
c         as ordered sequences of  n  vertices.
c
c meaning of the input-output parameters:
c nc(input)  = upper bound on the number of hamiltonian
c              circuits to be found ( nc = - 1 if all the
c              hamiltonian circuits are to be found).
c nc(output) = number of hamiltonian circuits found.
c nb(input)  = - 1  if hproc must be executed as an exact
c              procedure.
c            = upper bound on the number of backtrackings if
c              hproc must be executed as a heuristic procedure.
c nb(output) = number of backtrackings performed. when hproc
c              has been executed as a heuristic procedure,
c              if  nb(output) .lt. nb(input)  then the
c              result obtained is exact.
c
c meaning of the output parameter:
c s(i) = i-th  vertex in the last hamiltonian circuit found.
c
c on return of hproc  n, pr  and  kw  are unchanged, while in
c ar  the order of the elements within each record may be
c altered.
c
c meaning of the work arrays:
c pc(i)   = sum of the in-degrees of vertices  1, ..., i-1
c           ( pc(1) = 0 ).
c ac      = adjacency list (backward). the elements from
c           ac(pc(i)+1)  to  ac(pc(i+1))  contain, in any
c           order, all the vertices  j  such that arc  (j,i)
c           exists.
c when an arc is removed from the graph at the  k-th  level
c of the branch-decision tree, the corresponding elements
c ar(q)  and  ac(t)  are set to  - (k*(n+1) + ar(q))  and
c to  - (k*(n+1) + ac(t)) , respectively.
c vr(i)   = current out-degree of vertex  i .
c vc(i)   = current in-degree of vertex  i .
c subr(i) = - (k*(n+1) + j)  if arc  (i,j)  was implied at
c           the  k-th  level of the branch-decision tree.
c         = 0  otherwise.
c rbus(i) = - j  if arc  (j,i)  is currently implied.
c         = 0  otherwise.
c tor(k)  = q*(m+1) + t  if the arc going from  s(k)  to the
c           root, corresponding to  ar(q)  and to  ac(t),
c           was removed from the graph at the  k-th  level
c           of the branch-decision tree.
c         = 0  otherwise.
c p(i)    = pointer for the forward step. the next arc
c           starting from  i  to be considered in the
c           branch-decision tree is  (i,ar(pr(i)+p(i)).
c
c meaning of the main work simple variables:
c jr  = root. the hamiltonian circuits are determined as
c       paths starting and ending at  jr .
c k   = current level of the branch-decision tree.
c m   = number of arcs.
c mp1 = m + 1 (used for packing  tor ).
c np1 = n + 1 (used for packing  ar ,  ac  and  subr )
c
      integer pr(np1), pc(np1), ar(m), ac(m), s(n), vr(n), vc(n), p(n),
     * subr(n), rbus(n), tor(n)
c
c s t e p   0   (initialize).
c
      nco = nc
      nc = 0
      nbo = nb
      nb = 0
      do 10 i=1,n
        vc(i) = 0
        subr(i) = 0
        rbus(i) = 0
        p(i) = 1
   10 continue
      do 30 i=1,n
        j1 = pr(i) + 1
        j2 = pr(i+1)
        vr(i) = j2 - j1 + 1
        if (vr(i).eq.0) return
        do 20 j=j1,j2
          ja = ar(j)
          vc(ja) = vc(ja) + 1
   20   continue
   30 continue
      pc(1) = 0
      do 40 i=1,n
        if (vc(i).eq.0) return
        pc(i+1) = pc(i) + vc(i)
        vc(i) = 0
   40 continue
      do 60 i=1,n
        j1 = pr(i) + 1
        j2 = pr(i+1)
        do 50 j=j1,j2
          jj = ar(j)
          vc(jj) = vc(jj) + 1
          ja = pc(jj) + vc(jj)
          ac(ja) = i
   50   continue
   60 continue
      mp1 = m + 1
c select as root  jr  the vertex  i  with maximum  vc(i)
c (break ties by choosing  i  with minimum  vr(i) ).
      maxe = vc(1)
      minu = vr(1)
      jr = 1
      do 100 i=2,n
        CRES=vc(i)-maxe
        if (CRES .lt. 0) then 
           goto 100
        elseif (CRES .eq. 0) then
           goto 70
        else
           goto 80
        endif
   70   if (vr(i).ge.minu) go to 100
        go to 90
   80   maxe = vc(i)
   90   minu = vr(i)
        jr = i
  100 continue
      k1 = -np1
      k = 1
      s(1) = jr
c
c s t e p   1   (search for implied arcs).
c
  110 do 120 j=1,n
        if (vr(j).eq.1) go to 130
        if (vc(j).eq.1) go to 170
  120 continue
c no further arc can be implied.
      go to 220
c arc  (j,jl)  is implied because  vr(j) = 1 .
  130 l1 = pr(j) + 1
      l2 = pr(j+1)
      do 140 l=l1,l2
        if (ar(l).gt.0) go to 150
  140 continue
  150 jl = ar(l)
c find the starting vertex  it1  and the ending vertex  it2
c of the largest path of implied arcs containing  (j,jl) .
      call pathp(j, jl, subr, rbus, ar, pr, s, n, np, it1, it2, k, jr,
     * m, np1)
      if (np.eq.0) go to 160
      if (np.eq.(-1)) go to 340
c subroutine pathp found a hamiltonian circuit.
      k = k + 1
      go to 320
  160 subr(j) = k1 - jl
      rbus(jl) = j
c remove from the graph all arcs terminating at  jl .
      call iupd(j, jl, l, ac, ar, pc, pr, vc, vr, k1, n, m, np1)
      if (j.eq.0) go to 340
      go to 210
c arc  (jl,j)  is implied because  vc(j) = 1 .
  170 l1 = pc(j) + 1
      l2 = pc(j+1)
      do 180 l=l1,l2
        if (ac(l).gt.0) go to 190
  180 continue
  190 jl = ac(l)
c find the starting vertex  it1  and the ending vertex  it2
c of the largest path of implied arcs containing  (jl,j) .
      call pathp(jl, j, subr, rbus, ar, pr, s, n, np, it1, it2, k, jr,
     * m, np1)
      if (np.eq.0) go to 200
      if (np.eq.(-1)) go to 340
c subroutine pathp found a hamiltonian circuit.
      i = s(k)
      k = k + 1
      go to 320
  200 subr(jl) = k1 - j
      rbus(j) = jl
c remove from the graph all arcs emanating from  jl .
      call iupd(j, jl, l, ar, ac, pr, pc, vr, vc, k1, n, m, np1)
      if (j.eq.0) go to 340
c if arc  (it2,it1)  is in the graph, remove it.
  210 call rarc(it2, it1, ar, ac, pr, pc, vr, vc, k1, jj, ll, n, m, np1)
      if (jj.eq.(-1)) go to 340
      go to 110
c
c s t e p   2   (add implied arcs to  s ).
c
  220 i = s(k)
      if (subr(i).eq.0) go to 230
      jsubr = -subr(i) + subr(i)/np1*np1
      if (jsubr.eq.jr) go to 340
      k = k + 1
      s(k) = jsubr
      if (k.ne.n) go to 220
      if (subr(jsubr).lt.0) go to 320
      go to 340
c
c s t e p   3   (branch).
c
  230 l1 = pr(i) + p(i)
      l2 = pr(i+1)
      if (l1.gt.l2) go to 340
c find the next arc  (i,jl)  to be added to  s .
      dens = n**3
      j1 = 0
      j2 = 0
      do 310 j=l1,l2
        jl = ar(j)
        if (jl.lt.0) go to 310
        if (vr(jl).gt.0) go to 260
        if (subr(jl).eq.0) go to 310
        if (jl.eq.jr) go to 310
        iend = jl
  240   iend = -subr(iend) + subr(iend)/np1*np1
        if (subr(iend).ne.0) go to 240
        if (vc(jl).lt.vr(iend)) go to 250
        score = vr(iend)*n + vc(jl)
        go to 280
  250   score = vc(jl)*n + vr(iend)
        go to 280
  260   if (vc(jl).lt.vr(jl)) go to 270
        score = vr(jl)*n + vc(jl)
        go to 280
  270   score = vc(jl)*n + vr(jl)
  280   if (dens.le.score) go to 290
        dens = score
        ipi = j
  290   if (j1.eq.0) go to 300
        if (j2.eq.0) j2 = j
        go to 310
  300   j1 = j
  310 continue
      if (j1.eq.0) go to 340
      jl = ar(ipi)
      ar(ipi) = ar(j1)
      ar(j1) = jl
      if (j2.eq.0) j2 = pr(i+1) + 1
      p(i) = j2 - pr(i)
      k = k + 1
      s(k) = jl
      k1 = -k*np1
c remove from the graph all arcs emanating from  i .
      call fupd(ar, ac, pr, pc, vr, vc, i, k1, n, m, np1)
c remove from the graph all arcs terminating at jl .
      call fupd(ac, ar, pc, pr, vc, vr, jl, k1, n, m, np1)
      tor(k) = 0
c if arc  (jl,jr)  is in the graph, remove it.
      call rarc(jl, jr, ar, ac, pr, pc, vr, vc, k1, jj, ll, n, m, np1)
      if (jj.eq.0) go to 110
      if (jj.eq.(-1)) go to 340
      tor(k) = jj*mp1 + ll
      go to 110
c
c s t e p   4   (hamiltonian circuit found).
c
  320 nc = nc + 1
      if (kw.eq.(-1)) go to 330
c ww s(kj),kj=1,n)
  330 if (nc.eq.nco) go to 430
      k = k - 1
c
c s t e p   5   (backtrack).
c
  340 if (k.le.1) go to 430
      ja = s(k)
      p(ja) = 1
      ja = s(k-1)
      if (subr(ja).eq.0) go to 350
c backtracking for an implied arc.
      k = k - 1
      go to 340
  350 if (nb.eq.nbo) go to 430
      nb = nb + 1
      k1 = -k*np1
      k2 = -(k+1)*np1
      i = s(k-1)
c backtracking for the arcs implied at level  k .
      iff = 0
      do 360 j=1,n
        if (subr(j).gt.k1) go to 360
        if (subr(j).lt.k2) go to 360
        ja = k1 - subr(j)
        rbus(ja) = 0
        subr(j) = 0
        iff = 1
  360 continue
      if (iff.eq.1) go to 370
c no arc was implied at level  k .
      call bupd(ar, ac, pr, pc, vr, vc, i, k1, k2, n, m, np1)
      call bupd(ac, ar, pc, pr, vc, vr, s(k), k1, k2, n, m, np1)
      if (tor(k).eq.0) go to 420
      j1 = tor(k)/mp1
      j2 = tor(k) - j1*mp1
      ar(j1) = jr
      ja = s(k)
      vr(ja) = vr(ja) + 1
      ac(j2) = s(k)
      vc(jr) = vc(jr) + 1
      go to 420
c at least one arc was implied at level  k .
  370 do 410 j=1,n
        l1 = pr(j) + 1
        l2 = pr(j+1)
        do 400 l=l1,l2
          jl = ar(l)
          if (jl.gt.k1) go to 400
          if (jl.lt.k2) go to 400
          jl = k1 - jl
          ar(l) = jl
          vr(j) = vr(j) + 1
          ll1 = pc(jl) + 1
          ll2 = pc(jl+1)
          do 380 ll=ll1,ll2
            if (k1-ac(ll).eq.j) go to 390
  380     continue
  390     ac(ll) = j
          vc(jl) = vc(jl) + 1
  400   continue
  410 continue
  420 k = k - 1
      go to 230
c
c re-store the original vector  ar .
c
  430 do 440 j=1,m
        if (ar(j).gt.0) go to 440
        ar(j) = -ar(j) + ar(j)/np1*np1
  440 continue
      return
      end
      subroutine pathp(i,j,subr,rbus,ar,pr,s,n,np,i1,i2,k,    
     * jr, m, np1)
c subroutine to find the starting vertex  i1  and the ending
c vertex  i2  of the largest path of implied arcs containing
c arc  (i,j) .
c meaning of the output parameter  np :
c np =  0  if the path contains  l .lt. n  vertices.
c    =  1  if the path contains  n  vertices and arc (i2,i1)
c          exists (the hamiltonian circuit is stored in  s )
c    = -1  if the path contains  n  vertices but arc (i2,i1)
c          does not exist.
      integer subr(n), rbus(n), ar(m), pr(np1), s(n)
      np = 0
      l = 1
      i1 = i
   10 if (rbus(i1).eq.0) go to 20
      i1 = rbus(i1)
      l = l + 1
      go to 10
   20 i2 = j
      l = l + 1
   30 if (subr(i2).eq.0) go to 40
      i2 = -subr(i2) + subr(i2)/np1*np1
      l = l + 1
      go to 30
   40 continue
      if (l.lt.n) return
c the path contains  n  vertices.
      k1 = -k*np1
      l1 = pr(i2) + 1
      l2 = pr(i2+1)
      do 60 l=l1,l2
        if (ar(l).lt.0) go to 50
        if (ar(l).eq.i1) go to 70
        go to 60
   50   if (k1-ar(l).eq.i1) go to 70
   60 continue
c no hamiltonian circuit can be determined.
      np = -1
      return
c a hamiltonian circuit exists. store it in  s .
   70 np = 1
      rbus(j) = i
      rbus(i1) = i2
      s(n) = rbus(jr)
      l = n - 1
   80 if (l.eq.k) go to 90
      ja = s(l+1)
      s(l) = rbus(ja)
      l = l - 1
      go to 80
   90 rbus(i1) = 0
      rbus(j) = 0
      return
      end
      subroutine fupd(a1, a2, p1, p2, v1, v2, i1, k1, n, m, np1)
c forward step updating
      integer a1(m), a2(m), p1(np1), p2(np1), v1(n), v2(n)
      j1 = p1(i1) + 1
      j2 = p1(i1+1)
      do 30 j=j1,j2
        if (a1(j).lt.0) go to 30
        ia = a1(j)
        l1 = p2(ia) + 1
        l2 = p2(ia+1)
        do 10 l=l1,l2
          if (a2(l).eq.i1) go to 20
   10   continue
   20   v2(ia) = v2(ia) - 1
        a2(l) = k1 - a2(l)
        a1(j) = k1 - ia
   30 continue
      v1(i1) = 0
      return
      end
      subroutine bupd(a1, a2, p1, p2, v1, v2, ii, k1, k2, n, m, np1)
c backtracking step updating
      integer a1(m), a2(m), p1(np1), p2(np1), v1(n), v2(n)
      l1 = p1(ii) + 1
      l2 = p1(ii+1)
      do 30 l=l1,l2
        if (a1(l).gt.k1) go to 30
        if (a1(l).lt.k2) go to 30
        ia = k1 - a1(l)
        a1(l) = ia
        v1(ii) = v1(ii) + 1
        ll1 = p2(ia) + 1
        ll2 = p2(ia+1)
        do 10 ll=ll1,ll2
          if (k1-a2(ll).eq.ii) go to 20
   10   continue
   20   a2(ll) = ii
        v2(ia) = v2(ia) + 1
   30 continue
      return
      end
      subroutine iupd(ia, ib, l, a1, a2, p1, p2, v1, v2, k1, n, m, np1)
      integer a1(m), a2(m), p1(np1), p2(np1), v1(n), v2(n)
c updating for implied arc
      m1 = p1(ib) + 1
      m2 = p1(ib+1)
      do 40 mm=m1,m2
        iarc = a1(mm)
        if (iarc.lt.0) go to 40
        if (v2(iarc).ne.1) go to 10
        if (iarc.ne.ia) go to 50
        jj = l
        go to 30
   10   j1 = p2(iarc) + 1
        j2 = p2(iarc+1)
        do 20 jj=j1,j2
          if (a2(jj).eq.ib) go to 30
   20   continue
   30   a2(jj) = k1 - a2(jj)
        v2(iarc) = v2(iarc) - 1
        a1(mm) = k1 - iarc
        v1(ib) = v1(ib) - 1
   40 continue
      return
   50 ia = 0
      return
      end
      subroutine rarc(ia, ib, ar, ac, pr, pc, vr, vc, k1, jj, ll, n, m,
     * np1)
c subroutine to remove arc  (ia,ib)  from the graph.
c meaning of the output parameters  jj  and  ll :
c jj =  location of the element of  ar  corresponding to the removed arc.
c    =  0  if arc  (ia,ib)  is not in the graph.
c    = -1  if, after the removal of arc  (ia,ib) , the graph
c          would admit no hamiltonian circuit.
c ll =  location of the element of  ac  corresponding to the
c       removed arc (defined only if  jj .gt. 0 ).
      integer ar(m), ac(m), pr(np1), pc(np1), vr(n), vc(n)
      j1 = pr(ia) + 1
      j2 = pr(ia+1)
      do 20 jj=j1,j2
        if (ar(jj).lt.0) go to 20
        if (ar(jj).ne.ib) go to 20
        l1 = pc(ib) + 1
        l2 = pc(ib+1)
        do 10 ll=l1,l2
          if (ac(ll).eq.ia) go to 30
   10   continue
   20 continue
c arc  (ia,ib)  is not in the graph.
      jj = 0
      return
   30 if (vr(ia).eq.1) go to 40
      if (vc(ib).eq.1) go to 40
      ar(jj) = k1 - ib
      vr(ia) = vr(ia) - 1
      ac(ll) = k1 - ia
      vc(ib) = vc(ib) - 1
      return
c arc  (ia,ib)  cannot be removed from the graph.
   40 jj = -1
      return
      end

