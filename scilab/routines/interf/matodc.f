      subroutine  matodc
c ====================================================================
c     simulation non lineaire hybride
c ====================================================================
c     Copyright INRIA
      include '../stack.h'
      integer iadr,sadr
c
c     common de lsode,lsoda,lsodar
      double precision xxxx,yyyy,rlsr
      integer ilsr
      common/ls0001/xxxx(219),iiii(39)
      common/lsa001/yyyy(22),jjjj(9)
      common/lsr001/ rlsr(5),ilsr(9)
      common/eh0001/kkkk(2)
      save /ls0001/,/lsa001/,/lsr001/,/eh0001/
c
c     commons avec bydot,bjac,....
c
      character*(nlgh+1) namef,namej,names
      common/cydot/namef
      common/cjac/namej
      common/csurf/names
      common/ierajf/iero
      common/odecd/nd,iflag
c
c
      double precision atol,rtol,t0,tleft,tright,tf,hf
      double precision h0,hmax,hmin,tcrit,tmax,hstep,delta
      integer meth,jactyp
      integer top1,top2,tope,hsize
      logical jaco,achaud,withw,single,update
      external bydot2,bjac,bsurf
      integer raide(2),root(2),adams,discre,rgk(2),rk(2),fix(2)
      integer params(nsiz)
      data raide/28,29/,adams/10/,root/27,24/
      data discre/13/,rgk/27,16/,rk/27,20/,fix/15,18/
      data params/-235739080,-303896856,669247720,3*673720360/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matodc '//buf(1:4))
      endif

c
      top2=top+1-rhs
c
c     get %ODEOPTIONS variable
      ifin=fin
      fin=-1
      istate=1
      call stackg(params)
      if(fin.eq.0) then
c         call msgs(72,0)
         iopt=0
         itask=4
         jactyp=2
         ml=-1
         mu=-1
      else
         iopt=1
         il=iadr(lstk(fin))
         l=sadr(il+4)
c     %ODEOPTIONS=[itask,tcrit,h0,hmax,hmin,jactyp,mxstep,..
c                                  mxordn,mxords,ixpr, ml,mu]
         itask=int(stk(l))
         if(itask.ne.4.and.itask.ne.5) then
            call msgs(78,0)
            if(itask.eq.1) then
               itask=4
            else
               itask=5
            endif
         endif
         tcrit=stk(l+1)
         h0=stk(l+2)
         hmax=stk(l+3)
         hmin=stk(l+4)
         if(hmin.gt.hmax) then
            buf=' what? hmin greater than hmax?'
            call error(9999)
            return
         endif
         jactyp=int(stk(l+5))
         if(jactyp.lt.0.or.jactyp.gt.5) then
            buf=' invalid option (entry no 4)'
            call error(9999)
            return
         endif
         mxstep=int(stk(l+6))
         mxordn=int(stk(l+7))
         mxords=int(stk(l+8))
         ixpr=int(stk(l+9))
         ml=int(stk(l+10))
         mu=int(stk(l+11))
      endif
c      .....
c
      fin=ifin
      withw=.false.

      tope=top
      itype=0

      nd=0
      iflag=0      
c
c     odedc
c
      if(rhs.lt.4) then
         call error(39)
         return
      endif
c
c     lw=premiere adresse libre dans la pile
      lw=lstk(top+1)
c
c     test demarrage a chaud
      ifin=iadr(lstk(top))
      achaud=istk(ifin).eq.1
      if(achaud) then
c     ilc=adresse of lsod* integer work space  
         il=iadr(lstk(top))
         if(istk(il).ne.1) then
            err=rhs
            call error(53)
            return
         endif
         liwp=istk(il+2)*istk(il+1)
         lci=sadr(il+4)
         ilc=iadr(lci)
c     lc=adresse of lsod* real work space  
         il=iadr(lstk(top-1))
         if(istk(il).ne.1) then
            err=rhs-1
            call error(53)
            return
         endif
         lc=sadr(il+4)
         lrwp=istk(il+1)*istk(il+2)
c         if(meth.le.3) then
c     check for input hot start table consistency
c            if(lrwp.ne.lrw+nsizd) then
c               buf=' Real hot start table has incorrect size'
c               call error(9999)
c               return
c            endif
c            if(liwp.ne.liw+nsizi) then
c               buf=' Input hot start table has incorrect size'
c               call error(9999)
c               return
c            endif
c         endif
      endif
c
      ile=iadr(lstk(top2))
c
      if(istk(ile).eq.10) then
         top2=top2+1
         if(abs(istk(ile+6)).eq.adams) then
c     lsode (adams)
            meth=1
         elseif(abs(istk(ile+6)).eq.raide(1) .and.
     $           abs(istk(ile+7)).eq.raide(2)) then
c     lsode (gear)
            meth=2
         elseif(abs(istk(ile+6)).eq.root(1) .and.
     $           abs(istk(ile+7)).eq.root(2)) then
c     lsodar
            meth=3
         elseif(abs(istk(ile+6)).eq.discre) then
c     ldisc
            meth=4
         elseif(abs(istk(ile+6)).eq.rgk(1) .and.
     $           abs(istk(ile+7)).eq.rgk(2)) then
c     runge-kutta
            meth=5
         elseif(abs(istk(ile+6)).eq.rk(1) .and.
     $           abs(istk(ile+7)).eq.rk(2)) then
c     rkf45
            meth=6
         elseif(abs(istk(ile+6)).eq.fix(1) .and.
     $           abs(istk(ile+7)).eq.fix(2)) then
c     rksimp
            meth=7
         else
            call error(42)
            return
         endif
      else
c     lsoda
         meth=0
      endif
c
      if(meth.lt.3) then
         if(lhs.ne.3.and.lhs.ne.1) then
            call error(41)
            return
         endif
      elseif(meth.eq.3) then
         if(lhs.ne.2.and.lhs.ne.4) then
            call error(41)
            return
         endif
      elseif(meth.ge.4) then
         if(lhs.ne.1) then
            call error(41)
            return
         endif
      endif
c
      top1=top
      if(achaud) top1=top-2
c     top1 points to last external
c
      if(meth.eq.3) then
c     on recupere le simulateur des equations des surfaces
         ilsurf=iadr(lstk(top1))
         if(istk(ilsurf).ne.10.and.istk(ilsurf).ne.15.and.
     $        istk(ilsurf).ne.11.and.istk(ilsurf).ne.13) then
            err=rhs-(tope-top1)
            call error(80)
            return
         endif
         if(istk(ilsurf).eq.10) then
            names=' '
            call cvstr(istk(ilsurf+5)-1,istk(ilsurf+6),names,1)
            names(istk(ilsurf+5):istk(ilsurf+5))=char(0)
            call setfsurf(names,irep)
            if ( irep.eq.1) then 
               buf = names
               call error(50)
               return
            endif
         endif
         ksurf=top1
         top1=top1-1
c     ... et le nombre d'equations
         il=iadr(lstk(top1))
         if(istk(il).ne.1) then
            err=rhs-(tope-top1)
            call error(53)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=rhs-(tope-top1)
            call error(89)
            return
         endif
         l=sadr(il+4)
         nsurf=stk(l)
         top1=top1-1
      else
         ksurf=0
      endif
      
      il=iadr(lstk(top1-1))
      if(istk(il).eq.10.or.istk(il).eq.15.or.
     $     istk(il).eq.11.or.istk(il).eq.13) then
c     JACOBIAN IS GIVEN (variable top1)
         ilj=iadr(lstk(top1))
         islj=istk(ilj)
         if(islj.lt.10.or.islj.gt.15.or.islj.eq.12) then
            err=rhs-(tope-top1)
            call error(80)
            return
         endif
         if(islj.eq.10) then
            namej=' '
            call cvstr(istk(ilj+5)-1,istk(ilj+6),namej,1)
            namej(istk(ilj+5):istk(ilj+5))=char(0)
            call setfjac(namej,irep)
            if ( irep.eq.1) then 
               buf = namej
               call error(50)
               return
            endif
         endif
         if (meth.ge.4) then
            call msgs(75,0)
         endif
         if(jactyp.eq.0.and.(meth.eq.2.or.meth.eq.1)) then
            call msgs(75,0)
         endif
         jaco=.true.
         if(iopt.eq.0) then
c     set jactyp (jacobian is supposed full)
            jactyp=1
         else 
c     check jactyp
            if(jactyp.eq.2.or.jactyp.eq.5) then
               call msgs(75,0)
            endif
         endif
         kjac=top1
         top1=top1-1
      else
c     JACOBIAN NOT GIVEN
         if(iopt.eq.0) then
c     set jactyp (estimated jacobian is supposed full)
            jactyp=2
         else
c     check jactyp
            if(jactyp.eq.1.or.jactyp.eq.4) then
c     %ODEOPTIONS requires the jacobian
               call msgs(76,0)
               jactyp=jactyp+1
            endif
         endif
         jaco=.false.         
         kjac=0
      endif

      kytop=top1

c
c     rhs
      ilf=iadr(lstk(top1))
      islf=istk(ilf)
      if(islf.ne.10.and.islf.ne.15.and.islf.ne.11.and.islf.ne.13) then
         err=rhs-(tope-top1)
         call error(80)
         return
      endif
      kydot=top1
      if(islf.eq.10) then
         namef=' '
         call cvstr(istk(ilf+5)-1,istk(ilf+6),namef,1)
         namef(istk(ilf+5):istk(ilf+5))=char(0)
         call setfydot2(namef,irep)
         if ( irep.eq.1) then 
            buf = namef
            call error(50)
            return
         endif
c    test list('fex',w)
      elseif(islf.eq.15) then
        le1=sadr(ilf+istk(ilf+1)+3)
        if(istk(iadr(le1)).eq.10) then
        withw=.true.
c     next line just to tell to bydot that external is in fortran
        istk(ilf)=10 
        if(istk(ilf+1).ne.2) then
           buf='wrong list passed: needs two elts in list'
           call error(9999)
           return
        endif
        long1=istk(ilf+3)
        lf=lstk(top1)
        illl=iadr(lf+istk(ilf+3))
        nblett=istk(illl-1)-1
        namef=' '
        call cvstr(istk(ilf+11)-1,istk(ilf+12),namef,1)
        namef(istk(ilf+11):istk(ilf+11))=char(0)
        call setfydot2(namef,irep)
        if ( irep.eq.1) then 
           buf = namef
           call error(50)
           return
        endif
        ll1=sadr(ilf+5)
        ll2=ll1+long1-1
        il2=iadr(ll2)
        nbw=istk(il2+1)*istk(il2+2)
        if(istk(il2+3).ne.0) then
           buf='working array must be real'
           call error(9999)
           return
        endif
        lww=sadr(il2+4)
c     lww = adr w , nbw = size (w) 
      endif
      endif
c
c     jaco,type and meth initialized ...
c     top2 point on y0
c
c     y0
      kynew=top2
      il=iadr(lstk(top2))
      ilws=il
      it=istk(il+3)
      if(it.ne.0) then
         buf='odedc: y0 must be real!'
         call error(9999)
         return
      endif
c
      if(istk(il).eq.1) then
         hsize=4
         ny1=istk(il+1)
         ny2=istk(il+2)
         ny=ny1*ny2
         else
            buf='odedc: invalid y0'
            call error(9999)
      endif
      ly=sadr(il+hsize)

c     list('fex',w)
      if(withw) then
         if(top+1.ge.bot) then
            call error(18)
            return
         endif
         top=top+1
c         kynew=top
         ily=iadr(lstk(top))
         err=sadr(ily+4)+ny+nbw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         istk(ily+1)=1
         istk(ily+2)=ny+nbw
         istk(ily+3)=1
         istk(ily+4)=0
         ly1=sadr(ily+4)
         call unsfdcopy(ny,stk(ly),1,stk(ly1),1)
         call unsfdcopy(nbw,stk(lww),1,stk(ly1+ny),1)
         lstk(top+1)=ly1+ny+nbw
         ly=ly1
         lw=lstk(top+1)
      endif
      lw1=lw
      lw=sadr(iadr(lw1)+13)
c 
c **************************************
c      nd discrete part size
      top2=top2+1
      il=iadr(lstk(top2))
      if(istk(il).ne.1) then
         err=rhs-(tope-top2)
         call error(53)
         return
      endif
      l=sadr(il+4)
      if((istk(il+1)*istk(il+2)).ne.1) then
         buf='odedc: argument nc must be 1x1 and integer!'
         call error(9999)
         return
      endif
      nd=int(stk(l))
      ny=ny-nd
      if(ny.lt.0) then
         buf='odedc: value of nd exceeds dimension of y0'
         call error(9999)
         return
      endif
c 
c      hstep or [hstep delta]
      top2=top2+1
      il=iadr(lstk(top2))
      if(istk(il).ne.1) then
         err=rhs-(tope-top2)
         call error(53)
         return
      endif
      l=sadr(il+4)
      nsi=istk(il+1)*istk(il+2)
      if(.not.(nsi.eq.1.or.nsi.eq.2)) then
         buf='odedc: argument hstep must be 1 or 2'
         call error(9999)
         return
      endif
      hstep=stk(l)
      delta=0.0d0
      if(nsi.eq.2) delta=stk(l+1)
c ************************
c     t0
      top2=top2+1
      kttop=top2
      il=iadr(lstk(top2))
      if(istk(il).ne.1) then
         err=rhs-(tope-top2)
         call error(53)
         return
      endif
      l=sadr(il+4)
      t0=stk(l)
c     t1
      top2=top2+1
      il=iadr(lstk(top2))
      if(istk(il).ne.1) then
         err=rhs-(tope-top2)
         call error(53)
         return
      endif

c     number of output points
      nn=istk(il+1)*istk(il+2)
c     pointer on  output time vector
      lt1=sadr(il+4)
c
c     optionnal parameters rtol et atol
      top2=top2+1
c     default values
      if(meth.eq.6.or.meth.eq.7) then
         rtol=1.d-3
         atol=1.d-4
      else
         rtol=1.0d-7
         atol=1.0d-9
      endif
      nr=1
      na=1
      jobtol=kytop-top2+1
c     jobtol=(nothing ,rtol only,rtol and atol)
c
      if(jobtol.eq.1) then
c     default tolerances
         lr=lw
         la=lr+1
         stk(la)=atol
         stk(lr)=rtol
      else
c     rtol given
         lr=lw
c     rtol
         il=iadr(lstk(top2))
         if(istk(il).ne.1) then
            err=rhs-(tope-top2)
            call error(53)
            return
         endif
         nr=istk(il+1)*istk(il+2)
         if(nr.ne.1.and.nr.ne.ny) then
            err=rhs-(tope-top2)
            call error(89)
            return
         endif
         lrt=sadr(il+4)
         call unsfdcopy(nr,stk(lrt),1,stk(lr),1)
         la=lr+nr
c     atol
         if(jobtol.eq.2) then
c        .  default
            stk(la)=atol
         else
c        .  atol given
            top2=top2+1
            il=iadr(lstk(top2))
            if(istk(il).ne.1) then
               err=rhs-(tope-top2)
               call error(53)
               return
            endif
            na=istk(il+1)*istk(il+2)
            if(na.ne.1.and.na.ne.ny) then
               err=rhs-(tope-top2)
               call error(89)
               return
            endif
            lat=sadr(il+4)
            call unsfdcopy(na,stk(lat),1,stk(la),1)
         endif
      endif
      lw=la+na

c     set input top value
      if(achaud) top=top+2
c
      if(nr.eq.1.and.na.eq.1) itol=1
      if(nr.eq.1.and.na.gt.1) itol=2
      if(nr.gt.1.and.na.eq.1) itol=3
      if(nr.gt.1.and.na.gt.1) itol=4

c     compute integrator workspace  sizes
      if(meth.eq.0) then
c     lsoda
         lrw=22+ny*max(16,ny+9)
         liw=20+ny
         nsizd=241
         nsizi=50
         if(jactyp.eq.4.or.jactyp.eq.5) then
            lrn=20+16*ny
            lrs=22+10*ny+(2*ml+mu)*ny
            lrw=max(lrn,lrs)
         endif
      elseif(meth.eq.1) then
c     lsode - adams
         if(jactyp.eq.1.or.jactyp.eq.2) then
            lrw=22+16*ny+ny*ny
         elseif(jactyp.eq.4.or.jactyp.eq.5) then
            lrw=22+16*ny+(2*ml+mu+1)*ny
         else
            lrw=20+16*ny
         endif
         liw=20+ny
         nsizd=219
         nsizi=41
      elseif(meth.eq.2) then
c     lsode gear
         if(jactyp.eq.1.or.jactyp.eq.2) then
            lrw=22+9*ny+ny*ny
         elseif(jactyp.eq.4.or.jactyp.eq.5) then
            lrw=22+9*ny+(2*ml+mu+1)*ny
         else
            lrw=20+9*ny
         endif
         liw=20+ny
         nsizd=219
         nsizi=41
      elseif(meth.eq.3) then
c     lsodar
         ilroot=iadr(lw)
         lw=sadr(ilroot+nsurf)
         lrw= 22 + ny * max(16, ny + 9) + 3*nsurf
         liw=20+ny
         nsizd=246
         nsizi=59
      elseif(meth.eq.4) then
c     lsdisc
         lrw=ny
         liw=1
      elseif(meth.eq.5) then
c     lsrgk
         lrw=3*ny
         liw=1
      elseif(meth.eq.6) then
c     rkf45
         lrw=3+8*ny
         liw=5
      elseif(meth.eq.7) then
c     rksimp
         lrw=3+8*ny
         liw=1
      endif
c
c     hot start
c
      if(achaud) then
         istate=2
c     commons retrieval from hot start tables
         if(meth.eq.0) then
c     lsoda
            lsavs=lc+lrwp-nsizd
            lsavi=lci+liwp-nsizi
            call rscma1(stk(lsavs),stk(lsavi))
         elseif(meth.eq.1.or.meth.eq.2) then
c     lsode
            lsavs=lc+lrwp-nsizd
            lsavi=lci+liwp-nsizi
            call rscom1(stk(lsavs),stk(lsavi))
         elseif(meth.eq.3) then
c     lsodar
            lsavs=lc+lrwp-nsizd
            lsavi=lci+liwp-nsizi
            call rscar1(stk(lsavs),stk(lsavi))
         endif
c     integer workspace retrieval
         do 40 k=1,liw
            istk(ilc+k-1)=int(stk(lci+k-1))
 40      continue
      endif
c
c
c     compute pointer on ode real and integer work spaces
      lc0=lw
      li=lc0+lrw
c
      ili=iadr(li)
      lw=sadr(ili+liw)
c
c     get memory to store results
      lyp=lw
      if(itask.eq.5) then
c     unknown number of output points.  space for  points 
c     will be allocated later
         single=.true.
         lw=lyp
         if(nn.ne.1) then
            call msgs(77,0)
            stk(lt1)=stk(lt1+nn-1)
            nn=1
         endif
         if(it.ne.0) then
            buf='itask=5: y0 must be a real vector'
            call error(9999)
            return
         endif
      else
c     number of output points is equal to number of t points all 
c     space allocated here
         single=.false.
         lw=lw+nn*(ny+nd)
      endif
c     top points on external workspace
      top=top+1
      lstk(top+1)=lw
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      call xsetf(1)
      call xsetun(wte)
c
      if(.not.achaud) then
         lc=lc0
         ilc=ili
      endif
c
c     data structure passed to externals, it contains pointer
c     to externals parameters
c
      ilw1=iadr(lw1)
      istk(ilw1)=3
      istk(ilw1+1)=ilw1+4
      istk(ilw1+2)=ilw1+7
      istk(ilw1+3)=ilw1+10
      istk(ilw1+4)=kydot
      istk(ilw1+5)=kttop
      istk(ilw1+6)=kynew
      istk(ilw1+7)=kjac
      istk(ilw1+8)=kttop
      istk(ilw1+9)=kynew
      istk(ilw1+10)=ksurf
      istk(ilw1+11)=kttop
      istk(ilw1+12)=kynew
c 
      if(iopt.eq.1) then
c     copy integration options in lsod* workspace
         if(itask.ge.4) then
            stk(lc)=tcrit
         endif
         stk(lc+4)=h0
         stk(lc+5)=hmax
         stk(lc+6)=hmin
         if(meth.eq.0.or.meth.eq.3) then
c   lsoda/lsodar
            if(jactyp.eq.4.or.jactyp.eq.5) then
               istk(ilc)=ml
               istk(ilc+1)=mu
            endif
            istk(ilc+4)=ixpr
            istk(ilc+5)=mxstep
            istk(ilc+6)=0
            istk(ilc+7)=mxordn
            istk(ilc+8)=mxords
         elseif(meth.lt.3) then
c   lsode 
            if(jactyp.eq.4.or.jactyp.eq.5) then
               istk(ilc)=ml
               istk(ilc+1)=mu
            endif
            if(meth.lt.2) then
               istk(ilc+4)=mxordn
            else
               istk(ilc+4)=mxords
            endif
            istk(ilc+5)=mxstep
            istk(ilc+6)=0
         endif
      endif
      tmax=stk(lt1+nn-1)
      niter=nn
c     
      if(ixpr.eq.1.and.iopt.eq.1) then
         write(buf,'(''itask = '',i3,'' meth = '',i3,'' jactyp = '','//
     $        'i3,'' ml = '',i3,'' mu = '',i3)') itask,meth,jactyp,ml
     $        ,mu
         call basout(io,wte,buf(1:80))
      write(buf,'(''tcrit= '',e9.4,'' h0= '',e9.4, '' hmax= '','//
     $       'e9.4,'' hmin = '',e9.4)')
     $    tcrit,stk(lc+4),stk(lc+5),stk(lc+6)
         call basout(io,wte,buf(1:80)) 
      endif

      tleft=t0
      iflag=0
      ntpass=0
      nhpass=0
      if(single) then
c     loop til t=tout
c     --------------
         nn=0
         k=0
         tf=tmax
 50      continue
         hf=min(t0+nhpass*hstep+delta*hstep,tmax)
c     set continuuous integration time
 51      continue
         if(abs(tleft-hf).le.1.d-12) goto 52
         istore=1
         if(ixpr.eq.1.and.iopt.eq.1) then
            write(buf
     $           ,'(''integ. from tleft='',e10.3,'' to tf= '','//
     $           'e10.3)') tleft,tright 
            call basout(io,wte,buf(1:50))
         endif
         tright=hf
         tcrit=hf
         stk(lc)=tcrit
c     integrate continuous part
         if(meth.eq.0) then
            call lsoda(bydot2,ny,stk(ly),tleft,tright,itol
     $           ,stk(lr),stk(la),itask,istate,iopt,stk(lc),lrw
     $           ,istk(ilc),liw,bjac,10*meth+jactyp)
         elseif(meth.eq.1.or.meth.eq.2) then
            call lsode(bydot2,ny,stk(ly),tleft,tright,itol
     $           ,stk(lr),stk(la),itask,istate,iopt,stk(lc),lrw
     $           ,istk(ilc),liw,bjac,10*meth+jactyp)
         elseif(meth.eq.3) then
            call lsodar(bydot2,ny,stk(ly),tleft,tright,itol,stk(lr)
     $           ,stk(la),itask,istate,iopt,stk(lc),lrw,istk(ilc)
     $           ,liw,bjac,jactyp,bsurf,nsurf,istk(ilroot)) 

         elseif(meth.eq.4) then
            call lsdisc(bydot2,ny,stk(ly),tleft,tright, stk(lc)
     $           ,lrw,istate)
         elseif(meth.eq.5) then
            call lsrgk(bydot2,ny,stk(ly),tleft,tright,itol
     $           ,stk(lr),stk(la),itask,istate,iopt,stk(lc),lrw
     $           ,istk(ilc),liw,bjac,meth)
            if(iero.eq.-1) then
               write(buf,'(e10.3)') tright
               call msgs(70,0)
            endif
         elseif(meth.eq.6) then
            call rkf45(bydot2,ny,stk(ly),tleft,tright,itol,rtol,
     1           atol,itask,istate,iopt,stk(lc),lrw,istk(ilc),
     2           liw,bjac,meth)
         elseif(meth.eq.7) then
            call rksimp(bydot2,ny,stk(ly),tleft,tright,itol,rtol
     $           ,atol,itask,istate,iopt,stk(lc),lrw,istk(ilc)
     $           ,liw,bjac,meth)
         endif         
         if(err.gt.0) return
         if(istate.lt.0) then
            if(meth.le.3) then
               if(istate.eq.-3) then
                  buf='illegal input'
                  call error(9999)
                  return
                endif
            endif
            if(meth.eq.5) call msgs(71,0)
            call msgs(4,ierr)
            nn=k
            goto 500
         endif
         if((meth.eq.6.or.meth.eq.7).and.istate.ne.2) then
            nn=k
            call msgs(74,0)
            goto 500
         endif  
c     store intermediate result
         k=k+1
         lys=lyp+(k-1)*(ny+nd+1)
         lstk(top+1)=lys+(ny+nd+1)
         err=lstk(top+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         stk(lys)=tleft
         call unsfdcopy(ny+nd,stk(ly),1,stk(lys+1),1)
         if(meth.eq.3.and.istate.eq.3) then
c     lsodar: a root found
            nn=k
            goto 500
         endif
         goto 51
c     update discrete part if necessary
 52      continue
         iflag=1
         if(ixpr.eq.1.and.iopt.eq.1) then
            write(buf,'(''update at t = '',e10.3)') tright
            call basout(io,wte,buf(1:20))
         endif
         call bydot2(ny,tright,stk(ly),stk(ly+ny))
         if(err.gt.0) return
         iflag=0
         nhpass=nhpass+1
c     store intermediate result
         k=k+1
         lys=lyp+(k-1)*(ny+nd+1)
         lstk(top+1)=lys+(ny+nd+1)
         err=lstk(top+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         stk(lys)=tleft
         call unsfdcopy(ny+nd,stk(ly),1,stk(lys+1),1)
         if (abs(tleft-tmax).gt.1.d-12) goto 50
         nn=k
      else
         do 60 k=1,niter
 59         tf=stk(lt1+k-1)
            hf=t0+nhpass*hstep+delta*hstep
            if(ixpr.eq.1.and.iopt.eq.1) then
               write(buf,'(''tf-hf = '',e10.3)') tf-hf
               call basout(io,wte,buf(1:20))
            endif
c     set continuous integration time
            if(abs(tf-hf).le.1.d-12) then
               tright=hf
               nhpass=nhpass+1
               istore=1
               if(ixpr.eq.1.and.iopt.eq.1) then
                  write(buf,'(''integ. from tleft='',e10.3,'//
     $                 ''' to hf=tf= '',e10.3)') tleft,tright 
                  call basout(io,wte,buf(1:50))
               endif
               update=.true.
            elseif(tf.lt.hf) then
               tright=tf
               istore=1
               if(ixpr.eq.1.and.iopt.eq.1) then
                  write(buf
     $                 ,'(''integ. from tleft='',e10.3,'' to tf= '','//
     $                 'e10.3)') tleft,tright 
                  call basout(io,wte,buf(1:50))
               endif
               update=.false.
            elseif(tf.gt.hf) then
               tright=hf
               nhpass=nhpass+1
               istore=0
               if(ixpr.eq.1.and.iopt.eq.1) then
                  write(buf
     $                 ,'(''integ. from tleft='',e10.3,'' to hf= '','//
     $                 'e10.3)') tleft,tright 
                  call basout(io,wte,buf(1:50))
               endif
               update=.true.
            endif
            tcrit=hf
            stk(lc)=tcrit
c     integrate continuuous part
            if(meth.eq.0) then
               call lsoda(bydot2,ny,stk(ly),tleft,tright,itol
     $              ,stk(lr),stk(la),itask,istate,iopt,stk(lc),lrw
     $              ,istk(ilc),liw,bjac,10*meth+jactyp)
            elseif(meth.eq.1.or.meth.eq.2) then
               call lsode(bydot2,ny,stk(ly),tleft,tright,itol
     $              ,stk(lr),stk(la),itask,istate,iopt,stk(lc),lrw
     $              ,istk(ilc),liw,bjac,10*meth+jactyp)
            elseif(meth.eq.3) then
               call lsodar(bydot2,ny,stk(ly),tleft,tright,itol,stk(lr)
     $              ,stk(la),itask,istate,iopt,stk(lc),lrw,istk(ilc)
     $              ,liw,bjac,jactyp,bsurf,nsurf,istk(ilroot)) 

            elseif(meth.eq.4) then
               call lsdisc(bydot2,ny,stk(ly),tleft,tright, stk(lc)
     $              ,lrw,istate)
            elseif(meth.eq.5) then
               call lsrgk(bydot2,ny,stk(ly),tleft,tright,itol
     $              ,stk(lr),stk(la),itask,istate,iopt,stk(lc),lrw
     $              ,istk(ilc),liw,bjac,meth)
               if(iero.eq.-1) then
                  write(buf,'(e10.3)') tright
                  call msgs(70,0)
               endif
            elseif(meth.eq.6) then
               call rkf45(bydot2,ny,stk(ly),tleft,tright,itol,rtol,
     1              atol,itask,istate,iopt,stk(lc),lrw,istk(ilc),
     2              liw,bjac,meth)
            elseif(meth.eq.7) then
               call rksimp(bydot2,ny,stk(ly),tleft,tright,itol,rtol
     $              ,atol,itask,istate,iopt,stk(lc),lrw,istk(ilc)
     $              ,liw,bjac,meth)
            endif         
            if(err.gt.0) return
            if(istate.lt.0) then
            if(meth.le.3) then
               if(istate.eq.-3) then
                  buf='illegal input'
                  call error(9999)
                  return
                endif
            endif
               if(meth.eq.5) call msgs(71,0)
               call msgs(4,ierr)
               nn=k-1
               goto 500
            endif
            if((meth.eq.6.or.meth.eq.7).and.istate.ne.2) then
               nn=k-1
               call msgs(74,0)
               goto 500
            endif     
c     update discrete part if necessary
            if(update) then
               iflag=1
               if(ixpr.eq.1.and.iopt.eq.1) then
                  write(buf,'(''update at t = '',e10.3)') tright
                  call basout(io,wte,buf(1:20))
               endif
               call bydot2(ny,tright,stk(ly),stk(ly+ny))
               if(err.gt.0) return
               iflag=0
            endif
c     store intermediate result
            if(meth.eq.3.and.istate.eq.3) then
c     lsodar: a root found
               nn=k
               goto 500
            endif
            if(istore.eq.1) then
               if(it.eq.0) then
                  lys=lyp+(k-1)*(ny+nd)
                  call unsfdcopy(ny+nd,stk(ly),1,stk(lys),1)
               else
                  lys=lyp+(k-1)*nys2
                  call unsfdcopy(nys2,stk(ly),1,stk(lys),1)
                  call unsfdcopy(nys2,stk(ly+nys2),1,stk(lys+nn*nys2),1)
               endif
            else
               goto 59
            endif
 60      continue
      endif
 500  continue
      if(lhs.ge.3) then
c     preserve lsod* working spaces
         lw=lyp+nn*(ny+nd+1)
         ilw=iadr(lw+lrw)
         err=sadr(ilw+liw)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(lrw,stk(lc),1,stk(lw),1)
         call icopy(liw,istk(ilc),1,istk(ilw),1)
      endif
c form state output
      ils=iadr(lstk(kynew))
      top=tope-rhs+1
c      
      call icopy(hsize,istk(ils),1,istk(ile),1)
      ly=sadr(ile+hsize)
      nel=istk(ile+1)*istk(ile+2)
      if(single) then
         istk(ile+1)=istk(ile+1)+1
         istk(ile+2)=nn*istk(ile+2)
         inc=1
         if(ly.gt.lyp) inc=-1
         call unsfdcopy((ny+nd+1)*nn,stk(lyp),inc,stk(ly),inc)
         lstk(top+1)=ly+(ny+nd+1)*nn
      else
         istk(ile+2)=nn*istk(ile+2)
         inc=1
         if(ly.gt.lyp) inc=-1
c
         call unsfdcopy((ny+nd)*nn,stk(lyp),inc,stk(ly),inc)
         lstk(top+1)=ly+(ny+nd)*nn
      endif

      if(meth.eq.3) then
c     lsodar: form roots output
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+3)=0
         l=sadr(il+4)
         if(istate.eq.3) then
            istk(il+1)=1
            istk(il+2)=1
            stk(l)=t0
            do 503 i=0,nsurf-1
               if(istk(ilroot+i).ne.0) then
                  l=l+1
                  istk(il+2)=istk(il+2)+1
                  stk(l)=i+1
               endif
 503        continue
         else
            istk(il+1)=0
            istk(il+2)=0
         endif
         lstk(top+1)=l+1
      endif
c form w and iw output
      if(lhs.lt.3) return
c     w
      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=lrw+nsizd
      istk(il+3)=0
      l=sadr(il+4)
      lstk(top+1)=l+lrw+nsizd
      call unsfdcopy(lrw,stk(lw),1,stk(l),1)
      lsvs=l+lrw
c     iw
      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=liw+nsizi
      istk(il+3)=0
      l=sadr(il+4)
      lstk(top+1)=l+liw+nsizi
      do 506 k=1,liw
         stk(l+k-1)=dble(istk(ilw+k-1))
 506  continue
      lsvi=l+liw
      if(meth.eq.0) then
         call svcma1(stk(lsvs),stk(lsvi))
      elseif(meth.lt.3) then
         call svcom1(stk(lsvs),stk(lsvi))
      else
         call svcar1(stk(lsvs),stk(lsvi))
      endif

      return
c     fin de odedc.....
      end
c      subroutine pristk(il,n)
c      include '../stack.h'
c      write(6,*) (istk(il+i),i=0,n-1)
c      end


C     For C function who need to read odedc common
      subroutine getcodc(nd1,iflag1)
      integer nd1,iflag1
      common/odecd/nd,iflag
      nd1=nd
      iflag1=iflag
      return
      end
