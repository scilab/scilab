c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine n1fc1o(unit,job,i1,i2,i3,i4,i5,d1,d2,d3,d4)
c
c     impression des traces
c
      implicit double precision (a-h,o-z)
      integer unit,lunit,job,i1,i2,i3,i4,i5(*)
      dimension d4(*),d3(*)
c     
      character*120  buf
     
      lunit=unit
c
      buf=' '
      goto(11,12,13,14,15,16,17,18,19,20,
     &     21,22,23,24,25,26,27,28,29,30,
     &     31,32,33,34,35,36,37,38,39,40,
     &     41,42,43,44,45,46,47,48,49,50,
     &     51,52,53,54,55,56,57,58,59,60) job
c
 11   continue
      call basout(io,lunit,'n1fc1   incorrect call')
      goto 100 
 12   continue
      n=i1
      memax=i2
      niz=i3
      nrz=i4
      ndz=i5(1)
      write (buf,'(''entry in n1fc1 . n='',i4,'' memax='',i3)')  n,memax
      call basout(io,lunit,buf(1:35))
      write (buf,"(a24,i6,a6,i6,a6,i6,a1)") 
     &     "minimal array sizes: iz(", niz, 
     &     ")  rz(", nrz, 
     &     ")  dz(", ndz,
     &     ")"
      call basout(io,lunit,buf(1:55))
      goto 100
 13   continue
      call basout(io,lunit,'n1fc1 initial gradient norm is zero')
      goto 100
 14   continue
 1000 format (19h n1fc1   iter  nsim,6x,2hfn,11x,3heps,7x,2hs2,
     19x,1hu,5x,2hnv)
      goto 100
 15   continue
      iter=i1
      write(buf,'(''n1fc1    end with iter ='',i1)') i1
      call basout(io,lunit,buf(1:30))
      goto 100
 16   continue
      call  basout(io,lunit,'n1fc1      Incorrect end of fprf2')
      goto 100
 17   continue
      eta2=d1
      write(buf,'(''n1fc1   eta2 assigned to '',d10.3)') eta2
      call basout(io,lunit,buf(1:35))
      goto 100
 18   continue
      iter=i1
      nsim=i2
      fn=d1
      epsm=d2
      s2=d3(1)
      write (buf,1018) iter,nsim,fn,epsm,s2
 1018 format(6h n1fc1,i7,i5,d16.7,16h   convergence a,d10.3,5h pres,
     13h  (,d9.2,1h))
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 19   continue
      call basout(io,lunit,'n1fc1   normal end')
      goto 100
 20   continue
      call basout(io,lunit,' ')
      goto 100
 21   continue
      iter=i1
      nsim=i2
      nv=i3
      fn=d1
      eps=d2
      s2=d3(1)
      u=d4(1)
      write (buf,'(''n1fc1   '',1i4,i5,2x,d14.7,3d10.2,i3)') iter,
     $     nsim,fn,eps,s2,u,nv 
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100 
 22   continue
      ntot=i1
      call basout(io,lunit,'n1fc1  ponderation table')
      nn=ntot/7
      if(7*nn.lt.ntot) nn=nn+1
      l=0
      do 2201 i=1,nn
         ln=min(7,ntot-l)
         write (buf,'(7x,7d10.3)') (d4(l+j),j=1,ln)
         call basout(io,lunit,buf(1:lnblnk(buf)))
         l=l+7
 2201 continue
 23   continue
      call basout(io,lunit,'n1fc1  la direction ne pivote plus')
      goto 100
 24   continue
      call basout(io,lunit,'n1fc1  end (dxmin reached)')
      goto 100
 25   continue
      call basout(io,lunit,'n1fc1  end (nsim reached)')
      goto 100
 26   continue
      call basout(io,lunit,'n1fc1  end (indic=0)')
      goto 100
 27   continue
      call basout(io,lunit,'n1fc1  warning txmax reached, reduce scale')
      goto 100
 28   continue
      diam1=d1
      eta2=d2
      ap=d3(1)
      write (buf,2801) diam1,eta2,ap
 2801 format (6h n1fc1,12x,6hdiam1=,d10.3,4x,5heta2=,d10.3,4x,
     1 3hap=,d10.3)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 29   continue
      iter=i1
      nsim=i2
      ntot=i3
      fn=d1
      write (buf,2901) iter,nsim,fn,ntot
 2901 format(6h n1fc1,i7,i5,d16.7,20h   faisceau reduit a,
     1 i3,10h gradients)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 30   continue
      logic=i1
      ro=d1
      tps=d2
      tnc=d3(1)
      write (buf,3001) logic,ro,tps,tnc
 3001 format (6h n1fc1,10x,6hlogic=,i2,4x,3hro=,d10.3,
     1 4x,4htps=,d10.3,4x,4htnc=,d10.3)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
c     ==================
c     MESSAGES de frepf2
c     ==================
 31   continue
      nt1=i1
      mm1=i2
      deps=d1
      call basout(io,lunit,'a = ')
      nn=nt1/10
      if(10*nn.lt.nt1) nn=nn+1
      l=0
      do 3101 i=1,nn
         ln=min(10,nt1-l)
         write (buf,'(6x,10d10.3)') (d3(l+j),j=1,ln)
         call basout(io,lunit,buf(1:lnblnk(buf)))
         l=l+10
 3101 continue
      write(buf,'(''    epsilon ='',d10.3)') deps
      call basout(io,lunit,buf(1:lnblnk(buf)))

      call basout(io,lunit,'(g,g) = ')
      do 3103 j=1,nt1
         mej=(j-1)*mm1
         nn=j/10
         if(10*nn.lt.j) nn=nn+1
         l=0
         do 3102 i=1,nn
            ln=min(10,j-l)
            write (buf,'(6x,10d10.3)') (d4(mej+l+jj),jj=1,ln)
            call basout(io,lunit,buf(1:lnblnk(buf)))
            l=l+10
 3102    continue
 3103 continue
      goto 100
 32   continue
      nv=i1
      call basout(io,lunit,'       initial corral')
      write(buf,'(20i6)') (i5(k),k=1,nv)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 33   continue
      call basout(io,lunit,
     $     ' error from fprf2. old solution already optimal')
      goto 100
 34   continue
      call basout(io,lunit,'     epsilon smaller than a')
      goto 100
 35   continue
      j=i1
      write(buf,'('' start with variables 1 and,'',i4)') j
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 36   continue
      nv=i1
      call basout(io,lunit,'x = ')
      nn=nv/10
      if(10*nn.lt.nv) nn=nn+1
      l=0
      do 3601 i=1,nn
         ln=min(10,nv-l)
         write (buf,'(3x,10d10.3)') (d4(l+j),j=1,ln)
         call basout(io,lunit,buf(1:lnblnk(buf)))
         l=l+10
 3601 continue
      goto 100
 37   continue
      call basout(io,lunit,' fprf2 is apparently looping')
      goto 100
 38   continue
      j0=i1
      s2=d1
      sp=d2
      write(buf,3801) s2,j0,sp
 3801 format(7h (s,s)=,d12.4,10h  variable,i4,
     &2h (,d12.4,12h) coming in.)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 39   continue
      s2=d1
      u1=d2
      write(buf,3901) s2,u1
 3901 format(7h (s,s)=,d12.4,5h  u1=,d12.3,23h  variable 1 coming in.)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 40   continue
      write(buf,'(''   duplicate variable '',i3)') j0
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 41   continue
      nv=i1
      mm1=i2
c     d3=rr,d4=r
      write(buf,'(''cholesky '',d11.3)') d3(1)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      if(nv.ge.2) then
         do 4103 ll=2,nv
            k1=ll-1
            nn=k1/10
            if(10*nn.lt.k1) nn=nn+1
            l=0
            if(nn.gt.1) then
            do 4102 i=1,nn-1
               ln=min(10,k1-l)
               write (buf,'(3x,10d10.3)') (d4((l+kk-1)*mm1+ll),kk=1,nn)
               call basout(io,lunit,buf(1:lnblnk(buf)))
               l=l+10
 4102       continue
            endif
            write(buf,'(3x,10d10.3)') (d4((l+kk-1)*mm1+ll),kk=1,nn),
     $           d3(ll)
            call basout(io,lunit,buf(1:lnblnk(buf)))
 4103    continue
      endif
      goto 100
 42   continue
      k0=i1
      l=i2
      yk0=d1
      ps1=d2
      ps2=d3(1)
      write(buf,4201) k0,l,yk0,ps1,ps2
 4201 format(9h variable,i4,2h (,i4,3h) =,d11.3,11h going out.,
     & 17h  feasible (s,s)=,d11.4,12h unfeasible=,d11.4)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 43   continue
      goto 100
 44   continue
      nc=i1
      nv=i2
c     jc=i5
      s2=d1
      sp=d2
      u1=d3(1)
      write(buf,4401) nc,nv
      call basout(io,lunit,buf(1:lnblnk(buf)))
      
      write(buf,44010)
      call basout(io,lunit,buf(1:lnblnk(buf)))

      write(buf,44011) s2,sp
      call basout(io,lunit,buf(1:lnblnk(buf)))      
      
      write(buf,44012) u1
      call basout(io,lunit,buf(1:lnblnk(buf)))
      
 4401     format(14h finished with,i3,10h gradients,i3)
44010     format(11h variables.)
44011     format(7h (s,s)=,d11.4,6h test=,d11.4)
44012     format(32h cost of the extra constraint u=,d12.5)
      
      nn=nv/20
      if(10*nn.lt.nv) nn=nn+1
      l=0
      do 4402 i=1,nn
         ln=min(20,nv-l)
         write (buf,'(20i6)') (i5(l+k),k=1,ln)
         call basout(io,lunit,buf(1:lnblnk(buf)))
         l=l+20
 4402 continue
      goto 100
c     ================
c     MESSAGE DE NLIS2
c     ================
 45   continue
      write (buf,4501)
 4501 format (4x,6h nlis2,10x,17htmin force a tmax)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 46   continue
      fpn=d1
      tmin=d3(1)
      tmax=d4(1)
      call basout(io,lunit,' ')
      write (buf,4601) fpn,d2,tmin,tmax
 4601 format (4x,9h nlis2   ,4x,4hfpn=,d10.3,4h d2=,d9.2,
     1 7h  tmin=,d9.2,6h tmax=,d9.2)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 47   continue
      call basout(io,lunit,' ')
      write(buf,4701) nap
 4701 format (4x,6h nlis2,3x,i5,22h simulations atteintes)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 48   continue
      call basout(io,lunit,'Stop required by user')
      goto 100
 49   continue
      indic=i1
      t=d1
      write(buf,4901) t,indic
 4901 format (4x,6h nlis2,36x,1hi,d10.3,7h indic=,i3)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 50   continue
      t=d1
      ffn=d2
      fp=d3(1)
      write(buf,5001) t,ffn,fp
 5001 format (4x,6h nlis2,36x,1hi,d10.3,2d11.3)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100

 51   continue
      write(buf,5101) t,ffn,fp
 5101 format (4x,6h nlis2,d13.3,2d11.3,2h i)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 52   continue
      logic=i1
      write(buf,5201) logic
 5201 format (4x,6h nlis2,3x,20hcontrainte implicite,i4,7h active)
      call basout(io,lunit,buf(1:lnblnk(buf)))
      goto 100
 53   continue
      logic=i1
      call basout(io,lunit,'nlis2   end (tmin reached)')
      goto 100
 54   continue
      goto 100
 55   continue
      goto 100
 56   continue
      goto 100
 57   continue
      goto 100
 58   continue
      goto 100
 59   continue
      goto 100
 60   continue
      goto 100
c
 100  return
      end
