      subroutine wsvdc(xr,xi,ldx,n,p,sr,si,er,ei,ur,ui,ldu,vr,vi,ldv,
     *     workr,worki,job,info)
      integer ldx,n,p,ldu,ldv,job,info
      double precision xr(ldx,*),xi(ldx,*),sr(1),si(1),er(1),ei(1),
     *                 ur(ldu,*),ui(ldu,*),vr(ldv,*),vi(ldv,*),
     *                 workr(1),worki(1)
c!purpose
c
c
c     wsvdc is a subroutine to reduce a double-complex nxp matrix x by
c     unitary transformations u and v to diagonal form.  the
c     diagonal elements s(i) are the singular values of x.  the
c     columns of u are the corresponding left singular vectors,
c     and the columns of v the right singular vectors.
c
c!calling sequence
c
c      subroutine wsvdc(xr,xi,ldx,n,p,sr,si,er,ei,ur,ui,ldu,vr,vi,ldv,
c     on entry
c
c         x         double-complex(ldx,p), where ldx.ge.n.
c                   x contains the matrix whose singular value
c                   decomposition is to be computed.  x is
c                   destroyed by wsvdc.
c
c         n         integer.
c                   n is the number of rows of the matrix x.
c
c         p         integer.
c                   p is the number of columns of the matrix x.
c
c         ldx       integer.
c                   ldx is the leading dimension of the array x.
c
c         ldu       integer.
c                   ldu is the leading dimension of the array u
c                   (see below).
c
c         ldv       integer.
c                   ldv is the leading dimension of the array v
c                   (see below).
c
c         work      double-complex(n).
c                   work is a scratch array.
c
c         job       integer.
c                   job controls the computation of the singular
c                   vectors.  it has the decimal expansion ab
c                   with the following meaning
c
c                        a.eq.0    do not compute the left singular
c                                  vectors.
c                        a.eq.1    return the n left singular vectors
c                                  in u.
c                        a.ge.2    returns the first min(n,p)
c                                  left singular vectors in u.
c                        b.eq.0    do not compute the right singular
c                                  vectors.
c                        b.eq.1    return the right singular vectors
c                                  in v.
c
c     on return
c
c         s         double-complex(mm), where mm=min(n+1,p).
c                   the first min(n,p) entries of s contain the
c                   singular values of x arranged in descending
c                   order of magnitude.
c
c         e         double-complex(p).
c                   e ordinarily contains zeros.  however see the
c                   discussion of info for exceptions.
c
c         u         double-complex(ldu,k), where ldu.ge.n.
c                   if joba.eq.1 then k.eq.n,
c                   if joba.eq.2 then k.eq.min(n,p).
c                   u contains the matrix of right singular vectors.
c                   u is not referenced if joba.eq.0.  if n.le.p
c                   or if joba.gt.2, then u may be identified with x
c                   in the subroutine call.
c
c         v         double-complex(ldv,p), where ldv.ge.p.
c                   v contains the matrix of right singular vectors.
c                   v is not referenced if jobb.eq.0.  if p.le.n,
c                   then v may be identified whth x in the
c                   subroutine call.
c
c         info      integer.
c                   the singular values (and their corresponding
c                   singular vectors) s(info+1),s(info+2),...,s(m)
c                   are correct (here m=min(n,p)).  thus if
c                   info.eq.0, all the singular values and their
c                   vectors are correct.  in any event, the matrix
c                   b = ctrans(u)*x*v is the bidiagonal matrix
c                   with the elements of s on its diagonal and the
c                   elements of e on its super-diagonal (ctrans(u)
c                   is the conjugate-transpose of u).  thus the
c                   singular values of x and b are the same.
c
c!originator
c     linpack. this version dated 07/03/79 .
c     g.w. stewart, university of maryland, argonne national lab.
c
c!auxiliary routines
c
c     blas waxpy,pythag,wdotcr,wdotci,wscal,wswap,wnrm2,drotg
c     fortran abs,dimag,max
c     fortran max,min,mod,sqrt
c
c!
c     Copyright INRIA
c     internal variables
c
      integer i,iter,j,jobu,k,kase,kk,l,ll,lls,lm1,lp1,ls,lu,m,maxit,
     *        mm,mm1,mp1,nct,nctp1,ncu,nrt,nrtp1
      double precision pythag,wdotcr,wdotci,tr,ti,rr,ri
      double precision b,c,cs,el,emm1,f,g,wnrm2,scale,shift,sl,sm,sn,
     *                 smm1,t1,test,ztest
      logical wantu,wantv
c
      double precision zdumr,zdumi
      double precision cabs1
      cabs1(zdumr,zdumi) = abs(zdumr) + abs(zdumi)
c
c     set the maximum number of iterations.
c     MODIFIED ACCORDING TO EISPACK HQR2
c
      maxit = 30*min(n,p)
c
c
c     determine what is to be computed.
c
      wantu = .false.
      wantv = .false.
      jobu = mod(job,100)/10
      ncu = n
      if (jobu .gt. 1) ncu = min(n,p)
      if (jobu .ne. 0) wantu = .true.
      if (mod(job,10) .ne. 0) wantv = .true.
c
c     reduce x to bidiagonal form, storing the diagonal elements
c     in s and the super-diagonal elements in e.
c
      info = 0
      nct = min(n-1,p)
      nrt = max(0,min(p-2,n))
      lu = max(nct,nrt)
      if (lu .lt. 1) go to 190
      do 180 l = 1, lu
         lp1 = l + 1
         if (l .gt. nct) go to 30
c
c           compute the transformation for the l-th column and
c           place the l-th diagonal in s(l).
c
            sr(l) = wnrm2(n-l+1,xr(l,l),xi(l,l),1)
            si(l) = 0.0d+0
            if (cabs1(sr(l),si(l)) .eq. 0.0d+0) go to 20
               if (cabs1(xr(l,l),xi(l,l)) .eq. 0.0d+0) go to 10
                  call wsign(sr(l),si(l),xr(l,l),xi(l,l),sr(l),si(l))
   10          continue
               call wdiv(1.0d+0,0.0d+0,sr(l),si(l),tr,ti)
               call wscal(n-l+1,tr,ti,xr(l,l),xi(l,l),1)
               xr(l,l) = 1.0d+0 + xr(l,l)
   20       continue
            sr(l) = -sr(l)
            si(l) = -si(l)
   30    continue
         if (p .lt. lp1) go to 60
         do 50 j = lp1, p
            if (l .gt. nct) go to 40
            if (cabs1(sr(l),si(l)) .eq. 0.0d+0) go to 40
c
c              apply the transformation.
c
               tr = -wdotcr(n-l+1,xr(l,l),xi(l,l),1,xr(l,j),xi(l,j),1)
               ti = -wdotci(n-l+1,xr(l,l),xi(l,l),1,xr(l,j),xi(l,j),1)
               call wdiv(tr,ti,xr(l,l),xi(l,l),tr,ti)
               call waxpy(n-l+1,tr,ti,xr(l,l),xi(l,l),1,xr(l,j),
     *                    xi(l,j),1)
   40       continue
c
c           place the l-th row of x into  e for the
c           subsequent calculation of the row transformation.
c
            er(j) = xr(l,j)
            ei(j) = -xi(l,j)
   50    continue
   60    continue
         if (.not.wantu .or. l .gt. nct) go to 80
c
c           place the transformation in u for subsequent back
c           multiplication.
c
            do 70 i = l, n
               ur(i,l) = xr(i,l)
               ui(i,l) = xi(i,l)
   70       continue
   80    continue
         if (l .gt. nrt) go to 170
c
c           compute the l-th row transformation and place the
c           l-th super-diagonal in e(l).
c
            er(l) = wnrm2(p-l,er(lp1),ei(lp1),1)
            ei(l) = 0.0d+0
            if (cabs1(er(l),ei(l)) .eq. 0.0d+0) go to 100
               if (cabs1(er(lp1),ei(lp1)) .eq. 0.0d+0) go to 90
                  call wsign(er(l),ei(l),er(lp1),ei(lp1),er(l),ei(l))
   90          continue
               call wdiv(1.0d+0,0.0d+0,er(l),ei(l),tr,ti)
               call wscal(p-l,tr,ti,er(lp1),ei(lp1),1)
               er(lp1) = 1.0d+0 + er(lp1)
  100       continue
            er(l) = -er(l)
            ei(l) = +ei(l)
            if (lp1 .gt. n .or. cabs1(er(l),ei(l)) .eq. 0.0d+0)
     *         go to 140
c
c              apply the transformation.
c
               do 110 i = lp1, n
                  workr(i) = 0.0d+0
                  worki(i) = 0.0d+0
  110          continue
               do 120 j = lp1, p
                  call waxpy(n-l,er(j),ei(j),xr(lp1,j),xi(lp1,j),1,
     *                       workr(lp1),worki(lp1),1)
  120          continue
               do 130 j = lp1, p
                  call wdiv(-er(j),-ei(j),er(lp1),ei(lp1),tr,ti)
                  call waxpy(n-l,tr,-ti,workr(lp1),worki(lp1),1,
     *                       xr(lp1,j),xi(lp1,j),1)
  130          continue
  140       continue
            if (.not.wantv) go to 160
c
c              place the transformation in v for subsequent
c              back multiplication.
c
               do 150 i = lp1, p
                  vr(i,l) = er(i)
                  vi(i,l) = ei(i)
  150          continue
  160       continue
  170    continue
  180 continue
  190 continue
c
c     set up the final bidiagonal matrix or order m.
c
      m = min(p,n+1)
      nctp1 = nct + 1
      nrtp1 = nrt + 1
      if (nct .ge. p) go to 200
         sr(nctp1) = xr(nctp1,nctp1)
         si(nctp1) = xi(nctp1,nctp1)
  200 continue
      if (n .ge. m) go to 210
         sr(m) = 0.0d+0
         si(m) = 0.0d+0
  210 continue
      if (nrtp1 .ge. m) go to 220
         er(nrtp1) = xr(nrtp1,m)
         ei(nrtp1) = xi(nrtp1,m)
  220 continue
      er(m) = 0.0d+0
      ei(m) = 0.0d+0
c
c     if required, generate u.
c
      if (.not.wantu) go to 350
         if (ncu .lt. nctp1) go to 250
         do 240 j = nctp1, ncu
            do 230 i = 1, n
               ur(i,j) = 0.0d+0
               ui(i,j) = 0.0d+0
  230       continue
            ur(j,j) = 1.0d+0
            ui(j,j) = 0.0d+0
  240    continue
  250    continue
         if (nct .lt. 1) go to 340
         do 330 ll = 1, nct
            l = nct - ll + 1
            if (cabs1(sr(l),si(l)) .eq. 0.0d+0) go to 300
               lp1 = l + 1
               if (ncu .lt. lp1) go to 270
               do 260 j = lp1, ncu
                  tr = -wdotcr(n-l+1,ur(l,l),ui(l,l),1,ur(l,j),
     *                         ui(l,j),1)
                  ti = -wdotci(n-l+1,ur(l,l),ui(l,l),1,ur(l,j),
     *                         ui(l,j),1)
                  call wdiv(tr,ti,ur(l,l),ui(l,l),tr,ti)
                  call waxpy(n-l+1,tr,ti,ur(l,l),ui(l,l),1,ur(l,j),
     *                       ui(l,j),1)
  260          continue
  270          continue
               call wrscal(n-l+1,-1.0d+0,ur(l,l),ui(l,l),1)
               ur(l,l) = 1.0d+0 + ur(l,l)
               lm1 = l - 1
               if (lm1 .lt. 1) go to 290
               do 280 i = 1, lm1
                  ur(i,l) = 0.0d+0
                  ui(i,l) = 0.0d+0
  280          continue
  290          continue
            go to 320
  300       continue
               do 310 i = 1, n
                  ur(i,l) = 0.0d+0
                  ui(i,l) = 0.0d+0
  310          continue
               ur(l,l) = 1.0d+0
               ui(l,l) = 0.0d+0
  320       continue
  330    continue
  340    continue
  350 continue
c
c     if it is required, generate v.
c
      if (.not.wantv) go to 400
         do 390 ll = 1, p
            l = p - ll + 1
            lp1 = l + 1
            if (l .gt. nrt) go to 370
            if (cabs1(er(l),ei(l)) .eq. 0.0d+0) go to 370
               do 360 j = lp1, p
                  tr = -wdotcr(p-l,vr(lp1,l),vi(lp1,l),1,vr(lp1,j),
     *                         vi(lp1,j),1)
                  ti = -wdotci(p-l,vr(lp1,l),vi(lp1,l),1,vr(lp1,j),
     *                         vi(lp1,j),1)
                  call wdiv(tr,ti,vr(lp1,l),vi(lp1,l),tr,ti)
                  call waxpy(p-l,tr,ti,vr(lp1,l),vi(lp1,l),1,vr(lp1,j),
     *                       vi(lp1,j),1)
  360          continue
  370       continue
            do 380 i = 1, p
               vr(i,l) = 0.0d+0
               vi(i,l) = 0.0d+0
  380       continue
            vr(l,l) = 1.0d+0
            vi(l,l) = 0.0d+0
  390    continue
  400 continue
c
c     transform s and e so that they are real.
c
      do 420 i = 1, m
            tr = pythag(sr(i),si(i))
            if (tr .eq. 0.0d+0) go to 405
            rr = sr(i)/tr
            ri = si(i)/tr
            sr(i) = tr
            si(i) = 0.0d+0
            if (i .lt. m) call wdiv(er(i),ei(i),rr,ri,er(i),ei(i))
            if (wantu) call wscal(n,rr,ri,ur(1,i),ui(1,i),1)
  405    continue
c     ...exit
         if (i .eq. m) go to 430
            tr = pythag(er(i),ei(i))
            if (tr .eq. 0.0d+0) go to 410
            call wdiv(tr,0.0d+0,er(i),ei(i),rr,ri)
            er(i) = tr
            ei(i) = 0.0d+0
            call wmul(sr(i+1),si(i+1),rr,ri,sr(i+1),si(i+1))
            if (wantv) call wscal(p,rr,ri,vr(1,i+1),vi(1,i+1),1)
  410    continue
  420 continue
  430 continue
c
c     main iteration loop for the singular values.
c
      mm = m
      iter = 0
  440 continue
c
c        quit if all the singular values have been found.
c
c     ...exit
         if (m .eq. 0) go to 700
c
c        if too many iterations have been performed, set
c        flag and return.
c
         if (iter .lt. maxit) go to 450
            info = m
c     ......exit
            go to 700
  450    continue
c
c        this section of the program inspects for
c        negligible elements in the s and e arrays.  on
c        completion the variable kase is set as follows.
c
c           kase = 1     if sr(m) and er(l-1) are negligible and l.lt.m
c           kase = 2     if sr(l) is negligible and l.lt.m
c           kase = 3     if er(l-1) is negligible, l.lt.m, and
c                        sr(l), ..., sr(m) are not negligible (qr step).
c           kase = 4     if er(m-1) is negligible (convergence).
c
         do 470 ll = 1, m
            l = m - ll
c        ...exit
            if (l .eq. 0) go to 480
            test = pythag(sr(l),si(l)) + pythag(sr(l+1),si(l+1))
            ztest = test + pythag(er(l),ei(l))
            if (ztest .ne. test) go to 460
               er(l) = 0.0d+0
               ei(l) = 0.0d+0
c        ......exit
               go to 480
  460       continue
  470    continue
  480    continue
         if (l .ne. m - 1) go to 490
            kase = 4
         go to 560
  490    continue
            lp1 = l + 1
            mp1 = m + 1
            do 510 lls = lp1, mp1
               ls = m - lls + lp1
c           ...exit
               if (ls .eq. l) go to 520
               test = 0.0d+0
               if (ls .ne. m) test=test + pythag(er(ls),ei(ls))
               if (ls .ne. l + 1) test=test + pythag(er(ls-1),ei(ls-1))
               ztest = test + pythag(sr(ls),si(ls))
               if (ztest .ne. test) go to 500
                  sr(ls) = 0.0d+0
                  si(ls) = 0.0d+0
c           ......exit
                  go to 520
  500          continue
  510       continue
  520       continue
            if (ls .ne. l) go to 530
               kase = 3
            go to 550
  530       continue
            if (ls .ne. m) go to 540
               kase = 1
            go to 550
  540       continue
               kase = 2
               l = ls
  550       continue
  560    continue
         l = l + 1
c
c        perform the task indicated by kase.
c
         go to (570, 600, 620, 650), kase
c
c        deflate negligible s(m).
c
  570    continue
            mm1 = m - 1
            f = er(m-1)
            er(m-1) = 0.0d+0
            ei(m-1) = 0.0d+0
            do 590 kk = l, mm1
               k = mm1 - kk + l
               t1 = sr(k)
               call drotg(t1,f,cs,sn)
               sr(k) = t1
               si(k) = 0.0d0
               if (k .eq. l) go to 580
                  f = -sn*er(k-1)
                  er(k-1) = cs*er(k-1)
                  ei(k-1) = cs*ei(k-1)
  580          continue
               if (wantv) call drot(p,vr(1,k),1,vr(1,m),1,cs,sn)
               if (wantv) call drot(p,vi(1,k),1,vi(1,m),1,cs,sn)
  590       continue
         go to 690
c
c        split at negligible s(l).
c
  600    continue
            f = er(l-1)
            er(l-1) = 0.0d+0
            ei(l-1) = 0.0d+0
            do 610 k = l, m
               t1 = sr(k)
               call drotg(t1,f,cs,sn)
               sr(k) = t1
               si(k) = 0.0d0
               f = -sn*er(k)
               er(k) = cs*er(k)
               ei(k) = cs*ei(k)
               if (wantu) call drot(n,ur(1,k),1,ur(1,l-1),1,cs,sn)
               if (wantu) call drot(n,ui(1,k),1,ui(1,l-1),1,cs,sn)
  610       continue
         go to 690
c
c        perform one qr step.
c
  620    continue
c
c           calculate the shift.
c
            scale = max(pythag(sr(m),si(m)),pythag(sr(m-1),si(m-1)),
     *        pythag(er(m-1),ei(m-1)),
     *        pythag(sr(l),si(l)),pythag(er(l),ei(l)))
            sm = sr(m)/scale
            smm1 = sr(m-1)/scale
            emm1 = er(m-1)/scale
            sl = sr(l)/scale
            el = er(l)/scale
            b = ((smm1 + sm)*(smm1 - sm) + emm1**2)/2.0d+0
            c = (sm*emm1)**2
            shift = 0.0d+0
            if (b .eq. 0.0d+0 .and. c .eq. 0.0d+0) go to 630
               shift = sqrt(b**2+c)
               if (b .lt. 0.0d+0) shift = -shift
               shift = c/(b + shift)
  630       continue
c            f = (sl + sm)*(sl - sm) - shift
        f=(sl+sm)*(sl-sm)+shift
            g = sl*el
c
c           chase zeros.
c
            mm1 = m - 1
            do 640 k = l, mm1
               call drotg(f,g,cs,sn)
               if (k .ne. l) then
                  er(k-1) = f
                  ei(k-1) = 0.0d0
               endif
               f = cs*sr(k) + sn*er(k)
               er(k) = cs*er(k) - sn*sr(k)
               ei(k) = cs*ei(k) - sn*si(k)
               g = sn*sr(k+1)
               sr(k+1) = cs*sr(k+1)
               si(k+1) = cs*si(k+1)
               if (wantv) call drot(p,vr(1,k),1,vr(1,k+1),1,cs,sn)
               if (wantv) call drot(p,vi(1,k),1,vi(1,k+1),1,cs,sn)
               call drotg(f,g,cs,sn)
               sr(k) = f
               si(k) = 0.0d0
               f = cs*er(k) + sn*sr(k+1)
               sr(k+1) = -sn*er(k) + cs*sr(k+1)
               si(k+1) = -sn*ei(k) + cs*si(k+1)
               g = sn*er(k+1)
               er(k+1) = cs*er(k+1)
               ei(k+1) = cs*ei(k+1)
               if (wantu .and. k .lt. n)
     *            call drot(n,ur(1,k),1,ur(1,k+1),1,cs,sn)
               if (wantu .and. k .lt. n)
     *            call drot(n,ui(1,k),1,ui(1,k+1),1,cs,sn)
  640       continue
            er(m-1) = f
            ei(m-1) = 0.0d0
            iter = iter + 1
         go to 690
c
c        convergence
c
  650    continue
c
c           make the singular value  positive
c
            if (sr(l) .ge. 0.0d+0) go to 660
               sr(l) = -sr(l)
               si(l) = -si(l)
             if (wantv) call wrscal(p,-1.0d+0,vr(1,l),vi(1,l),1)
  660       continue
c
c           order the singular value.
c
  670       if (l .eq. mm) go to 680
c           ...exit
               if (sr(l) .ge. sr(l+1)) go to 680
               tr = sr(l)
               sr(l) = sr(l+1)
               sr(l+1) = tr
               tr = si(l)
               si(l) = si(l+1)
               si(l+1) = tr
               if (wantv .and. l .lt. p)
     *            call wswap(p,vr(1,l),vi(1,l),1,vr(1,l+1),vi(1,l+1),1)
               if (wantu .and. l .lt. n)
     *            call wswap(n,ur(1,l),ui(1,l),1,ur(1,l+1),ui(1,l+1),1)
               l = l + 1
            go to 670
  680       continue
            iter = 0
            m = m - 1
  690    continue
      go to 440
  700 continue
      return
      end
