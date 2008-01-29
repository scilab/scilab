//==========================================
// INRIA 2008
// Allan CORNET
//==========================================
function r = with_lcc()

 if MSDOS then // Windows
    // Visual Studio C++ 
    if ( findmsvccompiler() <> 'unknown' ) then 
      r = %F;
    else
      // LCC-WIN32
      if ( findlcccompiler() == %T ) then
        r = %T;
      else
      // another compiler
        r = %F;
      end
    end
  
  else // LINUX
    r = %F;
  end

endfunction
//==========================================