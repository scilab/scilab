/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2011-2014 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.modelica.listener;

import java.awt.event.ActionEvent;
import java.io.File;
import java.util.List;
import java.util.logging.Logger;

import javax.swing.AbstractAction;
import javax.swing.JProgressBar;
import javax.swing.SwingWorker;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.xcos.modelica.ModelStatistics;
import org.scilab.modules.xcos.modelica.Modelica;
import org.scilab.modules.xcos.modelica.ModelicaController;
import org.scilab.modules.xcos.modelica.ModelicaMessages;
import org.scilab.modules.xcos.modelica.model.Model;

/**
 * Solve the model.
 */
@SuppressWarnings(value = { "serial" })
public final class SolveAction extends AbstractAction {
    private static final String COMPILE_STRING = "fw='%s'; paremb='%s'; jaco='%s'; " + "if(compile_init_modelica(fw, paremb, jaco)) then "
            + "mopen('%s', 'w'); end ";
    private static final String COMPUTE_STRING = "method='%s'; Nunknowns='%s'; " + "if(Compute_cic(method,Nunknowns)) then " + "mopen('%s', 'w'); " + "end ";

    private static final String IMF_INIT = "_init";
    private static final String EXTENSION = ".xml";
    private static final String INCIDENCE = "i_incidence_matrix";

    private enum ActionToPerform {EXPORT, COMPILE, COMPUTE, FINISH};
    private class Worker extends SwingWorker<Void, ActionToPerform> {

        private final ModelicaController controller;
        private final JProgressBar progress;

        public Worker(ModelicaController controller, JProgressBar progress) {
            this.controller = controller;
            this.progress = progress;
        }

        @Override
        protected Void doInBackground() throws Exception {
            String cmd;

            final String modelName = controller.getRoot().getName();
            // creating a temporary status file
            final File statusFile = File.createTempFile(modelName, null);

            // defensive programming
            if (!controller.isSquare()) {
                Logger.getLogger(SolveAction.class.getName()).severe(ModelicaMessages.MODEL_INVALID);
            }

            /*
             * Export the compilation data to xml
             */
            publish(ActionToPerform.EXPORT);

            // store the updated values
            final File updatedInitFile = new File(ScilabConstants.TMPDIR, modelName + IMF_INIT + EXTENSION);
            Modelica.getInstance().save(controller.getRoot(), updatedInitFile);

            /*
             * Compile the data from xml to a modelica file
             */
            publish(ActionToPerform.COMPILE);
            final int paremb;
            if (controller.isParameterEmbedded()) {
                paremb = 1;
            } else {
                paremb = 0;
            }

            final int jaco;
            if (controller.isJacobianEnable()) {
                jaco = 1;
            } else {
                jaco = 0;
            }

            statusFile.delete();
            cmd = String.format(COMPILE_STRING, modelName, paremb, jaco, statusFile.getAbsolutePath());

            Logger.getLogger(SolveAction.class.getName()).finest("Compiling");
            ScilabInterpreterManagement.synchronousScilabExec(cmd);

            if (!statusFile.exists()) {
                publish(ActionToPerform.FINISH);
                return null;
            } else {
                statusFile.delete();
            }

            /*
             * Launch a modelica initialization with the generated file
             */
            publish(ActionToPerform.COMPUTE);

            // update the model with the computed identifiers
            final File incidence = new File(ScilabConstants.TMPDIR, modelName + INCIDENCE + EXTENSION);
            final Model incidenceModel = Modelica.getInstance().load(incidence);
            controller.getRoot().setIdentifiers(incidenceModel.getIdentifiers());
            controller.getRoot().setOutputs(incidenceModel.getOutputs());

            // update the statistics
            controller.getStatistics().fireChange();

            // update state
            controller.setCompileNeeded(false);

            statusFile.delete();
            final ModelStatistics stats = controller.getStatistics();
            long nUnknowns = stats.getUnknowns() - stats.getEquations();
            cmd = String.format(COMPUTE_STRING, controller.getComputeMethod(), nUnknowns, statusFile.getAbsolutePath());

            // compute now
            Logger.getLogger(SolveAction.class.getName()).finest("Computing");
            ScilabInterpreterManagement.synchronousScilabExec(cmd);

            if (!statusFile.exists()) {
                publish(ActionToPerform.FINISH);
                return null;
            } else {
                statusFile.delete();
            }

            /*
             * Finish the action using a state instead of the done() method
             */
            publish(ActionToPerform.FINISH);

            return null;
        }

        @Override
        protected void process(List<ActionToPerform> chunks) {
            // only visual notify accordingly to the last performed action
            final ActionToPerform action = chunks.get(chunks.size() - 1);

            switch (action) {
                case EXPORT:
                case COMPILE:
                case COMPUTE:
                    if (!progress.isIndeterminate()) {
                        progress.setIndeterminate(true);
                    }
                    break;

                case FINISH:
                default:
                    progress.setIndeterminate(false);
                    break;
            }
        }
    }

    private final ModelicaController controller;
    private final JProgressBar progress;

    /**
     * Default constructor
     *
     * @param controller
     *            the associated controller
     */
    public SolveAction(final ModelicaController controller, final JProgressBar progress) {
        super();

        putValue(NAME, ModelicaMessages.SOLVE);
        this.controller = controller;
        this.progress = progress;

    }

    /**
     * action !!!
     *
     * @param e
     *            the event
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        new Worker(controller, progress).execute();
    }
}
