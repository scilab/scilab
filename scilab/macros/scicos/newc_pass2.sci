function cpr=newc_pass2(bllst,connectmat,clkconnect,cor,corinv)
// cor    ; correspondance table with initial block ordering
//
// bllst: list with nblk elts where nblk denotes number of blocks.
//        Each element must be a list with 12 elements:
//          1- function name (in string form if fortran routine) 
//          2- vector of number of inputs
//          3- vector of number of ouputs
//          4- vector of number of clock inputs 
//          5- vector of number of clock outputs
//          6- vector (column) of continuous initial condition
//          7- vector (column) of discrete initial condition
//          8- vector (column) of real parameters
//          9- vector (column) of integer parameters
//          10- string: 'z' if zero-crossing,
//                      'l' logical
//          11- vector of size <number of clock outputs> including
//              preprogrammed event firing times (<0 if no firing) 
//              or [for backward compatibility]
//              boolean vector: i-th entry %t if initially output is fired
//          12- boolean vector (1x2): 1st entry for dependence on u, 2nd on t 
//
// connectmat: nx4 matrix. Each row contains, in order, the block
//             number and the port number of an outgoing scicopath,
//             and the block number and the port number of the target
//             ingoing scicopath.
//
// clkconnect: same as connectmat but for clock scicopaths.
//
// define some constants
// Copyright INRIA

show_trace=%f
if exists('%scicos_solver')==0 then %scicos_solver=0,end
[state,sim,funs,%scicos_solver,corinv]=scicos_cpass2(bllst,connectmat,clkconnect,corinv,%scicos_solver);
if show_trace then disp('c_pass2:'+string(timer())),end
if (type(sim) == 1) then 
  cpr=list();
else
 
  sim.funs=funs;

  for i=1:sim.nb

    if sim.funs(i)=='sciblock' then
      sim.funs(i)=genmac(bllst(i).ipar,size(bllst(i).in,'*'),..
				size(bllst(i).out,'*'));
      
    end
  end

 cpr=scicos_cpr(state=state,sim=sim,cor=cor,corinv=corinv);
end
endfunction
