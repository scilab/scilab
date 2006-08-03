      subroutine anfm05(h,ih,r,ir,z,iz,p,w,ipvt,x,n,m,np,ind,modo,io)
C     SUBROUTINE ANFM05(H,IH,R,IR,Z,IZ,P,W,IPVT,X,N,M,NP,IND,MODO,IO)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C     Copyright:        Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                             ENERO  1988                              *
C                             JULIO  2005                              *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta subrutina modifica la factorizacion  de Cholesky  (LL') de
C        una matriz de la forma   PAP' , (P es una matriz de permutacion
C        de filas y  A  es de la forma  Z'HZ,  donde las columnas de  Z
C        forman una base del nucleo de un conjunto activo en un problema
C        de  optimizacion  con  restricciones ),   cuando  se a¤ade una
C        restriccion al conjunto  activo.
C        La matriz de entrada puede encontrarse parcialmente factorizada
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        H      Matriz de dimension  (IH,NP). En las  NP  primeras filas
C               contiene  la matriz  H.  Es  suficiente  suministrar  la
C               parte triangular inferior. El resto  de la matriz  no es
C               utilizado.
C
C        IH     Primera dimension de  H. IH >= NP.
C
C        R      Matriz de dimension (IR,N). En las  N+1  primeras filas
C               almacena  a la  matriz triangular superior (L')  de la
C               factorizacion de Cholesky,  (total o parcial), salvo la
C               primera columna. El resto de la matriz no se utiliza.
C
C        IR     Primera dimension de la matriz R. IR >= N+1.
C
C        Z      Matriz de dimension  (IZ,N). En las  NP  primeras filas
C               contiene la matriz  Z.
C
C        IZ     Primera dimension de la matriz  Z. IZ >= NP.
C
C        P      Vector de dimension 2*N.(Almacena ciertos coeficientes
C               de Givens).
C
C        W      Vector de trabajo de dimension igual al maximo de
C               2*(N+1) y NP.
C
C        IPVT   Vector entero de trabajo de dimension  N+1. Contiene la
C               informacion  sobre la permutacion   P   realizada  para
C               obtener la factorizacion de entrada; asi,
C                        IPVT(I)=J, indica que la fila J-esima de  A se
C               encuentra en el lugar I-esimo de  PAP' (analogamente con
C               las columnas).
C
C        X      Variable que contiene al primer elemento de la primera
C               columna de  L'.
C
C        N      N+1  es el numero de filas y columnas de la matriz  L'
C               de entrada,   N  es  el numero de filas y columnas  de
C               la matriz  L' de salida.
C
C        M      Dimension de la parte factorizada.
C
C        NP     Numero de filas de Z, numero de filas y columnas de H.
C
C        IND    Indicador que toma los valores:
C               IND > 2*(N+1) : No existe  la factorizacion completa  de
C                               la matriz de entrada. En la ultima parte
C                               de la matriz  R  se tiene  la  caja  no
C                               factorizada. Esta caja es de la forma:
C
C                                                         -VV'
C                               donde  V  es un vector, (cuya dimension
C                               es  IND-2*(N+1)), almacenado  en la fila
C                               3*(N+1)-IND   de   R    (en las ultimas
C                               columnas.
C               (N+1,2*N+3)   : No se tiene la factorizacion completa de
C                               la matriz de entrada. En la ultima parte
C                               de la matriz  R  se tiene  la  caja  no
C                               factorizada, que es de la forma:
C
C                                                             0      V
C
C                                                             V'     b
C
C                               con  V  un vector y  b  un escalar. La
C                               dimension de esta caja es  IND-(N+1).
C                               El vector  (V',b) esta almacenado en  la
C                               ultima columna  de  R , a partir  de la
C                               fila  2*(N+1)-IND+1.
C
C               (-1,N+2)      : Se suministra la factorizacion completa,
C                               N+1-IND  es  la dimension  de la caja no
C                               nula de  L' .
C               (-N-2,0)      : No se ha suministrado la factorizacion,
C                               por ser la matriz indefinida:el elemento
C                               -IND  de la diagonal es negativo.
C               (-2*N-3),-N-1): No se da  la factorizacion completa por
C                               ser  la matriz indefinida:  el elemento
C                               -IND-(N+1) de la diagonal es nulo  y el
C                               del lugar (-IND-(N+1),-IND-N) es no nulo
C
C        MODO   Variable que indica :
C                  0  : No  se  calcula  la  factorizacion  en  el  caso
C                       indefinido.
C               <> 0  : Se calcula la factorizacion (total o parcial)
C
C        IO     Numero de canal de salida de resultados.
C
C     DE SALIDA:
C
C        R      En   R   se  almacena  la   modificacion  de  la matriz
C               L'  de la factorizacion de Cholesky (total o parcial).
C
C        IPVT   Vector que contiene, en sus  N  primeras coordenadas, la
C               modificacion del vector  IPVT de entrada.
C
C        IND    Indicador que toma los valores:
C                  NP         : No se ha calculado la factorizacion por
C                               ser indefinida la matriz (y MODO=0).
C               (-1,N+1)      : Se ha obtenido la factorizacion completa
C                               N-IND es la dimension de la caja no nula
C                               de  L'.
C               (-(N+1),0)    : La factorizacion no se ha comletado  al
C                               ser indefinida la matriz:   el elemento
C                               -IND  de la diagonal es nulo.
C               (-2*N-1),-N)  : La factorizacion no se ha completado por
C                               ser indefinida la matriz: en la diagonal
C                               el elemento  -IND-N  es nulo y   el del
C                               lugar (-IND-N,-IND-N+1) es no nulo.
C
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: anfm03,dcopy,ddot,dipvtf,dswap,d1mach,
C                              zthz
C     FUNCIONES FORTRAN INTRINSECAS: abs,max,mod,sqrt
C
C
      implicit double precision(a-h,o-z)
      dimension h(ih,*),r(ir,*),z(iz,*),p(*),w(*),ipvt(*)
C
C     Se comprueba si los valores de las variables son correctos
C
      if(ih.lt.np .or. ir.le.n .or. iz.lt.np .or. n.lt.1 .or. m.lt.0 .or
     &. np.le.1) then
         write(io,'(10x,A)') 'INCORRECT LIST OF CALLING IN ANFM05.'
         stop
      end if
C
C     Se inicializan algnunas variables de trabajo
C
      n1=n+1
css   epsmch=d1mach(4)
      epsmch=dlamch('p')
      if(ind.eq.n1) then
         ind=ind-1
         return
      end if
C
C     Se calcula la dimension de la parte factorizada  (no nula)
C
      if(ind.gt.0 .and. ind.le.n1) then
         m2=n1-ind
      else
         m2=m
      end if
      m1=m2+1
C
C     Se calculan las cajas n11' y n21' y el vector r1
C
      n2=n1+1
      nm2=n1+m2
      k1=ipvt(1)
      if(m2.gt.0) then
         w(1)=x
      else
         w(1)=1
      end if
      do 80 i=1,n
         i1=i+1
         ni1=n1+i1
         k2=ipvt(i1)
         ni=n+i
         pni=p(ni)
         pi=p(i)
         if(i.lt.m2) then
            l=i1
         else
            l=m2
         end if
         call dcopy(l,r(1,i),1,w(n2),1)
         if(k1.lt.k2) then
            j=k1
            k1=k2
            k2=j
            do 10 k=m1,i1
10          w(n1+k)=0
            if(i.gt.m) w(ni1)=1
            call dswap(i1,w,1,w(n2),1)
            j=-1
         else
            j=0
         end if
         if(i.lt.m2) l=l-1
         ipvt(i)=k2
         do 20 k=2,l
20       r(k-1,i)=w(k)*pni-w(n1+k)*pi
         if(i.lt.m) then
            if(i.lt.m2) then
               r(i1,i)=w(1)*pni-w(n2)*pi
            else
               r(m2,i)=w(1)*pni-w(n2)*pi
            end if
            do 30 k=1,l
30          w(k)=w(k)*pi+w(n1+k)*pni
         end if
         if(i.lt.m2) then
            if(j.eq.0) then
               r(i,i)=-pi*w(ni1)
               w(i1)=pni*w(ni1)
            else
               r(i,i)=pni*w(i1)
               w(i1)=pi*w(i1)
            end if
         else if(i.ge.m) then
C
C     Se calcula el vector r2
C
            r(m2,i)=w(1)*pni-w(n2)*pi
            do 35 k=1,l
35          w(k)=w(k)*pi+w(n1+k)*pni
         end if
80    continue
C
C     Se utilizan transformaciones de Givens
C
      m3=m2-1
      do 100 i=1,m3
         i1=i+1
         ri=r(i,i)
         ri1=r(i1,i)
         if(abs(ri1).gt.epsmch) then
            s1=sqrt(ri1*ri1+ri*ri)
            s=ri1/s1
            c=ri/s1
            r(i,i)=s1
            do 90 j=i1,n
               if(j.le.m3) then
                  j1=j+1
               else
                  j1=m2
               end if
               rj1=r(j1,j)
               rij=r(i,j)
               r(i,j)=c*rij+s*rj1
               r(j1,j)=s*rij-c*rj1
90          continue
         end if
100   continue
C
C     En el caso semidefinido positivo se obtiene la factorizacion
C
      if(ind.gt.0 .and. ind.le.n1) then
         j=m2
         s=abs(r(m2,m2))
         do 110 i=m2+1,n
            s1=abs(r(m2,i))
            if(s1.gt.s) then
               s=s1
               j=i
            end if
110      continue
         if(r(m2,j).lt.-epsmch) then
            do 120 i=m2,n
120         r(m2,i)=-r(m2,i)
         end if
         if(j.ne.m2) call dipvtf(r,ir,ipvt,m2,m2,j)
      end if
      if(ind.ge.0 .and. ind.le.n1) then
C modified 15-july-2005.Before:if(ind.gt.0 .and. r(m2,m2).gt.epsmch) ind=ind-1. But see anrs01.
         if(ind.gt.0 .and. r(m2,m2).gt.epsmch**0.9) ind=ind-1
         return
       end if
C
C     Se continua la factorizacion en los demas casos
C
      ind=m3
      call anfm03(h,ih,r(1,m2),ir,z,iz,w,ipvt(m2),np,n,ind,modo,io)
C
C     Se adapta el valor de la variable  ind  en los distintos casos
C
      if(ind.le.-10*iz) then
         iibeta=1
         ind=ind+10*iz
      else
         iibeta=0
      end if
      k2=n-m3
      if(ind.lt.0 .and. ind.ge.-k2) then
         ind=ind-m3
      else if(ind.lt.-k2) then
         ind=ind-2*m3
      end if
      if(iibeta.eq.1) ind=ind-10*iz
      end
