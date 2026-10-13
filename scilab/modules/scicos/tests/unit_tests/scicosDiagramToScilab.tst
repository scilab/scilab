// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//<-- CLI SHELL MODE -->
//<-- NO CHECK REF -->
//
// Load the reference SSP file from https://www.easy-ssp.com/app/
// Check various properties, sub-systems names and ports
//

function assert_check(scs_m)
    assert_checkequal(scs_m.props.title(1), "DC-Motor")
    assert_checkequal(length(scs_m.objs), 14);

    printf("checking scs_m.objs(1)\n")
    assert_checkequal(scs_m.objs(1).gui, "SUPER_f")
    assert_checkequal(scs_m.objs(1).graphics.in_label, ["U";"M_load"])
    assert_checkequal(scs_m.objs(1).graphics.out_label, ["wB";"phiB";"M_mot";"I"])

    printf("checking scs_m.objs(2)\n")
    assert_checkequal(scs_m.objs(2).gui, "FMU_WRAPPER")
    assert_checkequal(scs_m.objs(2).graphics.exprs, ["stimuli_model.fmu";"TMPDIR/resources/stimuli_model";"me 2.0"])
    assert_checkequal(scs_m.objs(2).graphics.out_label, ["U";"M_load"])

    printf("checking scs_m.objs(10)\n")
    assert_checkequal(scs_m.objs(10).gui, "SSPOutputConnector")
    assert_checkequal(scs_m.objs(10).graphics.exprs(1), "M_mot")

    printf("checking scs_m.objs(11)\n")
    assert_checkequal(scs_m.objs(11).gui, "SSPOutputConnector")
    assert_checkequal(scs_m.objs(11).graphics.exprs(1), "wB")

    printf("checking scs_m.objs(12)\n")
    assert_checkequal(scs_m.objs(12).gui, "SSPOutputConnector")
    assert_checkequal(scs_m.objs(12).graphics.exprs(1), "U")

    printf("checking scs_m.objs(13)\n")
    assert_checkequal(scs_m.objs(13).gui, "SSPOutputConnector")
    assert_checkequal(scs_m.objs(13).graphics.exprs(1), "I")

    printf("checking scs_m.objs(14)\n")
    assert_checkequal(scs_m.objs(14).gui, "SSPOutputConnector")
    assert_checkequal(scs_m.objs(14).graphics.exprs(1), "M_load")

    //
    // Checking the sub-system
    //

    inner = scs_m.objs(1).model.rpar;
    assert_checkequal(inner.props.title(1), "SuT")
    assert_checkequal(length(inner.objs), 17);

    printf("checking scs_m.objs(1).model.rpar.objs(1)\n")
    assert_checkequal(inner.objs(1).gui, "FMU_WRAPPER")
    assert_checkequal(inner.objs(1).graphics.exprs, ["edrive_mass.fmu";"TMPDIR/resources/edrive_mass";"me 2.0"])
    assert_checkequal(inner.objs(1).graphics.in_label, ["M_A";"M_B"])
    assert_checkequal(inner.objs(1).graphics.out_label, ["wA";"phiA";"wB";"phiB"])

    printf("checking scs_m.objs(1).model.rpar.objs(2)\n")
    assert_checkequal(inner.objs(2).gui, "FMU_WRAPPER")
    assert_checkequal(inner.objs(2).graphics.exprs, ["emachine_model.fmu";"TMPDIR/resources/emachine_model";"me 2.0"])
    assert_checkequal(inner.objs(2).graphics.in_label, ["w";"phi";"U"])
    assert_checkequal(inner.objs(2).graphics.out_label, ["M";"I"])

    printf("checking scs_m.objs(1).model.rpar.objs(12)\n")
    assert_checkequal(inner.objs(12).gui, "OUT_f")
    assert_checkequal(inner.objs(12).graphics.exprs, "1")
    assert_checkequal(inner.objs(12).model.label, "wB")

    printf("checking scs_m.objs(1).model.rpar.objs(13)\n")
    assert_checkequal(inner.objs(13).gui, "IN_f")
    assert_checkequal(inner.objs(13).graphics.exprs, "2")
    assert_checkequal(inner.objs(13).model.label, "M_load")

    printf("checking scs_m.objs(1).model.rpar.objs(14)\n")
    assert_checkequal(inner.objs(14).gui, "OUT_f")
    assert_checkequal(inner.objs(14).graphics.exprs, "2")
    assert_checkequal(inner.objs(14).model.label, "phiB")

    printf("checking scs_m.objs(1).model.rpar.objs(15)\n")
    assert_checkequal(inner.objs(15).gui, "IN_f")
    assert_checkequal(inner.objs(15).graphics.exprs, "1")
    assert_checkequal(inner.objs(15).model.label, "U")

    printf("checking scs_m.objs(1).model.rpar.objs(16)\n")
    assert_checkequal(inner.objs(16).gui, "OUT_f")
    assert_checkequal(inner.objs(16).graphics.exprs, "3")
    assert_checkequal(inner.objs(16).model.label, "M_mot")

    printf("checking scs_m.objs(1).model.rpar.objs(17)\n")
    assert_checkequal(inner.objs(17).gui, "OUT_f")
    assert_checkequal(inner.objs(17).graphics.exprs, "4")
    assert_checkequal(inner.objs(17).model.label, "I")
endfunction

scs_m = scicosDiagramToScilab("SCI/modules/xcos/tests/unit_tests/DC Motor.ssp");
assert_check(scs_m);

// save and load back
scicosDiagramToScilab("TMPDIR/DC Motor.ssp", scs_m);
scs_m_reloaded = scicosDiagramToScilab("TMPDIR/DC Motor.ssp");
assert_check(scs_m_reloaded);
