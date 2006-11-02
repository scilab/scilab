function [rep]=x_choices(Title,choices_l)
// Copyright INRIA
[lhs,rhs]=argn(0)
	if rhs<=0 then s_mat=['l1=list(''choice 1'',1,[''toggle c1'',''toggle c2'',''toggle c3'']);';
		 'l2=list(''choice 2'',2,[''toggle d1'',''toggle d2'',''toggle d3'']);';
		 'l3=list(''choice 3'',3,[''toggle e1'',''toggle e2'']);';
                 'rep=x_choices(''Toggle Menu'',list(l1,l2,l3));'];
         write(%io(2),s_mat);execstr(s_mat);
         return;end;

if typeof(Title)<>'string' then 
	write(%io(2),'x_choices first argument is not character string')
	return
end
if typeof(choices_l)<>'list' then 
	write(%io(2),'x_choices argument is not a list')
	return
end
n=size(choices_l)
items=['void']
defv=[]
for i=1:n, l_ch=choices_l(i);
 if typeof(l_ch)<>'list' then 
	write(%io(2),'x_choices(t,x): x('+string(i)+') is not a list');
	return
 end 
 if typeof(l_ch(1))<>'string' then 
	write(%io(2),'x_choices(t,x): x('+string(i)+')(1) is not a string');
	return	
 end
 items= [items, l_ch(1)];
 if typeof(l_ch(3))<>'string' then 
	write(%io(2),'x_choices(t,x): x('+string(i)+')(3) is not vector of strings');
	return	
 end
 [xxxl,xxxc]=size(l_ch(3));
 if xxxl<>1 then 
	write(%io(2),'x_choices(t,x): x('+string(i)+')(3) must be a row vector of strings');
	return	
 end 
 items= [items, l_ch(3)];
 if typeof(l_ch(2))<>'constant' then 
	write(%io(2),'x_choices(t,x): x('+string(i)+')(2) is not of type int');
	return	
 end
 if prod(size(l_ch(2)))<>1 then 
	write(%io(2),'x_choices(t,x): x('+string(i)+')(2) must be an integer');
	return	
 end
 defv=[defv,l_ch(2)];
 if n<>i then items=[items,"[--sep--]"];end
end 
items=items(2:prod(size(items)))
rep=xchoicesi(defv,Title,items)
endfunction
