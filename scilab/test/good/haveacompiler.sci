//-----------------------------------------------------------------------------
// Allan CORNET
// INRIA 2005
//-----------------------------------------------------------------------------
function bOK=haveacompiler()
if MSDOS then
  global LCC
  findLCC=LCC;
  clear LCC
  if (findLCC) then
    bOK=%T
  else
    msvc=findmsvccompiler();
    select msvc,
      case 'msvc80pro' then
    	  bOK=%T
    	case 'msvc80std' then
    	  bOK=%T
    	case 'msvc80express' then
    	  bOK=%T
      case 'msvc71' then
        bOK=%T
      case 'msvc70' then
        bOK=%T
      case 'msvc60' then
        bOK=%T
      case 'msvc50' then
        bOK=%T
      else
        bOK=%F;
     end
  end
else
  // To do under Unix
  bOK=%T;
end
endfunction
//-----------------------------------------------------------------------------
