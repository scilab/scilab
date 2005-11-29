      subroutine anfm06 (z,iz,r,ir,w,ipvt,n,m,ind,io)
C     SUBROUTINE ANFM06 (Z,IZ,R,IR,W,IPVT,N,M,IND,IO)
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
C                               MAYO  1987                             *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        Esta subrutina modifica la factorizacion de Cholesky (LL') de
C        una matriz del tipo  PAP' ,(P  es una matriz de permutacion y
C        A es del tipo Z'HZ), cuando a Z se le a¤ade una columna.  En Z
C        la columna a¤adida es la que ocupa el ultimo lugar.
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        Z      Matriz de dimension (IZ,M+1). En sus  N  primeras filas
C               contiene a la matriz  Z,  pero con las columnas en orden
C               inverso; asi, la columna a¤adida ocupa el primer lugar.
C
C        IZ     Primera dimension de  Z. IZ >= N.
C
C        R      Matriz de dimension (IR,N+1).   En la  parte  triangular
C               inferior,  (incluyendo la diagonal), tiene  almacenada a
C               la  caja  triangular  inferior de la matriz  NxN  H ; En
C               las  ultimas  columnas  y  en  las  M  primeras filas se
C               encuentra la matriz  L'.
C
C        IR     Primera dimension de la matriz  R. IR >= N.
C
C        W      Vector de trabajo de dimension  N.
C
C        IPVT   Vector entero de trabajo de dimension  M+1. Contiene, en
C               las   M   primeras coordenadas la informacion sobre la
C               permutacion  P  ; asi,
C                     IPVT(I)=J ,  indica que la fila j-esima de  A  se
C               encuentra en el lugar i-esimo de  PAP' (analogamente con
C               las columnas).
C
C        N      Numero de filas de  Z  y de  R.
C
C        M      Numero de filas y columnas de  L'.
C
C        IND    Indicador que toma los valores enteros:
C                 0    : La matriz factorizada es definida positiva o
C                        comienza la factorizacion.
C               (0,M+1): La matriz  A  es semidefinida positiva; La caja
C                        no nula de L' es de dimension M-IND.
C
C     DE SALIDA:
C
C        R     Matriz que contiene , en sus  M+1  ultimas columnas, la
C              matriz triangular superior  L'  modificada o , si no se
C              ha completado la factorizacion, contiene tambien la parte
C              no factorizada. Para conocer el contenido de  R  es util
C              observar el valor de la variable  IND  de salida.
C
C        IPVT  Vector que contiene la informacion sobre la permutacion
C              P  realizada para obtener la factorizacion de salida. Los
C              valores de  IPVT  siguen el convenio antes citado.
C
C        M     Numero de filas y columnas de la matriz  L'  de salida.
C
C        IND   Indicador que toma los valores:
C              (-1,M+1)  : Se ha obtenido la factorizacion completa. La
C                          dimension  de la caja  no nula de   L'  es
C                          M-IND.
C              (M,2*M+1) : No se ha obtenido la factorizacion. La caja
C                          no factorizada tiene la forma :
C                                                          0    V
C
C                                                          V'   b
C
C                          donde  V  es un vector y  b  es un  escalar.
C                          La dimension de esta caja es  IND-M.El vector
C                          (V',b)  esta almacenado en la ultima columna
C                          de  R  , a partir de la fila  2*M-IND+1.
C              IND > 2*M : No se ha obtenido la factorizacion . La caja
C                          no factorizada tiene la forma:
C                                                         -VV',
C                          donde  V  es un vector de dimension  IND-2*M,
C                          almacenado en la fila  3*M-IND de  R  (en las
C                          ultimas columnas).
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit" :
C                 implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: anrs01,dcopy,ddot,dipvtf,dnrm2,d1mach
C     FUNCIONES FORTRAN INTRINSECAS: abs,mod,sqrt
C
C
      implicit double precision (a-h,o-z)
      dimension z(iz,*),r(ir,*),w(*),ipvt(*)
C
C     Se comprueba si los valores de las variables son correctos
C
      if(ind.lt.0 .or. ind.gt.m .or. m.ge.n) then
         write(io,'(10x,A)') 'INCORRECT LIST OF CALLING IN ANFM06.'
         stop
      end if
C
C     Se inicializan algunas variables de trabajo
C
css   epsmch=d1mach(4)**0.75
      epsmch=dlamch('p')**0.75
      n1=n+1
      m1=m+1
      m2=m1+1
      nm=n1-m
C
C     Se calcula el producto de la matriz H por el ultimo vector anadido
C     a la base del nucleo
C
      do 10 i=1,n
         i1=i+1
         s=ddot(i,r(i,1),ir,z(1,1),1)
         if(i.lt.n) w(i)=s+ddot(n-i,r(i1,i),1,z(i1,1),1)
10    continue
      w(n)=s
C
C     Se calcula el elemento (m+1,m+1) de la matriz a factorizar
C
      s=ddot(n,w,1,z(1,1),1)
C
C     Se coloca adecuadamente la matriz  L' (inicial) en  R  para poder
C     almacenar en las ultimas columnas de  R  la matriz  L'  modificada
C
      k=0
      do 20 i=nm,nm+m-1
         k=k+1
         call dcopy(k,r(1,i+1),1,r(1,i),1)
20    continue
C
C     Se calculan los  m  primeros elementos de la ultima columna de la
C     matriz a factorizar
C
      do 30 i=1,m
30    r(i,n1)=ddot(n,w,1,z(1,m2-i),1)
      do 40 i=1,m
40    w(i)=r(ipvt(i),n1)
      ipvt(m1)=m1
C
C     Se obtiene la nueva columna de la matriz triangular superior de la
C     factorizacion de Cholesky
C
      m3=m-ind
      if(m3.gt.0) then
         call anrs01(r(1,nm),ir,m3,w,r(1,n1),1,io)
         s=s-ddot(m3,r(1,n1),1,r(1,n1),1)
      end if
C
C     Se calcula el vector que proporcionara una caja no factorizada,en
C     caso de que esta exista
C
      k1=0
      if (ind.gt.0) then
          k=n-ind
          if(m3.gt.0) then
             do 50 i=1,ind
                j=m3+i
                r(j,n1)=w(j)-ddot(ind,r(1,k+i),1,r(1,n1),1)
50           continue
          else
             call dcopy(ind,w,1,r(1,n1),1)
          end if
          rnorma=dnrm2(ind,r(m3+1,n1),1)
          if(rnorma.lt.epsmch) k1=1
      end if
C
C     Se distinguen varios casos,segun sea el nuevo elemento diagonal:
C     positivo,negativo o nulo
C
      if(s.gt.epsmch) then
         s=sqrt(s)
         r(m1,n1)=s
         if(ind.gt.0) then
            if(k1.eq.0) then
               do 60 i=m3+1,m
60             r(i,n1)=r(i,n1)/s
               ind=ind+2*m1
            end if
            m2=m3+1
            call dipvtf(r(1,nm),ir,ipvt,m3,m1,m2)
            nm1=n1-m
            r(m2,m2+n-m)=s
            do 70 i=m2,m
70          r(m2,i+nm1)=r(i,n1)
         end if
      else
         r(m1,n1)=s
         if(s.lt.-epsmch) then
            if(ind.eq.0) then
               ind=-m1
            else
               ind=ind+1+m1
            end if
         else
            if(ind.eq.0) then
               ind=1
            else if(k1.eq.1) then
               ind=ind+1
            else
               ind=ind+1+m1
            end if
         end if
      end if
      m=m1
      end
