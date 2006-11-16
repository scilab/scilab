//
//  -*- Scilab -*-
//
// sci2c.sci
// Made by  Bruno JOFRET <bruno.jofret@inria.fr>
//
// Started on  Thu Nov 16 16:32:54 2006 jofret
// Last update Thu Nov 16 16:54:10 2006 jofret
//
// Copyright INRIA 2006
//

function res=sci2c(sciFile)



  res=0
// Get default arguments
  [lhs,rhs]=argn(0)

  if rhs < 1 then
    disp("Usage : sci2c(<file_name>)")
    return
  end

  disp("Begin of a Scilab 2 C Tool...")
  disp("Need a better name for further sexy behaviour...")
  return


endfunction