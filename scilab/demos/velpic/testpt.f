      subroutine testpt(p1,p2,nc,xyline,flag,count,bav)
c <flag,bav>=testpoint(p1,p2,line)
c <flag,bav>=testpoint(p1,p2,line)
c macro which tests whether the line segment defined by the
c two points p1 and p2 intersects any of the line segments
c in line.
c  p1   :2x1 matrix giving the indices of point number one
c  p2   :2x1 matrix giving the indices of point number two
c  line :2xN matrix giving the indices of a line
c  flag :flag='on' indicates an intersection, flag='off' 
c       :indicates no intersection
c  bav  :3xM matrix giving all the intersections found (if any)
c       :and the position in the line of the intersection
c 
c!
c author: C. Bunks     date: 12-NOV-90
c /two line segments defined by (p1,p2) and (q1,q2)
c have a point in common if there exists two constants
c a and b such that 0<=a<=1 and 0<=b<=1 
c and 
c                (1-a)*p1+a*p2=(1-b)*q1-b*q2
c or 
c                                 [b]              
c                [(q2-q1) (p1-p2)][ ] = (p1-q1)
c                                 [a]              
c 
c NOTE: Could return the wrong value when the line segment
c passes through an end point of the line sequence. Thus
c all line segments on a grid point must be perturbed.
c Copyright INRIA
      integer      nc,flag,count
      real         bav(3,nc),xyline(2,nc)
      real         p1,p2,pd,q1
      real         q2,qd,vec,ba,detm
      dimension    p1(1,2),p2(1,2),pd(1,2),q1(1,2)
      dimension    q2(1,2),qd(1,2),vec(1,2),ba(1,2)

      count=0
      do 10 k=1,nc-1
         q1(1,1)=xyline(1,k)
         q1(1,2)=xyline(2,k)
         q2(1,1)=xyline(1,k+1)
         q2(1,2)=xyline(2,k+1)
         if(int(q1(1,1)).eq.q1(1,1).and.int(q1(1,2)).eq.q1(1,2))then
            q1(1,1)=q1(1,1)+.001
            q1(1,2)=q1(1,2)+.001
         endif
         if(int(q2(1,1)).eq.q2(1,1).and.int(q2(1,2)).eq.q2(1,2))then
            q2(1,1)=q2(1,1)+.001
            q2(1,2)=q2(1,2)+.001
         endif             
         qd(1,1)=q1(1,1)-q2(1,1)
         qd(1,2)=q1(1,2)-q2(1,2)
         pd(1,1)=p1(1,1)-p2(1,1)
         pd(1,2)=p1(1,2)-p2(1,2)

         detm=-qd(1,1)*pd(1,2)+qd(1,2)*pd(1,1)
         if(detm.ne.0)then
            vec(1,1)=p1(1,1)-q1(1,1)
            vec(1,2)=p1(1,2)-q1(1,2)
            ba(1,1)=(pd(1,2)*vec(1,1)-pd(1,1)*vec(1,2))/detm
            ba(1,2)=(qd(1,2)*vec(1,1)-qd(1,1)*vec(1,2))/detm

            if(0.le.ba(1,1).and.ba(1,1).le.1.and.
     +         0.le.ba(1,2).and.ba(1,2).le.1)then
               count=count+1
               bav(1,count)=(1-ba(1,2))*p1(1,1)+ba(1,2)*p2(1,1)
               bav(2,count)=(1-ba(1,2))*p1(1,2)+ba(1,2)*p2(1,2)
               bav(3,count)=k
            endif
         endif
 10   continue

c if the number of intersections is odd then an intersection
c occured

      if(int(count/2).ne.float(count)/2.) flag=1

      return
      end
