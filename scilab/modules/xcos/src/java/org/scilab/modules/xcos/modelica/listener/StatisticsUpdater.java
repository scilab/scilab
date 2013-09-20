/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.modelica.listener;

import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

import org.scilab.modules.xcos.modelica.ModelStatistics;
import org.scilab.modules.xcos.modelica.TerminalAccessor;
import org.scilab.modules.xcos.modelica.TerminalTableModel;
import org.scilab.modules.xcos.modelica.model.Terminal;

/**
 * Update the statistics for the current model on table changed events.
 */
public final class StatisticsUpdater implements TableModelListener {

    private final ModelStatistics statistics;

    /**
     * Default constructor
     *
     * @param statistics
     *            the statistic instance
     */
    public StatisticsUpdater(ModelStatistics statistics) {
        this.statistics = statistics;
    }

    /**
     * Update the statistics on table change.
     *
     * @param e
     *            the model event
     * @see javax.swing.event.TableModelListener#tableChanged(javax.swing.event.TableModelEvent)
     */
    @Override
    public void tableChanged(TableModelEvent e) {
        // filter out classic events
        if (e instanceof TerminalTableModel.TerminalTableModelEvent) {
            final int rowIndex = e.getFirstRow();
            final int columnIndex = e.getColumn();

            // update the statistics on weight change
            if (TerminalAccessor.values()[columnIndex] == TerminalAccessor.WEIGHT) {
                final TerminalTableModel model = (TerminalTableModel) e
                                                 .getSource();
                final Terminal terminal = model.getTerminals().get(rowIndex);

                final int increment;
                if (((TerminalTableModel.TerminalTableModelEvent) e)
                        .isAfterCommit()) {
                    increment = 1;
                } else {
                    increment = -1;
                }

                final String kind = TerminalAccessor.getData(
                                        TerminalAccessor.KIND, terminal);
                final Double weight = TerminalAccessor.getData(
                                          TerminalAccessor.WEIGHT, terminal);
                if ("fixed_parameter".equals(kind)) {
                    if (weight.doubleValue() == 1) {
                        statistics.incFixedParameters(increment);
                    } else {
                        statistics.incRelaxedParameters(increment);
                    }
                } else if ("variable".equals(kind)) {
                    if (weight.doubleValue() == 1) {
                        statistics.incFixedVariables(increment);
                    } else {
                        statistics.incRelaxedVariables(increment);
                    }
                }

                // alert the listeners
                statistics.fireChange();
            }
        }
    }

}
