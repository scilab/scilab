function Paleditor_()
    Cmenu='Open/Set'
//    load('.scicos_pal')

    scicos_palnew=do_edit_pal(scicos_pal)
    if scicos_palnew<>[] & scicos_palnew<>scicos_pal then 
      scicos_pal=scicos_palnew
      errcatch(-1,'continue')
      save('.scicos_pal',scicos_pal)
      errcatch(-1)
      if iserror(-1) then
	errclear(-1)
	message('Cannot save .scicos_pal in current directory')
      end
    end
    
