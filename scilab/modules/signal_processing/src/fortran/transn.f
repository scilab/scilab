      subroutine transn(ityp,om,norma,vsn,vd,a)
c!purpose
c computation of the parameters of the normalized lowpass
c!
c
      implicit double precision (a-h,o-z)
      double precision  om(*)
c
      tan2(aa) = sin(aa/2.0d+0)/cos(aa/2.0d+0)
c
c
c         Nomenclature Rabiner-Gold (page 241)
c         Si ityp = < 2
c         vsn=1/k
      v1 = tan2(om(1))
      v2 = tan2(om(2))
      if (ityp.le.2) go to 210
      v3 = tan2(om(3))
      v4 = tan2(om(4))
      if (ityp.eq.3) go to 10
      q = v1
      v1 = -v4
      v4 = -q
      q = v2
      v2 = -v3
      v3 = -q
c
  10  jj = 1
      j = norma + 1
      go to (30, 30, 40, 70), j
  30  vdq1 = v2*v3
      vsn1 = vdq1/v1 - v1
      q = v4 - vdq1/v4
      if (q.lt.vsn1) vsn1 = q
      a1 = 1.0d+0/(v3-v2)
      vsn1 = vsn1*a1
      go to (40, 50, 40), j
  40  vdq = v1*v4
      a = v2/(vdq-v2*v2)
      q = v3/(v3*v3-vdq)
      if (q.lt.a) a = q
      vsn = a*(v4-v1)
      if (norma.eq.2) go to 200
      if (vsn.ge.vsn1) go to 200
  50  vdq = vdq1
  60  vsn = vsn1
      a = a1
      go to 200
c
  70  vdq = sqrt(v1*v2*v3*v4)
      a1 = v3/(v3*v3-vdq)
      vsn1 = (v4-vdq/v4)*a1
      a = v2/(vdq-v2*v2)
      vsn = (vdq/v1-v1)*a
      if (vsn.ge.vsn1) go to 200
      go to 60
c
c       BUG:       NO PATH TO HERE !!!!!
cccp      vdq = v2*v3
cccp      vsn = v4 - vdq/v4
c			
cccp      a = 1.0d+0/(v3-v2)
cccp      vsn = vsn*a
c
 200  vd = sqrt(vdq)
      a = a*vd
      if (ityp.le.3) go to 270
      a = a/vsn
      go to 270
c
 210  j = ityp
      go to (220, 220, 230, 240, 250, 260), j
 220  vsn = v2/v1
      go to (250, 240), j
 230  vd = v2/vsn
      go to 270
 240  vd = v2
      go to 270
 250  vd = v1
      go to 270
 260  vd = v1*vsn
c
 270  return
      end
