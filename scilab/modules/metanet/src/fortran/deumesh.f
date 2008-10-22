c*****************************************************************************
c      from the algorithm written by :
c*****************************************************************************
c                                               robert renka
c                                       oak ridge natl. lab.
c                                             (615) 576-5139
c acm-trans. math. software, vol.10, no. 4, dec., 1984, p. 453.
c*****************************************************************************
      subroutine deumesh (n,n4,n6,x,y, iadj,iend,nbt,nu,iero)
      integer n,n6,n4,iadj(n6),iend(n),nbt,nu(3,n4),iero
      double precision    x(n), y(n)
c triangulation of n points in the plane.  the triangulation is as nearly 
c equiangular as possible. 
c
c input parameters -     
c     x,y - n-vectors of coordinates. (x(i),y(i)) defines node i.
c     iadj - vector of length .ge. n6 (= 6*n-9.)
c     iend - vector of length .ge. n.
c
c output parameters - 
c     iadj - adjacency lists of neighbors in counterclockwise order.  the
c           list for node i+1 follows that for node i where x and y define
c           the order.  the value 0 denotes the boundary (or a pseudo-node
c           at infinity) and is always the last neighbor of a boundary
c           node.  iadj is unchanged if iero .ne. 0.
c     iend - pointers to the ends of adjacency lists (sets of neighbors)
c           in iadj.  the  neighbors of node 1 begin in iadj(1).
c           for k .gt. 1, the neighbors of node k begin in iadj(iend(k-1)+1)
c           and k has iend(k) - iend(k-1) neighbors including (possibly) the
c           boundary.  iadj(iend(k)) .eq. 0 iff node k is on the boundary.
c           iend is unchanged if iero = 1. if iero = 3 iend contains the indices
c           of a sequence of n nodes ordered from left to right where left
c           and right are defined by assuming node 1 is to the left of node 2.
c     iero - error indicator
c           iero = 0 if no errors were encountered.
c           iero = 3 if n .ge. 3 and all nodes are collinear.
c
      integer nn,k,km1,nl,nr,ind,indx,n0,itemp,ierr,km1d2,kmi,i,kmin
      double precision xl,yl,xr,yr,dxr,dyr,xk,yk,dxk,dyk,cprod,sprod
c nn =     local copy of n
c k =      node (index) to be inserted into iend
c km1 =    k-1 - (variable) length of iend
c nl,nr =  iend(1), iend(km1) -- leftmost and rightmost nodes in iend as 
c          viewed from the right of 1-2 when iend contains the initial ordered
c          set of nodal indices
c xl,yl,xr,yr = x and y coordinates of nl and nr
c dxr,dyr =     xr-xl, yr-yl
c xk,yk =       x and y coordinates of node k
c dxk,dyk =     xk-xl, yk-yl
c cprod =   vector cross product of nl-nr and nl-k -- used to determine 
c               the position of node k with respect to the line defined by the
c                 nodes in iend
c sprod =       scalar product used to determine the interval containing 
c               node k when k is on the line defined by the nodes in iend
c ind,indx =    indices for iend and iadj, respectively
c n0,itemp =    temporary nodes (indices)
c ierr =        dummy parameter for call to adnode
c km1d2,kmi,i = km1/2, k-i, do-loop index -- used in iend reordering loop
c kmin =        first node index sent to adnode
      nn = n
      iero = 0
      do 732,i=1,3
         do 733,j=1,n4
            nu(i,j)=0
 733     continue
 732     continue
c initialize iend, nl, nr, and k
      do 734,i=1,n
         iend(i)=0
 734  continue
      do 735,i=1,n6
         iadj(i)=0
 735  continue
      iend(1) = 1
      iend(2) = 2
      xl = x(1)
      yl = y(1)
      xr = x(2)
      yr = y(2)
      k = 2
c begin loop on nodes 3,4,...
 1    dxr = xr-xl
      dyr = yr-yl
c next loop begins here if nl and nr are unchanged
 2    if (k .eq. nn) go to 13
      km1 = k
      k = km1 + 1
      xk = x(k)
      yk = y(k)
      dxk = xk-xl
      dyk = yk-yl
      cprod = dxr*dyk - dxk*dyr
      if (cprod .gt. 0.) go to 6
      if (cprod .lt. 0.) go to 8
c node k lies on the line containing nodes 1,2,...,k-1.
c   set sprod to (nl-nr,nl-k).
      sprod = dxr*dxk + dyr*dyk
      if (sprod .gt. 0.) go to 3
c node k is to the left of nl.  insert k as the first
c   (leftmost) node in iend and set nl to k.
      call shiftd(1,km1,1, iend )
      iend(1) = k
      xl = xk
      yl = yk
      go to 1
c node k is to the right of nl.  find the leftmost node
c   n0 which lies to the right of k. set sprod to (n0-nl,n0-k).
 3    do 4 ind = 2,km1
         n0 = iend(ind)
         sprod = (xl-x(n0))*(xk-x(n0)) + (yl-y(n0))*(yk-y(n0))
         if (sprod .ge. 0.) go to 5
 4    continue
c node k is to the right of nr.  insert k as the last
c   (rightmost) node in iend and set nr to k.
      iend(k) = k
      xr = xk
      yr = yk
      go to 1
c node k lies between iend(ind-1) and iend(ind).  insert k in iend.
 5    call shiftd(ind,km1,1, iend )
      iend(ind) = k
      go to 2
c node k is to the left of nl-nr.  reorder iend so that nl
c   is the leftmost node as viewed from k.
 6    km1d2 = km1/2
      do 7 i = 1,km1d2
         kmi = k-i
         itemp = iend(i)
         iend(i) = iend(kmi)
         iend(kmi) = itemp
 7    continue
c node k is to the right of nl-nr.  create a triangulation
c   consisting of nodes 1,2,...,k.
 8    nl = iend(1)
      nr = iend(km1)
c create the adjacency lists for the first k-1 nodes. insert neighbors in 
c reverse order. each node has four neighbors except nl and nr which have three.
      do 9 ind = 1,km1
         n0 = iend(ind)
         indx = 4*n0
         if (n0 .ge. nl) indx = indx-1
         if (n0 .ge. nr) indx = indx-1
         iadj(indx) = 0
         indx = indx-1
         if (ind .lt. km1) iadj(indx) = iend(ind+1)
         if (ind .lt. km1) indx = indx-1
         iadj(indx) = k
         if (ind .eq. 1) go to 9
         iadj(indx-1) = iend(ind-1)
 9    continue
c     create the adjacency list for node k
      indx = 5*km1 - 1
      iadj(indx) = 0
      do 10 ind = 1,km1
         indx = indx-1
         iadj(indx) = iend(ind)
 10   continue
c     replace iend elements with pointers to iadj
      indx = 0
      do 11 ind = 1,km1
         indx = indx + 4
         if (ind .eq. nl  .or.  ind .eq. nr) indx = indx-1
         iend(ind) = indx
 11   continue
      indx = indx + k
      iend(k) = indx
c     add the remaining nodes to the triangulation
      if (k .eq. nn) go to 14
      kmin = k+1
      do 12 k = kmin,nn
         call adnode(k,x,y, iadj,iend, ierr)
 12   continue
      goto 14
c     all nodes are collinear
 13   iero = 3
      return
 14   continue
c commented lines below replaced by the following
c last triangle forgotten during processing adjacency lists
c MG 28 May 2002
      nbt=0
      iee=0
      do 15 i=1,n
         is=iee+1
         iee=iend(i)
         ie=iee
         ie=ie-1
         if (iadj(ie).eq.0) go to 17
         ie=iee
         itr1=i
         itr2=iadj(is)
         if(itr2.lt.itr1) goto 17
         itr3=iadj(ie)
         if(itr3.lt.itr1) goto 17
         nbt=nbt+1
         nu(1,nbt)=itr1
         nu(2,nbt)=itr2
         nu(3,nbt)=itr3
 17      continue
         itr1=i
         do 16 i2=is,(ie-1)
            itr2=iadj(i2)
            if(itr2.lt.itr1) goto 16
            itr3=iadj(i2+1)
            if(itr3.lt.itr1) goto 16
            nbt=nbt+1
            nu(1,nbt)=itr1
            nu(2,nbt)=itr2
            nu(3,nbt)=itr3
 16      continue
 15   continue
c      nbt=0
c      iee=0
c      do 15 i=1,n
c         is=iee+1
c         iee=iend(i)
c         ie=iee
c         if (iadj(ie).eq.0) ie=ie-1
c         itr1=i
c         do 16 i2=is,(ie-1)
c            itr2=iadj(i2)
c            if(itr2.lt.itr1) goto 16
c            itr3=iadj(i2+1)
c            if(itr3.lt.itr1) goto 16
c            nbt=nbt+1
c            nu(1,nbt)=itr1
c            nu(2,nbt)=itr2
c            nu(3,nbt)=itr3
c 16      continue
c 15   continue
      return
      end
c***********************************************************
      subroutine shiftd (nfrst,nlast,kk, iarr )
      integer nfrst, nlast, kk, iarr(1)
c this routine shifts a set of contiguous elements of an
c integer array kk positions downward (upward if kk .lt. 0).
c
c input parameters - 
c     nfrst,nlast - bounds on the portion of iarr to be shifted.  all
c          elements between and including the bounds are
c          shifted unless nfrst .gt. nlast, in which case no shift occurs.
c     kk - number of positions each element is to be shifted.
c          if kk .lt. 0 shift up.  if kk .gt. 0 shift down.
c     iarr - integer array of length .ge. nlast + max(kk,0).
c
c nfrst, nlast, and kk are not altered by this routine.
c
c output parameter -        iarr - shifted array.
      integer inc,k,nf,nl,nlp1,ns,nsl,i,ibak,indx,imax
      data    inc/5/
c inc =  do-loop increment (unrolling factor) -- if inc is
c          changed, statements must be added to or deleted from the do-loops
c k =    local copy of kk
c nf =   local copy of nfrst
c nl =   local copy of nlast
c nlp1 = nl + 1
c ns =   number of shifts
c nsl =  number of shifts done in unrolled do-loop (multiple of inc)
c i =    do-loop index and index for iarr
c ibak = index for downward shift of iarr
c indx = index for iarr
c imax = bound on do-loop index
      k = kk
      nf = nfrst
      nl = nlast
      if (nf .gt. nl  .or.  k .eq. 0) return
      nlp1 = nl + 1
      ns = nlp1 - nf
      nsl = inc*(ns/inc)
      if ( k .lt. 0) go to 4
c     shift downward starting from the bottom
      if (nsl .le. 0) go to 2
      do 1 i = 1,nsl,inc
         ibak = nlp1 - i
         indx = ibak + k
         iarr(indx) = iarr(ibak)
         iarr(indx-1) = iarr(ibak-1)
         iarr(indx-2) = iarr(ibak-2)
         iarr(indx-3) = iarr(ibak-3)
         iarr(indx-4) = iarr(ibak-4)
 1    continue
c     perform the remaining ns-nsl shifts one at a time
 2    ibak = nlp1 - nsl
 3    if (ibak .le. nf) return
      ibak = ibak - 1
      indx = ibak + k
      iarr(indx) = iarr(ibak)
      go to 3
c     shift upward starting from the top
 4    if (nsl .le. 0) go to 6
      imax = nlp1 - inc
      do 5 i = nf,imax,inc
         indx = i + k
         iarr(indx) = iarr(i)
         iarr(indx+1) = iarr(i+1)
         iarr(indx+2) = iarr(i+2)
         iarr(indx+3) = iarr(i+3)
         iarr(indx+4) = iarr(i+4)
 5    continue
c     perform the remaining ns-nsl shifts one at a time
 6    i = nsl + nf
 7    if (i .gt. nl) return
      indx = i + k
      iarr(indx) = iarr(i)
      i = i + 1
      go to 7
      end
c*****************************************************************************
      subroutine adnode (kk,x,y, iadj,iend, iero)
      integer kk, iadj(1), iend(kk), iero
      double precision    x(kk), y(kk)
      logical swptst
      external idoigt
c this routine adds node kk to a triangulation of a set of points  
c producing a new triangulation.  a sequence of edge swaps is then
c applied to the mesh, resulting in an optimal triangulation.  
c
c input parameters -  
c     kk - index of the node to be added to the mesh.  kk .ge. 4.
c     x,y - vectors of coordinates of the nodes in the mesh.  (x(i),y(i))
c           defines node i for i = 1,..,kk.
c     iadj,iend - triangulation data structure 
c kk, x, and y are not altered by this routine.
c
c output parameters - 
c     iadj,iend - updated with the addition of node kk as the last entry.
c     iero - error indicator
c           iero = 0 if no errors were encountered.
c           iero = 1 if all nodes (including kk) are collinear.
      integer k, km1, i1, i2, i3, indkf, indkl, nabor1,
     .        io1, io2, in1, indk1, ind2f, ind21
      double precision    xk, yk
c k =        local copy of kk
c km1 =      k - 1
c i1,i2,i3 = vertices of a triangle containing k
c indkf =    iadj index of the first neighbor of k
c indkl =    iadj index of the last neighbor of k
c nabor1 =   first neighbor of k before any swaps occur
c io1,io2 =  adjacent neighbors of k defining an arc to be tested for a swap
c in1 =      vertex opposite k -- first neighbor of io2 which precedes io1.
c            in1,io1,io2 are in counterclockwise order.
c indk1 =    index of io1 in the adjacency list for k
c ind2f =    index of the first neighbor of io2
c ind21 =    index of io1 in the adjacency list for io2
c xk,yk =    x(k), y(k)
      iero = 0
      k = kk
c     initialization
      km1 = k - 1
      xk = x(k)
      yk = y(k)
c     add node k to the mesh
      call trfind(km1,xk,yk,x,y,iadj,iend, i1,i2,i3)
      if (i1 .eq. 0) go to 5
      if (i3 .eq. 0) call bdyadd(k,i1,i2, iadj,iend )
      if (i3 .ne. 0) call intadd(k,i1,i2,i3, iadj,iend )
c     initialize variables for optimization of the mesh
      indkf = iend(km1) + 1
      indkl = iend(k)
      nabor1 = iadj(indkf)
      io2 = nabor1
      indk1 = indkf + 1
      io1 = iadj(indk1)
c     begin loop -- find the vertex opposite k
 1    ind2f = 1
      if (io2 .ne. 1) ind2f = iend(io2-1) + 1
      ind21 = idoigt(io2,io1,iadj,iend)
      if (ind2f .eq. ind21) go to 2
      in1 = iadj(ind21-1)
      go to 3
c     in1 is the last neighbor of io2
 2    ind21 = iend(io2)
      in1 = iadj(ind21)
      if (in1 .eq. 0) go to 4
c     swap test -- if a swap occurs, two new arcs are opposite k
c     and must be tested.  indk1 and indkf must be decremented.
 3    if ( .not. swptst(in1,k,io1,io2,x,y) ) go to 4
      call swap(in1,k,io1,io2, iadj,iend )
      io1 = in1
      indk1 = indk1 - 1
      indkf = indkf - 1
      go to 1
c     no swap occurred.  reset io2 and io1, and test for termination.
 4    if (io1 .eq. nabor1) return
      io2 = io1
      indk1 = indk1 + 1
      if (indk1 .gt. indkl) indk1 = indkf
      io1 = iadj(indk1)
      if (io1 .ne. 0) go to 1
      return
c     all nodes are collinear
 5    iero = 1
      return
      end
c**************************************************************************
      subroutine trfind (nst,px,py,x,y,iadj,iend, i1,i2,i3)
      integer nst, iadj(1), iend(1), i1, i2, i3
      double precision    px, py, x(1), y(1)
c this routine locates a point p in a thiessen triangulation, returning the
c vertex indices of a triangle which contains p.
c
c input parameters -    
c     nst - index of node at which trfind begins search. 
c     px,py - x and y-coordinates of the point to be located.
c     x,y - vectors of coordinates of nodes in the mesh.  (x(i),y(i))
c           defines node i for i = 1,...,n where n .ge. 3.
c     iadj,iend - triangulation data structure 
c
c input parameters are not altered by this routine.
c
c output parameters - 
c     i1,i2,i3 - vertex indices in counter-clockwise order - vertices
c       of a triangle containing p if p is an interior node. if p is
c      outside of the boundary of the mesh, i1 and i2 are the first (right
c      -most) and last (leftmost) nodes which are visible from p, and 
c     i3 = 0.  if p  and all of the nodes lie on a single line then i1 = i2
c                                = i3 = 0.
      integer n0,n1,n2,n3,n4,indx,ind,nf,nl,next
      double precision    xp, yp
      double precision    x1, y1, x2, y2, x0, y0
      logical left
c xp,yp =     local variables containing px and py
c n0,n1,n2 =  nodes in counterclockwise order defining a
c               cone (with vertex n0) containing p
c n3,n4 =     nodes opposite n1-n2 and n2-n1, respectively
c indx,ind =  indices for iadj
c nf,nl =     first and last neighbors of n0 in iadj, or first (rightmost) 
c             and last (leftmost) nodes visible from p when p is outside the
c               boundary
c next =      candidate for i1 or i2 when p is outside of the boundary
c left =      statement function which computes the sign of a cross product
c         (z-component).  left(x1,...,y0) = .true. iff (x0,y0) is on or to the
c         left of the vector from (x1,y1) to (x2,y2).
c
      left(x1,y1,x2,y2,x0,y0) = (x2-x1)*(y0-y1) .ge.
     .                          (x0-x1)*(y2-y1)
      xp = px
      yp = py
c initialize variables and find a cone containing p
      n0 = max0(nst,1)
    1 indx = iend(n0)
      nl = iadj(indx)
      indx = 1
      if (n0 .ne. 1) indx = iend(n0-1) + 1
      nf = iadj(indx)
      n1 = nf
      if (nl .ne. 0) go to 3
c     n0 is a boundary node.  set nl to the last nonzero neighbor of n0.
      ind = iend(n0) - 1
      nl = iadj(ind)
      if ( left(x(n0),y(n0),x(nf),y(nf),xp,yp) ) go to 2
c     p is outside the boundary
      nl = n0
      go to 16
 2    if ( left(x(nl),y(nl),x(n0),y(n0),xp,yp) ) go to 4
c     p is outside the boundary and n0 is the rightmost visible boundary
c     node
      i1 = n0
      go to 18
c     n0 is an interior node.  find n1.
 3    if ( left(x(n0),y(n0),x(n1),y(n1),xp,yp) ) go to 4
      indx = indx + 1
      n1 = iadj(indx)
      if (n1 .eq. nl) go to 7
      go to 3
c     p is to the left of arc n0-n1.  initialize n2 to the next neighbor
c     of n0.
 4    indx = indx + 1
      n2 = iadj(indx)
      if ( .not. left(x(n0),y(n0),x(n2),y(n2),xp,yp) ) go to 8
      n1 = n2
      if (n1 .ne. nl) go to 4
      if ( .not. left(x(n0),y(n0),x(nf),y(nf),xp,yp) ) go to 7
      if (xp .eq. x(n0) .and. yp .eq. y(n0)) go to 6
c     p is left of or on arcs n0-nb for all neighbors nb of n0.
c     all points are collinear iff p is left of nb-n0 for
c     all neighbors nb of n0.  search the neighbors of n0
c     in reverse order.  note -- n1 = nl and indx points to nl.
 5    if ( .not. left(x(n1),y(n1),x(n0),y(n0),xp,yp) ) go to 6
      if (n1 .eq. nf) go to 20
      indx = indx - 1
      n1 = iadj(indx)
      go to 5
c     p is to the right of n1-n0, or p=n0.  set n0 to n1 and start over.
 6    n0 = n1
      go to 1
c     p is between arcs n0-n1 and n0-nf
 7    n2 = nf
c     p is contained in a cone defined by line segments n0-n1
c     and n0-n2 where n1 is adjacent to n2
 8    n3 = n0
 9    if ( left(x(n1),y(n1),x(n2),y(n2),xp,yp) ) go to 13
c     set n4 to the first neighbor of n2 following n1
      indx = iend(n2)
      if (iadj(indx) .ne. n1) go to 10
c     n1 is the last neighbor of n2. set n4 to the first neighbor.
      indx = 1
      if (n2 .ne. 1) indx = iend(n2-1) + 1
      n4 = iadj(indx)
      go to 11
c     n1 is not the last neighbor of n2
 10   indx = indx-1
      if (iadj(indx) .ne. n1) go to 10
      n4 = iadj(indx+1)
      if (n4 .ne. 0) go to 11
c     p is outside the boundary
      nf = n2
      nl = n1
      go to 16
c     define a new arc n1-n2 which intersects the line segment n0-p
 11   if ( left(x(n0),y(n0),x(n4),y(n4),xp,yp) ) go to 12
      n3 = n2
      n2 = n4
      go to 9
 12   n3 = n1
      n1 = n4
      go to 9
c     p is in the triangle (n1,n2,n3) and not on n2-n3.  if n3-n1
c     or n1-n2 is a boundary arc containing p, treat p as exterior.
 13   indx = iend(n1)
      if (iadj(indx) .ne. 0) go to 15
c     n1 is a boundary node.  n3-n1 is a boundary arc iff n3
c     is the last nonzero neighbor of n1.
      if (n3 .ne. iadj(indx-1)) go to 14
c     n3-n1 is a boundary arc
      if ( .not. left(x(n1),y(n1),x(n3),y(n3),xp,yp) ) go to 14
c     p lies on n1-n3
      i1 = n1
      i2 = n3
      i3 = 0
      return
c     n3-n1 is not a boundary arc containing p.  n1-n2 is a
c     boundary arc iff n2 is the first neighbor of n1.
 14   indx = 1
      if (n1 .ne. 1) indx = iend(n1-1) + 1
      if (n2 .ne. iadj(indx)) go to 15
c     n1-n2 is a boundary arc
      if ( .not. left(x(n2),y(n2),x(n1),y(n1),xp,yp) ) go to 15
c     p lies on n1-n2
      i1 = n2
      i2 = n1
      i3 = 0
      return
c     p does not lie on a boundary arc.
 15   i1 = n1
      i2 = n2
      i3 = n3
      return
c     nf and nl are adjacent boundary nodes which are visible
c     from p.  find the first visible boundary node.
c     set next to the first neighbor of nf.
 16   indx = 1
      if (nf .ne. 1) indx = iend(nf-1) + 1
      next = iadj(indx)
      if ( left(x(nf),y(nf),x(next),y(next),xp,yp) ) go to 17
      nf = next
      go to 16
c     nf is the first (rightmost) visible boundary node
 17   i1 = nf
c     find the last visible boundary node.  nl is the first candidate
c     for i2. set next to the last neighbor of nl.
 18   indx = iend(nl) - 1
      next = iadj(indx)
      if ( left(x(next),y(next),x(nl),y(nl),xp,yp) ) go to 19
      nl = next
      go to 18
c     nl is the last (leftmost) visible boundary node
 19   i2 = nl
      i3 = 0
      return
c     all points are collinear
 20   i1 = 0
      i2 = 0
      i3 = 0
      return
      end
c*************************************************************************
      subroutine intadd (kk,i1,i2,i3, iadj,iend )
      integer kk, i1, i2, i3, iadj(1), iend(kk)
c this routine adds an interior node to a triangulation of kk-1 points.
c iadj and iend are updated with the insertion of node kk in the triangle
c whose vertices are i1, i2, and i3.
c
c input parameters -        
c     kk - index of node to be inserted.  kk .ge. 4.
c     i1,i2,i3 - indices of the vertices of a triangle containing node
c                kk -- in counterclockwise order.
c     iadj,iend - triangulation data structure 
c
c kk, i1, i2, and i3 are not altered by this routine.
c
c output parameters - 
c     iadj,iend - updated with the addition of node kk as the last
c              entry.  node kk will be connected to nodes i1, i2,
c              and i3.  no optimization of the mesh is performed.
      integer k, km1, n(3), nft(3), ip1, ip2, ip3, indx, nf,
     .        nl, n1, n2, imin, imax, i, itemp
c k =           local copy of kk
c km1 =         k - 1
c n =           vector containing i1, i2, i3
c nft =         pointers to the tops of the 3 sets of iadj
c                 elements to be shifted downward
c ip1,ip2,ip3 = permutation indices for n and nft
c indx =        index for iadj and n
c nf,nl =       indices of first and last entries in iadj to be shifted down
c n1,n2 =       first 2 vertices of a new triangle -- (n1,n2,kk)
c imin,imax =   bounds on do-loop index -- first and last
c                 elements of iend to be incremented
c i =           do-loop index
c itemp =       temporary storage location
      k = kk
c     initialization
      n(1) = i1
      n(2) = i2
      n(3) = i3
c     set up nft
      do 2 i = 1,3
         n1 = n(i)
         indx = mod(i,3) + 1
         n2 = n(indx)
         indx = iend(n1) + 1
c     find the index of n2 as a neighbor of n1
 1       indx = indx - 1
         if (iadj(indx) .ne. n2) go to 1
         nft(i) = indx + 1
 2    continue
c     order the vertices by decreasing magnitude.
c     n(ip(i+1)) precedes n(ip(i)) in iend for i = 1,2.
      ip1 = 1
      ip2 = 2
      ip3 = 3
      if ( n(2) .le. n(1) ) go to 3
      ip1 = 2
      ip2 = 1
 3    if ( n(3) .le. n(ip1) ) go to 4
      ip3 = ip1
      ip1 = 3
 4    if ( n(ip3) .le. n(ip2) )  go to 5
      itemp = ip2
      ip2 = ip3
      ip3 = itemp
c     add node k to the adjacency lists of each vertex and update iend.
c     for each vertex, a set of iadj elements is shifted downward and 
c     and k is inserted.  shifting starts at the end of the array.
 5    km1 = k - 1
      nl = iend(km1)
      nf = nft(ip1)
      if (nf .le. nl) call shiftd(nf,nl,3, iadj )
      iadj(nf+2) = k
      imin = n(ip1)
      imax = km1
      do 6 i = imin,imax
         iend(i) = iend(i) + 3
 6    continue
c
      nl = nf - 1
      nf = nft(ip2)
      call shiftd(nf,nl,2, iadj )
      iadj(nf+1) = k
      imax = imin - 1
      imin = n(ip2)
      do 7 i = imin,imax
         iend(i) = iend(i) + 2
 7    continue
c
      nl = nf - 1
      nf = nft(ip3)
      call shiftd(nf,nl,1, iadj )
      iadj(nf) = k
      imax = imin - 1
      imin = n(ip3)
      do 8 i = imin,imax
         iend(i) = iend(i) + 1
 8    continue
c     add node k to iend and its neighbors to iadj
      indx = iend(km1)
      iend(k) = indx + 3
      do 9 i = 1,3
         indx = indx + 1
         iadj(indx) = n(i)
 9    continue
      return
      end
c******************************************************************************
      subroutine bdyadd (kk,i1,i2, iadj,iend )
      integer kk, i1, i2, iadj(1), iend(kk)
c this routine adds a boundary node to a triangulation of kk-1 points
c iadj and iend are updated with the insertion of node kk.
c
c input parameters -   
c     kk - index of an exterior node to be added.  kk .ge. 4.
c     i1 - first (rightmost as viewed from kk) boundary node in the mesh
c          which is visible from kk - the line segment kk-i1 intersects no arcs.
c     i2 - last (leftmost) boundary node which is visible from kk.
c     iadj,iend - triangulation data structure 
c
c output parameters - 
c     iadj,iend - updated with the addition of node kk as the last entry.
c                node kk will be connected to i1, i2, and all boundary
c                nodes between them.  no optimization of the mesh is performed.
      integer k, km1, nright, nleft, nf, nl, n1, n2, i,
     .        imin, imax, kend, next, indx
c k =            local copy of kk
c km1 =          k - 1
c nright,nleft = local copies of i1, i2
c nf,nl =        indices of iadj bounding the portion of the array to be shifted
c n1 =           iadj index of the first neighbor of nleft
c n2 =           iadj index of the last neighbor of nright
c i =            do-loop index
c imin,imax =    bounds on do-loop index -- first and last
c                  elements of iend to be incremented
c kend =         pointer to the last neighbor of k in iadj
c next =         next boundary node to be connected to kk
c indx =         index for iadj
      k = kk
      km1 = k - 1
      nright = i1
      nleft = i2
c     initialize variables
      nl = iend(km1)
      n1 = 1
      if (nleft .ne. 1) n1 = iend(nleft-1) + 1
      n2 = iend(nright)
      nf = max0(n1,n2)
c     insert k as a neighbor of max(nright,nleft)
      call shiftd(nf,nl,2, iadj )
      iadj(nf+1) = k
      imin = max0(nright,nleft)
      do 1 i = imin,km1
         iend(i) = iend(i) + 2
 1    continue
c     initialize kend and insert k as a neighbor of min(nright,nleft)
      kend = nl + 3
      nl = nf - 1
      nf = min(n1,n2)
      call shiftd(nf,nl,1, iadj )
      iadj(nf) = k
      imax = imin - 1
      imin = min(nright,nleft)
      do 2 i = imin,imax
         iend(i) = iend(i) + 1
 2    continue
c     insert nright as the first neighbor of k
      iadj(kend) = nright
c     initialize indx for loop on boundary nodes between nright and nleft
      indx = iend(nright) - 2
 3    next = iadj(indx)
      if (next .eq. nleft) go to 4
c     connect next and k
      kend = kend + 1
      iadj(kend) = next
      indx = iend(next)
      iadj(indx) = k
      indx = indx - 1
      go to 3
c     insert nleft and 0 as the last neighbors of k
 4    iadj(kend+1) = nleft
      kend = kend + 2
      iadj(kend) = 0
      iend(k) = kend
      return
      end
c****************************************************************************
      subroutine swap (nin1,nin2,nout1,nout2, iadj,iend )
      integer nin1, nin2, nout1, nout2, iadj(1), iend(1)
      external idoigt
c this subroutine swaps the diagonals in a convex quadrilateral.
c
c input parameters -  
c     nin1,nin2,nout1,nout2 - nodal indices of a pair of adjacent triangles
c          which form a convex quadrilateral.  nout1 and nout2 are connected
c          by an arc which is to be replaced by the arc nin1-nin2. (nin1,nout1,
c          nout2) must be triangle vertices in counterclockwise order.
c
c the above parameters are not altered by this routine.
c
c     iadj,iend - triangulation data structure 
c
c output parameters - iadj,iend - updated with the arc replacement.
      integer in(2), io(2), ip1, ip2, j, k, nf, nl, i,
     .        imin, imax
c in =        nin1 and nin2 ordered by increasing magnitude
c               (the neighbors of in(1) precede those of in(2) in iadj)
c io =        nout1 and nout2 in increasing order
c ip1,ip2 =   permutation of (1,2) such that io(ip1)
c               precedes io(ip2) as a neighbor of in(1)
c j,k =       permutation of (1,2) used as indices of in and io
c nf,nl =     iadj indices boundary a portion of the array to be shifted
c i =         iend index
c imin,imax = bounds on the portion of iend to be incremented or decremented
      in(1) = nin1
      in(2) = nin2
      io(1) = nout1
      io(2) = nout2
      ip1 = 1
c     order the indices so that in(1).lt.in(2) and io(1).lt.io(2), and choose
c     ip1 and ip2 such that (in(1),io(ip1), io(ip2)) forms a triangle.
      if (in(1) .lt. in(2)) go to 1
      in(1) = in(2)
      in(2) = nin1
      ip1 = 2
 1    if (io(1) .lt. io(2)) go to 2
      io(1) = io(2)
      io(2) = nout1
      ip1 = 3 - ip1
 2    ip2 = 3 - ip1
      if (io(2) .lt. in(1)) go to 8
      if (in(2) .lt. io(1)) go to 12
c     in(1) and io(1) precede in(2) and io(2).  for (j,k) =
c     (1,2) and (2,1), delete io(k) as a neighbor of io(j)
c     by shifting a portion of iadj either up or down and
c     and insert in(k) as a neighbor of in(j).
      do 7 j = 1,2
         k = 3 - j
         if (in(j) .gt. io(j)) go to 4
c     the neighbors of in(j) precede those of io(j) -- shift down by 1
         nf = 1 + idoigt(in(j),io(ip1),iadj,iend)
         nl = -1 + idoigt(io(j),io(k),iadj,iend)
         if (nf .le. nl) call shiftd(nf,nl,1, iadj )
         iadj(nf) = in(k)
         imin = in(j)
         imax = io(j)-1
         do 3 i = imin,imax
            iend(i) = iend(i) + 1
 3       continue
         go to 6
c     the neighbors of io(j) precede those of in(j) -- shift up by 1
 4       nf = 1 + idoigt(io(j),io(k),iadj,iend)
         nl = -1 + idoigt(in(j),io(ip2),iadj,iend)
         if (nf .le. nl) call shiftd(nf,nl,-1, iadj )
         iadj(nl) = in(k)
         imin = io(j)
         imax = in(j) - 1
         do 5 i = imin,imax
            iend(i) = iend(i) - 1
 5       continue
c     reverse (ip1,ip2) for (j,k) = (2,1)
 6       ip1 = ip2
         ip2 = 3 - ip1
 7    continue
      return
c     the vertices are ordered (io(1),io(2),in(1),in(2)).
c     delete io(2) by shifting up by 1
 8    nf = 1 + idoigt(io(1),io(2),iadj,iend)
      nl = -1 + idoigt(io(2),io(1),iadj,iend)
      if (nf .le. nl) call shiftd(nf,nl,-1, iadj )
      imin = io(1)
      imax = io(2)-1
      do 9 i = imin,imax
         iend(i) = iend(i) - 1
 9    continue
c     delete io(1) by shifting up by 2 and insert in(2)
      nf = nl + 2
      nl = -1 + idoigt(in(1),io(ip2),iadj,iend)
      if (nf .le. nl) call shiftd(nf,nl,-2, iadj )
      iadj(nl-1) = in(2)
      imin = io(2)
      imax = in(1)-1
      do 10 i = imin,imax
         iend(i) = iend(i) - 2
 10   continue
c     shift up by 1 and insert in(1)
      nf = nl + 1
      nl = -1 + idoigt(in(2),io(ip1),iadj,iend)
      call shiftd(nf,nl,-1, iadj )
      iadj(nl) = in(1)
      imin = in(1)
      imax = in(2)-1
      do 11 i = imin,imax
 11      iend(i) = iend(i) - 1
      return
c     the vertices are ordered (in(1),in(2),io(1),io(2)).
c     delete io(1) by shifting down by 1
 12   nf = 1 + idoigt(io(1),io(2),iadj,iend)
      nl = -1 + idoigt(io(2),io(1),iadj,iend)
      if (nf .le. nl) call shiftd(nf,nl,1, iadj )
      imin = io(1)
      imax = io(2) - 1
      do 13 i = imin,imax
 13      iend(i) = iend(i) + 1
c     delete io(2) by shifting down by 2 and insert in(1)
      nl = nf - 2
      nf = 1 + idoigt(in(2),io(ip2),iadj,iend)
      if (nf .le. nl) call shiftd(nf,nl,2, iadj )
      iadj(nf+1) = in(1)
      imin = in(2)
      imax = io(1) - 1
      do 14 i = imin,imax
 14      iend(i) = iend(i) + 2
c     shift down by 1 and insert in(2)
      nl = nf - 1
      nf = 1 + idoigt(in(1),io(ip1),iadj,iend)
      call shiftd(nf,nl,1, iadj )
      iadj(nf) = in(2)
      imin = in(1)
      imax = in(2) - 1
      do 15 i = imin,imax
 15      iend(i) = iend(i) + 1
      return
      end
c***********************************************************
      logical function swptst (in1,in2,io1,io2,x,y)
      integer in1, in2, io1, io2
      double precision    x(1), y(1)
c this function decides whether or not to replace a diagonal arc in a
c quadrilateral with the other diagonal. the determination is based on
c the sizes of the angles contained in the 2 triangles defined by the diagonal.
c the diagonal is chosen to maximize the smallest of the
c six angles over the two pairs of triangles.
c
c input parameters -  
c     in1,in2,io1,io2 - node indices of the four points defining the
c         quadrilateral.  io1 and io2 are currently connected by a
c         diagonal arc.  this arc should be replaced by an arc
c         connecting in1, in2 if the decision is made to swap.
c         in1,io1,io2 must be in counterclockwise order.
c     x,y - vectors of nodal coordinates. (x(i),y(i)) are the coord-
c         inates of node i for i = in1,in2, io1, or io2.
c
c none of the input parameters are altered by this routine.
c
c output parameter -  swptst - .true. iff the arc connecting
c                              io1 and io2 is to be replaced
      double precision dx11, dx12, dx22, dx21, dy11, dy12, dy22, dy21,
     .     sin1, sin2, cos1, cos2, sin12
c dx11,dy11 = x,y coordinates of the vector in1-io1
c dx12,dy12 = x,y coordinates of the vector in1-io2
c dx22,dy22 = x,y coordinates of the vector in2-io2
c dx21,dy21 = x,y coordinates of the vector in2-io1
c sin1 =      cross product of the vectors in1-io1 and in1-io2 -- proportional
c             to sin(t1) where t1 is the angle at in1 formed by the vectors
c cos1 =      inner product of the vectors in1-io1 and in1-io2 -- proportional
c             to cos(t1)
c sin2 =     cross product of the vectors in2-io2 and in2-io1 -- proportional  
c             to sin(t2) where t2 is the angle at in2 formed by the vectors
c cos2 =      inner product of the vectors in2-io2 and in2-io1 -- proportional
c             to cos(t2)
c sin12 =     sin1*cos2 + cos1*sin2 -- proportional to sin(t1+t2)
      swptst = .false.
c     compute the vectors containing the angles t1, t2
      dx11 = x(io1) - x(in1)
      dx12 = x(io2) - x(in1)
      dx22 = x(io2) - x(in2)
      dx21 = x(io1) - x(in2)
c
      dy11 = y(io1) - y(in1)
      dy12 = y(io2) - y(in1)
      dy22 = y(io2) - y(in2)
      dy21 = y(io1) - y(in2)
c     compute inner products
      cos1 = dx11*dx12 + dy11*dy12
      cos2 = dx22*dx21 + dy22*dy21
c     the diagonals should be swapped iff (t1+t2) .gt. 180
c     degrees.  the following two tests insure numerical stability.
      if (cos1 .ge. 0.  .and.  cos2 .ge. 0.) return
      if (cos1 .lt. 0.  .and.  cos2 .lt. 0.) go to 1
c     compute vector cross products
      sin1 = dx11*dy12 - dx12*dy11
      sin2 = dx22*dy21 - dx21*dy22
      sin12 = sin1*cos2 + cos1*sin2
      if (sin12 .ge. 0.) return
    1 swptst = .true.
      return
      end
c****************************************************************************
      integer function idoigt (nvertx,nabor,iadj,iend)
      integer nvertx, nabor, iadj(1), iend(1)
c this function returns the index of nabor in the adjacency list for nvertx.
c input parameters - 
c     nvertx - node whose adjacency list is to be searched.
c     nabor - node whose index is to be returned.  nabor must be
c                             connected to nvertx.
c     iadj - set of adjacency lists.
c     iend - pointers to the ends of adjacency lists in iadj.
c
c output parameter -  idoigt - iadj(idoigt) = nabor.
      integer nb, indx
      nb = nabor
c     initialization
      indx = iend(nvertx) + 1
c     search the list of nvertx neighbors for nb
    1 indx = indx - 1
      if (iadj(indx) .ne. nb) go to 1
c
      idoigt = indx
      return
      end
