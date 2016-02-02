/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
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
package org.scilab.modules.gui.ged.graphic_objects.properties;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Figure properties:
* ANTIALIASING, AUTORESIZE, AXESSIZE, CLOSEREQUESTFCN, EVENTHANDLER, IMMEDIATE_DRAWING,
* VIEWPORT, FIGURESIZE, FIGUREPOSITION, FIGUREID, PIXELDRAWINGMODE
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Figure extends ContentLayout {
    private JTextField cAxesSize;
    private JTextField cAxesSizeX;
    private JTextField cAxesSizeY;
    private JTextField cCloseRequestFcn;
    private JTextField cFigurePosition;
    private JTextField cFigurePositionX;
    private JTextField cFigurePositionY;
    private JTextField cFigureSize;
    private JTextField cFigureSizeX;
    private JTextField cFigureSizeY;
    private JTextField cViewport;
    private JTextField cViewportX;
    private JTextField cViewportY;

    /**
     * Components of the property: Anti-aliasing.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void antiAliasing(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lAliasing = new JLabel();
        final JComboBox cAliasing = new JComboBox();

        addLabelComboBox(panel, lAliasing, MessagesGED.anti_alising,
                         cAliasing, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cAliasing.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_ANTIALIASING__,
                    cAliasing.getSelectedIndex());
            }
        });

        // Get the current status of the property: Anti-aliasing
        cAliasing.setSelectedIndex((Integer) GraphicController.getController()
                                   .getProperty(UID, GraphicObjectProperties.__GO_ANTIALIASING__));
    }

    /**
     * Components of the property: Auto Resize.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void autoResize(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lResize = new JLabel();
        final JComboBox cResize = new JComboBox();

        addLabelComboBox(panel, lResize, MessagesGED.auto_resize,
                         cResize, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cResize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_AUTORESIZE__,
                    cResize.getSelectedIndex() == 0 ? false : true);
            }
        });

        // Get the current status of the property: Auto Resize
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_AUTORESIZE__);
        cResize.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Axes Size
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void axesSize(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        final JToggleButton bAxesSize = new JToggleButton();
        JLabel lAxesSize = new JLabel();
        cAxesSize = new JTextField();
        final JPanel pAxesSize = new JPanel();
        JLabel lAxesSizeX = new JLabel();
        cAxesSizeX = new JTextField();
        JLabel lAxesSizeY = new JLabel();
        cAxesSizeY = new JTextField();

        addInnerPanel(panel, pAxesSize, bAxesSize, lAxesSize, cAxesSize, MessagesGED.axes_size, ROW);
        bAxesSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pAxesSize.setVisible(!bAxesSize.isSelected());
            }
        });
        int rowAxesSize = 0;
        //Axes Size - X
        addLabelTextField(pAxesSize, lAxesSizeX, "X", cAxesSizeX, true, LEFTMARGINIP, COLUMNIP, rowAxesSize++);
        cAxesSizeX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesSize(UID);
            }
        });
        cAxesSizeX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesSize(UID);
            }
        });
        //Axes Size - Y
        addLabelTextField(pAxesSize, lAxesSizeY, "Y", cAxesSizeY, true, LEFTMARGINIP, COLUMNIP, rowAxesSize++);
        cAxesSizeY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateAxesSize(UID);
            }
        });
        cAxesSizeY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateAxesSize(UID);
            }
        });

        //Get the current status of the property: Axes Size
        Integer[] currentAxesSize = (Integer []) GraphicController.getController()
                                    .getProperty(UID, GraphicObjectProperties.__GO_AXES_SIZE__);
        cAxesSizeX.setText(currentAxesSize[0].toString());
        cAxesSizeY.setText(currentAxesSize[1].toString());
        titleAxesSize();
    }

    /**
    * Updates the property: Axes Size.
    */
    private void updateAxesSize(Integer UID) {
        Integer[] setPosition = new Integer[] {
            Integer.parseInt(cAxesSizeX.getText()),
            Integer.parseInt(cAxesSizeY.getText())
        };
        GraphicController.getController()
        .setProperty(UID, GraphicObjectProperties.__GO_AXES_SIZE__, setPosition);
        titleAxesSize();
    }

    /**
    * Inserts the current situation of the axes size.
    */
    public void titleAxesSize() {
        cAxesSize.setText(" [" + cAxesSizeX.getText() + " , " + cAxesSizeY.getText() + "]");
    }

    /**
     * Components of the property: CloseRequestFcn
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void closeRequestFcn(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lCloseRequestFcn = new JLabel();
        cCloseRequestFcn = new JTextField();

        addLabelTextField(panel, lCloseRequestFcn, MessagesGED.close_request_fcn,
                          cCloseRequestFcn, true, LEFTMARGIN, COLUMN, ROW++);
        cCloseRequestFcn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateCloseRequestFcn(UID);
            }
        });
        cCloseRequestFcn.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateCloseRequestFcn(UID);
            }
        });

        //Get the current status of the property: CloseRequestFcn
        cCloseRequestFcn.setText((String) GraphicController.getController()
                                 .getProperty(UID, GraphicObjectProperties.__GO_CLOSEREQUESTFCN__));
    }

    /**
    * Updates the property: CloseRequestFcn.
    */
    private void updateCloseRequestFcn(Integer UID) {
        GraphicController.getController().setProperty(
            UID, GraphicObjectProperties.__GO_CLOSEREQUESTFCN__,
            cCloseRequestFcn.getText());
    }


    /**
     * Components of the property: Event Handler
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void eventHandler(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lEventHandler = new JLabel();
        JComboBox cEventHandler = new JComboBox();
        addLabelComboBox(panel, lEventHandler, MessagesGED.event_handler,
                         cEventHandler, new String[] {MessagesGED.off, MessagesGED.function},
                         LEFTMARGIN, COLUMN, ROW++);
        cEventHandler.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                //not implemented yet
            }
        });
        //Get the current status of the property: Event Handle
        //not implemented yet.
    }

    /**
     * Components of the property: Figure ID
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void figureID(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lFigureID = new JLabel();
        final JTextField cFigureID = new JTextField();
        addLabelTextField(panel, lFigureID, MessagesGED.figure_id,
                          cFigureID, true, LEFTMARGIN, COLUMN, ROW++);
        cFigureID.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID,
                    GraphicObjectProperties.__GO_ID__,
                    Integer.parseInt(cFigureID.getText()));
            }
        });
        cFigureID.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                GraphicController.getController().setProperty(
                    UID,
                    GraphicObjectProperties.__GO_ID__,
                    Integer.parseInt(cFigureID.getText()));
            }
        });
        // Get the current status of the property: Figure ID
        cFigureID.setText(((Integer) GraphicController.getController()
                           .getProperty(UID, GraphicObjectProperties.__GO_ID__)).toString());
    }

    /**
     * Components of the property: Figure Name
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void figureName(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lFigureName = new JLabel();
        final JTextField cFigureName = new JTextField();
        addLabelTextField(panel, lFigureName, MessagesGED.figure_name,
                          cFigureName, true, LEFTMARGIN, COLUMN, ROW++);
        cFigureName.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID,
                    GraphicObjectProperties.__GO_NAME__,
                    cFigureName.getText());
            }
        });
        cFigureName.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                GraphicController.getController().setProperty(
                    UID,
                    GraphicObjectProperties.__GO_NAME__,
                    cFigureName.getText());
            }
        });
        // Get the current status of the property: Figure Name
        cFigureName.setText((String) GraphicController.getController()
                            .getProperty(UID, GraphicObjectProperties.__GO_NAME__));
    }

    /**
     * Components of the property: Figure Position
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void figurePosition(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        final JPanel pFigurePosition = new JPanel();
        JLabel lFigurePosition = new JLabel();
        final JToggleButton bFigurePosition = new JToggleButton();
        cFigurePosition = new JTextField();
        JLabel lFigurePositionX = new JLabel();
        cFigurePositionX = new JTextField();
        JLabel lFigurePositionY = new JLabel();
        cFigurePositionY = new JTextField();

        addInnerPanel(panel, pFigurePosition, bFigurePosition,
                      lFigurePosition, cFigurePosition, MessagesGED.figure_position, ROW);
        bFigurePosition.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pFigurePosition.setVisible(!bFigurePosition.isSelected());
            }
        });

        int rowFigurePosition = 0;
        //Figure Position - X
        addLabelTextField(pFigurePosition, lFigurePositionX, "X",
                          cFigurePositionX, true, LEFTMARGINIP, COLUMNIP, rowFigurePosition++);
        cFigurePositionX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateFigurePosition(UID);
            }
        });
        cFigurePositionX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateFigurePosition(UID);
            }
        });
        //Figure Position - Y
        addLabelTextField(pFigurePosition, lFigurePositionY, "Y",
                          cFigurePositionY, true, LEFTMARGINIP, COLUMNIP, rowFigurePosition++);
        cFigurePositionY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateFigurePosition(UID);
            }
        });
        cFigurePositionY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateFigurePosition(UID);
            }
        });

        //Get the current status of the property: Figure Position
        Integer[] currentFigurePosition = (Integer []) GraphicController.getController()
                                          .getProperty(UID, GraphicObjectProperties.__GO_POSITION__);
        cFigurePositionX.setText(currentFigurePosition[0].toString());
        cFigurePositionY.setText(currentFigurePosition[1].toString());
        titleFigurePosition();
    }

    /**
    * Updates the property: Figure Position.
    */
    private void updateFigurePosition(Integer UID) {
        Integer[] setPosition = new Integer[] {
            Integer.parseInt(cFigurePositionX.getText()),
            Integer.parseInt(cFigurePositionY.getText())
        };
        GraphicController.getController()
        .setProperty(UID, GraphicObjectProperties.__GO_POSITION__, setPosition);
        titleFigurePosition();
    }

    /**
    * Inserts the current situation of the Figure Position.
    */
    public void titleFigurePosition() {
        cFigurePosition.setText(" [" + cFigurePositionX.getText() + " , " + cFigurePositionY.getText() + "]");
    }

    /**
     * Components of the property: Figure Size
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void figureSize(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        final JPanel pFigureSize = new JPanel();
        JLabel lFigureSize = new JLabel();
        final JToggleButton bFigureSize = new JToggleButton();
        cFigureSize = new JTextField();
        JLabel lFigureSizeX = new JLabel();
        cFigureSizeX = new JTextField();
        JLabel lFigureSizeY = new JLabel();
        cFigureSizeY = new JTextField();

        addInnerPanel(panel, pFigureSize, bFigureSize,
                      lFigureSize, cFigureSize, MessagesGED.figure_size, ROW);
        bFigureSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pFigureSize.setVisible(!bFigureSize.isSelected());
            }
        });
        int rowFigureSize = 0;
        //Figure Size - X
        addLabelTextField(pFigureSize, lFigureSizeX, "X",
                          cFigureSizeX, true, LEFTMARGINIP, COLUMNIP, rowFigureSize++);
        cFigureSizeX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateFigureSize(UID);
            }
        });
        cFigureSizeX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateFigureSize(UID);
            }
        });
        //Figure Size - Y
        addLabelTextField(pFigureSize, lFigureSizeY, "Y",
                          cFigureSizeY, true, LEFTMARGINIP, COLUMNIP, rowFigureSize++);
        cFigureSizeY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateFigureSize(UID);
            }
        });
        cFigureSizeY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateFigureSize(UID);
            }
        });

        //Get the current status of the property: Figure Size
        Integer[] currentFigureSize = (Integer []) GraphicController.getController()
                                      .getProperty(UID, GraphicObjectProperties.__GO_SIZE__);
        cFigureSizeX.setText(currentFigureSize[0].toString());
        cFigureSizeY.setText(currentFigureSize[1].toString());
        titleFigureSize();
    }

    /**
    * Updates the property: Figure Size.
    */
    private void updateFigureSize(Integer UID) {
        Integer[] setPosition = new Integer[] {
            Integer.parseInt(cFigureSizeX.getText()),
            Integer.parseInt(cFigureSizeY.getText())
        };
        GraphicController.getController()
        .setProperty(UID, GraphicObjectProperties.__GO_SIZE__, setPosition);
        titleFigureSize();
    }

    /**
    * Inserts the current situation of the Figure Size.
    */
    public final void titleFigureSize() {
        cFigureSize.setText(" [" + cFigureSizeX.getText() + " , " + cFigureSizeY.getText() + "]");
    }

    /**
     * Components of the property: Info Message
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void infoMessage(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lInfoMessage = new JLabel();
        final JTextField cInfoMessage = new JTextField();
        addLabelTextField(panel, lInfoMessage, MessagesGED.info_message,
                          cInfoMessage, true, LEFTMARGIN, COLUMN, ROW++);
        cInfoMessage.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID,
                    GraphicObjectProperties.__GO_INFO_MESSAGE__,
                    cInfoMessage.getText());
            }
        });
        cInfoMessage.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                GraphicController.getController().setProperty(
                    UID,
                    GraphicObjectProperties.__GO_INFO_MESSAGE__,
                    cInfoMessage.getText());
            }
        });
        // Get the current status of the property: Info Message
        cInfoMessage.setText((String) GraphicController.getController()
                             .getProperty(UID, GraphicObjectProperties.__GO_INFO_MESSAGE__));
    }

    /**
     * Components of the property: Immediate Drawing
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void immediateDrawing(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lImmediateDrawing = new JLabel();
        final JComboBox cImmediateDrawing = new JComboBox();

        addLabelComboBox(panel, lImmediateDrawing, MessagesGED.immediate_drawing,
                         cImmediateDrawing, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cImmediateDrawing.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__,
                    cImmediateDrawing.getSelectedIndex() == 0 ? false : true);
            }
        });

        // Get the current status of the property: Immediate Drawing
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__);
        cImmediateDrawing.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Pixel Drawing Mode
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void pixelDrawingMode(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lPixelDrawingMode = new JLabel();
        final JComboBox cPixelDrawingMode = new JComboBox();

        String[] options = new String[] {"clear", "and", "andReverse", "copy", "andInverted", "noop",
                                         "xor", "or", "nor", "equiv", "invert", "orReverse", "copyInverted", "orInverted", "nand", "set"
                                        };
        addLabelComboBox(panel, lPixelDrawingMode, MessagesGED.pixel_drawing_mode,
                         cPixelDrawingMode, options,
                         LEFTMARGIN, COLUMN, ROW++);
        cPixelDrawingMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__,
                    cPixelDrawingMode.getSelectedIndex());
            }
        });

        //Get the current status of the property: Pixel Drawing Mode.
        cPixelDrawingMode.setSelectedIndex((Integer) GraphicController.getController()
                                           .getProperty(UID, GraphicObjectProperties.__GO_PIXEL_DRAWING_MODE__));
    }

    /**
     * Components of the property: ResizeFcn
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void resizeFcn(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lResizeFcn = new JLabel();
        final JTextField cResizeFcn = new JTextField();

        addLabelTextField(panel, lResizeFcn, MessagesGED.resize_fcn,
                          cResizeFcn, true, LEFTMARGIN, COLUMN, ROW++);
        cResizeFcn.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_RESIZEFCN__,
                    cResizeFcn.getText());
            }
        });
        cResizeFcn.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_RESIZEFCN__,
                    cResizeFcn.getText());
            }
        });

        //Get the current status of the property: ResizeFcn
        cResizeFcn.setText((String) GraphicController.getController()
                           .getProperty(UID, GraphicObjectProperties.__GO_RESIZEFCN__));
    }

    /**
     * Components of the property: Rotation Style
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void rotationStyle(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lRotationStyle = new JLabel();
        final JComboBox cRotationStyle = new JComboBox();

        addLabelComboBox(panel, lRotationStyle, MessagesGED.rotation_style,
                         cRotationStyle, new String[] {MessagesGED.unary, MessagesGED.multiple},
                         LEFTMARGIN, COLUMN, ROW++);
        cRotationStyle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_ROTATION_TYPE__,
                    cRotationStyle.getSelectedIndex());
            }
        });

        //Get the current status of the property: Rotation Style
        cRotationStyle.setSelectedIndex((Integer) GraphicController.getController()
                                        .getProperty(UID, GraphicObjectProperties.__GO_ROTATION_TYPE__));
    }

    /**
     * Components of the property: Viewport
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void viewport(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        final JPanel pViewport = new JPanel();
        JLabel lViewport = new JLabel();
        final JToggleButton bViewport = new JToggleButton();
        JLabel lViewportX = new JLabel();
        JLabel lViewportY = new JLabel();
        cViewport = new JTextField();
        cViewportX = new JTextField();
        cViewportY = new JTextField();

        addInnerPanel(panel, pViewport, bViewport, lViewport, cViewport, MessagesGED.viewport, ROW);
        bViewport.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pViewport.setVisible(!bViewport.isSelected());
            }
        });
        int rowViewport = 0;
        //Viewport - X
        addLabelTextField(pViewport, lViewportX, "X",
                          cViewportX, true, LEFTMARGINIP, COLUMNIP, rowViewport++);
        cViewportX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateViewport(UID);
            }
        });
        cViewportX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateViewport(UID);
            }
        });
        //Viewport - Y
        addLabelTextField(pViewport, lViewportY, "Y",
                          cViewportY, true, LEFTMARGINIP, COLUMNIP, rowViewport++);
        cViewportY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateViewport(UID);
            }
        });
        cViewportY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateViewport(UID);
            }
        });
        //Get the current status of the property: Viewport
        Integer[] currentViewport = (Integer []) GraphicController.getController()
                                    .getProperty(UID, GraphicObjectProperties.__GO_VIEWPORT__);
        cViewportX.setText(currentViewport[0].toString());
        cViewportY.setText(currentViewport[1].toString());
        titleViewport();
    }

    /**
    * Updates the property: Viewport.
    */
    private void updateViewport(Integer UID) {
        Integer[] setPosition = new Integer[] {
            Integer.parseInt(cViewportX.getText()),
            Integer.parseInt(cViewportY.getText())
        };
        GraphicController.getController()
        .setProperty(UID, GraphicObjectProperties.__GO_VIEWPORT__, setPosition);
        titleViewport();
    }

    /**
    * Inserts the current situation of the viewport.
    */
    public final void titleViewport() {
        cViewport.setText(" [" + cViewportX.getText() + " , " + cViewportY.getText() + "]");
    }
}
