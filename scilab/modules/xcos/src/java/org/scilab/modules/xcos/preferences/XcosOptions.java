/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises -Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.preferences;

import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.w3c.dom.Document;

/**
 * Xcos options
 */
public class XcosOptions {
    protected static final String PREFERENCES_XPATH = "//xcos/body/xcos-preferences";
    protected static final String EDITION_XPATH = "//xcos/edition/body/xcos-edition";
    protected static final String SIMULATION_XPATH = "//xcos/simulation/body/xcos-simulation";
    protected static final String KEYMAP_XPATH = "//general/shortcuts/body/actions/action-folder[@xconf-uid=\"xcos\"]/action";

    private static XcosOptions.Preferences preferences;
    private static XcosOptions.Edition edition;
    private static XcosOptions.Simulation simulation;

    private static Document doc;

    @XConfAttribute
    public static class Preferences {
        private Preferences() {
        }

        @XConfAttribute(tag = "xcos-preferences", attributes = {})
        private void set() {
        }
    }

    @XConfAttribute
    public static class Edition {

        private Edition() {
        }

        @XConfAttribute(tag = "xcos-edition", attributes = {})
        private void set() {
        }
    }

    @XConfAttribute
    public static class Simulation {

        private Simulation() {
        }

        @XConfAttribute(tag = "xcos-simulation", attributes = { "final-integration-time", "integrator-absolute-tolerance", "integrator-relative-tolerance",
                        "tolerance-on-time", "max-integration-interval", "real-time-scaling", "solver", "max-step-size", "trace"
                                                              })
        private void set(final String finalIntegrationTime, final String integratorAbsoluteTolerance, final String integratorRelativeTolerance,
                         final String toleranceOnTime, final String maxIntegrationTimeInterval, final String realTimeScaling, final int solver,
                         final String maximumStepSize, final int debugLevel) {

            ScicosParameters.FINAL_INTEGRATION_TIME = Double.parseDouble(finalIntegrationTime);
            ScicosParameters.INTEGRATOR_ABSOLUTE_TOLERANCE = Double.parseDouble(integratorAbsoluteTolerance);
            ScicosParameters.INTEGRATOR_RELATIVE_TOLERANCE = Double.parseDouble(integratorRelativeTolerance);
            ScicosParameters.TOLERANCE_ON_TIME = Double.parseDouble(toleranceOnTime);
            ScicosParameters.MAX_INTEGRATION_TIME_INTERVAL = Double.parseDouble(maxIntegrationTimeInterval);
            ScicosParameters.SOLVER = solver;
            ScicosParameters.MAXIMUM_STEP_SIZE = Double.parseDouble(maximumStepSize);
            ScicosParameters.DEBUG_LEVEL = debugLevel;
        }
    }

    public static void invalidate(XcosConfiguration.Conf conf) {
        if (conf.preferences) {
            preferences = null;
            doc = null;
        }
        if (conf.edition) {
            edition = null;
            doc = null;
        }
        if (conf.simulation) {
            simulation = null;
            doc = null;
        }
    }

    public static final XcosOptions.Preferences getPreferences() {
        if (preferences == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            preferences = XConfiguration.get(XcosOptions.Preferences.class, doc, PREFERENCES_XPATH)[0];
        }

        return preferences;
    }

    public static final XcosOptions.Edition getEdition() {
        if (edition == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            edition = XConfiguration.get(XcosOptions.Edition.class, doc, EDITION_XPATH)[0];
        }

        return edition;
    }

    public static final XcosOptions.Simulation getSimulation() {
        if (simulation == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            simulation = XConfiguration.get(XcosOptions.Simulation.class, doc, SIMULATION_XPATH)[0];
        }

        return simulation;
    }
}
