function  CodeGeneration_()
Cmenu='Open/Set'
xinfo('Click on a Superblock (without activation output)'+..
	' to obtain a coded block ! ')
  k=[]
  while %t 
    if %pt==[] then
      [btn,%pt,win,Cmenu]=cosclick()
      
      if Cmenu<>[] then
	[%win,Cmenu]=resume(win,Cmenu)
      end
    else 
      win=%win
    end
    
    xc=%pt(1);yc=%pt(2);%pt=[]
    k=getobj(scs_m,[xc;yc])
    if k<>[] then break,end
  end
  if scs_m.objs(k).model.sim(1)=='super' then
    disablemenus()
    all_scs_m=scs_m;
    XX=scs_m.objs(k);
    [ok,XX,alreadyran]=do_compile_superblock(XX,all_scs_m,k,alreadyran)
    enablemenus()
    if ok then 
      scs_m.objs(k)=XX
      edited=%t;
      needcompile=4
      Cmenu='Replot';
    else
      Cmenu='Open/Set' 
    end     
  else
    message('Generation Code only work for a Superblock ! ')
  end    
endfunction

function [ok,Makename]=buildnewblock() 
// compiles the generated C code and link it with Scilab
//Copyright INRIA
//Author : Rachid Djenidi
  files=[rdnom rdnom+'_void_io' rdnom+'_Cblocks']
  [fd,ierr]=mopen(rpat+'/'+rdnom+'f.f','r')
  if ierr==0 then mclose(fd),files=[files,rdnom+'f'],end
    Makename=gen_make(rdnom,files,archname,rpat+'/'+rdnom+'_Makefile')
    //unlink if necessary
    [a,b]=c_link(rdnom); while a ; ulink(b);[a,b]=c_link(rdnom);end
    libn=ilib_compile('lib'+rdnom,Makename)
    ierr=execstr('libnumber=link(libn)','errcatch')
    ierr=execstr('link(libnumber,rdnom,''c'')','errcatch')
    
    if ierr<>0 then 
      ok=%f;x_message(['sorry link problem';lasterror()]);
      return;
    end

endfunction


function Code=c_make_doit1(cpr)
// produces the code for ddoit1 and edoit1
//Copyright INRIA
//Author : Rachid Djenidi


  Code=[make_ddoit1()
	make_edoit1()]
endfunction
function Code=make_ddoit1()
//les pointeurs de cpr :
  z=cpr.state.z;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  clkptr=cpr.sim.clkptr;
  nZ=size(z,'*')+size(outtb,'*')+clkptr($)

  Code=['';
	 '/*'+part('-',ones(1,40))+' ddoit1.c */ ';
	 'int '
	 cformatline(rdnom+'ddoit1(z, told, outtb, iwa)',70); 
	 '';
	 '     double  *z; ';
	 '     double  *told; ';
	 ''
	 '     double  *outtb; '; 
	 '     integer *iwa; '; 
	 '{'; 
	 '  /* System generated locals */ '; 
	 ' '; 
	 '  /* Local variables */ '; 
	 '  integer kiwa; ';
	 '  integer urg; '; 
	 ' '; 
	 '  /* Function Body */ '; 
	 '  kiwa = 0; ';
	 '  urg = 1; '];
 //////////////////////////////////////////////////
  if  szclkIN>=1 then
    pointi=clkptr(howclk)-1;
    Code($+1)='  pointi='+string(pointi)+'+ totalnevprt;';
  else
    pointi=clkptr(howclk)
    Code($+1)='  pointi='+string(pointi)+';';
  end
  Code=[Code;
	'  tevts[pointi-1]=*told;'];


  Code=[Code;
	 '  while (urg > 0) {';		 
	 cformatline('    '+rdnom+'edoit1(told, '+..
		    '&(z['+string(size(z,1))+']), &urg, '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1))+..
		    '),&kiwa);',70);	
	'  }';
	'  iwa['+string(clkptr($)-1)+']=kiwa;';
	'  return 0;'
	' ';
	'} /* ddoit1 */';
	'  '];
endfunction

function Code=make_edoit1()
  //les pointeurs de cpr :
  z=cpr.state.z;  
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  ordptr=cpr.sim.ordptr;ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;evtspt=cpr.state.evtspt;
  nZ=size(z,'*')+size(outtb,'*')+clkptr($)
  //////////////////////////////////////////////////
  maxkeve=size(evtspt,1);
  
  Code=[ '/*'+part('-',ones(1,40))+' edoit1.c */ ';
	 'int '
	 cformatline(rdnom+'edoit1(told, '+..
		    'outtb, urg, iwa, kiwa)',70)
	'     double  *told; ';
	'     double  *outtb; ';
	'     integer *urg,  *iwa, *kiwa; ';
	'{ ';
	'  /* System generated locals */ ';
	'  integer i2; ';
	' ';
	'  /* Local variables */ ';
	'  integer flag, keve, nport; ';
	'  integer nord; ';
	'  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	'  double  *args[100]; '; 
	'  integer ierr1; ';
	'  integer ntvecm, nevprt; ';
	' ';
	'  /* Function Body */ ';
	'  --(*urg); '
	'  keve = pointi; ';
	'  pointi = evtspt[keve-1]; ';
	'  evtspt[keve-1] = -1; ';
	' ';
	'  nord = ordptr[keve] - ordptr[keve-1]; ';
	'  if (nord == 0) { ';
	'    return 0; ';
	'  } ';
	'  ++(*kiwa);   ';
	'  iwa[*kiwa-1] = keve;   ';
	'  switch(keve) {'];

  for keve=1:maxkeve
    Code($+1)='  case '+string(keve)+':';

    for ii=ordptr(keve):ordptr(keve+1)-1
      //***********************************
      fun=ordclk(ii,1);
      i=fun;
      if outptr(fun+1)-outptr(fun)>0  then
	nclock=ordclk(ii,2);
	Code=[Code;
	      '    flag = 1 ;';
	      '    nevprt='+string(nclock)+';';
	      '   '+wfunclist(i);
	      ' ';
	      ' '];
       end
    end
    Code($+1)='    break;';
  end
  Code($+1)='  }  ';
  Code($+1)='  ';
  Code($+1)='  switch(keve) {';
  for keve=1:maxkeve
    Code($+1)='  case '+string(keve)+':';

    for ii=ordptr(keve):ordptr(keve+1)-1
      //***********************************
      fun=ordclk(ii,1);
      i=fun;
      nevprt=ordclk(ii,2);
      ntvec=clkptr(fun+1)-clkptr(fun);
      if ntvec >0  & funs(fun) <> 'bidon' then
	nxx=lnkptr(inplnk(inpptr(fun)));
	if funtyp(fun)==-1 then
	  Code=[Code;
		'    if (outtb['+string(nxx-1)+']>0) {';
		'      ntvecm=1;';
		'    }';
		'    else {';
		'      ntvecm=2;';
		'    }']
	elseif funtyp(fun)==-2 then
	  Code=[Code;
		'    ntvecm=(integer)outtb['+string(nxx-1)+'];';
		'    if(ntvecm>'+string(ntvec)+'){ntvecm='+string(ntvec)+';}';
		'    if(ntvecm<1){ntvecm=1;}']
	else
	  message('Block '+fun+' has funtyp '+string(funtyp(fun))+': not allowed')
	end
	
	Code=[Code;
	      '     ++(*urg);';
	      '     i2 = ntvecm + clkptr['+string(fun-1)+'] - 1;';
	      '    '+rdnom+'_putevs(told, &i2, &ierr1); ';
	      '     if (ierr1 != 0) return 3;']
      end
    end
    Code($+1)='    break;';
  end
  Code=[Code;
	'  }  ';
	'  return 0;'
	'} /* edoit1 */      '];
endfunction

function Code=c_make_doit2(cpr);
//procuces the code for ddoit2 
//Copyright INRIA
//Author : Rachid Djenidi

///////////////////
//les pointeurs de cpr :
  z=cpr.state.z;
  funs=cpr.sim.funs;
  zptr=cpr.sim.zptr;inpptr=cpr.sim.inpptr;
  ordptr=cpr.sim.ordptr;ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;evtspt=cpr.state.evtspt;
  nZ=size(z,'*')+size(outtb,'*')+clkptr($)
  Code=['';
	'/*'+part('-',ones(1,40))+' ddoit2 */ ';
	'int '
	cformatline(rdnom+'ddoit2(z, told, outtb, iwa)',70);  
	''; 
	'     double  *z; '; 
	'     double  *told; '; 
	'     double  *outtb; '; 
	'     integer *iwa; '; 
	'{ '; 
	'  /* System generated locals */ '; 
	'  integer  i1, i; '; 
	' '; 
	'  /* Local variables */ '; 
	'  integer flag, keve, kiwa, nport; '; 
	'  double  *args[100]; ';
	'  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	'  integer nevprt; '; 
	' '; 
	'  /* Function Body */ '; 
	' '
	'  /*update continuous and discrete states on event */';
	'  kiwa = iwa['+string(clkptr($)-1)+'];';
	'  if (kiwa == 0) {';
	'    return 0 ;';
	'  }';
	'  i1 = kiwa;';
	'  for (i = 1; i <= i1; ++i) {';
	'    keve = iwa[i-1];';
	'    switch(keve) {'];
  maxkeve=size(evtspt,1);
  for keve=1:maxkeve
    Code($+1)='    case '+string(keve)+':';
    for ii=ordptr(keve):ordptr(keve+1)-1
      //***********************************
      fun=ordclk(ii,1);
      i=fun;
      //Actuator blocks have to be called even if they have no discrete
      //state
      is_act=or(i==act)
      is_cap=or(i==cap);
      if (zptr(i+1)-zptr(i)+xptr(i+1)-xptr(i))>0|is_act  then
      //if is_act  then
         Code($+1)='/*     .     If continuous state jumps, do cold restart */';
	 Code=[Code
	       '    flag = 2;';
	       '    nevprt='+string(ordclk(ii,2))+';';
	       '   '+wfunclist(i);];
      elseif ~is_cap then
	Code=[Code
	      '    if (block_'+rdnom+'['+string(i-1)+'].nx+block_'+rdnom+'['+string(i-1)+'].nz > 0||'
	      '        *block_'+rdnom+'['+string(i-1)+'].work !=NULL) {'
	      '    flag = 2;';
	      '    nevprt='+string(ordclk(ii,2))+';';
	      '   '+wfunclist(i);
              '   }'];

      end  
    end   
    Code($+1)='    break;';
  end
  Code=[Code
	'    }'; //switch
	'  }';
	'  return 0;'
	'} /* ddoit2 */'];

endfunction

function Code=c_make_endi(cpr)
//Copyright INRIA
//Author : Rachid Djenidi

///////////////////
//les pointeurs de cpr :
z=cpr.state.z;
funs=cpr.sim.funs;
inpptr=cpr.sim.inpptr;

Code=[	'/*'+part('-',ones(1,40))+' endi */ '; 
	'/* file_end.c */ ';
	'/* Subroutine */ int ';
	rdnom+'_endi(told, outtb, iwa) ';  
	''; 
	'     double  *told, *outtb; '; 
	'     integer *iwa; '; 
	'{ '; 
	'  /* Local variables */ '; 
	'  integer flag; '; 
	'  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	'  double  *args[100]; '; 
	'  integer nport; '; 
	'  integer nevprt=0; '; 
	' '; 
	'  /* Function Body */ ';  
	''
	'  /*     ending  subroutine */';
	'  flag=5 ;'];

for i=1:size(wfunclist)
  Code=[Code;
	'';
	'  '+wfunclist(i);
	'';];
end   
Code=[Code;
      '  return 0;'
      '} /* ending */';
      ''];
endfunction

function Code=c_make_initi(cpr)
//Copyright INRIA
//Author : Rachid Djenidi

///////////////////
//les pointeurs de cpr :
  z=cpr.state.z;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  
  Code=['/*'+part('-',ones(1,40))+' initi */ ';
	'int '
	rdnom+'_initi(told,outtb, iwa) ';  
	''; 
	'     /*Block initialization (flag=4)*/'
	'     double  *told, *outtb; '; 
	'     integer *iwa; '; 
	'{ '; 
	' '; 
	'  /* Local variables */ '; 
	'  integer flag; '; 
	'  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	'  double  *args[100]; '; 
	'  integer nport; '; 
	'  integer nevprt=0; '; 
	' '; 
	'  /* Function Body */ ';  
	'  ';
	'  flag=4 ;'];

  for i=1:size(wfunclist)
    Code=[Code
	  '  '+wfunclist(i);
	  '  '];
  end   
  Code=[Code
	'  return 0;'
	'} /* '+rdnom+'_initi */'];
endfunction

function Code=c_make_outtb()
//Copyright INRIA
//Author : Rachid Djenidi

///////////////////
//les pointeurs de cpr :
  z=cpr.state.z;
  funs=cpr.sim.funs;
  inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  
  iord=cpr.sim.iord;niord=size(iord,1);
  
  Code=['/*'+part('-',ones(1,40))+' outtbini */ ';
	'int '
	rdnom+'_outtb(told, outtb, iwa) ';  
	''; 
	'     /*Constants propagation*/'
	'     double  *told, *outtb; '; 
	'     integer *iwa; '; 
	'{ '; 
	' '; 
	'  /* Local variables */ '; 
	'  integer flag; '; 
	'  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	'  double  *args[100]; '; 
	'  integer nport; '; 
	'  integer nevprt=0; '; 
	' '; 
	'  /* Function Body */ ';  
	'  ';
	'  flag=1 ;'];
  
  for i=iord(:,1)'
    if (outptr(i+1)-outptr(i))>0 then
      Code=[Code
	    '  '+wfunclist(i);]
    end
  end   
  Code=[Code
	'  return 0;'
	'} /* '+rdnom+'_outtb */'];
endfunction

function [Code,actt,proto]=call_actuator(i)
  
  nin=inpptr(i+1)-inpptr(i); ///* number of input ports */
  nout=outptr(i+1)-outptr(i); ///* number of output ports */
  
  if funtyp(i)==0 then
    if nin==0 then
      uk=0;
      nuk=0;
      Code=+'args[0]=&(outtb[0]);';
    else
      lprt=inplnk(inpptr(i));
      uk=lnkptr(lprt);
      nuk=(lnkptr(lprt+1))-uk;
      Code='args[0]=&(outtb['+string(uk-1)+']);';
    end
  end
  
  //pour la fonction gui ont a : num de bloc l'adresse dans
  //nouveau  z et la taille du port

  actt=[i uk nuk bllst(i).ipar]
  Code($+1)='block_'+rdnom+'['+string(i-1)+'].nevprt=nevprt;'
  Code=[Code;
	 'nport = '+string(nbact)+';';
	 rdnom+'_actuator(&flag, &nport, &block_'+rdnom+'['+string(i-1)+'].nevprt, told, '+..
	 '(double *)args[0], &nrd_'+string(nuk)+');'];
  proto='void '+rdnom+'_actuator(int *, int *, int *, double *,"+...
	" double *, int *);"
  proto=cformatline(proto,70);
endfunction



function [Code,proto]=call_block(i)
 //Copyright INRIA
//Author : Rachid Djenidi
 
  nx=xptr(i+1)-xptr(i);
  nz=zptr(i+1)-zptr(i);
  nrpar=rpptr(i+1)-rpptr(i);
  nipar=ipptr(i+1)-ipptr(i);
  nin=inpptr(i+1)-inpptr(i); ///* number of input ports */
  nout=outptr(i+1)-outptr(i); ///* number of output ports */
  fun=funs(i);
  
   //l'adresse du pointeur de ipar
  if nipar<>0 then ipar=ipptr(i);else ipar=1;end
  
  //l'adresse du pointeur de rpar
  if nrpar<>0 then rpar=rpptr(i);else rpar=1; end

  //l'adresse du pointeur de z attention -1 pas sur
  if nz<>0 then z=zptr(i)-1;else z=0;end

  //l'adresse du pointeur de x
  if nx<>0 then x=xptr(i)-1;else x=0;end
  //
  [Code,proto]=callf(i);
endfunction


function [Code,proto]=callf(i)
//Generates the code associated with the call to the simulation function 
//of one original block
  
//Copyright INRIA
//Author : Rachid Djenidi
  Code=[];proto=''
  ftyp=funtyp(i)
  if ftyp<0 then return,end
  if ftyp>2000 then ftyp=ftyp-2000,end
  if ftyp>1000 then ftyp=ftyp-1000,end
  
  //******************************************
  //Generate c code for input output arguments
  //******************************************
  Code($+1)='block_'+rdnom+'['+string(i-1)+'].nevprt=nevprt;'
  if ftyp==0 then
    if nin>1 then
      for k=1:nin
	lprt=inplnk(inpptr(i)-1+k);
	uk=lnkptr(lprt);
	nuk=(lnkptr(lprt+1))-uk;
	Code($+1)='rdouttb['+string(k-1)+']=outtb['+string(uk-1)+'];';
      end
      Code($+1)='args[0]=&(rdouttb[0]);';
    elseif nin==0
      uk=0;
      nuk=0;
      Code($+1)=+'args[0]=&(outtb[0]);';
    else

      lprt=inplnk(inpptr(i));
      uk=lnkptr(lprt);
      nuk=(lnkptr(lprt+1))-uk;
      Code($+1)='args[0]=&(outtb['+string(uk-1)+']);';
    end
    if nout>1 then
      for k=1:nout
	lprt=outlnk(outptr(i)-1+k);
	yk=lnkptr(lprt);
	nyk=(lnkptr(lprt+1))-yk;
	Code($+1)='rdouttb['+string(k+nin-1)+']=outtb['+..
	    string(yk-1)+'];';
      end
      Code($+1)='args[1]=&(rdouttb['+string(nin)+']);';
    elseif nout==0
      yk=0;
      nyk=0;
      Code($+1)=+'args[1]=&(outtb[0]);';
    else
      lprt=outlnk(outptr(i));
      yk=lnkptr(lprt);
      nyk=(lnkptr(lprt+1))-yk;
      Code($+1)='args[1]=&(outtb['+string(yk-1)+']);';
    end
  elseif ftyp==1
    if nin>=1 then
      for k=1:nin
	lprt=inplnk(inpptr(i)-1+k);
	uk=lnkptr(lprt);
	nuk=(lnkptr(lprt+1))-uk;
      end
    end
    if nout>=1 then
      for k=1:nout
	lprt=outlnk(outptr(i)-1+k);
	yk=lnkptr(lprt);
	nyk=(lnkptr(lprt+1))-yk;
      end
    end
  end
  //************************
  //generate the call itself
  //************************
  if ftyp==0 | ftyp==1 then
    if (cpr.sim.funtyp(i)> 2000 & cpr.sim.funtyp(i)< 3000)
      CodeC=fun+'(&flag,&block_'+rdnom+'['+string(i-1)+'].nevprt,told,&(w['+..
           string(x)+']),&(x['+string(x)+']),&nrd_'+string(nx)+',..
           block_'+rdnom+'['+string(i-1)+'].z,&block_'+rdnom+'['+string(i-1)+'].nz,block_'+rdnom+'['+string(i-1)+'].evout,..
           &block_'+rdnom+'['+string(i-1)+'].nevout,block_'+rdnom+'['+string(i-1)+'].rpar,&block_'+rdnom+'['+string(i-1)+'].nrpar,.. 
           block_'+rdnom+'['+string(i-1)+'].ipar,&block_'+rdnom+'['+string(i-1)+'].nipar';
    proto='void '+fun+"(int *, int *, double *,"+...
	  " double *, double *, int *, double *, int *, double *,"+...
	  " int *, double *, int *,int *,int *"
    
    elseif (cpr.sim.funtyp(i)< 2000)
    CodeC='C2F(' +fun+')(&flag,&block_'+rdnom+'['+string(i-1)+'].nevprt,told,&(w['+..
           string(x)+']),&(x['+string(x)+']),&nrd_'+string(nx)+',..
           block_'+rdnom+'['+string(i-1)+'].z,&block_'+rdnom+'['+string(i-1)+'].nz,block_'+rdnom+'['+string(i-1)+'].evout,..
           &block_'+rdnom+'['+string(i-1)+'].nevout,block_'+rdnom+'['+string(i-1)+'].rpar,&block_'+rdnom+'['+string(i-1)+'].nrpar,.. 
           block_'+rdnom+'['+string(i-1)+'].ipar,&block_'+rdnom+'['+string(i-1)+'].nipar';
    proto='void '+"C2F(" +string(fun)+")(int *, int *, double *,"+...
	  " double *, double *, int *, double *, int *, double *,"+...
	  " int *, double *, int *,int *,int *"
    end
    if ftyp==0 then
      CodeC=CodeC+',(double *)args[0],&nrd_'+string(nuk)+..
	    ',(double *)args[1],&nrd_'+string(nyk)+');';
       proto=proto+", double *, int *, double *, int *);"
    elseif ftyp==1 
      if nin>=1 then
	for k=1:nin
	  lprt=inplnk(inpptr(i)-1+k);
	  uk=lnkptr(lprt);
	  CodeC=CodeC+',&(outtb['+string(uk-1)+']),&nrd_'+string(nuk);
	  proto=proto+", double *, int *"
	end
      end
      if nout>=1 then
	for k=1:nout
	  lprt=outlnk(outptr(i)-1+k);
	  yk=lnkptr(lprt);
	  CodeC=CodeC+',&(outtb['+string(yk-1)+']),&nrd_'+string(nyk);
	  proto=proto+", double *, int *"
	end
      end
//RN if ztyp, add two dummy arguments at the end
      if ztyp(i) then
	CodeC=CodeC+',w,&nrd_0);';
	proto=proto+',double *,int * );'
      else
	CodeC=CodeC+');';
	proto=proto+');'
      end
    end
  elseif ftyp==2 
    if ~ztyp(i) then
      CodeC=fun+'(&flag,&block_'+rdnom+'['+string(i-1)+'].nevprt,told,&(w['+string(x)+..
	    ']),&(x['+string(x)+']),&nrd_'+string(nx);
      CodeC=CodeC+',block_'+rdnom+'['+string(i-1)+'].z,&block_'+rdnom+'['+string(i-1)+'].nz,block_'+rdnom+'['+string(i-1)+'].evout,..
	    &block_'+rdnom+'['+string(i-1)+'].nevout,block_'+rdnom+'['+string(i-1)+'].rpar,&block_'+rdnom+'['+string(i-1)+'].nrpar,..
	    block_'+rdnom+'['+string(i-1)+'].ipar';
      CodeC=CodeC+',&block_'+rdnom+'['+string(i-1)+'].nipar,block_'+rdnom+'['+string(i-1)+'].inptr,block_'+rdnom+'['+string(i-1)+'].insz';
      CodeC=CodeC+',&block_'+rdnom+'['+string(i-1)+'].nin,block_'+rdnom+'['+string(i-1)+'].outptr,block_'+rdnom+'['+string(i-1)+'].outsz';
      CodeC=CodeC+',&block_'+rdnom+'['+string(i-1)+'].nout);';
      proto='void '+fun+"(int *, int *,double *,"+...
	  " double *, double *, int *, double *, int *, double *, "+...
	  " int *, double *, int *, int *, int *, "+...
	  " double **, int *, int *, double **,int"+...
	  " *, int *);" 
    else
	CodeC=fun+'(&flag,&block_'+rdnom+'['+string(i-1)+'].nevprt,told,&(w['+string(x)+..
	      ']),&(x['+string(x)+']),&nrd_'+string(nx);
	CodeC=CodeC+',block_'+rdnom+'['+string(i-1)+'].z,&block_'+rdnom+'['+string(i-1)+'].nz,block_'+rdnom+'['+string(i-1)+'].evout,..
	&block_'+rdnom+'['+string(i-1)+'].nevout,block_'+rdnom+'['+string(i-1)+'].rpar,&block_'+rdnom+'['+string(i-1)+'].nrpar,..
	block_'+rdnom+'['+string(i-1)+'].ipar';
	CodeC=CodeC+',&block_'+rdnom+'['+string(i-1)+'].nipar,block_'+rdnom+'['+string(i-1)+'].inptr,block_'+rdnom+'['+string(i-1)+'].insz,..
	    &block_'+rdnom+'['+string(i-1)+'].nin,block_'+rdnom+'['+string(i-1)+'].outptr,block_'+rdnom+'['+string(i-1)+'].outsz,..
            &block_'+rdnom+'['+string(i-1)+'].nout,block_'+rdnom+'['+string(i-1)+'].g,&block_'+rdnom+'['+string(i-1)+'].ng);';
	proto='void '+fun+"(int *, int *,double *,"+...
	  " double *, double *, int *, double *, int *, double *, "+...
	  " int *, double *, int *, int *, int *, "+...
	  " double **, int *, int *, double **,int"+...
	  " *, int *,double *,int * );" 
    end
  elseif ftyp==4
     CodeC=fun+'(&block_'+rdnom+'['+string(i-1)+'], flag);'
     proto='void '+fun+"(scicos_block *, int );" 
  end
  //
  Code=[Code;cformatline(CodeC,70);' ']
  proto=cformatline(proto,70);
endfunction
function [Code,capt,proto]=call_sensor(i)
  
  nin=inpptr(i+1)-inpptr(i); ///* number of input ports */
  nout=outptr(i+1)-outptr(i); ///* number of output ports */
  
  //declaration des I/O des blocs de type 1
  if funtyp(i)==0 then
    if nout==0 then
      yk=0;
      nyk=0;
      Code=+'args[1]=&(outtb[0]);';
    else
      lprt=outlnk(outptr(i));
      yk=lnkptr(lprt);
      nyk=(lnkptr(lprt+1))-yk;
      Code='args[1]=&(outtb['+string(yk-1)+']);';
    end

  end
  capt=[i yk nyk bllst(i).ipar]
  Code($+1)='block_'+rdnom+'['+string(i-1)+'].nevprt=nevprt;'
  Code=[Code;
	 'nport = '+string(nbcap)+';'; 
	 rdnom+'_sensor(&flag, &nport, &block_'+rdnom+'['+string(i-1)+'].nevprt, '+..
	 'told, (double *)args[1], &nrd_'+string(nyk)+');'];
  proto='void '+rdnom+'_sensor(int *, int *, int *, double *,"+...
	" double *, int *);"
  proto=cformatline(proto,70);
endfunction

function t1=cformatline(t ,l)
//utilitary fonction used to format long C instruction
//t : a string containing a C instruction
//l : max line length allowed

// Copyright INRIA
//Author : Rachid Djenidi
  
  sep=[',','+']
  l1=l-2
  t1=[]
  kw=strindex(t,' ')
  nw=0
  if kw<>[] then
    if kw(1)==1 then // there is leading blanks
      k1=find(kw(2:$)-kw(1:$-1)<>1)
      if k1==[] then // there is a single blank
	nw=1
      else
	nw=kw(k1(1))
      end
    end
  end
  t=part(t,nw+1:length(t));
  bl=part(' ',ones(1,nw))
  l1=l-nw;first=%t
  while %t 
    if length(t)<=l then t1=[t1;bl+t],return,end
    k=strindex(t,sep);
    if k==[] then t1=[t1;bl+t],return,end
    k($+1)=length(t)+1 // positions of the commas
    i=find(k(1:$-1)<=l&k(2:$)>l) //nearest left comma (reltively to l)
    if i==[] then i=1,end
    t1=[t1;bl+part(t,1:k(i))]
    t=part(t,k(i)+1:length(t))
    if first then l1=l1-2;bl=bl+'  ';first=%f;end
  end
endfunction
function  [ok,XX,alreadyran]=do_compile_superblock(XX,all_scs_m,numk,alreadyran)
// Transforms a given Scicos discrete and continuous SuperBlock into a C defined Block
// Copyright INRIA
//Author : Rachid Djenidi

  scs_m=XX.model.rpar
  par=scs_m.props;
  if alreadyran then 
    //terminate current simulation
    do_terminate()
    alreadyran=%f
  end
  hname=scs_m.props.title(1) //superblock name
  //***********************************************************
  //Check blocks properties and adapt them if necessary
  //***********************************************************

  IN=[];OUT=[];clkIN=[];numa=[];numc=[];
  for i=1:size(scs_m.objs)
     if typeof(scs_m.objs(i))=='Block' then  
      if scs_m.objs(i).gui=='CLKOUT_f' then	
        ok=%f;%cpr=list()
	message('Superblock should not have any activation output port.')
	return
      elseif scs_m.objs(i).gui=='IN_f' then
	//replace input ports by sensor blocks
	numc=numc+1
	scs_m.objs(i).gui='INPUTPORTEVTS';
	scs_m.objs(i).model.evtin=1
	scs_m.objs(i).model.sim(1)='capteur'+string(numc)
	IN=[IN scs_m.objs(i).model.ipar]
      elseif scs_m.objs(i).gui=='OUT_f' then
	//replace output ports by actuator blocks
	numa=numa+1
	scs_m.objs(i).gui='OUTPUTPORTEVTS';
	scs_m.objs(i).model.sim(1)='actionneur'+string(numa)
	OUT=[OUT  scs_m.objs(i).model.ipar]
      elseif scs_m.objs(i).gui=='CLKINV_f' then
	//replace event input ports by  fictious block
	scs_m.objs(i).gui='EVTGEN_f';
	scs_m.objs(i).model.sim(1)='bidon'
	clkIN=[clkIN scs_m.objs(i).model.ipar];
      elseif scs_m.objs(i).model.dep_ut(2)==%t then
	//check for time dependency PAS IICI
	ok=%f;%cpr=list()
	message('a block have time dependence.')
	return
      end
    end
  end

  //Check if input/output ports are numered properly
  IN=-sort(-IN);
  if or(IN<>[1:size(IN,'*')]) then 
    ok=%f;%cpr=list()
    message('Input ports are not numbered properly.')
    return
  end
  OUT=-sort(-OUT);
  if or(OUT<>[1:size(OUT,'*')]) then 
    ok=%f;%cpr=list()
    message('Output ports are not numbered properly.')
    return
  end
  clkIN=-sort(-clkIN);
  if or(clkIN<>[1:size(clkIN,'*')]) then 
    ok=%f;%cpr=list()
    message('Event input ports are not numbered properly.')
    return
  end

  //Check if there is more than one clock in the diagram
  szclkIN=size(clkIN,2);
  if szclkIN==0 then
    szclkIN=[]
  end

  [bllst,connectmat,clkconnect,cor,corinv,ok]=c_pass1(scs_m);

if ok==%f then message('Sorry: problem in the pre-compilation step.'),return, end
  a=[];b=[];tt=[];howclk=[];allhowclk=[];cap=[];act=[];

  ///**********************************
  for i=1:size(bllst)  
    for j=1:size(bllst) 
      
      if (bllst(i).sim(1)=='actionneur'+string(j)) then
	if tt<>i then
	  act=[act;i];
	  tt=i;
	end
      elseif (bllst(i).sim(1)=='capteur'+string(j)) then
	if tt<>i then
	  cap=[cap;i];
	  tt=i;
	end	
      elseif (bllst(i).sim(1)=='bidon') then
	if tt<>i then
	  allhowclk=[allhowclk;i];
	  tt=i;
	end
      end
    end
    ///**********************************
  end  
  if szclkIN>1 then
    //replace the N Event inputs by a fictious block with 2^N as many event
    //outputs
    output=ones((2^szclkIN)-1,1)
    bllst($+1)=scicos_model(sim=list('bidon',1),evtout=output,..
			    blocktype='d',..
			    firing=-output',dep_ut=[%f %f])
    corinv(size(bllst))=size(bllst)+1;
    howclk=size(bllst)
    // adjust the links accordingly
    for i=1:(2^szclkIN)-1
      vec=codebinaire(i,szclkIN)
      for j=1:szclkIN
	if vec(j)*allhowclk(j)>=1 then
	  for k=1:size(clkconnect,1)
	    if clkconnect(k,1)==allhowclk(j) then
	      clkconnect=[clkconnect;[howclk i clkconnect(k,3:4)]]
	    end
	  end
	end	
      end
    end
  elseif szclkIN==[]  then
    //superblock has no Event input, add a fictious clock
    bllst($+1)=scicos_model(sim=list('bidon',1),evtout=1,..
			    firing=0,blocktype='d',dep_ut=[%f %f])
    corinv(size(bllst))=size(bllst)+1;
    howclk=size(bllst);
  elseif szclkIN==1  then
    howclk=allhowclk;
  end  

  //mise en ordre de clkconnect
  if szclkIN>1 then
    newclkconnect=clkconnect;
    clkconnect=[];
    for i=1:size(newclkconnect,1)-1
      if  or(newclkconnect(i,:)<>newclkconnect(i+1,:)) then
	clkconnect=[clkconnect;newclkconnect(i,:)]
      end
    end
    if  or(newclkconnect($-1,:)<>newclkconnect($,:)) then
      clkconnect=[clkconnect;newclkconnect($,:)]
    end

    //suppression des blocs bidons
    newclkconnect=clkconnect;nkt=[];
    for i=1:szclkIN
      for k=1:size(newclkconnect,1)
	if newclkconnect(k,1)~=allhowclk(i) then
	  nkt=[nkt;newclkconnect(k,:)];
	end
      end
      newclkconnect=nkt;
      nkt=[];
    end
    clkconnect=newclkconnect;
  end


  //**************************************************
  // nouveau clkconnect avec liaisons sur les capteurs
  //**************************************************

  for i=1:size(cap,1)
    if szclkIN>1 then
      for j=1:(2^szclkIN)-1
	clkconnect=[clkconnect;[howclk j cap(i) 1]];
      end	
    else
      clkconnect=[clkconnect;[howclk 1 cap(i) 1]];
    end
  end
Code_gene_run=[];
%windo=xget('window')

cpr=newc_pass2(bllst,connectmat,clkconnect,cor,corinv);

//cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv)
xset('window',%windo)

  if cpr==list() then ok=%f,return, end

  ///////////////////
  //les pointeurs de cpr :
  x=cpr.state.x;z=cpr.state.z;
  outtb=cpr.state.outtb;
 
  
//RN
zcptr=cpr.sim.zcptr;
//
  rpptr=cpr.sim.rpptr;
  ipptr=cpr.sim.ipptr;funs=cpr.sim.funs;xptr=cpr.sim.xptr;
  zptr=cpr.sim.zptr;inpptr=cpr.sim.inpptr;inplnk=cpr.sim.inplnk;
  outptr=cpr.sim.outptr;outlnk=cpr.sim.outlnk;
  lnkptr=cpr.sim.lnkptr;
  ordclk=cpr.sim.ordclk;
  funtyp=cpr.sim.funtyp;cord=cpr.sim.cord;ncord=size(cord,1);
  nblk=cpr.sim.nblk;ztyp=cpr.sim.ztyp;
  clkptr=cpr.sim.clkptr
  //taille totale de z : nztotal
  nztotal=size(z,1);
  
  //*******************************
  //Checking if superblock is valid
  //*******************************
  msg=[]
  for i=1:length(funs)-1
    if funtyp(i)==3 then
      msg=[msg;'Type 3 block''s not allowed']
//RN   elseif ztyp(i)<>0 then
    elseif (zcptr(i+1)-zcptr(i))<>0 then
//
      msg=[msg;'Active zero crossing block''s not allowed']
    elseif (xptr(i+1)-xptr(i))<>0 then
      msg=[msg;'Continuous state block''s not allowed']
    elseif (clkptr(i+1)-clkptr(i))<>0 &funtyp(i)>-1 &funs(i)~='bidon' then
      msg=[msg;'Regular block generating activation not allowed yet']
    end
    if msg<>[] then message(msg),ok=%f,return,end
  end
  
  //********************************************************
  // Change logical units for readf and writef blocks if any ???
  //********************************************************
  lunit=0
  for d=1:length(funs)
    if funs(d)=='readf'  then
      z(zptr(d)+2)=lunit
      lunit=lunit+1;
    elseif funs(d)=='writef'
      z(zptr(d)+1)=lunit
      lunit=lunit+1;
    end
  end
  
  
  //***********************************
  // Get the name of the file
  //***********************************
  foo=3;okk=%f;rdnom='foo';rpat=getcwd();archname='';
  label1=[hname;getcwd()+'/'+hname;''];
 
  while %t do
    [okk,rdnom,rpat,archname,label1]=getvalue(..
			'PLEASE, GIVE US SOME INFORMATION. ',..
		        ['New block''s name :';
		        'Created files Path';
		        'Other object files to link with ( if any)'],..
		        list('str',1,'str',1,'str',1),label1);
    if okk==%f then ok=%f;return; end
  
    dirinfo=fileinfo(rpat)
    if dirinfo==[] then
      ok=mkdir(rpat)    
      if ~ok then 
	x_message('Directory '+rpat+' cannot be created');
      end
    elseif filetype(dirinfo(2))<>'Directory' then
      ok=%f;x_message(rpat+' is not a directory');
    end

    if stripblanks(rdnom)==emptystr() then 
      ok=%f;x_message('sorry C file name not defined');
    end
    if ok then break,end
  end
  
  //////////////////////////////////////////////////
  
  maxnrpar=max(rpptr(2:$)-rpptr(1:$-1))
  maxnipar=max(ipptr(2:$)-ipptr(1:$-1))
  maxnx=max(xptr(2:$)-xptr(1:$-1))
  maxnz=max(zptr(2:$)-zptr(1:$-1))
  maxnin=max(inpptr(2:$)-inpptr(1:$-1))
  maxnout=max(outptr(2:$)-outptr(1:$-1))
  
  maxtotal=max([maxnrpar;maxnipar;maxnx;maxnz;maxnin;maxnout]);
  if maxtotal< length(cpr(2).outtb) then
    maxtotal=length(cpr(2).outtb)
  end
  

  //************************************************************************
  //generate the call to the blocks and blocs simulation function prototypes
  //************************************************************************
  wfunclist=list();
  nbcap=0;nbact=0;capt=[];actt=[];Protos=[];
  dfuns=[]
  for i=1:length(funs)
    if or(i==act) then //block is an actuator
      nbact=nbact+1;
      [Code,actti,protoi]=call_actuator(i)
      wfunclist($+1)=[Code;'   if(flag < 0 ) return(5 - flag); ']
      if nbact==1 then Protos=[Protos;'';protoi],end   
      actt=[actt;actti]
    elseif or(i==cap) then //block is a sensor 
      nbcap=nbcap+1;
      [Code,capti,protoi]=call_sensor(i)
      wfunclist($+1)=[Code;'   if(flag < 0 ) return(5 - flag); ']
      if nbcap==1 then Protos=[Protos;'';protoi] ,end
      capt=[capt;capti]
    elseif funs(i)=='bidon'
      wfunclist($+1)=' '
    else
      ki=find(funs(i)==dfuns)           
      dfuns=[dfuns;funs(i)]
      [Code,protoi]=call_block(i)
      if Code<>[] then
	wfunclist($+1)=[Code;'   if(flag < 0 ) return(5 - flag); ']
      else
	wfunclist($+1)=' ';
      end
      if ki==[] then Protos=[Protos;'';protoi],end 
    end

  end


  //**************************************
  //on prend en compte l'ordre des numerotation graphique des
  //capteur et des actionneur 
  [junk,index]=sort(-actt(:,$));
  actt=actt(index,1:$) ;
  [junk,index]=sort(-capt(:,$));
  capt=capt(index,1:$) ;
  
  
  //***************************************************
  //Compute the initial state and outtb (links)  values
  //***************************************************
  //petite modification pour uniformiser les fichiers capteurs 
  //et actionneurs ???
  rdcpr=cpr.sim.funs;
  for r=1:length(cap),rdcpr(cap(r))='bidon';end
  for r=1:length(act),rdcpr(act(r))='bidon';end
  Total_rdcpr=cpr.sim;Total_rdcpr.funs=rdcpr;
  //
  tcur=0;
  tf=scs_m.props.tf;
  tolerances=scs_m.props.tol;
  //[state,t]=scicosim(cpr.state,tcur,tf,Total_rdcpr,'start',tolerances);
 // cpr.state=state;
  z=cpr.state.z;outtb=cpr.state.outtb;
  //[junk_state,t]=scicosim(cpr.state,tcur,tf,Total_rdcpr,'finish',tolerances);
 
  
  //***********************************
  // Scilab and C files generation
  //***********************************
  ok=gen_gui();
  if ok then ok=gen_ccode(),end
  
  //*****************
  //Compile and link
  //*****************

  if ok then [ok,Makename]=buildnewblock(),end
  if ok then ok=gen_loader(),end

  if ok then
    //load the gui function
    getf(rpat+'/'+rdnom+'_c.sci');
    //Change diagram superblock to new generated block
    XX=update_block(XX);
    execstr(rdnom+'_c=resume('+rdnom+'_c)')
  end
endfunction

function vec=codebinaire(v,szclkIN)
  vec=zeros(1,szclkIN)
  for i=1:szclkIN
    w=v/2;
    vec(i)=v-2*int(w);
    v=int(w);
  end

endfunction

function XX=update_block(XX)
  execstr('o='+rdnom+'_c(''define'')')
  XX.model=o.model
  XX.gui=rdnom+'_c';
  XX.graphics.gr_i=o.graphics.gr_i
endfunction

 
function [CCode,FCode]=gen_blocks()
//Generates Code for dynamically linked Fortran and C Blocks  
  CCode=[]
  FCode=[]
  
  kdyn=find(funtyp>1000) //dynamically linked blocs
			 //100X : Fortran blocks
			 //200X : C blocks
  			 
  //if kdyn==[] then
    //return
  //end
  
  if (size(kdyn,'*') >1)
    kfuns=[]; 
    //get the block data structure in the initial scs_m structure
    if size(corinv(kdyn(1)),'*')==1 then
      O=scs_m.objs(corinv(kdyn(1)));
    else
      path=list('objs');
      for l=corinv(kdyn(1))(1:$-1),path($+1)=l;path($+1)='model';path($+1)='rpar';path($+1)='objs';end
      path($+1)=corinv(kdyn(1))($);
      O=scs_m(path);
    end
    if funtyp(kdyn(1))>2000 then
      //C block
      CCode=[CCode;O.graphics.exprs(2)]
    else
      FCode=[FCode;O.graphics.exprs(2)]
    end
    kfuns=funs(kdyn(1));
    for i=2:size(kdyn,'*')
      //get the block data structure in the initial scs_m structure
      if size(corinv(kdyn(i)),'*')==1 then
        O=scs_m.objs(corinv(kdyn(i)));
      else
        path=list('objs');
         for l=corinv(kdyn(i))(1:$-1),path($+1)=l;path($+1)='model';path($+1)='rpar';path($+1)='objs';end
        path($+1)=corinv(kdyn(i))($);
        O=scs_m(path);
      end
      if (find(kfuns==funs(kdyn(i))) == [])
	kfuns=[kfuns;funs(kdyn(i))];
        if funtyp(kdyn(i))>2000  then
          //C block
          CCode=[CCode;O.graphics.exprs(2)]
        else
          FCode=[FCode;O.graphics.exprs(2)]
        end
      end
    end
  elseif (size(kdyn,'*')==1)
    //get the block data structure in the initial scs_m structure
    if size(corinv(kdyn),'*')==1 then
      O=scs_m.objs(corinv(kdyn));
    else
      path=list('objs');
      for l=corinv(kdyn)(1:$-1),path($+1)=l;path($+1)='model';path($+1)='rpar';path($+1)='objs';end
      path($+1)=corinv(kdyn)($);
      O=scs_m(path);
    end
    if funtyp(kdyn)>2000 then
      //C block
      CCode=[CCode;O.graphics.exprs(2)]
    else
      FCode=[FCode;O.graphics.exprs(2)]
    end
  end
  if CCode==[]
    CCode=['void no_ccode()'
           '{'
           '  return;'
           '}']
  end
endfunction
function ok=gen_ccode();
//Generates the C code for new block simulation
  
//Copyright INRIA
//Author : Rachid Djenidi
  [CCode,FCode]=gen_blocks()

  Code=[make_decl();
	Protos;
	make_static()
	make_computational()         
	make_main1();
	make_main2();
	make_init();
	make_end()
	c_make_doit1();
	c_make_doit2();
	c_make_outtb();
	c_make_initi();
	c_make_endi();
	make_putevs()]
  
	
  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end
  if FCode<>[] then
    ierr=execstr('mputl(FCode,rpat+''/''+rdnom+''f.f'')','errcatch')
    if ierr<>0 then
      message(lasterror())
      ok=%f
      return
    end
  end
  if CCode<>[] then
    ierr=execstr('mputl(CCode,rpat+''/''+rdnom+''_Cblocks.c'')','errcatch')
    if ierr<>0 then
      message(lasterror())
      ok=%f
      return
    end
  end
  Code=['#include '"scicos_block.h'"';
	'#include '"machine.h'"';
	make_actuator(%f)
	make_sensor(%f)]
  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_void_io.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end

  Code=[make_decl_standalone()
	Protos;
	make_static_standalone()
        make_standalone()]
  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_standalone.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end
 
  
  Code=[make_decl_standalone()
	make_outevents()
	make_actuator(%t)
	make_sensor(%t)]
  
  created=[];reponse=[];  
  created=fileinfo(rpat+'/'+rdnom+'_act_sens_events.c')
  if created~=[] then
    reponse=x_message(['File: ""'+rdnom+'_act_sens_events.c"" already exists,';'do you want to replace it ?'],['Yes','No']);
  end
  if reponse==1 |  reponse==[] then
    ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_act_sens_events.c'')', ...
		 'errcatch')
  end
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end
  
  
endfunction


  
function ok=gen_gui();
//creates the Scicos GUI function associated with the new block
//Copyright INRIA
//Author : Rachid Djenidi
  clkinput=ones(clkIN)';
  Code=['function [x,y,typ]='+rdnom+'_c(job,arg1,arg2)';
	'// Copyright INRIA';
	'x=[];y=[];typ=[];';
	'select job';
	'case ''plot'' then';
	'  standard_draw(arg1)';
	'case ''getinputs'' then';
	'  [x,y,typ]=standard_inputs(arg1)';
	'case ''getoutputs'' then';
	'  [x,y,typ]=standard_outputs(arg1)';
	'case ''getorigin'' then';
	'  [x,y]=standard_origin(arg1)';
	'case ''set'' then';
	'  x=arg1;';
	'case ''define'' then'
	'  '+sci2exp(capt(:,3),'in',70); //input ports sizes
	'  '+sci2exp(actt(:,3),'out',70); //output ports sizes
	'  '+sci2exp(x,'x',70); //initial continuous state
	'  '+sci2exp(z,'z',70); //initial discrete state
	'  '+sci2exp(outtb,'outtb',70); //initial link values
	'  iwa=zeros('+string(clkptr($))+',1)';
	'  work=zeros('+string(nblk)+',1)';
	'  Z=[z;outtb;iwa;work]';
        '  '+sci2exp(cpr.sim.rpar,'rpar',70); //real parameters
        '  '+sci2exp(cpr.sim.ipar,'ipar',70);//integer parameters
	'  '+sci2exp(clkinput,'clkinput',70);
        '  nzcross='+string(sum(cpr.sim.zcptr(2:$)-cpr.sim.zcptr(1:$-1)))';
	'  model=scicos_model(sim=list('''+rdnom+''',4),in=in,out=out,..'
	'          evtin=clkinput,state=x,dstate=Z,rpar=rpar,ipar=ipar,..'
	'          blocktype=''c'',dep_ut=[%t %f],'' '',nzcross=nzcross)'
	'  gr_i=''xstringb(orig(1),orig(2),'''''+rdnom+''''',sz(1),..'
	'         sz(2),''''fill'''')''';
	'  x=standard_define([2 2],model,[],gr_i)';
	'end'
        'endfunction'];
  //Create file
  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_c.sci'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
  end
endfunction
  
  

function ok=gen_loader()
//generates the Scilab script for defining the newly created block
//into Scilab.
 
  SCode=['//exec file used to load the ""compiled"" block into Scilab'
	 'rdnom='+sci2exp(rdnom);
	 '// get the absolute path of this loader file' 
	 'DIR=get_absolute_file_path(rdnom+''_loader.sce'')'
	 'Makename = DIR+rdnom+''_Makefile'';'
	 'select COMPILER'
	 'case ''VC++''   then '
	 '  Makename = strsubst(Makename,''/'',''\'')+''.mak'';'
	 'case ''ABSOFT'' then '
	 '  Makename = strsubst(Makename,''/'',''\'')+''.amk'';'
	 'end'
	 '//unlink if necessary'
	 '[a,b]=c_link(rdnom); while a ;ulink(b);[a,b]=c_link(rdnom);end';
	 'libn=ilib_compile('+sci2exp('lib'+rdnom)+',Makename)';
	 'if MSDOS then'
	 '  fileso=strsubst(libn,''/'',''\'')';
	 'else'
	 '  fileso=strsubst(libn,''\'',''/'')';
	 'end';
	 'link(fileso,rdnom,''c'')';
	 '//load the gui function';
	 'getf(DIR+''/''+rdnom+''_c.sci'');']
  ierr=execstr('mputl(SCode,rpat+''/''+rdnom+''_loader.sce'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end
endfunction
function Makename=gen_make(name,files,libs,Makename)
  if getenv('WIN32','NO')=='OK' then
    Makename=gen_make_win32(name,files,libs)
  else
    Makename=gen_make_unix(name,files,libs)
  end
endfunction

function Makename=gen_make_unix(name,files,libs,Makename)
  //   "OBJSSTAN="+strcat(strsubst(files,'_void_io','_standalone')+'.o',' ...
  //		')+' '+rdnom+'_act_sens_events.o'

  T=["# generated by builder.sce: Please do not edit this file"
     "# ------------------------------------------------------"
     "SCIDIR = "+SCI
     "OBJS = "+strcat(files+'.o',' ')      
     "OBJSSTAN="+rdnom+'_standalone.o '+rdnom+'_act_sens_events.o '+rdnom+'_Cblocks.o '
     "SCILIBS = $(SCIDIR)/libs/scicos.a $(SCIDIR)/libs/lapack.a "+..
     "$(SCIDIR)/libs/poly.a $(SCIDIR)/libs/calelm.a "+..
     "$(SCIDIR)/libs/blas.a $(SCIDIR)/libs/lapack.a"
     "LIBRARY =  lib"+name
     "OTHERLIBS = "+libs
     "include $(SCIDIR)/Makefile.incl";
     "CFLAGS = $(CC_OPTIONS) -I$(SCIDIR)/routines/"
     "FFLAGS = $(FC_OPTIONS) -I$(SCIDIR)/routines/" 
     "include $(SCIDIR)/config/Makeso.incl"
     "standalone: $(OBJSSTAN) $(OTHERLIBS) "
      ascii(9)+"f77 $(FFLAGS) -o $@  $(OBJSSTAN) $(OTHERLIBS) $(SCILIBS)"];
  mputl(T,Makename)
endfunction
//     "OBJSSTAN="+strcat(strsubst(files,'_void_io','_standalone')+'.o',' ') 

function Makename=gen_make_win32(name,files,libs,Makename)
WSCI=strsubst(SCI,'/','\') 
//  "OBJSSTAN="+strcat(strsubst(files,'_void_io','_standalone')+'.obj',' ')+' '+rdnom+'_act_sens_events.obj'
  T=["# generated by builder.sce: Please do not edit this file"
     "# ------------------------------------------------------"
     "SHELL = /bin/sh"
     "SCIDIR = "+SCI
     "SCIDIR1 = "+WSCI
     "SCILIBS = """+WSCI+"\bin\LibScilab.lib"""
     "LIBRARY = lib"+name
     "OBJS = "+strcat(files+'.obj',' ')
     'OBJSSTAN='+rdnom+'_standalone.obj '+rdnom+'_act_sens_events.obj '+rdnom+'_Cblocks.obj'
     "OTHERLIBS = "+libs
     ""
     "DUMPEXTS="""+WSCI+"\bin\dumpexts"""
     "SCIIMPLIB="""+WSCI+"\bin\LibScilab.lib"""
     ""
     "all::"
     "CC=cl"
     "LINKER=link"
     "LINKER_FLAGS=/NOLOGO /machine:ix86"
     "INCLUDES=-I"""+WSCI+"\routines\f2c""" 
     "CC_COMMON=-D__MSC__ -DWIN32 -c -DSTRICT -nologo $(INCLUDES)" 
     "CC_OPTIONS = $(CC_COMMON) -Od  -GB -Gd -W3"
     "CC_LDFLAGS = "
     "CFLAGS = $(CC_OPTIONS) -DFORDLL -I"""+WSCI+"\routines"""
     "FFLAGS = $(FC_OPTIONS) -DFORDLL -I"""+WSCI+"\routines"""
     ""
     "all :: $(LIBRARY).dll"
     " "
     "$(LIBRARY).dll: $(OBJS)"
     ascii(9)+"@echo Creation of dll $(LIBRARY).dll and import lib from ..."
     ascii(9)+"@echo $(OBJS)"
     ascii(9)+"@$(DUMPEXTS) -o ""$*.def"" ""$*.dll"" $**"
     ascii(9)+"@$(LINKER) $(LINKER_FLAGS) $(OBJS) $(SCIIMPLIB) $(XLIBSBIN) $(TERMCAPLIB) /nologo /dll /out:""$*.dll"" /implib:""$*.ilib"" /def:""$*.def""" 
     ".c.obj:"
     ascii(9)+"@echo ------------- Compile file $< --------------"
     ascii(9)+"$(CC) $(CFLAGS) $< "
     ".f.obj:"
     ascii(9)+"@echo ----------- Compile file $*.f (using f2c) -------------"
     ascii(9)+"@"""+WSCI+"\bin\f2c.exe"" $(FFLAGS) $*.f "
     ascii(9)+"@$(CC) $(CFLAGS) $*.c "
     ascii(9)+"@del $*.c "
     "clean::"
     ascii(9)+"@del *.CKP "
     ascii(9)+"@del *.ln "
     ascii(9)+"@del *.BAK "
     ascii(9)+"@del *.bak "
     ascii(9)+"@del *.def"
     ascii(9)+"@del *.dll"
     ascii(9)+"@del *.exp"
     ascii(9)+"@del *.ilib"
     ascii(9)+"@del errs" 
     ascii(9)+"@del *~ "
     ascii(9)+"@del *.obj"
     ascii(9)+"@del .emacs_* "
     ascii(9)+"@del tags "
     ascii(9)+"@del tags "
     ascii(9)+"@del TAGS "
     ascii(9)+"@del make.log "
     ""
     "distclean:: clean "
     " "
     "standalone: $(OBJSSTAN) $(OTHERLIBS) "
      ascii(9)+"$(LINKER) $(LINKER_FLAGS)  $(OBJSSTAN) $(OTHERLIBS) $(SCILIBS)  /out:standalone.exe "]

  select COMPILER;
    case 'VC++'   then 
    makename = strsubst(Makename,'/','\')+'.mak'
    case 'ABSOFT' then 
    makename = strsubst(Makename,'/','\')+'.amk'
  end
  mputl(T,makename)   
endfunction

function Code=make_actuator(standalone)
// Generating the routine for actuators interfacing
//Copyright INRIA
//Author : Rachid Djenidi
  
Call=['/*'+part('-',ones(1,40))+' Actuators */ ';
      'void ';
      rdnom+'_actuator(flag,nport,nevprt,t,u,nu)']
comments=['     /*'
	  '      * To be customized for standalone execution';
	  '      * flag  : specifies the action to be done'
	  '      * nport : specifies the  index of the Super Bloc '
	  '      *         regular input (The input ports are numbered '
	  '      *         from the top to the bottom ) '
	  '      * nevprt: indicates if an activation had been received'
	  '      *         0 = no activation'
	  '      *         1 = activation'
	  '      * t     : the current time value'
	  '      * u     : the vector inputs value'
	  '      * nu    : the input  vector size'
	  '      */']

dcl=['     integer *flag,*nevprt,*nport;'
     '     integer *nu;'
     ''
     '     double  *t, u[];'
     '{'
     '  int k;'];

if standalone then
  a_actuator=['  /* skeleton to be customized */' 
	      '  switch (*flag) {'
	      '  case 2 : ' 
	      '    if(*nevprt>0) {/* get the input value */'
	      '      for (k=0;k<*nu;k++) printf(""Actuator: time=%f, u(%d) of actuator %d is %f \n"", *t, k, *nport, u[k]);  '
	      '    } ;'
	      '    break;'
	      '  case 4 : /* actuator initialisation */'
	      '    /* do whatever you want to initialize the actuator */ '
	      '    break;'
	      '  case 5 : /* actuator ending */'
	      '    /* do whatever you want to end the actuator */'
	      '    break;'
	      '  }']
else
 a_actuator=[]
end
nc=size(act,'*')
Code=[]
if nc==1|~standalone then
  Code=[Call
	comments
	dcl
	a_actuator
	'}']
elseif nc>1 then
  S='  switch (*nport) {'
  for k=1:nc
    S=[S;
       '  case '+string(k)+' :/* Port number '+string(k)+' ----------*/'
       '  '+a_actuator
       '  break;']
  end
  S=[S;'  }']  
  Code=[Code
	Call
	comments
	dcl
	S
	'}']
end
endfunction

function Code=make_computational()
//generates the scicos computational function associated with the block
//Copyright INRIA
  z=cpr.state.z;
  outtb=cpr.state.outtb;
  tevts=cpr.state.tevts;evtspt=cpr.state.evtspt;
  // n'est pas utilise
  //nevts=size(evtspt,1);
  
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  clkptr=cpr.sim.clkptr;ordptr=cpr.sim.ordptr;
  pointi=cpr.state.pointi;ztyp=cpr.sim.ztyp;
  zcptr=cpr.sim.zcptr;zptr=cpr.sim.zptr;
  rpptr=cpr.sim.rpptr;ipptr=cpr.sim.ipptr;
  inpptr=cpr.sim.inpptr;
  funs=cpr.sim.funs;xptr=cpr.sim.xptr;
  modptr=cpr.sim.modptr;inplnk=cpr.sim.inplnk;
  nblk=cpr.sim.nblk;outlnk=cpr.sim.outlnk;
  nZ=size(z,'*')+size(outtb,'*')+clkptr($)
//copy inputs
Code=[Code;
      '  /*Copy inputs in the block outtb */'];
for i=1:size(capt,1)
   ni=capt(i,3) // dimension of ith input
  Code=[Code;
        '  block_outtb['+string(capt(i,2)-1+(0:ni-1)')+'] = ..
         u['+string(capt(i,4)-1)+']['+string(0:ni-1)'+'];']
end

Code=['/*'+part('-',ones(1,40))+' Block Computational function */ ';
      'int  '+rdnom+'(scicos_block *block, int flag)'     
      '{'
      '  double* z = block->z;' 
      '  double** u = block->inptr;'
      '  double** y = block->outptr;'
      '  int nevprt  = block->nevprt;'
      '  double* rpar = block->rpar;' 
      '  int nrpar = block->nrpar;'
      '  int* ipar = block->ipar;'
      '  int nipar = block->nipar;'
      '  void **work = block->work;'
      '  double t;'
      '  /*  block_outtb is catenated at the end of z*/'
      '  double* block_outtb = z+'+string(nztotal)+';'
      '  int kf;' 
      '  int* reentryflag;'
      '  work = z+'+string(nZ)+'; '
      '  t=get_scicos_time();' 
      ' '];
Code=[Code;
      '  /*Copy inputs in the block outtb */'];
for i=1:size(capt,1)
   ni=capt(i,3) // dimension of ith input
  Code=[Code;
        '  block_outtb['+string(capt(i,2)-1+(0:ni-1)')+'] = u['+..
	              string(capt(i,4)-1)+']['+string(0:ni-1)'+'];';]
end

Code=[Code;
      '  set_nevprt(nevprt);';
      ' '  
      '  if (flag != 4 && flag != 6 && flag != 5){'
      '    reentryflag=(int*) ((scicos_block *)(*block->work)+'+string(nblk)+');'
      '    if ( *reentryflag ==0){'      
      '      *reentryflag =1;'
      '      block_'+rdnom+'=(scicos_block*) *block->work;']
      for kf=1:nblk        
        nin=inpptr(kf+1)-inpptr(kf); ///* number of input ports */
        nout=outptr(kf+1)-outptr(kf); ///* number of output ports */
        for k=1:nin
          lprt=inplnk(inpptr(kf)-1+k);
          Code=[Code;
	        '      block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+'] = &(block_outtb['+string(lnkptr(lprt)-1)+']);']
        end
        for k=1:nout
          lprt=outlnk(outptr(kf)-1+k);
          Code=[Code
	        '      block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+']=&(block_outtb['+string(lnkptr(lprt)-1)+']);']
        end
        Code=[Code
	      '      block_'+rdnom+'['+string(kf-1)+'].z=&(z['+string(zptr(kf)-1)+']);'
	     ]
	if ~(part(cpr.sim.funs(kf),1:7) == 'capteur' | part(cpr.sim.funs(kf),1:10) == 'actionneur' |cpr.sim.funs(kf)=='bidon') then
	  Code=[Code
		'      block_'+rdnom+'['+string(kf-1)+'].rpar=&(rpar['+string(rpptr(kf)-1)+']);';
		'      block_'+rdnom+'['+string(kf-1)+'].ipar=&(ipar['+string(ipptr(kf)-1)+']);';
		'      block_'+rdnom+'['+string(kf-1)+'].work=(void **)(((double *)work)+'+string(kf-1)+');']
	  
	end
      end 
      Code=[Code
      '    }'
      '  }'
      '  if (flag == 1) { /* update outputs */';
      '    '+rdnom+'main1(block_'+rdnom+',z,&t);'; 
      '  }else if (flag == 2) { /* update discrete states */']

      if (outptr(2)-outptr(1) ~=0) then                    
        Code=[Code;
	'    '+rdnom+'main2(block_'+rdnom+',z,&t);']
      else
        Code=[Code;
	'    /* exception block */';
        '    '+rdnom+'main1(block_'+rdnom+',z,&t);';
	'    '+rdnom+'main2(block_'+rdnom+',z,&t);']
      end
      Code=[Code;
      '  }else if (flag == 4) { /* initialisation */'
      '    if ((*block->work=scicos_malloc(sizeof(scicos_block)*'+string(nblk)+'+sizeof(int)))== NULL ) return 0;';
	    '    reentryflag=(int*) ((scicos_block *)(*block->work)+'+string(nblk)+');'
            '    *reentryflag=0;'
	    '    block_'+rdnom+'=(scicos_block*) *block->work;'];
      
 for kf=1:nblk        

     nin=inpptr(kf+1)-inpptr(kf); ///* number of input ports */
     nout=outptr(kf+1)-outptr(kf); ///* number of output ports */
     Code=[Code;
	   '    block_'+rdnom+'['+string(kf-1)+'].type = '+string(funtyp(kf))+';';
	   '    block_'+rdnom+'['+string(kf-1)+'].ztyp = '+string(ztyp(kf))+';';
	   '    block_'+rdnom+'['+string(kf-1)+'].ng = '+string(zcptr(kf+1)-zcptr(kf))+';';
	   '    block_'+rdnom+'['+string(kf-1)+'].nz = '+string(zptr(kf+1)-zptr(kf))+';';
	   '    block_'+rdnom+'['+string(kf-1)+'].nrpar = '+string(rpptr(kf+1)-rpptr(kf))+';';
	   '    block_'+rdnom+'['+string(kf-1)+'].nipar = '+string(ipptr(kf+1)-ipptr(kf))+';'
	   '    block_'+rdnom+'['+string(kf-1)+'].nin = '+string(inpptr(kf+1)-inpptr(kf))+';';
	   '    block_'+rdnom+'['+string(kf-1)+'].nout = '+string(outptr(kf+1)-outptr(kf))+';';
	   '    block_'+rdnom+'['+string(kf-1)+'].nevout = '+string(clkptr(kf+1)-clkptr(kf))+';';
	   '    block_'+rdnom+'['+string(kf-1)+'].nmode = '+string(modptr(kf+1)-modptr(kf))+';';
	   '    if ((block_'+rdnom+'['+string(kf-1)+'].insz=malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL ) return 0;';
	   '    if ((block_'+rdnom+'['+string(kf-1)+'].inptr=malloc(sizeof(double*)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL ) return 0;';
	   '    if ((block_'+rdnom+'['+string(kf-1)+'].evout=calloc(block_'+rdnom+'['+string(kf-1)+'].nevout,sizeof(double)))== NULL )return 0;'];
     
     for k=1:nin
       lprt=inplnk(inpptr(kf)-1+k);
       Code=[Code
	     '    block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+'] = &(block_outtb['+string(lnkptr(lprt)-1)+']);'
	     '    block_'+rdnom+'['+string(kf-1)+'].insz['+string(k-1)+'] = '+string(lnkptr(lprt+1)-lnkptr(lprt))+';'];
     end 
     Code=[Code
	   '    if ((block_'+rdnom+'['+string(kf-1)+'].outsz=malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL ) return 0;';
	   '    if ((block_'+rdnom+'['+string(kf-1)+'].outptr=malloc(sizeof(double*)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL ) return 0;'];
     
     for k=1:nout
       lprt=outlnk(outptr(kf)-1+k);
       Code=[Code
	     '    block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+']=&(block_outtb['+string(lnkptr(lprt)-1)+']);'
	     '    block_'+rdnom+'['+string(kf-1)+'].outsz['+string(k-1)+']='+string(lnkptr(lprt+1)-lnkptr(lprt))+';'];
     end
     Code=[Code
	   '    block_'+rdnom+'['+string(kf-1)+'].z=&(z['+ ...
	   string(zptr(kf)-1)+']);';
	   ]
     if ~(part(cpr.sim.funs(kf),1:7) == 'capteur' | part(cpr.sim.funs(kf),1:10) == 'actionneur' |cpr.sim.funs(kf)=='bidon') then
       Code=[Code
	     '    block_'+rdnom+'['+string(kf-1)+'].rpar=&(rpar['+string(rpptr(kf)-1)+']);';
	     '    block_'+rdnom+'['+string(kf-1)+'].ipar=&(ipar['+string(ipptr(kf)-1)+']);';
	     '    block_'+rdnom+'['+string(kf-1)+'].work=(void **)(((double *"+...
	     " )work)+'+string(kf-1)+');'
	    ]
     end
     Code=[Code
	   '    block_'+rdnom+'['+string(kf-1)+'].nevprt=nevprt;']
     //end

 end 
 Code=[Code;
       ' ';
       '    '+rdnom+'_init(block_'+rdnom+',z,&t);';
       '  } ';
       '  else if (flag == 5) { /* ending */';
       '    block_'+rdnom+'=*block->work;']
 for kf=1:nblk        
   nin=inpptr(kf+1)-inpptr(kf); ///* number of input ports */
   nout=outptr(kf+1)-outptr(kf); ///* number of output ports */
   for k=1:nin
     lprt=inplnk(inpptr(kf)-1+k);
     Code=[Code;
	   '    block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+'] = &(block_outtb['+string(lnkptr(lprt)-1)+']);']
   end
   for k=1:nout
     lprt=outlnk(outptr(kf)-1+k);
     Code=[Code
	   '    block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+']=&(block_outtb['+string(lnkptr(lprt)-1)+']);']
   end
   Code=[Code
	 '    block_'+rdnom+'['+string(kf-1)+'].z=&(z['+ ...
	 string(zptr(kf)-1)+']);'
	 ]
   if ~(part(cpr.sim.funs(kf),1:7) == 'capteur' | part(cpr.sim.funs(kf),1:10) == 'actionneur' |cpr.sim.funs(kf)=='bidon') then
     Code=[Code
	 '    block_'+rdnom+'['+string(kf-1)+'].rpar=&(rpar['+string(rpptr(kf)-1)+']);';
	 '    block_'+rdnom+'['+string(kf-1)+'].ipar=&(ipar['+string(ipptr(kf)-1)+']);';
	 '    block_'+rdnom+'['+string(kf-1)+'].work=(void **)(((double *)work)+'+string(kf-1)+');']
     
   end
 end 
 Code=[Code
       '    '+rdnom+'_end(block_'+rdnom+',z,&t);';]
 Code=[Code;
       '    for (kf = 0; kf < '+string(nblk)+'; ++kf) {'
       '      if (block_'+rdnom+'[kf].insz!=NULL) {'
       '        free(block_'+rdnom+'[kf].insz);'
       '      }else {'
       '        break;'
       '      } '   
       '      if (block_'+rdnom+'[kf].outsz!=NULL){'
       '        free(block_'+rdnom+'[kf].outsz);'
       '      }else {'
       '        break;'
       '      }'    
       '      if (block_'+rdnom+'[kf].evout!=NULL){'
       '        free(block_'+rdnom+'[kf].evout);'
       '      }else {'
       '        break;'
       '      }'
       '    }' 
       '    scicos_free(block_'+rdnom+');'    
       '  } '];
 
 
 //copy outputs
 for i=1:size(actt,1)
   ni=actt(i,3) // dimension of ith output
   Code=[Code;
	 '  y['+string(actt(i,4)-1)+']['+string(0:ni-1)'+'] =  block_outtb['+..
	 string(actt(i,2)-1+(0:ni-1)')+'];']
 end
 
 //Assemble the code
 
 Code=[Code;
       '  return 0;'
       ''
       '} /* '+rdnom+' */']
 
endfunction
function Code=make_decl()
//generates  procedure declarations
//Copyright INRIA
//Author : Rachid Djenidi

  Code=['/* SCILAB Computational function  */';
	'/*     Copyright INRIA */';
	'/*     Generated by Code_Generation toolbox of Scicos with '+ ..
	getversion()+' */';
	'/*     date : '+date(getdate())+' */';
	''
	'#include <stdio.h>';
	'#include <memory.h>';
	'#include <string.h>';
	'#include '"'+SCI+'/routines/machine.h'"';
	'#include '"'+SCI+'/routines/sun/link.h'"';
	'#include '"'+SCI+'/routines/scicos/scicos.h'"';	
	' ';
        cformatline('void '+rdnom+'main1(scicos_block *,double *, double * );',70);
	' ';
	cformatline('void '+rdnom+'main2(scicos_block *,double *, double * );',70);
	' ';
	cformatline('void '+rdnom+'_init(scicos_block *,double *, double * );',70);
	' ';
	cformatline('void '+rdnom+'_end(scicos_block *,double *, double * );',70);
	' ';
	'int '+rdnom+'()  ;';
	' ';
	cformatline('int '+rdnom+'ddoit1(double *, double *,'+...
		     'double *, int *);',70);
	' ';
        cformatline('int '+rdnom+'ddoit2(double *, double *,'+...
		     'double *, int *);',70);
	' ';
	cformatline('int '+rdnom+'edoit1(double *, double *,'+...
		     'int *, int *, int *);',70);	
	' ';
        cformatline('int '+rdnom+'_initi(double *, double *, int *);',70);
	' ';
	cformatline('int '+rdnom+'_endi(double *, double *, int *);',70);
	' ';
	cformatline('int '+rdnom+'_outtb(double *, double *, int *);',70);
	' ';
	cformatline('int '+rdnom+'_putevs(double *, int *, int *);',70);
	'/* ---- block simulation functions -------*/'
	' '];
endfunction

function Code=make_decl_standalone()
//generates  procedure declarations
//Copyright INRIA
//Author : Rachid Djenidi
  
  Code=['/* Code prototype for standalone use  */';
	'/*     Generated by Code_Generation toolbox of Scicos with '+ ..
	getversion()+' */';
	'/*     date : '+date(getdate())+' */';
	''
	'#include <stdio.h>';
	'#include <string.h>';
	'#include '"scicos_block.h'"';
	'#include '"machine.h'"';
	'';
	cformatline('void '+rdnom+'main1(scicos_block *block_'+rdnom+',double *z, double *t);',70);
	'';
	cformatline('void '+rdnom+'main2(scicos_block *block_'+rdnom+',double *z, double *t) ;',70);
	'';
	cformatline('void '+rdnom+'_init(scicos_block *block_'+rdnom+',double *z, double *t) ;',70);
	'';
	cformatline('void '+rdnom+'_end(scicos_block *block_'+rdnom+',double *z, double *t) ;',70);
	''
	cformatline('int '+rdnom+'_sim(double tf)"+...
		    " ;',70);
	cformatline('int '+rdnom+'ddoit1(double *, double *,'+...
		     'double *, int *);',70);
	' ';
        cformatline('int '+rdnom+'ddoit2(double *, double *,'+...
		     'double *, int *);',70);
	' ';
	cformatline('int '+rdnom+'edoit1(double *, double *,'+...
		     'int *, int *, int *);',70);	
	' ';
        cformatline('int '+rdnom+'_initi(double *, double *, int *);',70);
	' ';
	cformatline('int '+rdnom+'_endi(double *, double *, int *);',70);
	' ';
	cformatline('int '+rdnom+'_outtb(double *, double *, int *);',70);
	' ';
	cformatline('int '+rdnom+'_putevs(double *, int *, int *);',70);
	''
	cformatline('void '+rdnom+'_events(int *nevprt, double *t);',70)
	''
	'static void set_nevprt(int nevprt);']
endfunction
function Code=make_end()
//Copyright INRIA
//Author : Rachid Djenidi
  Code=['/*'+part('-',ones(1,40))+' end */ ';
	'void '
	cformatline(rdnom+'_end(scicos_block *block_'+rdnom+',double *z, double *t)',70);
	'{'];
  
  Code=[Code
	cformatline('  '+rdnom+'_endi(t, '+..
		    '&(z['+string(size(z,1))+']), '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1))+'));',70);	
	            '} '];
  
endfunction

function Code=make_init()
//Copyright INRIA
//Author : Rachid Djenidi
  
  Code=['/*'+part('-',ones(1,40))+' init */ ';
	'void '
	cformatline(rdnom+'_init(scicos_block *block_'+rdnom+',double *z, double *t)',70);
	'{'];
  
  Code=[Code;
	'  /*Block initializations*/'
	cformatline('  '+rdnom+'_initi(t, '+..
		    '&(z['+string(size(z,1))+']), '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1))+'));',70);
	'  /*Constants propagation*/'
	cformatline('  '+rdnom+'_outtb(t, '+..
		    '&(z['+string(size(z,1))+']), '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1))+'));',70);
       '} '];
  
endfunction

function Code=make_main1()
//generate code for flag=1 case  
//Copyright INRIA
//Author : Rachid Djenidi
  Code=['/*'+part('-',ones(1,40))+' main1 */ ';
	'void '
	cformatline(rdnom+'main1(scicos_block *block_'+rdnom+',double *z, double *t)',70);
	'{'];
  Code($+1)=cformatline('    '+rdnom+'ddoit1(z, t, '+..
		    '&(z['+string(size(z,1))+']), '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1))+'));',70);  
  
  Code($+1)='} '
endfunction

function Code=make_main2()
//generate code for flag=2 case
//Copyright INRIA
//Author : Rachid Djenidi
  Code=['/*'+part('-',ones(1,40))+' main2 */ ';
	'void '
	cformatline(rdnom+'main2(scicos_block *block_'+rdnom+',double *z, double *t)',70);
	'{'];
 
  Code=[Code;
	cformatline('  '+rdnom+'ddoit2(z, t, '+..
		    '&(z['+string(size(z,1))+']), '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1))+'));',70);	
	'} ']; 
endfunction

function Code=make_outevents()
//generates skeleton of external world events handling function
  z='0'
  if szclkIN==[] then
    newszclkIN=0;
  else
    newszclkIN=szclkIN;
  end 

  Code=[ '/*'+part('-',ones(1,40))+'  External events handling function */ ';
	 'void '
	 rdnom+'_events(int *nevprt,double *t)';
	 '{'
	 '/*  set next event time and associated events ports  '
	 ' *  nevprt has binary expression b1..b'+string(newszclkIN)+' where bi is a bit '
	 ' *  bi is set to 1 if an activation is received by port i. Note that'
	 ' *  more than one activation can be received simultaneously '
	 ' *  Caution: at least one bi should be equal to one */'
	 ''
	 '    int i,p,b[]={'+strcat(z(ones(1,newszclkIN)),',')+'};'
	 ''
	 '/* this is an example for the activation of events ports */'
	 '    b[0]=1;']
  if newszclkIN>1 then
    for bb=2:newszclkIN
      Code($+1)='    b['+string(bb-1)+']=1;'
    end
  end
  Code=[Code;' '
	'/* definition of the step time  */'
	'    *t = *t + 0.1;'
	' '
	'/* External events handling process */'
	'    *nevprt=0;p=1;'
	'    for (i=0;i<'+string(newszclkIN)+';i++) {'
	'      *nevprt=*nevprt+b[i]*p;'
	'      p=p*2;}'
	'}']
endfunction
function Code=make_sensor(standalone)
// Generating the routine for sensors interfacing
//Copyright INRIA
//Author : Rachid Djenidi
  
Call=['/*'+part('-',ones(1,40))+' Sensor */ ';
      'void ';
      rdnom+'_sensor(flag,nport,nevprt,t,y,ny)']
comments=['     /*'
          '      * To be customized for standalone execution';
	  '      * flag  : specifies the action to be done'
	  '      * nport : specifies the  index of the Super Bloc '
	  '      *         regular input (The input ports are numbered '
	  '      *         from the top to the bottom ) '
	  '      * nevprt: indicates if an activation had been received'
	  '      *         0 = no activation'
	  '      *         1 = activation '
	  '      * t     : the current time value'
	  '      * y     : the vector outputs value'
	  '      * ny    : the output  vector size'
	  '      */']
dcl=['     integer *flag,*nevprt,*nport;'
     '     integer *ny;'
     ''
     '     double  *t, y[];'
     '{'
     '  int k;'];
if standalone then

  a_sensor=['  switch (*flag) {'
	    '  case 1 : /* set the output value */'
	    '    printf(""Require outputs of sensor number %d\n"", *nport);'
	    '    printf(""time is: %f\n"", *t);'
	    '    printf(""size of the sensor output is: %d\n"", *ny);'
	    '    puts(""Please set the sensor output values""); '
	    '    for (k=0;k<*ny;k++) scanf(""%lf"", &(y[k]));'
	    '    break;'
	    '  case 4 : /* sensor initialisation */'
	    '    /* do whatever you want to initialize the sensor */'
	    '    break;'
	    '  case 5 : /* sensor ending */'
	    '    /* do whatever you want to end the sensor */'
	    '    break;'
	    '  }']
else
  a_sensor=[]
end
nc=size(cap,'*')
Code=[]
if nc==1|~standalone then
  Code=[Code;
	Call
	comments
	dcl
	a_sensor
	'}'];
elseif nc>1 then
  S='  switch (*nport) {'
  for k=1:nc
    S=[S;
       '  case '+string(k)+' : /* Port number '+string(k)+' ----------*/'
       '  '+a_sensor
       '  break;']
  end
    S=[S;'  }']  
    Code=[Code
	  Call
	  comments
	  dcl
	  S
	  '}']
end
endfunction
function Code=make_standalone()
//generates code of the standalone simulation procedure
//Copyright INRIA
//Author : Rachid Djenidi

txt1=mgetl(SCI+'/routines/machine.h');
mputl(txt1,rpat+'/machine.h');
txt2=mgetl(SCI+'/routines/scicos/scicos_block.h');
mputl(txt2,rpat+'/scicos_block.h');

modptr=cpr.sim.modptr;
nZ=size(z,'*')+size(outtb,'*')+clkptr($);
nztotal=size(z,1);
//Generates simulation routine for standalone simulation
  iwa=zeros(clkptr($),1),Z=[z;outtb;iwa]';
  Code=[ '/*Main program */'
	 'int main()'
	 '{'
	 '   double tf=10.0;';
	 '   '+rdnom+'_sim(tf);';
	 '   return 0;'
	 '}'
	 ''
	 '/*'+part('-',ones(1,40))+'  External simulation function */ ';
	 'int '
	 rdnom+'_sim(tf)';
	 ' '
	 '     double tf; ';
	 '{'
	 '  double t;'
	 '  int nevprt=1;'
	 ''
	 '  /*Initial values */';
	 cformatline('  double z[]={'+strcat(string(Z),',')+'};',70);
	 '/* Note that z[]=[z_initial_condition;outtb;iwa]';
	 cformatline('z_initial_condition= {'+strcat(string(z),",")+'};',70);
	 cformatline('outtb= {'+strcat(string(outtb),"," )+'};',70);
	 cformatline('iwa= {'+strcat(string(iwa),"," )+'};',70);
	 '*/ ']
  Code=[Code
	'double* block_outtb = z+'+string(nztotal)+';'
	'void **work;'
	'work = z+'+string(nZ)+'; ']
  if size(z,1) <> 0 then
    for i=1:(length(zptr)-1) 
      
      if zptr(i+1)-zptr(i)>0 then
	if size(corinv(i),'*')==1 then
	  OO=scs_m.objs(corinv(i));
	else
	  path=list('objs');
	  for l=cpr.corinv(i)(1:$-1),path($+1)=l;path($+1)='model';path($+1)='rpar'; path($+1)='objs';end
	 path($+1)=cpr.corinv(i)($);
	  OO=scs_m(path);
	end
	aaa=OO.gui;bbb=emptystr(3,1);
	if and(aaa+bbb~= ['INPUTPORTEVTS';'OUTPUTPORTEVTS';'EVTGEN_f']) then
	  Code($+1)=' ';	
	  Code($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
	  Code($+1)=' Gui name of block: '+strcat(string(OO.gui));
	  //Code($+1)='/* Name block: '+strcat(string(cpr.sim.funs(i)));
	//Code($+1)='Object number in diagram: '+strcat(string(cpr.corinv(i)));
	Code($+1)='Compiled structure index: '+strcat(string(i));
	if stripblanks(OO.model.label)~=emptystr() then	    
	  Code=[Code;cformatline('Label: '+strcat(string(OO.model.label)),70)];
	end
	if stripblanks(OO.graphics.exprs(1))~=emptystr() then
	  Code=[Code;cformatline('Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
	end
	if stripblanks(OO.graphics.id)~=emptystr() then
	  Code=[Code;
		cformatline('Identification: '+..
			    strcat(string(OO.graphics.id)),70)];
	  
	end
	Code=[Code;cformatline('z= {'+strcat(string(z(zptr(i):zptr(i+1)-1)),",")+'};',70)];
	Code($+1)='*/';
	end
      end
    end
  end

for kf=1:nblk        
      nin=inpptr(kf+1)-inpptr(kf); ///* number of input ports */
      nout=outptr(kf+1)-outptr(kf); ///* number of output ports */
      Code=[Code;
	    '    block_'+rdnom+'['+string(kf-1)+'].type = '+string(funtyp(kf))+';';
            '    block_'+rdnom+'['+string(kf-1)+'].ztyp = '+string(ztyp(kf))+';';
            '    block_'+rdnom+'['+string(kf-1)+'].ng = '+string(zcptr(kf+1)-zcptr(kf))+';';
            '    block_'+rdnom+'['+string(kf-1)+'].nz = '+string(zptr(kf+1)-zptr(kf))+';';
            '    block_'+rdnom+'['+string(kf-1)+'].nrpar = '+string(rpptr(kf+1)-rpptr(kf))+';';
            '    block_'+rdnom+'['+string(kf-1)+'].nipar = '+string(ipptr(kf+1)-ipptr(kf))+';'
            '    block_'+rdnom+'['+string(kf-1)+'].nin = '+string(inpptr(kf+1)-inpptr(kf))+';';
            '    block_'+rdnom+'['+string(kf-1)+'].nout = '+string(outptr(kf+1)-outptr(kf))+';';
            '    block_'+rdnom+'['+string(kf-1)+'].nevout = '+string(clkptr(kf+1)-clkptr(kf))+';';
            '    block_'+rdnom+'['+string(kf-1)+'].nmode = '+string(modptr(kf+1)-modptr(kf))+';';
	    '    if ((block_'+rdnom+'['+string(kf-1)+'].insz=malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL ) return 0;';
	    '    if ((block_'+rdnom+'['+string(kf-1)+'].inptr=malloc(sizeof(double*)*block_'+rdnom+'['+string(kf-1)+'].nin))== NULL ) return 0;';
	    '    if ((block_'+rdnom+'['+string(kf-1)+'].evout=calloc(block_'+rdnom+'['+string(kf-1)+'].nevout,sizeof(double)))== NULL )return 0;'];
    
      for k=1:nin
         lprt=inplnk(inpptr(kf)-1+k);
         Code=[Code
               '    block_'+rdnom+'['+string(kf-1)+'].inptr['+string(k-1)+'] = &(block_outtb['+string(lnkptr(lprt)-1)+']);'
	       '    block_'+rdnom+'['+string(kf-1)+'].insz['+string(k-1)+'] = '+string(lnkptr(lprt+1)-lnkptr(lprt))+';'];
      end 
      Code=[Code
            '    if ((block_'+rdnom+'['+string(kf-1)+'].outsz=malloc(sizeof(int)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL ) return 0;';
            '    if ((block_'+rdnom+'['+string(kf-1)+'].outptr=malloc(sizeof(double*)*block_'+rdnom+'['+string(kf-1)+'].nout))== NULL ) return 0;'];
          
      for k=1:nout
        lprt=outlnk(outptr(kf)-1+k);
        Code=[Code
	      '    block_'+rdnom+'['+string(kf-1)+'].outptr['+string(k-1)+']=&(block_outtb['+string(lnkptr(lprt)-1)+']);'
	      '    block_'+rdnom+'['+string(kf-1)+'].outsz['+string(k-1)+']='+string(lnkptr(lprt+1)-lnkptr(lprt))+';'];
      end

      Code=[Code
	    '    block_'+rdnom+'['+string(kf-1)+'].z=&(z['+string(zptr(kf)-1)+']);'
	    ]
      if ~(part(cpr.sim.funs(kf),1:7) == 'capteur' |part(cpr.sim.funs(kf),1:10) == 'actionneur' |cpr.sim.funs(kf)=='bidon') then
	Code=[Code
	    '    block_'+rdnom+'['+string(kf-1)+'].rpar=&(RPAR1['+string(rpptr(kf)-1)+']);';
            '    block_'+rdnom+'['+string(kf-1)+'].ipar=&(IPAR1['+string(ipptr(kf)-1)+']);';
	    '    block_'+rdnom+'['+string(kf-1)+'].work=(void **)(((double *)work)+'+string(kf-1)+');']
      end
end
    

  Code=[Code;'  t=0.0;'
	'  '+rdnom+'_init(block_'+rdnom+',z,&t);'
	'  while (t<=tf) {   ';
	'    sci_time=t;'
	'    '+rdnom+'_events(&nevprt,&t);'
	'    set_nevprt(nevprt);'
	'    '+rdnom+'main1(block_'+rdnom+',z,&t);'
	'    '+rdnom+'main2(block_'+rdnom+',z,&t);'
	'  }'
	'  '+rdnom+'_end(block_'+rdnom+',z,&t);'
	'  return 0;'
	'}'  
	'/*'+part('-',ones(1,40))+'  Lapack messag function */ ';
	'void'
	'C2F(xerbla)(SRNAME,INFO,L)'
	'char *SRNAME;'
	'int *INFO;'
	'long int L;'
	'{'
	'printf(""** On entry to %s, parameter number %d  had an illegal value\n"",SRNAME,*INFO);'
	'}']
//duplication de main1 et main2 pour le standalone
  Code=[Code
	make_main1();
	make_main2();
	make_init();
	make_end()
	c_make_doit1();
	c_make_doit2();
	c_make_outtb();
	c_make_initi();
	c_make_endi();
	make_putevs()]
  Code=[Code
	'void set_block_error(int err)'
	'{'
  	'  return;'
	'}'
	'int get_phase_simulation()'
	'{'
	'  return 1;'
	'}'
	'void * scicos_malloc(size_t size)'
	'{'
  	'  return malloc(size);'
	'}'
	'void scicos_free(void *p)'
	'{'
	'  free(p);'
	'}'
	'double get_scicos_time()'
	'{'
  	'  return sci_time;'
	'}'
	'void do_cold_restart()'
	'{'
  	'  return;'
	'}'
	'void sciprint (char *fmt){'
        '  return;'
	'}']

endfunction
function Code=cg_sa_static()

  //generates  static table definitions
  //Copyright INRIA
  
  tevts=cpr.state.tevts;evtspt=cpr.state.evtspt;
  zptr=cpr.sim.zptr;clkptr=cpr.sim.clkptr;
  ordptr=cpr.sim.ordptr;pointi=cpr.state.pointi;
  funs=cpr.sim.funs;

  Code= ['/* Table of constant values */ ';
	 'static  integer nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';';
	 ' ';
	 'static integer totalnevprt; '];
  
  if size(evtspt,1) <> 0 then
    Code=[Code;cformatline('static integer evtspt[ ]={'+..
			   strcat(string(evtspt),",")+'};',70)] 
  else
    Code($+1)='static integer evtspt[1];';
  end
  
  Code($+1)='static double x[1];';
    
  if size(clkptr,1) <> 0 then
    Code=[Code;cformatline('static integer clkptr[ ]={'+..
			   strcat(string(clkptr),",")+'};',70)] 
  else
    Code($+1)='static integer clkptr[1];';
  end

  if size(ordptr,1) <> 0 then
    Code=[Code;cformatline('static integer ordptr[ ]={'+..
			   strcat(string(ordptr),",")+'};',70)] 
  else
    Code($+1)='static integer ordptr[1];';
  end  

  
  if size(tevts,1) <> 0 then
    Code=[Code;cformatline('static double tevts[ ] = {'+..
			   strcat(string(tevts),",")+'};',70)]     
  else
    Code($+1)='static double tevts[1];';
  end

  if size(pointi,1) <> 0 then
    Code($+1)='static integer pointi={'+strcat(string(pointi),",")+'};'; 
  else
    Code($+1)='static integer pointi=1;';
  end
  Code($+1)='static double w[1];'
  Code=[Code;
	'static void set_nevprt(int nevprt)'
	'{'
	'  totalnevprt=nevprt;'
	'}']
endfunction
function Code=make_static()
  Code=cg_sa_static();
  Code($+1)='scicos_block *block_'+rdnom+';'   
endfunction

function Code=make_static_standalone()
//generates  static table definitions
//Copyright INRIA
//Author : Rachid Djenidi
  rpptr=cpr.sim.rpptr;ipptr=cpr.sim.ipptr;
  rpar=cpr.sim.rpar;ipar=cpr.sim.ipar;
  Code=cg_sa_static();
  Code=[Code;	
	'static double sci_time;'
	'scicos_block block_'+rdnom+'['+string(nblk)+'];'];
  if size(rpar,1) <> 0 then
    Code=[Code;
	'static double RPAR1[ ] = {'];		
    
    for i=1:(length(rpptr)-1) 
      if rpptr(i+1)-rpptr(i)>0  then
	
	if size(corinv(i),'*')==1 then
	  OO=scs_m.objs(corinv(i));
	else
	  path=list('objs');
	  for l=cpr.corinv(i)(1:$-1),path($+1)=l;path($+1)='model';path($+1)='rpar';path($+1)='objs';end
	  path($+1)=cpr.corinv(i)($);
	  OO=scs_m(path);
	end
	aaa=OO.gui;bbb=emptystr(3,1);
	// the following is a useless test because these block do not
        // have rpar
	if and(aaa+bbb~= ['INPUTPORTEVTS'; ...
			  'OUTPUTPORTEVTS';'EVTGEN_f']) then
	  
	  Code($+1)=' ';
	  Code($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
	  Code($+1)='   Gui name of block: '+strcat(string(OO.gui));
	  if stripblanks(OO.model.label)~=emptystr() then	    
	    Code=[Code;cformatline('Label: '+strcat(string(OO.model.label)),70)];
	  end
	  if stripblanks(OO.graphics.exprs(1))~=emptystr() then
	    Code=[Code;cformatline('Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
	  end
	  if stripblanks(OO.graphics.id)~=emptystr() then
	    Code=[Code;
		  cformatline('Identification: '+strcat(string(OO.graphics.id)),70)];
	    
	  end
	  Code=[Code;'rpar= '];
	  Code($+1)='*/';
	  Code=[Code;cformatline(strcat(string(rpar(rpptr(i):rpptr(i+1)-1))+','),70)];
	end
      end
    end
    Code($+1)='};';
    //    strcat(string(rpar),",")+'};',70)] 
  else
    Code($+1)='static double RPAR1[1];';
  end

  if size(ipar,1) <> 0 then
    
    //Code=[Code;'static integer IPAR1[ ] = {'];
    Code_TMP=[]
    
    for i=1:(length(ipptr)-1) 
      
      if ipptr(i+1)-ipptr(i)>0  then
	
	if size(corinv(i),'*')==1 then
	  OO=scs_m.objs(corinv(i));
	else
	  path=list('objs');
	  for l=cpr.corinv(i)(1:$-1),path($+1)=l;path($+1)='model';path($+1)='rpar';path($+1)='objs';end
	  path($+1)=cpr.corinv(i)($);
	  OO=scs_m(path);
	end
	aaa=OO.gui;bbb=emptystr(3,1);
	if and(aaa+bbb~= ['INPUTPORTEVTS';'OUTPUTPORTEVTS';'EVTGEN_f']) then
	  Code_TMP($+1)=' ';
	  Code_TMP($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
	  Code_TMP($+1)=' Gui name of block: '+strcat(string(OO.gui));
	  // Code($+1)='/* Name block: '+strcat(string(cpr.sim.funs(i)));
	  // Code($+1)='Object number in diagram: '+strcat(string(cpr.corinv(i)));
	  Code_TMP($+1)='Compiled structure index: '+strcat(string(i));
	  if stripblanks(OO.model.label)~=emptystr() then	    
	    Code_TMP=[Code_TMP;cformatline('Label: '+strcat(string(OO.model.label)),70)];
	  end

	  if stripblanks(OO.graphics.exprs(1))~=emptystr() then
            Code_TMP=[Code_TMP;cformatline('Exprs: '+strcat(OO.graphics.exprs(1),","),70)];
	  end
	  if stripblanks(OO.graphics.id)~=emptystr() then
	    Code_TMP=[Code_TMP;
		  cformatline('Identification: '+strcat(string(OO.graphics.id)),70)];
	    
	  end
	  Code_TMP=[Code_TMP;cformatline('ipar= {'+strcat(string(ipar(ipptr(i):ipptr(i+1)-1)),",")+'};',70)];
	  Code_TMP($+1)='*/';
	  Code_TMP=[Code_TMP;cformatline(strcat(string(ipar(ipptr(i):ipptr(i+1)-1))+','),70)];
	end
      end
    end
    if  Code_TMP<>[] then
      Code=[Code;'static integer IPAR1[ ] = {';Code_TMP;'};']
    else
      Code($+1)='static integer IPAR1[1];';
    end
    //    Code($+1)='};';
  else
    Code($+1)='static integer IPAR1[1];';
  end
  
endfunction

function ok=mkdir(path)
  if MSDOS then
    cmd='mkdir '+strsubst(path,'/','\')
  else
    cmd='mkdir '+strsubst(path,'\','/')
  end
  ok=0==execstr('unix_s(cmd)','errcatch')
endfunction

function t=filetype(m)
  m=int32(m)
  filetypes=['Directory','Character device','Block device',...
             'Regular file','FIFO','Symbolic link','Socket']
  bits=[16384,8192,24576,32768,4096,40960,49152]
  m=int32(m)&int32(61440)
  t=filetypes(find(m==int32(bits)))
endfunction
  
function Code=make_putevs()
  //RN
  Code=['int '+rdnom+'_putevs(t, evtnb, ierr)';
	'double *t;'
	'integer *evtnb, *ierr;'
	'{'
	'*ierr = 0;'
	'if (evtspt[*evtnb-1] != -1) {'
	'*ierr = 1;'
	'return 0;'
	'} else {'
	'evtspt[*evtnb-1] = 0;'
	'tevts[*evtnb-1] = *t;'
	'}'
	'if (pointi == 0) {'
	'pointi = *evtnb;'
	'return 0;'
	'}'
	'evtspt[*evtnb-1] = pointi;'
	'pointi = *evtnb;'
	'return 0;'
	'} ']
endfunction
