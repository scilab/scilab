c     ====================================      
      subroutine arnol (neq, t, y, ydot)
c     ====================================
c     used in fydottable.c
c     ====================================
      double precision t, y, ydot,aa
      common / aac / aa(6)
      dimension y(3), ydot(3)
      data aa  /1.0 ,1.0 ,1.0 ,1.0 ,1.0 ,1.0 /
      
      ydot(1)=aa(1)*cos(y(2)) +aa(2)*sin(y(3))
      ydot(2)=aa(3)*cos(y(3)) +aa(4)*sin(y(1))
      ydot(3)=aa(5)*cos(y(1)) +aa(6)*sin(y(2))
      
      return
      end
c     ==================================== 
     
    