function [v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,..
v17,v18,v19,v20,v21,v22,v23,v24,v25,v26,v27,v28,v29,v30]= fscanf(fil,frmt)
// Copyright INRIA

// fscanf - Emulator of C language fscanf
//!
[lhs,rhs]=argn(0)
v='v'
args=strcat(v(ones(lhs,1))+string(1:lhs)',',')
buf=read(fil,1,1,'(a)')
execstr('['+args+']=sscanf(buf,frmt)')
endfunction
