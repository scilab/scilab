// -----------------------------------------------------------
// Allan CORNET
// INRIA 2005
// -----------------------------------------------------------
function TypeReturn=GetUiControlValueType(PropertyName)
  type_real=1;
  type_string=10;
  TableProperties    = ['backgroundcolor' ..
												'callback' ..
												'fontangle' ..
												'fontsize' ..
												'fontunits' ..
												'fontweight' ..
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
												'label'];
	
	TablePropertiesValueType = [type_real ..
	      											type_string ..
				       								type_real ..
								      				type_real ..
											      	type_string ..
												      type_string ..
												      type_real ..
												      type_real ..
												      type_real ..
												      type_real ..
												      type_real ..
												      type_real ..
												      type_string ..
												      type_string ..
												      type_string ..
												      type_string ..
												      type_string ..
												      type_string];											

  str = convstr(PropertyName);
  k=find(part(TableProperties,1:length(str))==str);
  
  if (k == []) then
    TypeReturn=-1;
    return;
  end                           						
  TypeReturn=TablePropertiesValueType(k);
endfunction
// -----------------------------------------------------------
