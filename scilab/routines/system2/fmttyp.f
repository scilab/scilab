C/MEMBR ADD NAME=FMTTYP,SSI=0
c     Copyright INRIA
      integer function fmttyp(form,nf)
c!but
c     analyse un format fortran code en scilab, pour en deduire
c     sont type:
c          2 : entier s'il ne comporte que des formats "i"
c          1 : flottant s'il ne comporte que des formats f,e,d ou g
c          3 : logique s'il ne comporte que des formats l
c          4 : caractere s'il ne comporte que des formats a
c          0 : format incorrect ou ayant des types mixtes
c!
      integer form(nf),nf
      integer count,quote,lparen,rparen,typf(7),typb(7),ntyp,fk
      data quote/53/,lparen/41/,rparen/42/,ntyp/7/
      data typf/18,15,14,13,16,21,10/,typb/2,1,1,1,1,3,4/
c
      count=0
      fmttyp=0
      if(form(1).ne.lparen.or.form(nf).ne.rparen.or.nf.le.2) goto 90
c
      k=2
   05 continue
      fk=abs(form(k))
      if(fk.eq.quote)then
        if(count.eq.0) then
           count=count+1
        else
           k=k+1
           if(abs(form(k)).ne.quote) count=count-1
        endif
      endif
      if(count.eq.1) goto 20
      l=1
   10 if(fk.eq.typf(l)) then
                                 if(fmttyp.eq.0) fmttyp=typb(l)
                                 if(typb(l).ne.fmttyp) goto 90
                                 l=ntyp
      endif
      l=l+1
      if(l.le.ntyp) goto 10
   20 continue
      k=k+1
      if(k.gt.nf-1) return
      goto 05
c
   90 fmttyp=0
      return
      end
