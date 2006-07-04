function [scs_m,edited]=do_SaveAs()
//
// Copyright INRIA
  tit=['Use .cos extension for binary and .cosf for ascii file'];
  fname=savefile('*.cos*',emptystr(),tit)

  if fname==emptystr() then return,end

  [path,name,ext]=splitfilepath(fname)
  select ext
    case 'cos' then
    ok=%t
    frmt='unformatted'
    case 'cosf' then
    ok=%t
    frmt='formatted'
  else
    message('Only *.cos binary or cosf ascii files allowed');
    return
  end

  if ~super_block&~pal_mode then
    //update %cpr data structure to make it coherent with last changes
    if needcompile==4 then
      %cpr=list()
    else
      [%cpr,%state0,needcompile,alreadyran,ok]=do_update(%cpr,%state0,needcompile)
      if ~ok then return,end
      %cpr.state=%state0
    end
  else
    %cpr=list()
  end

  // open the selected file
  if frmt=='formatted'
    [u,err]=file('open',fname,'unknown',frmt)
  else
    [u,err]=mopen(fname,'wb')
  end
  if err<>0 then
    message('File or directory write access denied')
    return
  end


  scs_m;
  scs_m.props.title=[name,path] // Change the title
  
  // save
  if ext=='cos' then
    save(u,scicos_ver,scs_m,%cpr)
  else
    //  disablemenus()
    if execstr('write(u,sci2exp(scicos_ver,''scicos_ver''),''(a)'')',..
	       'errcatch')<>0 then
      message('Directory write access denied')
      file('close',u)
      return
    end
    cos2cosf(u,do_purge(scs_m))
    //  enablemenus()
  end
  file('close',u)

  drawtitle(scs_m.props)  // draw the new title

  edited=%f
  if pal_mode then 
    scicos_pal=update_scicos_pal(path,scs_m.props.title(1),fname),
    scicos_pal=resume(scicos_pal)
  end
endfunction
