// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

A = [	"1.2.3.4" ; ..
		"0.10"    ; ..
		"0.11"    ; ..
		"0.32"    ; ..
		"45"      ; ..
		"0.5"     ; ..
		"3.0.14"  ; ..
		"7"       ; ..
		"5.1"     ; ..
		"5.1.1"   ; ..
		"6"       ; ..
		"0.1"     ];

A_ASC = [	"0.1" ; ..
		"0.5"     ; ..
		"0.10"    ; ..
		"0.11"    ; ..
		"0.32"    ; ..
		"1.2.3.4" ; ..
		"3.0.14"  ; ..
		"5.1"     ; ..
		"5.1.1"   ; ..
		"6"       ; ..
		"7"       ; ..
		"45"      ];

A_DESC = [	"45"  ; ..
		"7"       ; ..
		"6"       ; ..
		"5.1.1"   ; ..
		"5.1"     ; ..
		"3.0.14"  ; ..
		"1.2.3.4" ; ..
		"0.32"    ; ..
		"0.11"    ; ..
		"0.10"    ; ..
		"0.5"     ; ..
		"0.1"     ];

if or( atomsVersionSort(A,"ASC")  <> A_ASC )        then pause, end
if or( atomsVersionSort(A,"DESC") <> A_DESC)        then pause, end
if or( atomsVersionSort(gsort(A),"ASC")  <> A_ASC ) then pause, end
if or( atomsVersionSort(gsort(A),"DESC") <> A_DESC) then pause, end

B = [	"3.1"      ; ..
		"5.2.0"    ; ..
		"4.0"      ; ..
		"6.0"      ; ..
		"5.1.1"    ; ..
		"4.1.2"    ; ..
		"5.0.1"    ; ..
		"4.1.1"    ; ..
		"5.2.1"    ; ..
		"3.1.1"    ; ..
		"3.0"      ; ..
		"5"        ; ..
		"5.1"      ];

B_ASC = [	"3.0"  ; ..
		"3.1"      ; ..
		"3.1.1"    ; ..
		"4.0"      ; ..
		"4.1.1"    ; ..
		"4.1.2"    ; ..
		"5"        ; ..
		"5.0.1"    ; ..
		"5.1"      ; ..
		"5.1.1"    ; ..
		"5.2.0"    ; ..
		"5.2.1"    ; ..
		"6.0"      ];

B_DESC = [	"6.0"  ; ..
		"5.2.1"    ; ..
		"5.2.0"    ; ..
		"5.1.1"    ; ..
		"5.1"      ; ..
		"5.0.1"    ; ..
		"5"        ; ..
		"4.1.2"    ; ..
		"4.1.1"    ; ..
		"4.0"      ; ..
		"3.1.1"    ; ..
		"3.1"      ; ..
		"3.0"      ];

if or( atomsVersionSort(B,"ASC")  <> B_ASC )        then pause, end
if or( atomsVersionSort(B,"DESC") <> B_DESC)        then pause, end
if or( atomsVersionSort(gsort(B),"ASC")  <> B_ASC ) then pause, end
if or( atomsVersionSort(gsort(B),"DESC") <> B_DESC) then pause, end
