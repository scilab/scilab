// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2012 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

function demo_param()

    lines(0);

    messagebox(_("This demo shows examples of plist utilizations: creation, insertion, removal, ..."), "modal", "info");

    printf(_("\nCreate a plist ''%s'' containing two fields ''min'' = [0 0 0] and ''max'' = [1 1 1].\n"), "test_list");

    test_list = init_param();

    test_list = add_param(test_list, "min");
    test_list = add_param(test_list, "max");
    disp(list_param(test_list));

    printf(_("\nCheck fields: \n"));
    printf("  is_param(test_list, ''min'') = %f\n", 1.0*is_param(test_list, "min"));
    printf("  is_param(test_list, ''max'') = %f\n", 1.0*is_param(test_list, "max"));
    printf("  is_param(test_list, ''mex'') = %f\n\n", 1.0*is_param(test_list, "mex"));

    printf(_("Set ''min'' = [0 0 0] and ''max'' = [1 1 1]:\n"));
    printf("  set_param(test_list, ''min'', [0 0 0]);\n");
    printf("  set_param(test_list, ''max'', [1 1 1]);\n\n");
    test_list = set_param(test_list, "min", [0 0 0]);
    test_list = set_param(test_list, "max", [1 1 1]);

    printf(_("Check values: \n"));
    printf("  get_param(test_list, ''min'') = "); disp(get_param(test_list, "min"));
    printf("  get_param(test_list, ''max'') = "); disp(get_param(test_list, "max"));
    printf("  get_param(test_list, ''mex'') = "); disp(get_param(test_list, "mex"));

    printf(_("\nRemove the parameter ''min'' from %s.\n"), "test_list");
    printf(_("\nBefore remove_param: \n"));
    disp(list_param(test_list));

    printf("\n  test_list = remove_param(test_list, ''min'');\n")
    test_list = remove_param(test_list, "min");

    printf(_("\nAfter remove_param: \n"));
    disp(list_param(test_list));

    printf(_("\nCheck removal: \n"));
    printf("  get_param(test_list, ''min'') = ");
    disp(get_param(test_list, "min"));

    printf(_("\nCheck type: \n"));
    printf("  typeof(test_list) = %s\n\n", typeof(test_list));

endfunction

demo_param();
clear demo_param;
