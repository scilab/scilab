function Version=OS_Version()
// return OS version
// Copyright INRIA
// Allan CORNET 2004
Version="";
[OS,Ver]=getos()
Version=OS+' '+Ver;
endfunction