      subroutine wspmsp(p,q,r,ar,ai,nela,inda,br,bi,nelb,indb,
c     Copyright INRIA
     $     cr,ci,nelc,indc,
     $     ib,ic,xr,xi,xb,ita,itb,ierr)
c multiply complex sparse matrices by the method of gustafson,acm t.o.m.s.
c  vol 4 (1978) p250.
c*** input
c p           number of rows in a,
c q           number of columns in a
c r           number of columns in b
c a            a one-dimensional array containing the non-zero elements
c                of the first matrix,arranged row-wise, but not
c                necessarily in order within rows.
c nela        number of non-zero elements in a
c inda(i)     1<=i<=p number of non-zero elements in row i of a.
c inda(p+i)   1<=i<nela column index of i'th non-zero element of a.

c b,nelb,indb as a,nela,inda but for second matrix.
c nelc        maximum non zero element for the result

c*** output
c c,nelc,indc  as a,nela,inda but for result matrix.
c c            a one-dimensional array containing the non-zero elements
c                of the product matrix,arranged row-by-row,but not
c                usually in order within rows.
c             ordering by increasing column number may be attained
c               by subsequently calling trsmgu twice,i.e.:
c     call trsmgu(c,mc,ic,ct,mct,ict)
c     call trsmgu(ct,mct,ic,c,mc,ic)
c               where ct,mct,ict are working storage areas of
c               same dimension as c,mc,ic respectively.
c ierr        =1 if space exceeded in c
c             =0 otherwise.
c*** working storage parameters.
c  ib         ib(i) is address in b of first element of row i of b.
c               ib(number of rows +1)=number of elements in b,+1.
c  ic         as above,but for c.
c  x          a one-dimensional array of size ge number of cols of c,
c                to contain elements of current row of c,
c                in full,i.e. non-sparse form.
c  xb         an array of same size as x. xb(j)=i if element in row i,
c                column j of c is non-zero.
c!
      integer p, q, r, nela, nelb, nelc, ierr
      double precision  ar(nela),ai(nela), br(nelb),bi(nelb)
      double precision  cr(*),ci(*), xr(r),xi(r)
c  the following may be changed on ibm/370 type machines by:-
      integer inda(*), indb(*), ib(*), indc(*), ic(*), xb(r)

      integer v, vp, vpppp4

      ndc = nelc
      ndmc = nelc + p
      ib(1) = 1

      do 20 i=1,q
        ib(i+1) = ib(i) + indb(i)
   20 continue
      ierr = 0
      ip = 1
c initialize the non-zero -element indicator for row i of c.
      do 30 v=1,r
        xb(v) = 0
   30 continue
c process the rows of a.
c    inext will point to start of next row,i.e. row i+1
      inext = 1
      do 80 i=1,p
        ic(i) = ip
c  istart points to start of current row.
        istart = inext
        inext = inext + inda(i)
        i1 = istart
        i2 = inext - 1
        if (i1.gt.i2) go to 80
c process row i of a.
        do 60 jp=i1,i2
c j is column-index of current element of a,i.e. row-index of row of b
c  to be processed.
          jpppp4 = jp + p
          j = inda(jpppp4)
          i3 = ib(j)
          i4 = ib(j+1) - 1
          if (i3.gt.i4) go to 60
c process row of b.
          do 50 kp=i3,i4
c k is column index of current element of b.
            kppqp4 = kp + q
            k = indb(kppqp4)
c check if contribution already exixts to c(i,k)
            if (xb(k).eq.i) go to 40
c set column-index and non-zero indicator for new element of c.
            ipppp4 = ip + p
            if (ipppp4.gt.ndmc) then 
               ierr=1
               return
            endif
            indc(ipppp4) = k
            ip = ip + 1
            xb(k) = i
            if(ita.eq.0) then
               xr(k) = ar(jp)*br(kp) 
               xi(k) = ar(jp)*bi(kp) 
            elseif(itb.eq.0) then
               xr(k) = ar(jp)*br(kp) 
               xi(k) = ai(jp)*br(kp)
            else
               xr(k) = ar(jp)*br(kp) - ai(jp)*bi(kp)
               xi(k) = ar(jp)*bi(kp) + ai(jp)*br(kp)
            endif
            go to 50
c add new contribution to existing element of c
 40         continue
            if(ita.eq.0) then
               xr(k) = xr(k) + ar(jp)*br(kp)
               xi(k) = xi(k) + ar(jp)*bi(kp)
            elseif(itb.eq.0) then
               xr(k) = xr(k) + ar(jp)*br(kp)
               xi(k) = xi(k) + ai(jp)*br(kp)
            else
               xr(k) = xr(k) + ar(jp)*br(kp) - ai(jp)*bi(kp)
               xi(k) = xi(k) + ar(jp)*bi(kp) + ai(jp)*br(kp)
         endif
   50     continue
   60   continue
c check for overflow in c.
        if ((ip-1).gt.ndc) then
           ierr=1
           return
        endif
        i5 = ic(i)
        i6 = ip - 1
c extract non-zeros from current row of c (stored in x).
        do 70 vp=i5,i6
          vpppp4 = vp + p
          v = indc(vpppp4)
          cr(vp) = xr(v)
          ci(vp) = xi(v)          
   70   continue
   80 continue
c ic(p+1)= number of non-zeros in c,+1.
      ic(p+1) = ip
c  extract control information in required form for indc.
      do 90 i=1,p
        indc(i) = ic(i+1) - ic(i)
        if(indc(i).gt.1) then
           call isort1(indc(p+ic(i)),indc(i),xb,1)
           call wperm(cr(ic(i)),ci(ic(i)),indc(i),xb)
        endif
   90 continue
      nelc  = ip - 1
      end
