class Switch
  parameter Real Ron=0.01 "Resistance when the Switch is closed";
  parameter Real Roff=1e5  "Resistance when the switch is open";
  Pin p,n;
  Real inp,Rx;
equation
  Rx*p.i= p.v - n.v;
  p.i = -n.i;
  Rx=if inp >0 then Ron	
              else Roff;
end Switch;
