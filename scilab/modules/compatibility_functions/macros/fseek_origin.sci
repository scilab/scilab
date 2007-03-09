function flag=fseek_origin(origin)
// Copyright INRIA
// S.S. V.C.

select  origin
case "bof" then
  flag="set"
case "cof" then
  flag="cur"
case "eof" then
  flag="end"
case -1 then
  flag="set"
case 0 then
  flag="cur"
case 1 then
  flag="end" 
else
  error(gettext("errors","compatibility_functions_error_8"));
end
endfunction
