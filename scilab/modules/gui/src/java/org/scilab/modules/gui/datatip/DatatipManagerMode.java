/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
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

package org.scilab.modules.gui.datatip;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.editor.ObjectSearcher;
import org.scilab.modules.gui.editor.EntityPicker;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * Manage the datatip mode activation
 * @author Gustavo Barbosa Libotte
 */
public class DatatipManagerMode {
    private static DatatipManagerMode instance;

    private Integer selectedTip;
    private Integer markColor;

    private Integer figureUid;
    private boolean datatipManagerModeStatus;

    private Map<Integer, Stack<Integer>> lastDatatips;

    private static String datatipMessage = Messages.gettext("Left click on the curve creates a datatip and right click on the datatip removes it.");

    public DatatipManagerMode() {
        instance = this;
        selectedTip = null;
        figureUid = null;
        lastDatatips = new HashMap<Integer, Stack<Integer>>();
        datatipManagerModeStatus = false;
    }

    public void setEnabled(boolean b) {
        datatipManagerModeStatus = b;
        if (!datatipManagerModeStatus) {
            GraphicController.getController().setProperty(figureUid, GraphicObjectProperties.__GO_INFO_MESSAGE__, "");
        } else {
            GraphicController.getController().setProperty(figureUid, GraphicObjectProperties.__GO_INFO_MESSAGE__, datatipMessage);
        }
    }

    /**
     * Get instance
     * @return instance
     */
    public static DatatipManagerMode getInstance() {
        return instance;
    }

    /**
     * Get the actual color of mark
     * @return markColor
     */
    public Integer getMarkColor() {
        return markColor;
    }

    /**
     * Set mark color
     * @param newColor scilab color
     */
    public void setMarkColor(Integer newColor) {
        markColor = newColor;
    }

    /**
     * Get the datatip's actual manager mode
     *
     * @return Datatip manager mode status.
     */
    public boolean isEnabled() {
        return datatipManagerModeStatus;
    }

    public void setSelectedTip(Integer uid) {
        markColor = highlightSelected(uid, selectedTip, markColor);
        selectedTip = uid;
    }

    public Integer getSelectedTip() {
        return selectedTip;
    }

    public void setLastTip(Integer axes, Integer uid) {
        Stack<Integer> stack = lastDatatips.get(axes);
        if (stack == null) {
            stack = new Stack<Integer>();
            lastDatatips.put(axes, stack);
        }

        stack.push(uid);
    }

    public Integer getLastTip(Integer axes) {
        Stack<Integer> stack = lastDatatips.get(axes);
        if (stack != null && !stack.isEmpty()) {
            return stack.pop();
        }

        return null;
    }

    public boolean hasLast(Integer[] pos) {
        Integer axes = AxesHandler.clickedAxes(getFigure(), pos);
        return axes != null && lastDatatips.containsKey(axes) && !lastDatatips.get(axes).isEmpty();
    }

    public void enableTranslation(boolean enabled) {
        DrawerVisitor.getVisitor(figureUid).getInteractionManager().setTranslationEnable(enabled);
    }

    public boolean isInteractiveZoom() {
        return DrawerVisitor.getVisitor(figureUid).getInteractionManager().isInteractiveZoom();
    }

    /**
     * Set figure uid
     *
     * @param uid Figure unique identifier.
     */
    public void setFigure(Integer uid) {
        figureUid = uid;
    }

    /**
     * Set figure uid
     *
     * @param uid Figure unique identifier.
     */
    public Integer getFigure() {
        return figureUid;
    }

    /**
     * Highlight the datatip mark when selected
     *
     * @param newTip datatip to highlight.
     * @param odTip datatip to restore the orginal color.
     * @param oldTipColor coor to restore.
     * @return the original datatip mark color.
     */
    private Integer highlightSelected(Integer newTip, Integer oldTip, Integer oldTipColor) {
        Integer color = 0;
        if (oldTip != null) {
            GraphicController.getController().setProperty(oldTip, GraphicObjectProperties.__GO_MARK_BACKGROUND__, oldTipColor);
        }
        if (newTip != null) {
            color = (Integer) GraphicController.getController().getProperty(newTip, GraphicObjectProperties.__GO_MARK_BACKGROUND__);
            GraphicController.getController().setProperty(newTip, GraphicObjectProperties.__GO_MARK_BACKGROUND__, -3);
        }
        return color;
    }


    public boolean pickAndHighlight(Integer x, Integer y) {
        Integer pos[] = {x, y};
        Integer datatip = (new EntityPicker()).pickDatatip(figureUid, pos);
        setSelectedTip(datatip);
        return (datatip != null);
    }

    public boolean pickAndDelete(Integer x, Integer y) {
        Integer pos[] = {x, y};
        Integer datatip = (new EntityPicker()).pickDatatip(figureUid, pos);

        if (datatip != null) {
            DatatipDelete.deleteDatatip(datatip);
            if (datatip.equals(selectedTip)) {
                selectedTip = null;
            }
        }
        return (datatip != null);
    }

    /**
     * On right mouse click, set popup menu visible.
     * @param event MouseEvent to retrieve click positon in figure.
     */
    public void onRightMouseClick(final MouseEvent e) {
        JPopupMenu menu = new JPopupMenu();
        JMenuItem deleteAll = new JMenuItem(Messages.gettext("Delete all the datatips"));
        deleteAll.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                deleteAll(new Integer[] {e.getX(), e.getY()});
            }
        });

        JMenuItem deleteAllOnCurve = new JMenuItem(Messages.gettext("Delete all the datatips on the selected curve"));
        deleteAllOnCurve.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                deleteAllOnCurve(new Integer[] {e.getX(), e.getY()});
            }
        });
        deleteAllOnCurve.setEnabled(onCurve(new Integer[] {e.getX(), e.getY()}));

        JMenuItem deleteNearest = new JMenuItem(Messages.gettext("Delete the nearest datatip"));
        deleteNearest.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                deleteNearest(new Integer[] {e.getX(), e.getY()});
            }
        });

        JMenuItem deleteLast = new JMenuItem(Messages.gettext("Delete the last datatip"));
        deleteLast.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                deleteLast(new Integer[] {e.getX(), e.getY()});
            }
        });
        deleteLast.setEnabled(hasLast(new Integer[] {e.getX(), e.getY()}));

        JMenuItem editDisplayFunction = new JMenuItem(Messages.gettext("Edit curve datatip display function"));
        editDisplayFunction.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                editDisplayFunction(e.getComponent(), new Integer[] {e.getX(), e.getY()});
            }
        });
        editDisplayFunction.setEnabled(onCurve(new Integer[] {e.getX(), e.getY()}));

        JMenuItem enableInterpolation = new JMenuItem(Messages.gettext("Enable interpolation"));
        enableInterpolation.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                enableInterpolation(new Integer[] {e.getX(), e.getY()}, true);
            }
        });
        enableInterpolation.setEnabled(onCurve(new Integer[] {e.getX(), e.getY()}));

        JMenuItem disableInterpolation = new JMenuItem(Messages.gettext("Disable interpolation"));
        disableInterpolation.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                enableInterpolation(new Integer[] {e.getX(), e.getY()}, false);
            }
        });
        disableInterpolation.setEnabled(onCurve(new Integer[] {e.getX(), e.getY()}));

        menu.add(deleteAll);
        menu.add(deleteAllOnCurve);
        menu.add(deleteNearest);
        menu.add(deleteLast);
        menu.addSeparator();
        menu.add(editDisplayFunction);
        menu.addSeparator();
        menu.add(enableInterpolation);
        menu.add(disableInterpolation);

        menu.show(e.getComponent(), e.getX(), e.getY());
    }

    /**
     * Enable interpolation
     * @param pos the mouse position
     * @param enabled to enable or not tuhe interpolation
     */
    private void enableInterpolation(Integer[] pos, boolean enabled) {
        Integer axes = AxesHandler.clickedAxes(getFigure(), pos);
        if (axes != null) {
            final Integer curve = (new EntityPicker()).pick(getFigure(), pos[0], pos[1]);
            if (curve != null) {
                DatatipCreate.datatipSetInterp(curve, enabled);
            }
        }
    }

    /**
     * Edit display function
     * @param c the component
     * @param pos the mouse position
     */
    private void editDisplayFunction(Component c, Integer[] pos) {
        Integer axes = AxesHandler.clickedAxes(getFigure(), pos);
        if (axes != null) {
            final Integer curve = (new EntityPicker()).pick(getFigure(), pos[0], pos[1]);
            if (curve != null) {
                String fnc = (String) GraphicController.getController().getProperty(curve, GraphicObjectProperties.__GO_DATATIP_DISPLAY_FNC__);
                JFrame parentWin = (JFrame) SwingUtilities.getAncestorOfClass(JFrame.class, c);
                final JDialog dialog = new JDialog(parentWin);
                dialog.setIconImage(new ImageIcon(FindIconHelper.findIcon("datatips")).getImage());
                dialog.setTitle(Messages.gettext("Display function name"));
                dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
                ScilabSwingUtilities.closeOnEscape(dialog);
                boolean isWin = System.getProperty("os.name").toLowerCase().contains("windows");
                JPanel main = new JPanel();
                final JTextField text = new JTextField(30);
                text.setText(fnc);
                JButton ok = new JButton(Messages.gettext("OK"));
                ActionListener valid = new ActionListener() {
                    public void actionPerformed(ActionEvent actionEvent) {
                        String s = text.getText();
                        if (s != null && !s.isEmpty()) {
                            GraphicController.getController().setProperty(curve, GraphicObjectProperties.__GO_DATATIP_DISPLAY_FNC__, s);
                        }
                        dialog.dispose();
                    }
                };

                ok.addActionListener(valid);
                text.addActionListener(valid);

                JButton cancel = new JButton(Messages.gettext("Cancel"));
                cancel.addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent actionEvent) {
                        dialog.dispose();
                    }
                });

                main.setLayout(new GridBagLayout());
                GridBagConstraints gbc = new GridBagConstraints();

                gbc.gridx = gbc.gridy = 0;
                gbc.gridwidth = 3;
                gbc.weightx = gbc.weighty = 1;
                gbc.fill = GridBagConstraints.HORIZONTAL;
                gbc.insets = new Insets(10, 5, 5, 5);
                main.add(text, gbc);

                gbc.gridx = 0;
                gbc.gridy = 1;
                gbc.weighty = 0;
                gbc.gridwidth = 1;
                gbc.fill = GridBagConstraints.HORIZONTAL;
                main.add(new JPanel(), gbc);

                gbc.gridx = isWin ? 1 : 2;
                gbc.weightx = gbc.weighty = 0;
                gbc.anchor = GridBagConstraints.LINE_START;
                gbc.fill = GridBagConstraints.NONE;
                gbc.insets = new Insets(0, 0, 5, 5);
                main.add(ok, gbc);

                gbc.gridx = isWin ? 2 : 1;
                main.add(cancel, gbc);

                dialog.setContentPane(main);
                dialog.pack();

                Point loc = parentWin.getLocationOnScreen();
                Dimension pdim = parentWin.getSize();
                Dimension ddim = dialog.getSize();
                dialog.setLocation(loc.x + (pdim.width - ddim.width) / 2, loc.y + (pdim.height - ddim.height) / 2);
                dialog.setVisible(true);

                text.requestFocus();
            }
        }
    }

    /**
     * Delete last datatip
     * @param pos the mouse position
     */
    private void deleteLast(Integer[] pos) {
        Integer axes = AxesHandler.clickedAxes(getFigure(), pos);
        if (axes != null) {
            Integer datatip = getLastTip(axes);
            if (datatip != null) {
                DatatipDelete.deleteDatatip(datatip);
            }
        }
    }

    /**
     * Delete the nearest datatip
     * @param pos the mouse position
     */
    private void deleteNearest(Integer[] pos) {
        Integer axes = AxesHandler.clickedAxes(getFigure(), pos);
        if (axes != null) {
            Integer[] datatips = (new ObjectSearcher()).search(axes, GraphicObjectProperties.__GO_DATATIP__, true);
            if (datatips != null) {
                int x = pos[0];
                int y = pos[1];
                double min = Double.POSITIVE_INFINITY;
                int datatip = -1;
                for (int i : datatips) {
                    Double[] tip_pos = (Double[]) GraphicController.getController().getProperty(i, GraphicObjectProperties.__GO_DATATIP_DATA__);
                    double[] point = new double[] {tip_pos[0], tip_pos[1], tip_pos[2]};
                    double[] pix = CallRenderer.getPixelFrom3dCoordinates(axes, point);
                    double d = Math.hypot(x - pix[0], y - pix[1]);
                    if (d < min) {
                        min = d;
                        datatip = i;
                    }
                }

                if (datatip != -1) {
                    DatatipDelete.deleteDatatip(datatip);
                }
            }
        }
    }

    /**
     * Delete all datatips on a curve
     * @param pos the mouse position
     */
    private void deleteAllOnCurve(Integer[] pos) {
        Integer axes = AxesHandler.clickedAxes(getFigure(), pos);
        if (axes != null) {
            Integer curve = (new EntityPicker()).pick(getFigure(), pos[0], pos[1]);
            if (curve != null) {
                Integer[] datatips = (Integer[]) GraphicController.getController().getProperty(curve, GraphicObjectProperties.__GO_DATATIPS__);
                if (datatips != null) {
                    for (int i : datatips) {
                        DatatipDelete.deleteDatatip(i);
                    }
                }
            }
        }
    }

    /**
     * Delete all datatips
     * @param pos the mouse position
     */
    private void deleteAll(Integer[] pos) {
        Integer axes = AxesHandler.clickedAxes(getFigure(), pos);
        if (axes != null) {
            Integer[] datatips = (new ObjectSearcher()).search(axes, GraphicObjectProperties.__GO_DATATIP__, true);
            if (datatips != null) {
                for (int i : datatips) {
                    DatatipDelete.deleteDatatip(i);
                }
            }
        }
    }

    /**
     * Check if we are on a curve
     * @param pos the mouse position
     * @return true if the click occured on a curve
     */
    private boolean onCurve(Integer[] pos) {
        Integer axes = AxesHandler.clickedAxes(getFigure(), pos);
        if (axes != null) {
            Integer curve = (new EntityPicker()).pick(getFigure(), pos[0], pos[1]);
            return curve != null;
        }

        return false;
    }
}
