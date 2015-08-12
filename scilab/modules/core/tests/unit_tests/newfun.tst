// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

fptr_cos = funptr("cos");
newfun("cosAlias",fptr_cos);

if cos(2*%pi) <> cosAlias(2*%pi)                then pause,end
if clearfun("cosAlias") <> %T                   then pause,end
if execstr("cosAlias(2*%pi)","errcatch") ==  0  then pause,end

ierr = execstr('newfun(''more_than_twenty_four_characters_function_name'',fptr_cos)','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('newfun(''1_function_name'',fptr_cos)','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('newfun(''new_function_name'',[fptr_cos,fptr_cos])','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('newfun([''new_function_name'',''new_function_name''],[fptr_cos,fptr_cos])','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('newfun(1,[fptr_cos,fptr_cos])','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('newfun(1,''fptr_cos'')','errcatch');
if ierr <> 999 then pause,end
