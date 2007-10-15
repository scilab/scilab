function full_path=get_subobj_path(path)
// Copyright INRIA
// path      : vector of integer gives the path of the required block from 
//             top level (main diagram) to bottom level (required block). 
//             Each vector entry is a block index in the current level structure.
// full_path : is the  path relative to scs_m  of the required block
//             scs_m.objs(full_path) is the data structure of the required block

  full_path=list('objs',path(1));
  for l=path(2:$),
    full_path($+1)='model';
    full_path($+1)='rpar';
    full_path($+1)='objs';
    full_path($+1)=l;
  end
endfunction
