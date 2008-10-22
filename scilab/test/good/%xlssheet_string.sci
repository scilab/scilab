function T=%xlssheet_string(sheet)
  //display a xls sheet
  T=sheet.text
  V=sheet.value
  k=find(~isnan(V))
  T(k)=string(V(k))
endfunction
