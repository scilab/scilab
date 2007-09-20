function [scicos_ver]=find_scicos_version(scs_m)
// Copyright INRIA
//**
//** find_scicos_version try to retrieve a version of a
//** scs_m
//**
//** 21/08/07: Alan, inital revision
//**

scicos_ver = "scicos2.2"

if type(scs_m)==17 then
  //case for diagrams >= 4.2
  if find(getfield(1,scs_m)=='version')<>[] then
    if scs_m.version<>'' then
      scicos_ver=scs_m.version
    else
      n=size(scs_m.objs);
      for j=1:n //loop on objects
        o=scs_m.objs(j);
        if typeof(o)=='Block' then
          if find(getfield(1,o.model)=='equations')<>[] then
            scicos_ver = "scicos2.7.3"
            break;
          else
            //** the last version supported here is scicos2.7
            //** other tests can be done
            scicos_ver = "scicos2.7"
            break;
          end
        end
      end //** end for
    end
  //case for diagrams < 4.2
  else
    n=size(scs_m.objs);
    for j=1:n //loop on objects
      o=scs_m.objs(j);
      if typeof(o)=='Block' then
        if find(getfield(1,o.model)=='equations')<>[] then
          scicos_ver = "scicos2.7.3"
          break;
        else
          //** the last version supported here is scicos2.7
          //** other tests can be done
          scicos_ver = "scicos2.7"
          break;
        end
      end
    end //** end for
  end
end

endfunction