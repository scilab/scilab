#ifndef fmiPlatformTypes_h
#define fmiPlatformTypes_h

/*
This header file defines the data types of FMI 1.0.
It must be used by both FMU and FMI master.

Copyright (C) 2008-2011 MODELISAR consortium,
              2012-2022 Modelica Association Project "FMI"
              All rights reserved.

This file is licensed by the copyright holders under the 2-Clause BSD License
(https://opensource.org/licenses/BSD-2-Clause):

----------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------------
*/

/* Include the integer and boolean type definitions */
#include <stdint.h>
#include <stdbool.h>


/* tag::Component[] */
typedef           void* fmiComponent;             /* Pointer to the FMU instance */
/* end::Component[] */

/* tag::ValueReference[] */
typedef        uint32_t fmiValueReference;       /* Handle to the value of a variable */
/* end::ValueReference[] */

/* tag::fmiFloat64[] */
typedef          double fmiReal;  /* Double precision floating point (64-bit) */
/* end::fmiFloat64[] */
typedef         int fmiInteger;    /* 32-bit signed integer */
typedef            char fmiBoolean;  /* Data type to be used with fmiTrue and fmiFalse */
typedef            char fmiChar;     /* Data type for one character */
typedef fmiChar* fmiString;   /* Data type for character strings
                                         ('\0' terminated, UTF-8 encoded) */

/* Values for fmiBoolean */
#define fmiTrue  true
#define fmiFalse false

/* Undefined value for fmiValueReference (largest unsigned int value) */
#define fmiUndefinedValueReference (fmiValueReference)(-1)


#endif /* fmiPlatformTypes_h */
