//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// Test the model variables of XML files from differents exporters of FMUs.

// <-- NO CHECK REF -->
// <-- LINUX ONLY -->

path = fmigetPath();
[version, opts] = getversion();

if opts(2) == "x64" then
    // JModelica
     fmu = importFMU(fullfile(path, "tests", "unit_tests", "JModelica_CoupledClutches.fmu"));
     // type definitions
     typeNames = "StateSelect";
     description = "";
     quantity = "";
     unit = "";
     displayUnit = "";
     gain = 1;
     offset = 0;
     relativeQuantity = %f;
     minValue = 1;
     maxValue = 5;
     nominal = 1;
     items = 5;
     itemList_1 = ["never";"avoid";"default";"prefer";"always"];
     itemList_Descr = ["Do not use as state at all.";...
     "Use as state, if it cannot be avoided (but only if variable appears differentiated and no other potential "+... 
     "state with attribute default, prefer, or always can be selected).";...
     "Use as state if appropriate, but only if variable appears differentiated.";...
     "Prefer it as state over those having the default value (also variables can "+...  
      "be selected, which do not appear differentiated). ";"Do use it as a state."]; 
     
     assert_checkequal(fmu.modelVariables.Type.name, typeNames);
     assert_checkequal(fmu.modelVariables.Type.description, description);
     assert_checkequal(fmu.modelVariables.Type.quantity, quantity);
     assert_checkequal(fmu.modelVariables.Type.unit, unit);
     assert_checkequal(fmu.modelVariables.Type.displayUnit, displayUnit);
     assert_checkequal(fmu.modelVariables.Type.gain, gain);
     assert_checkequal(fmu.modelVariables.Type.offset, offset);
     assert_checkequal(fmu.modelVariables.Type.relativeQuantity, relativeQuantity);
     assert_checkequal(fmu.modelVariables.Type.nominal, nominal);
     assert_checkequal(fmu.modelVariables.Type.min, minValue);
     assert_checkequal(fmu.modelVariables.Type.max, maxValue);
     assert_checkequal(fmu.modelVariables.Type.items, items);
     assert_checkequal(fmu.modelVariables.Type.itemList(1)(1:5).', itemList_1);
     assert_checkequal(fmu.modelVariables.Type.itemList(1)(6:10).', itemList_Descr);
     // real 
     names = ["u","torque.tau","torque.support.tau","torque.support.phi","torque.phi_support",...
              "torque.flange.tau","torque.flange.phi","step1.y","step1.startTime","step1.offset"];
     refs = [107,145,145,47,47,145,108,124,53,25];
     description = ["","Accelerating torque acting at flange (= -flange.tau)",...
                    "Reaction torque in the support/housing","Absolute rotation angle of the support/housing",...
                    "Absolute angle of support flange","Cut torque in the flange",....
                    "Absolute rotation angle of flange","Connector of Real output signal",...
                    "Output y = offset for time < startTime","Offset of output signal y"];
     variability = ["continuous","continuous","continuous","continuous","continuous","continuous",...
                  "continuous","continuous","parameter","parameter"];
     causality = ["input","internal","internal","internal","internal","internal",...
                  "internal","internal","internal","internal"];
     alias = ["noAlias","negatedAlias","negatedAlias","alias","alias",...
              "alias","alias","alias","noAlias","noAlias"];
     declaredType = "";
     quantity = ["","","Torque","Angle","Angle","Torque","Angle","","Time",""];
     unit = ["","","N.m","rad","rad","N.m","rad","","s",""];
     displayUnit = ["","","","deg","deg","","deg","","",""];
     relativeQuantity = %f;
     minValue = -%inf;
     maxValue = %inf; 
     startValue = [0,%nan,%nan,%nan,%nan,%nan,%nan,%nan,%nan,0];
     nominal = 1;
     fixed = %t;
        assert_checkequal(fmu.modelVariables.Real.name($:-1:158), names);
        assert_checkequal(fmu.modelVariables.Real.valueReference($:-1:158), refs);
        assert_checkequal(fmu.modelVariables.Real.description($:-1:158), description);
        assert_checkequal(fmu.modelVariables.Real.variability($:-1:158), variability);
        assert_checkequal(fmu.modelVariables.Real.causality($:-1:158), causality);
        assert_checkequal(fmu.modelVariables.Real.alias($:-1:158), alias);
        assert_checkequal(fmu.modelVariables.Real.quantity($:-1:158), quantity);
        assert_checkequal(fmu.modelVariables.Real.displayUnit($:-1:158), displayUnit);
        assert_checkequal(fmu.modelVariables.Real.start($:-1:158), startValue);
     i = 158;
     while (i<168)
         assert_checkequal(fmu.modelVariables.Real.declaredType(i), declaredType);
         assert_checkequal(fmu.modelVariables.Real.relativeQuantity(i), relativeQuantity);
         assert_checkequal(fmu.modelVariables.Real.min(i), minValue);
         assert_checkequal(fmu.modelVariables.Real.max(i), maxValue);
         assert_checkequal(fmu.modelVariables.Real.nominal(i), nominal);
         assert_checkequal(fmu.modelVariables.Real.fixed(i), fixed);
         i = i + 1;
     end
    // integer 
    names = ["clutch3.mode","clutch3.Unknown","clutch3.Stuck","clutch3.Free","clutch3.Forward"];
    refs = [268435609,268435521,268435524,268435522,268435523];
    description = ["","Value of mode is not known","w_rel = 0 (forward sliding, locked or backward sliding)",...
                   "Element is not active","w_rel > 0 (forward sliding)"];
    variability = ["discrete","constant","constant","constant","constant"];
    causality = "internal";
    alias = "noAlias";
    declType = "";
    quantity = "";
    minValue = [-1,-2147483648,-2147483648,-2147483648,-2147483648];
    maxValue = [3,2147483647,2147483647,2147483647,2147483647];
    startValue = [%nan,3,0,2,1];
    fixed = %t;
    assert_checkequal(fmu.modelVariables.Integer.name($:-1:16), names);
    assert_checkequal(fmu.modelVariables.Integer.valueReference($:-1:16), refs);
    assert_checkequal(fmu.modelVariables.Integer.description($:-1:16), description);
    assert_checkequal(fmu.modelVariables.Integer.variability($:-1:16), variability);
    assert_checkequal(fmu.modelVariables.Integer.min($:-1:16), minValue);
    assert_checkequal(fmu.modelVariables.Integer.max($:-1:16), maxValue);
    assert_checkequal(fmu.modelVariables.Integer.start($:-1:16), startValue);
    for i=20:-1:16
        assert_checkequal(fmu.modelVariables.Integer.causality(i), causality);
        assert_checkequal(fmu.modelVariables.Integer.alias(i), alias);
        assert_checkequal(fmu.modelVariables.Integer.declaredType(i), declType);
        assert_checkequal(fmu.modelVariables.Integer.quantity(i), quantity);
        assert_checkequal(fmu.modelVariables.Integer.fixed(i), fixed);
    end
    // boolean 
    names = ["torque.useSupport","clutch3.useHeatPort","clutch3.startForward",...
             "clutch3.startBackward","clutch3.locked"];
    refs = [536870991,536870994,536871075,536871076,536871077];
    description = ["= true, if support flange enabled, otherwise implicitly grounded",...
                   "=true, if heatPort is enabled","true, if w_rel=0 and start of forward sliding",...
                   "true, if w_rel=0 and start of backward sliding","true, if w_rel=0 and not sliding"];
    variability = ["parameter","parameter","discrete","discrete","discrete"];
    causality = "internal";
    alias = "noAlias";
    declType = "";
    startValue = [%t,%f,%f,%f,%f];
    fixed = %t;
    assert_checkequal(fmu.modelVariables.Boolean.name($:-1:20), names);
    assert_checkequal(fmu.modelVariables.Boolean.valueReference($:-1:20), refs);
    assert_checkequal(fmu.modelVariables.Boolean.description($:-1:20), description);
    assert_checkequal(fmu.modelVariables.Boolean.variability($:-1:20), variability);
    assert_checkequal(fmu.modelVariables.Boolean.start($:-1:20), startValue);
    for i=24:-1:20
        assert_checkequal(fmu.modelVariables.Boolean.causality(i), causality);
        assert_checkequal(fmu.modelVariables.Boolean.alias(i), alias);
        assert_checkequal(fmu.modelVariables.Boolean.declaredType(i), declType);
        assert_checkequal(fmu.modelVariables.Boolean.fixed(i), fixed);
    end
    // string 
    assert_checkequal(fmu.modelVariables.String.name, []);
    // enumeration
    names = ["J1.stateSelect","J2.stateSelect","J3.stateSelect"];
    refs = [268435528,268435530,268435532];
    description = ["Priority to use phi and w as states","Priority to use phi and w as states",...
                   "Priority to use phi and w as states"];
    variability = "parameter";
    causality = "internal";
    alias = "noAlias";
    declType = "StateSelect";
    quantity = "";
    startValue = 3;
    minValue = 1;
    maxValue = 5;
    fixed = %t;
        assert_checkequal(fmu.modelVariables.Enumeration.name(1:3), names);
        assert_checkequal(fmu.modelVariables.Enumeration.valueReference(1:3), refs);
        assert_checkequal(fmu.modelVariables.Enumeration.description(1:3), description);
        for i=1:3
            assert_checkequal(fmu.modelVariables.Enumeration.variability(i), variability);
            assert_checkequal(fmu.modelVariables.Enumeration.causality(i), causality);
            assert_checkequal(fmu.modelVariables.Enumeration.alias(i), alias);
            assert_checkequal(fmu.modelVariables.Enumeration.declaredType(i), declType);
            assert_checkequal(fmu.modelVariables.Enumeration.start(i), startValue);
            assert_checkequal(fmu.modelVariables.Enumeration.min(i), minValue);
            assert_checkequal(fmu.modelVariables.Enumeration.max(i), maxValue);
            assert_checkequal(fmu.modelVariables.Enumeration.fixed(i), fixed);
        end
else
    // MapleSIM
    fmu = importFMU(fullfile(path, "tests", "unit_tests", "MapleSIM_CoupledClutches_Linux32bits.fmu"));
    // real 
    names = ["CC_r(0)","CC_r(1)","CC_r(2)","CC_r(3)","CC_r(4)","CC_r(5)","CC_r(6)",...
             "CC_r(7)","CC_r(8)","CC_r(9)"];
    refs = [0,1,2,3,4,5,6,7,8,9];
    description = ["`internal time`","`Main.J1.phi`(t)","`internal variable`","`Main.J1.w`(t)",...
                   "`internal variable`","`Main.J2.phi`(t)","`internal variable`",...
                   "`Main.J2.w`(t)","`internal variable`","`Main.J3.phi`(t)"];
    variability = "continuous";
    causality = "internal";
    alias = "noAlias";
    declaredType = "";
    quantity = "";
    unit = "";
    displayUnit = "";
    relativeQuantity = %f;
    minValue = -%inf;
    maxValue = %inf; 
    startValue = %nan;
    nominal = 1;
    fixed = %t;
        assert_checkequal(fmu.modelVariables.Real.name(1:10), names);
        assert_checkequal(fmu.modelVariables.Real.valueReference(1:10), refs);
        assert_checkequal(fmu.modelVariables.Real.description(1:10), description);
    for i=1:10
        assert_checkequal(fmu.modelVariables.Real.variability(i), variability);
        assert_checkequal(fmu.modelVariables.Real.causality(i), causality);
        assert_checkequal(fmu.modelVariables.Real.alias(i), alias);
        assert_checkequal(fmu.modelVariables.Real.declaredType(i), declaredType);
        assert_checkequal(fmu.modelVariables.Real.relativeQuantity(i), relativeQuantity);
        assert_checkequal(fmu.modelVariables.Real.displayUnit(i), displayUnit);
        assert_checkequal(fmu.modelVariables.Real.quantity(i), quantity);
        assert_checkequal(fmu.modelVariables.Real.min(i), minValue);
        assert_checkequal(fmu.modelVariables.Real.max(i), maxValue);
        assert_checkequal(fmu.modelVariables.Real.start(i), startValue);
        assert_checkequal(fmu.modelVariables.Real.nominal(i), nominal);
        assert_checkequal(fmu.modelVariables.Real.fixed(i), fixed);
    end
end
