      subroutine fpq2 (inout,x,cx,fx,gx,d,sthalf,penlty,
     1iyflag,y,cy,fy,gy,z,cz,fz,gz,gg,hh,s)
      implicit double precision (a-h,o-z)
      data zero/0.0d+0/,half/0.50d+0/
      absd=abs(d)
      if (inout.eq.0) then
        iyflag=0
        gg=zero
        hh=zero
        s=absd
        if ((sthalf.le.zero).or.(sthalf.ge.half)) sthalf=half*half
        if (penlty.le.zero) penlty=half+half
        if (gx.ne.zero) d=-sign(absd,gx)
        inout=1
      else
        if ((cz.gt.zero).or.(fz.ge.fx)) then
          inout=3
          if (iyflag.eq.0) then
c            if (cz.gt.zero) go to 100
            gg=(gz-gx)/d
            hh=gg
  100       s=sthalf/absd
            iyflag=1
          else
            hh=(gz-gy)/(d-(y-x))
          end if
          y=z
          cy=cz
          fy=fz
          gy=gz
        else
          if (gx*gz.lt.zero) then
            inout=2
            hh=gg
            if (iyflag.eq.0) then
              gg=(gz-gx)/d
              s=sthalf/absd
              iyflag=1
            else
              gg=(gz-gy)/(d-(y-x))
            end if
            y=x
            cy=cx
            fy=fx
            gy=gx
          else
            inout=1
            gg=(gz-gx)/d
          end if
          x=z
          cx=cz
          fx=fz
          gx=gz
        end if
        if (iyflag.eq.0) then
          dlower=s
          dupper=absd/sthalf
          xminsy=-d
        else
          xminsy=x-y
          smallh=min(zero,hh)*xminsy*half
          gyplus=gy+smallh
c          if (cy.le.zero) then
            p=fx-fy-gyplus*xminsy
            denom=abs(gyplus+smallh-gx)
c          else
c            p=-cy-gyplus*xminsy
c            denom=abs(gyplus+smallh-gx*p*penlty)
c          end if
          if (p.ge.zero) go to 500
          p=zero
          s=sthalf/abs(xminsy)
  500     dlower=s*xminsy*xminsy
          dupper=abs(xminsy)-dlower
          if(abs(p).lt.denom*dupper) dupper=max(dlower,p/denom)
        end if
        absgx=abs(gx)
        absd=dupper
        if (absgx.lt.gg*dupper) absd=max(dlower,absgx/gg)
        d=-sign(absd,xminsy)
      end if
      z=x+d
      return
      end
