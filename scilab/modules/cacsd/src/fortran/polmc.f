      subroutine polmc(nm,ng,n,m,a,b,g,wr,wi,z,inc,invr,ierr,jpvt,
     x                  rm1,rm2,rv1,rv2,rv3,rv4)
c
      double precision a(nm,n),b(nm,m),g(ng,n),wr(n),wi(n),z(nm,n),
     x     rm1(m,m),rm2(m,*),rv1(n),rv2(n),rv3(m),rv4(m)
      double precision p,q,r,s,t,zz
      integer invr(n),jpvt(m)
      logical complx
c!purpose
c     this subroutine determines the state feedback matrix  g  of the
c     linear time-invariant multi-input system
c
c        dx / dt = a * x + b * u,
c
c     where  a  is a  nxn  and  b  is a  nxm  matrix, such that the
c     closed-loop system
c
c        dx / dt = (a - b * g) * x
c
c     has desired poles. the system must be preliminary reduced into
c     orthogonal canonical form using the subroutine  trmcf.
c!calling sequence
c
c     subroutine polmc(nm,ng,n,m,a,b,g,wr,wi,z,inc,invr,ierr,jpvt,
c    x                  rm1,rm2,rv1,rv2,rv3,rv4)
c
c     on input-
c
c        nm    is an integer variable set equal to the row dimension
c              of the two-dimensional arrays  a,  b  and  z  as
c              specified in the dimension statements for  a,  b  and  z
c              in the calling program,
c
c        ng    is an integer variable set equal to the row dimension
c              of the two-dimensional array  g  as specified in the
c              dimension statement for  g  in the calling program,
c
c        n     is an integer variable set equal to the order of the
c              matrices  a  and  z.  n  must be not greater than  nm,
c
c        m     is an integer variable set equal to the number of the
c              columns of the matrix  b.  m  must be not greater than
c              ng,
c
c        a     is a working precision real two-dimensional variable with
c              row dimension  nm  and column dimension at least  n
c              containing the block-hessenberg canonical form of the
c              matrix  a.  the elements below the subdiagonal blocks
c              must be equal to zero,
c
c        b     is a working precision real two-dimensional variable with
c              row dimension  nm  and column dimension at least  m
c              containing the canonical form of the matrix  b. the
c              elements below the  invr(1)-th row must be equal to zero,
c
c        wr,wi are working precision real one-dimensional variables
c              of dimension at least  n  containing the real and
c              imaginery parts, respectively, of the desired poles,
c              the poles can be unordered except that the complex
c              conjugate pairs of poles must appfar consecutively.
c              note that on output the imaginery parts of the poles
c              may be modified,
c
c        z     is a working precision real two-dimensonal variale with
c              row dimension  nm  and column dimension at least  n
c              containing the orthogonal transformation matrix produced
c              in  trmcf  which reduces the system into canonical form,
c
c        inc   is an integer variable set equal to the controllability
c              index of the system,
c
c        invr  is an integer one-dimensional variable of dimension at
c              least  inc  containing the dimensons of the
c              controllable subsystems in the canonical form.
c
c     on output-
c
c        a     contains the upper quast-triangular form of the closed-
c              loop system matrix  a - b * g,  that is triangular except
c              of possible  2x2  blocks on the diagonal,
c
c        b     contains the transformed matrix  b,
c
c        g     is a working precision real two-dimensional variable with
c              row dimension  ng  and column dimension at least  n
c              containing the state feedback matrix  g  of the original
c              system,
c
c        z     contains the orthogonal matrix which reduces the closed-
c              loop system matrix  a - b * g  to the upper quasi-
c              triangular form,
c
c        ierr  is an integer variable set equal to
c              zero  for normal return,
c              1     if the system is not completely controllable,
c
c        jpvt  is an integer temporary one-dimensonal array of
c              dimension at least  m  used in the solution of linear
c              equations,
c
c        rm1   is a working precision real temporary two-dimensonal
c              array of dimension at least  mxm  used in the solution
c              of linear equations,
c
c        rm2   is a working precision real temporary two-dimensional
c              array od dimension at least  mxmax(2,m)  used in the
c              solution of linear equations,
c
c        rv1,  are working precision real temporary one-dimensional
c          rv2 arrays of dimension at least  n  used to hold the
c              real and imaginery parts, respectively, of the
c              eigenvectors during the reduction,
c
c        rv3,  are working precision real temporary one-dimensional
c          rv4 arrays of dimension at least  m  used in the solution
c              of linear equations.
c
c!auxiliary routines
c
c        sqrsm
c        fortran  abs,min,sqrt
c!originator
c     p.hr.petkov, higher institute of mechanical and electrical
c     engineering, sofia, bulgaria.
c     modified by serge Steer INRIA
c     Copyright SLICOT
c!
c
      ierr = 0
      m1 = invr(1)
      l = 0
   10    l = l + 1
         mr = invr(inc)
         if (inc .eq. 1) go to 350
         lp1 = l + m1
         inc1 = inc - 1
         mr1 = invr(inc1)
         nr = n - mr + 1
         nr1 = nr - mr1
         complx = wi(l) .ne. 0.0d+0
        do 15 i = nr, n
           rv1(i) = 0.0d+0
           if (complx) rv2(i) = 0.0d+0
   15   continue
c
        rv1(nr) = 1.0d+0
        if (.not. complx) go to 20
        if (mr .eq. 1) rv2(nr) = 1.0d+0
        if (mr .gt. 1) rv2(nr+1) = 1.0d+0
        t = wi(l)
        wi(l) = 1.0d+0
        wi(l+1) = t * wi(l+1)
c
c       compute and transform eigenvector
c
   20   do 200 ip = 1, inc
           if (ip .eq. inc .and. inc .eq. 2) go to 200
           if (ip .eq. inc) go to 120
c
           do 40 ii = 1, mr
              i = nr + ii - 1
c
              do 30 jj = 1, mr1
                 j = nr1 + jj - 1
                 rm1(ii,jj) = a(i,j)
   30         continue
c
   40      continue
c
           if (ip .eq. 1) go to 70
c
c          scaling
c
           s = 0.0d+0
           mp1 = mr + 1
           np1 = nr + mp1
c
           do 50 ii = 1, mp1
              i = nr + ii - 1
              s = s + abs(rv1(i))
              if (complx) s = s + abs(rv2(i))
   50      continue
c
           do 60 ii = 1, mp1
              i = nr + ii - 1
              rv1(i) = rv1(i) / s
              if (complx) rv2(i) = rv2(i) / s
   60      continue
c
           if (complx .and. np1 .le. n) rv2(np1) = rv2(np1) / s
   70      if (ip .eq. 1) mp1 = 1
           np1 = nr + mp1
c
           do 100 ii = 1, mr
              i = nr + ii - 1
              s = wr(l) * rv1(i)
c
              do 80 jj = 1, mp1
                 j = nr + jj - 1
                 s = s - a(i,j) * rv1(j)
   80         continue
c
              rm2(ii,1) = s
              if (.not. complx) go to 100
              rm2(ii,1) = rm2(ii,1) + wi(l+1) * rv2(i)
              s = wr(l+1) * rv2(i) + wi(l) * rv1(i)
c
              do 90 jj = 1, mp1
c la ligne suivante a ete rajoutee par mes soins
                 j = nr + jj - 1
                 s = s - a(i,j) * rv2(j)
   90         continue
c
              if (np1 .le. n) s = s - a(i,np1) * rv2(np1)
              rm2(ii,2) = s
  100      continue
c
c          solving linear equations for the eigenvector elements
c
           nc = 1
           if (complx) nc = 2
           call dqrsm(rm1,m,mr,mr1,rm2,m,nc,rm2,m,ir,jpvt,
     x                             rv3,rv4)
           if (ir .lt. mr) go to 600
c
           do 110 ii = 1, mr1
              i = nr1 + ii - 1
              rv1(i) = rm2(ii,1)
              if (complx) rv2(i) = rm2(ii,2)
  110      continue
c
           if (ip .eq. 1 .and. inc .gt. 2) go to 195
  120      nj = nr
           if (ip .lt. inc) nj = nr1
           ni = nr + mr - 1
           inc2 = inc - ip + 2
           if (ip .gt. 1) ni = ni + invr(inc2)
           if (ip .gt. 2) ni = ni + 1
           if (complx .and. ip .gt. 2) ni = min(ni+1,n)
           kmr = mr1
           if (ip .gt. 1) kmr = mr
c
           do 190 kk = 1, kmr
              ll = 1
              k = nr + mr - kk
              if (ip .eq. 1) k = nr - kk
  130         p = rv1(k)
              if (ll .eq. 2) p = rv2(k)
              q = rv1(k+1)
              if (ll .eq. 2) q = rv2(k+1)
              s = abs(p) + abs(q)
              p = p / s
              q = q / s
              r = sqrt(p*p+q*q)
              t = s * r
              rv1(k) = t
              if (ll .eq. 2) rv2(k) = t
              rv1(k+1) = 0.0d+0
              if (ll .eq. 2) rv2(k+1) = 0.0d+0
              p = p / r
              q = q / r
c
c             transform  a
c
              do 140 j = nj, n
                 zz = a(k,j)
                 a(k,j) = p * zz + q * a(k+1,j)
                 a(k+1,j) = p * a(k+1,j) - q * zz
  140         continue
c
              do 150 i = 1, ni
                 zz = a(i,k)
                 a(i,k) = p * zz + q * a(i,k+1)
                 a(i,k+1) = p * a(i,k+1) - q * zz
  150         continue
c
              if (k .eq. lp1 .and. ll .eq. 1 .or. k .gt. lp1) go to 170
c
c        transform b
c
             do 160 j = 1, m
                zz = b(k,j)
                b(k,j) = p * zz + q * b(k+1,j)
                b(k+1,j) = p * b(k+1,j) - q * zz
  160         continue
c
c             accumulate transformations
c
  170          do 180 i = 1, n
                  zz = z(i,k)
                  z(i,k) = p * zz + q * z(i,k+1)
                  z(i,k+1) = p * z(i,k+1) - q * zz
  180          continue
c
               if (.not. complx .or. ll .eq. 2) go to 190
               zz = rv2(k)
               rv2(k) = p * zz + q * rv2(k+1)
               rv2(k+1) = p * rv2(k+1) - q * zz
               if (k + 2 .gt. n) go to 190
               k = k + 1
               ll = 2
               go to 130
  190       continue
c
            if (ip .eq. inc) go to 200
  195       mr = mr1
            nr = nr1
            if (ip .eq. inc1) go to 200
            inc2 = inc - ip - 1
            mr1 = invr(inc2)
            nr1 = nr1 - mr1
  200    continue
c
         if (complx) go to 250
c
c        find one column of  g
c
         do 220 ii = 1, m1
            i = l + ii
c
            do 210 j = 1, m
  210       rm1(ii,j) = b(i,j)
c
            rm2(ii,1) = a(i,l)
  220    continue
c
         call dqrsm(rm1,m,m1,m,rm2,m,1,g(1,l),ng,ir,jpvt,rv3,rv4)
         if (ir .lt. m1) go to 600
c
         do 240 i = 1, lp1
c
            do 230 j = 1, m
  230       a(i,l) = a(i,l) - b(i,j) * g(j,l)
c
  240    continue
c
         go to 330
c
c        find two columns of  g
c
  250    l = l + 1
         if (lp1 .lt. n) lp1 = lp1 + 1
c
         do 270 ii = 1, m1
              i = l + ii
              if (l + m1 .gt. n) i = i - 1
c
c la ligne suivante a ete rajoutee par mes soins
              do 260 j = 1 , m
cxxx        if(abs(b(i,j)).le.abs(b(l,j))) i=i-1
  260         rm1(ii,j) = b(i,j)
c
              p = a(i,l-1)
              if (i .eq. l) p = p - (rv2(i) / rv1(i-1)) * wi(i)
              rm2(ii,1) = p
              q = a(i,l)
              if (i .eq. l) q = q - wr(i) + (rv2(i-1) / rv1(i-1)) *wi(i)
              rm2(ii,2) = q
  270      continue
c
           call dqrsm(rm1,m,m1,m,rm2,m,2,rm2,m,ir,jpvt,rv3,rv4)
           if (ir .lt. m1) go to 600
c
           do 290 i = 1, m
c
              do 280 jj = 1, 2
                 j = l + jj - 2
                 g(i,j) = rm2(i,jj)
  280         continue
c
  290      continue
c
           do 320 i = 1, lp1
c
              do 310 jj = 1, 2
                 j = l + jj - 2
c
                 do 300 k = 1, m
  300            a(i,j) = a(i,j) - b(i,k)*g(k,j)
c
  310         continue
c
  320      continue
c
           if (l .eq. n) go to 500
  330      invr(inc) = invr(inc) - 1
           if (invr(inc) .eq. 0) inc = inc - 1
           if (complx) invr(inc) = invr(inc) - 1
           if (invr(inc) .eq. 0) inc = inc - 1
        go to 10
c
c       find the rest columns of  g
c
  350   do 370 ii = 1, mr
           i = l + ii - 1
c
           do 360 j = 1, m
  360   rm1(ii,j) = b(i,j)
c
  370 continue
c
      do 400 ii = 1, mr
         i = l + ii - 1
c
         do 380 jj = 1, mr
            j = l + jj - 1
            if (ii .lt. jj) rm2(ii,jj) = 0.0d+0
            if (ii .gt. jj) rm2(ii,jj) = a(i,j)
  380    continue
c
  400 continue
c
      ii = 0
  410    ii = ii + 1
         i = l + ii - 1
         if (wi(i) .ne. 0.0d+0) go to 420
         rm2(ii,ii) = a(i,i) - wr(i)
         if (ii .eq. mr) go to 430
c la ligne suivante a ete rajoutee par mes soins
      goto 410
  420    rm2(ii,ii) = a(i,i) - wr(i)
         rm2(ii,ii+1) = a(i,i+1) - wi(i)
         rm2(ii+1,ii) = a(i+1,i) - wi(i+1)
         rm2(ii+1,ii+1) = a(i+1,i+1) - wr(i+1)
         ii = ii + 1
      if (ii .lt. mr) go to 410
  430 call dqrsm(rm1,m,mr,m,rm2,m,m,rm2,m,ir,jpvt,rv3,rv4)
      if (ir .lt. mr) go to 600
c
      do 450 i = 1, m
c
         do 440 jj = 1, mr
            j = l + jj - 1
            g(i,j) = rm2(i,jj)
  440    continue
c
  450 continue
c
      do 480 i = 1, n
c
         do 470 j = l, n
c
            do 460 k = 1, m
  460       a(i,j) = a(i,j) - b(i,k) * g(k,j)
c
  470    continue
c
  480 continue
c
c     transform  g
c
  500 do 540 i = 1, m
c
         do 520 j = 1, n
            s = 0.0d+0
c
            do 510 k = 1, n
  510       s = s + g(i,k) * z(j,k)
c
            rv1(j) = s
  520    continue
c
         do 530 j = 1, n
  530    g(i,j) = rv1(j)
c
  540 continue
c
      go to 610
c
c     set error -- the system is not completely controllable
c
  600 ierr = 1
  610 return
c
c     last card of subroutine polmc
c
      end
