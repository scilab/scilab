c      algorithm 750, collected algorithms from acm.
c      this work published in transactions on mathematical software,
c      vol. 21, no. 4, december, 1995, p.  410--415.
c *************************************************************************
      subroutine visitor(n,a,nstac,iroad,x,fstar)
      integer a(n,n),x(nstac)
c      real alpha,avson,spars
c      integer active,err,i,inf,lb0,lbc,lopt,maxnd,n,nass,nexp,nprobq,
c     +        ordx,zeur,zstar
      integer fstar(n),iroad(n),active,err,ordx,zeur,zstar
      do 11,i=1,n
         iroad(i)=0
 11   continue
      ordx = nstac
      inf = 99999999
      alpha = -1.
      maxnd = -1
      zeur = -1
      do 1 i=1,nstac
         x(i)=0.
 1    continue
      do 2 i=1,n
         do 2 j=1,n
            k=i+(j-1)*n
            x(k)=a(i,j)
 2    continue
c
      call cdt(n,ordx,x,maxnd,inf,alpha,zeur,zstar,fstar,lb0,lbc,nexp,
     +         nprobq,nass,active,lopt,spars,avson,err)
c
      if (err.eq.0) go to 10
c wwfmt='('' solution not optimal '')'
   10 continue
      iroad(1)=1
      do 20 i=2,n
      iroad(i)=fstar(iroad(i-1))
 20   continue
      return
      end
c *************************************************************************
      subroutine cdt(n,ordx,x,maxnd,inf,alpha,zeur,zstar,fstar,lb0,lbc,
     +               nexp,nprobq,nass,active,lopt,spars,avson,err)
c i/o parameter --------------------------------------------------------
c input parameters
c   alpha   =  if .gt. 0 define the artificial upper bound ub=alpha*lb0
c              if .le. 0 no effect
c   n       =  number of nodes
c   inf     =  very large number
c   maxnd   =  if greater or equal to zero it is the maximum number of
c              node to be explored on the branch decision-tree
c   ordx    =  dimension of vector x
c   zeur    =  if greater then zero it must define a valid upper bound
c   the cost matrix is stored (column by column) in the first n**2 elem.
c   of the vector x. the cost matrix it is not available in output.
c
c output value
c
c   active  = number of active problems in queue when the program stops
c             (not yet and not currently examined)
c   avson   = average number of son nodes for the explored problems
c   err     =  .gt. zero if an error occurred
c           =  -1 if no solution with value less than the artificial
c              upper bound was found.
c              in this case zstar is the artificial upper bound value
c   fstar(i)=  successor of node i in the current best solution
c   lb0     =  value of the optimal assignment at the root node
c   lbc     =  value of the current best lb, i.e. value of the lower
c              bound of the last problem extracted from the queue;
c              lbc is a valid lower bound for the instance
c   nass    =  number of assignment solved
c   nexp    =  number of explored problems
c   nprobq  =  number of problems stored in the queue
c   spars   =  sparsity of the reduced matrix
c   zeur    =  minimum value between the value of zeur in input and the
c              value of the approximate solution computed using the
c              patching heuristic
c   zstar   =  value of the current best solution
c
c internal storage of  vectors and matrices  in vector x
c
c   x(mm1)  =  the queue v(ordv) (dinamically allocated)
c   x(mm2)  =  first element of matrices mv,mf (dinamically allocated)
c   x(mm3)  =  vector ic(*)
c   x(mm4)  =  vectors c(*) (cra(*) in ctcs,
c   x(mm5)  =                ica(*) in ctcs)
c   x(mm6)  =  iva(n)
c   x(mm7)  =  ivb(n)
c   x(mm8)  =  cr(n+1)
c   x(mm9)  =  primal solution of the map associated with the current
c              branched node f(n)
c   x(mm10) =  dual var. of the columns in the solution of ap, dualv(n)
c   x(mm11) =  primal solution of the map at the current node fc(n)
c   x(mm12) =  current dual variables of map, vd(n)
c   x(mm13) =  the heuristic solution of atsp
c   x(mm14)..x(mm26) = temporary vectors
c
c   fmvf = pointer to the first free element to store columns of
c          matrices mv,mf
c   x(fmvf) = pointer to the next free element for matrices mv,mf
c
c local variables
c     .. scalar arguments ..
c      real alpha,avson,spars
c      integer active,err,inf,lb0,lbc,lopt,maxnd,n,nass,nexp,nprobq,ordx,
c     +        zeur,zstar
      integer active,err,ordx,zeur,zstar
c     .. array arguments ..
      integer fstar(n),x(ordx)
c     .. local scalars ..
c      integer fmvf,i,ibig,idelta,ienlrg,imp,inf2,ipp,isalva,
c     +        isalvi,ivai,k,maxdix,maxica,mm1,mm10,mm11,mm12,mm13,mm14,
c     +        mm15,mm16,mm17,mm18,mm19,mm2,mm20,mm21,mm22,mm23,mm24,
c     +        mm25,mm26,mm3,mm4,mm5,mm6,mm7,mm8,mm9,nc,nci,ncodal,ngen,
c     +        nlsp,nnodi,nnodin,nodoim,nprob,nprobv,nq,offv,orcr,orcra,
c     +        ordsp,ordv,pnuovo,psalvo,punta,puntb,puntlv,r,sc1,totass,
c     +        ur,vimpa,vimpb,z,zarf,zc,zeri
      integer fmvf,offv,orcr,orcra,ordsp,ordv,pnuovo,psalvo,punta,
     +puntb,puntlv,r,sc1,totass,ur,vimpa,vimpb,z,zarf,zc,zeri
      logical artif,eur
c subroutines ..
c actpro,agmhp,calcud,calcur,cercsb,clearq,contci,copyx,creams,ctcs,
c enlarg,enlini,errors,exque,genson,inque,inquer,karp
c     ..
      inf2 = float(inf)/2.
      nq = n**2
      err = 0
c
      mm26 = ordx - n + 1
      mm25 = mm26 - n
      mm24 = mm25 - n
      mm23 = mm24 - n
      mm22 = mm23 - n
      mm21 = mm22 - n
      mm20 = mm21 - n
      mm19 = mm20 - n
      mm18 = mm19 - n
      mm17 = mm18 - n
      mm16 = mm17 - n
      mm15 = mm16 - n
      mm14 = mm15 - n
      mm13 = mm14 - n
      mm12 = mm13 - n
      mm11 = mm12 - n
      mm10 = mm11 - n
      mm9 = mm10 - n
      mm8 = mm9 - n - 1
      mm7 = mm8 - n
      mm6 = mm7 - n
      if (mm6.gt.nq) then
          mm5 = nq + (mm6-nq)*2./3.
          mm4 = nq + (mm6-nq)/3.
          mm3 = nq + 1
          maxica = mm6 - mm5 + 1
          orcra = mm5 - mm4 + 1
          maxdix = mm4 - mm3 + 1
          orcr = n + 1
c
          nprob = 1
          psalvo = 0
          ncodal = 0
          puntlv = 1
          vimpa = 0
          vimpb = 0
          sc1 = -1
          ngen = 1
          fmvf = 0
          nass = 1
c statistics information
          nexp = 1
          nprobq = 1
          lopt = 1
          avson = 0.
          spars = 0.
          active = 0
c   solve the initial assignment problem
          call ctcs(n,z,1,x(1),x(mm9),x(mm13),x(mm14),x(mm15),x(mm16),
     +              x(mm17),x(mm4),x(mm5),x(mm8),x(mm3),x(mm26),x(mm10),
     +              x(mm20),x(mm21),x(mm23),x(mm22),x(mm24),maxdix,
     +              maxica,x(mm25),orcr,orcra,inf2,err)
          if (err.eq.0) then
              lb0 = z
              lbc = z
c
c   totass stores the value of the ap solution at the root node
              totass = z
              call contci(x(mm9),nci,n,x(mm14))
              call enlini(n,x(mm8),x(mm3),x(mm9),x(mm26),x(mm10),
     +                    x(mm14),x(mm15),x(mm16),x(mm17),x(1),x(mm4),
     +                    x(mm5),nc,inf2)
              zstar = totass
              if (nc.eq.1) then
c  optimal solution found at the root node by ctcs or enlini
                  call copyx(x(mm9),fstar,n)
                  zstar = totass
                  return
              else
c
                  call copyx(x(mm9),x(mm13),n)
                  call karp(n,x(1),x(mm13),x(mm15),x(mm16),x(mm17),
     +                      x(mm18),totass,zstar,inf2)
c     check if the heuristic solution is an optimum
                  if (zeur.gt.0 .and. zeur.lt.zstar) then
                      eur = .true.
                      zstar = zeur
                  else
                      eur = .false.
                      call copyx(x(mm13),fstar,n)
                  end if
                  if (zstar.eq.totass) return
                  avson = nc
                  artif = .false.
                  if (alpha.gt.0.0) then
                      if (totass*alpha.lt.zstar) then
                          zstar = totass*alpha
                          zarf = zstar
                          artif = .true.
                      end if
                  end if
c
                  idelta = zstar - lb0
                  call creams(idelta,x(1),n,x(mm8),x(1),mm6,mm4,mm3,
     +                        x(mm26),x(mm10),zeri,nlsp,err)
                  if (err.eq.0) then
                      ordsp = nlsp
                      spars = nlsp*100./float(n* (n-1))
                      ienlrg = 0
                      if (float(zeri).gt.2.5*float(n)) ienlrg = 1
c
c   choose the subtour for the branch phase
c
                      call cercsb(x(mm9),x(mm14),x(mm10),n,nnodi,isalva,
     +                            inf2)
                      nnodin = nnodi
                      isalvi = isalva
c
c   define the working arrays to store the queue:
c   scalar and vectorial information
                      mm1 = 1
                      mm2 = mm3
                      ordv = mm2 - mm1
c
c mm1 = first element containing scalar informations
c mm2 = first position occupied by  matrices mf,mv
c
c    insert the root node in queue and initialize the vectors
c    for the branching phase
                      puntlv = 1
                      call inquer(isalvi,nnodin,psalvo,ngen,n,totass,
     +                            vimpa,vimpb,x(mm9),x(mm10),puntlv,
     +                            punta,puntb,ordv,x(mm6),x(mm7),x(mm1),
     +                            pnuovo,nprob,sc1,offv,err)
                      if (err.eq.0) then
                          psalvo = pnuovo
                          active = 0
c ww(6,fmt='('' root node: zstar='',i10,'' lb0='',i10)') zstar,lb0
                      else
                          call errors(err,4)
                          return
                      end if
                  else
                      call errors(err,3)
                      return
                  end if
              end if
          else
              call errors(err,2)
              return
          end if
      else
          call errors(err,1)
          return
      end if
c psalvo is the pointer to the actual problem
  100 do 200 i = 1,nprob
c
c  generate the descending nodes of psalvo
c
          call genson(n,x(mm6),x(mm7),x(mm10),nprob,i,vimpa,vimpb,
     +                nodoim,x(mm8),ordsp,x(mm3),x(mm4),inf2)
          call copyx(x(mm9),x(mm11),n)
          call copyx(x(mm10),x(mm12),n)
          zc = totass
          ivai = x(mm6+i-1)
c
c   solve the new map problem
c
          call calcur(n,x(mm8),ordsp,x(mm3),x(mm4),x(mm11),ivai,x(mm12),
     +                ur,inf2)
          call agmhp(n,ivai,x(mm8),ordsp,x(mm3),x(mm4),x(mm11),x(mm12),
     +               ur,zc,x(mm14),x(mm15),x(mm16),x(mm17),x(mm18),
     +               zstar,imp,inf2)
          if (nass.eq.maxnd) then
              call errors(err,-1)
              return
          else if (imp.ne.1) then
              if (zc.lt.zstar) then
                  nass = nass + 1
c
c   count the subtours
c
                  call contci(x(mm11),nc,n,x(mm15))
                  if (ienlrg.ne.0) then
                      call calcud(n,ordsp,x(mm3),x(mm8),x(mm4),x(mm12),
     +                            x(mm11),x(mm14))
                      call enlarg(n,x(mm8),ordsp,x(mm3),x(mm4),x(mm11),
     +                            x(mm14),x(mm12),x(mm15),x(mm16),
     +                            x(mm17),x(mm18),nc,inf2)
                  end if
                  if (nc.eq.1) then
c
c  found a new feasible solution
c
                      zstar = zc
c
c   clear the queue of the active problems
c
                      call clearq(zstar,puntb,fmvf,ordx,x(1),ncodal,
     +                            ordv,x(mm1),active)
                      call copyx(x(mm11),fstar,n)
c
c wwfmt='('' n.exp='',i8,'' n.prob.q='',i8,'' n.ass='',i8,     '' active='
c ',i8,'' av.son='',f8.2)' nexp,nprobq,nass,active,avson
c
c wwfmt='('' zstar='',i10,'' lbc='',i10,'' nprobq='',i8,'' active='',i8)'
c zstar,lbc,nprobq,active
c statistics
                      lopt = 2
                      ipp = psalvo
  105                 ipp = x(mm1+ipp+2)
                      lopt = lopt + 1
                      if (ipp.gt.1) go to 105
c
c   if the cost zc of the solution of the current problem p is ugual to
c   the lb of the problem father of p, say pf,  no other son of pf
c   have to be generated
c
                      ibig = x(mm1+psalvo+1)
                      if (zc.le.ibig) then
                          nprobv = i
                          go to 300
                      end if
                  else
                      call cercsb(x(mm11),x(mm14),x(mm12),n,nnodi,
     +                            isalva,inf2)
                      ngen = i
c
c  insert the problem in queue
c
                      if (fmvf.ne.0) then
                          k = fmvf
                          fmvf = x(fmvf)
                      else
                          mm2 = mm2 - 2*n
                          k = mm2
                      end if
                      ordv = mm2 - mm1
                      if (mm1+puntlv+offv+nnodi.lt.mm2) then
                          call inque(isalva,nnodi,sc1,psalvo,ngen,
     +                               x(mm12),x(mm11),n,zc,x(k),x(k+n),k,
     +                               vimpa,vimpb,ordv,x(mm1),ncodal,
     +                               puntlv,punta,puntb,offv,inf2)
                          nprobq = nprobq + 1
                          active = active + 1
                          r = int(nprobq/1000.)*1000
                          if (r.eq.nprobq) continue
c wwfmt='('' zstar='',i10,'' lbc='',i10,'' nprobq='',i8,       '' active='
c ',i8)' zstar,lbc,nprobq,active
                      else
                          call errors(err,5)
                          return
                      end if
                  end if
              end if
          end if
c
  200 continue
c
      nprobv = nprob
  300 if (ncodal.ne.0) then
c
c  extract the problem with lowest bound
          call exque(ordv,x(mm1),ordx,x(1),fmvf,x(mm10),x(mm9),
     +               pnuovo,ngen,punta,nprob,x(mm6),x(mm7),ncodal,n,sc1,
     +               totass,offv)
          avson = (avson*nexp+nprob)/float(nexp+1)
          nexp = nexp + 1
          active = active - 1
          if (lbc.lt.totass) then
              lbc = totass
c wwfmt='('' zstar='',i10,'' lbc='',i10,'' nprobq='',
c i8,'' active='',i8)' zstar,lbc,nprobq,active
          end if
          lbc = totass
 
c
c   update sets of included and excluded arcs
c
          call actpro(psalvo,n,nprobv,pnuovo,ngen,vimpa,vimpb,ordv,
     +                x(mm1),x(mm8),ordsp,x(mm3),x(mm4),offv,inf2)
          psalvo = pnuovo
          go to 100
      end if
c
c
c the tree search is terminated
c
      if (artif .and. zstar.eq.zarf) call errors(err,6)
      if (eur) then
c wwfmt='('' zeur is the optimal solution value'')'
c wwfmt='('' fstar() do not contain the optimal tour'')'
      end if
      return
      end
c *************************************************************************
      subroutine contci(f,nc,n,flag)
      integer f(n),flag(n)
c     .. local scalars i,k
      nc = 0
      do 100 i = 1,n
          flag(i) = 0
  100 continue
      do 200 i = 1,n
          if (flag(i).le.0) then
              k = i
              nc = nc + 1
  120         flag(k) = 1
              k = f(k)
              if (k.ne.i) go to 120
          end if
  200 continue
      return
      end
c *************************************************************************
      subroutine loadfv(f,v,n,vroot,froot)
      integer f(n),froot(n),v(n),vroot(n)
      do 100 i = 1,n
          froot(i) = f(i)
          vroot(i) = v(i)
  100 continue
      return
      end
c *************************************************************************
      subroutine backfv(vroot,froot,n,vd,f)
      integer f(n),froot(n),vd(n),vroot(n)
      do 100 i = 1,n
          f(i) = froot(i)
          vd(i) = vroot(i)
  100 continue
      return
      end
c *************************************************************************
      subroutine inquer(isalva,nnodi,psalvo,ngen,n,totass,vimpa,vimpb,f,
     +                  vd,puntlv,punta,puntb,ordv,iva,ivb,v,p2p,nprob,
     +                  sc1,offv,ierr)
      integer offv,ordv,p2p,psalvo,punta,
     +        puntb,puntlv,sc1,totass,vimpa,vimpb
      integer f(n),iva(n),ivb(n),v(ordv),vd(n)
      integer i,i32,ksalva,nm1,nodob,nodopa,plvp
c     ..
      i32 = 32000
      offv = 7
      if (puntlv+offv+nnodi.le.ordv) then
          nm1 = ngen - 1
          sc1 = sc1 + (nm1)*10 + 1
          v(puntlv+2) = totass
          v(puntlv+3) = psalvo
          v(puntlv+4) = ngen*i32 + nnodi
          v(puntlv+5) = 1
          v(puntlv+6) = vimpa*i32 + vimpb
          v(puntlv+7) = sc1
          plvp = puntlv + offv
          i = 1
          ksalva = isalva
          nodopa = isalva
   50     nodob = f(nodopa)
          v(plvp+i) = nodopa*i32 + nodob
          iva(i) = nodopa
          ivb(i) = nodob
          i = i + 1
          nodopa = nodob
          if (nodopa.ne.ksalva) go to 50
          punta = puntlv
          puntb = puntlv + 1
          puntlv = puntlv + offv + nnodi + 1
          p2p = punta
          nprob = nnodi
          sc1 = 0
          return
      end if
      ierr = 2
      return
      end
c *************************************************************************
      subroutine copyx(f,fc,n)
      integer f(n),fc(n)
      do 100 i = 1,n
          fc(i) = f(i)
  100 continue
      return
      end
c *************************************************************************
      subroutine ctcs(n,z,ks,a,f,r,vs,fs,uold,sur,cra,ica,cr,ic,u,v,fb,
     +                pi,c,dm,ll,maxdix,maxica,rx,orcr,orcra,inf,ierr)
c   solution of the linear min-sum assignment problem through the
c   algorithm presented in the paper:
c   g. carpaneto and p. toth, "primal-dual algorithms for the
c   assignment problem", discrete applied mathematics 18, 137-153,1987
      integer ierr,inf,ks,maxdix,maxica,n,orcr,orcra,z
      integer a(n,n),c(n),cr(orcr),cra(orcra),dm(n),f(n),fb(n),fs(n),
     +        ic(maxdix),ica(maxica),ll(n),pi(n),r(n),rx(n),sur(n),u(n),
     +        uold(n),v(n),vs(n)
c     .. local scalars ..
      integer i,iflag,j,k,kfeas,kopt,krip,kth,m,m1,max,maxdim,
     +        maxfea,maxrip,nfeas,nkk,nur,zz
c  subroutines apmmix,asmixm,feaso,indus3,opto,setupo
c     .. data statements ..
      data maxrip/5/,maxfea/1/
c     ..
      maxdim = maxdix - n
      krip = 0
      nfeas = 0
      do 100 i = 1,n
          f(i) = 0
          fb(i) = 0
          cra(i) = 0
          a(i,i) = inf
  100 continue
      nkk = 0
      cr(1) = -1
      m1 = -1
      call indus3(n,a,f,m,u,v,fb,pi,max,inf)
      z = 0
      zz = 0
      do 200 i = 1,n
          z = z + v(i)
          zz = zz + u(i)
  200 continue
      if (m.ne.n) then
          if (ks.eq.1) then
c
              if (float(m)/float(n).gt.0.6) then
                  ierr = 0
                  call setupo(n,a,u,v,m,ic,cr,kth,maxdim,iflag,inf)
                  if (iflag.ne.1) then
                      if (cr(1).ge.0) then
                          do 202 i = 1,n
                              vs(i) = v(i)
                              fs(i) = f(i)
                              uold(i) = u(i)
  202                     continue
  204                     call asmixm(n,a,ic,cr,f,fb,u,v,z,pi,r,c,cra,
     +                                ica,sur,nur,dm,ll,rx,inf)
                          if (ierr.ne.0) return
                          if (nur.gt.0) then
                              if (nfeas.ne.maxfea) then
                                  nfeas = nfeas + 1
                                  call feaso(n,a,kfeas,kth,uold,vs,
     +                                       nfeas,cra,ica,nkk,sur,nur,
     +                                       maxica)
                                  if (kfeas.eq.1) go to 204
                              end if
                              do 206 j = 1,n
                                  fb(j) = 0
  206                         continue
                              do 208 i = 1,n
                                  j = fs(i)
                                  if (j.gt.0) fb(j) = i
                                  u(i) = uold(i)
                                  v(i) = vs(i)
                                  f(i) = fs(i)
  208                         continue
                          else
                              call opto(n,a,u,v,kopt,cra,ica,nkk,f,fb,
     +                                  uold,maxica,ierr)
                              if (ierr.eq.0) then
                                  if (kopt.eq.1) return
                                  if (kopt.ne.m1) then
                                      if (krip.ne.maxrip) then
                                          krip = krip + 1
                                          go to 204
                                      end if
                                  end if
                              end if
                          end if
                      end if
                  end if
              end if
          end if
          call apmmix(n,a,f,z,fb,u,v,pi,r,c,dm,ll,inf)
          return
      end if
      z = 0
      do 300 k = 1,n
          z = z + u(k) + v(k)
  300 continue
      return
      end
c *************************************************************************
      subroutine opto(n,a,u,v,kopt,cra,ica,nkk,f,fb,uold,maxica,ierr)
c check the feasibility of the dual solution , and hence the
c optimality of the primal solution found by the sparse matrix
c procedure.
      integer ierr,kopt,maxica,n,nkk
      integer a(n,n),cra(*),f(n),fb(n),ica(maxica),u(n),uold(n),v(n)
      integer i,ia,j,jc,k,kk,kkn,min,neg
c     ..
      kopt = 0
      kk = nkk
      neg = 0
      do 100 i = 1,n
          if (u(i).ne.uold(i)) then
              min = 0
              k = i
              do 20 j = 1,n
                  ia = a(i,j) - u(i) - v(j)
                  if (ia.lt.0) then
                      if (ia.lt.0) neg = neg + 1
                      kk = kk + 1
                      if (kk.le.maxica) then
                          kkn = kk + n
    2                     if (cra(k).eq.0) then
                              cra(k) = kkn
                              cra(kkn) = 0
                              ica(kk) = j
                          else
                              k = cra(k)
                              go to 2
                          end if
                      end if
                      if (ia.lt.min) min = ia
                  end if
   20         continue
              u(i) = u(i) + min
              uold(i) = u(i)
              if (min.ne.0) then
                  jc = f(i)
                  f(i) = 0
                  fb(jc) = 0
                  kopt = kopt - 1
              end if
          end if
  100 continue
      nkk = kk
      if (nkk.le.maxica) then
          if (kopt.lt.0) then
              kopt = 0
              return
          else
              kopt = 1
              return
          end if
      end if
      ierr = 1
      kopt = -1
      return
      end
c *************************************************************************
      subroutine asmixm(n,a,ic,cr,f,fb,dualu,dualv,z,pre,uv,sr,cra,ica,
     +                  sur,nur,dm,ll,r,inf)
c   shortest augmenting path and hungarian method :
c   version for sparse matrices
      integer inf,n,nur,z
      integer a(n,n),cr(*),cra(*),dm(n),dualu(n),dualv(n),f(n),fb(n),
     +        ic(*),ica(*),ll(n),pre(n),r(n),sr(n),sur(n),uv(n)
      integer aa,d,i,i1,i2,i2p1,ii,iix,ik,ind,index,indexv,iv,j,j1,k,k1,
     +        k2,kdir,kmn,knuv,li,nuv,u,w
c     ..
      nur = 0
      do 500 u = 1,n
          if (f(u).gt.0) go to 500
          do 50 i = 1,n
              ll(i) = 0
              dm(i) = inf
   50     continue
          nuv = n
          j1 = 0
          knuv = 0
          k1 = cr(u)
          k2 = cr(u+1) - 1
          do 100 ik = k1,k2
              i = ic(ik)
              dm(i) = a(u,i) - dualu(u) - dualv(i)
              pre(i) = u
              j1 = j1 + 1
              sr(j1) = i
  100     continue
          k = u
  150     if (cra(k).eq.0) then
              r(1) = u
          else
              k = cra(k)
              kmn = k - n
              i = ica(kmn)
              dm(i) = a(u,i) - dualu(u) - dualv(i)
              pre(i) = u
              j1 = j1 + 1
              sr(j1) = i
              go to 150
          end if
  200     d = inf
          index = 0
          i2 = 1
          i1 = i2
c
          if (nuv.lt.j1) then
              kdir = 1
              if (knuv.ne.1) then
                  iv = 0
                  do 210 i = 1,n
                      if (ll(i).ne.1) then
                          iv = iv + 1
                          uv(iv) = i
                      end if
  210             continue
                  knuv = 1
              end if
              do 220 iv = 1,nuv
                  i = uv(iv)
                  if (dm(i).le.d) then
                      if (dm(i).ne.d) then
                          index = 0
                          i2 = i1
                      end if
                      d = dm(i)
                      if (fb(i).le.0) then
                          index = i
                          if (d.eq.0) go to 300
                      end if
                      i2 = i2 + 1
                      r(i2) = iv
                  end if
  220         continue
          else
              kdir = 0
c
              do 240 li = 1,j1
                  i = sr(li)
                  if (dm(i).le.d) then
                      if (ll(i).ne.1) then
                          if (dm(i).ne.d) then
                              index = 0
                              i2 = i1
                          end if
                          d = dm(i)
                          if (fb(i).le.0) then
                              index = i
                              if (d.eq.0) go to 300
                          end if
                          i2 = i2 + 1
                          r(i2) = i
                      end if
                  end if
  240         continue
          end if
          if (d.eq.inf) then
c
              nur = nur + 1
              sur(nur) = u
              go to 500
          else if (index.le.0) then
              i1 = i1 + 1
              i2p1 = i2 + i1
c
              do 280 iix = i1,i2
                  ii = i2p1 - iix
                  if (kdir.eq.1) then
                      indexv = r(ii)
                      index = uv(indexv)
                      uv(indexv) = uv(nuv)
                  else
                      index = r(ii)
                  end if
                  ll(index) = 1
                  nuv = nuv - 1
                  w = fb(index)
                  k1 = cr(w)
                  k2 = cr(w+1) - 1
c
                  do 250 ik = k1,k2
                      i = ic(ik)
                      if (ll(i).ne.1) then
                          aa = d + a(w,i) - dualu(w) - dualv(i)
                          if (dm(i).gt.aa) then
                              if (dm(i).ge.inf) then
                                  j1 = j1 + 1
                                  sr(j1) = i
                              end if
                              dm(i) = aa
                              pre(i) = w
                          end if
                      end if
  250             continue
                  k = w
  260             if (cra(k).ne.0) then
                      k = cra(k)
                      kmn = k - n
                      i = ica(kmn)
                      if (ll(i).ne.1) then
                          aa = d + a(w,i) - dualu(w) - dualv(i)
                          if (dm(i).gt.aa) then
                              if (dm(i).ge.inf) then
                                  j1 = j1 + 1
                                  sr(j1) = i
                              end if
                              dm(i) = aa
                              pre(i) = w
                          end if
                      end if
                      go to 260
                  end if
  280         continue
              go to 200
          end if
c
  300     do 350 j = 1,n
              if (dm(j).lt.d) then
                  dualv(j) = dualv(j) + dm(j) - d
                  i = fb(j)
                  dualu(i) = dualu(i) - dm(j) + d
              end if
  350     continue
          dualu(u) = dualu(u) + d
c
  400     w = pre(index)
          fb(index) = w
          ind = f(w)
          f(w) = index
          if (w.ne.u) then
              index = ind
              go to 400
          end if
  500 continue
      if (nur.le.0) then
c
          z = 0
          do 550 i = 1,n
              z = z + dualu(i) + dualv(i)
  550     continue
          return
      end if
      z = -1
      return
      end
c *************************************************************************
      subroutine feaso(n,a,kfeas,kth,us,vs,nfeas,cra,ica,nkk,sur,nur,
     +                 maxica)
c insert new entries in rows sur (l) (l = 1,nur) in order to find
c a feasible assignment in the sparse cost matrix.
      integer kfeas,kth,maxica,n,nfeas,nkk,nur
      integer a(n,n),cra(*),ica(maxica),sur(n),us(n),vs(n)
      integer coef,i,ia,ii,iukth,iukthn,j,k,kk,kkn,kth0,kthn
      data coef/3/
c     ..
      kk = nkk
      kth0 = kth
      if (kth0.eq.0) kth0 = 1
      do 200 ii = 1,nur
          i = sur(ii)
          cc = 0.
   50     cc = cc + float(coef*nfeas)
          kthn = cc*float(kth0)
          iukthn = kthn + us(i)
          iukth = kth + us(i)
          k = i
          do 100 j = 1,n
              ia = a(i,j) - vs(j)
              if (ia.le.iukthn) then
                  if (ia.gt.iukth) then
                      kk = kk + 1
                      if (kk.gt.maxica) go to 300
                      kkn = kk + n
                      cra(k) = kkn
                      cra(kkn) = 0
                      ica(kk) = j
                  end if
              end if
  100     continue
          if (cra(i).eq.0) go to 50
  200 continue
      nkk = kk
      kfeas = 1
      return
  300 kfeas = -1
      return
      end
c
c *************************************************************************
      subroutine indus3(n,a,f,m,u,v,fb,fu,max,inf)
c search for an initial dual solution ( u(i),v(j) ) and an initial
c partial primal solution ( f(i),fb(j) ) of the ap problem
      integer inf,m,max,n
      integer a(n,n),f(n),fb(n),fu(n),u(n),v(n)
      integer i,ia,ii,imin,j,j1,jj,jmin,l,maxl,min
c     ..
      m = 0
      do 100 j = 1,n
          u(j) = 0
          min = inf
          do 50 i = 1,n
              ia = a(i,j)
              if (ia.le.min) then
                  if (ia.ge.min) then
                      if (f(i).ne.0) go to 50
                  end if
                  min = ia
                  imin = i
              end if
   50     continue
          v(j) = min
          if (f(imin).eq.0) then
              m = m + 1
              fb(j) = imin
              f(imin) = j
              fu(imin) = j + 1
          end if
  100 continue
      max = 0
      do 400 i = 1,n
          if (f(i).ne.0) go to 400
          min = inf
          maxl = 0
          do 150 j = 1,n
              l = a(i,j) - v(j)
              if (l.gt.max) maxl = l
              if (l.le.min) then
                  if (l.ge.min) then
                      if (fb(j).ne.0) go to 150
                      if (fb(jmin).eq.0) go to 150
                  end if
                  min = l
                  jmin = j
              end if
  150     continue
          u(i) = min
          if (maxl-min.gt.max) max = maxl - min
          j = jmin
          if (fb(j).eq.0) go to 300
          do 200 j = jmin,n
              if (a(i,j)-v(j).le.min) then
                  ii = fb(j)
                  j1 = fu(ii)
                  if (j1.le.n) then
                      do 155 jj = j1,n
                          if (fb(jj).le.0) then
                              if (a(ii,jj)-u(ii).eq.v(jj)) go to 250
                          end if
  155                 continue
                      fu(ii) = n + 1
                  end if
              end if
  200     continue
          go to 400
  250     f(ii) = jj
          fb(jj) = ii
          fu(ii) = jj + 1
  300     m = m + 1
          fb(j) = i
          f(i) = j
          fu(i) = j + 1
  400 continue
      return
      end
c *************************************************************************
      subroutine apmmix(n,a,f,z,fb,dualu,dualv,pre,uv,r,dm,dp,inf)
c    shortest augmenting path and hungarian method for complete matrices
      integer inf,n,z
      integer a(n,n),dm(n),dp(n),dualu(n),dualv(n),f(n),fb(n),pre(n),
     +        r(n),uv(n)
      integer d,i,i1,i2,i2p1,ii,iix,ind,index,indexv,iv,nuv,u,vgl,w
c     ..
      do 400 u = 1,n
          if (f(u).gt.0) go to 400
          do 50 i = 1,n
              pre(i) = u
              uv(i) = i
              dp(i) = inf
              dm(i) = a(u,i) - dualu(u) - dualv(i)
   50     continue
          nuv = n
          i2 = 0
          r(1) = u
          dp(u) = 0
  100     d = inf
          index = 0
          i1 = i2
          do 150 iv = 1,nuv
              i = uv(iv)
              if (dm(i).le.d) then
                  if (dm(i).ne.d) then
                      index = 0
                      i2 = i1
                  end if
                  d = dm(i)
                  if (fb(i).le.0) then
                      if (d.eq.0) go to 250
                      index = i
                  end if
                  i2 = i2 + 1
                  r(i2) = iv
              end if
  150     continue
          if (index.gt.0) go to 300
          i1 = i1 + 1
          i2p1 = i2 + i1
          do 200 iix = i1,i2
              ii = i2p1 - iix
              indexv = r(ii)
              index = uv(indexv)
              uv(indexv) = uv(nuv)
              nuv = nuv - 1
              w = fb(index)
              dp(w) = d
              do 160 iv = 1,nuv
                  i = uv(iv)
                  vgl = d + a(w,i) - dualu(w) - dualv(i)
                  if (dm(i).gt.vgl) then
                      dm(i) = vgl
                      pre(i) = w
                  end if
  160         continue
  200     continue
          go to 100
c
  250     index = i
  300     w = pre(index)
          fb(index) = w
          ind = f(w)
          f(w) = index
          if (w.eq.u) then
c
              do 320 i = 1,n
                  if (dp(i).ne.inf) dualu(i) = dualu(i) + d - dp(i)
                  if (dm(i).lt.d) dualv(i) = dualv(i) + dm(i) - d
  320         continue
          else
              index = ind
              go to 300
          end if
  400 continue
c
      z = 0
      do 500 i = 1,n
          z = z + dualu(i) + dualv(i)
  500 continue
      return
      end
c *************************************************************************
      subroutine agmhp(n,r,cr,ordsp,ic,c,f,v,ur,z,p,q,hp,dm,fb,zs,imp,
     +                 inf)
c agmheap search for an augmenting path starting at row r, using
c johnson algorithm, implemented via a heap-queue
      integer imp,inf,n,ordsp,r,ur,z,zs
      integer c(ordsp),cr(n+1),dm(n),f(n),fb(n),hp(n),ic(ordsp),p(n),
     +        q(n),v(n)
      integer delta,dj,dp1,du,fb1,fbj,h,hp1,hp2,hp3,i,ifin,ind,iniz,j,k,
     +        k2,nhp,w
c     ..
      delta = zs - z + ur
      imp = 0
      f(r) = 0
      do 100 i = 1,n
          fb(i) = 0
  100 continue
      do 200 i = 1,n
          q(i) = 0
          p(i) = 0
          j = f(i)
          if (j.gt.0) fb(j) = i
          dm(i) = inf
  200 continue
      nhp = 0
      p(r) = 0
      du = 0
      w = r
      iniz = cr(w)
      ifin = cr(w+1) - 1
      if (iniz.le.ifin) go to 500
      go to 700
  300 w = fb(i)
c
      iniz = cr(w)
      ifin = cr(w+1) - 1
      if (iniz.gt.ifin) go to 700
c
      do 400 h = iniz,ifin
          j = ic(h)
          if (j.eq.i) then
              du = du - c(h)
              go to 500
          end if
  400 continue
c
  500 do 600 h = iniz,ifin
          j = ic(h)
          dj = du + c(h) - v(j)
          if (w.ne.r) dj = dj + v(i)
          if (dm(j).gt.dj) then
              dm(j) = dj
              p(j) = w
              fbj = fb(j)
              if (q(j).eq.0) then
c
                  nhp = nhp + 1
                  q(j) = nhp
              end if
c
              k = q(j)
  520         k2 = k/2
              if (k2.gt.0) then
                  hp2 = hp(k2)
                  if (dj.le.dm(hp2)) then
                      if (dj.ge.dm(hp2)) then
                          if (fbj.ne.0 .or. fb(hp2).eq.0) go to 540
                      end if
                      hp(k) = hp2
                      q(hp2) = k
                      k = k2
                      go to 520
                  end if
              end if
  540         hp(k) = j
              q(j) = k
          end if
  600 continue
c
  700 i = hp(1)
      du = dm(i)
      if (du.gt.delta) go to 1000
      if (fb(i).eq.0) then
  750     w = p(i)
          fb(i) = w
          ind = f(w)
          f(w) = i
          if (w.ne.r) then
c
              i = ind
              go to 750
          else
c
             do 760 i = 1,n
                  if (dm(i).lt.du) v(i) = v(i) + dm(i) - du
  760         continue
              z = z + du - ur
              return
          end if
      else
          q(i) = 0
          nhp = nhp - 1
c
          if (nhp.lt.0) then
c
              imp = 1
              return
          else if (nhp.eq.0) then
              go to 300
          else
c
              hp1 = hp(nhp+1)
              dp1 = dm(hp1)
              fb1 = fb(hp1)
              k = 1
          end if
      end if
  800 k2 = k*2
      if (k2.lt.nhp) then
          hp2 = hp(k2)
          hp3 = hp(k2+1)
          if (dm(hp2).ge.dm(hp3)) then
              if (dm(hp2).ne.dm(hp3) .or. fb(hp2).ne.0) then
                  hp2 = hp3
                  k2 = k2 + 1
              end if
          end if
      else if (k2.ne.nhp) then
          go to 900
      end if
      hp2 = hp(k2)
      if (dp1.le.dm(hp2)) then
          if (dp1.lt.dm(hp2)) go to 900
          if (fb1.eq.0 .or. fb(hp2).ne.0) go to 900
      end if
      hp(k) = hp2
      q(hp2) = k
      k = k2
      go to 800
  900 hp(k) = hp1
      q(hp1) = k
      go to 300
 1000 z = inf
      return
      end
c *************************************************************************
      subroutine enlini(n,cr,ic,f,u,v,s,p,flag,cont,a,cra,ica,nc,inf)
c     try to connect subtours of the solution of the assignment  at
c     the root node, with zero reduced cost arcs
      integer inf,n,nc
      integer a(n,n),cont(n),cr(n+1),cra(*),f(n),flag(n),ic(*),ica(*),
     +        p(n),s(n),u(n),v(n)
      integer card,corid,flaga,fsalva,i,iall,ii,ij,ijn,ik,iolds,ip1,ip2,
     +        istart,j,k,kc,kk,l,lm,lm2,m1,nc2,ninf2,pj,si,ult
c     ..
      ninf2 = -float(inf)/2.
      m1 = -1
      kc = 0
      iall = 0
      do 100 i = 1,n
          j = f(i)
          s(i) = j
          p(j) = i
          flag(i) = 0
          cont(i) = 0
  100 continue
      do 200 istart = 1,n
          if (flag(istart).ne.0) go to 200
          kc = kc + 1
          k = istart
          card = 0
  150     flag(k) = -kc
          k = s(k)
          if (v(k).gt.ninf2) card = card + 1
          if (k.ne.istart) go to 150
          if (card.ne.n) then
              j = 2*kc
              do 160 i = 1,j,2
                  if (cont(i).le.card) then
                      if (cont(i).ne.0) then
                          ip2 = i + 2
                          do 152 l = ip2,j
                              lm = j + ip2 - l
                              lm2 = lm - 2
                              cont(lm) = cont(lm2)
  152                     continue
                      end if
                      cont(i) = card
                      ip1 = i + 1
                      cont(ip1) = istart
                      go to 200
                  end if
  160         continue
          else
              nc = 1
              return
          end if
  200 continue
      nc = kc
      nc2 = nc*2
      do 600 ii = 2,nc2,2
          istart = cont(ii)
          if (flag(istart).gt.0) go to 600
          fsalva = iabs(flag(istart))
          flaga = 0
          i = istart
  250     j = f(i)
          if (v(j).le.ninf2) go to 550
          if (cr(1).ne.m1) then
c
              ip1 = i + 1
              ult = cr(ip1) - 1
              ik = i
              k = cr(i)
              j = ic(k)
              go to 400
          end if
  300     k = 1
          ult = n
  350     j = k
c
  400     if (i.eq.j) go to 500
  450     corid = a(i,j) - u(i) - v(j)
          if (corid.le.0) then
              if (iabs(flag(j)).ne.fsalva) then
                  if (flaga.ne.0 .or. flag(j).le.0) then
                      pj = p(j)
                      if (v(j).ge.ninf2) then
                          si = f(i)
                          if (a(pj,si)-u(pj).le.v(si)) then
c
                              flaga = 1
                              iall = iall + 1
                              nc = nc - 1
                              si = f(i)
                              pj = p(j)
                              f(i) = j
                              p(j) = i
                              f(pj) = si
                              p(si) = pj
                              iolds = s(i)
                              kk = j
                              if (flag(j).gt.0) s(i) = pj
                              if (flag(j).le.0) s(i) = j
                              s(pj) = iolds
  452                         flag(kk) = fsalva
                              kk = f(kk)
                              if (kk.ne.si) go to 452
                              if (cr(1).eq.m1) go to 300
                              k = cr(i)
                              j = ic(k)
                              go to 400
                          end if
                      end if
                  end if
              end if
          end if
  500     k = k + 1
          if (cr(1).ne.m1) then
              if (k.le.ult) then
                  j = ic(k)
                  go to 400
              else if (cra(ik).ne.0) then
                  ij = cra(ik)
                  ik = ij
                  ijn = ij - n
                  j = ica(ijn)
                  go to 450
              end if
          else if (k.le.ult) then
              go to 350
          end if
  550     flag(i) = iabs(flag(i))
          i = s(i)
          if (i.ne.istart) go to 250
  600 continue
      return
      end
c *************************************************************************
      subroutine enlarg(n,cr,ordsp,ic,c,f,u,vd,s,p,flag,cont,nc,inf)
c     try to connect subtours of the current modified assignment
c     problem, with zero reduced cost arcs
      integer inf,n,nc,ordsp
      integer c(ordsp),cont(n),cr(n+1),f(n),flag(n),ic(ordsp),p(n),s(n),
     +        u(n),vd(n)
      integer card,corid,flaga,fsalva,i,iall,ii,incr,iolds,ip1,ip2,ipj,
     +        istart,j,jj,k,kc,kk,l,lm,lm2,m1,meta,minf2,nc2,pj,si,ult,
     +        ult2
c     ..
      minf2 = -float(inf)/2.
      kc = 0
      m1 = -1
      iall = 0
      do 100 i = 1,n
          j = f(i)
          s(i) = j
          p(j) = i
          flag(i) = 0
          cont(i) = 0
  100 continue
      do 200 istart = 1,n
          if (flag(istart).ne.0) go to 200
          kc = kc + 1
          k = istart
          card = 0
  150     flag(k) = -kc
          k = s(k)
          if (vd(k).gt.minf2) card = card + 1
          if (k.ne.istart) go to 150
          if (card.ne.n) then
              j = 2*kc
              do 160 i = 1,j,2
                  if (cont(i).le.card) then
                      if (cont(i).ne.0) then
                          ip2 = i + 2
                          do 152 l = ip2,j
                              lm = j + ip2 - l
                              lm2 = lm - 2
                              cont(lm) = cont(lm2)
  152                     continue
                      end if
                      cont(i) = card
                      ip1 = i + 1
                      cont(ip1) = istart
                      go to 200
                  end if
  160         continue
          else
              nc = 1
              return
          end if
  200 continue
      nc = kc
      nc2 = nc*2
      do 400 ii = 2,nc2,2
          istart = cont(ii)
          if (flag(istart).gt.0) go to 400
          fsalva = iabs(flag(istart))
          flaga = 0
          i = istart
  250     j = f(i)
          if (vd(j).le.minf2) go to 350
          ip1 = i + 1
          ult = cr(ip1) - 1
          k = cr(i)
  300     j = ic(k)
          corid = c(k) - u(i) - vd(j)
          if (corid.le.0) then
              if (iabs(flag(j)).ne.fsalva) then
                  if (flaga.ne.0 .or. flag(j).le.0) then
                      pj = p(j)
                      if (vd(j).ge.minf2) then
                          si = f(i)
                          ipj = pj + 1
                          ult2 = cr(ipj) - 1
                          kk = cr(pj)
                          meta = float(ic(ult2)-ic(kk))/2.
                          if (si.gt.meta) then
                              ipj = pj + 1
                              kk = cr(ipj) - 1
                              ult2 = cr(pj)
                              incr = -1
                          else
                              incr = 1
                          end if
  302                     jj = ic(kk)
                          if (jj.eq.si) then
                              if (c(kk)-u(pj).le.vd(si)) then
                                  flaga = 1
                                  iall = iall + 1
                                  nc = nc - 1
                                  si = f(i)
                                  pj = p(j)
                                  f(i) = j
                                  p(j) = i
                                  f(pj) = si
                                  p(si) = pj
                                  iolds = s(i)
                                  kk = j
                                  if (flag(j).gt.0) s(i) = pj
                                  if (flag(j).le.0) s(i) = j
                                  s(pj) = iolds
  304                             flag(kk) = fsalva
                                  kk = f(kk)
                                  if (kk.ne.si) go to 304
                                  k = cr(i)
                                  go to 300
                              end if
                          else if ((jj.le.si) .or. (incr.ne.1)) then
                              if ((jj.ge.si) .or. (incr.ne.m1)) then
                                  if (kk.ne.ult2) then
                                      kk = kk + incr
                                      go to 302
                                  end if
                              end if
                          end if
                      end if
                  end if
              end if
          end if
          k = k + 1
          if (k.le.ult) go to 300
  350     flag(i) = iabs(flag(i))
          i = s(i)
          if (i.ne.istart) go to 250
  400 continue
      return
      end
c *************************************************************************
      subroutine karp(n,a,f,p,flag,na,nd,totass,ub,inf)
c     heuristic solution of the atsp using the patching algorithm
      integer inf,n,totass,ub
      integer a(n,n),f(n),flag(n),na(n),nd(n),p(n)
      integer cont,costo,fsalva,i,im1,istart,j,k,kk,kp,min,mn1,mn2,mn3,
     +        nc,nim1,pmn1,rr,s2,temp,z
c     ..
      do 100 i = 1,n
          flag(i) = 0
  100 continue
      cont = 0
      nc = 0
      do 200 i = 1,n
          if (flag(i).le.0) then
              k = i
              nc = nc + 1
  120         flag(k) = nc
              cont = cont + 1
              kk = f(k)
              p(kk) = k
              k = kk
              if (k.ne.i) go to 120
              na(nc) = cont
              nd(nc) = i
              cont = 0
          end if
  200 continue
      if (nc.eq.1) return
c
c sort the subtours
c
      ub = totass
      z = nc
  300 min = n
      do 400 i = 1,z
          if (na(i).lt.min) then
              min = na(i)
              k = i
          end if
  400 continue
      na(k) = na(z)
      na(z) = min
      temp = nd(k)
      nd(k) = nd(z)
      nd(z) = temp
      z = z - 1
      if (z.ge.2) go to 300
      rr = 2
  500 istart = nd(rr)
      i = istart
      im1 = rr - 1
      nim1 = nd(im1)
      fsalva = flag(nim1)
      mn2 = inf
  600 do 700 k = 1,n
          if (flag(k).eq.fsalva) then
              j = f(i)
              kp = p(k)
              costo = a(i,k) + a(kp,j) - a(kp,k) - a(i,j)
              if (costo.lt.mn2) then
                  mn1 = k
                  mn2 = costo
                  mn3 = i
                  if (costo.eq.0) go to 800
              end if
          end if
  700 continue
      i = f(i)
      if (i.ne.istart) go to 600
  800 s2 = f(mn3)
      pmn1 = p(mn1)
      f(mn3) = mn1
      p(mn1) = mn3
      f(pmn1) = s2
      p(s2) = pmn1
      ub = ub + mn2
c
c update flag
c
      fsalva = flag(istart)
      kk = mn1
  900 flag(kk) = fsalva
      kk = f(kk)
      if (kk.ne.s2) go to 900
      nc = nc - 1
      if (nc.eq.1) return
      rr = rr + 1
      go to 500
      end
c *************************************************************************
      subroutine cercsb(f,flag,vd,n,cmin,isalva,inf)
c chose the subtour for the branch phase
      integer cmin,inf,isalva,n
      integer f(n),flag(n),vd(n)
      integer card,i,istart,k,kc,minf2
c     ..
      minf2 = -float(inf)/2.
      cmin = inf
      kc = 0
      do 100 i = 1,n
          flag(i) = 0
  100 continue
      do 200 istart = 1,n
          if (flag(istart).ne.0) go to 200
          kc = kc + 1
          card = 0
          k = istart
  150     flag(k) = kc
          k = f(k)
          if (vd(k).ge.minf2) card = card + 1
          if (k.ne.istart) go to 150
          if (card.lt.cmin) then
              cmin = card
              isalva = istart
          end if
  200 continue
      return
      end
c *************************************************************************
      subroutine clearq(zstar,puntb,fmvf,ordx,x,ncodal,ordv,v,active)
c     make not active nodes with lb greater than zstar
      integer active,fmvf,ncodal,ordv,ordx,puntb,zstar
      integer v(ordv),x(ordx)
      integer j
c     ..
  100 if (ncodal.eq.0) return
      if (v(puntb+1).lt.zstar) return
      j = v(puntb+4)
      x(j) = fmvf
      fmvf = j
      puntb = v(puntb)
      ncodal = ncodal - 1
      active = active - 1
      go to 100
      end
c *************************************************************************
      subroutine inque(isalva,nnodi,sc1,psalvo,ngen,vd,f,n,totass,mv,mf,
     +                 mm2,vimpa,vimpb,ordv,v,ncodal,puntlv,punta,puntb,
     +                 ioffv,inf)
c  psalvo = pointer to the father
c  nnodi  = number of nodes in the subtour
      integer inf,ioffv,isalva,mm2,n,ncodal,ngen,nnodi,ordv,psalvo,
     +        punta,puntb,puntlv,sc1,totass,vimpa,vimpb
      integer f(n),mf(n),mv(n),v(ordv),vd(n)
      integer i,i32,ksalva,lb,lb1,minf2,nm1,
     +        nodob,nodopa,pcorr,plvp,puntol,sc2,sc3
c     ..
      i32 = 32000
c
c     insert the problem in the queue in this order
c     (from the third location):
c     lb,
c     psalvo,
c     the generation number (among the sons of the same father) and the
c       number of not imposed arcs in the subtour choosen for the branch
c       phase (packed),
c     the pointer to the column of matrices mv,mf,
c     the additional arc to exclude from solution
c       (start node + end node, packed),
c     the score sc1 ((n.of excluded arcs)*10+number of imposed arcs),
c     the not imposed arcs in the subtour choosen for the
c       branch phase (packed)
c
      minf2 = -float(inf)/2.
      nm1 = ngen - 1
      sc1 = sc1 + (nm1)*10 + 1
      v(puntlv+2) = totass
      v(puntlv+3) = psalvo
      v(puntlv+4) = ngen*i32 + nnodi
      v(puntlv+5) = mm2
      do 100 i = 1,n
          mv(i) = vd(i)
          mf(i) = f(i)
  100 continue
      v(puntlv+6) = vimpa*i32 + vimpb
      v(puntlv+7) = sc1
      plvp = puntlv + ioffv
      i = 1
      ksalva = isalva
      nodopa = isalva
  200 nodob = f(nodopa)
      if (vd(nodob).ge.minf2) then
          v(plvp+i) = nodopa*i32 + nodob
          i = i + 1
      end if
      nodopa = nodob
      if (nodopa.ne.ksalva) go to 200
c
c     puntlv: pointer at the first  empty position of v()
c     punta : pointer at the active problem with best lb
c     puntb : pointer at the active problem with worst lb
c
      ncodal = ncodal + 1
      if (ncodal.eq.1) then
c
c     insert the first problem
c
          punta = puntlv
          puntb = puntlv + 1
      else
          sc2 = v(punta+ioffv)
          sc3 = v(puntb+ioffv-1)
          lb = totass
          if (lb.lt.v(punta+2) .or. (lb.eq.v(punta+2).and.(sc1.ge.sc2)))
     +        then
c
              v(puntlv) = punta
              v(punta+1) = puntlv + 1
              punta = puntlv
          else if (lb.gt.v(puntb+1) .or. (lb.eq.v(puntb+1).and.
     +            (sc1.le.sc3)))  then
c
              v(puntlv+1) = puntb
              v(puntb-1) = puntlv
              puntb = puntlv + 1
          else
c
c     find  the first active problem with cost greater than lb
c
              pcorr = v(punta)
  220         sc2 = v(pcorr+ioffv)
              lb1 = v(pcorr+2)
              if ((lb.lt.lb1) .or. ((lb.eq.lb1).and. (sc1.ge.sc2))) then
c
c     insert the problem in queue
c
                  puntol = v(pcorr+1) - 1
                  v(puntol) = puntlv
                  v(puntlv) = pcorr
                  v(pcorr+1) = puntlv + 1
                  v(puntlv+1) = puntol + 1
              else
                  pcorr = v(pcorr)
                  go to 220
              end if
          end if
      end if
      puntlv = puntlv + ioffv + nnodi + 1
      return
      end
c *************************************************************************
      subroutine exque(ordv,v,ordx,x,fmvf,vd,f,p2,ngen,punta,nprob,
     +                 iva,ivb,ncodal,n,sc1,totass,ioffv)
      integer fmvf,ioffv,n,ncodal,ngen,nprob,ordv,ordx,p2,punta,sc1,
     +        totass
      integer f(n),iva(n),ivb(n),v(ordv),vd(n),x(ordx)
c     .. local scalars r32,i,i32,ix,j,jp
c     ..
      i32 = 32000
      r32 = 32000.
      p2 = punta
      ngen = float(v(punta+4))/r32
      nprob = v(punta+4) - ngen*i32
      totass = v(punta+2)
      jp = v(punta+5)
      j = 1
      do 100 i = 1,nprob
          ix = float(v(punta+ioffv+i))/r32
          iva(j) = ix
          ivb(j) = v(punta+ioffv+i) - ix*i32
          j = j + 1
  100 continue
c
c   load the new vectors vd and f
c
      do 200 i = 0,n - 1
          vd(i+1) = x(jp+i)
          f(i+1) = x(jp+n+i)
  200 continue
c
      x(jp) = fmvf
      fmvf = jp
      sc1 = v(punta+ioffv)
      punta = v(punta)
      ncodal = ncodal - 1
      return
      end
c *************************************************************************
      subroutine actpro(patt,n,nprobv,pnuovo,ngen,vimpa,vimpb,ordv,v,cr,
     +                  ordsp,ic,c,ioffv,inf)
      integer ordsp,ordv,patt,pnuovo,vimpa,vimpb
      integer c(ordsp),cr(n+1),ic(ordsp),v(ordv)
      integer p1,pold,pp
c  subroutines ..modmat
      m1 = -1
      p1 = 1
      i32 = 32000
      r32 = 32000.
c     update the cost matrix from the actual one(associated with patt)
c     to the new one(associated with pnuovo)
      if (v(pnuovo+3).ne.patt) then
c     the problem patt is not the father of pnuovo
c
c     "mark" the street from pnuovo to the root node
          pp = pnuovo
   50     if (pp.eq.0) then
              ngenol = nprobv
c      find a marked problem
   60         nodoa = float(v(patt + ioffv + ngenol))/r32
              nodob = v(patt + ioffv + ngenol) - nodoa*i32
              nodoc = vimpa
              nodod = vimpb
c     update the matrix from the one associated with the son of patt
c     to the one associated with patt
              call modmat(cr,ordsp,ic,c,nodoa,nodob,m1,inf)
              if (nodoc.ne.0) call modmat(cr,ordsp,ic,c,nodoc,nodod,m1,
     +                                    inf)
              if (v(patt+5).le.0) then
c     the actual matrix (associated with the problem patt)
c     is the one associated with common predecessor of pnew
c     and the first patt
c
c     pold : common predecessor problem
                  pp = pnuovo
                  pold = patt
c
   70             patt = v(pp+3)
                  ngenpp = float(v(pp+4))/r32
c     prohibit arcs
                  nodoa = float(v(patt + ioffv + ngenpp))/r32
                  nodob = v(patt + ioffv + ngenpp) - nodoa*i32
                  nodoc = float(v(pp+6))/r32
                  nodod = v(pp+6) - nodoc*i32
                  call modmat(cr,ordsp,ic,c,nodoa,nodob,p1,inf)
                  if (nodoc.ne.0) call modmat(cr,ordsp,ic,c,nodoc,nodod,
     +                                 p1,inf)
                  if (patt.eq.pold) then
c
c     update the marked problems
c
                      pp = pnuovo
   75                 v(pp+5) = -v(pp+5)
                      pp = v(pp+3)
                      if (pp.ne.0) go to 75
                  else
                      pp = patt
                      go to 70
                  end if
              else
                  ngenol = float(v(patt+4))/r32
                  vimpa = float(v(patt+6))/r32
                  vimpb = v(patt+6) - vimpa*i32
                  patt = v(patt+3)
                  go to 60
              end if
          else
c
              v(pp+5) = -v(pp+5)
              pp = v(pp+3)
              go to 50
          end if
      else
c
c     the problem patt is the father of pnuovo
c
          if (ngen.eq.nprobv) return
          nodoa = float(v(patt + ioffv + nprobv))/r32
          nodob = v(patt + ioffv + nprobv) - nodoa*i32
c
c      update the cost of the prohibited arc
c
          call modmat(cr,ordsp,ic,c,nodoa,nodob,m1,inf)
          nodoc = vimpa
          nodod = vimpb
c     update the cost of the ''implicit'' prohibited arc
          if (nodoc.ne.0) call modmat(cr,ordsp,ic,c,nodoc,nodod,m1,
     +                                inf)
          nodoa = float(v(patt + ioffv + ngen))/r32
          nodob = v(patt + ioffv + ngen) - nodoa*i32
c     prohibit the arc nodoa,nodob
          call modmat(cr,ordsp,ic,c,nodoa,nodob,p1,inf)
          nodoc = float(v(pnuovo+6))/r32
          nodod = v(pnuovo+6) - nodoc*i32
c     prohibit the ''implicit''arc
          if (nodoc.ne.0) call modmat(cr,ordsp,ic,c,nodoc,nodod,p1,
     +                                inf)
          return
      end if
      return
      end
c *************************************************************************
      subroutine genson(n,iva,ivb,vd,nprob,i,vimpa,vimpb,nodoim,cr,
     +                  ordsp,ic,c,inf)
      integer i,inf,n,nodoim,nprob,ordsp,vimpa,vimpb
      integer c(ordsp),cr(n+1),ic(ordsp),iva(n),ivb(n),vd(n)
      integer i2,ibm1,im1,m1,p1
c subroutines modmat
      m1 = -1
      p1 = 1
      vimpa = 0
      vimpb = 0
c      prohibit i-th arc
      call modmat(cr,ordsp,ic,c,iva(i),ivb(i),p1,inf)
      if (i.ne.1) then
          im1 = i - 1
          ibm1 = ivb(im1)
          i2 = ibm1
          call modmat(cr,ordsp,ic,c,iva(im1),i2,m1,inf)
          vd(ibm1) = vd(ibm1) - inf
c    don't prohibit 2 times the same arc
          if (nodoim.ne.ivb(i)) then
              i2 = nodoim
              call modmat(cr,ordsp,ic,c,iva(i),i2,p1,inf)
              vimpa = iva(i)
              vimpb = nodoim
          else
              vimpa = 0
              vimpb = 0
          end if
      else
          nodoim = ivb(nprob)
          return
      end if
c
c    if necessary adjust the 'implicit' prohibited arc
c
      i2 = nodoim
      if (i.ge.3) call modmat(cr,ordsp,ic,c,iva(im1),i2,m1,inf)
      return
      end
c *************************************************************************
      subroutine calcur(n,cr,ordsp,ic,c,f,ivai,vd,ur,inf)
c   compute the value of the dual variable associated with row ivai
      integer inf,ivai,n,ordsp,ur
      integer c(ordsp),cr(n+1),f(n),ic(ordsp),vd(n)
      integer jk,kl,pr,ul,vr
c     ..
      jk = f(ivai)
      vr = vd(jk)
      pr = cr(ivai)
      ul = cr(ivai+1) - 1
      do 100 kl = pr,ul
          if (ic(kl).eq.jk) then
              ur = c(kl) - vr - inf
              go to 200
          end if
  100 continue
  200 return
      end
c *************************************************************************
      subroutine calcud(n,ordsp,ic,cr,c,vc,fc,uc)
c  compute the current dual variables u
      integer n,ordsp
      integer c(ordsp),cr(n+1),fc(n),ic(ordsp),uc(n),vc(n)
      integer i,ifs,ii,ils,jk
c     ..
      do 100 i = 1,n
          jk = fc(i)
          ifs = cr(i)
          ils = cr(i+1) - 1
          do 50 ii = ifs,ils
              if (ic(ii).eq.jk) uc(i) = c(ii) - vc(jk)
   50     continue
  100 continue
      return
      end
c *************************************************************************
      subroutine modmat(cr,ordsp,ic,c,nodoa,nodob,flag,inf)
c     if flag=1  exclude arc (nodoa,nodob)
c     if flag=-1 remove exclusion constraint form arc (nodoa,nodob)
      integer flag,inf,nodoa,nodob,ordsp
      integer c(ordsp),cr(*),ic(ordsp)
      integer j,k,pr,ul
c     ..
      pr = cr(nodoa)
      ul = cr(nodoa+1) - 1
      do 100 j = pr,ul
          k = ic(j)
          if (k.eq.nodob) then
              c(j) = c(j) + flag*inf
              go to 200
          end if
  100 continue
  200 return
      end
c *************************************************************************
      subroutine creams(gap,a,n,cr,x,mm6,mm4,mm3,u,v,zeri,nlsp,err)
c     reduction procedure
c     store the sparse matrix in x() and  defines mm3,mm4
c     vector ic() is stored from mm3 and vector c() from mm4
      integer err,gap,mm3,mm4,mm6,n,nlsp,zeri
      integer a(n,n),cr(n+1),u(n),v(n),x(*)
      integer gap2,i,ia,iu,j,k,middle
c     ..
      zeri = 0
      err = 0
c
      mm4 = mm6
      middle = (mm4+mm3)/2.
      k = middle
      do 100 i = n,1,-1
          cr(i+1) = mm4
          iu = u(i)
          gap2 = gap + iu
          do 50 j = n,1,-1
              if (i.ne.j) then
                  ia = a(i,j) - v(j)
                  a(i,j) = ia - iu
                  if (ia.le.gap2) then
                      if (ia.eq.iu) zeri = zeri + 1
                      mm4 = mm4 - 1
                      if (mm4.le.middle) go to 500
                      x(k) = j
                      x(mm4) = ia - u(i)
                      k = k - 1
                  end if
              end if
   50     continue
  100 continue
      cr(1) = mm4
      do 200 i = 1,n + 1
          cr(i) = cr(i) - mm4 + 1
  200 continue
      nlsp = cr(n+1) - cr(1)
      k = middle
      mm3 = mm4 - 1
      do 300 i = nlsp,1,-1
          x(mm3) = x(k)
          k = k - 1
          mm3 = mm3 - 1
  300 continue
      mm3 = mm3 + 1
      do 400 i = 1,n
          u(i) = 0
          v(i) = 0
  400 continue
      return
  500 err = 1
      return
      end
c *************************************************************************
      subroutine errors(outerr,err)
      integer err,outerr
      outerr = 1
      if (err.ne.-1) then
         if (err.eq.1) call out('Insufficient memory, increase nstac')
         if (err.eq.2) call out('Insufficient memory, increase nstac')
         if (err.eq.3) call out('Insufficient memory, increase nstac')
         if (err.eq.4) call out('Insufficient memory, increase nstac')
         if (err.eq.5) call out('Insufficient memory, increase nstac')
         if (err.eq.6) then
            call out('solution not optimal. increase alpha')
            return
          end if
      else
      call out('maxnd nodes explored.solution not optimal')
          return
      end if
      call out('increase ordx')
      return
      end
c *************************************************************************
      subroutine setupo(n,a,u,v,m,ic,cr,kth,maxdim,iflag,inf)
c define the sparse matrix corresponding to the complete cost matrix a.
c the sparse matrix is stored through vectors ic,cr and matrix a.
      integer a(n,n),cr(n+1),ic(*),u(n),v(n)
c      real alpha,am,dd,aisum,ps,qq,ra,sum,th
c      integer i,ia,inf2,ips,j,jstep,l,lstep,nr
      data qq/2./
c     ..
      inf2 = float(inf)/2.
      iflag = 0
      lstep = 10
      jstep = n/lstep
      if (n.lt.lstep) jstep = 1
      nr = (n+jstep-1)/jstep
      aisum = 0.0
      do 100 j = 1,n,jstep
          aisum = aisum - v(j)
  100 continue
      aisum = aisum*n
      do 200 i = 1,n
          do 150 j = 1,n,jstep
              ia = a(i,j)
              if (ia.gt.inf2) then
                  aisum = aisum + u(i) + v(j)
              else
                  aisum = aisum + ia
              end if
  150     continue
          aisum = aisum - nr*u(i)
  200 continue
      sum = aisum
      dd = n*nr - nr
      alpha = sum/dd
      am = m
      ra = qq*alog(float(n))/am
      kth = alpha*ra + 0.5
c
      l = 1
      do 300 i = 1,n
          cr(i) = l
          th = kth + u(i)
          do 250 j = 1,n
              if (a(i,j)-v(j).le.th) then
                  ic(l) = j
                  l = l + 1
              end if
  250     continue
          if (l.gt.maxdim) go to 400
  300 continue
      cr(n+1) = l
      ps = 0.005
      if (n.le.950) ps = 0.01
      if (n.le.450) ps = 0.02
      if (n.le.250) ps = 0.03
      ips = ps*float(n*n)
      if (l.lt.ips) iflag = 1
      return
  400 cr(1) = -1
      return
      end

