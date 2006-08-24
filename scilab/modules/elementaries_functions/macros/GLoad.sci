function GLoad(name)
// Copyright INRIA
name1=name;
k=strindex(name,'.');
if k<>[] then 
	name1=part(name1,k($)+1:length(name1));
else
	break;
end
select name1
	case "sci" then getf(name), 
	case "sce" then exec(name),
	case "scg" then xload(name),
	case "bin" then load(name), 
	else error("Gload : unknown suffix in file name "+name);
end

	
endfunction
