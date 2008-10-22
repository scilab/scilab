// -----------------------------------------------------------
// Allan CORNET
// INRIA 2005
// -----------------------------------------------------------
 function PName=GetUiControlPropertyName(PropertyName)
 TableProperties    = ['backgroundcolor' ..
												'callback' ..
												'fontangle' ..
												'fontsize' ..
												'fontunits' ..
												'fontweight' ..
												'fontname' ..
												'listboxtop' ..
												'max' ..
												'min' ..
												'parent' ..
												'position' ..
												'sliderstep' ..
												'string' ..
												'style' ..
												'tag' ..
												'units' ..
												'userdata' ..
												'value' ..
												'label' ..
												'figure_name' ..
											  'verticalalignment' ..
												'horizontalalignment' ..
												'foregroundcolor'];
  //conversion to lower format
  str = convstr(PropertyName);
  k=find(part(TableProperties,1:length(str))==str);

  if (k == []) then
    disp("Error in Property specification : bad argument specified");
    PName='';
    return;
  end

  PName=TableProperties(k);

endfunction
// -----------------------------------------------------------