function [x,y,typ]=standard_block(job,arg1,arg2)
//%Description
// job=='plot' :      block drawing
//                    arg1 is block data structure
//                    arg2 :unused
// job=='getinputs' : return position and type of inputs ports
//                    arg1 is block data structure
//                    x  : x coordinates of ports
//                    x  : y coordinates of ports
//                    typ: type of ports
// job=='getoutputs' : return position and type of outputs ports
//                    arg1 is block data structure
//                    x  : x coordinates of ports
//                    x  : y coordinates of ports
//                    typ: type of ports
// job=='getorigin'  : return block origin coordinates
//                    x  : x coordinates of block origin
//                    x  : y coordinates of block origin
// job=='set'        : block parameters acquisition
//                    arg1 is block data structure
//                    x is returned block data structure
// job=='define'     : corresponding block data structure initialisation
//                    arg1: name of block parameters acquisition macro (init)
//                    x   : block data structure
//%Block data-structure definition
// bl=list('Block',graphics,model,init,'standard_block')
//  graphics=list([xo,yo],[l,h],orient,label)
//          xo          - x coordinate of block origin
//          yo          - y coordinate of block origin
//          l           - block width
//          h           - block height
//          orient      - boolean, specifies if block is tilded
//          label       - string block label
//  model=list(eqns,#input,#output,#clk_input,#clk_output,state,rpar,ipar,..
//            typ,firing,input_time_dep)
//          eqns        - function name (in string form if fortran routine)
//          #input      - number of inputs
//          #output     - number of ouputs
//          #clk_input  - number of clock inputs
//          #clk_output - number of clock outputs
//          state       - vector (column) of initial condition
//          rpar        - vector (column) of real parameters
//          ipar        - vector (column) of integer parameters
//          typ         - string: 'c' if block is continuous, 'd' if discrete
//                        'z' if zero-crossing.
//
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  execstr('[model,label,ok]='+arg1(5)+'()')
  if ok then
    graphics=arg1(2);graphics(4)=label;
    x=list('Block',graphics,model,arg1(4),arg1(5))
  end
case 'define' then
  model=list(arg1,1,1,[],[],[],[],[],[],%f,[%f %f],' ',list())
  x=standard_define([2 2],model,[],[])
end


