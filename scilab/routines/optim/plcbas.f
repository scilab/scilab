      subroutine plcbas(h,p,c,d,ci,cs,ira,mi,md,x,f,w,iv,dlagr,imp,io,n,
     &                  modo,info,iter)
C     SUBROUTINE PLCBAS(H,P,C,D,CI,CS,IRA,MI,MD,X,F,W,IV,DLAGR,IMP,IO,N,
C    &                  MODO,INFO,ITER)
C
C***********************************************************************
C                                                                      *
C                                                                      *
C     Copyright:        Eduardo Casas Renteria                         *
C                       Cecilia Pola Mendez                            *
C                                                                      *
C       Departamento de Matematicas,Estadistica y Computacion          *
C       -----------------------------------------------------          *
C                       UNIVERSIDAD DE CANTABRIA                       *
C                       ------------------------                       *
C                             FECHA: Julio 2001                        *
C                             VERSION 2.1                              *
C                                                                      *
C***********************************************************************
C
C     OBJETIVO:
C        La minimizacion  de un funcional cuadratico  con restricciones
C        lineales,  (de acotacion, igualdad  y  desigualdad), sobre las
C        variables.
C        El problema planteado es de la forma:
C
C               F(X)=  1/2*X'*H*X + P'*X
C
C              (1)  CI(I) <= X(I) <= CS(I), I=1,N
C              (2)  <(C(1,J),C(2,J),...,C(N,J)),X>  = D(J), J=1,MI
C              (3)  <(C(1,J),C(2,J),...,C(N,J)),X> <= D(J), J=MI+1,MI+MD
C
C     LISTA DE LLAMADA:
C     DE ENTRADA:
C
C        H      Matriz  de  dimension  (N,N), que contiene la matriz  H
C               del  termino  cuadratico  del  funcional.  Es suficiente
C               suministrar la parte triangular inferior.
C
C        P      Vector  N-dimensional.  Contiene  los  coeficientes  del
C               termino lineal del funcional.
C
C        C      Matriz de dimension (N,MI+MD). Contiene los coeficientes
C               de las restricciones  de igualdad y  desigualdad. Los de
C               igualdad se almacenan en las  MI primeras columnas de la
C               matriz  C  y los de desigualdad en las restantes.
C
C        D      Vector de dimension  MI+MD. Contiene los coeficientes de
C               los  terminos  independientes  de  las  restricciones de
C               igualdad y desigualdad.
C
C        CI     Si  IRA= 0  o  2  esta  variable  no sera  utilizada. Si
C               IRA= 1  o  3,   CI  sera  un  vector  de  dimension  N
C               conteniendo las cotas inferiores de  X. Si  X(I) no esta
C               acotado  inferiormente, CI(I)  debera ser  menor  que la
C               raiz cuadrada negativa  de la constante real  mas grande
C               de la maquina.
C
C        CS     Si  IRA= 0  o  1  esta  variable no  sera  utilizada. Si
C               IRA= 2  o  3,  CS   sera  un  vector  de  dimension  N
C               conteniendo las cotas superiores de  X. Si  X(I) no esta
C               acotado  superiormente,  CS(I)  debera  ser  mayor  que
C               la raiz cuadrada de la constante real  mas grande  de la
C               maquina.
C
C        IRA    Variable  que  indica  si  existen  restricciones  de
C               acotacion en el problema. Toma los valores:
C                  0  : No existen restricciones de acotacion.
C                  1  : Se  tienen  solo  restricciones  de  acotacion
C                       inferior.
C                  2  : Existen solo restricciones de acotacion superior
C                  3  : Se tienen ambos tipos de restriccion.
C
C        MI     Numero de restricciones de igualdad del problema.
C
C        MD     Numero  de  restricciones  de desigualdad  del  problema
C
C        W      Vector de trabajo de dimension  N*N+6*N+2*MD
C
C        IV     Vector entero  de dimension  3*N+2*MD+MI+1.
C
C        IMP    Indicador del nivel de impresion de salida de resultados
C               Toma los valores:
C                  6  : Sin salida de resultados.
C                  7  : Escribe el motivo de finalizacion del proceso.
C                  8  : Se obtiene informacion referente  a la solucion:
C                       el numero de iteraciones; el  optimo  calculado,
C                       la norma del vector de Kuhn-Tucker, el numero de
C                       restricciones activas y cuales   son    (para
C                       identificar estas restricciones  se  sigue el
C                       siguiente convenio para los valores que se
C                       obtienen:
C
C                       [-N,-1]         : Restriciones de cota inferior
C                       [ 1,N ]         : Restricciones de cota superior
C                       [N+1,N+MI]      : Restricciones de igualdad
C                       [N+MI+1,N+MI+MD]: Restricciones de desigualdad)
C
C                       y los multiplicadores de  Lagrange  asociados  a
C                       ellas  (si IND=0);  el valor  del funcional  ;
C                       la norma del punto calculado (si  IND=-3).
C
C                  9  : Se obtiene  informacion referente  al desarrollo
C                       de las iteraciones:  el numero de  restricciones
C                       activas   y cuales  son  (para identificarlas se
C                       sigue el convenio anterior), la restriccion que
C                       se a¤ade  o  se  elimina  del conjunto activo,
C                       el  tipo  de  direccion   de  descenso calculada
C                       y las iteraciones  con  punto degenerado.
C
C               > 10  : Ademas,  se  obtiene  el punto calculado en cada
C                       iteracion e informacion sobre el proceso llevado
C                       a cabo por OPTR01. (Ver valores posibles de  IMP
C                       en  OPTR01).
C
C        IO     Numero de canal de salida de resultados.
C
C        N      Numero de variables del problema.
C
C        MODO   Variable que indica el modo de comienzo del proceso con
C               los valores:
C                  1  : No se facilita un punto inicial,ni restricciones
C                       recomendadas como activas.
C                  2  : Igual que MODO = 1, pero la subrutina trabaja
C                       de forma diferente. Este caso debe utilizarse
C                       cuando hay valores de seguridad para las
C                       variables o las restricciones que son grandes y
C                       que previsiblemente no seran alcanzados.
C                       Tambien puede ser util este MODO cuando la
C                       forma cuadratica es definida positiva.
C                  3  : Se facilita un punto inicial. En este caso
C                       el punto ha de ser admisible para todas las
C                       restricciones.
C
C       ITER  : Numero maximo de iteraciones que realizara el proceso.
C               Si ITER <= 0, se cambiara por 14*(N+MI+MD)
C
C     DE SALIDA:
C
C        X      Vector de dimension  N que contiene el optimo calculado,
C               (si el proceso ha finalizado sin problemas), o la ultima
C               aproximacion obtenida.
C
C        F      Variable  que  contiene  el  valor  del funcional  en el
C               optimo si el proceso  ha finalizado sin problemas.
C
C      DLAGR    Vector de doble precision de dimension N+MI+MD si IRA es
C               mayor que cero y de dimension MI+MD si IRA=0.En las N
C               primeras componentes contiene los multiplicadores de
C               Lagrange asociados a las restricciones de cota (si IRA
C               es mayor que cero) y en las MI+MD ultimas componentes
C               contiene los multiplicadores asociados a las de igualdad
C               y desigualdad respectivamente.
C
C        INFO   Variable que indica el motivo por el cual se finaliza el
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
C        ITER   Numero de iteraciones que realiza el proceso.
C
C        Esta subrutina trabaja en doble precision via una sentencia
C     "implicit":
C                Implicit double precision (a-h,o-z)
C
C     SUBPROGRAMAS AUXILIARES: anfm01,anfm02,anfm03,anfm04,anfm05,anfm06
C                anrs01,anrs02,auxo01,aux003,dadd,daxpy,dcopy,ddif,ddot,
C                desr03,dimp03,dipvtf,dmmul,dnrm0,dnrm2,dscal,dswap,
C                d1mach,idamax,nvtk03,optr01,optr03,opvf03,pasr03,
C                tol03,zthz
C     FUNCIONES FORTRAN INTRINSECAS: abs,max,min,mod,sign,sqrt
C
C
      implicit double precision(a-h,o-z)
      dimension h(n,*),p(*),c(n,*),d(*),ci(*),cs(*),x(*),w(*),iv(*),
     &	      dlagr(*)
      iw=1
      alfa=1.d0
      mif=0
      mdf=0
      n1=n+1
      k=n1
      mid=mi+md
      nmd=n+md
      nmid=n+mid
      do 10 i=1,n
         k=k-1
         j=n*(i-1)+i
         call dcopy(k,h(i,i),1,w(j),1)
10    continue
      nipvt=1+nmd
      njpvt=nipvt+nmid+1
      nw=n*n1+1
      modo1=modo
      if(modo.eq.1) modo=6
      if(modo.eq.2) modo=1
      if(modo.eq.3) modo=2
      call optr03(w(1),1,c,n,h,n,w(1),n,p,b,d,ci,cs,x,w(nw),iw,iv,
     &            iv(nipvt),iv(njpvt),alfa,ira,n,m,mi,mi1,md,mif,
     &            mdf,modo,ind,imp,io,iter)
      modo=modo1
      info=ind
      k=n1
      do 20 i=1,n
         k=k-1
         j=n*(i-1)+i
         call dcopy(k,w(j),1,h(i,i),1)
 20   continue
      do 40 j=2,n
         do 30 i=1,j-1
 30      h(i,j)=h(j,i)
 40   continue
      if(ind.ne.0) return
      if(ind.eq.0) then
         f=w(nw+iw-1)
         nmul=nmd+nw
         if(ira.gt.0) then
            nl=n
         else
            nl=0
         end if
         do 12 i=1,nl+mid
12       dlagr(i)=0
         k=nipvt
         do 15 i=1,mi1
            j=iv(k)+nl
            dlagr(j)=w(nmul)
            nmul=nmul+1
            k=k+1
15       continue
         do 17 i=mi1+1,m
            j=iv(k)
            if(j.lt.0) then
               dlagr(-j)=-w(nmul)
            else if(j.le.n) then
               dlagr(j)=w(nmul)
            else
               if(ira.eq.0) j=j-n
               dlagr(j+mi)=w(nmul)
            end if
            nmul=nmul+1
            k=k+1
17       continue
      end if
      end
