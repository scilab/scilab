function ExportAll_()
    %lwin=xget('window')
    %oh=%t
    while %oh==%t
      %oh=%f
      %exp_dir=x_dialog('Directory where exported figures are placed',%exp_dir)
      if size(%exp_dir,'*')>1 then x_message('Try again');%oh=%t;end
    end
    if %exp_dir<>[] then 
      systexport(scs_m,%exp_dir),
      if MSDOS then
	fname=%exp_dir+'\'+'navigator'
      else
	fname=%exp_dir+'/'+'navigator'
      end
      [%junk,%jwin]=do_navigator(scs_m,windows)
      %winc=%jwin(find(%jwin(:,1)==100000),2)
      xbasimp(%winc,fname)
      if MSDOS then
	unix_g(''"'+SCI+'\bin\BEpsf'" -landscape '+fname+'.'+string(%winc))
      else
	unix_g(SCI+'/bin/BEpsf -landscape '+fname+'.'+string(%winc))
      end
      xdel(%winc)
      //
      xset('window',%lwin)
      window_set_size()
     
      set_background()
      Cmenu='Replot'
    else
      Cmenu=[]
    end
  
  
  
