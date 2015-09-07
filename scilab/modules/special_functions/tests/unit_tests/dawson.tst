//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - S/E - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


assert_checkalmostequal(dawson(2 + 1*%i), 0.1635394094345355614904345232875688576839 + -0.1531245755371229803585918112683241066853*%i, 1.e-10);
assert_checkalmostequal(dawson(-2 + 1*%i), -0.1635394094345355614904345232875688576839 + -0.1531245755371229803585918112683241066853*%i, 1.e-10);
assert_checkalmostequal(dawson(2 + -1*%i), 0.1635394094345355614904345232875688576839 + 0.1531245755371229803585918112683241066853*%i, 1.e-10);
assert_checkalmostequal(dawson(-2 + -1*%i), -0.1635394094345355614904345232875688576839 + 0.1531245755371229803585918112683241066853*%i, 1.e-10);
assert_checkalmostequal(dawson(-28 + 9*%i), -0.01619082256681596362895875232699626384420 + -0.005210224203359059109181555401330902819419*%i, 1.e-10);
assert_checkalmostequal(dawson(33 + -21*%i), 0.01078377080978103125464543240346760257008 + 0.006866888783433775382193630944275682670599*%i, 1.e-10);
assert_checkalmostequal(dawson(1e3 + 1e3*%i), -0.5808616819196736225612296471081337245459 + 0.6688593905505562263387760667171706325749*%i, 1.e-10);
if getos() == "Windows" then
assert_checkalmostequal(dawson(-1000 + -3001*%i), complex(%nan, -%nan), 1.e-10);
else
assert_checkalmostequal(dawson(-1000 + -3001*%i), complex(%inf, -%inf), 1.e-10);
end
assert_checkalmostequal(dawson(1e-8 + 5.1e-3*%i), 0.1000052020902036118082966385855563526705e-7 + 0.005100088434920073153418834680320146441685*%i, 1.e-10);
assert_checkalmostequal(dawson(4.95e-3 + -4.9e-3*%i), 0.004950156837581592745389973960217444687524 + -0.004899838305155226382584756154100963570500*%i, 1.e-10);
assert_checkalmostequal(dawson(5.1e-3 + 5.1e-3*%i), 0.005100176864319675957314822982399286703798 + 0.005099823128319785355949825238269336481254*%i, 1.e-10);
assert_checkalmostequal(dawson(0.5 + 4.9e-3*%i), 0.4244534840871830045021143490355372016428 + 0.002820278933186814021399602648373095266538*%i, 1.e-10);
assert_checkalmostequal(dawson(-0.5e1 + 4.9e-4*%i), -0.1021340733271046543881236523269967674156 + -0.00001045696456072005761498961861088944159916*%i, 1.e-10);
assert_checkalmostequal(dawson(-0.5e2 + -4.9e-5*%i), -0.01000200120119206748855061636187197886859 + 0.9805885888237419500266621041508714123763e-8*%i, 1.e-10);
assert_checkalmostequal(dawson(0.5e3 + 4.9e-6*%i), 0.001000002000012000023960527532953151819595 + -0.9800058800588007290937355024646722133204e-11*%i, 1.e-10);
assert_checkalmostequal(dawson(0.5 + 5.1e-3*%i), 0.4244549085628511778373438768121222815752 + 0.002935393851311701428647152230552122898291*%i, 1.e-10);
assert_checkalmostequal(dawson(-0.5e1 + 5.1e-4*%i), -0.1021340732357117208743299813648493928105 + -0.00001088377943049851799938998805451564893540*%i, 1.e-10);
assert_checkalmostequal(dawson(-0.5e2 + -5.1e-5*%i), -0.01000200120119126652710792390331206563616 + 0.1020612612857282306892368985525393707486e-7*%i, 1.e-10);
assert_checkalmostequal(dawson(1e-6 + 2e-6*%i), 0.1000000000007333333333344266666666664457e-5 + 0.2000000000001333333333323199999999978819e-5*%i, 1.e-10);
assert_checkalmostequal(dawson(2e-6 + 0*%i), 0.1999999999994666666666675199999999990248e-5 + 0*%i, 1.e-10);
assert_checkalmostequal(dawson(2 + 0*%i), 0.3013403889237919660346644392864226952119 + 0*%i, 1.e-10);
assert_checkalmostequal(dawson(20 + 0*%i), 0.02503136792640367194699495234782353186858 + 0*%i, 1.e-10);
assert_checkalmostequal(dawson(200 + 0*%i), 0.002500031251171948248596912483183760683918 + 0*%i, 1.e-10);
assert_checkalmostequal(dawson(0 + 4.9e-3*%i), 0 + 0.004900078433419939164774792850907128053308*%i, 1.e-10);
assert_checkalmostequal(dawson(0 + -5.1e-3*%i), 0 + -0.005100088434920074173454208832365950009419*%i, 1.e-10);
assert_checkalmostequal(dawson(0 + 2e-6*%i), 0 + 0.2000000000005333333333341866666666676419e-5*%i, 1.e-10);
assert_checkalmostequal(dawson(0 + -2*%i), 0 + -48.16001211429122974789822893525016528191*%i, 1.e-10);
assert_checkalmostequal(dawson(0 + 20*%i), 0 + 0.4627407029504443513654142715903005954668e174*%i, 1.e-10);
assert_checkalmostequal(dawson(0 + -200*%i), complex(0,-%inf), 1.e-10);
assert_checkalmostequal(dawson(%inf + 0*%i), 0 + 0*%i, 1.e-10);
assert_checkalmostequal(dawson(-%inf + 0*%i), -0 + 0*%i, 1.e-10);
assert_checkalmostequal(dawson(0 + %inf*%i), complex(%nan, %nan), 1.e-10);
assert_checkalmostequal(dawson(0 + -%inf*%i), complex(%nan, %nan), 1.e-10);
assert_checkalmostequal(dawson(%inf + %inf*%i), %nan + %nan*%i, 1.e-10);
assert_checkalmostequal(dawson(%inf + -%inf*%i), %nan + %nan*%i, 1.e-10);
assert_checkalmostequal(dawson(%nan + %nan*%i), %nan + %nan*%i, 1.e-10);
assert_checkalmostequal(dawson(%nan + 0*%i), %nan + 0*%i, 1.e-10);
assert_checkalmostequal(dawson(0 + %nan*%i), 0 + %nan*%i, 1.e-10);
assert_checkalmostequal(dawson(%nan + %inf*%i), %nan + %nan*%i, 1.e-10);
assert_checkalmostequal(dawson(%inf + %nan*%i), %nan + %nan*%i, 1.e-10);
assert_checkalmostequal(dawson(39 + 6.4e-5*%i), 0.01282473148489433743567240624939698290584 + -0.2105957276516618621447832572909153498104e-7*%i, 1.e-10);
assert_checkalmostequal(dawson(41 + 6.09e-5*%i), 0.01219875253423634378984109995893708152885 + -0.1813040560401824664088425926165834355953e-7*%i, 1.e-10);
assert_checkalmostequal(dawson(4.9e7 + 5e-11*%i), 0.1020408163265306334945473399689037886997e-7 + -0.1041232819658476285651490827866174985330e-25*%i, 1.e-10);
assert_checkalmostequal(dawson(5.1e7 + 4.8e-11*%i), 0.9803921568627452865036825956835185367356e-8 + -0.9227220299884665067601095648451913375754e-26*%i, 1.e-10);
assert_checkalmostequal(dawson(1e9 + 2.4e-12*%i), 0.5000000000000000002500000000000000003750e-9 + -0.1200000000000000001800000188712838420241e-29*%i, 1.e-10);
assert_checkalmostequal(dawson(1e11 + 2.4e-14*%i), 5.00000000000000000000025000000000000000000003e-12 + -1.20000000000000000000018000000000000000000004e-36*%i, 1.e-10);
assert_checkalmostequal(dawson(1e13 + 2.4e-16*%i), 5.00000000000000000000000002500000000000000000e-14 + -1.20000000000000000000000001800000000000000000e-42*%i, 1.e-10);
assert_checkalmostequal(dawson(1e300 + 2.4e-303*%i), 5e-301 + 0*%i, 1.e-10);

assert_checkerror("dawson(''foo'')",[],999);
assert_checkerror("dawson(sparse([1,2;4,5;3,10],[1,2,3]))",[],999);
