function demo_scicos()
  funcprot(0)
  S=remove_builtins()
  load SCI/macros/scicos/lib
  load SCI/macros/scicos_blocks/Branching/lib
  load SCI/macros/scicos_blocks/Electrical/lib
  load SCI/macros/scicos_blocks/Events/lib
  load SCI/macros/scicos_blocks/Hydraulics/lib
  load SCI/macros/scicos_blocks/Linear/lib
  load SCI/macros/scicos_blocks/Misc/lib
  load SCI/macros/scicos_blocks/NonLinear/lib
  load SCI/macros/scicos_blocks/Sinks/lib
  load SCI/macros/scicos_blocks/Sources/lib
  load SCI/macros/scicos_blocks/Threshold/lib
  deff('mode(x)','x=x')
  deff('halt()',' ')
  deff('x=lines(x)','x=0')
  getf('SCI/tests/demos/dialogs.sci')
  message=x_message
  dialog=x_dialog

  //recompile some Scilab function not to use builtins
  tobe_recompiled=['x_matrix';'getvalue';'scicos';'choosefile';
		   'do_addnew';'do_block';'do_color';'do_copy';
		   'do_copy_region';'do_delete';'do_delete_region';
		   'do_help';'do_move';'do_palettes';'do_replace';
		   'do_run';'do_tild';'do_view';'do_exit';'getlink';
		   'cosclick';'prt_align';'scifunc_block'];

  for f=tobe_recompiled'
    w=whereis(f)
    w=string(evstr(w));w=w(1)
    getf(w+f+'.sci')
  end


  sel=26 
  dialogs='demo'+string(sel)+'.dialogs'
  
  O=file('open',pathconvert(TMPDIR+'/sciout',%f,%f),'unknown')
  for dia=dialogs
    I=file('open','dialogs/'+dia,'old');
    %IO=[I,O];
    exec('SCI/demos/alldems.dem')
    file('close',I)
  end
  file('close',O)

  reload_builtins(S)
  
endfunction
function S=remove_builtins()
  builtins=['lines','x_message','x_dialog','x_mdialog','x_choose', ...
	    'mode','xclick','xgetmouse']
  ptr=list();N=[];
  for b=builtins
    N=[N funptr(b)]
    execstr('ptr($+1)='+b)
    clearfun(b)
  end
  S=tlist(['builtins','names','number','ptr'],builtins,N,ptr)
endfunction
function reload_builtins(S)
  for k=1:size(S.names,'*')
    newfun(S.names(k),S.number(k))
  end
endfunction

