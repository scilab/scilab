function buf=sprintf(frmt,varargin)
// sprintf - Emulator of C language sprintf
//!
// Copyright INRIA
buf=msprintf(frmt,varargin(:))
endfunction
