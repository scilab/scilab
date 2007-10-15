function txt=do_newblk(o,nam)
// Copyright INRIA
//create text of GUI function associated with a a block
// o     : block data structure
// nam   : name of the created GUI function

//for  k=5:8,o(2)(k)=0*o(2)(k),end // disconnect links
o.graphics.pin=0*o.graphics.pin;o.graphics.pout=0*o.graphics.pout;
o.graphics.pein=0*o.graphics.pein;;o.graphics.peout=0*o.graphics.peout;
//o(2)(1)=0*o(2)(1) // set origin to zero
o.graphics.orig=0*o.graphics.orig // set origin to zero
//o(2)(2)=o(2)(2)/20 // set definition size ?????
o.graphics.sz=o.graphics.sz/20 // set definition size ?????
// form text of the macro
nam=stripblanks(nam)
txt=[
    'function [x,y,typ]='+nam+'(job,arg1,arg2)';
    'x=[];y=[],typ=[]';
    'select job';
    'case ''plot'' then';
    '  standard_draw(arg1)';
    'case ''getinputs'' then';
    '  [x,y,typ]=standard_inputs(arg1)';
    'case ''getoutputs'' then';
    '  [x,y,typ]=standard_outputs(arg1)';
    'case ''getorigin'' then';
    '  [x,y]=standard_origin(arg1)';
    'case ''set'' then';
    'case ''define'' then';
    sci2exp(o,'x');
    'end']
endfunction
