function i1=%i_x_i(i1,i2)
  t1=inttype(i1); t2=inttype(i2);
  //logical promotions
  if t1<10 & t2<10 then t=max(t1,t2); end
  if t1>10 & t2>10 then t=max(t1,t2); end
  //more arbitrary promotions
  if t1<10 & t2>10 then t=max(t1+10,t2); end
  if t1>10 & t2<10 then t=max(t1,t2+10); end
  i1=iconvert(i1,t) .* iconvert(i2,t)
endfunction
