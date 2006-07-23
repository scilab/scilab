c the program is based on the paper  u. derigs "solving non-bipartite 
c matching problems via shortest path techniques" annals of operations
c research 7, 1988.  
c Author :
c ulrich derigs 
c lehrstuhl fur betriebswirtschaftslehre vii, 
c universitat bayreuth, postfach 3008,
c d-8580 bayreuth, germany.            
c ** *****************************************************************
      subroutine prfmatch(n,m,np1,m2,nbl,cc,index,cost,nmatch,
     + basis,mem,ka,kb,sm,tma,tmb,y1,y2,dminus,dplus)
c     ***     (min-cost perfect matching problem)              ***
      integer basis(n),mem(n),ka(n),kb(n),sm(n),tma(n),tmb(n)
      integer zfw,cost,nmatch(n),cc(m2),nbl(m2)
      integer index(np1)
      double precision  y1(n),y2(n),dminus(n),dplus(n),eps
      eps=10.**(-38)  
      sup=40000000
      cost=400000000
      do 1,i=1,n
         nmatch(i)=0
 1    continue
      call sap(n,m,cc,nbl,index,zfw,nmatch,basis,mem,ka,kb,sm,
     *tma,tmb,y1,y2,dplus,dminus,sup,eps)
      cost=zfw
c     zfw       cost of the optimal matching         
c     nmatch(n) optimal matching                     
      return
      end
      subroutine sap (n,m,cc,nbl,index,zfw,nmatch,basis,mem,ka,kb,
     f               sm,tma,tmb,y1,y2,dplus,dminus,sup,eps)
c ** *****************************************************************
c input:            
c     n    number of nodes (even)
c     m          number of edges                           
c     eps        machine accuracy                          
c     sup        sufficiently large real number            
c     nbl(2*m)   list of neighbours                        
c     cc(2*m)    cost of edges according to list of neighbours
c     index(i)   nbl(index(i)) start of neighbourlist of
c                vertex i (i=1,..,n) with index(n+1)=2*m+1 
c output:                                            
c     zfw       cost of the optimal matching         
c     nmatch(n) optimal matching                     
c     integer   arrays of length n:   basis,mem,ka,kb,sm,tma,tmb
c     real*8    arrays of length n:   y1,y2,dplus,dminus 
c     integer   array  of length n+1: index  
c     integer   array  of length 2*m: cc
c     integer*2 array of length 2*m:  nbl
c 5. external subroutines :      
c     augmnt expand grow ograph scan1 scan2 shrink start  
c ** *****************************************************************
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      integer    basis(1),mem(1),ka(1),kb(1),zfw,top,nmatch(1)
      integer    cc(1),sm(1),tma(1),tmb(1),index(1)
      integer  nbl(1)
      double precision y1(1),y2(1),dminus(1),dplus(1),c0,d,dbest,y1b,
     *                 y2b,eps
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** start
          top =n+2
          n2  =n/2
      call start(n,ncard,top,cc,nbl,index,nmatch,y1)
      if (ncard.eq.n2)                 goto 700
      do 100      n1=1,n
          basis(n1) =n1
          mem(n1)   =n1
          y2(n1)    =0.
          sm(n1)    =top
          tma(n1)   =top
          tmb(n1)   =top
          dplus(n1) =sup
          dminus(n1)=sup
          ka(n1)    =0
          kb(n1)    =n1
  100 continue
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** initialization
          nn        =0
      do 110      ni=1,n
      if (nmatch(ni).ne.top)            goto 110
          nn        =nn+1
          sm(ni)    =0
          dplus(ni) =0.
 110  continue
      if (nn.le.1)                      goto 700
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** determination of the new dminus-values
 120  do 140      n1=1,n
          nb1       =basis(n1)
      if (sm(nb1).ne.0)                 goto 140
          y1b       =y1(nb1)
          y2b       =y2(n1)
          i1        =index(n1)
          i2        =index(n1+1)-1
      do 130      i3=i1,i2
          n2        =nbl(i3)
          nb2       =basis(n2)
      if (nb1.eq.nb2)                   goto 130
          nc        =cc(i3)
          c0        =dfloat(nc)-y1b-y2b
          c0        =c0-y1(nb2)-y2(n2)
      if (c0.ge.dminus(nb2))            goto 130
          ka(nb2)   =n1
          kb(nb2)   =n2
          dminus(nb2)=c0
 130  continue
 140  continue
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** control-routine of the procedure
 200   continue
          dbest     =sup
      do 215      nb=1,n
      if (basis(nb).ne.nb)              goto 215
          d         =dminus(nb)
      if (sm(nb).ge.top)                goto 205
          d         =.5*(d+dplus(nb))
      if (d.gt.dbest)                   goto 215
          nbest     =nb
          dbest     =d
      g o t o 215
 205  if (tma(nb).ge.top)               goto 210
      if (mem(nb).eq.nb)                goto 215
          d         =d+y1(nb)
      if (d.ge.dbest)                   goto 215
           nbest    =nb
          dbest     =d
      g o t o 215
  210 continue
      if (d.ge.dbest)                   goto 215
          nbest     =nb
          dbest     =d
  215 continue
      if (tma(nbest).ge.top) goto 217
             call bexpand(n,m,top,nmatch,cc,basis,mem,ka,kb,sm,tma,tmb,
     f                y1,y2,dplus,dminus,sup,eps,nbl,index,nbest,dbest)
      goto 200
  217 if (sm (nbest).lt.top) goto 218
                 call grow(n,top,nmatch,cc,basis,mem,ka,kb,sm,tma,tmb,
     f                y1,y2,dplus,dminus,sup,eps,nbl,index,nbest,dbest)
      goto 200
  218     nka       =ka(nbest)
          nkb       =kb(nbest)
          n1        =nbest
          nb1       =n1
          n2        =basis(nka)
          nb2       =n2
  220     tma(nb1)  =nb2
          nk        =sm(nb1)
      if (nk.eq.0)                      goto 225
          nb2       =basis(nk)
          nb1       =tma(nb2)
          nb1       =basis(nb1)
      g o t o 220
  225     nb        =nb1
          nb1       =n2
          nb2       =n1
  230 if (tma(nb1).lt.top)              goto 235
          tma(nb1)  =nb2
          nk        =sm(nb1)
      if (nk.ne.0) goto 232
               call augmnt(n,top,nmatch,cc,basis,mem,ka,kb,sm,tma,tmb,
     f                y1,y2,dplus,dminus,sup,eps,nbl,index,dbest,n1,n2,
     f                nka,nkb,ncard,j700)
      if ( j700 .eq. 1 ) goto 700
      goto 120
  232     nb2       =basis(nk)
          nb1       =tma(nb2)
          nb1       =basis(nb1)
      g o t o 230
  235 if (nb1.ne.nb) goto 240
               call shrink(n,top,nmatch,cc,basis,mem,ka,kb,sm,tma,tmb,
     f                y1,y2,dplus,dminus,sup,eps,nbl,index,nbest,dbest,
     f                nb,n1,n2,nb2,nka,nkb)
      goto 200
  240     nk        =tma(nb)
          tma(nb)   =top
          nm        =nmatch(nk)
          nb        =basis(nm)
      g o t o 235
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** generation of the original graph by expansion of all
c     shrunken blossoms
  700 call ograph(n,zfw,eps,index,nbl,cc,sm,tma,tmb,nmatch,mem,
     f                  basis,ka,kb,dplus,dminus,y1,y2)
      return
      end
c************************************************************************
      subroutine augmnt(n,top,nmatch,cc,basis,mem,ka,kb,sm,tma,tmb,
     f                y1,y2,dplus,dminus,sup,eps,nbl,index,dbest,n1,n2,
     f                nka,nkb,ncard,jret1)
      integer    basis(1),mem(1),ka(1),kb(1)
      integer    cc(1),sm(1),tma(1),tmb(1),index(1)
      integer    top,nmatch(1),nbl(1)
      double precision y1(1),y2(1),dminus(1),dplus(1),d,dbest,eps
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** augmentation of the matching
c exchange of the matching- and non-matching-edges along the augmenting path
          jret1 = 0
          nb        =n1
          nk        =nka
  605     nb1       =nb
  606     nmatch(nb1)=nk
          nk        =sm(nb1)
          tma(nb1)  =top
      if (nk.eq.0)                      goto 607
          nb2       =basis(nk)
          nk1       =tma(nb2)
          nk        =tmb(nb2)
          nb1       =basis(nk1)
          nmatch(nb2)=nk1
      g o t o 606
  607 if (nb.ne.n1)                     goto 608
          nb        =n2
          nk        =nkb
      g o t o 605
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** removing all labels on non-exposed base nodes
c
  608 continue
      do 620      nb=1,n
      if (basis(nb).ne.nb)              goto 620
      if (sm(nb).ge.top)                goto 610
          d         =dbest-dplus(nb)
          y1(nb)    =y1(nb)+d
          sm(nb)    =top
      if (nmatch(nb).ne.top)            goto 615
          sm(nb)    =0
          dplus(nb) =0.
      g o t o 616
  610 if (tma(nb).ge.top)               goto 615
          d         =dminus(nb)-dbest
          y1(nb)    =y1(nb)+d
          tma(nb)   =top
          tmb(nb)   =top
  615     dplus(nb) =sup
  616     dminus(nb)=sup
  620 continue
      ncard=ncard+1
      ndiff=n-2*ncard
      if(ndiff.gt.1) return
      jret1=1
      return
      end
c*************************************************************************
      subroutine bexpand(n,m,top,nmatch,cc,basis,mem,ka,kb,sm,tma,tmb,
     f     y1,y2,dplus,dminus,sup,eps,nbl,index,nbest,dbest)
      integer    basis(1),mem(1),ka(1),kb(1)
      integer    cc(1),sm(1),tma(1),tmb(1),index(1)
      integer    top,nmatch(1)
      integer  nbl(1)
      double precision y1(1),y2(1),dminus(1),dplus(1),dbest,y1b,eps
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** expansion of a t-labeled blossom
c
  500 continue
          n1        =mem(nbest)
          nb3       =n1
          nka       =ka(n1)
          nk2       =n1
  505     nk1       =nk2
          nkb       =kb(nk1)
          y1b       =y1(nk1)
  510     basis(nk2)=nk1
          y2(nk2)   =y2(nk2)-y1b
      if (nk2.eq.nkb)                   goto 515
          nk2       =mem(nk2)
      goto 510
  515     nk2       =mem(nkb)
          mem(nkb)  =nk1
      if (nk2.ne.nka)                   goto 505
          y1b       =dplus(n1)
          y1(nbest) =y1b
          mem(nbest)=nka
          nk2       =nka
  520     y2(nk2)   =y2(nk2)-y1b
      if (nk2.eq.nbest)                 goto 525
          nk2       =mem(nk2)
      goto 520
  525 continue
          nk1       =nmatch(nbest)
          nb1       =basis(nk1)
          nk2       =sm(nb1)
          nb        =basis(nk2)
      if (nb.eq.nbest)                  goto 545
          nb2       =nb
  530     nk        =tma(nb2)
          nb1       =basis(nk)
      if (nb1.eq.nbest)                 goto 535
          nb2       =sm(nb1)
          nb2       =basis(nb2)
      goto 530
  535     tma(nb)   =tma(nbest)
          tma(nbest)=tmb(nb2)
          tmb(nb)   =tmb(nbest)
          tmb(nbest)=nk
          nk3       =sm(nb)
          nb3       =basis(nk3)
          nk4       =sm(nb3)
          sm(nb)    =top
          nmatch(nb)=nk1
          nb1       =nb3
  540     nk1       =tma(nb1)
          nk2       =tmb(nb1)
          tma(nb1)  =nk4
          tmb(nb1)  =nk3
          sm(nb1)   =nk1
          nmatch(nb1)=nk1
          nb2       =basis(nk1)
          nmatch(nb2)=nk2
          nk3       =sm(nb2)
          sm(nb2)   =nk2
      if (nb2.eq.nbest)                 goto 545
          nb1       =basis(nk3)
          nk4       =sm(nb1)
          tma(nb2)  =nk3
          tmb(nb2)  =nk4
      goto 540
  545 continue
          nk2       =tmb(nb)
          nb1       =basis(nk2)
          dminus(nb1)=dbest
          n1        =0
      if (nb1.eq.nb)                    goto 555
          nk1       =tma(nb1)
          nb3       =basis(nk1)
          tma(nb1)  =tma(nb)
          tmb(nb1)  =nk2
  550     nk        =sm(nb1)
          sm(nb1)   =top
          nb2       =basis(nk)
          nk        =tma(nb2)
          tma(nb2)  =top
          n2        =tmb(nb2)
          tmb(nb2)  =n1
          n1        =nb2
          dplus(nb2)=dbest
          nb1       =basis(nk)
          dminus(nb1)=dbest
      if (nb1.ne.nb)                    goto 550
          tma(nb)   =n2
          tmb(nb)   =nk
          sm(nb)    =top
      if (nb3.eq.nb)                    goto 570
  555     nb1       =0
          nb2       =nb3
  560     nk        =sm(nb2)
          sm(nb2)   =top
          tma(nb2)  =top
          tmb(nb2)  =nb1
          nb1       =basis(nk)
          nk        =tma(nb1)
          sm(nb1)   =top
          tma(nb1)  =top
          tmb(nb1)  =nb2
          nb2       =basis(nk)
      if (nb2.ne.nb)                    goto 560
      call scan2(nb1,n,sup,cc,basis,mem,ka,kb,sm,tma,tmb,
     *            y1,y2,dplus,dminus,nbl,index)
  570 continue
  575 if (n1.eq.0)                      return
          nb        =n1
      call scan1(nb,n,sup,cc,basis,mem,ka,kb,sm,tma,tmb,
     *            y1,y2,dplus,dminus,nbl,index)
          n1        =tmb(nb)
          tmb(nb)   =top
      g o t o 575
      end
c***********************************************************************
      subroutine grow (n,top,nmatch,cc,basis,mem,ka,kb,sm,tma,tmb,y1,
     f                y2,dplus,dminus,sup,eps,nbl,index,nbest,dbest)
c
      integer    basis(1),mem(1),ka(1),kb(1)
      integer    cc(1),sm(1),tma(1),tmb(1),index(1)
      integer    top,nmatch(1)
      integer  nbl(1)
      double precision y1(1),y2(1),dminus(1),dplus(1),dbest,eps
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** growing an alternating tree by adding two edges
c
          tma(nbest)=ka(nbest)
          tmb(nbest)=kb(nbest)
          nm        =nmatch(nbest)
          nmb       =basis(nm)
          dplus(nmb)=dbest
          sm(nmb)   =nmatch(nmb)
      call scan1(nmb,n,sup,cc,basis,mem,ka,kb,sm,tma,tmb,
     *            y1,y2,dplus,dminus,nbl,index)
      return
      end
c***********************************************************************
      subroutine ograph(n,zfw,eps,index,nbl,cc,sm,tma,tmb,nmatch,mem,
     1                  basis,ka,kb,dplus,dminus,y1,y2)
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      integer    index(1),cc(1),sm(1),tma(1),tmb(1),n,zfw,
     f           nmatch(1),mem(1),basis(1),ka(1),kb(1)
      integer  nbl(1)
      double precision dplus(1),dminus(1),y1(1),y2(1),d,yb,
     f                 y1b,eps
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** generation of the original graph by expansion of all
c     shrunken blossoms
c
          zfw       =0
      do 702     nb1=1,n
      if (basis(nb1).ne.nb1)            goto 702
      if (sm(nb1).lt.0)                 goto 702
          n2        =nmatch(nb1)
          nb2       =basis(n2)
          n1        =nmatch(nb2)
          sm(nb1)   =-1
          sm(nb2)   =-1
          i1        =index(n1)
          i2        =index(n1+1)-1
          do 9000 i3=i1,i2
          if(nbl(i3).eq.n2) go to 9001
 9000     continue
 9001     nc        =cc(i3)
          d         =dfloat(nc)-y1(nb1)-y1(nb2)
          d         =d-y2(n1)-y2(n2)
      if (dabs(d).gt.eps) continue    
c see n1,n2,d
          zfw       =zfw+nc
  702 continue
      do 750      n1=1,n
  705     nb        =basis(n1)
      if (nb.eq.n1)                     goto 750
          nk2       =mem(nb)
          nka       =ka(nk2)
          nb3       =nk2
          yb        =dplus(nk2)
  710     nk1       =nk2
          nkb       =kb(nk1)
          y1b       =y1(nk1)
  715     basis(nk2)=nk1
          y2(nk2)   =y2(nk2)-y1b
      if (nk2.eq.nkb)                   goto 720
          nk2       =mem(nk2)
      goto 715
  720     nk2       =mem(nkb)
          mem(nkb)  =nk1
      if (nk2.ne.nka)                   goto 710
          y1(nb)    =yb
          mem(nb)   =nka
          nk2       =nka
  725     y2(nk2)   =y2(nk2)-yb
      if (nk2.eq.nb)                    goto 730
          nk2       =mem(nk2)
      goto 725
  730     nk        =nmatch(nb)
          nk1       =basis(nk)
          nk1       =nmatch(nk1)
          nb1       =basis(nk1)
      if (nb.eq.nb1)                    goto 745
          nmatch(nb1)=nk
          nb3       =tma(nb1)
          nb3       =basis(nb3)
  735     nk3       =sm(nb1)
          nb2       =basis(nk3)
          nk1       =tma(nb2)
          nk2       =tmb(nb2)
          nb1       =basis(nk1)
          nmatch(nb1)=nk2
          nmatch(nb2)=nk1
          i1        =index(nk1)
          i2        =index(nk1+1)-1
          do 9002 i3=i1,i2
          if(nbl(i3).eq.nk2) go to 9003
 9002     continue
 9003     nc        =cc(i3)
          d         =dfloat(nc)-y1(nb1)-y1(nb2)
          d         =d-y2(nk1)-y2(nk2)
      if (dabs(d).gt.eps) continue    
c see nk1,nk2,d
          zfw       =zfw+nc
      if (nb1.ne.nb)                    goto 735
  740 if (nb3.eq.nb)                    goto 705
  745     n2        =sm(nb3)
          nb2       =basis(n2)
          n3        =sm(nb2)
          i1        =index(n2)
          i2        =index(n2+1)-1
          do 9004 i3=i1,i2
          if(nbl(i3).eq.n3) go to 9005
 9004     continue
 9005     nc        =cc(i3)
          d         =dfloat(nc)-y1(nb2)-y1(nb3)
          d         =d-y2(n2)-y2(n3)
      if (dabs(d).gt.eps) continue    
c see n2,n3,d
          zfw       =zfw+nc
          n3        =tma(nb2)
          nb3       =basis(n3)
      goto 740
  750 continue
      return
      end
c************************************************************************
      subroutine scan1(nb1,n,sup,cc,basis,mem,ka,kb,sm,tma,tmb,
     *                  y1,y2,dplus,dminus,nbl,index)
      integer    n,top,basis(n),mem(n),ka(n),kb(n)
      integer    cc(1),sm(1),tma(1),tmb(1),index(1), nbl(1)
      double precision     y1(1),y2(1),dplus(1),dminus(1),d1,d2,c0
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** scanning of node nb1
          top       =n+2
          d1        =dplus(nb1)-y1(nb1)
          dminus(nb1)=sup
          d2        =d1-y2(nb1)
          tma(nb1)  =0
          ia        =index(nb1)
          ib        =index(nb1+1)-1
      do 300      ic=ia,ib
          n2        =nbl(ic)
          nb2       =basis(n2)
      if (tma(nb2).lt.top)              goto 300
          nc        =cc(ic)
          c0        =dfloat(nc)+d2
          c0        =c0-y1(nb2)-y2(n2)
      if (c0.ge.dminus(nb2))            goto 300
          ka(nb2)   =nb1
          kb(nb2)   =n2
          dminus(nb2)=c0
  300 continue
          n1        =nb1
      g o t o 315
  305     d2        =d1-y2(n1)
          i1        =index(n1)
          i2        =index(n1+1)-1
      do 310      i3=i1,i2
          n2        =nbl(i3)
          nb2       =basis(n2)
      if (tma(nb2).lt.top) go to 310
          nc        =cc(i3)
          c0        =dfloat(nc)+d2
          c0        =c0-y1(nb2)-y2(n2)
      if (c0.ge.dminus(nb2))            goto 310
          ka(nb2)   =n1
          kb(nb2)   =n2
          dminus(nb2)=c0
  310 continue
  315     n1        =mem(n1)
      if (n1.ne.nb1)                    goto 305
          tma(nb1)  =top
      return
      end
      subroutine scan2(nb,n,sup,cc,basis,mem,ka,kb,sm,tma,tmb,
     *                  y1,y2,dplus,dminus,nbl,index)
      integer    n,top
      integer    basis(n),mem(n),ka(n),kb(n)
      integer    cc(1),sm(1),tma(1),tmb(1),index(1)
      integer  nbl(1)
      double precision     y1(1),y2(1),dminus(1),dplus(1),d,c0,y1b,
     f                     y2b
c
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** scanning of node nb
c
          top       =n+2
  300     nb1       =nb
          nb        =tmb(nb1)
          tmb(nb1)  =top
          d         =sup
          nka       =0
          nkb       =0
          n1        =nb1
          y1b       =y1(nb1)
  315 continue
          y2b       =y2(n1)
          i1        =index(n1)
          i2        =index(n1+1)-1
      do 320      i3=i1,i2
          n2        =nbl(i3)
          nb2       =basis(n2)
      if (sm(nb2).ge.top)               goto 320
          nc        =cc(i3)
          c0        =dfloat(nc)-y1b-y2b
          c0        =c0-y1(nb2)-y2(n2)
          c0        =c0+dplus(nb2)
      if (c0.ge.d)                      goto 320
          nka       =n2
          nkb       =n1
          d         =c0
  320 continue
          n1        =mem(n1)
      if (n1.ne.nb1)                    goto 315
          ka(nb1)   =nka
          kb(nb1)   =nkb
          dminus(nb1)=d
      if (nb.ne.0)                      goto 300
      return
      end
c***********************************************************************
      subroutine shrink (n,top,nmatch,cc,basis,mem,ka,kb,sm,tma,tmb,
     f                y1,y2,dplus,dminus,sup,eps,nbl,index,nbest,dbest,
     f                nb,n1,n2,nb2,nka,nkb)
      integer    basis(1),mem(1),ka(1),kb(1)
      integer    cc(1),sm(1),tma(1),tmb(1),index(1)
      integer    top,nmatch(1)
      integer  nbl(1)
      double precision y1(1),y2(1),dminus(1),dplus(1),dbest,y1b,
     f                 yb,eps
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
c *** shrinking a blossom
c
  400 continue
          yb        =y1(nb)+dbest-dplus(nb)
          y1(nb)    =0.
          nk1       =nb
  430     y2(nk1)   =y2(nk1)+yb
          nk1       =mem(nk1)
      if (nk1.ne.nb)                    goto 430
          nk        =mem(nb)
      if (nb.ne.n2)                     goto 436
  435     n2        =n1
          nb2       =tma(nb)
  436     mem(nk1)  =nb2
          nm        =nmatch(nb2)
          sm(nb2)   =nm
          y1b       =y1(nb2)+dminus(nb2)-dbest
          nk1       =nb2
  440     nk2       =nk1
          y2(nk2)   =y2(nk2)+y1b
          basis(nk2)=nb
          nk1       =mem(nk2)
      if (nk1.ne.nb2)                   goto 440
          kb(nb2)   =nk2
          y1(nb2)   =y1b
          nb1       =basis(nm)
          mem(nk2)  =nb1
          y1b       =y1(nb1)+dbest-dplus(nb1)
          nk2       =nb1
  445     nk1       =nk2
          y2(nk1)   =y2(nk1)+y1b
          basis(nk1)=nb
          nk2       =mem(nk1)
      if (nk2.ne.nb1)                   goto 445
          kb(nb1)   =nk1
          y1(nb1)   =y1b
      if (n2.eq.nb1)                    goto 450
          nb2       =tma(nb1)
          tma(nb1)  =tmb(nb2)
          tmb(nb1)  =tma(nb2)
      goto 436
  450 if (n2.eq.nbest)                  goto 455
          tma(n2)   =nkb
          tmb(n2)   =nka
      if (nb.ne.nbest)                  goto 435
      goto 460
  455     tma(nbest)=nka
          tmb(nbest)=nkb
  460     mem(nk1)  =nk
          n1        =mem(nb)
          ka(n1)    =nk
          dplus(n1) =yb
          tma(nb)   =top
          dplus(nb) =dbest
      call scan1(nb,n,sup,cc,basis,mem,ka,kb,sm,tma,tmb,
     *            y1,y2,dplus,dminus,nbl,index)
      return
      end
      subroutine start(n,ncard,top,ce,nb,index,nmatch,y1)
c *** ****************************************************************
c     * determination of an initial partial matching and a dual      *
c     * solution for starting the shortest augmenting path code      *
c *** ****************************************************************
c     *  1.  call:                                                   *
c     *      call start(n,ncard,top,ce,nb,index,nmatch,y1)           *
c     *  3.  method:                                                 *
c     *      - 1-saturated matching via greedy                       *
c     *  4.  parameters:                                             *
c     *      input:                                                  *
c     *         n           number of nodes                          *
c     *         top         = n+2                                    *
c     *         nb(.)       list of neighbours                       *
c     *         ce(.)       costs of edges according to list of      *
c     *                       neighbours                             *
c     *         index(i)    nb(index(i)) first neighbour of          *
c     *                       vertex i                               *
c     *         index(n+1)  = n*(n-1)+1   ( for complete graphs )    *
c     *      output:                                                 *
c     *         nmatch(.)   initial partial matching                 *
c     *         y1(.)       initial dual solution                    *
c     *         ncard       cardinality of partial matching          *
c     *                                                              *
c     *      integer array of length n :                             *
c     *         nmatch                                               *
c     *                                                              *
c     *      real*8 array of length n :                              *
c     *         y1                                                   *
c     *                                                              *
c     *      integer array of length n+1 :                           *
c     *         index                                                *
c     *                                                              *
c     *      integer*2 array of length n*(n-1) :                     *
c     *         nb                                                   *
c     *                                                              *
c     *      integer array of length n*(n-1) :                       *
c     *         ce                                                   *
c *** ****************************************************************
c
c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      integer    ce(1),cq,index(1),nmatch(1),top
      integer  nb(1)
      double precision     d,dd,y1(1)
      do  10  i=1,n
   10 nmatch(i)=top
      jjce=index(1)
      cq=ce(jjce)
      n1=index(n)-1
      do 100  j=1,n1
         if(cq.gt.ce(j))               cq=ce(j)
  100 continue
      d=dfloat(cq)/2.
      do 110  i=1,n
  110 y1(i)=d
      ncard=0
      do 150  i=1,n
         if(nmatch(i).lt.top)          goto 150
            n1=0
            n2=index(i)
            n3=index(i+1)-1
            jjnb=nb(n2)
            d=dfloat(ce(n2))-y1(jjnb)
            do 130  ik=n2,n3
               j=nb(ik)
               dd=dfloat(ce(ik))-y1(j)
               if(dd.ge.d)             goto 120
                  n1=j
                  d=dd
                  goto 130
  120          if(dd.gt.d)             goto 130
                  if(nmatch(j).lt.top) goto 130
                     n1=j
  130       continue
            if(n1.eq.0)                goto 140
               if(nmatch(n1).lt.top)   goto 140
                  nmatch(i) =n1
                  nmatch(n1)=i
                  ncard=ncard+1
  140    y1(i)=d
  150 continue
      return
      end

