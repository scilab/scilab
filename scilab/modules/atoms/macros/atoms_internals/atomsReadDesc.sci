// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Internal function

// Parsing of the DESCRIPTION file

// =============================================================================
// description_out = atomsReadDesc(file_in,description_in)
// Parse a DESCRIPTION file
//
// Date : 20/03/2009
// =============================================================================

function description_out = atomsReadDesc(file_in,description_in)

    description_out = struct();

    rhs  = argn(2);

    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsReadDesc",1,2));
    end

    if regexp( file_in,"/DESCRIPTION$/") == [] then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: String that ends with DESCRIPTION expected.\n"),"atomsReadDesc",1));
    end

    if rhs==2 & type(description_in)<>17 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: mlist expected.\n"),"atomsReadDesc",2));
    end

    if rhs==2 then
        description_out = description_in;
    end

    tabDesc         = mgetl(file_in);
    current_field   = "";

    for i=1:size(tabDesc,"*")

        // First case : new field
        if regexp(tabDesc(i),"/^[a-zA-Z0-9]*:\s/","o") == 1 then
            current_field_length    = regexp(tabDesc(i),"/:\s/","o")
            current_field           = part(tabDesc(i),1:current_field_length-1);
            current_value           = part(tabDesc(i),current_field_length+2:length(tabDesc(i)));
            description_out(current_field) = current_value;
            continue;
        end

        // Second case : Current field continuation
        if regexp(tabDesc(i),"/^\s/","o") == 1 then
            current_value = part(tabDesc(i),2:length(tabDesc(i)));
            description_out(current_field) = [ description_out(current_field) ; current_value ];
            continue;
        end

        // Third case : Blank line
        if length(tabDesc(i)) == 0 then
            continue;
        end

        // Else Error
        error(msprintf(gettext("%s: The description is not well formated at line %d\n"),"atomsReadDesc",i));

    end

endfunction
