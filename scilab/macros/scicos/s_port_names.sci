function s_port_names(sbloc)
// Copyright INRIA
IDENTIFICATION=15
graphics=sbloc(2);
model=sbloc(3);


x=model(8)
etiquettes_in = []
etiquettes_out = []
etiquettes_clkin = []
etiquettes_clkout = []
font=xget('font')

xset('font',options('ID')(2)(1),options('ID')(2)(2))
inp=[],outp=[],cinp=[],coutp=[]
for k=2:size(x)
  o=x(k);
  if o(1)=='Block' then
    modelb=o(3);
    if size(modelb) >= IDENTIFICATION then
      ident = modelb(IDENTIFICATION);
    else
      ident = emptystr();
    end
    if ident<>emptystr()&ident<>[] then
      select o(5)
      case 'IN_f' then
	inp=[inp modelb(9)];
	etiquettes_in = [etiquettes_in; ident];
      case 'OUT_f' then
	outp=[outp modelb(9)];
	etiquettes_out = [etiquettes_out; ident];
      case 'CLKIN_f' then
	cinp=[cinp modelb(9)];
	etiquettes_clkin = [etiquettes_clkin; ident];
      case 'CLKINV_f' then
	cinp=[cinp modelb(9)];
	etiquettes_clkin = [etiquettes_clkin; ident];
      case 'CLKOUT_f' then
	coutp=[coutp modelb(9)];
	etiquettes_clkout = [etiquettes_clkout; ident];
      case 'CLKOUTV_f' then
	coutp=[coutp modelb(9)];
	etiquettes_clkout = [etiquettes_clkout; ident];
      end
    end
  end
end
if inp<>[] then
  [tmp,n_in]=sort(-inp)
  standard_etiquette(sbloc, etiquettes_in(n_in), 'in')
end
if outp<>[] then
  [tmp,n_out]=sort(-outp)
  standard_etiquette(sbloc, etiquettes_out(n_out), 'out')
end
if cinp<>[] then
  [tmp,n_cin]=sort(-cinp)
  standard_etiquette(sbloc, etiquettes_clkin(n_cin), 'clkin')
end
if coutp<>[] then
  [tmp,n_cout]=sort(-coutp)
  standard_etiquette(sbloc, etiquettes_clkout(n_cout), 'clkout')
end
xset('font',font(1),font(2))
