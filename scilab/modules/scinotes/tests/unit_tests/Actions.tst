// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

f=TMPDIR + "/file_to_correct.sci";
mputl(["a=''abcdef''									";
       "if a==1";
       "   b=1+""abcdef''         ";
       "   			c=2";
       " d=3			";
       "					        if		                 		";
       "a=''abcdef""""""";
       "           end";
       "				end              "], f);

scinotes(f, ["indent" "trailing" "quote"]);

good = ["a=""abcdef""";
        "if a==1";
	"    b=1+""abcdef""";
	"    c=2";
	"    d=3";
	"    if";
	"        a=""abcdef""""""";
	"    end";
	"end"]

corrected = mgetl(f);

if (good <> corrected) then pause, end;