//** 23 Jun 2006

function [x,y,typ] = SPLIT_f(job,arg1,arg2)

// Copyright INRIA

x=[];y=[],typ=[];

select job
   
   case 'plot' then
   //**--- This is the function that DRAW the object
   //pause ; //** debug
   orig = arg1.graphics.orig ;
   xarc(orig(1), orig(2)+1.0 , 1.0 , 1.0 , 0, 360*64)
    
   case 'getinputs' then
      graphics = arg1.graphics ;
      orig = graphics.orig;
      x = orig(1)
      y = orig(2)
      typ = ones(x)
   
   case 'getoutputs' then
      graphics=arg1.graphics;orig=graphics.orig;
      x=[1 1]*orig(1)
      y=[1 1]*orig(2)
      typ=ones(x)
   
   case 'getorigin' then
      [x,y]=standard_origin(arg1)
   
   case 'set' then
      x=arg1;
  
   case 'define' then
      model=scicos_model()         ;
      model.sim       = 'lsplit'   ;
      model.in        = -1         ;
      model.out       = [-1;-1;-1] ;
      model.blocktype = 'c'        ;
      model.dep_ut    = [%t %f]    ;
      //**
      x=standard_define([1 1]/3,model,[],[])
end

endfunction
