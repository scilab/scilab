function r=%s_m_i(a,b)

if a==[] then r=[],end
select inttype(b)
case 1 then
  r=int8(a)*b
case 2 then
  r=int16(a)*b
case 4 then
  r=int32(a)*b
end

