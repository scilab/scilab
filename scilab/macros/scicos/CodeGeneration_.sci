function  CodeGeneration_()
  Cmenu='Open/Set'
  xinfo('Click on a Superblock (without activation output)'+..
	' to obtain a coded block ! ')
  k=[]
  while %t 
    [btn,xc,yc,win,Cmenu]=cosclick()

    if Cmenu<>[] then
      %pt=[];break
    elseif btn>31 then
      Cmenu=%tableau(min(100,btn-31));%pt=[xc;yc];%win=win
      if Cmenu==emptystr() then 
	Cmenu=[];%pt=[];
      end 
      break
    end
    k=getobj(scs_m,[xc;yc])
    if k<>[] then
      if scs_m.objs(k).model.sim(1)=='super' then
	disablemenus()
	XX=scs_m.objs(k);
	[ok,XX]=do_compile_superblock(XX)
	enablemenus()
	if ok then 
	  scs_m.objs(k)=XX
	  edited=%t;
	  needcompile=4
	  Cmenu='Replot';
	  break
	end     
      else
	message('Generation Code only work for a Superblock ! ')
	break
      end
    end
    
  end
endfunction

function [ok,Makename]=buildnewblock() 
// compiles the generated C code and link it with Scilab
//Copyright INRIA
//Author : Rachid Djenidi
  files=[rdnom rdnom+'_void_io']
  [fd,ierr]=mopen(rpat+'/'+rdnom+'f.f','r')
  if ierr==0 then mclose(fd),files=[files,rdnom+'f'],end
    Makename=gen_make(rdnom,files,archname,rpat+'/'+rdnom+'_Makefile')
    //unlink if necessary
    [a,b]=c_link(rdnom); while a ; ulink(b);[a,b]=c_link(rdnom);end
    libn=ilib_compile('lib'+rdnom,Makename)

    
    ierr=execstr('link(libn,rdnom,''c'')','errcatch')
    if ierr<>0 then 
      ok=%f;x_message(['sorry link problem';lasterror()]);
      return;
    end

endfunction

function [ok,Makename]=buildstandalone() 
// compiles the generated C code and link it with Scilab
//Copyright INRIA
//Author : Rachid Djenidi
  files=[rdnom rdnom+'_standalone']
  [fd,ierr]=mopen(rpat+'/'+rdnom+'f.f','r')
  if ierr==0 then mclose(fd),files=[files,rdnom+'f'],end
  
  Makename=gen_make(rdnom,files,archname,rpat+'/'+rdnom+'_Makefile')
  //unlink if necessary
  [a,b]=c_link(rdnom); while a ; ulink(b);[a,b]=c_link(rdnom);end
  libn=ilib_compile('lib'+rdnom,Makename)
  
    
  
  ierr=execstr('link(libn,rdnom,''c'')','errcatch')
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
  z=cpr.state.z;tevts=cpr.state.tevts;evtspt=cpr.state.evtspt;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  zptr=cpr.sim.zptr;inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  ordptr=cpr.sim.ordptr;ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;evtspt=cpr.state.evtspt;
  sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;
  nZ=size(z,'*')+size(outtb,'*')+nblk 
  Code=['';
	 '/*'+part('-',ones(1,40))+' ddoit1.c */ ';
	 'int '
	 cformatline(rdnom+'ddoit1( z, zptr, told, tevts, evtspt, nevts, '+..
		     'pointi, outptr, clkptr, ordptr, ordclk, nordcl, rpar, '+..
		     'ipar, funptr, funtyp, outtb, iwa)',70); 
	 '';
	 '     double  *z; ';
	 '     integer *zptr; ';
	 '     double  *told,  *tevts; ';
	 ''
	 '     integer *evtspt, *nevts, *pointi, *outptr; ';
	 '     integer *clkptr, *ordptr, *ordclk, *nordcl; ';
	 '     double  *rpar, *outtb; '; 
	 '     integer *ipar, *funptr, *funtyp; '; 
	 '     integer *iwa; '; 
	 '{'; 
	 '  /* System generated locals */ '; 
	 '  integer ordclk_dim1, ordclk_offset, i2; '; 
	 ' '; 
	 '  /* Local variables */ '; 
	 '  integer flag, keve, kiwa, nport; ';
	 '  double  tvec['+string(sztvec)+']; '; 
	 '  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	 '  double  *args[100]; '; 
	 '  integer sz[100]; ';
	 '  integer ierr1; '; 
	 '  integer nevprt; '; 
	 '  integer urg; '; 
	 ' '; 
	 '  /* Generated constants */'
	 '  integer nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';';
	 '  /* Parameter adjustments */ '; 
	 '  --z; '; 
	 '  --zptr; '; 
	 '  --tevts; '; 
	 '  --evtspt; '; 
	 '  --outptr; '; 
	 '  --clkptr; '; 
	 '  --ordptr; '; 
	 ''
	 '  ordclk_dim1 = *nordcl; '; 
	 '  ordclk_offset = ordclk_dim1 + 1; '; 
	 '  ordclk -= ordclk_offset; '; 
	 '  --rpar; '; 
	 '  --ipar; '; 
	 '  --funptr; '; 
	 '  --funtyp; ';  
	 '  --iwa; '; 
	 '  --outtb; ';
	 ' '; 
	 '  /* Function Body */ '; 
	 '  kiwa = 0; ';
	 '  urg = 1; '];
 //////////////////////////////////////////////////
  if  szclkIN>=1 then
    pointi=clkptr(howclk)-1;
    Code($+1)='  *pointi='+string(pointi)+'+ totalnevprt;';
  else
    pointi=clkptr(howclk)
    Code($+1)='  *pointi='+string(pointi)+';';
  end
  Code=[Code;
	'  tevts[*pointi]=*told;'];


  Code=[Code;	
	 '  if (urg > 0) {';
	 ' L43:';
	cformatline('    '+rdnom+'edoit1(&z[1], &zptr[1], '+..
		    'told, &tevts[1], &evtspt[1], nevts, pointi, '+..
		    '&outptr[1], &clkptr[1], &ordptr[1],  '+..
		    '&ordclk[ordclk_offset], nordcl, &rpar[1], '+..
		    '&ipar[1], &funptr[1], &funtyp[1], '+..
		    '&(z['+string(size(z,1)+1)+']), &urg, '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1)+1)+..
		    '),&kiwa);',70);
	'    if (urg > 0) {';
	'      goto L43;';
	'    }';
	'  }';
	'  iwa['+string(clkptr($))+']=kiwa;';
	'  return 0;'
	' ';
	'} /* ddoit1 */';
	'  '];
endfunction

function Code=make_edoit1()
    ///////////////////
  //les pointeurs de cpr :
  z=cpr.state.z;tevts=cpr.state.tevts;evtspt=cpr.state.evtspt;
  pointi=cpr.state.pointi;

  funs=cpr.sim.funs;
  zptr=cpr.sim.zptr;inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  ordptr=cpr.sim.ordptr;ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;;evtspt=cpr.state.evtspt;
  sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;
  nZ=size(z,'*')+size(outtb,'*')+nblk 
  //////////////////////////////////////////////////
  maxkeve=size(evtspt,1);
  
  Code=[ '/*'+part('-',ones(1,40))+' edoit1.c */ ';
	 'int '
	 cformatline(rdnom+'edoit1( z, zptr, told,tevts, evtspt, nevts, '+..
		    'pointi,  outptr, clkptr, ordptr, ordclk, nordcl, '+..
		    'rpar, ipar, funptr, funtyp, outtb, urg, iwa, kiwa)',70)
	'     double  *z; ';
	'     integer *zptr; ';
	'     double  *told,  *tevts; ';
	'     integer *evtspt, *nevts, *pointi, *outptr; ';
	'     integer *clkptr, *ordptr, *ordclk, *nordcl; ';
	'     double  *rpar, *outtb; ';
	'     integer *ipar,  *funptr, *funtyp; ';
	'     integer *urg,  *iwa, *kiwa; ';
	'{ ';
	'  /* System generated locals */ ';
	'  integer ordclk_dim1, ordclk_offset, i2; ';
	' ';
	'  /* Local variables */ ';
	'  integer flag, keve, nport; ';
	'  integer nord; ';
	'  double  tvec['+string(sztvec)+']; '; 
	'  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	'  double  *args[100]; '; 
	'  integer sz[100]; ';
	'  integer ierr1, i, nx=0; ';
	'  integer ntvec, ntvecm, nevprt; ';
	' ';
	'  /* Generated constants */'
	'  integer nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';';
	'  /* Parameter adjustments */ ';
	'  --z; ';
	'  --zptr; ';
	'  --tevts; ';
	'  --evtspt; ';
	'  --outptr; ';
	'  --clkptr; ';
	'  --ordptr; ';
	'  ordclk_dim1 = *nordcl; ';
	'  ordclk_offset = ordclk_dim1 + 1; ';
	'  ordclk -= ordclk_offset; ';
	'  --rpar; ';
	'  --ipar; ';
	'  --funptr; ';
	'  --funtyp; ';
	'  --iwa; ';
	'  --outtb; ';
	' ';
	'  /* Function Body */ ';
	'  --(*urg); '
	'  keve = *pointi; ';
	'  *pointi = evtspt[keve]; ';
	'  evtspt[keve] = -1; ';
	' ';
	'  nord = ordptr[keve + 1] - ordptr[keve]; ';
	'  if (nord == 0) { ';
	'    return 0; ';
	'  } ';
	'  ++(*kiwa);   ';
	'  iwa[*kiwa] = keve;   ';
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
		'    if (outtb['+string(nxx)+']>0) {';
		'      ntvecm=1;';
		'    }';
		'    else {';
		'      ntvecm=2;';
		'    }']
	elseif funtyp(fun)==-2 then
	  Code=[Code;
		'    ntvecm=(integer)outtb['+string(nxx)+'];';
		'    if(ntvecm>'+string(ntvec)+'){ntvecm='+string(ntvec)+';}';
		'    if(ntvecm<1){ntvecm=1;}']
	else
	  message('Block '+fun+' has funtyp '+string(funtyp(fun))+': not allowed')
	end
	
	Code=[Code;
	      '     ++(*urg);';
	      '     i2 = ntvecm + clkptr['+string(fun)+'] - 1;';
	      '     C2F(putevs)(&tevts[1], &evtspt[1],nevts,pointi, told, &i2, &ierr1); ';
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
  z=cpr.state.z;tevts=cpr.state.tevts;evtspt=cpr.state.evtspt;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  zptr=cpr.sim.zptr;inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  ordptr=cpr.sim.ordptr;ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;evtspt=cpr.state.evtspt;
  sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;
  nZ=size(z,'*')+size(outtb,'*')+nblk 
  Code=['';
	'/*'+part('-',ones(1,40))+' ddoit2 */ ';
	'int '
	cformatline(rdnom+'ddoit2( z, zptr, told, tevts, evtspt, nevts, '+..
		    'pointi, outptr, clkptr, ordptr, ordclk, nordcl, rpar,  '+..
		    'ipar, funptr, funtyp, outtb, iwa)',70);  
	''; 
	'     double  *z; '; 
	'     integer *zptr; '; 
	'     double  *told,  *tevts; '; 
	'     integer *evtspt, *nevts, *pointi, *outptr; '; 
	'     integer *clkptr, *ordptr, *ordclk, *nordcl; '; 
	'     double  *rpar, *outtb; '; 
	'     integer *ipar, *funptr, *funtyp; '; 
	'     integer *iwa; '; 
	'{ '; 
	'  /* System generated locals */ '; 
	'  integer  i1, i; '; 
	' '; 
	'  /* Local variables */ '; 
	'  integer flag, keve, kiwa, nport; '; 
	'  double  tvec['+string(sztvec)+']; '; 
	'  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	'  double  *args[100]; '; 
	'  integer sz[100]; ';
	'  integer ntvec; '; 
	'  integer nevprt; '; 
	' '; 
	'  /* Generated constants */'
	'  integer nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';';	
	'    /* Parameter adjustments */ '; 
	'  --z; '; 
	'  --zptr; '; 
	'  --tevts; '; 
	'  --evtspt; '; 
	'  --outptr; '; 
	'  --clkptr; '; 
	'  --ordptr; '; 
	'  --rpar; '; 
	'  --ipar; '; 
	'  --funptr; '; 
	'  --funtyp; '; 
	'  --iwa; '; 
	'  --outtb; ';
	' '; 
	'  /* Function Body */ '; 
	' '
	'  /*update continuous and discrete states on event */';
	'  kiwa = iwa['+string(clkptr($))+'];';
	'  if (kiwa == 0) {';
	'    return 0 ;';
	'  }';
	'  i1 = kiwa;';
	'  for (i = 1; i <= i1; ++i) {';
	'    keve = iwa[i];';
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
      if (zptr(i+1)-zptr(i))>0|is_act  then
	//  Code($+1)='/*     .     If continuous state jumps, do cold restart */';
	Code=[Code
	      '    flag = 2;';
	      '    nevprt='+string(ordclk(ii,2))+';';
	      '   '+wfunclist(i);];

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
z=cpr.state.z;tevts=cpr.state.tevts;evtspt=cpr.state.evtspt;
pointi=cpr.state.pointi;


funs=cpr.sim.funs;
zptr=cpr.sim.zptr;inpptr=cpr.sim.inpptr;
outptr=cpr.sim.outptr;
funtyp=cpr.sim.funtyp;
ordptr=cpr.sim.ordptr;ordclk=cpr.sim.ordclk;
clkptr=cpr.sim.clkptr;evtspt=cpr.state.evtspt;
sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;

Code=[	'/*'+part('-',ones(1,40))+' endi */ '; 
	'/* file_end.c */ ';
	'/* Subroutine */ int ';
	rdnom+'_endi( z, zptr, told, ';
	'    tevts, evtspt, nevts, pointi, outptr, '; 
	'    clkptr, ordptr, ordclk, nordcl,  '; 
	'    rpar, ipar, funptr, funtyp, outtb, iwa) ';  
	''; 
	'     double  *z; '; 
	'     integer *zptr; '; 
	'     double  *told,  *tevts; '; 
	'     integer *evtspt, *nevts, *pointi, *outptr; '; 
	'     integer *clkptr, *ordptr, *ordclk, *nordcl; '; 
	'     double  *rpar, *outtb; '; 
	'     integer *ipar, *funptr, *funtyp; '; 
	'     integer *iwa; '; 
	'{ '; 
	'  /* Local variables */ '; 
	'  integer flag; '; 
	'  double  tvec['+string(sztvec)+']; '; 
	'  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	'  double  *args[100]; '; 
	'  integer sz[100]; ';
	'  integer nport; '; 
	'  integer ntvec; '; 
	'  integer nevprt=0; '; 
	' '; 
	'  /* Generated constants */'
	'  integer nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';';
	'  /* Parameter adjustments */ '; 
	'  --z; '; 
	'  --zptr; '; 
	'  --tevts; '; 
	'  --evtspt; '; 
	'  --outptr; '; 
	'  --clkptr; '; 
	'  --ordptr; '; 
	'  --rpar; '; 
	'  --ipar; '; 
	'  --funptr; '; 
	'  --funtyp; '; 
	'  --iwa; '; 
	'  --outtb; ';
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
  z=cpr.state.z;tevts=cpr.state.tevts;evtspt=cpr.state.evtspt;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  zptr=cpr.sim.zptr;inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  ordptr=cpr.sim.ordptr;ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;evtspt=cpr.state.evtspt;
  sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;

  Code=['/*'+part('-',ones(1,40))+' initi */ ';
	'int '
	rdnom+'_initi( z, zptr, told, tevts, evtspt, nevts, pointi, outptr,'; 
	'	clkptr, ordptr, ordclk, nordcl, rpar, ipar, funptr, funtyp, '
	'    outtb, iwa) ';  
	''; 
	'     /*Block initialization (flag=4)*/'
	'     double  *z; '; 
	'     integer *zptr; '; 
	'     double  *told,  *tevts; '; 
	'     integer *evtspt, *nevts, *pointi, *outptr; '; 
	'     integer *clkptr, *ordptr, *ordclk, *nordcl; '; 
	'     double  *rpar, *outtb; '; 
	'     integer *ipar, *funptr, *funtyp; '; 
	'     integer *iwa; '; 
	'{ '; 
	' '; 
	'  /* Local variables */ '; 
	'  integer flag; '; 
	'  double  tvec['+string(sztvec)+']; '; 
	'  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	'  double  *args[100]; '; 
	'  integer sz[100]; ';
	'  integer nport; '; 
	'  integer ntvec; '; 
	'  integer nevprt=0; '; 
	' '; 
	'  /* Generated constants */'
	'  integer nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';';
	'  /* Parameter adjustments */ '; 
	'  --z; '; 
	'  --zptr; '; 
	'  --tevts; '; 
	'  --evtspt; '; 
	'  --outptr; '; 
	'  --clkptr; '; 
	'  --ordptr; '; 
	'  --rpar; '; 
	'  --ipar; '; 
	'  --funptr; '; 
	'  --funtyp; '; 
	'  --iwa; '; 
	'  --outtb; ';
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
  z=cpr.state.z;tevts=cpr.state.tevts;evtspt=cpr.state.evtspt;
  pointi=cpr.state.pointi;
  funs=cpr.sim.funs;
  zptr=cpr.sim.zptr;inpptr=cpr.sim.inpptr;
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  ordptr=cpr.sim.ordptr;ordclk=cpr.sim.ordclk;
  clkptr=cpr.sim.clkptr;evtspt=cpr.state.evtspt;
  iord=cpr.sim.iord;niord=size(iord,1);
  sztvec=max(clkptr(2:$)-clkptr(1:$-1))+1;

  Code=['/*'+part('-',ones(1,40))+' outtbini */ ';
	'int '
	rdnom+'_outtb( z, zptr, told, tevts, evtspt, nevts, pointi, outptr,'; 
	'	clkptr, ordptr, ordclk, nordcl, rpar, ipar, funptr, funtyp, '
	'    outtb, iwa) ';  
	''; 
	'     /*Constants propagation*/'
	'     double  *z; '; 
	'     integer *zptr; '; 
	'     double  *told,  *tevts; '; 
	'     integer *evtspt, *nevts, *pointi, *outptr; '; 
	'     integer *clkptr, *ordptr, *ordclk, *nordcl; '; 
	'     double  *rpar, *outtb; '; 
	'     integer *ipar, *funptr, *funtyp; '; 
	'     integer *iwa; '; 
	'{ '; 
	' '; 
	'  /* Local variables */ '; 
	'  integer flag; '; 
	'  double  tvec['+string(sztvec)+']; '; 
	'  double  rdouttb['+string(size(outtb,1)+1)+']; ';
	'  double  *args[100]; '; 
	'  integer sz[100]; ';
	'  integer nport; '; 
	'  integer ntvec; '; 
	'  integer nevprt=0; '; 
	' '; 
	'  /* Generated constants */'
	'  integer nrd_'+string(0:maxtotal)'+' = '+string(0:maxtotal)'+';';
	'  /* Parameter adjustments */ '; 
	'  --z; '; 
	'  --zptr; '; 
	'  --tevts; '; 
	'  --evtspt; '; 
	'  --outptr; '; 
	'  --clkptr; '; 
	'  --ordptr; '; 
	'  --rpar; '; 
	'  --ipar; '; 
	'  --funptr; '; 
	'  --funtyp; '; 
	'  --iwa; '; 
	'  --outtb; ';
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
      Code=+'args[0]=&(outtb[1]);';
    else
      lprt=inplnk(inpptr(i));
      uk=lnkptr(lprt);
      nuk=(lnkptr(lprt+1))-uk;
      Code='args[0]=&(outtb['+string(uk)+']);';
    end
  end
  
  //pour la fonction gui ont a : num de bloc l'adresse dans
  //nouveau  z et la taille du port

  actt=[i uk nuk bllst(i).ipar]
  
  Code=[Code;
	 'nport = '+string(nbact)+';';
	 rdnom+'_actuator(&flag, &nport, &nevprt, told, '+..
	 '(double *)args[0], &nrd_'+string(nuk)+');'];
  proto='void '+rdnom+'_actuator(int *flag, int *nport, int *nevprt, double *t,"+...
	" double *u, int *nu);"
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

  //l'adresse du pointeur de z 
  if nz<>0 then z=zptr(i);else z=1;end

  //l'adresse du pointeur de x
  if nx<>0 then x=xptr(i);else x=1;end
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
  if ftyp==0 then
    if nin>1 then
      for k=1:nin
	lprt=inplnk(inpptr(i)-1+k);
	uk=lnkptr(lprt);
	nuk=(lnkptr(lprt+1))-uk;
	Code($+1)='rdouttb['+string(k-1)+']=outtb['+string(uk)+'];';
      end
      Code($+1)='args[0]=&(rdouttb[0]);';
    elseif nin==0
      uk=0;
      nuk=0;
      Code($+1)=+'args[0]=&(outtb[1]);';
    else

      lprt=inplnk(inpptr(i));
      uk=lnkptr(lprt);
      nuk=(lnkptr(lprt+1))-uk;
      Code($+1)='args[0]=&(outtb['+string(uk)+']);';
    end
    if nout>1 then
      for k=1:nout
	lprt=outlnk(outptr(i)-1+k);
	yk=lnkptr(lprt);
	nyk=(lnkptr(lprt+1))-yk;
	Code($+1)='rdouttb['+string(k+nin-1)+']=outtb['+..
	    string(yk)+'];';
      end
      Code($+1)='args[1]=&(rdouttb['+string(nin)+']);';
    elseif nout==0
      yk=0;
      nyk=0;
      Code($+1)=+'args[1]=&(outtb[1]);';
    else
      lprt=outlnk(outptr(i));
      yk=lnkptr(lprt);
      nyk=(lnkptr(lprt+1))-yk;
      Code($+1)='args[1]=&(outtb['+string(yk)+']);';
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
  elseif ftyp==2

    for k=1:nin
      lprt=inplnk(inpptr(i)-1+k);
      uk=lnkptr(lprt)-1;
      nuk=(lnkptr(lprt+1))-uk;
      Code=[Code
	    'args['+string(k-1)+'] = &(outtb['+string(uk+1)+']);';
	    'sz['+string(k-1)+'] = '+string(nuk-1)+';'];
    end
    for k=1:nout
      lprt=outlnk(outptr(i)-1+k);
      yk=lnkptr(lprt)-1;
      nyk=(lnkptr(lprt+1))-yk;
      Code=[Code
	    'args['+string(k+nin-1)+'] = &(outtb['+string(yk+1)+']);';
	    'sz['+string(k+nin-1)+'] = '+string(nyk-1)+';'];
    end
  end
  //************************
  //generate the call itself
  //************************
  if ftyp==0 | ftyp==1 then
    CodeC='C2F(' +fun+')(&flag, &nevprt,told, &(w['+..
	  string(x)+']), &(x['+string(x)+']), &nrd_'+string(nx)+..
	  ', &(z['+string(z)+']), &nrd_'+string(nz)+', '+..
	  'tvec, &ntvec, &(rpar['+string(rpar)+']), &nrd_'+..
	  string(nrpar)+', &(ipar['+..
	  string(ipar)+']), &nrd_'+string(nipar);
    proto='void '+"C2F(" +string(fun)+")(int *flag, int *nevprt, double *told,"+...
	  " double *xd, double *x, int *nx, double *z, int *nz, double *tvec,"+...
	  " int *ntvec, double *rpar, int *nrpar,int *ipar,int *nipar"

    if ftyp==0 then
      CodeC=CodeC+', (double *)args[0], &nrd_'+string(nuk)+..
	    ', (double *)args[1], &nrd_'+string(nyk)+');';
       proto=proto+", double *u, int *nu, double *y, int *ny);"
    elseif ftyp==1
      if nin>=1 then
	for k=1:nin
	  lprt=inplnk(inpptr(i)-1+k);
	  uk=lnkptr(lprt);
	  CodeC=CodeC+', &(outtb['+string(uk)+']), &nrd_'+string(nuk);
	  proto=proto+", double *u"+string(k)+", int *nu"+string(k)
	end
      end
      if nout>=1 then
	for k=1:nout
	  lprt=outlnk(outptr(i)-1+k);
	  yk=lnkptr(lprt);
	  CodeC=CodeC+', &(outtb['+string(yk)+']), &nrd_'+string(nyk);
	  proto=proto+", double *y"+string(k)+", int *ny"+string(k)
	end
      end
      CodeC=CodeC+');';
       proto=proto+');'
    end
  elseif ftyp==2 
    CodeC=fun+'(&flag, &nevprt, told,&(w['+string(x)+..
	 ']), &(x['+string(x)+']), &nrd_'+string(nx);
    CodeC=CodeC+', &(z['+string(z)+']),&nrd_'+string(nz)+..
	 ', tvec, &ntvec, &(rpar['+string(rpar)+']), &nrd_'+..
       string(nrpar)+',&(ipar['+string(ipar)+'])';
    CodeC=CodeC+', &nrd_'+string(nipar)+', &(args[0]), &(sz[0]), &nrd_'+..
       string(nin)+',&(args['+string(nin)+']),&(sz['+string(nin)+..
	 ']),&nrd_'+string(nout)+');';
    
    proto='void '+fun+"(int *flag, int *nevprt,double *told,"+...
	  " double *xd, double *x, int *nx, double *z, int *nz, double *tvec, "+...
	  " int *ntvec, double *rpar, int *nrpar, int *ipar, int *nipar, "+...
	  " double **inptr, int* insz, int *nin, double **outptr,int"+...
	  " *outsz, int *nout);" 
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
      Code=+'args[1]=&(outtb[1]);';
    else
      lprt=outlnk(outptr(i));
      yk=lnkptr(lprt);
      nyk=(lnkptr(lprt+1))-yk;
      Code='args[1]=&(outtb['+string(yk)+']);';
    end

  end
  capt=[i yk nyk bllst(i).ipar]
  Code=[Code;
	 'nport = '+string(nbcap)+';'; 
	 rdnom+'_sensor(&flag, &nport, &nevprt, '+..
	 'told, (double *)args[1], &nrd_'+string(nyk)+');'];
  proto='void '+rdnom+'_sensor(int *flag, int *nport, int *nevprt, double *t,"+...
	" double *y, int *ny);"
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
function  [ok,XX]=do_compile_superblock(XX)
// Transforms a given Scicos discrete SuperBlock into a C defined Block
// Copyright INRIA
//Author : Rachid Djenidi

  scs_m=XX.model.rpar
  par=scs_m.props;
  if alreadyran then 
    //terminate current simulation
    do_terminate()
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
cpr=newc_pass2(bllst,connectmat,clkconnect,cor,corinv)

  if cpr==list() then ok=%f,return, end

  ///////////////////
  //les pointeurs de cpr :
  x=cpr.state.x;z=cpr.state.z;tevts=cpr.state.tevts;
  evtspt=cpr.state.evtspt;
  pointi=cpr.state.pointi;outtb=cpr.state.outtb;
  
  rpptr=cpr.sim.rpptr;
  ipptr=cpr.sim.ipptr;funs=cpr.sim.funs;xptr=cpr.sim.xptr;
  zptr=cpr.sim.zptr;inpptr=cpr.sim.inpptr;inplnk=cpr.sim.inplnk;
  outptr=cpr.sim.outptr;outlnk=cpr.sim.outlnk;
  lnkptr=cpr.sim.lnkptr;
  ordclk=cpr.sim.ordclk;nordcl=size(ordclk,1);
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
    elseif ztyp(i)<>0 then
      msg=[msg;'Zero crossing block''s not allowed']
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
      [Code,protoi]=call_block(i)
      if Code<>[] then
	wfunclist($+1)=[Code;'   if(flag < 0 ) return(5 - flag); ']
      else
	wfunclist($+1)=' ';
      end
      if ki==[] then Protos=[Protos;'';protoi],end      
      dfuns=[dfuns;funs(i)]
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
			 
  if kdyn==[] then
    return
  end
  for i=1:size(kdyn,'*')
    //get the block data structure in the initial scs_m structure
    if size(corinv(kdyn(i)),'*')==1 then
      O=scs_m.objs(corinv(kdyn(i)));
    else
      path=list();
      for l=corinv(kdyn(i))(1:$-1),path($+1)=l;path($+1)='model';path($+1)='rpar';end
      path($+1)=corinv(kdyn(i))($);
      O=scs_m.objs(path);
    end
    if funtyp(kdyn(i))>2000 then
      //C block
      CCode=[CCode;strsubst(O.graphics.exprs(2),"#include <machine.h>","")]
    else
      FCode=[FCode;O.graphics.exprs(2)]
    end
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
	c_make_endi()]
  
	
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
  Code=['#include '"'+SCI+'/routines/machine.h'"';
	make_actuator(%f)
	make_sensor(%f)]
  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_void_io.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end
 
  
  Code=['#include '"'+SCI+'/routines/machine.h'"';
	make_decl_standalone()
	make_static_standalone()
        make_standalone()]
	//make_outevents()
	//make_actuator(%t)
	//make_sensor(%t)
  ierr=execstr('mputl(Code,rpat+''/''+rdnom+''_standalone.c'')','errcatch')
  if ierr<>0 then
    message(lasterror())
    ok=%f
    return
  end
 
  
  Code=['#include '"'+SCI+'/routines/machine.h'"';
	make_decl_standalone()
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
	'  graphics=arg1.graphics;label=graphics.exprs';
	'  model=arg1.model;';
	'case ''define'' then'
	'  '+sci2exp(capt(:,3),'in',70); //input ports sizes
	'  '+sci2exp(actt(:,3),'out',70); //output ports sizes
	'  '+sci2exp(z,'z',70); //initial state
	'  '+sci2exp(outtb,'outtb',70); //initial link values
	'  iwa=zeros('+string(clkptr($))+',1)';
	'  Z=[z;outtb;iwa]';
        '  '+sci2exp(cpr.sim.rpar,'rpar',70); //real parameters
        '  '+sci2exp(cpr.sim.ipar,'ipar',70);//integer parameters
	'  '+sci2exp(clkinput,'clkinput',70);
	'  model=scicos_model(sim=list('''+rdnom+''',1),in=in,out=out,..'
	'          evtin=clkinput,dstate=Z,rpar=rpar,ipar=ipar,..'
	'          blocktype=''c'',dep_ut=[%t %f])'
	'  label=string(in)';
	'  gr_i=''xstringb(orig(1),orig(2),'''''+rdnom+''''',sz(1),..'
	'         sz(2),''''fill'''')''';
	'  x=standard_define([2 2],model,label,gr_i)';
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
     "OBJSSTAN="+rdnom+'.o '+rdnom+'_standalone.o '+rdnom+'_act_sens_events.o'
     "SCILIBS = $(SCIDIR)/libs/scicos.a $(SCIDIR)/libs/lapack.a "+..
     "$(SCIDIR)/libs/poly.a $(SCIDIR)/libs/calelm.a "+..
     "$(SCIDIR)/libs/blas.a $(SCIDIR)/libs/lapack.a"
     "LIBRARY = lib"+name
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
     "OBJSSTAN="+rdnom+'.obj '+rdnom+'_standalone.obj '+rdnom+'_act_sens_events.obj'
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
	      '      for (k=0;k<*nu;k++) {printf(""Actuator:'
	      ' time=%f, u(%d) of actuator %d is %f \n"", *t, k, *nport, u[k]);}  '
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
//Author : Rachid Djenidi


nin=size(capt,1);
if nin>0 then In='u'+string(1:nin);else In=[];end

nout= size(actt,1);
if nout then Out='y'+string(1:nout);else Out=[];end

//function arg list
Args=['flag','nevprt','t','xd','x','nx','z','nz','tvec','ntvec','rpar',..
      'nrpar','ipar','nipar']
if In<>[]  then Args=[Args matrix([In ;'n'+In ],1,-1)],end
if Out<>[] then Args=[Args matrix([Out;'n'+Out],1,-1)],end

//integer arguments
Iargs='*'+[Args([1 2 6 8 10 12:14])];
if In<>[]  then Iargs=[Iargs '*n'+In] ,end
if Out<>[] then Iargs=[Iargs '*n'+Out],end

//double precision arguments
Dargs='*'+[Args([3 4 5 7 9 11]),In,Out];

Code=['/*'+part('-',ones(1,40))+' Block Computational function */ ';
      'int ';
      cformatline(rdnom+'('+strcat(Args,' ,')+')',70)
      ''
      cformatline('  integer '+strcat(Iargs,' ,')+';',70)
      cformatline('  double  '+strcat(Dargs,' ,')+';' ,70)
      ''
      '{'
      '  /*  block_outtb is catenated at the end of z*/'
      '  double* block_outtb = z+'+string(nztotal)+';'
      ''];
     
//copy inputs
Code=[Code;
      '  /*Copy inputs in the block outtb */'];
for i=1:size(capt,1)
   ni=capt(i,3) // dimension of ith input
  Code=[Code;
        '  block_outtb['+string(capt(i,2)-1+(0:ni-1)')+'] = u'+..
	              string(capt(i,4))+'['+string(0:ni-1)'+'];';]
end

Code=[Code;
      '   set_nevprt(*nevprt);';
      ' '
      '  if (*flag == 1) { /* update outputs */';
      '    '+rdnom+'main1(z,t,rpar,nrpar,ipar,nipar);';
      '  } ';
      '  else if (*flag == 2) { /* update discrete states */';
      '    '+rdnom+'main2(z,t,rpar,nrpar,ipar,nipar);';
      '  } ';
      '  else if (*flag == 4) { /* initialisation */';
      '    '+rdnom+'_init(z,t,rpar,nrpar,ipar,nipar);';
      '  } ';
      '  else if (*flag == 5) { /* ending */';
      '    '+rdnom+'_end(z,t,rpar,nrpar,ipar,nipar);';
      '  } '];

	                    
//copy outputs
for i=1:size(actt,1)
  ni=actt(i,3) // dimension of ith output
  Code=[Code;
	'  y'+string(actt(i,4))+'['+string(0:ni-1)'+'] =  block_outtb['+..
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
	'#include <string.h>';
	'#include '"'+SCI+'/routines/machine.h'"';
	'#include '"'+SCI+'/routines/sun/link.h'"';
	'#include '"'+SCI+'/routines/scicos/scicos.h'"';
	' ';
	cformatline('void '+rdnom+'main1(double *z, double *t, '+..
		    'double * rpar, int *nrpar, int *ipar, int *nipar);',70);
	' ';
	cformatline('void '+rdnom+'main2(double *z, double *t, '+..
		    'double * rpar, int *nrpar, int *ipar, int *nipar) ;',70);
	' ';
	cformatline('void '+rdnom+'_init(double *z, double *t, '+..
		    'double * rpar, int *nrpar, int *ipar, int *nipar) ;',70);
	' ';
	cformatline('void '+rdnom+'_end(double *z, double *t, '+..
		    'double * rpar, int *nrpar, int *ipar, int *nipar) ;',70);
	' ';
	'int '+rdnom+'()  ;';
	' ';
	cformatline('int '+rdnom+'ddoit1(double *z, int * zptr, double *t, double"+...
		    " *tevts, int *evtspt, int * nevts, int *pointi, int * outptr,"+...
		    " int *clkptr, int *ordptr, int *ordclk, int *nordcl, double *"+...
		    " rpar, int *ipar, int *funptr, int *funtyp, double *outtb, int"+...
		    " *iwa)  ;',70);
	' ';
	cformatline('int '+rdnom+'ddoit2(double *z, int * zptr, double *t, double"+...
		    " *tevts, int *evtspt, int * nevts, int *pointi, int * outptr,"+...
		    " int *clkptr, int *ordptr, int *ordclk, int *nordcl, double *"+...
		    " rpar, int *ipar, int *funptr, int *funtyp, double *outtb, int"+...
		    " *iwa)  ;',70);
	' ';
	cformatline('int '+rdnom+'edoit1(double *z, int * zptr, double *t, double"+...
		    " *tevts, int *evtspt, int * nevts, int *pointi, int * outptr,"+...
		    " int *clkptr, int *ordptr, int *ordclk, int *nordcl, double *"+...
		    " rpar, int *ipar, int *funptr, int *funtyp, double"+...
		    " *outtb, int * urg, int *iwa, int *kiwa)  ;',70);	
	' ';
	cformatline('int '+rdnom+'_initi(double *z, int * zptr, double *t, double"+...
		    " *tevts, int *evtspt, int * nevts, int *pointi, int * outptr,"+...
		    " int *clkptr, int *ordptr, int *ordclk, int *nordcl, double *"+...
		    " rpar, int *ipar, int *funptr, int *funtyp, double *outtb, int"+...
		    " *iwa)  ;',70);
	' ';
	cformatline('int '+rdnom+'_endi(double *z, int * zptr, double *t, double"+...
		    " *tevts, int *evtspt, int * nevts, int *pointi, int * outptr,"+...
		    " int *clkptr, int *ordptr, int *ordclk, int *nordcl, double *"+...
		    " rpar, int *ipar, int *funptr, int *funtyp, double *outtb, int"+...
		    " *iwa)  ;',70);
	' ';
	cformatline('int '+rdnom+'_outtb(double *z, int * zptr, double *t, double"+...
		    " *tevts, int *evtspt, int * nevts, int *pointi, int * outptr,"+...
		    " int *clkptr, int *ordptr, int *ordclk, int *nordcl, double *"+...
		    " rpar, int *ipar, int *funptr, int *funtyp, double *outtb, int"+...
		    " *iwa)  ;',70);
	' ';
	cformatline('int C2F(putevs)(double *tevts, int *evtspt, int * nevts, int"+...
		    " *pointi, double *told, int *i2, int *ierr);',70);
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
	'';
	cformatline('void '+rdnom+'main1(double *z, double *t, '+..
		    'double * rpar, integer *nrpar,integer *ipar,integer *nipar);',70);
	'';
	cformatline('void '+rdnom+'main2(double *z, double *t, '+..
		    'double * rpar, integer *nrpar,integer *ipar,integer *nipar) ;',70);
	'';
	cformatline('void '+rdnom+'_init(double *z, double *t, '+..
		    'double * rpar, integer *nrpar,integer *ipar,integer *nipar) ;',70);
	'';
	cformatline('void '+rdnom+'_end(double *z, double *t, '+..
		    'double * rpar, integer *nrpar,integer *ipar,integer *"+...
		    " nipar) ;',70);
	''
	cformatline('void '+rdnom+'_sim(double tf)"+...
		    " ;',70);
	''
	cformatline('void '+rdnom+'_events(int *nevprt, "+...
		    "double *t);',70)
	''
	'void set_nevprt(int nevprt);']
endfunction
function Code=make_end()
//Copyright INRIA
//Author : Rachid Djenidi
  nordcl=size(ordclk,1);
  Code=['/*'+part('-',ones(1,40))+' end */ ';
	'void '
	cformatline(rdnom+'_end(double *z, double *t, double * rpar, '+..
		    'integer *nrpar,integer *ipar,integer *nipar)',70);
	'{'];
  
  if size(pointi,1) <> 0 then
    Code($+1)='  integer pointi[ ]={'+strcat(string(pointi),",")+'};'; 
  else
    Code($+1)='  integer pointi[1];';
  end

  Code=[Code
	'integer nordcl = '+string(nordcl)+';';
	''
	cformatline('  '+rdnom+'_endi(z, zptr, t, tevts, evtspt, nevts, pointi, '+..
		    'outptr, clkptr, ordptr, ordclk, &nordcl, rpar, ipar, '+..
		    'rdfunptr, funtyp, &(z['+string(size(z,1))+']), '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1))+'));',70);	
	'} '];
  
endfunction
function Code=make_init()
//Copyright INRIA
//Author : Rachid Djenidi
  nordcl=size(ordclk,1);
  Code=['/*'+part('-',ones(1,40))+' init */ ';
	'void '
	cformatline(rdnom+'_init(double *z, double *t, double * rpar, '+..
		    'integer *nrpar,integer *ipar,integer *nipar)',70);
	'{'];
  if size(pointi,1) <> 0 then
    Code($+1)='  integer pointi[ ]={'+strcat(string(pointi),",")+'};'; 
  else
    Code($+1)='  integer pointi[1];';
  end

  Code=[Code;
	'integer nordcl = '+string(nordcl)+';';
	'  /*Block initializations*/'
	cformatline('  '+rdnom+'_initi(z, zptr, t, tevts, evtspt, nevts, '+..
		    'pointi, outptr, clkptr, ordptr, ordclk, &nordcl, rpar, ipar, '+..
		    'rdfunptr,funtyp, &(z['+string(size(z,1))+']), '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1))+'));',70);
	'  /*Constants propagation*/'
	cformatline('  '+rdnom+'_outtb(z, zptr, t, tevts, evtspt, nevts, '+..
		    'pointi, outptr, clkptr, ordptr, ordclk, &nordcl, rpar, ipar, '+..
		    'rdfunptr,funtyp, &(z['+string(size(z,1))+']), '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1))+'));',70);
       '} '];
  
endfunction
function Code=make_main1()
//generate code for flag=1 case  
//Copyright INRIA
//Author : Rachid Djenidi
  nordcl=size(ordclk,1);
  Code=['/*'+part('-',ones(1,40))+' main1 */ ';
	'void '
	cformatline(rdnom+'main1(double *z, double *t, double * rpar, '+..
		    'integer *nrpar,integer *ipar,integer *nipar)',70);
	'{'];
  
  if size(pointi,1) <> 0 then
    Code($+1)='  integer pointi[ ]={'+strcat(string(pointi),",")+'};'; 
  else
    Code($+1)='  integer pointi[1];';
  end
  
  Code=[Code;
	'integer nordcl = '+string(nordcl)+';';
	''
	cformatline('  '+rdnom+'ddoit1(z, zptr, t, tevts, evtspt, nevts, '+..
		    'pointi, outptr, clkptr, ordptr, ordclk, &nordcl, rpar, ipar, '+..
		    'rdfunptr, funtyp, &(z['+string(size(z,1))+']), '+..
		    '(int *)(z+'+string(size(z,1)+size(outtb,1))+'));',70);
	'} '];
endfunction
function Code=make_main2()
//generate code for flag=2 case
//Copyright INRIA
//Author : Rachid Djenidi
  nordcl=size(ordclk,1);
  Code=['/*'+part('-',ones(1,40))+' main2 */ ';
	'void '
	cformatline(rdnom+'main2(double *z, double *t, double * rpar, '+..
		    'integer *nrpar,integer *ipar,integer *nipar)',70);
	'{'];
  
  if size(pointi,1) <> 0 then
    Code($+1)='  integer pointi[ ]={'+strcat(string(pointi),",")+'};'; 
  else
    Code($+1)='  integer pointi[1];';
  end
  Code=[Code;
	'integer nordcl = '+string(nordcl)+';';
	''
	cformatline('  '+rdnom+'ddoit2(z, zptr, t, tevts, evtspt, nevts, '+..
		    'pointi, outptr, clkptr, ordptr, ordclk, &nordcl, rpar, '+..
		    'ipar, rdfunptr, funtyp, &(z['+string(size(z,1))+']), '+..
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
	 'void '
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

  if size(z,1) <> 0 then
    for i=1:(length(zptr)-1) 
      
      if zptr(i+1)-zptr(i)>0 then
	aaa=scs_m.objs(cpr.corinv(i)).gui;bbb=emptystr(3,1);
	if and(aaa+bbb~= ['INPUTPORTEVTS';'OUTPUTPORTEVTS';'EVTGEN_f']) then
	  Code($+1)=' ';	
	  Code($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
	  Code($+1)=' Gui name of block: '+strcat(string(scs_m.objs(cpr.corinv(i)).gui));
	  //Code($+1)='/* Name block: '+strcat(string(cpr.sim.funs(i)));
	//Code($+1)='Object number in diagram: '+strcat(string(cpr.corinv(i)));
	Code($+1)='Compiled structure index: '+strcat(string(i));
	if stripblanks(scs_m.objs(cpr.corinv(i)).model.label)~=emptystr() then	    
	  Code=[Code;cformatline('Label: '+strcat(string(scs_m.objs(cpr.corinv(i)).model.label)),70)];
	end
	if stripblanks(scs_m.objs(cpr.corinv(i)).graphics.exprs)~=emptystr() then
	  Code=[Code;cformatline('Exprs: '+strcat(scs_m.objs(cpr.corinv(i)).graphics.exprs,","),70)];
	end
	if stripblanks(scs_m.objs(cpr.corinv(i)).graphics.id)~=emptystr() then
	  Code=[Code;
		cformatline('Identification: '+..
			    strcat(string(scs_m.objs(cpr.corinv(i)).graphics.id)),70)];
	  
	end
	Code=[Code;cformatline('z= {'+strcat(string(z(zptr(i):zptr(i+1)-1)),",")+'};',70)];
	Code($+1)='*/';
	end
      end
    end
  end
  Code=[Code;'  t=0.0;'
	'  '+rdnom+'_init(z,&t,RPAR1,&NRPAR1,IPAR1,&NIPAR1);'
	'  while (t<=tf) {   ';
	'    '+rdnom+'_events(&nevprt,&t);'
	'    set_nevprt(nevprt);'
	'    '+rdnom+'main1(z,&t,RPAR1,&NRPAR1,IPAR1,&NIPAR1);'
	'    '+rdnom+'main2(z,&t,RPAR1,&NRPAR1,IPAR1,&NIPAR1);'
	'  }'
	'  '+rdnom+'_end(z,&t,RPAR1,&NRPAR1,IPAR1,&NIPAR1);'
	'  return ;'
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
endfunction
function Code=make_static()
//generates  static table definitions
//Copyright INRIA
//Author : Rachid Djenidi
  
  z=cpr.state.z;
  tevts=cpr.state.tevts;evtspt=cpr.state.evtspt;
  nevts=size(evtspt,1);
  zptr=cpr.sim.zptr;
  outptr=cpr.sim.outptr;
  funtyp=cpr.sim.funtyp;
  clkptr=cpr.sim.clkptr;ordptr=cpr.sim.ordptr;
  ordclk=cpr.sim.ordclk;nordcl=size(ordclk,1);

  funs=cpr.sim.funs;
  
  Code= ['/* Table of constant values */ ';
	 ' ';
	 'static integer totalnevprt; '];
  
  if size(evtspt,1) <> 0 then
    Code=[Code;cformatline('static integer evtspt[ ]={'+..
			   strcat(string(evtspt),",")+'};',70)] 
  else
    Code($+1)='static integer evtspt[1];';
  end
  
  if size(nevts,1) <> 0 then
    Code=[Code;cformatline('static integer nevts[ ]={'+..
			   strcat(string(nevts),",")+'};',70)] 
  else
    Code($+1)='static integer nevts[1];';
  end
  Code($+1)='static double x[1];';
  
  
  if size(zptr,1) <> 0 then
    Code=[Code;cformatline('static integer zptr[ ]={'+..
			   strcat(string(zptr),",")+'};',70)] 
  else
    Code($+1)='static integer zptr[1];';
  end
  
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
  

  if size(ordclk,1) <> 0 then
    Code=[Code;cformatline('static integer ordclk[ ]={'+..
			   strcat(string(ordclk),",")+'};',70)] 
  else
    Code($+1)='static integer ordclk [1];';
  end
  
  if size(outptr,1) <> 0 then
    Code=[Code;cformatline('static integer outptr[ ]={'+..
			   strcat(string(outptr),",")+'};',70)] 
  else
    Code($+1)='static integer outptr[1];';
  end
  
  if size(tevts,1) <> 0 then
    Code=[Code;cformatline('static double tevts[ ] = {'+..
			   strcat(string(tevts),",")+'};',70)]     
  else
    Code($+1)='static double tevts[1];';
  end

  if length(funs) <> 0 then
    Code=[Code;cformatline('static integer rdfunptr[ ]={'+..
			   strcat(string(1:length(funs)),",")+'};',70)] 
  else
    Code($+1)='static integer rdfunptr[1];';
  end
	  
	  
  if size(funtyp,1) <> 0 then
    Code=[Code;cformatline('static integer funtyp[ ]={'+..
			   strcat(string(funtyp),",")+'};',70)] 
  else
    Code($+1)='static integer funtyp[1];';
  end
  Code($+1)='static double w[1];'
  Code=[Code;
	'void set_nevprt(int nevprt)'
	'{'
	'  totalnevprt=nevprt;'
	'}']
endfunction
function Code=make_static_standalone()
//generates  static table definitions
//Copyright INRIA
//Author : Rachid Djenidi

  rpar=cpr.sim.rpar;ipar=cpr.sim.ipar;
  nrpar=size(rpar,1);nipar=size(ipar,1);
  
  
  Code=[];nbrpa=0;
  if size(rpar,1) <> 0 then
    
    Code=[Code;'static double RPAR1[ ] = {'];		
    
    for i=1:(length(rpptr)-1) 
      if rpptr(i+1)-rpptr(i)>0  then
	aaa=scs_m.objs(cpr.corinv(i)).gui;bbb=emptystr(3,1);
	if and(aaa+bbb~= ['INPUTPORTEVTS'; ...
			  'OUTPUTPORTEVTS';'EVTGEN_f']) then
	  
	  nbrpa=nbrpa+1;
	  Code($+1)=' ';
	  Code($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
	  Code($+1)='   Gui name of block: '+strcat(string(scs_m.objs(cpr.corinv(i)).gui));
	  if stripblanks(scs_m.objs(cpr.corinv(i)).model.label)~=emptystr() then	    
	    Code=[Code;cformatline('Label: '+strcat(string(scs_m.objs(cpr.corinv(i)).model.label)),70)];
	  end
	  if stripblanks(scs_m.objs(cpr.corinv(i)).graphics.exprs)~=emptystr() then
	    Code=[Code;cformatline('Exprs: '+strcat(scs_m.objs(cpr.corinv(i)).graphics.exprs,","),70)];
	  end
	  if stripblanks(scs_m.objs(cpr.corinv(i)).graphics.id)~=emptystr() then
	    Code=[Code;
		  cformatline('Identification: '+strcat(string(scs_m.objs(cpr.corinv(i)).graphics.id)),70)];
	    
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
  
  Code($+1)='static integer NRPAR1  = '+string(nbrpa)+';';  
  nbipa=0;
  if size(ipar,1) <> 0 then
    
    Code=[Code;'static integer IPAR1[ ] = {'];		
    
    for i=1:(length(ipptr)-1) 
      
      if ipptr(i+1)-ipptr(i)>0  then
	aaa=scs_m.objs(cpr.corinv(i)).gui;bbb=emptystr(3,1);
	if and(aaa+bbb~= ['INPUTPORTEVTS';'OUTPUTPORTEVTS';'EVTGEN_f']) then
	  nbipa=nbipa+1;
	  Code($+1)=' ';
	  Code($+1)='/* Routine name of block: '+strcat(string(cpr.sim.funs(i)));
	  Code($+1)=' Gui name of block: '+strcat(string(scs_m.objs(cpr.corinv(i)).gui));
	  // Code($+1)='/* Name block: '+strcat(string(cpr.sim.funs(i)));
	  // Code($+1)='Object number in diagram: '+strcat(string(cpr.corinv(i)));
	  Code($+1)='Compiled structure index: '+strcat(string(i));
	  if stripblanks(scs_m.objs(cpr.corinv(i)).model.label)~=emptystr() then	    
	    Code=[Code;cformatline('Label: '+strcat(string(scs_m.objs(cpr.corinv(i)).model.label)),70)];
	  end
	  if stripblanks(scs_m.objs(cpr.corinv(i)).graphics.exprs)~=emptystr() then
	    Code=[Code;cformatline('Exprs: '+strcat(scs_m.objs(cpr.corinv(i)).graphics.exprs,","),70)];
	  end
	  if stripblanks(scs_m.objs(cpr.corinv(i)).graphics.id)~=emptystr() then
	    Code=[Code;
		  cformatline('Identification: '+strcat(string(scs_m.objs(cpr.corinv(i)).graphics.id)),70)];
	    
	  end
	  Code=[Code;cformatline('ipar= {'+strcat(string(ipar(ipptr(i):ipptr(i+1)-1)),",")+'};',70)];
	  Code($+1)='*/';
	  Code=[Code;cformatline(strcat(string(ipar(ipptr(i):ipptr(i+1)-1))+','),70)];
	end
      end
    end
    Code($+1)='};';
    //Code=[Code;cformatline('static integer IPAR1[ ]= {'+..
    //		   strcat(string(ipar),",")+'};',70)] 
  else
    Code($+1)='static integer IPAR1[1];';
  end
  Code($+1)='static integer NIPAR1  = '+string(nbipa)+';';  
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
  


