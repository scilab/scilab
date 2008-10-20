function r=%ip_m_s(ip,s)
// ip*s
if size(s,'*')<>1 then error(10),end
r=(ip(1)*s):(ip(2)*s):(ip(3)*s)
endfunction
