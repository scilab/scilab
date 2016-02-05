// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function demo_fmin_tolx()

    mprintf(_("Running optimization ...\n"));

    function y = banana (x)
        y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
    endfunction

    x0 = [-1.2 1]
    opt = optimset ( "TolX" , 1.e-2 );
    [x , fval , exitflag , output] = fminsearch ( banana , x0 , opt );

    //
    // Display results
    //
    mprintf("x = %s\n",strcat(string(x)," "));
    mprintf("fval = %e\n",fval);
    mprintf("exitflag = %d\n",exitflag);
    mprintf("output.message:\n");

    for i =1:3
        mprintf(output.message(i));
        mprintf("\n");
    end

    mprintf("output.algorithm:%s\n",output.algorithm);
    mprintf("output.funcCount:%d\n",output.funcCount);
    mprintf("output.iterations:%d\n",output.iterations);

    //
    // Load this script into the editor
    //
    m = messagebox(_("View Code?"), "Question", "question", [_("Yes") _("No")], "modal")
    if(m == 1)
        filename = "fminsearch_tolx.sce";
        dname = get_absolute_file_path(filename);
        editor ( dname + filename, "readonly" );
    end
endfunction

demo_fmin_tolx();
clear demo_fmin_tolx;








