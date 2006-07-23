      subroutine meshmesh(inseed,
     * nodes,nsorc,nsink,mincst,maxcst,itsup,otsup,
     * bhicst,bcap,mincap,maxcap,
     * nedge,type,tail,head,xnod,ynod,u,c,
     * ns,ns4,ns12,ns10,
     * cr,front,ic,nu,tri,k,ma,nsorcv,nsinkv)
c     
c      parameter (lfront0=10000,ns=10000)
c      parameter (ns4=4*(ns-1),ns12=12*(ns-1),na1=10*ns)
c
      parameter (ix0=10,iy0=43,idel=40,iwmax=12900,ihmax=12900)
      parameter (iw=985,ih=585)
c     maxnod = 104652
      parameter (maxnod=(iwmax/idel+1)*(ihmax/idel+1))
c
      integer type(*),tail(*),head(*),xnod(*),ynod(*),u(*),c(*)
c     
      doubleprecision cr(2,ns)
      integer front(ns), ic(2,ns), nu(ns12), tri(ns4)
      integer k(ns), ma(3,ns10)
      integer nsorcv(ns), nsinkv(ns)
      integer iflag(maxnod), iquad(maxnod,2)
c
      integer otsup
      integer iop, nbt, err
      integer iseed(2)
      dimension ia(35,2),ia5(35,2),bunny(35)
c     
      err=0
      nbt=0
      iop=0
      lfront=0
c     
      call getran(iseed,1,1,no,ran,non,ia,ia5,bunny,inseed)
      imp=6
      lec=5
c     
c     input checking
c     
      if(nodes.le.3)then
         call erro('The number of nodes must be greater than 3')
         return
      endif
      if(nsorc.le.0.or.nsink.le.0) then
         call erro('Bad source or sink number')
         return
      endif
      ntrans=nodes-nsorc-nsink
      if(ntrans.le.0) then
         call erro('Too much sources and sinks')
         return
      endif
      if(itsup.lt.nsorc) then
         call erro('Not enough supply')
         return
      endif
      if(otsup.lt.nsink) then
         call erro('Not enough demand')
         return
      endif
      if(bhicst.gt.100..or.bhicst.lt.0.) then
         call erro('Bad bhicst')
         return
      endif
      if(bcap.gt.100..or.bcap.lt.0.) then
         call erro('Bad bcap')
         return
      endif
c     
c     coordinates creation
c     
      iwn=iw/idel
      ihn=ih/idel
      if(nodes.gt.maxnod) then
        call erro('Too much nodes')
         return
      endif
      kk=0
c     try to have the whole mesh visible
      do 9,i=1,iwn+1
         do 9,j=1,ihn+1
            kk=kk+1
            iquad(kk,1)=ix0+idel*(i-1)
            iquad(kk,2)=iy0+idel*(j-1)
 9    continue
      if (kk.ge.nodes) goto 11
      do 12,i=iwn+2,iwmax/idel+1
         do 12,j=1,ihn+1
            kk=kk+1
            iquad(kk,1)=ix0+idel*(i-1)
            iquad(kk,2)=iy0+idel*(j-1)
 12   continue
      if (kk.ge.nodes) goto 11
      do 13,i=1,iwmax/idel+1
         do 13,j=ihn+2,iwmax/idel+1
            kk=kk+1
            iquad(kk,1)=ix0+idel*(i-1)
            iquad(kk,2)=iy0+idel*(j-1)
 13   continue
 11   continue
c      open(unit=55, file='mesh.dat',status='unknown')
c      write(55,2001)nodes
c      write(55,2003)
      do 300,i=1,nodes
         type(i)=0
 300  continue
      do 8,i=1,kk
         iflag(i)=0
 8    continue
      do 1,i=1,nodes
 7       call getran(iseed,2,1,no,ran,non,ia,ia5,bunny,inseed)
         jnod=(kk-1)*ran+1
         if (iflag(jnod).eq.1) goto 7
         iflag(jnod)=1
         ix=iquad(jnod,1)
         iy=iquad(jnod,2)
         cr(1,i)=ix
         cr(2,i)=iy
c         write(55,2000)i, ix, iy
         xnod(i)=ix
         ynod(i)=iy
 1    continue
      call mesh2b(nodes,ns12,ns4,lfront,cr,ic,nu,tri,front,nbt,err)
c     
c     arcs
c     
         lma=3*ns10
         call carete (nu,nbt,nodes,ma,na,naf,k,lma)
         nedge=na
c     
c     sources creation
c     
         do 4,i=1,nodes
            iflag(i)=0
 4       continue
         nsorcv(1)=1
         isorc=1
         do 10,i=2,nsorc
 40         isorc=isorc+1
            if(isorc.gt.nodes) then
               call erro('Impossible to create sources')
               return
            endif
            do 20,j=1,i-1
               kk=nsorcv(j)
               do 30,l=1,na
                  if(isorc.eq.ma(1,l).and.kk.eq.ma(2,l)) goto 40
                  if(isorc.eq.ma(2,l).and.kk.eq.ma(1,l)) goto 40
 30            continue
 20         continue
            nsorcv(i)=isorc
            iflag(isorc)=1
 10      continue
         do 8000,i=1,nsorc
            type(nsorcv(i))=2
 8000    continue
c     
c     sinks creation
c     
         nsinkv(1)=nodes
         isink=nodes
         do 50,i=2,nsink
 80         isink=isink-1
            if(isink.le.1) then
               call erro(' impossible to create sinks')
               return
            endif
            if(iflag(isink).eq.1) goto 80
            do 60,j=1,i-1
               kk=nsinkv(j)
               do 70,l=1,na
                  if(isink.eq.ma(1,l).and.kk.eq.ma(2,l)) goto 80
                  if(isink.eq.ma(2,l).and.kk.eq.ma(1,l)) goto 80
 70            continue
 60         continue
            nsinkv(i)=isink
 50      continue
         do 8001,i=1,nsink
            type(nsinkv(i))=1
 8001    continue
c     
c     costs
c     
         namc=na*(100.-bhicst)/100.
         do 5,i=1,na
            iflag(i)=0
 5       continue
         do 90,i=1,namc
 100        call getran(iseed,2,1,no,ran,non,ia,ia5,bunny,inseed)
            j=na*ran+1
            if(iflag(j).eq.1) goto 100
            c(j)=mincst
            iflag(j)=1
 90      continue
         do 110,i=1,na
            if(iflag(i).eq.1) goto 110
            call getran(iseed,2,1,no,ran,non,ia,ia5,bunny,inseed)
            c(i)=mincst+(maxcst-mincst+1)*ran
 110     continue
c     
c     capacities
c     
         nocap=max(itsup,otsup)
         nanc=na*(100.-bcap)/100.
         do 6,i=1,na
            iflag(i)=0
 6       continue
         do 120,i=1,nanc
 130        call getran(iseed,2,1,no,ran,non,ia,ia5,bunny,inseed)
            j=na*ran+1
            if(iflag(j).eq.1) goto 130
            u(j)=nocap
            iflag(j)=1
 120     continue
         do 140,i=1,na
            if(iflag(i).eq.1) goto 140
            call getran(iseed,2,1,no,ran,non,ia,ia5,bunny,inseed)
            u(i)=mincap+(maxcap-mincap+1)*ran
 140     continue
c     
c     node for sources
c     
         nodes1=nodes+1
         ircap=itsup
         do 150,i=1,nsorc
            na=na+1
            ma(1,na)=nodes1
            ma(2,na)=nsorcv(i)
            c(na)=0
            if(i.eq.nsorc) then
               u(na)=ircap
               goto 170
            endif
            call getran(iseed,2,1,no,ran,non,ia,ia5,bunny,inseed)
            icap=ircap*ran+1
            if(ircap-icap.lt.nsorc-i) then
               u(na)=ircap-nsorc+i
               do 160,j=i+1,nsorc
                  na=na+1
                  ma(1,na)=nodes1
                  ma(2,na)=nsorcv(j)
                  u(na)=1
                  c(na)=0
 160           continue
               goto 170
            else
               u(na)=icap
               ircap=ircap-icap
               goto 150
            endif
 150     continue
 170     continue
c     
c     node for sinks
c     
         nodes2=nodes+2
         ircap=otsup
         do 180,i=1,nsink
            na=na+1
            ma(2,na)=nodes2
            ma(1,na)=nsinkv(i)
            c(na)=0
            if(i.eq.nsink) then
               u(na)=ircap
               goto 200
            endif
            call getran(iseed,2,1,no,ran,non,ia,ia5,bunny,inseed)
            icap=ircap*ran+1
            if(ircap-icap.lt.nsink-i) then
               u(na)=ircap-nsink+i
               do 190,j=i+1,nsink
                  na=na+1
                  ma(2,na)=nodes2
                  ma(1,na)=nsinkv(j)
                  u(na)=1
                  c(na)=0
 190           continue
               goto 200
            else
               u(na)=icap
               ircap=ircap-icap
               goto 180
            endif
 180     continue
 200     continue
         do 3,i=1,na
c           don't save augmented graph
            if (ma(1,i).le.nodes.and.ma(2,i).le.nodes) then
               tail(i)=ma(1,i)
               head(i)=ma(2,i)
            endif
 3       continue

 2000    format(10i7)
 2001    format('  nodes'/i7)
 2002    format('triangs'/i7)
 2003    format('   node      x      y') 
 2004    format(' triang   node   node   node')
 2005    format('  narcs'/i7)
 2006    format('    arc  start    end   capa   cost')
 2007    format('sources')
 2008    format('  sinks')
 2009    format('  nsorc')
 2010    format('  nsink')
         end
