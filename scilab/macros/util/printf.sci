function printf(frmt,varargin)
// printf - Emulator of C language printf
//!
// Copyright INRIA
write(%io(2),sprintf(frmt,varargin(:)),'(a)')

