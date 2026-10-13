//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Clement David
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// <-- NO CHECK REF -->

path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "SCADE_CruiseControl__CruiseControl_FMU.fmu"));

// direct FMI call
library = fmu_link(fmu.libraryFile, fmu.descriptionFMU.modelIdentifier);

instance = fmu_call(library, "fmiInstantiateModel", fmu.descriptionFMU.modelIdentifier, fmu.descriptionFMU.guid, %f);

// define variables with references value
refs = [fmu.modelVariables.Real.valueReference, fmu.modelVariables.Integer.valueReference, fmu.modelVariables.Boolean.valueReference, fmu.modelVariables.String.valueReference];
names = [fmu.modelVariables.Real.name,fmu.modelVariables.Integer.name,fmu.modelVariables.Boolean.name,fmu.modelVariables.String.name];
execstr(names + "=" + string(refs) + ";");

// utility function to check a function with refs and values
function status = fmiSetters(library, fun, instance, refs, values)
	status = list();
	status($+1) = fmu_call(library, fun, instance, refs, values);
//	status($+1) = fmu_call(library, fun, instance, refs, length(refs), values);
	status($+1) = fmu_call(library, fun, instance, uint32(refs), values);
//	status($+1) = fmu_call(library, fun, instance, uint32(refs), length(refs), values);
//	status($+1) = fmu_call(library, fun, instance, uint32(refs), uint32(length(refs)), values);
//	status($+1) = fmu_call(library, fun, instance, int32(refs), values);
//	status($+1) = fmu_call(library, fun, instance, int32(refs), length(refs), values);
//	status($+1) = fmu_call(library, fun, instance, int32(refs), int32(length(refs)), values);
endfunction

function [status, values] = fmiGetters(library, fun, instance, refs)
	status = list();
	values = list();
	[status($+1),values($+1)] = fmu_call(library, fun, instance, refs);
//	[status($+1),values($+1)] = fmu_call(library, fun, instance, refs, length(refs));
	[status($+1),values($+1)] = fmu_call(library, fun, instance, uint32(refs));
//	[status($+1),values($+1)] = fmu_call(library, fun, instance, uint32(refs), length(refs));
//	[status($+1),values($+1)] = fmu_call(library, fun, instance, uint32(refs), uint32(length(refs)));
//	[status($+1),values($+1)] = fmu_call(library, fun, instance, int32(refs));
//	[status($+1),values($+1)] = fmu_call(library, fun, instance, int32(refs), length(refs));
//	[status($+1),values($+1)] = fmu_call(library, fun, instance, int32(refs), int32(length(refs)));
endfunction

// fmiSetReal
refs = Speed;
values = 20;
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
status = fmiSetters(library, "fmiSetReal", instance, refs, values);
assert_checkequal(status, status_ref);

refs = [Speed Accel];
values = [20 0];
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
status = fmiSetters(library, "fmiSetReal", instance, refs, values);
assert_checkequal(status, status_ref);
refs = [Speed ; Accel];
values = [20 ; 0];
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
status = fmiSetters(library, "fmiSetReal", instance, refs, values);
assert_checkequal(status, status_ref);

// fmiSetInteger
refs = [CruiseState];
values = 1;
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
status = fmiSetters(library, "fmiSetInteger", instance, refs, values);
assert_checkequal(status, status_ref);

refs = [CruiseState];
values = int32(1);
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
status = fmiSetters(library, "fmiSetInteger", instance, refs, values);
assert_checkequal(status, status_ref);

// fmiSetBoolean
refs = [On];
values = %t;
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
status = fmiSetters(library, "fmiSetBoolean", instance, refs, values);
assert_checkequal(status, status_ref);

refs = [On Off];
values = [%t %f];
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
status = fmiSetters(library, "fmiSetBoolean", instance, refs, values);
assert_checkequal(status, status_ref);

refs = [On ; Off];
values = [%t ; %f];
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
status = fmiSetters(library, "fmiSetBoolean", instance, refs, values);
assert_checkequal(status, status_ref);

// fmiGetReal
refs = Speed;
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
values_ref = list();
for i=1:2, values_ref($+1) = 20; end
[status, values] = fmiGetters(library, "fmiGetReal", instance, refs);
assert_checkequal(status, status_ref);
assert_checkequal(values, values_ref);

refs = [Speed Accel];
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
values_ref = list();
for i=1:2, values_ref($+1) = [20 0]; end
[status, values] = fmiGetters(library, "fmiGetReal", instance, refs);
assert_checkequal(status, status_ref);
assert_checkequal(values, values_ref);

refs = [Speed ; Accel];
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
values_ref = list();
for i=1:2, values_ref($+1) = [20 ; 0]; end
[status, values] = fmiGetters(library, "fmiGetReal", instance, refs);
assert_checkequal(status, status_ref);
assert_checkequal(values, values_ref);

// fmiGetInteger
refs = CruiseState;
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
values_ref = list();
for i=1:2, values_ref($+1) = int32(1); end
[status, values] = fmiGetters(library, "fmiGetInteger", instance, refs);
assert_checkequal(status, status_ref);
assert_checkequal(values, values_ref);

// fmiGetBoolean
refs = [On];
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
values_ref = list();
for i=1:2, values_ref($+1) = %t; end
[status, values] = fmiGetters(library, "fmiGetBoolean", instance, refs);
assert_checkequal(status, status_ref);
assert_checkequal(values, values_ref);

refs = [On Off];
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
values_ref = list();
for i=1:2, values_ref($+1) = [%t %f]; end
[status, values] = fmiGetters(library, "fmiGetBoolean", instance, refs);
assert_checkequal(status, status_ref);
assert_checkequal(values, values_ref);

refs = [On ; Off];
status_ref = list();
for i=1:2, status_ref($+1) = 0; end
values_ref = list();
for i=1:2, values_ref($+1) = [%t ; %f]; end
[status, values] = fmiGetters(library, "fmiGetBoolean", instance, refs);
assert_checkequal(status, status_ref);
assert_checkequal(values, values_ref);

fmu_call(library, "fmiTerminate", instance);
fmu_ulink(library);
