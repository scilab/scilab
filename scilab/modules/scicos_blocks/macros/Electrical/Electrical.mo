model OutPort
  output Real v;
end OutPort; 

model OutPutPort
    output Real vo;
    Real vi;
   equation
    vi = vo;
end OutPutPort;

model InPutPort
  Real vo;
  input Real vi;
 equation
    vi=vo;
end InPutPort;

connector Pin
	Real v;
	flow Real i;
end Pin;


partial model TwoPin
    Pin p, n;
    Real i, v;
equation
    i = p.i;
    n.i = -i;
    v = p.v - n.v;
end TwoPin;

//---------------------------------------
model Ground "Ground"
	Pin p;
equation
	p.v = 0.0;
end Ground;


model Resistor
    extends TwoPin;
    parameter Real R =1 "Resistance";

equation
	R*i = v;
end Resistor;

model VariableResistor
    extends TwoPin;
   Real R "Resistance";
equation
    R*i = v ;
end VariableResistor;

model Capacitor
    extends TwoPin;
    parameter Real C(fixed=true)=1e-5 "Capacitance";
 equation
    C * der(v) = i;
end Capacitor;

model Inductor "Ideal electrical inductor"
  parameter Real L=1e-6 "Inductance";
  extends TwoPin;
equation
    L * der(i) = v;
end Inductor;

model ConstantVoltage "Source for constant voltage"
   extends TwoPin;
   parameter Real V (fixed=true)=1 "Volts";
equation
   V = v;
end ConstantVoltage;


model VsourceAC "Sin-wave voltage source"
   extends TwoPin;
	parameter Real VA = 220 "Amplitude";
	parameter Real f = 50 "Frequency";
	parameter Real PI=3.141592653589793;
equation
	v = VA* sin(2*PI*f*time);
end VsourceAC;
 

model VVsourceAC "Sin-wave voltage source"
   extends TwoPin;
	Real VA "voltage";
	parameter Real f = 50 "Frequency";
	parameter Real PI=3.141592653589793;
equation
	v = VA*sin(2*PI*f*time);
end VVsourceAC; 

model SineVoltage "Sine voltage source" 
  extends TwoPin;
  parameter Real V=1 "Amplitude of sine wave";
  parameter Real phase=0 "Phase of sine wave";
  parameter Real freqHz=1 "Frequency of sine wave";
  parameter Real offset=0 "Offset volatge of sine wave";
  parameter Real startTime=0 "sine wave start time";
  parameter Real PI=3.141592653589793;
equation
  v = offset + (if time < startTime then 0 else V*sin(2*PI*freqHz*(time - startTime) +phase));
end SineVoltage;

model CCS "controlled voltage source" 
  extends TwoPin;
  Real Iin;
equation 
 Iin = i;
end CCS;



model CVS "controlled voltage source" 
  extends TwoPin;
  Real vin;
equation 
 vin = v;
end CVS;

model VoltageSensor 
  extends TwoPin;
 equation
  i = 0;
 end VoltageSensor;

model CurrentSensor 
  extends TwoPin;
 equation
    v = 0;
 end CurrentSensor; 

model PotentialSensor 
  Pin p;
  Real v;
 equation
    p.i = 0;
    v = p.v;
 end PotentialSensor;

model Switch
  parameter Real Ron=0.01 "Resistance when the Switch is closed";
  parameter Real Roff=1e5  "Resistance when the switch is open";
  extends TwoPin;
  Real inp,Rx;
equation
  Rx*i= v;
  Rx=if inp >0 then Ron	
              else Roff;
end Switch;

model Interrupter1
    extends TwoPin;
    Real R;
    parameter Real RMAX=1e6;
    Real K;
  equation
    R = if K > 0. then 1. / RMAX else RMAX;
    R * i = v;
  end Interrupter1;

 model PowerSource
    extends TwoPin(v(start=v0), i(start=i0));
    parameter Real P, v0, i0;
  equation
    v * i = P;
 end PowerSource;


model Diode "Simple diode" 
   extends TwoPin;
    parameter Real Ids=1.e-6 "Saturation current";
    parameter Real Vt=0.04   "Voltage equivalent of temperature (kT/qn)";
    parameter Real Maxexp=15 "Max. exponent for linear continuation";
    parameter Real R=1.e8 "Parallel ohmic resistance";
  equation 

   i = if noEvent(v/Vt > Maxexp) then 
	Ids*(exp(Maxexp)*(1 + v/Vt - Maxexp) - 1) + v/R 
  else 
	Ids*(exp(v/Vt) - 1) + v/R;
  end Diode;




model Gyrator "Gyrator" 
  parameter Real G1=1 "Gyration conductance";
  parameter Real G2=1 "Gyration conductance";

  Pin p1,n1,p2,n2;    
  Real v2,v1,i1,i2;

equation 

  v1=p1.v-n1.v;
  0=p1.i+n1.i;
  i1=p1.i;

  v2=p2.v-n2.v;
  0=p2.i+n2.i;
  i2=p2.i;

  i1 = G2*v2;
  i2 = -G1*v1;

end Gyrator;

model IdealTransformer "Transformer with two ports" 
  parameter Real N=1 "Transformer turn ration N1/N2";  
  Pin p1, n1, p2, n2;    
  Real v2,v1,i1,i2;

equation 
  v1=p1.v-n1.v;
  0=p1.i+n1.i;
  i1=p1.i;

  v2=p2.v-n2.v;
  0=p2.i+n2.i;
  i2=p2.i;

/*  v1 = L1*der(i1) + M*der(i2); 
  v2 = M*der(i1) + L2*der(i2);
    Results very often in high index
  problem not handled in Scicos /Masoud
*/

v1=N*v2;
i2=-N*i1;

end IdealTransformer;

model NMOS "Simple MOS Transistor" 
  parameter Real W=20.e-6 "Width";
  parameter Real L=6.e-6 "Length";
  parameter Real Beta=0.041e-3    "Transconductance parameter";
  parameter Real Vt=0.8 "Zero bias threshold voltage";
  parameter Real K2=1.144 "Bulk threshold parameter";
  parameter Real K5= 0.7311"Reduction of pinch-off region";
  parameter Real dW=-2.5e-6 "narrowing of channel";
  parameter Real dL= -1.5e-6"shortening of channel";
  parameter Real RDS=1.e+7 "Drain-Source-Resistance";

  Pin D "Drain";
  Pin G "Gate";
  Pin S "Source";
  Pin B "Bulk";

  Real v;
  Real uds;
  Real ubs;
  Real ugst;
  Real ud;
  Real us;
  Real id,gds;
equation  
    //assert (L + dL > 0, "Effective length must be positive");
    //assert (W + dW > 0, "Effective width  must be positive");

  gds = if noEvent(RDS < 1.e-20 and RDS > -1.e-20) then 1.e20 else 1/RDS;
  v = Beta*(W + dW)/(L + dL);
  ud = if noEvent(D.v < S.v) then S.v else D.v;
  us = if noEvent(D.v < S.v) then D.v else S.v;
  uds = ud - us;
  ubs = if noEvent(B.v > us) then 0 else B.v - us;
  ugst = (G.v - us - Vt + K2*ubs)*K5;
  id = if noEvent(ugst <= 0) then uds*gds else if noEvent(ugst > uds) then v*uds*(ugst
     - uds/2) + uds*gds else v*ugst*ugst/2 + uds*gds;
  G.i = 0;
  D.i = if noEvent(D.v < S.v) then -id else id;
  S.i = -D.i;
  B.i = 0;
end NMOS;


model NPN "Simple BJT according to Ebers-Moll" 
  parameter Real Bf=50 "Forward beta";
  parameter Real Br=0.1 "Reverse beta";
  parameter Real Is=1.e-16 "Transport saturation current";
  parameter Real Vak=0.02 "Early voltage (inverse), 1/Volt";
  parameter Real Tauf=0.12e-9 "Ideal forward transit time";
  parameter Real Taur=5e-9 "Ideal reverse transit time";
  parameter Real Ccs=1e-12 "Collector-substrat(ground) cap.";
  parameter Real Cje=0.4e-12 "Base-emitter zero bias depletion cap.";
  parameter Real Cjc=0.5e-12 "Base-coll. zero bias depletion cap.";
  parameter Real Phie=0.8 "Base-emitter diffusion voltage";
  parameter Real Me=0.4 "Base-emitter gradation exponent";
  parameter Real Phic=0.8 "Base-collector diffusion voltage";
  parameter Real Mc=0.333 "Base-collector gradation exponent";
  parameter Real Gbc=1e-15 "Base-collector conductance";
  parameter Real Gbe=1e-15 "Base-emitter conductance";
  parameter Real Vt=0.02585 "Voltage equivalent of temperature";
  parameter Real EMinMax=40 "if x > EMax, the exp(x) function is linearized";
  Real vbc;
  Real vbe;
  Real qbk;
  Real ibc;
  Real ibe;
  Real cbc;
  Real cbe;
  Real ExMin;
  Real ExMax;
  Real Capcje;
  Real Capcjc;
  Real EMax;
  Real EMin;

  Pin C "Collector";
  Pin B "Base";
  Pin E "Emitter";
equation 
  EMax=EMinMax;
  EMin=-2*EMinMax;
  ExMin = exp(EMin);
  ExMax = exp(EMax);
  vbc = B.v - C.v;
  vbe = B.v - E.v;
  qbk = 1 - vbc*Vak;
  
  ibc = if noEvent(vbc/Vt < EMin) then Is*(ExMin*(vbc/Vt - EMin + 1) - 1) + vbc*Gbc
     else if noEvent(vbc/Vt > EMax) then Is*(ExMax*(vbc/Vt - EMax + 1) - 1) + vbc*Gbc
     else Is*(exp(vbc/Vt) - 1) + vbc*Gbc;

  ibe = if noEvent(vbe/Vt < EMin) then Is*(ExMin*(vbe/Vt - EMin + 1) - 1) + vbe*Gbe
     else if noEvent(vbe/Vt > EMax) then Is*(ExMax*(vbe/Vt - EMax + 1) - 1) + vbe*Gbe
     else Is*(exp(vbe/Vt) - 1) + vbe*Gbe;

  Capcjc = if (vbc/Phic > 0) then Cjc*(1 + Mc*vbc/Phic) else Cjc*(1 - vbc/Phic)^(-Mc);
  Capcje = if (vbe/Phie > 0) then Cje*(1 + Me*vbe/Phie) else Cje*(1 - vbe/Phie)^(-Me);

  cbc = if noEvent(vbc/Vt < EMin) then Taur*Is/Vt*ExMin*(vbc/Vt - EMin + 1) + Capcjc
     else if noEvent(vbc/Vt > EMax) then Taur*Is/Vt*ExMax*(vbc/Vt - EMax + 1) + Capcjc
     else Taur*Is/Vt*exp(vbc/Vt) + Capcjc;

  cbe = if noEvent(vbe/Vt < EMin) then Tauf*Is/Vt*ExMin*(vbe/Vt - EMin + 1) + Capcje
     else if noEvent(vbe/Vt > EMax) then Tauf*Is/Vt*ExMax*(vbe/Vt - EMax + 1) + Capcje
     else Tauf*Is/Vt*exp(vbe/Vt) + Capcje;

  C.i = (ibe - ibc)*qbk - ibc/Br - cbc*der(vbc) + Ccs*der(C.v);
  B.i = ibe/Bf + ibc/Br + cbc*der(vbc) + cbe*der(vbe);
  E.i = -B.i - C.i + Ccs*der(C.v);
end NPN;

model OpAmp 
//  parameter Real OLGain=1000 "Open Loop gain";
//  parameter Real SatH=10  "Positive saturation voltage";
//  parameter Real SatL=-10 "Negative Saturation voltage";
  Pin in_p "Positive pin of the input port";
  Pin in_n "Negative pin of the input port";
  Pin out "Output pin";
equation 
  in_p.i = 0;
  in_n.i = 0;
  in_p.v - in_n.v=0;
end OpAmp;
  

model PMOS "Simple MOS Transistor" 
  parameter Real W=50.0e-6 "Width";
  parameter Real L=6.0e-6 "Length";
  parameter Real Beta=0.0105e-3 "Transconductance parameter";
  parameter Real Vt=-1 "Zero bias threshold voltage";
  parameter Real K2=0.41 "Bulk threshold parameter";
  parameter Real K5=0.839 "Reduction of pinch-off region";
  parameter Real dW=-2.5e-6 "Narrowing of channel";
  parameter Real dL=-2.1e-6 "Shortening of channel";
  parameter Real RDS=1.e+7 "Drain-Source-Resistance";

  Pin D "Drain";
  Pin G "Gate";
  Pin S "Source";
  Pin B "Bulk";

  Real v;
  Real uds;
  Real ubs;
  Real ugst;
  Real ud;
  Real us;
  Real id,gds;
equation 
  gds = 1/RDS;//if (RDS < 1.e-20 and RDS > -1.e-20) then 1.e20 else 1/RDS;
  v = Beta*(W + dW)/(L + dL);
  ud = if noEvent(D.v > S.v) then S.v else D.v;
  us = if noEvent(D.v > S.v) then D.v else S.v;
  uds = ud - us;
  ubs = if noEvent(B.v < us) then 0 else B.v - us;
  ugst = (G.v - us - Vt + K2*ubs)*K5;
  id = if noEvent(ugst >= 0) then uds*gds else if noEvent(ugst < uds) then -v*uds*(
    ugst - uds/2) + uds*gds else -v*ugst*ugst/2 + uds*gds;
  G.i = 0;
  D.i = if noEvent(D.v > S.v) then -id else id;
  S.i = -D.i;
  B.i = 0;
end PMOS;

model PNP "Simple BJT according to Ebers-Moll" 
  parameter Real Bf=50 "Forward beta";
  parameter Real Br=0.1 "Reverse beta";
  parameter Real Is=1.e-16 "Transport saturation current";
  parameter Real Vak=0.02 "Early voltage (inverse), 1/Volt";
  parameter Real Tauf=0.12e-9 "Ideal forward transit time";
  parameter Real Taur=5e-9 "Ideal reverse transit time";
  parameter Real Ccs=1e-12 "Collector-substrat(ground) cap.";
  parameter Real Cje=0.4e-12 "Base-emitter zero bias depletion cap.";
  parameter Real Cjc=0.5e-12 "Base-coll. zero bias depletion cap.";
  parameter Real Phie=0.8 "Base-emitter diffusion voltage";
  parameter Real Me=0.4 "Base-emitter gradation exponent";
  parameter Real Phic=0.8 "Base-collector diffusion voltage";
  parameter Real Mc=0.333 "Base-collector gradation exponent";
  parameter Real Gbc=1e-15 "Base-collector conductance";
  parameter Real Gbe=1e-15 "Base-emitter conductance";
  parameter Real Vt=0.02585 "Voltage equivalent of temperature";
  parameter Real EMinMax=40 "if x < EMin, the exp(x) function is linearized";
  Real vbc;
  Real vbe;
  Real qbk;
  Real ibc;
  Real ibe;
  Real cbc;
  Real cbe;
  Real ExMin;
  Real ExMax;
  Real Capcje;
  Real Capcjc;
  Real EMax;
  Real EMin;

  Pin C "Collector";
  Pin B "Base";
  Pin E "Emitter";
equation 
  EMax=EMinMax;
  EMin=-2*EMinMax;
  ExMin = exp(EMin);
  ExMax = exp(EMax);
  vbc = C.v - B.v;
  vbe = E.v - B.v;
  qbk = 1 - vbc*Vak;
  
  ibc = if noEvent(vbc/Vt < EMin) then Is*(ExMin*(vbc/Vt - EMin + 1) - 1) + vbc*
    Gbc else if noEvent(vbc/Vt > EMax) then Is*(ExMax*(vbc/Vt - EMax + 1) - 1) + 
    vbc*Gbc else Is*(exp(vbc/Vt) - 1) + vbc*Gbc;
  
  ibe = if noEvent(vbe/Vt < EMin) then Is*(ExMin*(vbe/Vt - EMin + 1) - 1) + vbe*
    Gbe else if noEvent(vbe/Vt > EMax) then Is*(ExMax*(vbe/Vt - EMax + 1) - 1) + 
    vbe*Gbe else Is*(exp(vbe/Vt) - 1) + vbe*Gbe;
  
  Capcjc = if (vbc/Phic > 0) then Cjc*(1 + Mc*vbc/Phic) else Cjc*(1 -  vbc/Phic)^(-Mc);
  Capcje = if (vbe/Phie > 0) then Cje*(1 + Me*vbe/Phie) else Cje*(1 -   vbe/Phie)^(-Me);

  cbc = if noEvent(vbc/Vt < EMin) then Taur*Is/Vt*ExMin*(vbc/Vt - EMin + 1) + 
    Capcjc else if noEvent(vbc/Vt > EMax) then Taur*Is/Vt*ExMax*(vbc/Vt - EMax + 1)
     + Capcjc else Taur*Is/Vt*exp(vbc/Vt) + Capcjc;
  cbe = if noEvent(vbe/Vt < EMin) then Tauf*Is/Vt*ExMin*(vbe/Vt - EMin + 1) + 
    Capcje else if noEvent(vbe/Vt > EMax) then Tauf*Is/Vt*ExMax*(vbe/Vt - EMax + 1)
     + Capcje else Tauf*Is/Vt*exp(vbe/Vt) + Capcje;

  C.i = -((ibe - ibc)*qbk - ibc/Br - cbc*der(vbc) + Ccs*der(C.v));
  B.i = -(ibe/Bf + ibc/Br + cbe*der(vbe) + cbc*der(vbc));
  E.i = -B.i - C.i - Ccs*der(C.v);
end PNP;





//==================================================
model Resistorx
	Pin p, n;
	parameter Real R (fixed=false)=1 "Re{si}s  >< ance";
        //parameter Integer n = 3;
        // Real Z[3] (start={ 1.0 for i in 1 : 3 });
	
	parameter Real Rx1[3] (fixed = {false, false, true})={1,3,40000} "Resistance";	
	Real Z[3](fixed = {false, true, false},start={222,333,444})  "Resistance";
     	parameter Real Ry[3] (fixed = {false, false, true})={1,3,40000} "Resistance";
	Real T (start=3.4) "fff ggg hhh";
	/*Real c[2](start={22,33})  "ifffffffff";*/
	Real mytr (start=3.56,fixed=true);
	Real mytr2 (start=3.56,fixed=false);

      Resistor Rx[10](R={ i*2.3 for i in 1 : 10 });
equation

for k in 1: 10 loop
  Rx[k].n.v=0;
  Rx[k].p.v=0;
end for ;


	mytr=2.3;
	mytr2=2.3;
	T=R;
	Z[1]=3;	Z[2]=0;	Z[3]=Rx1[3];

	Rx[1]*p.i = p.v - n.v; 
	p.i = -n.i;

/*
if time < 0.5 then
     c = {1,0};
  elseif time < 7 then
    c[1] = 5;
    c[2] = 9; 
  elseif time < 8 then
    c = {5,8};
  else
     c[1] = 2;
     c[2] = 4;
  end if;

*/


end Resistorx;

/*  parameter Integer n = 400;
  Real x[n](start={ 1.0 for i in 1 : n });*/
