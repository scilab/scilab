function [ok,scs_m,%cpr,edited]=do_load(fname,typ)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  edited=%f
  if rhs<2 then typ='diagram',end

  if alreadyran&typ=='diagram' then 
    do_terminate(),//end current simulation
  end  

  current_version=scicos_ver
  scicos_ver='scicos2.2' //default version, for latter version scicos_ver is stored in files

  if rhs<=0 then fname=xgetfile('*.cos*'),end
  if fname<>emptystr() then
    %cpr=list()
    scs_m=[]
    [path,name,ext]=splitfilepath(fname)

    select ext
    case 'cosf'
      ierr=execstr('exec(fname,-1)','errcatch')
      ok=%t
    case 'cos' then
      ierr=execstr('load(fname)','errcatch')
      ok=%t
    else
      message(['Only *.cos (binary) and *.cosf (formatted) files';
	       'allowed'])
      ok=%f
      scs_m=list()
      return
    end
    if ierr<>0 then
      message(name+' cannot be loaded.') 
      ok=%f;return
    end
//    if scs_m==[] then scs_m=x,end //for compatibility
    if scicos_ver<>current_version then 
      scs_m=do_version(scs_m,scicos_ver),
      %cpr=list()
      edited=%t
    end

  else
    ok=%f
    //scs_m=list()
    return
  end
  scs_m=do_versionxx(scs_m);//TEMPORAIRE A SUPPRIMER
  scs_m=do_version26(scs_m);
  scs_m.props.title=[scs_m.props.title(1),path]

  if typ=='diagram' then
    if %cpr<>list() then
      for jj=1:size(%cpr.sim.funtyp,'*')
	if %cpr.sim.funtyp(jj)<10000 then
	  if %cpr.sim.funtyp(jj)>999 then
	    funam=%cpr.sim.funs(jj)
	    if ~c_link(funam) then
	      qqq=%cpr.corinv(jj)
	      path=list(qqq(1))
	      for kkk=qqq
		path($+1)='model'
		path($+1)='rpar'
		path($+1)='objs'
		path($+1)=kkk
	      end
	      path($+1)='graphics';path($+1)='exprs';path($+1)=2;
	      tt=scs_m.objs(path)
	      if %cpr.sim.funtyp(jj)>1999 then
		[ok]=do_ccomlink(funam,tt)
	      else
		[ok]=do_forcomlink(funam,tt)
	      end 
	    end
	  end
	end
      end
    end
  end
endfunction

function scs_m_new=do_versionxx(scs_m)
//A SUPPRIMER CF ci dessus
  if typeof(scs_m)=='diagram' then scs_m_new=scs_m,return,end
  if typeof(scs_m(1))<>'params' then scs_m_new=scs_m,return,end
  scs_m_new=scicos_diagram()
  scs_m_new.props=scs_m(1)
  scs_m_new.objs(1)=mlist('Deleted') // not to change the internal numbering
  n=size(scs_m)
  for i=2:n //loop on objects
    o=scs_m(i)
    scs_m_new.objs(i)=o
    if typeof(o)=='Block' then
      if o.model.sim(1)=='super'| o.model.sim(1)=='csuper' then
	scs_m_new.objs(i).model.rpar=do_versionxx(o.model.rpar)
      end
    end
  end
  edited=resume(%t)
endfunction
