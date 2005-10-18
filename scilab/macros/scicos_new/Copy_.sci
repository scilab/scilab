function Copy_()
  if Select<>[] then
    [pt1,win1,o]=get_selection(Select,%pt,%win)
    if typeof(o)=='Block' then  // block may no longer exist
      Clipboard=o
    else
      message(['No block or region is selected'; 
	       'click on a block or select a region first.'])
    end
    Cmenu=[];
    %pt=[];
    %ppt=[]
  elseif  SelectRegion<>list() then
    Clipboard=get_inside(SelectRegion)
    Cmenu=[];
    %pt=[]
    %ppt=[]
  else
    message(['No block or region is selected'; 
	     'click on a block or select a region first.'])
    Cmenu=[]; %pt=[]; %ppt=[]
  end
endfunction
