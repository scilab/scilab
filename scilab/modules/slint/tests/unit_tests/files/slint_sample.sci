// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [a,r] = slint_sample(a,b, a)
    global param  // 1: global keyword
    sin(abc)      // 3: uninitialized abc not in this scope may have side effects
    // 3wrongname = 3 // invalid variable name => parse error => stops code-checking
    // 5: Duplicated function arguments: a.            (reported after line #1)
    // 5: Function arguments used as output values: a. (reported after line #1)
    // 5: A function argument must be preceeded by a single space (a,b, a)(reported after line #1)
    // 7: function returned value might be unused: r   (reported after line #1)
    a = 1; b = 2; // 9: Two instructions on the same line
    select a
    case "a"    // 10: empty block
    case "b"
        b = 2;
    else
        b = 1;
    end
    mclose("all");  // 11: The instruction mclose("all") may have any side effects.
                 // 12: McCabe code complexity
    a = 1d3      // 13: Bad decimal exponent: "e" or "E"  instead of "d" or "D"
    printf("%s %3.2f %d", 3, "abc")           // 14: too few data wrt expected ones
    printf("%s %3.2f %d", 3, "abc", %pi, %t)  // 14: too many data wrt expected ones
    // 15: Maximum line length exceeded at lines #, #, .. (reported after line #1)
    // TODO: check codechecker codes // 17: TODO exists
    a > %nan      // 18: Comparison with NaN
    b == (a + 1)  // 19: == instead of =
    b - 2         // 20: Possible useless operation: Unassigned result
    for i = 1:3
        break
        b = 3   // 21: Unreachable code => !undetected: BUG
        return
        c = 3   // 21: Unreachable code => OK
    end
    xpause(1000)  // 22: deprecated function
    select a      // 23: Missing else case
    case "a"
        b = 1;
    case 3
        b = %pi;
    end
    select a     //23: Select with only one case: could be replaced with an if statement.
    case "a"
        b = 2;
    else
        b = 1;
    end
    a = 1:%inf;    // 24: Invalid list, it contains NaN or Inf.
    b = %nan:10;   // 24: Invalid list, it contains NaN or Inf.
    c = 3:(2:5);   // 24: Bad use of ':' operator.
    s = struct("r", %pi, "p")   // 25: missing value in a struct definition
    s = struct("r", %pi, %e)    // 25: missing field name in a struct definition
    save(TMPDIR + "/test.tmp", a)  // 26: variable's name expected instead of variable's handle
    b = @a         // 27: deprecated @ not operator
    a = b+2        // 28: Binary operator should be surrounded by single spaces
    a = linspace(1,3,10);  // 29: A function argument must be preceded by a single space.
    if a == 3 then
        if b == 4
            if c == 5
                if d == 6  // 31: Too many nested blocks: 3 max.
                    a = 1;
                end
            end
        end
    end
    if a | b | c | d | e | f // 32: Number of expressions in condition is limited: 5 max.
        e = %eps;
    end
    c = a & b == d;   // 33: Expression is not bracketed.
    c = ~~b           // 34: double negation
    disp("Hello")     // 35: Illegal call
                      // 36: minimal ratio lines of comments/code. Here all is OK
    slint_sample(b = 10, 3); // 37: Argument b declared at position 2 and assigned at position 1.
    a = mopen("abcd.txt"); // 38: Function mopen requires an error checking just after the call.
    a = %f            // 39: ';' expected at end of line
endfunction
