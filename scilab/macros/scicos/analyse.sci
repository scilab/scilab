function syst=analyse(scs_m)
// Analyse le graphique pour rechercher les connections
//given	a block-diagram	representation of a linear system bloc2exp returns
//  its symbolic evaluation.  The	first element of the list blocd	must be	the
//  string 'blocd'.  Each	other element of this list (blocd(2),blocd(3),...) is
//  itself a list	of one the following types :

// Copyright INRIA

//  list('transfer','name_of_linear_system')


//  list('link','name_of_link',
//		 [number_of_upstream_box,upstream_box_port],
//		 [downstream_box_1,downstream_box_1_portnumber],
//		 [downstream_box_2,downstream_box_2_portnumber],
//		 ...)
//  The strings 'transfer' and 'links' are keywords which	indicate the type of
//  element in the block diagram.
nx=size(scs_m)
nb=0
syst=list()
corresp=0*ones(nx,1)
links=[]
for k=2:nx
  o=scs_m(k)
  if o(1)=='Block' then
    model=o(3)
    if model(1)=='super'|model(1)=='csuper' then
      nb=nb+1
      syst(nb)=analyse(model)
      corresp(k)=nb
    else
      nb=nb+1
      select o(5)
      case 'GAIN_f' then
        syst(nb)=list('transfer',model)
        corresp(k)=nb
      case 'ECLAT_f' then
        syst(nb)=list('transfer',ones(3,1))
        corresp(k)=nb
      case 'FILTRE_f' then
        syst(nb)=list('transfer',model)
        corresp(k)=nb
      case 'SOM_f' then
        syst(nb)=list('transfer',model)
        corresp(k)=nb
      case 'FORMEL_f' then
         syst(nb)=list('transfer',model)
         corresp(k)=nb
       case 'IN_f' then
         nb=nb-1
       case 'OUT_f' then
         nb=nb-1
      else
        message('Non linear systems are not implemented yet')
        syst(nb)=list('transfer',o(5))
        corresp(k)=nb
      end
    end
  else
    links=[links k]
  end
end
for k=links
  o=scs_m(k)
  nb=nb+1
  [from,to]=o(8:9)
  o1=scs_m(from(1))
  o2=scs_m(to(1))
  from(1)=corresp(from(1))+1
  to(1)=corresp(to(1))+1
  if o1(4)=='IN_f' then
    syst(nb)=list('link','l'+string(nb),-1,to)
  elseif o2(4)=='OUT_f' then
    syst(nb)=list('link','l'+string(nb),from,-1)
  else
    syst(nb)=list('link','l'+string(nb),from,to)
  end
end
syst(0)='blocd'






