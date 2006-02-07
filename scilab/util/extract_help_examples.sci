function extract_help_examples(dirpaths)

// given a vector of strings giving help  chapter paths, or nothinf for
// the default scilab help chapter paths,
// this function build in SCI/tests/Examples the files required for
// testing the examples given in all  xml help files available in the
// given paths.
  
  logfile=mopen('extract_help_examples.log','w')
  lsave=lines();lines(0)
  if argn(2)<1 then
    dirpaths=%helps(:,1)';
  end
  for d=dirpaths
    helppaths=listfiles(pathconvert(d+'/*.xml',%f,%t))';
    mfprintf(logfile,'\nExtracting examples from '+d+'\n');
    mprintf('\n-- Extracting examples from '+d+'\n');
    count=0
    for h=helppaths
      mprintf(h+"\n")
      build_example_test(h,logfile)
    end
    mfprintf(logfile,'\n')
    mprintf('\n')
  end
  lines(lsave(2))
  mclose(logfile)
  
endfunction



function build_example_test(xml_path,logfile)
  
// given the path of an xml help file
// this function build in SCI/tests/Examples the files required for
// testing the examples given in the xml help file
  
  name=basename(xml_path)
  if argn(2)<2 then logfile=mopen(name+'.log','w'),end
  
  T=extract_help_example(xml_path); //look for examples
  
  if T<>[] then //an example is given
    t=gettesttype(T) //check if it includes interactive or garphic functions
    gen_test_files(T,name,t);
  end
  
endfunction




function t=gettesttype(T)
  
  interactive=['%io(1)','mscanf','dialog','tk','message','getcolor', ...
	       'xset()','xclick','xgetmouse', 'locate','edit', ...
	       'x_choices','x_matrix','show_graph','addmenu','delmenu',..
	       'uicontrol','uimenu','showprofile','scipad','emacs','setbpt',..
	       'eventhandler','dragrect','rubber','getfile','demoplay','getvalue',...
	       'portrait','plotprofile','getfont','getmark','getlinestyle']
  
  graphics=['plot','pie','bar','mesh','barh','xrect','xstring','xarc','xfarc','xpoly','xfpoly','xarrow', ...
	    'xset(', 'xget(','contour','bode','black','nyquist','evans', ...
	    'param3d','champ(','surf','gcf(','gca(', 'gce(', 'champ1', 'drawaxis',..
	    'xgraduate','glue','polar','mapsound']
  
  //deff('foo',T)
  //T=primitives('foo')
  
  if grep(T,interactive)<>[] then 
    t='i'
  elseif grep(T,graphics)<>[] then 
    t='g'
  else
    t=''
  end
  
endfunction




function T=extract_help_example(h)
  
//extract the example out of an xml help file
  
  if name=='man' then T=[],return,end
  T=mgetl(h);
  [row,which]=grep(T,['<EXAMPLE>','</EXAMPLE>']);
  if row<>[] then 
    if size(row,'*')==1 then row=[row row],end
    T=T(row(1):row(2));
    T(1)=stripblanks(strsubst(T(1),'<EXAMPLE>',''));
    T($)=stripblanks(strsubst(T($),'</EXAMPLE>',''));
    if length(T(1))==0 then T(1)=[];end
    if length(T($))==0 then T($)=[];end
    if T<>[] then
      T(1)=stripblanks(strsubst(T(1),'<![CDATA[',''));
      T($)=stripblanks(strsubst(T($),']]>',''));
      if length(T(1))==0 then T(1)=[];end
      if length(T($))==0 then T($)=[];end
    end
  else
    T=[]
  end
  
endfunction



function gen_test_files(instructions,name,t)
  
//set output directory
  
  select t
  case 'i' then 
    test_path='SCI/tests/interactive_tests/'
  case 'g'
    test_path='SCI/tests/graphical_tests/'
  else
    test_path='SCI/tests/automatic_tests/'
  end
  
  //set file names
  tst=pathconvert(test_path+name+'.tst',%f,%f)
  ref=pathconvert(test_path+name+'_data.ref',%f,%f)
  dia=strsubst(tst,'.tst','.dia')
  
  interactive=t=='i'
  
  info=fileinfo(tst)//to check if the .tst file exists
  
  generate_dataref=%f;generate_tst=%f;
  if info==[] //no <name>.tst file exists
    mfprintf(logfile,'\n New Example '+name+'  the reference is built. Check\n')
    generate_tst=%t
    if ~interactive then
      generate_dataref=%t
      T=instrument_instruction('cmp',instructions,ref);
    end
  else 
    // a tst file exists, check for validity
    //compare it with instructions
    T=instrument_instruction('cmp',instructions,ref);
    if newest(tst,xml_path)==1 then 
      if fileinfo(ref)==[]&(~interactive) then
	mfprintf(logfile,'\n Example in '+name+'_data.ref no more exist, It is regenerated\n')
	generate_dataref=%t
      end
    else
      T1=mgetl(tst);
      if or(T1<>T) then
	mfprintf(logfile,'\n Example in '+name+' help file has changed, the reference is updated. Check\n')
	generate_tst=%t
	generate_dataref=%t
      elseif fileinfo(ref)==[]&(~interactive) then
	mfprintf(logfile,'\n Example in '+name+'_data.ref no more exist, It is regenerated\n')
	generate_dataref=%t
      end
    end
  end
  
  if generate_dataref&(~interactive) then
    
    //build the reference data
    
    Tb=instrument_instruction('build',instructions,ref);
    build_reference_data(Tb)
    
    [x,ierr]=fileinfo(ref) 
    if x(1)==0 then mfprintf(logfile,'generated file: '+name+'_data.ref is Empty '),end
  end
  
  if generate_tst then
    // write down the test script
    mputl(T,tst),
    //remove the <name>.dia file
    if fileinfo(dia)<>[] then unix('rm -f '+pathconvert(dia,%f,%t)); end
    dia=dia+'.ref'
    //remove the <name>.dia.ref file
    if fileinfo(dia)<>[] then unix('rm -f '+pathconvert(dia,%f,%t)); end
  end
  
endfunction



function build_reference_data(instructions)
  
  mputl(instructions,TMPDIR+'/temp.tst')
  
  // Execute the temporary file to generate the data reference file
  if execstr('exec(TMPDIR+''/temp.tst'',-1)','errcatch')<>0 then
    mclose(%U)
  end

endfunction



function instructions=instrument_instruction(job,instructions,refpath)
  
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Executes the given instructions saving all the intermediate 
// results into a Scilab  binary file. 
// This function must mirror the  run_with_ref one.
  
  if argn(2)<>3 then error(42),end
  // Form a function with the given instructions
  mputl(['function %test()';instructions;'xdel(winsid());';'endfunction'],TMPDIR+'/test.sci')
  // Compile and load it
  //use exec instead of getf to handle inline functions definition
  exec(TMPDIR+'/test.sci',-1);
  // Get the pseudo-code image

  l1=macr2lst(%test)
  
  // Adapt the  pseudo-code to add build or run tools
  if job=='build' then
    l=add_ref_code(list(l1(5:$)));
  else //run
    l=add_cmp_code(list(l1(5:$)));
  end
  l=list(l(1:$-2));
  
  // Generate the Scilab instructions from the modified pseudo-code
  txt=pseudocode2text(l)
  
  // Add opening and closing instructions
  instructions=[]
  if job<>'build' then instructions='getf SCI/util/testexamples.sci',end
  instructions=[instructions;'reinit_for_test()'];
  if job=='build' then 
    instructions=[instructions;'%U=mopen('''+refpath+''',''wb'');']
  else
    instructions=[instructions;'%U=mopen('''+refpath+''',''rb'');']
  end
  instructions=[instructions; txt;'mclose(%U);'];
  
endfunction



function p=primitives(mac_name,p)

//search for the  primitives called by a given macro, the second arg is
//for recursive calls only
  
  funcprot(0)
  global scanned_macros
  
  if argn(2)==1  then p=[],scanned_macros=[];end
  ierr=execstr('vars=macrovar('+mac_name+')','errcatch')
  if ierr<>0 then return,end
  called=vars(4);nc=size(called,1)
  if nc==0 then p=[],return,end
  to_get=[]
  for k=1:nc
    if find(called(k)==scanned_macros)<>[] then 
      to_get=[to_get,%f];
    else
      to_get=[to_get,%t];
    end
  end
  called=called(to_get)
  nc=size(called,'*')
  for k=1:nc
    clear w
    execstr('w='+called(k))
    if type(w)==130 then
      if grep(p,called(k))==[] then p=[p called(k)],end
    else
      scanned_macros=[scanned_macros called(k)]
      p=primitives(called(k),p)
    end
  end
  
endfunction


function txt=pseudocode2text(p)
  
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Generate Scilab instructions corresponding to the pseudo-code  
  
//make ins2sci known
  prot=funcprot();funcprot(0);fun2string=fun2string;funcprot(prot);
  //initialize variables for it
  lcount=1;level=[0,0];
  quote=''''
  dquote='""'
  CR='\@'
  opstable()
  txt=ins2sci(mmodlst(p),1)
endfunction



function ln=add_ref_code(l)
  
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Given a  pseudo-code  this function add code after each affectation
//operation (29) to save lhs variables into a binary file
  
  ln=list()// initialize result pseudo-code image
  for k=1:size(l) //loop on pseudo-codes
    lk=l(k);
    if type(lk)==10 then //regular pseudo-code
      
      if lk(1)=='29' then //affectation operation
	if and(lk(2)<>['52' '43' '99']) then lk(2)='52',end
	
	if or(lk(2)==['52'  '99']) then 
	  
	  //display required build reference
	  lk(2)='43' //disable display
	  if lk(3)=="ans" then 
	    if and(l(k-1)==['5','25','1','1'])&l(k-2)(1)=='2' then
	      vv=l(k-2)(2)
	      lk(3)=l(k-2)(2)
	      if exists(vv)==1 then
		execstr('var='+vv)
		if exists(vv)==1&or(type(var)==[11 13 130]) then
		  //name is a function, syntax is equivalent to ans=name()
		  lk(3)="%ans";
		end
	      end
	    elseif and(l(k-1)==['20','deff','2','1']) then 
	      ln($+1)=lk
	      continue
	    else
	      lk(3)="%ans";
	    end
	  end 
	  
	  ln($+1)=lk
	  
	  for i=3:2:size(lk,'*')
	    ln($+1)="15" //newline
	    name=lk(i);ref=name+"_ref"
	    // add instruction: save_ref('''+name+''');
	    // 
	    ln($+1)=["3",name];
	    ln($+1)=["2","save_ref","-2","1"];
	    ln($+1)=["5","25","2","1"];
	    ln($+1)=["29","43","ans","0"];
	  end
	  
	  ln($+1)="15";
	else
	  ln($+1)=lk
	end
	
      elseif  lk(1)=='2'&or(lk(2)==['xbasc','xdel','clf']) then 
	//change some function names to allow overloading
	lk(2)=lk(2)+'_build'
	ln($+1)=lk
	
      else //other operations, just copy them
	ln($+1)=lk
      end
      
    elseif type(lk)==15 then //control structure starts
      lt=lk(1)
      //call the add_ref_code function recursively for imbedded instructions
      if or(lt(1)==['if','while']) then
	lnk=list(lk(1),lk(2))
	for i=3:size(lk), lnk($+1)=add_ref_code(lk(i));end
	ln($+1)=lnk
      elseif lt(1)=='for' then
	ln($+1)=list(lk(1),lk(2),add_ref_code(lk(3)))
      elseif lt(1)=='select' then
	lnk=list(lk(1),lk(2))
	for i=3:2:size(lk)-1 //the "case" sequence
	  lnk($+1)=lk(i)
	  lnk($+1)=add_ref_code(lk(i+1));
	end
	//the else
	if i+2==size(lk) then lnk($+1)=add_ref_code(lk(i+2));end
	ln($+1)=lnk
      end
    end
  end
endfunction



function ln=add_cmp_code(l)
  
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Given a  pseudo-code  this function add code after each affectation
//operation (29) to compare lhs variables with references given in  a binary file
// This function must mirror the  add_ref_code one
  
  ln=list() // initialize result pseudo-code image
  for k=1:size(l) //loop on pseudo-codes
    lk=l(k);
    if type(lk)==10 then //regular pseudo-code
      if lk(1)=='29' then //affectation operation
	if and(lk(2)<>['52' '43' '99']) then lk(2)='52',end
	if or(lk(2)==['52'  '99']) then 
	  //display required compare result with reference
	  lk(2)='43'// disable display
	  if lk(3)=="ans" then 
	    if and(l(k-1)==['5','25','1','1'])&l(k-2)(1)=='2' then
	      vv=l(k-2)(2)
	      lk(3)=l(k-2)(2)
	      if exists(vv)==1 then
		execstr('var='+vv)
		if exists(vv)==1&or(type(var)==[11 13 130]) then
		  //name is a function, syntax is equivalent to ans=name()
		  lk(3)="%ans";
		end
	      end
	    elseif and(l(k-1)==['20','deff','2','1']) then 
	      ln($+1)=lk
	      continue
	    else
	      lk(3)="%ans";
	    end
	  end 
	  
	  ln($+1)=lk
	  
	  for i=3:2:size(lk,'*')
	    ln($+1)="15"//newline
	    name=lk(i);ref=name+"_ref";
	    // add instruction: if load_ref('''+name+''') then pause;end
	    ln($+1)=list("if",list(["3",name],["2","load_ref","-2","1"],["5","25","2","1"]),..
			 list("12"),list())
	  end
	  
	  ln($+1)="15"
	else
	  ln($+1)=lk
	end
      elseif  lk(1)=='2'&or(lk(2)==['xbasc','xdel','clf']) then
	//change some function names to allow overloading
	lk(2)=lk(2)+'_run'
	ln($+1)=lk
      else //other operations, just copy them
	ln($+1)=lk
      end
    elseif type(lk)==15 then  //control structure starts
      lt=lk(1)
      //call the add_ref_code function recursively for imbedded instructions
      if or(lt(1)==['if','while']) then
	lnk=list(lk(1),lk(2))
	for i=3:size(lk), lnk($+1)=add_cmp_code(lk(i));end
	ln($+1)=lnk
      elseif lt(1)=='for' then	
	ln($+1)=list(lk(1),lk(2),add_cmp_code(lk(3)))
      elseif lt(1)=='select' then
	lnk=list(lk(1),lk(2))
	for i=3:2:size(lk)-1 //the "case" sequence
	  lnk($+1)=lk(i)
	  lnk($+1)=add_cmp_code(lk(i+1));
	end
	//the else
	if i+2==size(lk) then lnk($+1)=add_cmp_code(lk(i+2));end
	ln($+1)=lnk
      end
    end
  end
  
endfunction



function r=xbasc_build(w)
  
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Save the graphic windows to be cleared in  a Scilab  binary file. 
// This function must mirror the  xbasc_run one.
  
  r=%f
  if winsid()==[] then return,end
  cur=xget('window')
  //
  if argn(2)==1 then
    ids_ref=[]
    
    for k=1:size(w,'*')
      xset('window',w(k))
      if get('figure_style')=='new' then ids_ref=[ids_ref,w(k)],end
    end
    
    save(%U,ids_ref)
    
    for k=ids_ref
      %wins_ref=ghdl2tree(scf(k));save(%U,%wins_ref);
    end
    
    xbasc(w)
  else
    if get('figure_style')=='old' then return,end
    ids_ref=xget('window');
    save(%U,ids_ref)
    %wins_ref=ghdl2tree(gcf());
    save(%U,%wins_ref)
    xbasc()
  end
  if or(winsid()==cur) then xset('window',cur),end
  
endfunction



function r=xbasc_run(w)

//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Compare the graphic windows to be cleared with the reference givenin  a Scilab  binary file. 
// This function must mirror the  xbasc_build one.
  
  r=%f
  if winsid()==[] then return,end
  cur=xget('window')
  //
  if argn(2)==1 then
    ids_=[]
    for k=1:size(w,'*')
      xset('window',w(k))
      if get('figure_style')=='new' then ids_=[ids_,w(k)],end
    end
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    for k=ids_ref
      %wins_=ghdl2tree(scf(k));
      load(%U,'%wins_ref');
      if %CMP(%wins_, %wins_ref) then r=%t,return,end
    end
    xbasc(w)
  else
    if get('figure_style')=='old' then return,end
    ids_=xget('window');
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    %wins_=ghdl2tree(gcf());
    load(%U,'%wins_ref');
    if %CMP(%wins_, %wins_ref) then r=%t,return,end
    xbasc()
  end
  if or(winsid()==cur) then xset('window',cur),end
  
endfunction



function r=clf_build(w,opt)
  
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Save the graphic windows to be cleared in  a Scilab  binary file. 
// This function must mirror the  clf_run one.
  
  r=%f
  if winsid()==[] then return,end
  cur=xget('window')
  rhs=argn(2)
  if rhs==1&type(w)==10 then opt=w;rhs=0,end
  if winsid()==[] then return,end
  //
  if rhs>=1 then
    ids_ref=[]
    if type(w)==9 then 
      H=w;w=[]
      for h=H,w=[w,h.figure_id],end
    end
    for k=1:size(w,'*')
      xset('window',w(k))
      if get('figure_style')=='new' then ids_ref=[ids_ref,w(k)],end
    end
    save(%U,ids_ref)
    for k=ids_ref,%wins_ref=ghdl2tree(scf(k));save(%U,%wins_ref);end
    if rhs==1 then clf(w),else clf(w,opt),end
  else
    if get('figure_style')=='old' then return,end
    ids_ref=xget('window');
    save(%U,ids_ref)
    %wins_ref=ghdl2tree(gcf());
    save(%U,%wins_ref)
    clf()
  end
  if or(winsid()==cur) then xset('window',cur),end
  
endfunction



function r=clf_run(w,opt)
  
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Compare the graphic windows to be cleared with the reference givenin  a Scilab  binary file. 
// This function must mirror the  clf_build one.
  
  r=%f
  if winsid()==[] then return,end
  cur=xget('window')
  rhs=argn(2)
  if rhs==1&type(w)==10 then opt=w;rhs=0,end
  if winsid()==[] then return,end
  //
  if rhs==1 then
    ids_=[]
    if type(w)==9 then 
      H=w;w=[]
      for h=H,w=[w,h.figure_id],end
    end
    for k=1:size(w,'*')
      xset('window',w(k))
      if get('figure_style')=='new' then ids_=[ids_,w(k)],end
    end
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    for k=ids_ref
      %wins_=ghdl2tree(scf(k));
      load(%U,'%wins_ref');
      if %CMP(%wins_, %wins_ref) then r=%t,return,end
    end
    if rhs==1 then clf(w),else clf(w,opt),end
  else
    if get('figure_style')=='old' then return,end
    ids_=xget('window')
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    %wins_=ghdl2tree(gcf());
    load(%U,'%wins_ref');
    if %CMP(%wins_,%wins_ref) then r=%t,return,end
    clf()
  end
  if or(winsid()==cur) then xset('window',cur),end
  
endfunction



function r=xdel_build(w)

//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Save the graphic windows to be cleared in  a Scilab  binary file. 
// This function must mirror the  xdel_run one.
  
  r=%f  
  if winsid()==[] then return,end
  cur=xget('window')
  //
  if argn(2)>=1 then
    ids_ref=[]
    for k=1:size(w,'*')
      xset('window',w(k))
      if get('figure_style')=='new' then ids_ref=[ids_ref,w(k)],end
    end
    save(%U,ids_ref)
    for k=ids_ref,%wins_ref=ghdl2tree(scf(k));save(%U,%wins_ref);end
    xdel(w);
  else
    if get('figure_style')=='old' then return,end
    ids_ref=xget('window');
    save(%U,ids_ref)
    %wins_ref=ghdl2tree(gcf());
    save(%U,%wins_ref)
    xdel()
  end
  if or(winsid()==cur) then xset('window',cur),end
  
endfunction



function r=xdel_run(w,opt)
  
//Author : Serge Steer, april 2005, Copyright INRIA
//  
//Compare the graphic windows to be cleared with the reference givenin  a Scilab  binary file. 
// This function must mirror the  xdel_build one.
  
  r=%f
  if winsid()==[] then return,end
  cur=xget('window')
  //  
  if argn(2)==1 then
    ids_=[]
    for k=1:size(w,'*')
      xset('window',w(k))
      if get('figure_style')=='new' then ids_=[ids_,w(k)],end
    end
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    for k=ids_ref
      %wins_=ghdl2tree(scf(k));
      load(%U,'%wins_ref');
      if %CMP(%wins_, %wins_ref) then r=%t,return,end
    end
    xdel(w)
  else
    if get('figure_style')=='old' then return,end
    ids_=xget('window')
    load(%U,'ids_ref')
    if or(ids_ref<>ids_) then r=%t,return,end
    %wins_=ghdl2tree(gcf());
    load(%U,'%wins_ref');
    if %CMP(%wins_,%wins_ref) then r=%t,return,end
    xdel()
  end
  if or(winsid()==cur) then xset('window',cur),end
  
endfunction



function save_ref(name)

  if exists(name)==0 then return,end
  v=evstr(name)
  if type(v) == 9 then   v = ghdl2tree(v);end,
  if type(v) == 128 then   v = 128;end,// lu handle
  execstr(name+'_ref=v;save(%U,'+name+'_ref'+')')
  
endfunction



function r=load_ref(name)
  if exists(name)==0 then r=%f;return,end
  v=evstr(name)
  if type(v) == 9 then   v = ghdl2tree(v);end,
  execstr(name+'_ref=v;load(%U,'+name+'_ref'+');r=%CMP(v,'+name+'_ref);')
endfunction



function reinit_for_test()
  
//reinitialize some Scilab state to be able to reproduce the same tests
  
  sdf();sda()
  xdel(winsid())
  grand('setgen','clcg4');grand('setall',11111111,22222222,33333333,44444444);
  grand('setgen','kiss');grand('setsd',362436069,521288629,123456789,380116160);
  grand('setgen','clcg2');grand('setsd',1234567890,123456789);
  grand('setgen','urand');grand('setsd',0);
  grand('setgen','fsultra');grand('setsd',1234567,7654321);
  grand('setgen','mt');grand('setsd',5489);
  rand('seed',0);
  
  clearglobal() 
  format('v',10)
  
endfunction
