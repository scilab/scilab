      subroutine dhetr(na,nb,nc,l,m,n,low,igh,a,b,c,ort)
      double precision a(na,n),b(nb,m),c(nc,n),ort(n)
c
c     *** purpose
c
c     given a real general matrix a, shetr reduces a submatrix
c     of a in rows and columns low through igh to upper hessenberg
c     form by orthogonal similarity transformations.  these
c     orthogonal transformations are further accumulated into rows
c     low through igh of an n x m matrix b and columns low
c     through igh of an l x n matrix c by premultiplication and
c     postmultiplication, respectively.
c
c
c        b        double precision(nb,m)
c                 an n x m double precision matrix
c
c        c        double precision(nc,n)
c                 an l x n double precision matrix.
c
c     on return:
c
c        a        an upper hessenberg matric similar to (via an
c                 orthogonal matrix consisting of a sequence of
c                 householder transformations) the original matrix a;
c                 further information concerning the orthogonal
c                 transformations used in the reduction is contained
c                 in the elements below the first subdiagonal; see
c                 orthes documentation for details.
c
c        b        the original b matrix premultiplied by the transpose
c                 of the orthogonal transformation used to reduce a.
c
c        c        the original c matrix postmultiplied by the orthogonal
c                 transformation used to reduce a.
c
c        ort      double precision(n)
c                 a work vector containing information about the
c                 orthogonal transformations; see orthes documentation
c                 for details.
c
c     this version dated july 1980.
c     alan j. laub, university of southern california.
c
c     subroutines and functions called:
c
c     none
c
c     internal variables:
c
      integer i,ii,j,jj,k,kp1,kpn,la
      double precision f,g,h,scale
c
c     fortran functions called:
c
      la = igh-1
      kp1 = low+1
      if (la .lt. kp1) go to 170
      do 160 k = kp1,la
         h = 0.0d+0
         ort(k) = 0.0d+0
         scale = 0.0d+0
c
c        scale column
c
         do 10 i = k,igh
              scale = scale+abs(a(i,k-1))
   10    continue
         if (scale .eq. 0.0d+0) go to 150
          kpn=k+igh
         do 20 ii = k,igh
              i = kpn-ii
              ort(i) = a(i,k-1)/scale
              h = h+ort(i)*ort(i)
   20    continue
         g = -sign(sqrt(h),ort(k))
         h = h-ort(k) *g
         ort(k) = ort(k)-g
c
c        form  (i-(u*transpose(u))/h) *a
c
         do 50 j = k,n
              f = 0.0d+0
              do 30 ii = k,igh
                   i = kpn-ii
                   f = f+ort(i)*a(i,j)
   30         continue
              f = f/h
              do 40 i = k,igh
                   a(i,j) = a(i,j)-f*ort(i)
   40         continue
   50    continue
c
c        form  (i-(u*transpose(u))/h) *b
c
         do 80 j = 1,m
              f = 0.0d+0
              do 60 ii = k,igh
                   i = kpn-ii
                   f = f+ort(i) *b(i,j)
   60         continue
              f = f/h
              do 70 i = k,igh
                   b(i,j) = b(i,j)-f*ort(i)
   70         continue
   80    continue
c
c        form  (i-(u*transpose(u))/h) *a*(i-(u*transpose(u))/h)
c
         do 110 i = 1,igh
              f = 0.0d+0
              do 90 jj = k,igh
                   j = kpn-jj
                   f = f+ort(j)*a(i,j)
   90         continue
              f = f/h
              do 100 j = k,igh
                   a(i,j) = a(i,j)-f*ort(j)
  100         continue
  110    continue
c
c        form  c*(i-(u*transpose(u))/h)
c
         do 140 i = 1,l
              f = 0.0d+0
              do 120 jj = k,igh
                   j = kpn-jj
                   f = f+ort(j)*c(i,j)
  120         continue
              f = f/h
              do 130 j = k,igh
                   c(i,j) = c(i,j)-f*ort(j)
  130         continue
  140    continue
         ort(k) = scale*ort(k)
         a(k,k-1) = scale*g
  150    continue
  160 continue
  170 continue
      return
      end
