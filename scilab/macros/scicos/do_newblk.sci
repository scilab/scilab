function txt=do_newblk(o,nam)
//create text of GUI function associated with a a block
// o     : block data structure
// nam   : name of the created GUI function
//!

// Copyright INRIA
for  k=5:8,o(2)(k)=0*o(2)(k),end // disconnect links
o(2)(1)=0*o(2)(1) // set origin to zero
o(2)(2)=o(2)(2)/20 // set definition size ?????
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
