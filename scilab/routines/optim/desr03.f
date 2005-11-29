      subroutine desr03(z,iz,r,ir,g,a,w,d,ipvt,alfa,n,ng,ind,info,id,
     &                  ro,io)
C      SUBROUTINE DESR03(Z,IZ,R,IR,G,A,W,D,IPVT,ALFA,N,NG,IND,INFO,ID,
C     &                  RO,IO)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C      Copyright:       Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,Estadistica y Computacion          *
C       -----------------------------------------------------          *
C                     UNIVERSIDAD DE CANTABRIA                         *
C                     ------------------------                         *
C                           JUNIO   1987                               *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta es una subrutina auxiliar de OPTR03. Calcula una direccion
C        de descenso para el problema cuadratico que alli se plantea.
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        Z      Matriz de dimension  (IZ,N).  En sus  NG  primeras filas
C               contiene  a la matriz cuyas columnas forman una base del
C               nucleo  de la matriz  de  los coeficientes  del conjunto
C               activo.
C
C        IZ     Primera dimension de la matriz  Z. IZ >= NG.
C
C        R      Matriz  de  dimension  (IR,N). Almacena  la  matriz  R
C               de salida  de  la  subrutina  ANFM03,ANFM05  o  ANFM06.
C               La parte subdiagonal no es utilizada.
C
C        G      Vector de dimension  NG.  Contiene  al gradiente  del
C               funcional de OPTR03 en un punto.
C
C        A      Vector de dimension NG que contiene (si id < -NG  o  si
C               ID > NG) los coeficientes de la ultima restriccion
C               eliminada del conjunto activo (asociada a un
C               multiplicador extremo).
C
C        W      Vector de trabajo de dimension  2*N.
C
C        D      Vector de trabajo de dimension  NG.
C
C        IPVT   Vector entero de dimension  N. Guarda al vector  IPVT
C               de salida de la subrutina ANFM03, ANFM05  o  ANFM06.
C
C        ALFA   Variable asociada al funcional de OPTR03.
C
C        N      Numero de columnas de  Z.
C
C        NG     Dimension del vector gradiente.
C
C        IND    Variable que indica el tipo de factorizacion obtenida
C               en ANFM03, ANFM05  o  ANFM06.  Toma los valores de la
C               variable  IND  de salida de las subrutinas anteriores.
C
C        INFO   Variable que indica segun los valores:
C                   0  : el gradiente proyectado es un vector cualquiera
C                   1  : el gradiente proyectado es un multiplo escalar
C                        del vector n-esimo de la base canonica
C                  10  : se suministra el gradiente proyectado
C
C        ID     Variable que indica segun los valores:
C                <> 0  : en  OPTR03  se ha eliminado una restriccion
C                        que estaba asociada a un multiplicador extremo
C                        ID = ipvt(icol) si el multiplicador <> 1
C                        ID = -ipvt(icol) en otro caso
C                   0  : en otro caso.
C
C     DE SALIDA:
C
C        D      Vector que se recoge la direccion de descenso.
C
C        ID     Variable que indica el modo de calculo de la direccion
C               de descenso:
C                   0   : direccion de curvatura negativa o nula.
C                   1   : en otro caso.
C
C        RO     Variable que almacena (si ID=1)  el posible paso optimo
C               de la iteracion correspondiente.
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES:  anrs01,anrs02,daxpy,dcopy,ddot,dnrm2,
C                               dscal,d1mach
C     FUNCIONES FORTRAN INTRINSECAS: abs,mod,sqrt
C
C
      implicit double precision(a-h,o-z)
      dimension z(iz,*),r(ir,*),g(*),w(*),d(*),ipvt(*),a(*)
      indmul=id
      id=0
css      eps=d1mach(4)**0.75
      eps=dlamch('p')**0.75

      n1=n+1
C
C     Se calcula el gradiente proyectado cambiado de signo (si IND >= 0)
C
      if(ind.ge.0) then
         ro=1.d0
         if(info.eq.0) then
            do 10 i=1,n
10          w(i)=-ddot(ng,z(1,n1-i),1,g,1)
         else if(info.eq.1) then
            x=-ddot(ng,z,1,g,1)
         else if(info.eq.10) then
            info=0
            do 15 i=1,n
15          w(i)=-w(i)
         end if
      end if
C
C     Se estudia el caso en el que el Hessiano proyectado es definido
C     positivo
C
      if(ind.eq.0) then
         id=1
         if(info.eq.0) then
            if(alfa.ne.1.d0) call dscal(n,1.d0/alfa,w,1)
            call anrs02(r,ir,w,d,ipvt,n,io)
         else
            do 20 i=1,n-1
20          w(i)=0
            w(n)=-1
            call anrs01(r,ir,n,w,d,2,io)
            s=x*d(n)
            if(s.gt.0.d0) then
               do 30 i=1,n
30             w(ipvt(i))=d(i)
            else
               do 35 i=1,n
35             w(ipvt(i))=-d(i)
               s=-s
            end if
            if(alfa.ne.1.d0) then
               ro=s/alfa
            else
               ro=s
            end if
         end if
C
C     Se calcula una direccion de curvatura negativa en caso de Hessiano
C     proyectado indefinido
C
      else if(ind.lt.-1 .and. ind.ge.-n ) then
         m=-ind
         m2=m-1
         do 40 i=1,m2
40       d(i)=-r(i,m)
         call anrs01(r,ir,m2,d,d,2,io)
      else if (ind.lt.-n) then
         m=-ind-n
         m1=m+1
         s=r(m,m1)
         do 50 i=1,m-1
50       w(i)=s*r(i,m)-r(i,m1)
         if(m.gt.1) call anrs01(r,ir,m-1,w,d,2,io)
         d(m)=-s
         m2=m
C
C     Se analiza el caso en el que el Hessiano es semidefinido positivo
C
      else if(ind.gt.0 .and. ind.lt.n) then
         k=0
         m=n-ind
         if(info.eq.0) then
            do 60 i=1,n
60          w(n+i)=w(ipvt(i))
         end if
C
C     Se calcula una base del nucleo del Hessiano proyectado
C
         do 70 i=1,m
70       d(i)=0
         do 80 j=1,ind
            mj=m+j
            call anrs01(r,ir,m,r(1,mj),w,2,io)
            if(info.eq.0) then
               s=ddot(m,w,1,w(n1),1)-w(mj+n)
            else
               i=1
               if(n.ne.ipvt(i)) then
5010              i=i+1
                  if(n.ne.ipvt(i)) go to 5010
               end if
               if(i.eq.mj) then
                  s=-x
               else if(i.le.m) then
                  s=w(i)*x
               else
                  s=0.d0
               end if
            end if
            if(abs(s).gt.eps) then
               k=1
               call daxpy(m,s,w,1,d,1)
               d(mj)=-s
            end if
80       continue
C
C     Si el gradiente proyectado es ortogonal a la base del nucleo
C     obtenida  (k=0) se calcula una direccion de descenso
C
         if(k.eq.0) then
            id=1
            if(info.eq.0) then
               do 90 i=1,n
90             w(ipvt(i))=w(n+i)
               if(alfa.ne.1.d0) call dscal(n,1/alfa,w,1)
               call anrs02(r,ir,w,d,ipvt,m,io)
               do 95 i=m+1,n
95             w(ipvt(i))=0
            else
               d(m)=-1
               do 100 i=1,m-1
100            d(i)=0
               call anrs01(r,ir,m,d,d,2,io)
               do 110 i=m+1,n
110            d(i)=0
            end if
         end if
         if(k.eq.1 .or. info.eq.1) then
            do 120 i=1,n
120         w(ipvt(i))=d(i)
         end if
C
C     Se calcula una direccion de curvatura negativa
C
      else if (ind.gt.n .and.ind.le.2*n) then
         m=2*n-ind
         m3=m+1
         if(m.gt.0) then
            m1=n-m3
            do 140 i=1,m
140         d(i)=ddot(m1,r(i,m3),ir,r(m3,n),1)-r(i,n)
            call anrs01(r,ir,m,d,d,2,io)
         end if
         do 150 i=m3,n-1
150      d(i)=-r(i,n)
         d(n)=1
         if(x.lt.0)then
            do 160 i=1,n
160         d(i)=-d(i)
         end if
         do 170 i=1,n
170      w(ipvt(i))=d(i)
      else if(ind.gt.2*n) then
         m2=ind-2*n
         m=n-m2
         m3=m+1
         do 180 i=1,m-1
180      d(i)=-ddot(m2,r(i,m3),ir,r(m,m3),ir)
         s=dnrm2(m2,r(m,m3),ir)
         d(m)=-s*s
         call anrs01(r,ir,m,d,d,2,io)
         do 190 i=m3,n
190      d(i)=-r(m,i)
         s=d(m)*x
         if(s.lt.0.d0) then
            do 200 i=1,n
200         d(i)=-d(i)
         end if
         do 210 i=1,n
210      w(ipvt(i))=d(i)
      end if
      if(ind.lt.0) then
C
C     Se calcula una direccion de curvatura negativa
C
         if(n.gt.1) then
           do 220 i=1,n
220        w(i)=0
           if(ind.lt.-1) then
              do 230 i=1,m2
230           w(ipvt(i))=d(i)
              w(ipvt(m2+1))=1
           end if
         end if
      end if
C
C     Se proyecta la direccion obtenida
C
      if(ind.eq.n .and. info.eq.1) then
         if(abs(x).gt.eps) then
            do 240 i=1,ng
240         d(i)=x*z(i,1)
         else
            id=1
            do 245 i=1,ng
245         d(i)=0
         end if
      else if(ind.eq.-1) then
         call dcopy(ng,z(1,n1-ipvt(1)),1,d,1)
      else
         if(ind.eq.n) then
            s=dnrm2(n,w,1)
            if(s.le.eps) then
               id=1
               do 246 i=1,ng
246            d(i)=0
            end if
         end if
         if(ind.ne.n .or. (ind.eq.n .and. id.eq.0)) then
            do 250 i=1,ng
250         d(i)=ddot(n,z(i,1),-iz,w,1)
         end if
      end if
C
C     Se la afecta  del signo adecuado  para que sea  una direccion  de
C     descenso
C
      if(ind.lt.0 .or. (id.eq.1.and.info.eq.1.and.ind.gt.0)) then
         if(id.eq.1) then
            s=ddot(ng,d,1,g,1)
            if(s.gt.0.d0) then
               do 255 i=1,ng
255            d(i)=-d(i)
            else
               s=-s
            end if
            if(alfa.ne.1.d0) then
               ro=s/alfa
            else
               ro=s
            end if
         else
            if(indmul.eq.0) then
               s=ddot(ng,d,1,g,1)
            else
               if(indmul.gt.ng .or. indmul.lt.-ng) then
                   s=ddot(ng,d,1,a,1)
               else
                  s=d(abs(indmul))
               end if
            end if
            if((indmul.ge.0 .and. s.gt.0.d0) .or.
     &         (indmul.lt.0 .and. s.lt.0.d0)) then
               do 260 i=1,ng
260            d(i)=-d(i)
            end if
         end if
      end if
      end
