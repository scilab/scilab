      subroutine matnew
c ====================================================================
c
c             
c    Frequency response, time response
c
c ====================================================================
c
c     Copyright INRIA
      include '../stack.h'
c
      double precision dnrm2,tol,atol,eps,gi,rcond,ur,ui,vr,vi,xr,xi
      integer p,ind,q,job,ido,n,top1
      integer iadr,sadr
c
c         fin    1     2      3      4       5     6
c              xxxx   ppol  tzer    freq  ltitr  rtitr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
   
      goto (10,20,30,40,50,60) fin
 10   continue
      goto 99
c
c  ppol
c
   20 continue
      call scippol('ppol')
      return
c  tzer
   30 continue
      call scitzer('tzer')
      return
c
c reponse frequentielle freq
c ==========================
 40   continue
      call scifreq('freq')
      return

c
c ltitr
c =====
c
50    continue
      call sciltitr('ltitr')
      return

c
c rtitr  simulation temporelle des systemes rationnels discrets
c =============================================================
c
 60   continue
      call scirtitr('rtitr')
99    return
      end
