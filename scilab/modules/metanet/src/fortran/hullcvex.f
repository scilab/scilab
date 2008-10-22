c     from algorithm 523, acm.
      subroutine hullcvex(n,nn,xx,nhull,iwork,in,ih,il)
      implicit doubleprecision (a-h,o-z)
      dimension xx(2,n),in(n),ih(n),iwork(nn),il(nn)
      ncount=0
      nhull=0
      do 1,i=1,nn
         iwork(i)=0
 1    continue
c ncount is total number of points passed to split
      n1=n+1
      do 2 i=1,n
         j=n1-i
         in(j)=i
 2    continue
c array in contains indices 1-n in reverse order
      do 10 m=4,n
         call convex(n,xx,m,in,iwork,iwork(n+1),ih,nhull,il)
         ik=il(1)
         do 6 i=1,nhull
            j=ih(ik)
            iwork(i)=j
            ik=il(ik)
 6       continue
 10   continue
      end
      subroutine husplit(n,x,m,in,ii,jj,s,iabv,na,maxa,ibel,
     1  nb,maxb)
c this subroutine takes the m points of array x whose subscripts are in
c  array in
c and partitions them by the line joining the two points in array x whose  
c subscripts are ii and jj. the subscripts of the points above the line
c are put into array iabv, and the subscripts of the points below are put
c into array ibel. na and nb are,respectively, the number of points above 
c the line and the number below. maxa and maxb are the subscripts for array
c x of the point furthest above the line and the point furthest below, 
c respectively. if either subset is null the corresponding 
c subscript (maxa or maxb) is set to zero
c input
c n    integer           total number of data points
c x    real array (2,n)  (x,y) co-ordinates of the data
c m    integer           number of points in input subset
c in   integer array (m) subscripts for array x of the 
c                        points in the input subset
c ii   integer           subscript for array x of one point
c                        on the partitioning line
c jj   integer           subscript for array x of another
c                        point on the partitioning line
c s    integer           switch to determine output. refer
c                        to comments below
c output
c iabv integer array (m) subscripts for array x of the
c                        points above the partitioning line
c na   integer           number of elements in iabv
c maxa integer           subscript for array x of point
c                        furthest above the line. set to
c                        zero if na is zero
c ibel integer array (m) subscripts for array x of the
c                        points below the partitioning line
c nb   integer           number of elements in ibel
c maxb integer           subscript for array x of point
c                        furthest below the line. set to
c                        zero if nb is zero
      implicit doubleprecision (a-h,o-z)
      dimension x(2,n)
      dimension in(m),iabv(m),ibel(m)
      integer s
c if s = 2 dont save ibel,nb,maxb.
c if s =-2 dont save iabv,na,maxa.
c otherwise save everything
c if s is positive the array being partitioned is above the initial 
c partitioning line. if it is negative, then the set of points is below.
      logical t
      t=.false.
c check to see if the line is vertical
      if(x(1,jj).ne.x(1,ii))goto 1
      xt=x(1,ii)
      dir=sign(1.D0,x(2,jj)-x(2,ii))*sign(1.D0,dble(s))
      t=.true.
      goto 2
1     a=(x(2,jj)-x(2,ii))/(x(1,jj)-x(1,ii))
      b=x(2,ii)-a*x(1,ii)
2     up=0.
      na=0
      maxa=0
      down=0.
      nb=0
      maxb=0
      do 6 i=1,m
        is=in(i)
        if(t)goto 3
        z=x(2,is)-a*x(1,is)-b
        goto 4
3       z=dir*(x(1,is)-xt)
4       if(z.le.0.)goto 5
c the point is above the line
        if(s.eq.-2)goto 6
        na=na+1
        iabv(na)=is
        if(z.lt.up)goto 6
        up=z
        maxa=na
        goto 6
5       if(s.eq.2)goto 6
        if(z.ge.0.)goto 6
c the point is below the line
        nb=nb+1
        ibel(nb)=is
        if(z.gt.down)goto 6
        down=z
        maxb=nb
6     continue
      return
      end
      subroutine convex(n,x,m,in,ia,ib,ih,nh,il)
c this subroutine determines which of the m points of array x whose subscripts
c are in array in are vertices of the minimum area convex polygon containing
c the m points. the subscripts of the vertices are placed in array ih in the
c order they are found. nh is the number of elements in array ih and  array il.
c array il is a linked list giving the order of the elements of array ih
c in a counter clockwise direction. this algorithm corresponds to a
c preorder traversal of a certain binary tree. each vertex of the binary tree
c represents a subset of the m points. at each step the subset of points 
c corresponding to the current vertex of the tree is partitioned by a line
c joining two vertices of the convex polygon. the left son vertex in the binary
c tree represents the subset of points above the partitioning line and the 
c right son vertex, the subset below the line. the leaves of the tree represent
c either null subsets or subsets inside a triangle whose vertices coincide 
c with vertices of the convex polygon.
c formal parameters
c input
c n  integer           total number of data points
c x  real array (2,n)  (x,y) co-ordinates of the data
c m  integer           number of points in the input subset
c in integer array (m) subscripts for array x of the points
c                      in the input subset
c work area
c ia integer array (m) subscripts for array x of left son
c                      subsets. see comments after dimension
c                      statements
c ib integer array (m) subscripts for array x of right son
c                      subsets
c output
c ih integer array (m) subscripts for array x of the
c                      vertices of the convex hull
c nh integer           number of elements in array ih and
c                      array il. same as number of vertices
c                      of the convex polygon
c il integer array (m) a linked list giving in order in a
c                      counter-clockwise direction the
c                      elements of array ih
      implicit doubleprecision (a-h,o-z)
      dimension x(2,n)
      dimension in(m),ia(m),ib(m),ih(m),il(m)
c the upper end of array ia is used to store temporarily the sizes of the 
c subsets which correspond to right son vertices, while traversing down 
c the left sons when on the left half of the tree, and to store the sizes 
c of the left sons while traversing the right sons(down the right half)
      logical maxe,mine
      if(m.eq.1)goto 22
      il(1)=2
      il(2)=1
      kn=in(1)
      kx=in(2)
      if(m.eq.2)goto 21
      mp1=m+1
      min=1
      mx=1
      kx=in(1)
      maxe=.false.
      mine=.false.
c find two vertices of the convex hull for the initial partition
      do 6 i=2,m
        j=in(i)
        CRES=x(1,j)-x(1,kx)
        if (CRES .lt. 0) then 
           goto 3
        elseif (CRES .eq. 0) then
           goto 1
        else
           goto 2
        endif
1       maxe=.true.
        goto 3
2       maxe=.false.
        mx=i
        kx=j
3       CRES=x(1,j)-x(1,kn)
        if (CRES .lt. 0) goto 5
        if (CRES .eq. 0) goto 4
        goto 6
4       mine=.true.
        goto 6
5       mine=.false.
        min=i
        kn=j
6     continue
c if the max and min are equal, all m points lie on a vertical line
      if(kx.eq.kn)goto 18
c if maxe (or mine) has the value true there are several
c maxima (or minima) with equal first coordinates
      if(maxe.or.mine)goto 23
7     ih(1)=kx
      ih(2)=kn
      nh=3
      inh=1
      nib=1
      ma=m
      in(mx)=in(m)
      in(m)=kx
      mm=m-2
      if(min.eq.m)min=mx
      in(min)=in(m-1)
      in(m-1)=kn
c begin by partitioning the root of the tree
      call husplit(n,x,mm,in,ih(1),ih(2),0,ia,mb,mxa,ib,ia(ma),
     1  mxbb)
c first traverse the left half of the tree start with the left son
8     nib=nib+ia(ma)
      ma=ma-1
9     if(mxa.eq.0)goto 11
      il(nh)=il(inh)
      il(inh)=nh
      ih(nh)=ia(mxa)
      ia(mxa)=ia(mb)
      mb=mb-1
      nh=nh+1
      if(mb.eq.0)goto 10
      ilinh=il(inh)
      call husplit(n,x,mb,ia,ih(inh),ih(ilinh),1,ia,mbb,mxa,
     1  ib(nib),ia(ma),mxb)
      mb=mbb
      goto 8
c then the right son
10    inh=il(inh)
11    inh=il(inh)
      ma=ma+1
      nib=nib-ia(ma)
      if(ma.ge.m)goto 12
      if(ia(ma).eq.0)goto 11
      ilinh=il(inh)
c on the left side of the tree, the right son of a right son must represent 
c a subset of points which is inside a triangle with vertices which are also 
c vertices of the convex polygon and hence the subset may be neglected.
      call husplit(n,x,ia(ma),ib(nib),ih(inh),ih(ilinh),2,ia,
     1  mb,mxa,ib(nib),mbb,mxb)
      ia(ma)=mbb
      goto 9
c now traverse the right half of the tree
12    mxb=mxbb
      ma=m
      mb=ia(ma)
      nia=1
      ia(ma)=0
c start with the right son
13    nia=nia+ia(ma)
      ma=ma-1
14    if(mxb.eq.0)goto 16
      il(nh)=il(inh)
      il(inh)=nh
      ih(nh)=ib(mxb)
      ib(mxb)=ib(mb)
      mb=mb-1
      nh=nh+1
      if(mb.eq.0)goto 15
      ilinh=il(inh)
      call husplit(n,x,mb,ib(nib),ih(inh),ih(ilinh),-1,ia(nia),
     1  ia(ma),mxa,ib(nib),mbb,mxb)
      mb=mbb
      goto 13
c then the left son
15    inh=il(inh)
16    inh=il(inh)
      ma=ma+1
      nia=nia-ia(ma)
      if(ma.eq.mp1)goto 17
      if(ia(ma).eq.0)goto 16
      ilinh=il(inh)
c on the right side of the tree, the left son of a left son must represent 
c a subset of points which is inside a triangle with vertices which are 
c also vertices of the convex polygon and hence the subset may be neglected.
      call husplit(n,x,ia(ma),ia(nia),ih(inh),ih(ilinh),-2,
     1  ia(nia),mbb,mxa,ib(nib),mb,mxb)
      goto 14
17    nh=nh-1
      return
c all the special cases are handled down here
c if all the points lie on a vertical line
18    kx=in(1)
      kn=in(1)
      do 20 i=1,m
        j=in(i)
        if(x(2,j).le.x(2,kx))goto 19
        mx=i
        kx=j
19      if(x(2,j).ge.x(2,kn))goto 20
        min=i
        kn=j
20    continue
      if(kx.eq.kn)goto 22
c if there are only two points
21    ih(1)=kx
      ih(2)=kn
      nh=3
      if((x(1,kn).eq.x(1,kx)).and.(x(2,kn).eq.x(2,kx)))nh=2
      goto 17
c if there is only one point
22    nh=2
      ih(1)=in(1)
      il(1)=1
      goto 17
c multiple extremes are handled here if there are several points 
c with the (same) largest first coordinate
23    if(.not.maxe)goto 25
      do 24 i=1,m
        j=in(i)
        if(x(1,j).ne.x(1,kx))goto 24
        if(x(2,j).le.x(2,kx))goto 24
        mx=i
        kx=j
24    continue
c if there are several points with the (same) smallest first coordinate
25    if(.not.mine)goto 7
      do 26 i=1,m
        j=in(i)
        if(x(1,j).ne.x(1,kn))goto 26
        if(x(2,j).ge.x(2,kn))goto 26
        min=i
        kn=j
26    continue
      goto 7
      end
