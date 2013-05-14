// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 10393 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10393
//
// <-- Short Description -->
// Add palette after diagram loading, does not invalidate the styles

// start xcos, and create an empty test file
// accept to create this file on first call
xcos(TMPDIR + "/../bug_10393.zcos");

// define a function to load the test palette
function loadPalette()
    
    //Create a simple custom block.
    function [x, y, typ]=MyBlock(job, arg1, arg2)
        x=[];y=[];typ=[];
        select job
        case 'set'
            x=arg1;
            graphics=arg1.graphics;exprs=graphics.exprs
            model=arg1.model;
            while %t do
                [ok,slope,stt,iout,exprs]=scicos_getvalue([msprintf(gettext("Set %s block parameters"), "RAMP"); " "; gettext("Ramp function");" "], ..
                [gettext("Slope"); gettext("Start Time"); gettext("Initial Value")], ..
                list('vec',1,'vec',1,'vec',1), exprs)
                if ~ok then break,end
                if stt<0  then
                    block_parameter_error(msprintf(gettext("Wrong value for ''Start Time'' parameter: %e."), stt), ..
                    gettext("Null or positive integer expected."));
                else
                    model.rpar=[slope;stt;iout];
                    graphics.exprs=exprs
                    x.graphics=graphics;x.model=model
                    break
                end
            end
        case 'define'
            slope=0;iout=0;stt=0;rpar=[slope;stt;iout];
            model=scicos_model()
            model.sim=list('ramp',4)
            model.in=[]
            model.out=1
            model.rpar=rpar
            model.blocktype='c'
            model.nmode=1
            model.nzcross=1
            model.dep_ut=[%f %t]
    
            exprs=[string(rpar)]
            x=standard_define([20 20],model,exprs,[])
        end
    endfunction
    
    //create an instance of a new block
    o = MyBlock("define");
    
    //create a new palette
    pal = xcosPal("My palette");
    
    // customize the style
    style = struct();
    // inherit all from the default block implementation
    style.blockWithLabel = [];
    style.fillColor = "red";
    style.image = "file://" + SCI + "/modules/xcos/images/blocks/RAMP.svg"
    style.displayedLabel = "Parameters : %s <br> %3$s %2$s";
    // format string is a documented there : http://download.oracle.com/javase/6/docs/api/java/util/Formatter.html#syntax
    style.verticalLabelPosition = "middle"
    style.verticalAlign = "top"
    // documentation is into xcosPalAddBlock
    
    //add block to this palette using e.g. RAMP icon
    pal = xcosPalAddBlock(pal, o, "SCI\modules\xcos\images\palettes\RAMP.png", style);
    
    //deploy palette
    xcosPalAdd(pal);
endfunction

// ---- INTERACTIVE PART ----

loadPalette();
// drag the new block into the schema
// save it
// close scilab (without closing xcos directly)

// restart
// check that the xcos diagram is restored
// check the block style is not defined
// loadPalette();
// check the block style is defined
