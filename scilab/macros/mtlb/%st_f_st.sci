function R=%st_f_st(M1,M2)
// Copyright INRIA
//R=[M1' M2']'
  R=%st_t(%st_c_st(%st_t(M1),%st_t(M2)))
endfunction
