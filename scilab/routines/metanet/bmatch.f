      subroutine bmatch(n,m,np1,mt2,nd2,n8t8,nd8,ni,mi,ta,he,card,
     * match,degree,nbrll,ptr,nindex,nghbr,mark,visitd,evlev,odlev,
     * bloss,anom,aindex,base,bstar,peakl,peakr,f,path,bridge,
     * bindex,pred,pindex,derp,dindex,prdctr,edge,lr,leveli,mem,
     * member,estk,stack)
C*********************************************************************
C    CARDINALITY MATCHING PROBLEM 
C             NI = number of vertices input (<=N).
C             MI = number of edges input (<=M).  
C             TA(MI),HE(MI) tail,head arrays of the edges
C
C     OUTPUT: 
C        * the cardinality of an  optimal matching.
C        * U and MATCH(U) : U is a vertex and MATCH(U) is the vertex 
C          adjacent to U in the optimal matching or 0 if U is unmatched.
C-----------------------------------------------------------------------
C             Micali, S. and Vazirani, V. V.,"An O(square root of V * E)
C             Algorithm for Finding Maximum Matching in General Graphs",
C             Proc. 21st Annual Symposium on Foundation of Computer
C             Science, IEEE, 1980, pp. 17-27.
C     Programmers:
C              R. Bruce Mattingly  (FR132601@ysub.ysu.edu)
C              Nathan P. Ritchey   (nate@macs.ysu.edu)
C              Department of Mathematical and Computer Sciences
C              Youngstown State University
C              Youngstown, OH  44555
C     Version 1.11     Last modification:  02/14/92
C-----------------------------------------------------------------------
C Modifications and correction of bugs : Fev 97
C-----------------------------------------------------------------------
      implicit character (a-z)
      integer n, m, np1, mt2,nd2,nd8,n8t8
      integer i, j, p, u, v
      integer degree(n), nbrll(mt2, 2), ptr(n)
      integer ni, mi, match(n), card, nindex(np1), nghbr(mt2)
      integer ta(m),he(m)
c-----------------------------------------------------------------------
      character*1 mark(n)
      integer evlev(n), odlev(n), bloss(n)
      integer anom(2,n), aindex(n), afree
      logical visitd(n)
      integer base(nd2), bstar(nd2), peakl(nd2), peakr(nd2),
     +        f(n), path(n)
      integer bridge(3,m), bindex(nd2), bfree
      integer pred(2,m), pindex(n), pfree, derp(2,m), dindex(n), dfree,
     +        prdctr(n)
      integer edge(mt2)
      integer stklim, stktop
      integer lr(n), calls
c-----------------------------------------------------------------------
      integer leveli(n),mem(n),member(n),estk(n),stack(n8t8)
c adjacency lists in nbrll (neighbor linked lists)
      do 8 i = 1, ni
         degree(i) = 0
         ptr(i) = 0
    8 continue
      p = 0
      do 10 i = 1, mi
         u=ta(i)
         v=he(i)
         p = p+1
         nbrll(p,1) = v
         nbrll(p,2) = ptr(u)
         ptr(u) = p
         degree(u) = degree(u) + 1
         p = p + 1
         nbrll(p,1) = u
         nbrll(p,2) = ptr(v)
         ptr(v) = p
         degree(v) = degree(v) + 1
   10 continue
c  
      nindex(1) = 1
      do 12 i = 1, ni
         nindex(i+1) = nindex(i) + degree(i)
   12 continue
      do 16 i = 1, ni
         p = ptr(i)
         do 14 j = nindex(i), nindex(i+1)-1
            nghbr(j) = nbrll(p,1)
            p = nbrll(p,2)
   14    continue
   16 continue
c     call search to find optimal matching and cardinality
      call search(degree,n,m,np1,mt2,nd2,n8t8,nd8,ni,mi,match,card,
     +nindex,nghbr,mark,evlev,
     +odlev,bloss,anom,aindex,afree,visitd,base,bstar,peakl,peakr,
     +f,path,bridge,bindex,bfree,pred,pindex,pfree,derp, 
     +dindex,dfree,prdctr,edge,stklim,stktop,lr,calls,leveli,mem,
     +member,estk,stack)
c
  100 return
      end
c***********************************************************************
      subroutine search(degree,n,m,np1,mt2,nd2,n8t8,nd8,ni,mi,match,
     +card,nindex,nghbr,mark,evlev,odlev,bloss,
     +anom,aindex,afree,visitd,base,bstar,peakl,peakr,
     +f,path,bridge,bindex,bfree,pred,pindex,pfree,derp, 
     +dindex,dfree,prdctr,edge,stklim,stktop,lr,calls,leveli,mem,
     +member,estk,stack)
c
c     performs breadth-first searches from free (unmatched) vertices.
c     identifies bridges (edges joining alternating paths from 
c     free vertices)
c-----------------------------------------------------------------------
      implicit character (a-z)
c     parameters
      integer n, m, np1, mt2, infty, undef, nd2,nd8,n8t8
c     arguments
      integer  degree(*)
c     local variables
      integer b, u, v, e, i, uu, vv, temp, next, nlevi, leveli(n), code
      integer vp1, mem(n)
      logical didaug
c     common storage
      integer ni, mi, match(n), card, nindex(np1), nghbr(mt2)
      character*1 mark(n)
      integer evlev(n), odlev(n), bloss(n)
      integer anom(2,n), aindex(n), afree
      logical visitd(n)
      integer base(nd2), bstar(nd2), peakl(nd2), peakr(nd2),
     +        f(n), path(n)
      integer bridge(3,m), bindex(nd2), bfree
      integer pred(2,m), pindex(n), pfree, derp(2,m), dindex(n), dfree,
     +        prdctr(n)
      integer edge(mt2)
      integer stklim, stktop
      integer lr(n), calls,member(n),estk(n),stack(n8t8)
c-----------------------------------------------------------------------
      infty=np1
      undef=0
c  *** step -1 ***  compute initial matching 
c     initialize match array
      do 20 v = 1, ni
         match(v) = 0
         mem(v) = v
   20 continue
      card = 0
c
      call ssort(degree, mem, ni)
      do 55 vv = 1, ni
         v = mem(vv)
         if (match(v) .eq. 0) then
            do 53 uu = nindex(v), nindex(v+1)-1
               u = nghbr(uu)
               if (match(u) .eq. 0) then
                  match(u) = v
                  match(v) = u
                  card = card + 1
                  goto 55
               endif
   53       continue
         endif
   55 continue
      if (card .eq. ni/2) return
c
c *** step 0 *** initialization
c
    5 do 10 v = 1, ni
         evlev(v) = infty
         odlev(v) = infty
         bloss(v) = undef
         pindex(v) = 0
         dindex(v) = 0
         aindex(v) = 0
         visitd(v)   = .false.
         lr(v) = 0
         mark(v) = ' '
         f(v) = 0
         prdctr(v) = 0
   10 continue
      pfree = 1
      dfree = 1
      afree = 1
c
      do 12 e = 1, mi*2
   12    edge(e) = 0
c
      do 14 v = 0, ni/2
   14    bindex(v) = 0
      bfree = 1
c
      do 16 v = 1, mi-1
         vp1 = v + 1
         pred(1,v) = vp1
         derp(1,v) = vp1
   16 continue
      pred(1, mi) = 0
      derp(1, mi) = 0
      do 17 v = 1, ni - 1
         vp1 = v + 1
         anom(1,v) = vp1
   17 continue
      anom(1, ni) = 0
c
      do 18 v = 1, mi-1
            bridge(1,v) = v+1
   18 continue
      bridge(1,mi) = 0
c
      i = -1
      didaug = .false.
      calls = 0
      b = 0
c
c *** step 1 ***  check for free vertices
c
      do 101 v = 1, ni
         if (match(v) .eq. 0) evlev(v) = 0
  101 continue
c
c *** step 2 ***  proceed to next level (i) of search
c
  200 i = i + 1
      if (i .gt. n/2) then
         call out('search level limit exceeded')
         return
      endif
      nlevi = 0
c
c     loop 210 searches for vertices with level=i
c
      do 210 v = 1, ni
         if ((evlev(v) .eq. i) .or. (odlev(v) .eq. i)) then
c        if (min(evlev(v), odlev(v)) .eq. i) then
            nlevi = nlevi + 1
            leveli(nlevi) = v
         endif
  210 continue
c
c     if no more vertices have level i then halt (return to main)
c
      if (nlevi .eq. 0)  return
c
c     is search level i even or odd?
c
      if (mod(i,2) .eq. 0) then
c
c *** step 3 ***  for even search levels
c
         do 310 vv = 1, nlevi
            v = leveli(vv)
            if ((i .eq. 0) .and. (match(v) .ne. 0)) goto 310
c
c           loop 320 processes all neighbors of current v
c
            do 320 uu = nindex(v), nindex(v+1)-1
               u = nghbr(uu)
c
c              edge uv is unused if code is even
c
               call status(u, v, edge, code, 'r', nindex, nghbr)
               if ((match(u) .ne. v) .and. (mod(code,2) .eq. 0)) then
                  if (evlev(u) .ne. infty) then
                     temp = (evlev(u) + i) / 2
                     if ((evlev(u) .ne. i) .or. (u .lt. v)) then
                        call addbrg(u, v, temp, bridge, bindex, bfree)
                     endif
                  else
                     if (odlev(u) .eq. infty) odlev(u) = i + 1
                     if (odlev(u) .eq. i + 1) then
                        call addstk(pred, pindex, pfree, u, v)
                        call addstk(derp, dindex, dfree, v, u)
                        prdctr(u) = prdctr(u) + 1
                     else if (odlev(u) .lt. i) then
                        call addstk(anom, aindex, afree, u, v)
                     endif
                  endif
               endif
  320       continue
  310    continue
c
      else
c
c *** step 4 ***  for odd search levels
c
         do 410 vv = 1, nlevi
            v = leveli(vv)
            if (bloss(v) .eq. undef) then
               u = match(v)
               if (odlev(u) .eq. i) then
                  if (u .lt. v) then
                     call addbrg(u, v, i, bridge, bindex, bfree)
                  endif
               else if (odlev(u) .eq. infty) then
c
c                   make v the only predecessor of u
c
                    evlev(u) = i + 1
                    next = pindex(u)
c
c                   check for error condition
c
                    if (next .ne. 0) print *, 'warning from search: ',
     +              u, ' should not have any predecessors'
  408               if (next .ne. 0) then
                       pindex(u) = pred(1, next)
                       pred(1, next) = pfree
                       pfree = next
                       next = pindex(u)
                       goto 408
                    endif
c
                    call addstk(pred, pindex, pfree, u, v)
                    call addstk(derp, dindex, dfree, v, u)
                    prdctr(u) = prdctr(u) + 1
               endif
            endif
  410    continue
      endif
c
c *** step 5 ***
c
      next = bindex(i)
  510 if (next .ne. 0) then
         u = bridge(2, next)
         v = bridge(3, next)
         next = bridge(1, next)
         if ((mark(u) .eq. 'e') .or. (mark(v) .eq. 'e')) goto 510
         if ((bloss(u) .ne. undef) .and. (bloss(v) .eq. bloss(u)))
     +      goto 510
         call blsaug(u, v, didaug, b, i,n,m,np1,mt2,nd2,n8t8,nd8,ni,mi,
     +        match,card,nindex,nghbr,mark,evlev,odlev,bloss,
     +        anom,aindex,afree,visitd,base,bstar,peakl,peakr,
     +        f,path,bridge,bindex,bfree,pred,pindex,pfree,derp, 
     +        dindex,dfree,prdctr,edge,stklim,stktop,lr,calls,
     +        member,estk,stack)
         if (card .eq. ni/2) return
         goto 510
      endif
      if (didaug) then
         goto 5
      else
         goto 200
      endif
c
      end
c***********************************************************************
      subroutine blsaug(w1,w2,didaug,b,i,n,m,np1,mt2,nd2,n8t8,nd8,ni,
     +mi,match,card,nindex,nghbr,mark,evlev,odlev,bloss,
     +anom,aindex,afree,visitd,base,bstar,peakl,peakr,
     +f,path,bridge,bindex,bfree,pred,pindex,pfree,derp, 
     +dindex,dfree,prdctr,edge,stklim,stktop,lr,calls,
     +        member,estk,stack)
c
c     finds an augmenting path containing bridge w1-w2 or creates a new
c     blossom.  the common variable calls is incremented each time this
c     routine is called.  i is the search level.  b counts blossoms.
c
c     depth-first searches are conducted simultaneously from w1 and w2.
c     the left search starts at w1 and the right search starts at w2.
c     lr(n) = +calls if vertex n is visited by the left search.
c     lr(n) = -calls if vertex n is visited by the right search.
c-----------------------------------------------------------------------
      implicit character (a-z)
c     parameters
      integer n, m, np1, mt2, undef, nd2,nd8,n8t8
c     arguments
      integer w1, w2, b, i
      logical didaug
c     local variables
      integer vl, vr, dcv, barier, p1, p2, u, v, temp, next, 
     +        index, member(n), nmem, uu, lindex, rindex,
     +        estk(n), etop, ljump, rjump
c
c     common storage
      integer ni, mi, match(n), card, nindex(np1), nghbr(mt2)
      character*1 mark(n)
      integer evlev(n), odlev(n), bloss(n)
      integer anom(2,n), aindex(n), afree
      logical visitd(n)
      integer base(nd2), bstar(nd2), peakl(nd2), peakr(nd2),
     +        f(n), path(n)
      integer bridge(3,m), bindex(nd2), bfree
      integer pred(2,m), pindex(n), pfree, derp(2,m), dindex(n), dfree,
     +        prdctr(n)
      integer edge(mt2)
      integer stklim, stktop
      integer lr(n), calls,stack(n8t8)
      integer ijob, iv, icode
      undef=0
c-----------------------------------------------------------------------
      calls = calls + 1
      etop=0
c
c *** step 0 ***
      vl = w1
      ljump=0
      if ( bloss(w1) .ne. undef) then
         iv=0
         call bastar(iv, vl, bstar, f, bloss)
      endif
c
      vr = w2
      rjump=0
      if (bloss(w2) .ne. undef) then
         iv=0
         call bastar(iv, vr, bstar, f, bloss)
      endif
c
c     quit if an empty blossom is detected
      if (vl .eq. vr) goto 500
c
      lindex = pindex(vl)
      rindex = pindex(vr)
      lr(vl) = calls
      lr(vr) = -calls
      member(1) = vl
      member(2) = vr
      nmem = 2
      f(w1) = undef
      dcv = undef
c     in original algorithm w2 is replaced by vr below
      barier = w2
c
c *** step 1.1 ***  if vl and vr are both free vertices
c
  100 if ((match(vl) .eq. 0) .and. (match(vr) .eq. 0) ) then
         ijob=-1
         call fndpth(w1,vl,undef,ijob,n,m,np1,mt2,nd2,n8t8,nd8,ni,mi,
     +match,card,nindex,nghbr,mark,evlev,odlev,bloss,
     +anom,aindex,afree,visitd,base,bstar,peakl,peakr,
     +f,path,bridge,bindex,bfree,pred,pindex,pfree,derp, 
     +dindex,dfree,prdctr,edge,stklim,stktop,lr,calls,stack)
         ijob=1
         call fndpth(w2,vr,undef,ijob,n,m,np1,mt2,nd2,n8t8,nd8,ni,mi,
     +match,card,nindex,nghbr,mark,evlev,odlev,bloss,
     +anom,aindex,afree,visitd,base,bstar,peakl,peakr,
     +f,path,bridge,bindex,bfree,pred,pindex,pfree,derp, 
     +dindex,dfree,prdctr,edge,stklim,stktop,lr,calls,stack)
c
c        concatenate paths
c
         path(w1) = w2
c
c        augment matching 
c
         p1 = vl
  111    p2 = path(p1)
         match(p1) = p2
         match(p2) = p1
         etop = etop + 1
         estk(etop) = p1
         etop = etop + 1
         estk(etop) = p2
         p1 = path(p2)
         if (p2 .ne. vr) goto 111
         card = card + 1
         didaug = .true.
c
c        perform topological erase and return
c
  122    if (etop .gt. 0) then
            p1 =  estk(etop)
            etop = etop - 1
            if (mark(p1) .ne. 'e') then
               mark(p1) = 'e'
               next = dindex(p1)
  127          if (next .ne. 0) then
                  p2 = derp(2, next)
                  next = derp(1, next)
                  prdctr(p2) = prdctr(p2) - 1
                  if (prdctr(p2) .eq. 0) then
                     etop = etop + 1
                     estk(etop) = p2
                  endif
                  goto 127
               endif
            endif
            goto 122
         endif
         goto 500
c
c *** step 1.2 ***  if vl and vr are not both free
c
      else
         if (min(evlev(vl),odlev(vl)).ge.min(evlev(vr),odlev(vr))) then
            goto 200
         else
            goto 300
         endif
      endif
c
c does vl have unused ancestor edges?
c
  200 if (lindex .eq. 0) then
         u = 0
      else
         call ancest(vl, 1, u, lindex, pred, edge, mark, nindex, nghbr)
      endif
      if (u .eq. 0) then
c
c *** step 2.1 ***  vl has no more unused ancestors
c
         if (f(vl) .eq. undef) then
            if (dcv .ne. undef) then
               goto 400
            else
               print *, 'warning - blsaug(',w1,',',w2,') quitting ', 
     +                  'because dcv undefined at step 2.1'
               goto 500
            endif
         else
            vl = f(vl)
            lindex = pindex(vl)
            goto 100
         endif
      else
c
c ***  step 2.2 *** vl has some unused ancestors
c
c        mark vl-u 'used'
c
         icode=1
         call status(vl, u, edge,icode, 'w', nindex, nghbr)
         ljump = vl
         if (bloss(u) .ne. undef) then
            call bastar(vl, u, bstar, f, bloss)
            lindex = pindex(vl)
         endif
c
c    *** is u marked? ***
c
         if (abs(lr(u)) .ne. calls) then
c
c        *** step 2.2 (a) ***   u is unmarked
c
            lr(u) = calls
            call addmem(member, nmem, u)
            f(u) = vl
            vl = u
            lindex = pindex(vl)
            goto 100
         else
c
c        *** step 2.2 (b) ***   u is marked
c
            if ((u .eq. barier) .or. (u .ne. vr)) then
               goto 100
            else
               lr(u) = calls
               call addmem(member, nmem, u)
               vr = f(vr)
               if (rjump .ne. 0) vr = rjump
               rindex = pindex(vr)
               f(u) = vl
               vl = u
               lindex = pindex(vl)
               dcv = u
               goto 100
            endif
         endif
      endif
c
c does vr have unused ancestor edges?
c
  300 if (rindex .eq. 0) then
         u = 0
      else
         call ancest(vr, 1, u, rindex, pred, edge, mark, nindex, nghbr)
      endif
      if (u .eq. 0) then
c
c    *** step 3.1 ***  vr has no more unused ancestors
c
         if (vr .eq. barier) then
            if (dcv .eq. 0) then 
            print *, 'warning - blsaug(',w1,',',w2,') quitting ', 
     +               'because dcv undefined at step 3.1'
               goto 500
            endif
            vr = dcv
            rindex = pindex(vr)
            barier = dcv
            lr(vr) = -calls
            call addmem(member, nmem, vr)
            if (f(vl) .eq. undef) then
                goto 400
            else
               vl = f(vl)
               if (ljump .ne. 0) vl = ljump
               lindex = pindex(vl)
            endif
         else
            vr = f(vr)
            rindex = pindex(vr)
         endif
         goto 100
      else
c
c     *** step 3.2 ***  vr has some unused ancestors
c
c        mark vr-u 'used'
c
         icode=1
         call status(vr, u, edge, icode, 'w', nindex, nghbr)
         rjump = vr
         if (bloss(u) .ne. undef) then
            call bastar(vr, u, bstar, f, bloss)
            rindex = pindex(vr)
         endif
c
c    *** is u unmarked? ***
c
         if (abs(lr(u)) .ne. calls) then
c
c        *** step 3.2 (a) ***
c
            lr(u) = -calls
            call addmem(member, nmem, u)
            f(u) = vr
            vr = u
            rindex = pindex(vr)
            goto 100
         else
c
c        *** step 3.2 (b) ***
c
            if (u .eq. vl) then
               dcv = u
            endif
            goto 100
         endif
      endif
c
c *** step 4 ***  
c
  400 lr(dcv) = 0
c
c     create new blossom using all vertices marked l or r during call
c
      b = b + 1
      if (b .gt. nd2) then
         call out('out of storage')
         return
      endif
      do 425 uu = 1, nmem
         u = member(uu)
         if (u .eq. dcv) goto 425
         if (bloss(u) .eq. undef) then
            bloss(u) = b
            if (evlev(u) .lt. odlev(u)) then
               odlev(u) = 2 * i + 1 - evlev(u)
            else
               evlev(u) = 2 * i + 1 - odlev(u)
               index = aindex(u)
  421          if (index .ne. 0) then
                  v = anom(2, index)
                  index = anom(1, index)
                  temp = (evlev(u) + evlev(v))/2
                  call addbrg(u, v, temp, bridge, bindex, bfree)
c
c                 mark u-v 'used'
c
                  icode=1
                  call status(u,v,edge,icode,'w',nindex,nghbr)
                  goto 421
               endif
            endif
         endif
  425 continue
c
      peakl(b) = w1
      peakr(b) = w2
      base(b)  = dcv
      bstar(b) = dcv
c
c *** step 5 ***  return to search
c
  500 return
      end
c***********************************************************************
      subroutine addmem(member, nmem, u)
c     keeps track of vertices marked l or r during a call to blsaug
c     in case a new blossom is formed
c-----------------------------------------------------------------------
      integer member(1), nmem, u
c-----------------------------------------------------------------------
      nmem = nmem + 1
      member(nmem) = u
      return
      end
c***********************************************************************
      subroutine fndpth(high,low,b,job,n,m,np1,mt2,nd2,n8t8,nd8,ni,mi,
     +match,card,nindex,nghbr,mark,evlev,odlev,bloss,
     +anom,aindex,afree,visitd,base,bstar,peakl,peakr,
     +f,path,bridge,bindex,bfree,pred,pindex,pfree,derp, 
     +dindex,dfree,prdctr,edge,stklim,stktop,lr,calls,stack)
c     job .ne. -1:  returns a linked list from high to low
c     job .eq. -1:  returns a linked list from low to high
c     job .eq.  2:  allows transfers between left and right searchtrees
c                   (occurs only when opening a blossom)
c-----------------------------------------------------------------------
      implicit character (a-z)
c     parameters
      integer n, m, np1, nd8, mt2, nd2,n8t8
c     arguments 
      integer high, low, b, job
c     local variables 
      integer entrnc, bass, lb, retadd
      integer stack(8, nd8), u, v, pree, pntr, succ, vindex
c     common storage
      integer ni, mi, match(n), card, nindex(np1), nghbr(mt2)
      character*1 mark(n)
      integer evlev(n), odlev(n), bloss(n)
      integer anom(2,n), aindex(n), afree
      logical visitd(n)
      integer base(nd2), bstar(nd2), peakl(nd2), peakr(nd2),
     +        f(n), path(n)
      integer bridge(3,m), bindex(nd2), bfree
      integer pred(2,m), pindex(n), pfree, derp(2,m), dindex(n), dfree,
     +        prdctr(n)
      integer edge(mt2)
      integer stklim, stktop
      integer lr(n), calls
      integer icode
c-----------------------------------------------------------------------
      stklim = nd8
      stktop = 0
c
c *** step 0 ***
c
   10 if (high .eq. low) then
c
c        whole path is just "high"
c
         goto 820
      endif
c
      v = high
      vindex = pindex(v)
c
c *** step 1 ***
c
  100 if (vindex .eq. 0) then
         u = 0
      else
         call ancest(v,2,u,vindex,pred,edge,mark,nindex,nghbr)
      endif
      if (u .eq. 0) then
         if (f(v) .eq. 0) then
            call out('error-no path')
            return
         else 
            v = f(v)
         endif
         vindex = pindex(v)
         goto 100
      endif
c
c *** step 2 ***
c
      if (bloss(v) .eq.  b) then
c
c        mark u-v 'visited'
c
         icode=2
         call status(u, v, edge, icode, 'w', nindex, nghbr)
      else
         u = base(bloss(v))
      endif
c
c *** step 3 ***
c
      if (u .eq. low) goto 600
c
c *** step 4 ***  
c
      if  (visitd(u)) goto 100
      if ( min(evlev(u),odlev(u)) .le. min(evlev(low),odlev(low)) ) 
     +   goto 100
      if (job .eq. 2) goto 501
      if ((bloss(u) .eq. b) .and. (lr(u) .eq. -lr(high))) goto 100 
c *** step 5 ***
c
  501 visitd(u) = .true.
      f(u) = v
      v = u
      vindex = pindex(v)
      goto 100
c
c *** step 6 ***
c
  600 path(v) = low
  620 if (v .ne. high) then
         u = v
         v = f(v)
         path(v) = u
         goto 620
      endif
c
c *** step 7 ***
c
      entrnc = high
  710 if (entrnc .ne. low) then
         bass = path(entrnc)
         if (bloss(entrnc) .ne. b) then
c
c            simulate call to open(entrnc, bass)
c
             goto 900
c
         endif
  777    entrnc = bass
         goto 710
      endif
c
c *** step 8 ***
c
  800 if (job .eq. -1) then
c
c     invert path if necessary
c
      pree = 0
      pntr = high
      succ = path(high)
  810 if (pntr .ne. low) then
         path(pntr) = pree
         pree = pntr
         pntr = succ
         succ = path(pntr)
         goto 810
      endif
      path(pntr) = pree
      endif
c
c     check stack before returning
c
  820 if (stktop .ne. 0) then
         high   = stack(1, stktop)
         low    = stack(2, stktop)
         b      = stack(3, stktop)
         job    = stack(4, stktop)
         entrnc = stack(5, stktop)
         bass   = stack(6, stktop)
         lb     = stack(7, stktop)
         retadd = stack(8, stktop)
         stktop = stktop - 1
      else
         return
      endif
c
      if (retadd .eq. 777) then
         entrnc = bass
         goto 710
      else if (retadd .eq. 902) then
         goto 902
      else
         goto 904
      endif
c
c***********************************************************************
c
c     function open starts at line 900
c     embedded inside fndpth to remove recursion
c
  900 lb = bloss(entrnc)
      if (evlev(entrnc) .le. odlev(entrnc) ) then
         call push(high, low, b, job, entrnc, bass, lb, 777, stack,
     +        stklim, stktop)
         high = entrnc
         low = bass
         job = 2
         b = lb
         goto 10
      else
         if (lr(entrnc) .gt. 0) then
            call push(high,low,b,job,entrnc,bass,lb,902,stack,
     +           stklim,stktop)
            high = peakl(lb)
            low = entrnc
            job = -1
            b = lb
            goto 10
         else
            call push(high,low,b,job,entrnc,bass,lb,904,stack,
     +           stklim, stktop)
            high = peakr(lb)
            low = entrnc
            job = -1
            b = lb
            goto 10
         endif
      endif
 902        path(peakl(lb)) = peakr(lb)
            call push(high,low,b,job,entrnc,bass,lb,777,stack,
     +        stklim, stktop)
            high= peakr(lb)
            low = bass
            job = 1
            b = lb
            goto 10
 904        path(peakr(lb)) = peakl(lb)
            call push(high,low,b,job,entrnc,bass,lb,777,stack,
     +        stklim, stktop)
            high = peakl(lb)
            low = bass
            job = 1
            b = lb
            goto 10
      end
c***********************************************************************
      subroutine ancest(v,job,u,index,pred,edge,mark,nindex,nghbr)
c
c     searches predecessor list of v
c         job = 1:  check for unused edges
c         job = 2:  check for unvisited edges
c
c         u .eq. 0:  no suitable edge was found
c         u .ne. 0:  u-v is a suitable edge
c-----------------------------------------------------------------------
      implicit character (a-z)
c
      integer v, job, u, index, pred(2,*), edge(*), nindex(*), nghbr(*)
      character*1 mark(*)
c
      integer w, code
c-----------------------------------------------------------------------
      u = 0
  201 if ((index .ne. 0) .and. (u .eq. 0)) then
         w = pred(2, index)
         index = pred(1, index)
         if (mark(w) .ne. 'e') then
            call status(w, v, edge, code, 'r', nindex, nghbr)
            if (job .eq. 1) then
               if (mod(code, 2) .eq. 0) u = w
            else
               if (code .lt. 2) u = w
            endif
         endif
         goto 201
      endif
      return
      end
c***********************************************************************
      subroutine status(u, v, edge, code, job, nindex, nghbr)
c
c     job .eq. 'w' (write):, add code to status(u,v)
c     job .eq. 'r' (read) :  sets code =  status(u,v)
c     status(u,v) is symmetric so upper triangle is stored in edge
c-----------------------------------------------------------------------
      implicit character (a-z)
c
      integer u, v, edge(*), code, nindex(*), nghbr(*)
      character job
c
      integer i, j, k, index
c-----------------------------------------------------------------------
      if (u.lt.v) then
          i = u
          j = v
      else
          i = v
          j = u
      endif
      index = 0
      do 10 k=nindex(i), nindex(i+1)-1
         if (nghbr(k) .eq. j) index = k
   10 continue
      if (index .eq. 0) then
         call out('no edge between 2 nodes')
         return
      endif
      if (job .eq. 'w') then
         edge(index) = edge(index) + code
      else
         code = edge(index)
      endif
      return
      end
c***********************************************************************
      subroutine addbrg(u, v, br, bridge, bindex, bfree)
c
c     adds edge (u,v) to bridge(br)
c
c     u, v   -  integer verticies
c     br     -  integer bridge level number
c     bridge -  array of size 3 x m  holding all bridges
c     bindex -  array of size n/2 holding pointers to each level
c     bfree  -  integer pointing to first free location in bridge
c     next   -  integer pointer
c
c-----------------------------------------------------------------------
      implicit character (a-z)
c
      integer u, v, br, bridge(3,*), bindex(*), bfree
      integer next
c-----------------------------------------------------------------------
      if (bfree .eq. 0) then
         call out('stack overflow')
         return
      else
         next = bfree
         bfree = bridge(1, next)
         bridge(2, next) = u
         bridge(3, next) = v
         bridge(1, next) = bindex(br)
         bindex(br)       = next
      endif
      return
      end
c***********************************************************************
      subroutine addstk(stack1, index, free, u, v)
c
c     adds element v to substack u
c
c     stack1  -  array of size 2 x m  holding all substacks
c     index  -  array of size n holding pointers to each substack
c     free   -  integer pointing to first free location in stack1
c     u      -  integer vertex
c     v      -  integer vertex
c     next   -  integer pointer
c-----------------------------------------------------------------------
      implicit character (a-z)
c
      integer stack1(2,*), index(*), free, u, v
      integer next
c-----------------------------------------------------------------------
      if (free .eq. 0) then
         call out('stack overflow')
         return
      else
         next = free
         free = stack1(1, next)
         stack1(2, next) = v
         stack1(1, next) = index(u)
         index(u)       = next
      endif
      return
      end
c***********************************************************************
      subroutine push(hi,lo,b,job,ent,bas,lb,ra,stack,
     +stklim, stktop)
c
c     adds elements to stack used to simulate recursive calls
c     between fndpth and open
c-----------------------------------------------------------------------
      implicit character (a-z)
      integer hi, lo, b, job, ent, bas, lb, ra, stack(8,*)
      integer stklim, stktop
c-----------------------------------------------------------------------
      if (stktop .lt. stklim) then
          stktop = stktop + 1
          stack(1, stktop) = hi
          stack(2, stktop) = lo
          stack(3, stktop) = b
          stack(4, stktop) = job
          stack(5, stktop) = ent
          stack(6, stktop) = bas
          stack(7, stktop) = lb
          stack(8, stktop) = ra
      else
         call out('stack overflow')
         return
      endif
      return
      end
c***********************************************************************
      subroutine bastar(v, u, bstar, f, bloss)
c
c     sets u = base*(u) and preserves path back to oldv
c-----------------------------------------------------------------------
      implicit character (a-z)
      integer v, u, bstar(*), f(*), bloss(*)
      integer oldv, w
c-----------------------------------------------------------------------
      oldv = v
   10 f(u) = v
      v = u
      u = bstar(bloss(u))
      if (bloss(u) .ne. 0) goto 10
c
      w = f(v)
      if (oldv .eq. 0) then
         f(u) = v
         v = 0
      endif
c
c     path compression
c
   20 if (w .ne. oldv) then
         bstar(bloss(w)) = u
         w = f(w)
         goto 20
      endif
      return
      end
c***********************************************************************
      subroutine ssort(a,b,l)
c     *      sorting of the vector a(i) in increasing order by the     *
c     *      parameters:                                               *
c     *      input:                                                    *
c     *         l        dimension of the vector                       *
c     *         a(i)     vector to sort (integer)                      *
c     *         b(i)     = i    (integer)  i=1,...,n                   *
c     *      output:                                                   *
c     *         a(i)     the sorted vector                             *
c     *         b(i)     permutation vector of the sorted vector       *
c     *                                                                *
c *** ******************************************************************
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      implicit integer (a-z)
      dimension a(1), b(1)
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      f = 1
      if (l.le.f) return
      n2 = (l - f+1)/2
      s = 1023
      do 100 t = 1, 10
      if (s.gt.n2) goto 90
      ls = l - s
      do 20 i = f, ls
      is = i + s
      ah = a(is)
      bh = b(is)
      j = i
      js = is
    5 if (ah.ge.a(j)) goto 10
      a(js) = a(j)
      b(js) = b(j)
      js = j
      j = j - s
      if (j.ge.f) goto 5
   10 a(js) = ah
      b(js) = bh
   20 continue
   90 s = s/2
  100 continue
      return
      end
