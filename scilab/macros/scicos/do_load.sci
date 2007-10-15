function [ok, scs_m, %cpr, edited] = do_load(fname,typ)
// Copyright INRIA
//**
//** Load a Scicos diagram 
//**

  global %scicos_demo_mode ; 

  [lhs,rhs] = argn(0) ;
  edited = %f         ;
  
  if rhs<2 then
    typ = "diagram"
  end

  if alreadyran & typ=="diagram" then
    do_terminate(); //end current simulation
  end

  scicos_debug(0); //set debug level to 0 for new diagram loaded

  current_version = get_scicos_version() ;
  scicos_ver = "scicos2.2" //** default version,
                           //** for latter version scicos_ver is stored in files

  //** function [p] = tk_getfile(file_mask, path, Title, multip)	   
  if %scicos_demo_mode==1 then 
      //** open a demo file 
      if rhs<=0 then
        file_mask = "*.cos*" ;  //** force the demos/scicos path 
	path      =  SCI+"/demos/scicos" ; 
	fname = getfile(file_mask, path) ; 
      end
 
  else 
      //** conventional Open 
      if rhs<=0 then
        fname = getfile('*.cos*') ; 
      end
  end 
  %scicos_demo_mode = []; //** clear the variable  
  
  
  fname = stripblanks(fname) ; 
  
  if fname<>emptystr() then
    %cpr=list()
    scs_m=[]
    [path,name,ext]=splitfilepath_cos(fname)
    //first pass
    if ext=='cos'|ext=='COS'|ext=='cosf'|ext=='COSF'|ext=='' then
      if ext=='' then  // to allow user not to enter necessarily the extension
	fname=fname+'.cos'
	ext='cos'
      end
      [x,ierr]=fileinfo(fname)
      if ierr==0 then
        ww=stacksize()
        if ww(1)<2*x(1) then
          disp('stacksize increased to '+string(2*x(1)))
          stacksize(2*x(1))
        end
      else
        message([name+' cannot be loaded.';'Opening a new diagram'])
	ext='new'
      end
    else
      message(['Only *.cos (binary) and *.cosf (formatted) files';
               'allowed'])
      ok=%f
      scs_m=scicos_diagram(version=current_version)
      return
    end

    //second pass
    if ext=='cos'|ext=='COS' then
      ierr=execstr('load(fname)','errcatch')
      ok=%t
    elseif ext=='cosf'|ext=='COSF' then
      ierr=execstr('exec(fname,-1)','errcatch')
      ok=%t
    elseif ext=='new'
      ok=%t
      ierr=0
      scs_m=scicos_diagram(version=current_version)
      scs_m.props.title=name
    end
    if ierr<>0 then
      message('An error has occur during execution of '+name+'.')
      ok=%f
      scs_m=scicos_diagram(version=current_version)
      return
    end

    //check version
    if type(scs_m)==17 then
      if find(getfield(1,scs_m)=='version')<>[] then
        if scs_m.version<>'' then
          scicos_ver=scs_m.version
        else
          scicos_ver=find_scicos_version(scs_m)
        end
      end
    end
    if scicos_ver=='scicos2.2' then
      if scs_m==[] then scs_m=x,end //for compatibility
    end
    if scicos_ver<>current_version then
      scs_m=do_version(scs_m,scicos_ver)
      %cpr=list()
      edited=%t
    end

  else
    ok=%f
    //scs_m=list()
    return
  end
  scs_m.props.title=[scs_m.props.title(1),path]

  if typ=='diagram' then
    if %cpr<>list() then

      for jj=1:size(%cpr.sim.funtyp,'*')
	if type(%cpr.corinv(jj))==15 then
	  //force recompilation if diagram contains Modelica Blocks
	  //Can be improved later, re-generating C code only...
	  %cpr=list()
	  edited=%t
	  return
	end
	ft=modulo(%cpr.sim.funtyp(jj),10000)
	if ft>999 then
	  funam=%cpr.sim.funs(jj)
	  //regenerate systematically dynamically linked blocks forsafety
	  //[a,b]=c_link(funam); while a;  ulink(b);[a,b]=c_link(funam);end
	  //should be better than
	  //"if  ~c_link(funam) then"
	  //but ulink remove .so files and Makefile doesnt depends on .so file...
	  if ~c_link(funam) then

	    qqq=%cpr.corinv(jj)
	    path=list('objs',qqq(1))
	    for kkk=qqq(2:$)
	      path($+1)='model'
	      path($+1)='rpar'
	      path($+1)='objs'
	      path($+1)=kkk
	    end

	    path($+1)='graphics';path($+1)='exprs';path($+1)=2;
	    tt=scs_m(path)
	    if ft>1999 then
	      [ok]=scicos_block_link(funam,tt,'c')
	    else
	      [ok]=scicos_block_link(funam,tt,'f')
	    end
	  end
	end
      end
    end
  end
endfunction

