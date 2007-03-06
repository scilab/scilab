// -----------------------------------------------------------
// Allan CORNET
// INRIA 2005
// -----------------------------------------------------------
function SName=GetUiControlStyleName(StyleName)
  TableStyle = ["pushbutton" ..
                "radiobutton" ..
                "checkbox" ..
                "edit" ..
                "text" ..
                "slider" ..
                "frame" ..
                "listbox" ..
               "popupmenu"];
                        
  //conversion to lower format
  str = convstr(StyleName);
  k=find(part(TableStyle,1:length(str))==str);

  if (k == []) then
    disp(gettext('errors','tclsci_error_50'));
    SName='';
    return;
  end

  SName=TableStyle(k);
                        
endfunction
// -----------------------------------------------------------