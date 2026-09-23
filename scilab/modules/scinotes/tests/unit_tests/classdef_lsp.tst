// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// classdef symbols, folding, colors, completion, and definitions.
//
// Before execution:
// 1. Folding must work for classdef, properties, methods (including the Static
//    blocks), every function, and enumeration. Every matching end must have the
//    keyword style.
// 2. classdef section words (classdef, properties, methods, enumeration) are
//    keywords; the Static attribute is not highlighted as one. Class,
//    constructor and method names use MacroInFile. factor is a parameter, not
//    the existing Scilab macro.
// 3. Ctrl+click MonacoTestColor and both getName occurrences below: they must
//    navigate to their declarations without execution.

classdef MonacoTestColor
    properties
        R = 0
        G = 0
        B = 0
        Name = "black"
    end

    properties (Static)
        DefaultName = "unnamed"
    end

    methods

        function MonacoTestColor(r, g, b, name)
            this.R = r;
            this.G = g;
            this.B = b;
            this.Name = name;
        end

        function name = getName()
            name = this.Name;
        end

        function values = scale(factor)
            values = factor * [this.R, this.G, this.B];
        end
    end

    methods (Static)
        function color = fromGray(v)
            color = MonacoTestColor(v, v, v, MonacoTestColor.DefaultName);
        end
    end

    enumeration
        Red   (255, 0, 0, "red")
        Green (0, 255, 0, "green")
        Blue  (0, 0, 255, "blue")
    end
end
// After executing the file:
// 4. Request completion after "color.": the R/G/B/Name properties, the
//    getName/scale methods and must the static members DefaultName and
//    fromGray be proposed.
// 5. Request completion after "MonacoTestColor.": the Red/Green/Blue enumeration
//    entries and the public static members DefaultName and fromGray must be
//    proposed. The instance properties R/G/B/Name and the instance methods
//    getName/scale must NOT appear.
// 6. Ctrl+click both getName occurrences must still navigate to the method
//    declaration.
// 7. methods is a primitive call and color is an ordinary argument here.
color = MonacoTestColor(12, 34, 56, "custom");
color.getName();
color.getName
MonacoTestColor.Red
MonacoTestColor.fromGray(128)
methods(color)

