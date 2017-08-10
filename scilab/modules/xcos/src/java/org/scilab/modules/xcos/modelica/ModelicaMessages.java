/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2011 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.xcos.modelica;

import org.scilab.modules.localization.Messages;

/**
 * Contains the localized messages for the current packages and sub-packages.
 */
// CSOFF: JavadocVariable
// CSOFF: LineLength
public final class ModelicaMessages {
    public static final String MODELICA_SETTINGS = Messages
            .gettext("Modelica settings");

    public static final String NAME = Messages.gettext("name");
    public static final String ID = Messages.gettext("id");
    public static final String KIND = Messages.gettext("kind");
    public static final String FIXED = Messages.gettext("fixed");
    public static final String INITIAL = Messages.gettext("initial");
    public static final String WEIGHT = Messages.gettext("weight");
    public static final String MAX = Messages.gettext("max");
    public static final String MIN = Messages.gettext("min");
    public static final String NOMINAL = Messages.gettext("nominal");
    public static final String COMMENT = Messages.gettext("comment");
    public static final String SELECTED = Messages.gettext("selected");

    public static final String FIX_DERIVATIVES = Messages
            .gettext("Fix derivatives");
    public static final String FIX_STATES = Messages.gettext("Fix states");

    public static final String SOLVE = Messages.gettext("Solve");
    public static final String MODEL_INVALID = Messages
            .gettext("model is not valid");
    public static final String COMPILING = Messages.gettext("Compiling");
    public static final String COMPUTING = Messages.gettext("Computing");

    public static final String GLOBAL = Messages.gettext("Global");
    public static final String EQUATIONS = Messages.gettext("Equations");
    public static final String DISCRETES = Messages.gettext("Discretes");
    public static final String INPUTS = Messages.gettext("Inputs");
    public static final String OUTPUTS = Messages.gettext("Outputs");
    public static final String SOLVER = Messages.gettext("Solver");
    public static final String INITIAL_COMPUTING_METHOD = Messages
            .gettext("Initial computing method");
    public static final String PARAMETER_EMBEDDING = Messages
            .gettext("Parameter embedding");
    public static final String PARAMETER_EMBEDDING_EXPLAINED = Messages
            .gettext("lets the user change the parameters and call the solver without regenerating code.");
    public static final String GENERATE_JACOBIAN = Messages
            .gettext("Generate Jacobian");
    public static final String ERROR = Messages.gettext("Error");
    public static final String UNKNOWNS_NEQ_EQUATIONS = Messages
            .gettext("Unknowns not equal to Equations");
    public static final String UNKNOWNS = Messages.gettext("Unknowns");
    public static final String REDUCED = Messages.gettext("Reduced");
    public static final String EXTENDED = Messages.gettext("Extended");
    public static final String DIFF_ST = Messages.gettext("Diff. States");
    public static final String FIXED_PARAMS = Messages
            .gettext("Fixed parameters");
    public static final String RELAXED_PARAMS = Messages
            .gettext("Relaxed parameters");
    public static final String FIXED_VARIABLES = Messages
            .gettext("Fixed variables");
    public static final String RELAXED_VARIABLES = Messages
            .gettext("Relaxed variables");

    public static final String TAKE_A_LOOK_AT_SCILAB = Messages
            .gettext("Please take a look into the Scilab console");

    /**
     * Not used constructor
     */
    private ModelicaMessages() {
    }
}
// CSON: JavadocVariable
// CSON: LineLength
