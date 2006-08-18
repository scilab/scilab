      subroutine outl2(ifich,neq,neqbac,tq,v,t,tout)
c%but
c     cette subroutine contient les differents messages
c     a afficher suivant le deroulement de l execution.
c% liste d'appel
c     Entrees :
c     - ifich. est l'indice du message (-1 pour une
c        intersection avec la face, 1 pour une localisation
c        d un minimum local, 2 pour le resultat a un certain
c     degre ...)
c     - neq. est le degre (ou dimension) ou se situe
c        la recherche actuelle.
c     - neqbac. contient la valeur du degre avant le 1er
c        appel de lsoda
c     - tq. est le tableau contenant les coefficients du
c        polynome.
c     - w. trableau de travail
c
c     Sortie  : Aucune .
c%
c     Copyright INRIA
      implicit double precision (a-h,o-y)
      dimension tq(*),neq(*)
      dimension v(*)
      character*80 buf
      common/no2f/ef2
      common/comall/nall/sortie/nwf,info,ll

      nq=neq(1)

c
c
      write(buf(1:3),'(i3)') neq(1)
c
      if(ifich.ge.80) goto 400
      if(ifich.ge.70) goto 350
      if(ifich.ge.60) goto 300
      if(ifich.ge.50) goto 250
      if(ifich.ge.40) goto 200
      if(ifich.ge.30) goto 150
      if(ifich.ge.20) goto 100
      
      ng=neq(2)
      ltq   = 1
      ltg   = ltq+neq(3)+1
      ltqdot = ltg+ng+1+(nq+ng+1)
      ltr=ltqdot+nq
      lpd=ltr+ng+nq+1
      ltrti=lpd+nq*nq
      lfree=ltrti+nq+1

      if (ifich.lt.17) then
         write(buf(1:3),'(i3)') nq
         call basout(ifl,nwf,'----------------- TRACE AT  ORDER: '//
     $        buf(1:3)//' ----------------------')
c     
         if (ifich.lt.0) then
            call basout(ifl,nwf,' Intersection with a degree '//
     &           buf(1:3)//' facet ')
         else if (ifich.eq.1) then
            call basout(ifl,nwf,' Minimum found for order: '//
     $           buf(1:3))
         else if (ifich.eq.2) then
            call basout(ifl,nwf,' Local minimum found for order: '//
     $           buf(1:3))
         else if (ifich.eq.3) then
            call basout(ifl,nwf,' Maximum found for order: '//
     $           buf(1:3))
         else if (ifich.eq.4) then
            call basout(ifl,nwf,' Local maximum found for order: '//
     $           buf(1:3))
         else if (ifich.eq.14.or.ifich.eq.15.or.ifich.eq.16) then
            call basout(ifl,nwf,' Reached point:')
         endif
c     
         call basout(ifl,nwf,'Denominator:')
         call dmdspf(tq,1,1,nq+1,15,ll,nwf)
c     
         call basout(ifl,nwf,'Numerator')
         call dmdspf(v,1,1,nq,15,ll,nwf)
      else
c
         call basout(ifl,nwf,'Gradient :')
         call dmdspf(v,1,1,nq,15,ll,nwf)
         phi0=t
         write(buf(1:14),'(d14.7)') phi0
         call basout(ifl,nwf,' Error L2 norm                    : '//
     $        buf(1:14))
         write(buf(1:14),'(d14.7)') tout
         call basout(ifl,nwf,' Datas L2 norm                    : '//
     $        buf(1:14))
         errel= sqrt(phi0)
         write(buf(1:14),'(d14.7)') errel
         call basout(ifl,nwf,' Relative error norm              : '//
     $        buf(1:14))
         call basout(ifl,nwf,'------------------'//
     $        '---------------------------------------------')
         call basout(ifl,nwf, ' ')
         call basout(ifl,nwf, ' ')
         call basout(ifl,nwf,'------------------'//
     $        '---------------------------------------------')
         call basout(ifl,nwf, ' ')
         call basout(ifl,nwf, ' ')
      endif
 100  continue
c     messages du sous programme arl2
      if(ifich.eq.20) then
         call basout(ifl,nwf,'LSODE 1  '//
     $        '------------------------------------------------------')
         write(buf,'('' dg='',i2,''     dgback='',i2)') nq,neqbac
         call basout(ifl,nwf,buf(1:30))
      else if(ifich.eq.21) then
         call basout(ifl,nwf,'LSODE 2  '//
     $        '------------------------------------------------------')
      else if(ifich.eq.22) then
         call basout(ifl,nwf,
     $        ' Unwanted loop beetween two orders..., Stop')
      else if(ifich.eq.23) then
         write(buf(1:2),'(i2)') neqbac
         call basout(ifl,nwf,'Il y a eu '//buf(1:2)//
     $        ' retours de face.')
      endif
      return
c
 150  continue
c     messages du sous programme optml2
      if(ifich.eq.30) then
         call basout(ifl,nwf,'Optml2 =========='//
     $        ' parameters before lsode call =================')
         write(buf,'(2d14.7)') t,tout
         call basout(ifl,nwf,' t= '//buf(1:14)//
     $        ' tout= '//buf(15:28))
         call basout(ifl,nwf,' Q initial :')
         call dmdspf(tq,1,1,nq+1,14,ll,nwf)
      else if(ifich.eq.31) then
         call basout(ifl,nwf,'Optml2 =========='//
     $        ' parameters after lsode call   ================')
         write(buf,'(d14.7)') v(1)
         call basout(ifl,nwf,' |grad|= '//buf(1:14))
         write(buf,'(i3)') neqbac
         call basout(ifl,nwf,' nbout= '//buf(1:3))
         write(buf,'(2d14.7)') t,tout
         call basout(ifl,nwf,' t= '//buf(1:14)//
     $        ' tout= '//buf(15:28))
         call basout(ifl,nwf,' Q final :')
         call dmdspf(tq,1,1,nq+1,14,ll,nwf)
         call basout(ifl,nwf,'Optml2 ==========='//
     $        ' End of LSODE description======================')
         call basout(ifl,nwf,' ')
      else if(ifich.eq.32) then
         call basout(ifl,nwf,' Lsode: no convergence (istate=-5)')
         call basout(ifl,nwf,    'new call with reduced tolerances')
      else if(ifich.eq.33) then
         call basout(ifl,nwf,' Lsode: no convergence (istate=-6)')
      else if(ifich.eq.34) then
         write(buf,'(2d14.7)') t,tout
         call basout(ifl,nwf,' t= '//buf(1:14)//
     $        ' tout= '//buf(15:28))
         write(buf,'(i5,d14.7)') neqbac,v(1)
         call basout(ifl,nwf,' itol= '//buf(1:5)//
     $        ' rtol= '//buf(6:19))
         call basout(ifl,nwf,'atol=')
         call dmdspf(tq,1,1,nq,14,ll,nwf)
      else if(ifich.eq.35) then
         write(buf,'(i5,d14.7)') neqbac
         call basout(ifl,nwf,' itol= '//buf(1:5))
         call basout(ifl,nwf,'rtol=')
         call dmdspf(v,1,1,nq,14,ll,nwf)
         call basout(ifl,nwf,'atol=')
         call dmdspf(tq,1,1,nq,14,ll,nwf)
      else if(ifich.eq.36) then
         call basout(ifl,nwf,    'new call with increased tolerances')
      else if(ifich.eq.37) then
         write(buf(1:2),'(i2)') neqbac
         call basout(ifl,nwf,' LSODE stops with istate ='//buf(1:2))
      else if(ifich.eq.38) then
         call  basout(ifl,nwf,' Lsode stops: too many integration '//
     &        'steps  (istate= -1)')
         call basout(ifl,nwf,'   new call to go further')
      else if(ifich.eq.39) then
         call basout(ifl,nwf,
     $        'Repeated LSODE failure --  OPTML2 stops')
      endif
      return
 200  continue
c message relatifs au sous programme domout
      if(ifich.eq.40) then
         call basout(ifl,nwf,' ')
         call basout(ifl,nwf,'********LOOKING FOR INTERSECTION '//
     $        ' WITH STABILITY DOMAIN BOUNDS ********')
         write(buf(1:10),'(i10)') neqbac
         call basout(ifl,nwf,' kmax= '//buf(1:10))
      else if(ifich.eq.41) then
         call basout(ifl,nwf,'Domout =========='//
     $        ' parameters before lsode call =================')
         write(buf,'(2d14.7)') t,tout
         call basout(ifl,nwf,' t= '//buf(1:14)//
     $        ' tout= '//buf(15:28))
         call basout(ifl,nwf,' initial Q :')
         call dmdspf(tq,1,1,nq+1,14,ll,nwf)
      else if(ifich.eq.42) then
         call basout(ifl,nwf,'Domout =========='//
     $        ' parameters after lsode call  =================')
         write(buf,'(i3)') neqbac
         call basout(ifl,nwf,' nbout= '//buf(1:3))
         write(buf,'(2d14.7)') t,tout
         call basout(ifl,nwf,' t= '//buf(1:14)//
     $        ' tout= '//buf(15:28))
         call basout(ifl,nwf,' Q final :')
         call dmdspf(tq,1,1,nq+1,14,ll,nwf)
         call basout(ifl,nwf,'Domout =========='//
     $        ' End of LSODE description======================')
         call basout(ifl,nwf,' ')
      else if(ifich.eq.43) then
         call  basout(ifl,nwf,' Lsode stops: too many integration '//
     &        'steps  (istate= -1)')
         call basout(ifl,nwf,'   new call to go further')
      else if(ifich.eq.44) then
         write(buf(1:9),'(i9)') neqbac
         call basout(ifl,nwf,'Number of unstable roots: '//buf(1:9))
      else if(ifich.eq.45) then
         write(buf(1:3),'(i3)') neqbac
         call basout(ifl,nwf,' lsode problem (istate='//buf(1:3)//
     &        ') when looking for intersection with ')
         call basout(ifl,nwf,'   stability domain bounds... Stop ')
      else if(ifich.eq.46) then
         write(buf(1:9),'(i9)') neqbac
         call basout(ifl,nwf,'watface --> nface= '//buf(1:9))
         write(buf(1:9),'(i9)') nq
         call basout(ifl,nwf,'onface --> neq= '//buf(1:9))
         write(buf,'(2d14.4)') t,tout
         call basout(ifl,nwf,' yi= '//buf(1:14)//
     $        ' yf= '//buf(15:28))
         call dmdspf(tq,1,1,nq+1,14,ll,nwf)
      else if(ifich.eq.47) then
         call basout(ifl,nwf,' goto 314 ===========================')
         call basout(ifl,nwf,' qi = ')
         call dmdspf(v,1,1,nq+1,14,ll,nwf)
      else if(ifich.eq.47) then
         call basout(ifl,nwf,'********END OF INTERSECTION '//
     $        ' WITH STABILITY DOMAIN BOUNDS SEARCH ********')
      endif
      return
c
 250  continue
c     messages de deg1l2 et degl2
      if(ifich.eq.50) then
         call basout(ifl,nwf,' Non convergence  ...')
         call basout(ifl,nwf,'          look for next solution .')
      else if(ifich.eq.51) then
         write(buf(1:3),'(i3)') nq
         call basout(ifl,nwf,'+++++++++++++++++++++++++++++++++++++++'//
     $        '++++++++++++++++++++++++')
         Call basout(ifl,nwf,' Look for all minina of degree: '
     &        //buf(1:3))
         call basout(ifl,nwf,'+++++++++++++++++++++++++++++++++++++++'//
     $        '++++++++++++++++++++++++')
      else if(ifich.eq.52) then
         write(buf(1:3),'(i3)') nq
         call basout(ifl,nwf,'+++++++++++++++++++++++++++++++++++++++'//
     $        '++++++++++++++++++++++++')
         Call basout(ifl,nwf,' End of search degree '//buf(1:3)//
     $        ' minima ')
         call basout(ifl,nwf,'+++++++++++++++++++++++++++++++++++++++'//
     $        '++++++++++++++++++++++++')
         mxsol=tout
         call basout(ifl,nwf,' Q(0) :')
         call dmdspf(tq,1,1,nq,14,ll,nwf)
         call basout(ifl,nwf,' corresponding relatives errors')
         call dmdspf(tq(mxsol+1),1,1,neqbac,14,ll,nwf)
      else if(ifich.eq.53) then
         write(buf(1:3),'(i3)') nq
         call basout(ifl,nwf,'+++++++++++++++++++++++++++++++++++++++'//
     $        '++++++++++++++++++++++++')
         Call basout(ifl,nwf,' End of search degree '//buf(1:3)//
     $        ' minima ')
         call basout(ifl,nwf,'+++++++++++++++++++++++++++++++++++++++'//
     $        '++++++++++++++++++++++++')
         mxsol=tout
         call basout(ifl,nwf,' corresponding denominators:')
         call dmdspf(tq,mxsol,neqbac,nq,14,ll,nwf)
         call basout(ifl,nwf,' relatives errors')
         call dmdspf(tq(mxsol*nq+1),mxsol,neqbac,1,14,ll,nwf)
      endif
      return
c
 300  continue
c messages de roogp
      if(ifich.eq.60) then
         call basout(ifl,nwf,'Rootgp : No value found for Beta when '//
     &        'looking for intersection with a complex facet')
         call basout(ifl,nwf,'        Stop')
      endif
      return
c
 350   continue
c messages de onface
      if(ifich.eq.70) then
         write(buf(1:3),'(i2)') nq
         call basout(ifl,nwf,'Domain boundary reached, ')
         call basout(ifl,nwf,'Order is deacreased by'//buf(1:3))
      else if(ifich.eq.71) then
         call basout(ifl,nwf,'Remainder:')
         call dmdspf(tq,1,1,nq,14,ll,nwf)
      endif
      return
c
 400  continue
      if(ifich.eq.80) then
        call basout(ifl,nwf,'Already reached minimum ')
      else if(ifich.eq.81) then
        call basout(ifl,nwf,'Preserve minimun in  tback ')
      endif
      return
      end
