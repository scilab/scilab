      SUBROUTINE OPTR03(A,IA,C,IC,Q,IQ,R,IR,P,B,D,CI,CS,X,W,IW,IRE,IPVT,
     &                  JPVT,ALFA,IRA,N,M,MI,MI1,MD,MIF,MDF,MODO,IND,IMP
     &                  ,IO,ITER)
C
C!PURPOSE
C        La minimizacion  de un funcional cuadratico  con restricciones
C        lineales,  (de acotacion, igualdad  y  desigualdad), sobre las
C        variables:
C
C          (1)  CI(I) <= X(I) <= CS(I), I=1,N
C          (2)  <(C(1,J),C(2,J),...,C(N,J)),X>  = D(J), J=1,MI
C          (3)  <(C(1,J),C(2,J),...,C(N,J)),X> <= D(J), J=MI+1,MI+MD
C
C        El  funcional  puede  presentar  terminos  lineales  a  trozos
C        correspondientes  a  ciertas  restricciones  de  igualdad  y
C        desigualdad :
C
C          (1F) <(A(1,J),A(2,J),...,A(N,J)),X>  = B(J), J=1,MIF
C          (2F) <(A(1,J),A(2,J),...,A(N,J)),X> <= B(J), J=1+MIF,MIF+MDF
C
C        tomando la forma:
C
C               F(X)= ALFA*( 1/2*X'*H*X + P'*X ) +
C                    +( ABS(A'*X-B)) + ( MAX(A'*X-B,0))
C
C
C!     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        A      Matriz de dimension  (IA,MIF+MDF).   En sus  N  primeras
C               filas contiene los coeficientes de las restricciones que
C               dan lugar a terminos (lineales a trozos) en el funcional
C               Los  de  igualdad  se  almacenan  en  las  MIF  primeras
C               columnas y los de desigualdad en el resto.
C
C        IA     Primera dimension de A. IA >= N.
C
C        C      Matriz  de  dimension  (IC,MI+MD).   En sus  N  primeras
C               filas  contiene  los  coeficientes  de  restricciones de
C               igualdad y desigualdad no presentes en el funcional. Los
C               de igualdad se almacenan en las  MI  primeras columnas y
C               los de desigualdad en las restantes.
C
C        IC     Primera dimension de C. IC >= N.
C
C        Q      Matriz de trabajo de dimension  (IQ,N). Si  MODO=-2  o
C               MODO= 0, 3  o  5, contiene la matriz ortogonal  de la
C               factorizacion  QR  de la matriz de coeficientes de las
C               restricciones  recomendadas  o  de  las  restricciones
C               activas.
C
C        IQ     Primera dimension de la matriz  Q. IQ >= N.
C
C        R      Matriz de trabajo de dimension  (IR,NR), donde se tomara
C               NR= MAX(N,MI)+1 .  En las  N  primeras columnas contiene
C               la  matriz  H  del  termino  cuadratico.  Es  suficiente
C               suministrar la parte triangular inferior. Si  MODO= -2
C               o  MODO= 0, 3  o  5,  a partir de la segunda columna se
C               suministra  la parte  no nula  de la matriz triangular
C               superior  de  la  factorizacion  QR  de  la  matriz  de
C               coeficientes  de las restricciones recomendadas o de las
C               restricciones activas.  El resto de la matriz se utiliza
C               como area de trabajo.
C
C        IR     Primera dimension de la matriz  R. IR >= N.
C
C        P      Vector  N-dimensional.  Contiene  los  coeficientes  del
C               termino lineal del funcional.
C
C        B      Vector de dimension  MIF+MDF. Contiene los coeficientes
C               de los terminos independientes de las restricciones que
C               originan terminos en el funcional.
C
C        D      Vector de dimension  MI+MD.  Contiene  los  coeficientes
C               de los  terminos  independientes  de  las  restricciones
C               de igualdad y desigualdad ajenas al funcional.
C
C        CI     Si  IRA= 0  o  2  esta  variable  no sera  utilizada. Si
C               IRA= 1  o  3,  CI   sera  un  vector  de  dimension   N
C               conteniendo  las  cotas  inferiores de  X.  Si  X(I)  no
C               esta acotado inferiormente,  CI(I)  debera ser menor que
C               la raiz  cuadrada  negativa  de la constante  real  mas
C               grande de la maquina.
C
C        CS     Si  IRA= 0  o  1  esta  variable  no sera  utilizada. Si
C               IRA= 2  o  3,  CS  sera  un  vector  de  dimension   N ,
C               conteniendo  las  cotas  superiores  de  X. Si  X(I)  no
C               esta acotado superiormente, CS(I)  debera ser mayor que
C               la raiz cuadrada  de la constante real mas grande  de la
C               maquina.
C
C        X      Vector N-dimensional. Si  MODO <> 1, 6 contiene un punto
C               admisible para las restricciones externas al funcional.
C
C        W      Vector de trabajo de dimension :
C                N1+MAX(N+MD,N2), si  MODO= 1, 6
C                N1+MAX(MI,N2), si  MODO= 0, -1, -2, 2, 4
C                N1+N2,  si  MODO= 3, 5
C               con
C                N1=2*N+MD
C                N2=N+MIF+MDF+MAX(2*N-2,MD+MIF+MDF)
C
C        IW     Indicador que toma los valores:
C                  1  : Se obtiene el valor del funcional  al final del
C                       proceso si en salida  IND=0.
C                  0  : No se obtiene tal valor.
C
C        IRE    Vector entero de dimension  N+MD+MIF+MDF. Este vector se
C               facilita si  MODO=  -2, -1, 0, 3  o  5.
C               -Si  MODO= 3  o  5, indica como son las restricciones al
C               comienzo del proceso, en la forma siguiente:
C                 Si  1 <= I <= N,
C                  IRE(I)=  0  : Restriccion  de  acotacion  que  se
C                                satisface estrictamente.
C                  IRE(I)= -1  : Cota inferior activa.
C                  IRE(I)=  1  : Cota superior activa.
C                 Si  N < I <= N+MD,
C                  IRE(I)=  0  : Restriccion  de  desigualdad  (ajena al
C                                funcional) satisfecha estrictamente.
C                  IRE(I)=  1  : Restriccion activa.
C                 Si  N+MD < I <= N+MD+MIF+MDF,
C                  IRE(I)=  0  : Restricion (asociada al funcional) que
C                                se satisface estrictamente.
C                  IRE(I)=  1  : Restriccion activa.
C                  IRE(I)=  2  : Restriccion violada  (a'x-b > 0).
C                  IRE(I)= -2  : Restriccion  ( de  igualdad )  violada
C                                (a'x-b < 0).
C               -Si   MODO < 1,   se   indica   que   restricciones   se
C               recomiendan como activas, en la forma siguiente:
C                 Si  1 <= I <= N,
C                  IRE(I)=  0  : Restriccion de acotacion no recomendada
C                  IRE(I)= -1  : Cota inferior activa.
C                  IRE(I)=  1  : Cota superior activa.
C                 Si  N < I <= N+MD+MIF+MDF,
C                  IRE(I)=  0  : Restriccion (de desigualdad  o asociada
C                                al funcional) no recomendada.
C                  IRE(I)=  1  : Restriccion recomendada como activa.
C
C        IPVT   Vector entero  de trabajo  de dimension:
C                M+MAX(MIF+MDF,1) ,  si  MODO= 3  o  5
C                N+MAX(MI+MD+1,MIF+MDF), en otro caso.
C               Si  MODO= -2, 0, 3  o  5, en las  M primeras coordenadas
C               indica las restricciones que son recomendadas o activas:
C                Si  1 <= I <= MI1,
C                 IPVT(I)=J      indica  que  la  i-esima  restriccion
C                                activa es la  j-esima de igualdad.
C                Si  MI1 < I <= M,
C                 IPVT(I) <= N :
C                                Indica que la restriccion i-esima es de
C                                acotacion  inferior  si  IPVT(I) < 0, o
C                                de acotacion superior si  IPVT(I) > 0.
C                 IPVT(I) <= N+MD :
C                                Hace referencia a la restriccion numero
C                                IPVT(I)-N de desigualdad
C                 IPVT(I) <= N+MD+MIF+MDF
C                                Indica la restriccion IPVT(I)-N-MD del
C                                funcional, (de igualdad si IPVT(I) <=
C                                N+MD+MIF o de desigualdad).
C
C        JPVT   Vector entero de trabajo de dimension:
C                  N-MI1      si  MODO= 3  o  5
C                  N          en otro caso.
C
C        ALFA   Coeficiente de penalizacion asociado al funcional.
C
C        IRA    Variable  que  indica  si  existen  restricciones  de
C               acotacion en el problema. Toma los valores:
C                  0  : No existen restricciones de acotacion.
C                  1  : Se  tienen  solo  restricciones  de  acotacion
C                       inferior.
C                  2  : Existen solo restricciones de acotacion superior
C                  3  : Se tienen ambos tipos de restriccion.
C
C        N      Numero de variables del problema.
C
C        M      Indica el numero de restricciones recomendadas o activas
C               del problema. Se facilita si  MODO= -2, 0, 3  o  5.
C
C        MI     Numero de restricciones de igualdad del problema (ajenas
C               al funcional).
C
C        MI1    Numero de restricciones de igualdad linealmente indepen-
C               dientes. Se facilita con los  MODOS  -2, 0, 3  y  5.
C
C        MD     Numero  de  restricciones  de desigualdad  del  problema
C               (ajenas al funcional).
C
C        MIF    Numero de restricciones de igualdad que pueden originar
C               terminos en el funcional.
C
C        MDF    Numero de restricciones de desigualdad presentes en el
C               funcional.
C
C        MODO   Variable que indica el modo de comienzo del proceso con
C               los valores:
C                 -2  : Se  facilita  un  punto  inicial,  restricciones
C                       recomendadas  como activas, la factorizacion  QR
C                       de la matriz  de coeficientes  de  restricciones
C                       recomendadas y la factorizacion de Cholesky  del
C                       hessiano proyectado.
C                 -1  : Se suministra un punto inicial y  se recomiendan
c                       como  activas algunas restricciones, pero no se
C                       facilita la factorizacion  QR.
C                  0  : Se  facilita  un punto inicial  y  restricciones
C                       recomendadas como activas y la factorizacion  QR
C                       de la matriz  de coeficientes  de  restricciones
C                       recomendadas.
C                  1  : No  se  suministra  ni  un  punto  inicial,  ni
C                       restricciones recomendadas, ni factorizacion QR.
C                  2  : Se facilita un punto inicial.
C                  3  : Se da un punto inicial y la factorizacion  QR de
C                       las restricciones activas.
C                  4  : Se suministra un vertice como punto inicial.
C                  5  : Se  da  un vertice  como punto  de partida  y la
C                       factorizacion  QR  de las restricciones activas.
C                  6  : Igual  que  MODO = 1,  pero la subrutina trabaja
C                       de forma diferente, (calcula  como punto inicial
C                       un vertice)
C
C               Si  MODO <> 1, 6   el  punto  suministrado  ha  de  ser
C               admisible para las restricciones externas al funcional.
C               El  caso  de  vertice,  mencionado  anteriormente,  esta
C               referido tambien a estas restricciones.
C
C        IMP    Indicador del nivel de impresion de salida de resultados
C               Toma los valores:
C                  6  : Sin salida de resultados.
C                  7  : Escribe el motivo de finalizacion del proceso.
C                  8  : Se obtiene informacion referente  a la solucion:
C                       el numero de iteraciones; el  optimo  calculado,
C                       la norma del vector de Kuhn-Tucker, el numero de
C                       restricciones  activas  y  cuales   son    (para
C                       identificar estas restricciones  se  sigue el
C                       siguiente convenio para  los  valores   que   se
C                       obtienen:
C
C                       [-N,-1]         : Restriciones de cota inferior
C                       [ 1,N ]         : Restricciones de cota superior
C                       [N+1,N+MI]      : Restricciones de igualdad
C                       [N+MI+1,N+MI+MD]: Restricciones de desigualdad)
C
C                       y los multiplicadores de  Lagrange  asociados  a
C                       ellas  (si IND=0);  el valor  del funcional  (si
C                       IND=0  con  IW=1  o  IND=-3); la norma del punto
C                       calculado (si  IND=-3).
C                  9  : Se obtiene  informacion referente  al desarrollo
C                       de las iteraciones:  el numero de  restricciones
C                       activas  y  cuales  son   (vector   IPVT),   las
C                       restricciones  asociadas  al  funcional  que  se
C                       violan (vector  IRE, si existen), la restriccion
C                       que se anade o elimina  del conjunto activo,  el
C                       tipo  de  direccion  de descenso calculada y las
C                       iteraciones con punto degenerado.
C               > 10  : Ademas,  se  obtiene  el punto calculado en cada
C                       iteracion e informacion sobre el proceso llevado
C                       a cabo por OPTR01. (Ver valores posibles de  IMP
C                       en  OPTR01).
C
C        IO     Numero de canal de salida de resultados.
C
C     DE SALIDA:
C
C        Q      Contiene la matriz  Q  ortogonal de la factorizacion  QR
C               de la matriz de coeficientes de restricciones activas.
C
C        R      En  R  se tiene  la matriz  triangular  superior  de  la
C               factorizacion  QR  ,(columnas  2  a  M+1).   La   parte
C               triangular inferior de las  N  primeras columnas  no se
C               ha modificado.
C
C        IRE    Indica de que tipo son las restricciones en el punto  X
C               obtenido. Los valores de  IRE  siguen el convenio citado
C               en la entrada.
C
C        IPVT   Indica, en sus M primeras coordenadas, las restricciones
C               linealmente independientes que son activas en el punto X
C               Los  valores  de  IPVT  siguen  el  convenio  citado  en
C               entrada.
C
C        X      Vector que contiene el optimo calculado (si IND=0)  o la
C               ultima aproximacion obtenida.
C
C        IW     Indica, si en entrada  IW=1, la coordenada de  W  donde
C               se encuentra el valor  del  funcional  en  el  optimo.
C               Si ademas  IMP >= 8,  en la coordenada  IW+1  de  W  se
C               almacena el valor de la norma del vector de Kuhn-Tucker.
C
C        M      Numero de restricciones activas en el punto  X.
C
C        IND    Variable que indica el motivo por el cual se finaliza el
C               proceso. Toma el valor:
C                  1  : Se ha superado  el limite  de iteraciones fijado
C                       por el programa.
C                  0  : Se ha finalizado el proceso sin problemas.
C                 -1  : El funcional no esta acotado inferiormente.
C                 -2  : Se  encuentra  un punto  degenerado  con ciclaje
C                       indefinido.
C                 -3  : Posible problema no acotado.  La distancia entre
C                       los puntos  de dos iteraciones  consecutivas  es
C                       "demasiado grande".
C                 -4  : Los datos suministrados son incorrectos.
C                -11  : Incompatibilidad en el sistema de restricciones
C                       de igualdad en  OPTR01.
C                -12  : No existen puntos admisibles en  OPTR01 para las
C                       restricciones ajenas al funcional.
C                -13  : Se encuentra  un punto  degenerado  con  ciclaje
C                       indefinido en  OPTR01.
C                -14  : Se  ha  realizado  el limite  de iteraciones  en
C                       OPTR01 sin encontrar un punto admisible para las
C                       restricciones ajenas al funcional.
C
C        ITER   Numero de iteraciones realizadas.
C
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C!    SUBPROGRAMAS AUXILIARES:
C                anfm01,anfm02,anfm03,anfm04,anfm05,anfm06
C                anrs01,anrs02,auxo01,aux003,dadd,daxpy,dcopy,ddif,ddot,
C                desr03,dimp03,dipvtf,dmmul,dnrm0,dnrm2,dscal,dswap,
C                dlamch,idamax,nvkt03,optr01,opvf03,pasr03,tol03,zthz
C     FUNCIONES FORTRAN INTRINSECAS: abs,max,min,mod,sign,sqrt
C! ORIGEN:
C***********************************************************************
C                                                                      *
C                                                                      *
C                       Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,Estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                            OCTUBRE  1989                             *
C                                                                      *
C***********************************************************************
C
      implicit double precision(a-h,o-z)
      dimension c(ic,*),a(ia,*),p(*),x(*),d(*),b(*),ci(*),cs(*),q(iq,*),
     &          r(ir,*),w(*),ire(*),ipvt(*),jpvt(*)
C
C     Se comprueba si los valores de las variables son correctos
C
      if((ic.lt.n.and.(mi.gt.0.or.md.gt.0)) .or. n.le.1 .or. ir.lt.n .or
     &   . ((mif.gt.0.or.mdf.gt.0).and.ia.lt.n) .or. iq.lt.n .or. modo.
     &   lt.-1 .or. modo.gt.6 .or. mi.lt.0 .or. md.lt.0 .or. mif.lt.0 .
     &   or. mdf.lt.0 .or. ira.lt.0 .or. ira.gt.3 .or. io.lt.1) then
         ind=-4
         call dimp03(x,w,ire,ipvt,s,0,0,0,0,0,0,0,0,0,ind,imp,
     &               io,iter)
         return
      end if
C
C     Se toman algunos parametros de trabajo
C
      epsmch=dlamch('p')
      eps=epsmch**0.75
      eps0=epsmch**0.9
      gigant=dlamch('o')
      gig1=sqrt(gigant)
C
C     Se comprueba que los vectores  CI,CS,IRE toman valores correctos
C
      if(ira.gt.0) then
         do 10 i=1,n
            if(ira.eq.3) then
               if(ci(i).ge.-gig1 .and. cs(i).le.gig1 .and. ci(i).
     &            gt.cs(i)) then
                  if(imp.ge.7) call dimp03(x,w,ire,ipvt,s,i,0,0,0,0,
     &                                   0,0,0,0,-24,imp,io,iter)
                  ind=-4
                  return
               end if
            end if
            if(modo.eq.3 .or. modo.eq.5 .or. modo.le.0) then
               if(ire(i).lt.-1 .or. ire(i).gt.1) then
                  if(imp.ge.7) call dimp03(x,w,ire,ipvt,s,0,0,0,0,
     &                                0,0,0,0,0,-34,imp,io,iter)
                  ind=-4
                  return
               end if
            end if
10       continue
      end if
      if(modo.eq.3 .or. modo.eq.5 .or. modo.le.0) then
         do 20 i=n+1,n+md+mif+mdf
            if(((ire(i).lt.0 .or. ire(i).gt.1) .and. i.le.n+md) .or.
     &         ((ire(i).lt.-2 .or. ire(i).gt.2) .and. i.gt.n+md)) then
               if(imp.ge.7) call dimp03(x,w,ire,ipvt,s,0,0,0,0,0,
     &                                  0,0,0,0,-34,imp,io,iter)
               ind=-4
               return
            end if
20       continue
      end if
C
C     Se inicializan algunas variables enteras de trabajo
C
      if(modo.gt.3) then
         iver=1
         if(modo.eq.6) then
            modo=1
         else
            modo=modo-2
         end if
      else
         iver=-1
      end if
      n1=n+1
      n2=n1+n
      n3=n2+n
      n10=10*iq
      if(modo.ne.3 .or. modo.ne.0) mi1=mi
      mid=mi+md
      midf=mif+mdf
      nmd=n+md
      nmdi=nmd+mif
      nd1=nmd+1
      nd=nmd+midf+1
      icd=nd+n
      iad=icd+md
      idw=iad+midf
      iter=0
      id=0
      ind=0
      icicla=0
      il=0
      icol=0
      icol1=0
      icol2=0
      iicol=0
      info=0
      itemax=4*(n+mid+midf)
      icont=0
      if(ira.eq.0) then
         do 30 i=1,n
30       ire(i)=0
      end if
C
C     Si se han recomendado activas ciertas restricciones y no se ha
C     suministrado la factorizacion  QR , (MODO=-1), se procede a su
C     calculo
C
      if(modo.eq.-1) then
         if(mi.eq.0) m=0
         if(mid.eq.0) then
            do 34 i=1,n
               do 32 j=1,n
                  if(i.eq.j) then
                     q(i,j)=1
                  else
                     q(i,j)=0
                  end if
32             continue
34          continue
         end if
         if(mid.eq.0 .and. ira.gt.0) then
            do 36 i=1,n
               if(ire(i).eq.1) ind=i
               if(ire(i).eq.-1) ind=-i
               if(ind.ne.0) then
                  m=m+1
                  call anfm01(q,iq,r(1,2),ir,w,w,n,m,ind,io)
                  ipvt(m)=ire(i)*i
               end if
36          continue
         else if(mid.gt.0) then
            modo=22
            call optr01(c,ic,q,iq,r(1,2),ir,ci,cs,d,x,w,ipvt,ire,ira,n,
     &                  m,mi,mi1,md,ind,imp,io,modo)
            modo=-1
         end if
         if(midf.gt.0) then
            i=1
            if(i.le.midf .and. m.lt.n) then
 1000          ni=nmd+i
               if(ire(ni).eq.1) then
                  m1=m+1
                  ind=0
                  call anfm01(q,iq,r(1,2),ir,a(1,i),w,n,m1,ind,io)
                  if(ind.lt.0) then
                     ire(ni)=0
                  else
                     m=m1
                     ipvt(m)=ni
                  end if
               end if
               i=i+1
               if(i.le.midf .and. m.lt.n) go to 1000
            end if
         end if
      end if
C
C     Si  se  han  recomendado  restricciones  activas, (MODO <= 0), se
C     proyecta el punto suministrado sobre la variedad de restricciones
C     recomendadas
C
      if(modo.le.0) then
         i1=idamax(n,x,1)
         s1=x(i1)
         if(s1.eq.0.0d+0) then
            do 38 i=1,mi1
38          w(i)=d(ipvt(i))
            do 40 i=mi1+1,m
               l=ipvt(i)
               if(l.lt.0) then
                  w(i)=-ci(-l)
               else if(l.le.n) then
                  w(i)=cs(l)
               else if(l.le.nmd) then
                  w(i)=d(mi+l-n)
               else
                  w(i)=b(l-nmd)
               end if
40          continue
         else
            do 42 i=1,mi1
               l=ipvt(i)
               w(i)=d(l)-ddot(n,c(1,l),1,x,1)
42          continue
            do 44 i=mi1+1,m
               l=ipvt(i)
               if(l.lt.0) then
                  w(i)=-ci(-l)+x(-l)
               else if(l.le.n) then
                  w(i)=cs(l)-x(l)
               else if(l.le.nmd) then
                  ni=mi+l-n
                  w(i)=d(ni)-ddot(n,c(1,ni),1,x,1)
               else
                  ni=l-nmd
                  w(i)=b(ni)-ddot(n,a(1,ni),1,x,1)
               end if
44          continue
         end if
         ind=1
         call anrs01(r(1,2),ir,m,w,w(n1),ind,io)
         ind=0
         call dmmul(q,iq,w(n1),m,w,n,n,m,1)
         call dadd(n,x,1,w,1)
C
C     Se  verifica  si  el  punto  obtenido  al  proyectar  cumple  las
C     restricciones no activas  y  ajenas al funcional;  recomenzando el
C     proceso  (con  MODO=2) si el resultado es negativo
C
         iv=0
         if(mid.gt.0 .or. ira.gt.0) then
            i1=mi+1
            call auxo01(c(1,i1),ic,ci,cs,d(i1),w,w(n1),ire,ira,n,md,ind,
     &                  fun,iv)
         end if
         if(iv.eq.0) then
            call dcopy(n,w,1,x,1)
         else
            modo=2
         end if
      end if
C
C     Si no existe factorizacion  QR  de las restricciones activas, ni
C     restricciones de igualdad o desigualdad fuera del funcional, se
C     inicializan la matriz  Q   (MODO= 1 o 2)
C
      if(mid.eq.0 .and. (modo.eq.2 .or. (modo.eq.1 .and. ira.eq.0)))then
         m=0
         do 60 i=1,n
            do 50 j=1,n
               if(i.eq.j) then
                  q(i,j)=1
               else
                  q(i,j)=0
               end if
50          continue
60       continue
      end if
C
C     Si  no  se  da  un  punto  inicial, (modo=1), se calcula  un punto
C     admisible junto con la factorizacion  QR  de las  correspondientes
C     restricciones activas y los vectores  ire  e  ipvt.
C
      if(modo.eq.1) then
         do 70 i=1,n
70       x(i)=0
         if(mid.gt.0 .or. ira.gt.0) then
            if(iver.eq.1) modo=11
            call optr01(c,ic,q,iq,r(1,2),ir,ci,cs,d,x,w,ipvt,ire,ira,n,
     &                  m,mi,mi1,md,ind,imp,io,modo)
            if(iver.eq.1) modo=1
            if(ind.lt.0) then
               ind=ind-10
               if(imp.ge.7 .and.imp.le.10) call dimp03(x,w,ire,ipvt,s,0
     &                         ,0,0,0,0,0,0,0,0,ind,imp,io,iter)
               return
            end if
            call dcopy(md,w(n3),1,w(n1),1)
         end if
      end if
C
C     Se calcula la factorizacion  QR  de las restricciones activas en
C     caso de suministrarse un punto inicial (modo=2)
C
      if(modo.eq.2) then
         m=0
         if(ira.ge.1) then
            do 90 i=1,n
               ire(i)=0
               if(ira.ne.2) then
                  if(ci(i).ge.-gig1) then
                     if(x(i).lt.(ci(i)+eps)) then
                        x(i)=ci(i)
                        ire(i)=-1
                        if(mid.eq.0) then
                           m=m+1
                           ip=-i
                           call anfm01(q,iq,r(1,2),ir,x,w,n,m,ip,io)
                           ipvt(m)=-i
                        end if
                     end if
                  end if
               end if
               if(ira.ge.2) then
                  if(cs(i).le.gig1 .and. ire(i).eq.0) then
                     if(x(i).gt.(cs(i)-eps)) then
                        x(i)=cs(i)
                        ire(i)=1
                        if(mid.eq.0) then
                           m=m+1
                           ip=i
                           call anfm01(q,iq,r(1,2),ir,x,w,n,m,ip,io)
                           ipvt(m)=i
                         end if
                     end if
                  end if
               end if
90          continue
         end if
         do 100 i=1,md
            ii=mi+i
            s=ddot(n,c(1,ii),1,x,1)-d(ii)
            ni=n+i
            if(s.gt.-eps) then
               ire(ni)=1
            else
               ire(ni)=0
            end if
            w(ni)=s
100      continue
         if(mid.gt.0) then
            modo=22
            call optr01(c,ic,q,iq,r(1,2),ir,ci,cs,d,x,w(nd1),ipvt,ire,
     &                  ira,n,m,mi,mi1,md,ind,imp,io,modo)
            modo=2
         end if
      end if
      iv=0
      if(iver.eq.1) then
         iv=m
         iver=m
      end if
C
C     Se calcula el vector  IRE  asociado a las restricciones presentes
C     en el funcional (si MODO <= 2)
C
      if(modo.eq.3) then
         do 110 i=1,md
            ii=mi+i
            ni=n+i
            w(ni)=0
            if(ire(ni).eq.0) w(ni)=ddot(n,c(1,ii),1,x,1)-d(ii)
110      continue
         do 120 i=1,midf
            ii=abs(ire(nmd+i))
            if((i.le.mif .and. ii.eq.2) .or. (ii.ne.1)) then
               w(nmd+i)=ddot(n,a(1,i),1,x,1)-b(i)
            else
               w(nmd+i)=0
            end if
120      continue
      else
         do 130 i=1,midf
130      w(nmd+i)=0
         ind=0
         if(modo.gt.0) then
            do 140 i=nd1,nmd+midf
140         ire(i)=0
         end if
         call aux003(a,ia,x,b,q,iq,r(1,2),ir,w(nd1),ire(nd1),ipvt,nmd,
     &               mif,mdf,midf,n,m,ind,io)
      end if
      if(m.eq.n) then
         minimo=1
         id=2
         nm=0
      else
         minimo=0
      end if
C
C     Se calcula la matriz  Z'HZ  y se factoriza
C
      if(minimo.eq.0 .and. modo.ne.-2) then
         m1=m+1
         nm=n-m
         ind=0
         call anfm03(r,ir,r(1,m1+1),ir,q(1,m1),iq,w,jpvt,n,nm,ind,
     &               iver-iv,io)
         if(ind.le.-n10) then
            ind=ind+n10
            iibeta=1
         else
            iibeta=0
         end if
         if(ind.eq.n .and. iver.eq.iv) then
            ind=-1
            if(imp.ge.7) call dimp03(x,w,ire,ipvt,s,0,0,0,0,0,
     &                               0,0,0,0,ind,imp,io,iter)
            return
         end if
      else if(minimo.eq.0) then
         nm=n-m
      end if
C
C     Comienzan las iteraciones
C
      if(iter.le.itemax) then
2000     iadd=0
         ind1=0
         il=0
         if(iicol.eq.1) id=2
C
C     Se calcula el gradiente del funcional si no hay ciclaje
C
         if(icicla.eq.0) then
            do 160 i=1,n
               i1=i+1
               s=ddot(i,r(i,1),ir,x,1)
               if(i.lt.n) w(i)=s+ddot(n-i,r(i1,i),1,x(i1),1)
160         continue
            w(n)=s
            call dadd(n,p,1,w,1)
            if(alfa.ne.1.0d+0) call dscal(n,alfa,w,1)
            do 180 i=1,mif
               ni=nmd+i
               if(ire(ni).eq.2) then
                  call dadd(n,a(1,i),1,w,1)
               else if(ire(ni).eq.-2) then
                  call ddif(n,a(1,i),1,w,1)
               end if
180         continue
            do 190 i=mif+1,midf
190         if(ire(i+nmd).eq.2) call dadd(n,a(1,i),1,w,1)
         end if
C
C     Se calculan los multiplicadores de Lagrange si el paso dado en la
C     iteracion anterior conduce a un punto estacionario (id=2 o id=3)
C     y si se verifica el test sobre el gradiente proyectado
C
         s1=gigant
         s2=0
         inul=0
         if(id.ge.2) then
            i1=icd-1
            do 200 i=1,nm
200         w(i1+i)=ddot(n,q(1,n1-i),1,w,1)
            if(minimo.eq.0) s2=dnrm2(nm,w(icd),1)/(1+dnrm0(n,x,1))
            if(s2.ge.eps0) then
               info=10
               icont=icont+1
            else if(icont.gt.0) then
               icont=0
            end if
            if((m.gt.mi1 .or. ((imp.ge.8 .or. iw.eq.1) .and. m.gt.0))
     &         .and. (icont.eq.0 .or. icont.eq.3)) then
               do 220 i=1,m
220            w(nd+i-1)=-ddot(n,q(1,i),1,w,1)
               call anrs01(r(1,2),ir,m,w(nd),w(nd),2,io)
               if(m.gt.mi1) then
C
C     Se verifica si son correctos los valores de los multiplicadores
C     de Lagrange
C
                  indm=1
                  if(indm.eq.1) then
3000                 icol=0
                     j=nd-1+mi1
                     do 230 i=mi1+1,m
                        j=j+1
                        k=ipvt(i)
                        if(k.le.nmd) then
                           s=w(j)
                        else if(k.gt.nmd .and. k.le.nmdi) then
                           s=1.0d+0-abs(w(j))
                        else
                           sw=w(j)
                           s=min(sw,1.0d+0-sw)
                        end if
                        if(s.lt.s1) then
                           s1=s
                           icol=i
                        end if
230                  continue
                     if(icol.ne.0) then
                        if(ipvt(icol).gt.nmd) inul=1
                     end if
                     if(s1.lt.-eps .or. (s1.le.eps .and. inul.eq.0))
     &then
                        if(inul.eq.0 .and. abs(s1).le.eps)
     &                  call dcopy(m,w(nd),1,w(icd),1)
                        if(icont.gt.0) icont=0
C
C     Se elimina del conjunto activo la restriccion correspondiente a la
C     columna  ICOL  y se modifican la factorizacion  QR correspondiente
C     a las restricciones activas  y  la factorizacion de Cholesky de la
C     matriz  Z'HZ.
C
                        if(m.gt.1) call anfm02(q,iq,r(1,2),ir,n,m,icol,
     &io)
                        m1=m-1
                        il=ipvt(icol)
                        if(il.gt.n) w(il)=0
                        s=w(nd+icol-1)
                        ire(abs(il))=0
                        do 240 j=icol,m1
240                     ipvt(j)=ipvt(j+1)
                        if(minimo.eq.1) then
                           ind=0
                           nm=0
                        end if
                        call anfm06(q(1,m),iq,r,ir,w(nd),jpvt,n,nm,ind,
     &io)
                        info=1
                        m=m1
                        if(iver.ne.-1 .and. il.le.nmd) then
                           iver=iver-1
                        else if(iver.eq.iv .and. ind.lt.0) then
                           ind=-1
                           if(imp.ge.7) call dimp03(x,w,ire,ipvt,s,0,
     &                         0,0,0,0,0,0,0,0,ind,imp,io,iter)
                           return
                        end if
                     end if
                     if((ind.lt.0 .or. ind.gt.nm) .or. abs(s1).gt.eps)
     &then
                        indm=0
                     else
                        s1=gigant
                        call dcopy(icol-1,w(icd),1,w(nd),1)
                        j1=nd+icol-1
                        do 243 j=icd+icol,icd+m
                           w(j1)=w(j)
                           j1=j1+1
 243                    continue
                     end if
                     if(indm.eq.1) go to 3000
                  end if
               end if
            end if
         end if
C
C     Impresion de los resultados obtenidos si x es un minimo local
C
         if(id.ge.2 .and. (s1.gt.eps .or. (s1.ge.-eps.and.inul.eq.1))
     & .and. (s2.lt.eps0 .or. icont.ge.3)) then
            call tol03(q,iq,r(1,2),ir,c,ic,d,a,ia,b,ci,cs,x,w(nd+m),
     &                 ipvt,n,m,mi,mi1,nmd,io)
            ind=0
            if(iw.ne.0) then
               iw=nd+m
               w(iw)=opvf03(r,ir,a,ia,p,b,x,w,alfa,nd,n,mif,mdf)
            end if
            if(imp.ge.8) then
               if(iw.ne.0) s=w(iw)
               call nvkt03(a,ia,c,ic,w,w(nd),w(nd+m),ipvt,dnorma,n,m,
     &                     mi1,mi,nmd,nd)
               if(iw.ne.0) then
                  w(iw)=s
                  w(iw+1)=dnorma
               end if
            end if
            if(imp.ge.7) call dimp03(x,w,ire,ipvt,dnorma,n,m,nd,iw,0,
     &                               0,0,mi,mi1,ind,imp,io,iter)
            return
         end if
C
C     Si  se  ha  eliminado  una  restriccion  asociada  al  funcional
C     se modifica el vector gradiente calculado
C
         if(il.gt.nmd) then
            k=il-nmd
            if(s.gt.0.0d+0) then
               call dadd(n,a(1,k),1,w,1)
            else if(s.lt.0.0d+0 .and. il.le.nmdi) then
               call ddif(n,a(1,k),1,w,1)
            end if
         end if
C     Se calcula una direccion de descenso
C
         m1=m+1
         m2=m1+1
         call desr03(q(1,m1),iq,r(1,m2),ir,w,w(icd),w(nd),alfa,jpvt,nm,
     &                  n,ind,info,id,ro,io)
         if(imp.ge.9) then
            if(id.eq.1) then
               ides=1
            else if(ind.gt.0 .and. ind.le.nm)then
               ides=0
            else
               ides=-1
            end if
         end if
C
C     Se averigua si estamos ante un punto degenerado
C
         k=0
         if(iver.eq.-1 .or. iver.ne.iv) then
            if(ira.gt.0) then
               i=0
               if(i.lt.n .and. k.eq.0) then
4000             i1=i+1
                  ii=i+nd
                  xi1=x(i1)
                  if(ira.gt.1) then
                     csi=cs(i1)
                     if(csi.le.gig1 .and. ire(i1).eq.0 .and. w(ii)
     &                  .gt.eps .and. (xi1.ge.(csi-eps)))then
                        s2=dnrm2(nm,q(i1,m1),iq)
                        if(s2.ge.epsmch) then
                           k=1
                           ipvt(m1)=i1
                        else
                           w(ii)=0
                        end if
                     end if
                  end if
                  if(k.eq.0 .and. ira.ne.2) then
                     cii=ci(i1)
                     if(cii.ge.-gig1 .and. ire(i1).eq.0 .and. w(ii
     &                  ).lt.-eps .and. (xi1.le.cii+eps))then
                        s2=dnrm2(nm,q(i1,m1),iq)
                        if(s2.ge.epsmch) then
                           k=1
                           ipvt(m1)=-i1
                        else
                           w(ii)=0
                        end if
                     end if
                  end if
                  i=i1
                  if(i.lt.n .and. k.eq.0) go to 4000
               end if
            end if
            i=0
            if(i.lt.md .and. k.eq.0) then
5000           ii=icd+i
               i=i+1
               ni=n+i
               if(ire(ni).ne.1) then
                  w(ii)=ddot(n,c(1,mi+i),1,w(nd),1)
                  if(w(ni).ge.-eps .and. w(ii).gt.eps) then
                     jj=idw
                     do 245 j=m1,n
                        w(jj)=ddot(n,q(1,j),1,c(1,mi+i),1)
                        jj=jj+1
245                  continue
                     s2=dnrm2(nm,w(idw),1)
                     if(s2.ge.epsmch) then
                        k=1
                        ipvt(m1)=ni
                     else
                        w(ii)=0
                     end if
                  end if
               end if
               if(i.lt.md .and. k.eq.0) go to 5000
            end if
         else
            do 250 i=icd,icd+md-1
250         w(i)=0
         end if
         i=0
         if(i.lt.midf .and. k.eq.0) then
6000        i1=i+1
            in=nmd+i1
            if(ire(in).ne.1 ) then
               ii=iad+i
               w(ii)=ddot(n,a(1,i1),1,w(nd),1)
               if(ire(in).eq.0 .and. in.ne.il) then
                  if((i1.le.mif.and.abs(w(ii)).gt.eps) .or.
     &               (i1.gt.mif.and.w(ii).gt.eps.and.w(in).ge.-eps
     &               )) then
C
C     Se averigua si la restriccion que origina un punto degenerado es
C     linealmente dependiente de las restricciones que formaban el
C     conjunto activo al comienzo de la iteracion
C
                     if(il.eq.0) then
                        m0=m1
                     else
                        m0=m+2
                     end if
                     jj=idw
                     do 260 i=m0,n
                        w(jj)=ddot(n,q(1,i),1,a(1,i1),1)
                        jj=jj+1
260                  continue
                     s2=dnrm2(nm,w(idw),1)
C
C     Si la restriccion no es linealmente dependiente se apade al
C     conjunto activo y si no, se averigua si existe direccion de
C     descenso violandose esa restriccion
C
                     if(s2.ge.epsmch) then
                        k=1
                        ipvt(m1)=in
                     else if(il.eq.0) then
                        w(ii)=0
                     else
                        sj=ddot(n,q(1,m1),1,a(1,i1),1)
                        if(abs(sj).ge.epsmch) then
                           if(id.ne.1) ind1=10
                           if(il.lt.0) then
                              sj=-sj/q(-il,m1)
                           else if(il.le.n) then
                              sj=sj/q(il,m1)
                           else if(il.le.nmd) then
                              s2=ddot(n,q(1,m1),1,c(1,mi+il-n),1)
                              sj=sj/s2
                           else
                              s2=ddot(n,q(1,m1),1,a(1,il-nmd),1)
                              sj=sj/s2
                           end if
                           sk=0.0d+0
                           s3=-1.0d+0
                           if(i1.le.mif) then
                              s2=abs(sj)
                              if(il.le.nmd .or. (il.gt.nmdi.and.s.lt.
     &                           -eps)) then
                                 s3=s2+s
                                 if(sj.gt.eps) then
                                    sk=-1.0d+0
                                 else
                                    sk=1.0d+0
                                 end if
                              else
                                 s3=s2-abs(s)+1.0d+0
                                 if((s.lt.-eps.and. sj.gt.eps) .or.
     &                              (s.gt.eps.and. sj.lt.-eps)) then
                                    sk=-1.0d+0
                                 else
                                    sk=1.0d+0
                                 end if
                              end if
                           else
                              if(sj.lt.-eps .and. (il.le.nmd.or.
     &                           (il.gt.nmdi.and.s.lt.-eps))) then
                                 s3=s-sj
                                 sk=1.0d+0
                              else if(sj.gt.eps .and. il.gt.nmd .and.
     &                           s.gt.eps) then
                                 s3=sj+s1
                                 sk=1.0d+0
                              else if(sj.lt.-eps .and. il.gt.nmd .and.
     &                           il.le.nmdi .and. s.lt.-eps) then
                                 s3=s1-sj
                                 sk=1.0d+0
                              end if
                           end if
                           if(s3.gt.eps) then
                              ipvt(m1)=nmd+i1
                              if(id.eq.1) then
                                 id=2
                                 k=1
                              else
                                 ind1=11
                              end if
                           else if(id.eq.1) then
C
C     Se modifican la direccion de descenso o el paso calculado, y el
C     vector gradiente
C
                              if(s.gt.0.0d+0) then
                                 s1=-s+1
                              else if(s.lt.0.0d+0.and.il.le.nmdi.and.il
     &                           .gt.nmd) then
                                 s1=-s-1
                              else
                                 s1=-s
                              end if
                              s=1.0d+0+(sk*sj)/s1
                              if(info.eq.0) then
                                 call dscal(n,s,w(nd),1)
                              else
                                 ro=s*ro
                              end if
                              if(sk.eq.1.0d+0) then
                                 call dadd(n,a(1,i1),1,w,1)
                              else if(sk.eq.-1.0d+0) then
                                 call ddif(n,a(1,i1),1,w,1)
                              end if
                              w(ii)=0.0d+0
                           end if
                        else
                           w(ii)=0
                        end if
                     end if
                  end if
               end if
            end if
            i=i1
            if(i.lt.midf .and. k.eq.0) go to 6000
         end if
C
C     Calculo del paso en la direccion de descenso
C
         if(k.eq.0) then
            if(id.eq.0 .and. ind.le.nm .and. ind.gt.0) id=2
            if(ind1.eq.11 .and. id.ne.1) id=id+100
            if(ind1.eq.10 .and. id.ne.1) id=id+10
            call pasr03(a,ia,b,ci,cs,x,ro,w,ire,ipvt(m1),ira,n,md,mif,
     &                  mdf,m,id,io)
            if(id.eq.-1) then
               ind=-1
               if(imp.ge.7) call dimp03(x,w,ire,ipvt,s,0,0,0,0,0,
     &                                  0,0,0,0,ind,imp,io,iter)
               return
            else if(id.eq.0) then
               id=2
               k=1
            end if
            if(id.eq.1 .or. id.eq.3 .or. id.eq.11)then
               icol3=ipvt(m1)
               if(imp.ge.9) iadd=icol3
            end if
         end if
C
C     En caso de punto degenerado se estudia si se produce alternancia
C     entre las restricciones
C
         if(k.eq.1) then
            icicla=icicla+1
            icol3=ipvt(m1)
            if(imp.ge.9) iadd=icol3
            if((icicla.gt.m .and. m.gt.0).or.
     &         (icol3.eq.icol1 .and. icol.eq.m)) then
               ind=-2
               if(imp.ge.7) call dimp03(x,w,ire,ipvt,s,0,0,0,0,0,
     &                                 0,0,0,0,ind,imp,io,iter)
               return
            end if
            icol1=icol2
            icol2=icol3
         else if(icicla.gt.0) then
            icicla=0
            icol1=0
            icol2=0
         end if
C
C     Impresion de los datos correspondientes a la iteracion corriente
C
         if(imp.ge.9) call dimp03(x,w,ire(nd1),ipvt,s,n,il,m,midf,ides,
     &                           icicla,iadd,mi,mi1,2,imp,io,iter)
C
C     Calculo del punto de la iteracion siguiente
C
         if(icicla.eq.0) then
            i1=icd-1
            do 270 i=1,md
               ni=n+i
               if(ire(ni).eq.0 .and. (iver.ne.iv .or. iver.eq.-1) .and.
     &         id.lt.10)then
                  if(ro.eq.1.0d+0) then
                     w(ni)=w(ni)+w(i1+i)
                  else
                     w(ni)=w(ni)+ro*w(i1+i)
                  end if
               end if
270         continue
            if(ro.eq.1.0d+0) then
               call dadd(n,w(nd),1,x,1)
            else
               call daxpy(n,ro,w(nd),1,x,1)
            end if
            i1=nd-1
         end if
C
C     Se anade la restriccion  icol3  al conjunto activo y se modifica
C     el vector ire y la factorizacion  QR
C
         if(icicla.ne.0 .or. id.eq.1 .or. id.eq.11 .or. id.eq.3) then
            if(il.eq.-icol3) then
               iicol=1
            else
               iicol=0
            end if
            s=r(1,m2)
            nm1=nm-1
            m=m1
            if(icol3.lt.0) then
               ire(-icol3)=-1
            else
               ire(icol3)=1
            end if
            if(icol3.le.n) then
               ip=icol3
               call anfm04(q,iq,r(1,2),ir,w,w(nd),jpvt,n,m,ip,io)
            else
               ip=0
               if(icol3.le.nmd) then
                  call anfm04(q,iq,r(1,2),ir,c(1,mi+icol3-n),w(nd),jpvt,
     &                        n,m,ip,io)
               else
                  call anfm04(q,iq,r(1,2),ir,a(1,icol3-nmd),w(nd),jpvt,
     &                        n,m,ip,io)
               end if
            end if
C
C      Se modifica la factorizacion de Cholesky de la matriz  Z'HZ
C
            if(icol3.le.nmd .and. iver.ne.-1) iver=iver+1
            if(nm.gt.1) then
               if(iibeta.eq.1 .and. ind.eq.-2) ind=-1
               if(ind.gt.2*nm .and. ind.lt.3*nm) then
                  nf=3*nm-ind
               else if(ind.gt.nm .and. ind.lt.2*nm) then
                  nf=2*nm-ind
               else if(ind.lt.-1 .and. ind.ge.-nm) then
                  nf=-ind-1
               else if(ind.eq.-1 .or. ind.eq.(-nm-1) .or. ind.eq.3*nm
     &                 .or. ind.eq.2*nm) then
                  ind=0
                  call anfm03(r,ir,r(1,m+2),ir,q(1,m+1),iq,w(nd),jpvt,n,
     &                        nm1,ind,iver-iv,io)
                  nf=0
               else if(ind.ge.0) then
                  nf=nm
               else
                  nf=-nm-ind-1
               end if
               if(nf.ne.0) then
                  nmf=nm1-nf
                  if(ind.gt.nm) then
                     ii=nmf+i1
                  else if(ind.lt.0) then
                     ii=nmf*(nmf+1)/2+i1
                  end if
                  if(iibeta.eq.1) nf=nf-1
                  call anfm05(r,ir,r(1,m+2),ir,q(1,m+1),iq,w(nd),
     &                       w(nd+2*nm1),jpvt,s,nm1,nf,n,ind,iver-iv,io)
               end if
               if(ind.le.-n10) then
                  iibeta=1
                  ind=ind+n10
               else
                  iibeta=0
               end if
               if(ind.eq.n) then
                  ind=-1
                  if(imp.ge.7) call dimp03(x,w,ire,ipvt,s,0,0,0,0,
     &                                  0,0,0,0,0,ind,imp,io,iter)
                  return
               end if
               nm=nm1
            end if
         end if
C
C     Si x no es un punto degenerado se modifica el vector  IRE  de las
C     restricciones que aparecen en el funcional
C
         if(icicla.eq.0 .and. midf.gt.0) then
            i1=1
            call aux003(a,ia,x,b,q,iq,r(1,2),ir,w(nd1),ire(nd1),ipvt,
     &                  nmd,mif,mdf,midf,n,m,i1,io)
            if(i1.eq.0) info=0
         end if
         if(info.eq.1 .and. id.ne.2) then
            if(jpvt(nm).ne.nm .or. (ind.lt.0 .and. ind.ne.-n .and.
     &      ind.ne.(-2*n+1)) .or. ind.gt.n) info=0
         end if
C
C     Si el paso ha sido "demasiado grande", se calcula la norma y el
C     valor del funcional en el punto obtenido,  antes de regresar al
C     al subprograma de llamada
C
         if(id.eq.11) then
            iw=nd+m
            w(iw)=opvf03(r,ir,a,ia,p,b,x,w,alfa,nd,n,mif,mdf)
            ind=-3
            if(imp.ge.7) call dimp03(x,w,ire,ipvt,s,n,iw,0,0,0,0,
     &                               0,0,0,ind,imp,io,iter)
            return
         end if
         if(m.eq.n) then
            minimo=1
            id=2
            nm=0
         else
            minimo=0
         end if
         iter=iter+1
         if(iter.le.itemax) go to 2000
      end if
      ind=1
      if(imp.ge.7) call dimp03(x,w,ire,ipvt,s,il,ides,0,0,0,0,0,
     &                         0,0,ind,imp,io,iter)
      end
