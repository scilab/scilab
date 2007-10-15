function TkPopup_()
// Copyright INRIA
  Select=[]

  Cmenu = mpopup(list('Place in Diagram','Help','Details')) 
  if Cmenu=='Place in Diagram' then  
    Cmenu='PlaceinDiagram',
  elseif  Cmenu=='Help' then
    Cmenu=[]
    global scs_m_palettes
    %pa_=TCL_GetVar('blko');%pa_=part(%pa_,6:length(%pa_));execstr('%pa_=list('+%pa_+')');
    o=scs_m_palettes(scs_full_path(%pa_));
    help(o.gui);
  elseif Cmenu=='Details' then
    Cmenu=[]
    global scs_m_palettes
    %pa_=TCL_GetVar('blko');%pa_=part(%pa_,6:length(%pa_));execstr('%pa_=list('+%pa_+')');
    do_details(scs_m_palettes(scs_full_path(%pa_)));
  end

endfunction
