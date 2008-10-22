C/MEMBR ADD NAME=DSQRTC,SSI=0
      subroutine dsqrtc(dx, dy, du, dv)
c!purpose
c     computation of the complex square root in double precision
c!calling sequence
c     subroutine dsqrtc(dx, dy, du, dv)
c      du + j*dv = sqrt ( dx + j*dy )
c!
c
      implicit double precision (a-h,o-z)
      domi=2.0d+0*dlamch('p')
      dq = dx
      dp = dy
      dv = 0.50d+0*dq
      du = dq*dq + dp*dp
      du = sqrt(du)
      du = 0.50d+0*du
      dv = du - dv
      du = dv + dq
      if (abs(du).le.3.0d+0*dlamch('e')) du = 0.0d+0
      du = sqrt(du)
      if (abs(dv).le.3.0d+0*dlamch('e')) dv = 0.0d+0
      dv = sqrt(dv)
      if (dp.lt.(-domi)) du = -du
      return
      end
