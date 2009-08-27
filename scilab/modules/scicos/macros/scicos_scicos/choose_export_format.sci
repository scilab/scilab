function exp_format=choose_export_format()
// Ask for graphic export format
  if getos()=='Windows' then emf='emf',else emf=[],end
  available_formats = ["bmp"; emf;"eps";"fig";"gif";"jpg";"pdf";
		    "png";"ppm";"ps";"svg";"scg" ]
     
      
  exp_format = x_choose(available_formats, _("Choose export format:"))
  if ~exp_format then
    // User cancelled
    exp_format=[]
  end
  exp_format = available_formats(exp_format)
endfunction
