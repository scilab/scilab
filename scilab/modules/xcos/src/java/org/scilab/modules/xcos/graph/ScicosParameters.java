/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2017 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.graph;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyVetoException;
import java.beans.VetoableChangeSupport;
import java.io.Serializable;
import java.util.ArrayList;

import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfString;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.io.ScilabTypeCoder;
import org.scilab.modules.xcos.preferences.XcosOptions;
import org.scilab.modules.xcos.utils.Stack;

/**
 * Contains Scicos specific parameters.
 *
 * This class is a java beans and follow standard properties accessors.
 *
 * @see <a href="http://java.sun.com/docs/books/tutorial/javabeans/properties/bound.html">JavaBeans Bound Properties</a>
 */
public final class ScicosParameters implements Cloneable {

    /*
     * Bean properties
     */
    /**
     * Property bound to finalIntegrationTime modification.
     */
    public static final String FINAL_INTEGRATION_TIME_CHANGE = "finalIntegrationTime";
    /**
     * Property bound to integratorAbsoluteTolerance modification.
     */
    public static final String INTEGRATOR_ABSOLUTE_TOLERANCE_CHANGE = "integratorAbsoluteTolerance";
    /**
     * Property bound to integratorRelativeTolerance modification.
     */
    public static final String INTEGRATOR_RELATIVE_TOLERANCE_CHANGE = "integratorRelativeTolerance";
    /**
     * Property bound to toleranceOnTime modification.
     */
    public static final String TOLERANCE_ON_TIME_CHANGE = "toleranceOnTime";
    /**
     * Property bound to maxIntegrationTimeInterval modification.
     */
    public static final String MAX_INTEGRATION_TIME_INTERVAL_CHANGE = "maxIntegrationTimeInterval";
    /**
     * Property bound to realTimeScaling modification.
     */
    public static final String REAL_TIME_SCALING_CHANGE = "realTimeScaling";
    /**
     * Property bound to solver modification.
     */
    public static final String SOLVER_CHANGE = "solver";
    /**
     * Property bound to maximumStepSize modification.
     */
    public static final String MAXIMUM_STEP_SIZE_CHANGE = "maximumStepSize";
    /**
     * Property bound to debugLevel modification.
     */
    public static final String DEBUG_LEVEL_CHANGE = "debugLevel";
    /**
     * Property bound to context modification.
     */
    public static final String CONTEXT_CHANGE = "context";

    /*
     * Index inside the VectorOfDouble object
     */
    public static final int FINAL_INTEGRATION_TIME = 0;
    public static final int INTEGRATOR_ABSOLUTE_TOLERANCE = 1;
    public static final int INTEGRATOR_RELATIVE_TOLERANCE = 2;
    public static final int TOLERANCE_ON_TIME = 3;
    public static final int MAX_INTEGRATION_TIME_INTERVAL = 4;
    public static final int REAL_TIME_SCALING = 5;
    public static final int SOLVER = 6;
    public static final int MAXIMUM_STEP_SIZE = 7;

    /*
     * Default values from the preferences
     */
    public static VectorOfDouble DEFAULT_PARAMETERS;
    public static int DEFAULT_DEBUG_LEVEL;

    /**
     * Reference to the root diagram
     */
    private final ScicosObjectOwner root;

    /**
     * Reference to the current diagram
     */
    private final ScicosObjectOwner current;

    /*
     * Beans support, used to follow instance modification and validate changes.
     */
    private final VetoableChangeSupport vcs = new VetoableChangeSupport(this);

    /**
     * Default constructor
     *
     * Initialize parameters with their default values.
     * @param root the root diagram
     * @param current the current diagram
     */
    public ScicosParameters(final ScicosObjectOwner root, final ScicosObjectOwner current) {
        this.root = root;
        this.current = current;

        /*
         * This call will update static values from the configuration.
         */
        XcosOptions.getSimulation();

        // install the modification handler
        vcs.addVetoableChangeListener((PropertyChangeEvent evt) -> {
            Xcos xcos = Xcos.getInstance();
            if (!xcos.openedDiagrams(root).isEmpty()) {
                xcos.setModified(root, true);
                xcos.openedDiagrams(root).stream().forEach(d -> d.updateTabTitle());
            }
        });
    }

    public ScicosObjectOwner getRoot() {
        return root;
    }

    public ScicosObjectOwner getCurrent() {
        return current;
    }

    /**
     * Returns the properties values
     *
     * @param controller
     *            the controller instance
     * @return the current properties
     */
    public VectorOfDouble getProperties(final JavaController controller) {
        VectorOfDouble v = new VectorOfDouble(7);
        controller.getObjectProperty(root.getUID(), root.getKind(), ObjectProperties.PROPERTIES, v);
        return v;
    }

    /**
     * Set the properties
     *
     * @param controller
     *            the controller instance
     * @param v
     *            the values to set
     * @throws PropertyVetoException
     *             in case of the values are not valid
     */
    public void setProperties(final JavaController controller, VectorOfDouble v) throws PropertyVetoException {
        VectorOfDouble oldValue = getProperties(controller);
        vcs.fireVetoableChange(FINAL_INTEGRATION_TIME_CHANGE, oldValue, v);
        controller.setObjectProperty(root.getUID(), root.getKind(), ObjectProperties.PROPERTIES, v);
    }

    /**
     * @return current context
     */
    public VectorOfString getContext(final JavaController controller) {
        VectorOfString v = new VectorOfString();
        controller.getObjectProperty(current.getUID(), current.getKind(), ObjectProperties.DIAGRAM_CONTEXT, v);
        return v;
    }

    /**
     * Set the associated context if there is noticeable changes.
     *
     * @param controller
     *            the controller
     * @param v
     *            set context
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setContext(final JavaController controller, VectorOfString v) throws PropertyVetoException {
        VectorOfString oldValue = getContext(controller);
        vcs.fireVetoableChange(CONTEXT_CHANGE, oldValue, v);
        controller.setObjectProperty(current.getUID(), current.getKind(), ObjectProperties.DIAGRAM_CONTEXT, v);
    }

    /**
     * @param controller
     *            the controller instance
     * @return current version
     */
    public String getVersion(final JavaController controller) {
        String[] v = new String[1];
        controller.getObjectProperty(root.getUID(), root.getKind(), ObjectProperties.VERSION_NUMBER, v);
        return v[0];
    }

    /**
     * @return current debug level
     */
    public int getDebugLevel(final JavaController controller) {
        int[] v = new int[1];
        controller.getObjectProperty(root.getUID(), root.getKind(), ObjectProperties.DEBUG_LEVEL, v);
        return v[0];
    }

    /**
     * @param debugLevel
     *            change debug level
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setDebugLevel(final JavaController controller, int debugLevel) throws PropertyVetoException {
        int oldValue = getDebugLevel(controller);
        vcs.fireVetoableChange(DEBUG_LEVEL_CHANGE, oldValue, debugLevel);
        controller.setObjectProperty(root.getUID(), root.getKind(), ObjectProperties.DEBUG_LEVEL, debugLevel);
    }


    /**
     * Fill the hierarchy from the first element up to the root diagram
     * (included).
     *
     * @param controller the JavaController to use
     * @return the filled collection (the root at the end)
     */
    public Stack<ScicosObjectOwner> lookForHierarchy(JavaController controller) {
        Stack<ScicosObjectOwner> hierarchy = new Stack<>();

        long[] parent = new long[] {current.getUID()};
        if (current.getKind() == Kind.DIAGRAM) {
            hierarchy.push(current);
            return hierarchy;
        }

        while (parent[0] != 0l) {
            hierarchy.push(new ScicosObjectOwner(controller, parent[0], Kind.BLOCK));
            controller.getObjectProperty(parent[0], Kind.BLOCK, ObjectProperties.PARENT_BLOCK, parent);
        }

        controller.getObjectProperty(current.getUID(), current.getKind(), ObjectProperties.PARENT_DIAGRAM, parent);
        hierarchy.push(new ScicosObjectOwner(controller, parent[0], Kind.DIAGRAM));

        return hierarchy;
    }


    /**
     * Get the {@link ObjectProperties#DIAGRAM_CONTEXT} applicable to the cell children
     * @param controller current controller
     * @param cell current cell DIAGRAM or BLOCK
     * @return the user-entered context
     */
    public String[] getAllContext(JavaController controller) {
        final ArrayList<String> allContext = new ArrayList<>();
        final Stack<ScicosObjectOwner> hierarchy = lookForHierarchy(controller);

        final VectorOfString context = new VectorOfString();

        hierarchy.stream().forEach(o -> {
            // set the context first
            controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.DIAGRAM_CONTEXT, context);

            final int length = context.size();
            for (int i = 0; i < length; i++) {
                allContext.add(context.get(i));
            }
            allContext.add("");

            // in the case of mask presence, override previously defined variables
            VectorOfDouble vec = new VectorOfDouble();
            controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.EXPRS, vec);
            if (vec.size() > 0) {
                ScilabType rawExprs = new ScilabTypeCoder().vec2var(vec);
                if (rawExprs instanceof ScilabList)
                {
                    ScilabList exprs = (ScilabList) rawExprs;
                    if (exprs.size() == 2 && exprs.get(0) instanceof ScilabString && exprs.get(1) instanceof ScilabList)
                    {
                        ScilabString values = (ScilabString) exprs.get(0);
                        ScilabList descr = (ScilabList) exprs.get(1);
                        if (descr.size() > 1 && descr.get(0) instanceof ScilabString)
                        {
                            ScilabString varNames = (ScilabString) descr.get(0);
                            if (values.getHeight() == varNames.getHeight())
                            {
                                for (int i = 0; i < values.getHeight(); i++)
                                {
                                    allContext.add(varNames.getData()[i][0] + " = " + values.getData()[i][0]);
                                }
                            }
                        }
                    }
                }
            }
        });

        return allContext.toArray(new String[allContext.size()]);
    }


}
