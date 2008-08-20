c      
c     SUBROUTINE ener
c      
      subroutine ener(th,e)
        parameter (n=10)
        doubleprecision th(2*n),thd(n),et(1,1),r(n),j(n),m(n)
        integer i 
        data g / 9.81/
        data r / n*1.0/
        data m / n*1.0/
        data j / n*0.3/
c         
        do 1000, i =1,n ,1
          thd(i) = th(i+n)
 1000   continue
c       
      t1 = sin(th(6))
      t2 = r(6)*t1
      t3 = t2*thd(6)
      t4 = sin(th(3))
      t5 = r(3)*t4
      t6 = t5*thd(3)
      t7 = sin(th(1))
      t8 = r(1)*t7
      t9 = t8*thd(1)
      t10 = sin(th(2))
      t11 = r(2)*t10
      t12 = t11*thd(2)
      t13 = sin(th(4))
      t14 = r(4)*t13
      t15 = t14*thd(4)
      t16 = sin(th(5))
      t17 = r(5)*t16
      t18 = t17*thd(5)
      t19 = sin(th(7))
      t20 = r(7)*t19
      t21 = t20*thd(7)
      t22 = sin(th(8))
      t23 = r(8)*t22
      t24 = t23*thd(8)
      t26 = (-t3-t6-t9-t12-t15-t18-t21-t24)**2
      t27 = cos(th(3))
      t29 = r(3)*t27*thd(3)
      t30 = cos(th(1))
      t32 = r(1)*t30*thd(1)
      t33 = cos(th(2))
      t35 = r(2)*t33*thd(2)
      t36 = cos(th(4))
      t38 = r(4)*t36*thd(4)
      t39 = cos(th(5))
      t41 = r(5)*t39*thd(5)
      t42 = cos(th(6))
      t44 = r(6)*t42*thd(6)
      t45 = cos(th(7))
      t47 = r(7)*t45*thd(7)
      t48 = cos(th(8))
      t50 = r(8)*t48*thd(8)
      t52 = (t29+t32+t35+t38+t41+t44+t47+t50)**2
      t56 = sin(th(9))
      t57 = r(9)*t56
      t58 = t57*thd(9)
      t60 = (-t3-t6-t9-t12-t15-t18-t21-t24-t58)**2
      t61 = cos(th(9))
      t63 = r(9)*t61*thd(9)
      t65 = (t29+t32+t35+t38+t41+t44+t47+t50+t63)**2
      t69 = sin(th(10))
      t70 = r(10)*t69
      t73 = (-t3-t6-t9-t12-t15-t18-t21-t24-t58-t70*thd(10))**2
      t74 = cos(th(10))
      t78 = (t29+t32+t35+t38+t41+t44+t47+t50+t63+r(10)*t74*thd(10))**2
      t93 = thd(3)**2
      t97 = (-t6-t9-t12-t15-t18)**2
      t99 = (t29+t32+t35+t38+t41)**2
      t103 = thd(1)**2
      t106 = r(1)**2
      t107 = t7**2
      t110 = t30**2
      t116 = thd(2)**2
      t122 = thd(5)**2
      t125 = thd(4)**2
      t128 = m(8)*(t26+t52)/2+m(9)*(t60+t65)/2+m(10)*(t73+t78)/2+m(5)*g*
     +(t8+t11+t5+t14+t17)+m(10)*g*(t8+t11+t5+t14+t17+t2+t20+t23+t57+t70)
     ++m(9)*g*(t8+t11+t5+t14+t17+t2+t20+t23+t57)+m(1)*g*t8+J(3)*t93/2+m(
     +5)*(t97+t99)/2+J(1)*t103/2+m(1)*(t106*t107*t103+t106*t110*t103)/2+
     +J(2)*t116/2+m(6)*g*(t8+t11+t5+t14+t17+t2)+J(5)*t122/2+J(4)*t125/2
      t129 = thd(6)**2
      t132 = thd(7)**2
      t135 = thd(8)**2
      t138 = thd(9)**2
      t142 = (-t9-t12)**2
      t144 = (t32+t35)**2
      t151 = thd(10)**2
      t155 = (-t6-t9-t12-t15)**2
      t157 = (t29+t32+t35+t38)**2
      t165 = (-t6-t9-t12)**2
      t167 = (t29+t32+t35)**2
      t172 = (-t3-t6-t9-t12-t15-t18-t21)**2
      t174 = (t29+t32+t35+t38+t41+t44+t47)**2
      t182 = (-t3-t6-t9-t12-t15-t18)**2
      t184 = (t29+t32+t35+t38+t41+t44)**2
      t194 = J(6)*t129/2+J(7)*t132/2+J(8)*t135/2+J(9)*t138/2+m(2)*(t142+
     +t144)/2+m(3)*g*(t8+t11+t5)+J(10)*t151/2+m(4)*(t155+t157)/2+m(2)*g*
     +(t8+t11)+m(3)*(t165+t167)/2+m(7)*(t172+t174)/2+m(4)*g*(t8+t11+t5+t
     +14)+m(6)*(t182+t184)/2+m(7)*g*(t8+t11+t5+t14+t17+t2+t20)+m(8)*g*(t
     +8+t11+t5+t14+t17+t2+t20+t23)
         et(1,1) = t128+t194
        e = et(1,1)
        return
      end
