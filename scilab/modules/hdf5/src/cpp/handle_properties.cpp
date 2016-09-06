/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include "handle_properties.hxx"
#include "double.hxx"

extern "C"
{
#include "h5_fileManagement.h"
#include "h5_writeDataToFile.h"
#include "h5_readDataFromFile.h"
#include "h5_attributeConstants.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"
#include "FigureList.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"
#include "Matplot.h"
#include "HandleManagement.h"
}

extern types::InternalType* import_data(int dataset);
extern int export_data(int parent, const std::string& name, types::InternalType* data);

static int getHandleInt(int dataset, const char* prop, int* val)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    readInteger32Matrix(node, val);
    return 0;
}

static int getHandleIntVector(int dataset, const char* prop, int* row, int* col, int** vals)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return -1;
    }


    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size <= 0)
    {
        closeDataSet(node);
        return -1;
    }

    *row = d[0];
    *col = d[1];
    *vals = new int[size];
    readInteger32Matrix(node, *vals);
    return 0;
}

static int getHandleBool(int dataset, const char* prop, int* val)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    readBooleanMatrix(node, val);
    return 0;
}

static int getHandleBoolVector(int dataset, const char* prop, int* row, int* col, int** vals)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return -1;
    }


    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size <= 0)
    {
        closeDataSet(node);
        return -1;
    }

    *row = d[0];
    *col = d[1];

    *vals = new int[size];
    readBooleanMatrix(node, *vals);
    return 0;
}

static double getHandleDouble(int dataset, const char* prop, double* val)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    readDoubleMatrix(node, val);
    return 0;
}

static int getHandleDoubleVector(int dataset, const char* prop, int* row, int* col, double** vals)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return -1;
    }


    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size <= 0)
    {
        closeDataSet(node);
        return -1;
    }

    *row = d[0];
    *col = d[1];

    *vals = new double[size];
    readDoubleMatrix(node, *vals);
    return 0;
}

static int getHandleString(int dataset, const char* prop, char** val)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return -1;
    }


    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size <= 0)
    {
        closeDataSet(node);
        return -1;
    }

    readStringMatrix(node, val);
    return node;
}

static int getHandleStringVector(int dataset, const char* prop, int* row, int* col, char*** vals)
{
    int node = 0;
    node = getDataSetIdFromName(dataset, prop);
    if (node < 0)
    {
        return -1;
    }

    int complex = 0;
    int dims = 0;;
    int ret = getDatasetInfo(node, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return -1;
    }


    std::vector<int> d(dims);
    int size = getDatasetInfo(node, &complex, &dims, d.data());

    if (dims == 0 || size <= 0)
    {
        closeDataSet(node);
        return -1;
    }

    *row = d[0];
    *col = d[1];

    *vals = new char*[size];
    readStringMatrix(node, *vals);
    return node;
}

static int import_handle_generic(int dataset, int uid, int parent, const HandleProp& props, bool childrenFirst);

static void import_userdata(int dataset, int uid)
{
    types::InternalType* ud = nullptr;
    int node = 0;
    node = getDataSetIdFromName(dataset, "userdata");
    if (node < 0)
    {
        //no user data ?
        //assign an empty matrix
        ud = types::Double::Empty();
    }
    else
    {
        ud = import_data(node);
    }

    //increase ref before trying to delete old value to avoid double free
    ud->IncreaseRef();

    //get previous value
    int size = 0;
    int *psize = &size;
    int *data = NULL;

    getGraphicObjectProperty(uid, __GO_USER_DATA_SIZE__, jni_int, (void **)&psize);
    getGraphicObjectProperty(uid, __GO_USER_DATA__, jni_int_vector, (void **)&data);

    if (size != 0)
    {
        types::InternalType* previous = nullptr;
        if (size == 1)
        {
            //32 bits
            int* p = (int*)data;
            previous = ((types::InternalType*) * p);
        }
        else
        {
            //64 bits
            long long* p = (long long*)data;
            previous = ((types::InternalType*) * p);
        }

        previous->DecreaseRef();
        previous->killMe();
    }

    //set new value
    size = sizeof(void*) / sizeof(int);
    setGraphicObjectProperty(uid, __GO_USER_DATA__, &ud, jni_int_vector, size);
}

static void import_handle_tag(int dataset, int uid)
{
    char* tag = nullptr;
    int node = getHandleString(dataset, "tag", &tag);
    setGraphicObjectProperty(uid, __GO_TAG__, tag, jni_string, 1);
    freeStringMatrix(node, &tag);
}

static int import_handle_children(int dataset, int parent)
{
    //reload children
    int children = getDataSetIdFromName(dataset, "children");
    int childcount = 0;
    getListDims6(children, &childcount);

    //reverse order
    for (int i = childcount - 1; i >= 0; --i)
    {
        int c = getDataSetIdFromName(children, std::to_string(i).data());
        int newChild = import_handle(c, parent);
    }

    closeList6(children);
    return parent;
}

static int import_handle_generic(int dataset, int uid, int parent, const HandleProp& props, bool childrenFirst)
{
    //link current handle with its parent
    if (parent != -1)
    {
        setGraphicObjectRelationship(parent, uid);
    }
    //restore children before other property in case of properties has an
    //effect on children

    //reload children
    if (childrenFirst)
    {
        import_handle_children(dataset, uid);
    }

    for (auto & prop : props)
    {
        const char* name = prop.first.data();
        std::vector<int> info(prop.second);

        if (info[0] == SAVE_ONLY)
        {
            continue;
        }

        int go = info[1];
        int type = info[2];
        int row = 0;
        int col = 0;

        switch (type)
        {
            case jni_bool:
            {
                int val = 0;
                getHandleBool(dataset, name, &val);
                setGraphicObjectProperty(uid, go, &val, jni_bool, 1);
                break;
            }
            case jni_int:
            {
                int val = 0;
                getHandleInt(dataset, name, &val);
                setGraphicObjectProperty(uid, go, &val, jni_int, 1);
                break;
            }
            case jni_double:
            {
                double val = 0;
                getHandleDouble(dataset, name, &val);
                setGraphicObjectProperty(uid, go, &val, jni_double, 1);
                break;
            }
            case jni_string:
            {
                char* data = nullptr;
                int node = getHandleString(dataset, name, &data);
                setGraphicObjectProperty(uid, go, data, jni_string, 1);
                freeStringMatrix(node, &data);
                break;
            }
            case jni_bool_vector:
            {
                int* vals = nullptr;
                getHandleBoolVector(dataset, name, &row, &col, &vals);
                if (vals)
                {
                    setGraphicObjectProperty(uid, go, vals, jni_bool_vector, row * col);
                    delete[] vals;
                }
                break;
            }
            case jni_int_vector:
            {
                int* vals = nullptr;
                getHandleIntVector(dataset, name, &row, &col, &vals);
                if (vals)
                {
                    setGraphicObjectProperty(uid, go, vals, jni_int_vector, row * col);
                    delete[] vals;
                }
                break;
            }
            case jni_double_vector:
            {
                double* vals = nullptr;
                getHandleDoubleVector(dataset, name, &row, &col, &vals);
                if (vals)
                {
                    setGraphicObjectProperty(uid, go, vals, jni_double_vector, row * col);
                    delete[] vals;
                }
                break;
            }
            case jni_string_vector:
            {
                char** vals = nullptr;
                int node = getHandleStringVector(dataset, name, &row, &col, &vals);
                if (vals)
                {
                    setGraphicObjectProperty(uid, go, vals, jni_string_vector, row * col);
                }

                freeStringMatrix(node, vals);
                delete[] vals;
                break;
            }
        }
    }

    //userdata
    import_userdata(dataset, uid);

    //tag
    import_handle_tag(dataset, uid);

    //reload children
    if (childrenFirst == false)
    {
        import_handle_children(dataset, uid);
    }

    return uid;
}

static int import_handle_border(int dataset);

static int import_handle_border_none(int dataset, int border)
{
    closeList6(dataset);
    return border;
}

static int import_handle_border_line(int dataset, int border)
{
    int status = 0;
    //color
    char* color = nullptr;
    int nc = getHandleString(dataset, "color", &color);
    setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_COLOR__, color, jni_string, 1);
    freeStringMatrix(nc, &color);

    //thickness
    int thickness = 0;
    status = getHandleInt(dataset, "thickness", &thickness);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_LINE_THICKNESS__, &thickness, jni_int, 1);
    }

    //rounded
    int rounded = 0;
    status = getHandleBool(dataset, "rounded", &rounded);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_ROUNDED__, &rounded, jni_bool, 1);
    }

    closeList6(dataset);
    return border;
}

static int import_handle_border_bevel(int dataset, int border)
{
    char* data = nullptr;
    int node = 0;
    //type
    int type = 0;
    getHandleInt(dataset, "type", &type);
    setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_TYPE__, &type, jni_int, 1);

    //highlight out
    node = getHandleString(dataset, "highlight_out", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //highlight in
    node = getHandleString(dataset, "highlight_in", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //shadow out
    node = getHandleString(dataset, "shadow_out", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_SHADOW_OUT__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //shadow in
    node = getHandleString(dataset, "shadow_in", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_SHADOW_IN__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    closeList6(dataset);
    return border;
}

static int import_handle_border_soft_bevel(int dataset, int border)
{
    return import_handle_border_bevel(dataset, border);
}

static int import_handle_border_etched(int dataset, int border)
{
    int status = 0;
    char* data = nullptr;
    int node = 0;

    //type
    int type = 0;
    status = getHandleInt(dataset, "type", &type);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_TYPE__, &type, jni_int, 1);
    }

    //highlight out
    node = getHandleString(dataset, "highlight_out", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //shadow out
    node = getHandleString(dataset, "shadow_out", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_SHADOW_OUT__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    closeList6(dataset);
    return border;
}

static int import_handle_border_titled(int dataset, int border)
{
    char* data = nullptr;
    int node = 0;
    int status = 0;

    //title border
    int title_border = getDataSetIdFromName(dataset, "title_border");
    if (title_border != -1)
    {
        int hidden = 1;
        int uiborder = import_handle_border(title_border);
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_TITLE__, &uiborder, jni_int, 1);
        //force new border to be hide in scilab view
        setGraphicObjectProperty(uiborder, __GO_HIDDEN__, &hidden, jni_bool, 1);
    }

    //title
    node = getHandleString(dataset, "title", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_TITLE__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //justification
    int justification = 0;
    status = getHandleInt(dataset, "justification", &justification);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_JUSTIFICATION__, &justification, jni_int, 1);
    }

    //fontname
    node = getHandleString(dataset, "fontname", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FONTNAME__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //fontangle
    node = getHandleString(dataset, "fontangle", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FONTANGLE__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //fontsize
    int fontsize = 0;
    status = getHandleInt(dataset, "fontsize", &fontsize);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_UI_FONTSIZE__, &fontsize, jni_int, 1);
    }

    //fontweight
    node = getHandleString(dataset, "fontweight", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FONTWEIGHT__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    //position
    int position = 0;
    status = getHandleInt(dataset, "position", &position);
    if (status != -1)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_POSITION__, &position, jni_int, 1);
    }

    //color
    node = getHandleString(dataset, "color", &data);
    if (data)
    {
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_COLOR__, data, jni_string, 1);
    }

    freeStringMatrix(node, &data);
    data = nullptr;

    closeList6(dataset);
    return border;
}

static int import_handle_border_empty(int dataset, int border)
{
    int row = 0;
    int col = 0;
    double* pos = nullptr;

    getHandleDoubleVector(dataset, "position", &row, &col, &pos);
    if (pos && row * col == 4)
    {
        setGraphicObjectProperty(border, __GO_POSITION__, pos, jni_double_vector, row * col);
    }
    delete[] pos;

    closeList6(dataset);
    return border;
}

static int import_handle_border_compound(int dataset, int border)
{
    //out_border
    int out_border = getDataSetIdFromName(dataset, "out_border");
    if (out_border != -1)
    {
        int hidden = 1;
        int uiborder = import_handle_border(out_border);
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_OUT_BORDER__, &uiborder, jni_int, 1);
        //force new border to be hide in scilab view
        setGraphicObjectProperty(uiborder, __GO_HIDDEN__, &hidden, jni_bool, 1);
    }

    //in_border
    int in_border = getDataSetIdFromName(dataset, "in_border");
    if (in_border != -1)
    {
        int hidden = 1;
        int uiborder = import_handle_border(in_border);
        setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_IN_BORDER__, &uiborder, jni_int, 1);
        //force new border to be hide in scilab view
        setGraphicObjectProperty(uiborder, __GO_HIDDEN__, &hidden, jni_bool, 1);
    }

    closeList6(dataset);
    return border;
}

static int import_handle_border_matte(int dataset, int border)
{
    int row = 0;
    int col = 0;
    double* pos = nullptr;

    getHandleDoubleVector(dataset, "position", &row, &col, &pos);
    setGraphicObjectProperty(border, __GO_POSITION__, pos, jni_double_vector, row * col);
    delete[] pos;

    //color
    char* data = nullptr;
    int nc = getHandleString(dataset, "color", &data);
    setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_COLOR__, data, jni_string, 1);
    freeStringMatrix(nc, &data);

    closeList6(dataset);
    return border;
}

static int import_handle_border(int dataset)
{
    int border = createGraphicObject(__GO_UI_FRAME_BORDER__);
    int style = 0;
    getHandleInt(dataset, "style", &style);

    setGraphicObjectProperty(border, __GO_UI_FRAME_BORDER_STYLE__, &style, jni_int, 1);

    switch (style)
    {
        default:
        case NONE:
            return import_handle_border_none(dataset, border);
        case LINE:
            return import_handle_border_line(dataset, border);
        case BEVEL:
            return import_handle_border_bevel(dataset, border);
        case SOFTBEVEL:
            return import_handle_border_soft_bevel(dataset, border);
        case ETCHED:
            return import_handle_border_etched(dataset, border);
        case TITLED:
            return import_handle_border_titled(dataset, border);
        case EMPTY:
            return import_handle_border_empty(dataset, border);
        case COMPOUND:
            return import_handle_border_compound(dataset, border);
        case MATTE:
            return import_handle_border_matte(dataset, border);
    }
}

static int import_handle_uicontrol(int dataset, int parent)
{
    int style = 0;
    getHandleInt(dataset, "style", &style);
    //uicontrol was created by their style instead of type like others graphic objects.
    int uic = createGraphicObject(style);

    //some properties must be set before parent ( in import_handle_generic )

    //scrollable
    int scrollable = 0;
    getHandleBool(dataset, "scrollable", &scrollable);
    setGraphicObjectProperty(uic, __GO_UI_SCROLLABLE__, &scrollable, jni_bool, 1);

    //margins
    int row = 0;
    int col = 0;

    double* margins = nullptr;
    getHandleDoubleVector(dataset, "margins", &row, &col, &margins);
    setGraphicObjectProperty(uic, __GO_MARGINS__, margins, jni_double_vector, row * col);
    delete[] margins;

    //constraints

    //border_position
    int border_position = 0;
    getHandleInt(dataset, "border_position", &border_position);
    setGraphicObjectProperty(uic, __GO_UI_BORDER_POSITION__, &border_position, jni_int, 1);

    //border_size
    int* border_size = nullptr;
    getHandleIntVector(dataset, "border_size", &row, &col, &border_size);
    setGraphicObjectProperty(uic, __GO_UI_BORDER_PREFERREDSIZE__, border_size, jni_int_vector, row * col);
    delete[] border_size;

    //gridbad_grid
    int* gridbad_grid = nullptr;
    getHandleIntVector(dataset, "gridbad_grid", &row, &col, &gridbad_grid);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_GRID__, gridbad_grid, jni_int_vector, row * col);
    delete[] gridbad_grid;

    //gridbad_weight
    double* gridbad_weight = nullptr;
    getHandleDoubleVector(dataset, "gridbad_weight", &row, &col, &gridbad_weight);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_WEIGHT__, gridbad_weight, jni_double_vector, row * col);
    delete[] gridbad_weight;

    //gridbad_fill
    int gridbad_fill = 0;
    getHandleInt(dataset, "gridbad_fill", &gridbad_fill);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_FILL__, &gridbad_fill, jni_int, 1);

    //gridbad_anchor
    int gridbad_anchor = 0;
    getHandleInt(dataset, "gridbad_anchor", &gridbad_anchor);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_ANCHOR__, &gridbad_anchor, jni_int, 1);

    //gridbad_padding
    int* gridbad_padding = nullptr;
    getHandleIntVector(dataset, "gridbad_padding", &row, &col, &gridbad_padding);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_PADDING__, gridbad_padding, jni_int_vector, row * col);
    delete[] gridbad_padding;

    //gridbad_size
    int* gridbad_size = nullptr;
    getHandleIntVector(dataset, "gridbad_size", &row, &col, &gridbad_size);
    setGraphicObjectProperty(uic, __GO_UI_GRIDBAG_PREFERREDSIZE__, gridbad_size, jni_int_vector, row * col);
    delete[] gridbad_size;

    //import "standards" properties
    import_handle_generic(dataset, uic, parent, UicontrolHandle::getPropertyList(), false);

    //string
    char** string = nullptr;
    int node = getHandleStringVector(dataset, "string", &row, &col, &string);
    setGraphicObjectProperty(uic, __GO_UI_STRING_COLNB__, &col, jni_int, 1);
    setGraphicObjectProperty(uic, __GO_UI_STRING__, string, jni_string_vector, row * col);
    freeStringMatrix(node, string);
    delete[] string;

    //border
    int dborder = getDataSetIdFromName(dataset, "border");
    int border = import_handle_border(dborder);
    setGraphicObjectProperty(uic, __GO_UI_FRAME_BORDER__, &border, jni_int, 1);


    //value
    double* value = nullptr;
    getHandleDoubleVector(dataset, "value", &row, &col, &value);
    if (value)
    {
        setGraphicObjectProperty(uic, __GO_UI_VALUE__, value, jni_double_vector, row * col);
        delete[] value;
    }

    closeList6(dataset);
    return uic;
}

static int import_handle_uicontextmenu(int dataset, int parent)
{
    int menu = createGraphicObject(__GO_UICONTEXTMENU__);

    //import "standards" properties
    import_handle_generic(dataset, menu, parent, UicontextmenuHandle::getPropertyList(), false);

    closeList6(dataset);
    return menu;
}

static int import_handle_uimenu(int dataset, int parent)
{
    int menu = createGraphicObject(__GO_UIMENU__);

    //import "standards" properties
    import_handle_generic(dataset, menu, parent, UimenuHandle::getPropertyList(), false);

    closeList6(dataset);
    return menu;
}

static int import_handle_light(int dataset, int parent)
{
    int light = createGraphicObject(__GO_LIGHT__);

    //import "standards" properties
    import_handle_generic(dataset, light, parent, LightHandle::getPropertyList(), true);

    closeList6(dataset);
    return light;
}

static int import_handle_axis(int dataset, int parent)
{
    int axis = createGraphicObject(__GO_AXIS__);

    //import "standards" properties
    import_handle_generic(dataset, axis, parent, AxisHandle::getPropertyList(), true);

    closeList6(dataset);
    return axis;
}

static int import_handle_text(int dataset, int parent)
{
    int t = createGraphicObject(__GO_TEXT__);

    //import "standards" properties
    import_handle_generic(dataset, t, parent, TextHandle::getPropertyList(), true);


    //text
    int dims[2];
    char** text = nullptr;
    int textnode = getHandleStringVector(dataset, "text", &dims[0], &dims[1], &text);
    setGraphicObjectProperty(t, __GO_TEXT_ARRAY_DIMENSIONS__, dims, jni_int_vector, 2);
    setGraphicObjectProperty(t, __GO_TEXT_STRINGS__, text, jni_string_vector, dims[0] * dims[1]);
    freeStringMatrix(textnode, text);
    delete[] text;
    closeList6(dataset);
    return t;
}

static int import_handle_legend(int dataset, int parent)
{
    int legend = createGraphicObject(__GO_LEGEND__);

    //import "standards" properties
    import_handle_generic(dataset, legend, parent, LegendHandle::getPropertyList(), true);


    //text
    int dims[2];
    char** text = nullptr;
    int textnode = getHandleStringVector(dataset, "text", &dims[0], &dims[1], &text);
    setGraphicObjectProperty(legend, __GO_TEXT_ARRAY_DIMENSIONS__, dims, jni_int_vector, 2);
    setGraphicObjectProperty(legend, __GO_TEXT_STRINGS__, text, jni_string_vector, dims[0] * dims[1]);
    freeStringMatrix(textnode, text);
    delete[] text;
    //links

    //to retore links we need to have the entire hierarchie loaded.
    //store links information in a "global" variable and update variable at the end of process.
    int node = getDataSetIdFromName(dataset, "links");
    int count = 0;
    getListDims6(node, &count);

    Links::PathList paths;

    for (int i = 0; i < count; ++i)
    {
        int row = 0;
        int col = 0;
        int* path = nullptr;
        getHandleIntVector(node, std::to_string(i).data(), &row, &col, &path);
        std::vector<int> res(row * col);
        for (int j = 0; j < row * col; ++j)
        {
            res[j] = path[j];
        }

        delete[] path;

        paths.push_back(res);
    }

    Links::add(legend, paths);

    closeList6(node);
    closeList6(dataset);
    return legend;
}

static int import_handle_fec(int dataset, int parent)
{
    int fec = createGraphicObject(__GO_FEC__);
    createDataObject(fec, __GO_FEC__);

    //import "standards" properties
    import_handle_generic(dataset, fec, parent, FecHandle::getPropertyList(), true);

    //triangles
    int coordR = 0;
    int coordC = 0;
    int valuesR = 0;
    int valuesC = 0;
    int trianglesR = 0;
    int trianglesC = 0;

    double* coords = nullptr;
    getHandleDoubleVector(dataset, "coords", &coordR, &coordC, &coords);
    double* values = nullptr;
    getHandleDoubleVector(dataset, "values", &valuesR, &valuesC, &values);
    double* triangles = nullptr;
    getHandleDoubleVector(dataset, "triangles", &trianglesR, &trianglesC, &triangles);
    int realcol = trianglesC - 2;

    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_NUM_VERTICES__, &valuesC, jni_int, 1);
    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, &realcol, jni_int, 1);
    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_NUM_INDICES__, &trianglesR, jni_int, 1);
    //setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_X__, coords, jni_double_vector, coordR);
    //setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_Y__, coords + coordR, jni_double_vector, coordR);
    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_COORDINATES__, coords, jni_double_vector, coordC);
    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_FEC_ELEMENTS__, triangles, jni_double_vector, trianglesR);
    setGraphicObjectPropertyAndNoWarn(fec, __GO_DATA_MODEL_VALUES__, values, jni_double_vector, valuesC);

    delete[] coords;
    delete[] values;
    delete[] triangles;

    closeList6(dataset);
    return fec;
}

static int import_handle_matplot(int dataset, int parent)
{
    int plot = createGraphicObject(__GO_MATPLOT__);
    createDataObject(plot, __GO_MATPLOT__);

    //import "standards" properties
    import_handle_generic(dataset, plot, parent, MatplotHandle::getPropertyList(), true);

    int row = 0;
    int col = 0;
    //rect
    double* rect = nullptr;
    getHandleDoubleVector(dataset, "rect", &row, &col, &rect);
    double tmp = rect[1];
    rect[1] = rect[2];
    rect[2] = tmp;

    //data
    getHandleInt(dataset, "num_x", &row);
    getHandleInt(dataset, "num_y", &col);
    double* data = nullptr;
    int data_x = 0;
    int data_y = 0;
    getHandleDoubleVector(dataset, "data", &data_x, &data_y, &data);

    int grid[4];
    grid[0] = col;
    grid[1] = 1;
    grid[2] = row;
    grid[3] = 1;

    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_GRID_SIZE__, grid, jni_int_vector, 4);

    double scale[2];
    setGraphicObjectProperty(plot, __GO_MATPLOT_TRANSLATE__, rect, jni_double_vector, 2);
    scale[0] = (rect[2] - rect[0]) / (col - 1.0);
    scale[1] = (rect[3] - rect[1]) / (row - 1.0);
    setGraphicObjectProperty(plot, __GO_MATPLOT_SCALE__, scale, jni_double_vector, 2);

    setGraphicObjectProperty(plot, __GO_DATA_MODEL_MATPLOT_BOUNDS__, rect, jni_double_vector, 4);

    setGraphicObjectProperty(plot, __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__, data, jni_double_vector, data_x * data_y);

    delete[] rect;
    delete[] data;

    closeList6(dataset);
    return plot;
}

static int import_handle_grayplot(int dataset, int parent)
{
    int plot = createGraphicObject(__GO_GRAYPLOT__);
    createDataObject(plot, __GO_GRAYPLOT__);

    //import "standards" properties
    import_handle_generic(dataset, plot, parent, GrayplotHandle::getPropertyList(), true);

    int row = 0;
    int col = 0;
    double* dataX = nullptr;
    double* dataY = nullptr;
    double* dataZ = nullptr;
    getHandleDoubleVector(dataset, "data_x", &row, &col, &dataX);
    getHandleDoubleVector(dataset, "data_y", &row, &col, &dataY);
    getHandleDoubleVector(dataset, "data_z", &row, &col, &dataZ);

    int grid[4];
    grid[0] = row;
    grid[1] = 1;
    grid[2] = col;
    grid[3] = 1;

    //set dimensions
    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_GRID_SIZE__, grid, jni_int_vector, 4);

    //set data
    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_X__, dataX, jni_double_vector, row);
    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, col);
    setGraphicObjectProperty(plot, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, row * col);

    closeList6(dataset);
    return plot;
}

static int import_handle_segs(int dataset, int parent)
{
    int segs = createGraphicObject(__GO_SEGS__);

    //import "standards" properties
    import_handle_generic(dataset, segs, parent, SegsHandle::getPropertyList(), true);

    closeList6(dataset);
    return segs;
}

static int import_handle_arc(int dataset, int parent)
{
    int arc = createGraphicObject(__GO_ARC__);

    //import "standards" properties
    import_handle_generic(dataset, arc, parent, ArcHandle::getPropertyList(), true);

    closeList6(dataset);
    return arc;
}

static int import_handle_rectangle(int dataset, int parent)
{
    int rect = createGraphicObject(__GO_RECTANGLE__);

    //import "standards" properties
    import_handle_generic(dataset, rect, parent, RectangleHandle::getPropertyList(), true);

    closeList6(dataset);
    return rect;
}

static int import_handle_compound(int dataset, int parent)
{
    int compound = createGraphicObject(__GO_COMPOUND__);

    //import "standards" properties
    import_handle_generic(dataset, compound, parent, CompoundHandle::getPropertyList(), true);

    closeList6(dataset);
    return compound;
}

static int import_handle_datatip(int dataset, int parent)
{
    int datatip = createGraphicObject(__GO_DATATIP__);
    //set parent manually, these no real releationship between datatip and parent
    setGraphicObjectProperty(datatip, __GO_PARENT__, &parent, jni_int, 1);

    //data
    int index = 0;
    getHandleInt(dataset, "data_index", &index);
    double indexes[2];
    indexes[0] = index;
    indexes[1] = 0;

    setGraphicObjectProperty(datatip, __GO_DATATIP_INDEXES__, indexes, jni_double_vector, 2);

    //import "standards" properties
    import_handle_generic(dataset, datatip, -1, DatatipHandle::getPropertyList(), true);

    closeList6(dataset);
    return datatip;
}

static int import_handle_datatips(int dataset, int uid)
{
    int datatip = getDataSetIdFromName(dataset, "datatips");
    int count = 0;
    getListDims6(datatip, &count);

    std::vector<int> datatips(count);
    for (int i = 0; i < count; ++i)
    {
        int d = getDataSetIdFromName(datatip, std::to_string(i).data());
        datatips[i] = import_handle_datatip(d, uid);
    }

    setGraphicObjectProperty(uid, __GO_DATATIPS__, datatips.data(), jni_int_vector, count);

    closeList6(datatip);
    return uid;
}

static int import_polyline_shift(int dataset, int uid, const std::string& name, int go_set, int go_data)
{
    int row = 0;
    int col = 0;
    int set = 0;
    double* data = nullptr;

    getHandleDoubleVector(dataset, name.data(), &row, &col, &data);
    if (row * col != 0)
    {
        set = 1;
        setGraphicObjectProperty(uid, go_data, data, jni_double_vector, row * col);
    }
    else
    {
        set = 0;
    }

    delete[] data;
    setGraphicObjectProperty(uid, go_set, &set, jni_int, 1);
    return uid;
}

static int import_handle_polyline(int dataset, int parent)
{
    int polyline = createGraphicObject(__GO_POLYLINE__);
    createDataObject(polyline, __GO_POLYLINE__);

    //import "standards" properties
    import_handle_generic(dataset, polyline, parent, PolylineHandle::getPropertyList(), true);


    //x_shift
    import_polyline_shift(dataset, polyline, "x_shift", __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_X_COORDINATES_SHIFT__);
    //y_shift
    import_polyline_shift(dataset, polyline, "y_shift", __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__);
    //z_shift
    import_polyline_shift(dataset, polyline, "z_shift", __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__);


    //interp_color_vector
    {
        int row = 0;
        int col = 0;
        int set = 0;
        int* data = nullptr;

        getHandleIntVector(dataset, "interp_color_vector", &row, &col, &data);
        if (row * col != 0)
        {
            set = 1;
            setGraphicObjectProperty(polyline, __GO_INTERP_COLOR_VECTOR__, data, jni_double_vector, row * col);
        }
        else
        {
            set = 0;
        }

        delete[] data;
        setGraphicObjectProperty(polyline, __GO_INTERP_COLOR_VECTOR_SET__, &set, jni_bool, 1);
    }

    //data
    {
        int numElementsArray[2];
        int size = 0;
        double* dataX = nullptr;
        double* dataY = nullptr;
        double* dataZ = nullptr;
        getHandleDoubleVector(dataset, "data_x", &numElementsArray[0], &numElementsArray[1], &dataX);
        size = numElementsArray[0] * numElementsArray[1];
        getHandleDoubleVector(dataset, "data_y", &numElementsArray[0], &numElementsArray[1], &dataY);

        if (numElementsArray[0] * numElementsArray[1] != size)
        {
            std::cout << "size trouble !!!" << std::endl;
        }

        setGraphicObjectPropertyAndNoWarn(polyline, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 2);
        setGraphicObjectPropertyAndNoWarn(polyline, __GO_DATA_MODEL_X__, dataX, jni_double_vector, size);
        setGraphicObjectPropertyAndNoWarn(polyline, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, size);

        //data z
        int zSet = 0;
        numElementsArray[0] = 0;
        numElementsArray[1] = 0;
        getHandleDoubleVector(dataset, "data_z", &numElementsArray[0], &numElementsArray[1], &dataZ);
        if (numElementsArray[0] * numElementsArray[1] != 0)
        {
            setGraphicObjectPropertyAndNoWarn(polyline, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, size);
            zSet = 1;
        }

        setGraphicObjectProperty(polyline, __GO_DATA_MODEL_Z_COORDINATES_SET__, &zSet, jni_int, 1);

        delete[] dataX;
        delete[] dataY;
        delete[] dataZ;
    }

    //datatips
    import_handle_datatips(dataset, polyline);

    closeList6(dataset);
    return polyline;
}

static int import_handle_surface(int dataset, int uid, int parent)
{
    //import "standards" properties
    import_handle_generic(dataset, uid, parent, SurfaceHandle::getPropertyList(), true);
    return uid;
}

static int import_handle_plot3d(int dataset, int parent)
{
    int plot = createGraphicObject(__GO_PLOT3D__);
    createDataObject(plot, __GO_PLOT3D__);
    import_handle_surface(dataset, plot, parent);

    //data
    int xR = 0, xC = 0;
    double* dataX = nullptr;
    int yR = 0, yC = 0;
    double* dataY = nullptr;
    int zR = 0, zC = 0;
    double* dataZ = nullptr;

    getHandleDoubleVector(dataset, "data_x", &xR, &xC, &dataX);
    getHandleDoubleVector(dataset, "data_y", &yR, &yC, &dataY);
    getHandleDoubleVector(dataset, "data_z", &zR, &zC, &dataZ);

    int gridSize[4];
    int result;

    gridSize[0] = xR;
    gridSize[1] = xC;
    gridSize[2] = yR;
    gridSize[3] = yC;

    result = setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_X__, dataX, jni_double_vector, xR * xC);
    setGraphicObjectPropertyAndNoWarn(plot, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, yR * yC);
    setGraphicObjectProperty(plot, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, zR * zC);

    delete[] dataX;
    delete[] dataY;
    delete[] dataZ;


    closeList6(dataset);
    return plot;
}

static int import_handle_fac3d(int dataset, int parent)
{
    int fac = createGraphicObject(__GO_FAC3D__);
    createDataObject(fac, __GO_FAC3D__);

    import_handle_surface(dataset, fac, parent);

    //data
    int xR = 0, xC = 0;
    double* dataX = nullptr;
    int yR = 0, yC = 0;
    double* dataY = nullptr;
    int zR = 0, zC = 0;
    double* dataZ = nullptr;

    getHandleDoubleVector(dataset, "data_x", &xR, &xC, &dataX);
    getHandleDoubleVector(dataset, "data_y", &yR, &yC, &dataY);
    getHandleDoubleVector(dataset, "data_z", &zR, &zC, &dataZ);

    int cR = 0;
    int cC = 0;
    double* colors = nullptr;
    getHandleDoubleVector(dataset, "colors", &cR, &cC, &colors);
    int cSize = cR * cC;

    int numElementsArray[3];

    numElementsArray[0] = xC;
    numElementsArray[1] = xR;
    numElementsArray[2] = cSize;

    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 3);

    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_X__, dataX, jni_double_vector, xR * xC);
    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_Y__, dataY, jni_double_vector, yR * yC);
    setGraphicObjectPropertyAndNoWarn(fac, __GO_DATA_MODEL_Z__, dataZ, jni_double_vector, zR * zC);
    setGraphicObjectProperty(fac, __GO_DATA_MODEL_COLORS__, colors, jni_double_vector, cSize);

    delete[] dataX;
    delete[] dataY;
    delete[] dataZ;
    delete[] colors;


    //cdata_mapping
    int cdata = 0;
    getHandleInt(dataset, "cdata_mapping", &cdata);
    setGraphicObjectProperty(fac, __GO_DATA_MAPPING__, &cdata, jni_int, 1);


    closeList6(dataset);
    return fac;
}


static int import_handle_champ(int dataset, int parent)
{
    //need to get properties and call a "creator" :x

    int champ = createGraphicObject(__GO_CHAMP__);

    //data
    int row = 0;
    int col = 0;
    int dims[2];
    int num = 0;
    double* baseX = nullptr;
    getHandleDoubleVector(dataset, "base_x", &row, &col, &baseX);
    dims[0] = col;

    double* baseY = nullptr;
    getHandleDoubleVector(dataset, "base_y", &row, &col, &baseY);
    dims[1] = col;
    num = dims[0] * dims[1];

    double* directionX = nullptr;
    getHandleDoubleVector(dataset, "direction_x", &row, &col, &directionX);

    double* directionY = nullptr;
    getHandleDoubleVector(dataset, "direction_y", &row, &col, &directionY);

    setGraphicObjectProperty(champ, __GO_NUMBER_ARROWS__, &num, jni_int, 1);
    setGraphicObjectProperty(champ, __GO_CHAMP_DIMENSIONS__, dims, jni_int_vector, 2);
    setGraphicObjectProperty(champ, __GO_BASE_X__, baseX, jni_double_vector, dims[0]);
    setGraphicObjectProperty(champ, __GO_BASE_Y__, baseY, jni_double_vector, dims[1]);
    setGraphicObjectProperty(champ, __GO_DIRECTION_X__, directionX, jni_double_vector, dims[0] * dims[1]);
    setGraphicObjectProperty(champ, __GO_DIRECTION_Y__, directionY, jni_double_vector, dims[0] * dims[1]);

    delete[] baseX;
    delete[] baseY;
    delete[] directionX;
    delete[] directionY;

    //import "standards" properties
    import_handle_generic(dataset, champ, parent, ChampHandle::getPropertyList(), true);

    closeList6(dataset);
    return champ;
}
static int import_handle_label(int dataset, int uid)
{
    //import "standards" properties
    //do not create releationship between parent
    import_handle_generic(dataset, uid, -1, LabelHandle::getPropertyList(), true);

    //text
    std::vector<int> dims(2);
    char** data = nullptr;
    int node = getHandleStringVector(dataset, "text", &dims[0], &dims[1], &data);

    setGraphicObjectProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, dims.data(), jni_int_vector, 2);
    setGraphicObjectProperty(uid, __GO_TEXT_STRINGS__, data, jni_string_vector, dims[0] * dims[1]);
    freeStringMatrix(node, data);
    delete[] data;

    closeList6(dataset);
    return uid;
}

static int import_handle_axes(int dataset, int parent)
{
    //how to manage call by %h_copy ?

    int axes = createSubWin(parent);

    //hide current axes
    int visible = 0;
    setGraphicObjectProperty(axes, __GO_VISIBLE__, &visible, jni_bool, 1);

    //import "standards" properties
    import_handle_generic(dataset, axes, parent, AxesHandle::getPropertyList(), true);

    //title
    int title = 0;
    int *ptitle = &title;
    int nodeTitle = getDataSetIdFromName(dataset, "title");
    getGraphicObjectProperty(axes, __GO_TITLE__, jni_int, (void **)&ptitle);
    import_handle_label(nodeTitle, title);

    //x_label
    int x_label = 0;
    int *px_label = &x_label;
    int nodeX = getDataSetIdFromName(dataset, "x_label");
    getGraphicObjectProperty(axes, __GO_X_AXIS_LABEL__, jni_int, (void **)&px_label);
    import_handle_label(nodeX, x_label);

    //y_label
    int y_label = 0;
    int *py_label = &y_label;
    int nodeY = getDataSetIdFromName(dataset, "y_label");
    getGraphicObjectProperty(axes, __GO_Y_AXIS_LABEL__, jni_int, (void **)&py_label);
    import_handle_label(nodeY, y_label);

    //z_label
    int z_label = 0;
    int *pz_label = &z_label;
    int nodeZ = getDataSetIdFromName(dataset, "z_label");
    getGraphicObjectProperty(axes, __GO_Z_AXIS_LABEL__, jni_int, (void **)&pz_label);
    import_handle_label(nodeZ, z_label);

    //set real visible state
    getHandleInt(dataset, "visible", &visible);
    setGraphicObjectProperty(axes, __GO_VISIBLE__, &visible, jni_bool, 1);

    closeList6(dataset);
    return axes;
}

static int import_handle_layout_options(int dataset, int frame)
{
    int layout_type = 0;
    getHandleInt(dataset, "layout", &layout_type);
    int row = 0;
    int col = 0;
    int* data = nullptr;

    switch (layout_type)
    {
        case 2: //grid
        {
            int node = getDataSetIdFromName(dataset, "layout_options");
            getHandleIntVector(node, "grid", &row, &col, &data);
            if (data && row * col == 2)
            {
                setGraphicObjectProperty(frame, __GO_GRID_OPT_GRID__, data, jni_int_vector, 2);
            }

            delete[] data;
            data = nullptr;

            getHandleIntVector(node, "padding", &row, &col, &data);
            if (data && row * col == 2)
            {
                setGraphicObjectProperty(frame, __GO_GRID_OPT_PADDING__, data, jni_int_vector, 2);
            }

            delete[] data;
            data = nullptr;

            closeList6(node);
            break;
        }
        case 3: //border
        {
            int node = getDataSetIdFromName(dataset, "layout_options");
            getHandleIntVector(node, "padding", &row, &col, &data);
            if (data && row * col == 2)
            {
                setGraphicObjectProperty(frame, __GO_BORDER_OPT_PADDING__, data, jni_int_vector, 2);
            }

            delete[] data;
            data = nullptr;

            closeList6(node);
            break;
        }
    }

    return frame;
}

static int import_handle_figure(int dataset, int parent)
{
    //some properties must be set @ creation time
    int menubar = 0;
    getHandleInt(dataset, "menubar", &menubar);
    int toolbar = 0;
    getHandleInt(dataset, "toolbar", &toolbar);
    int dockable = 0;
    getHandleBool(dataset, "dockable", &dockable);
    int default_axes = 0;
    getHandleBool(dataset, "default_axes", &default_axes);

    //force visible true FOR DEBUG ONLY
    int visible = 0;

    //create a new hidden figure
    int fig = createFigure(dockable, menubar, toolbar, default_axes, visible);
    int id = getValidDefaultFigureId();
    setGraphicObjectProperty(fig, __GO_ID__, &id, jni_int, 1);

    int menu = 0;
    getHandleBool(dataset, "menubar_visible", &menu);
    int notmenu = !menu;
    int info = 0;
    getHandleBool(dataset, "infobar_visible", &info);
    int notinfo = !info;
    int tool = 0;
    getHandleBool(dataset, "toolbar_visible", &tool);
    int nottool = !tool;

    //force inverse flag
    setGraphicObjectProperty(fig, __GO_MENUBAR_VISIBLE__, &notmenu, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_INFOBAR_VISIBLE__, &notinfo, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_TOOLBAR_VISIBLE__, &nottool, jni_bool, 1);

    //set real value
    setGraphicObjectProperty(fig, __GO_MENUBAR_VISIBLE__, &menu, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_INFOBAR_VISIBLE__, &info, jni_bool, 1);
    setGraphicObjectProperty(fig, __GO_TOOLBAR_VISIBLE__, &tool, jni_bool, 1);

    //import "standards" properties
    import_handle_generic(dataset, fig, -1, FigureHandle::getPropertyList(), true);

    import_handle_layout_options(dataset, fig);
    closeList6(dataset);
    return fig;
}

int import_handle(int dataset, int parent)
{
    //get type
    int type = 0;
    getHandleInt(dataset, "type", &type);
    switch (type)
    {
        case __GO_FIGURE__:
        {
            return import_handle_figure(dataset, parent);
        }
        case __GO_AXES__:
        {
            return import_handle_axes(dataset, parent);
        }
        case __GO_CHAMP__:
        {
            return import_handle_champ(dataset, parent);
        }
        case __GO_FAC3D__:
        {
            return import_handle_fac3d(dataset, parent);
        }
        case __GO_PLOT3D__:
        {
            return import_handle_plot3d(dataset, parent);
        }
        case __GO_COMPOUND__:
        {
            return import_handle_compound(dataset, parent);
        }
        case __GO_POLYLINE__:
        {
            return import_handle_polyline(dataset, parent);
        }
        case __GO_RECTANGLE__:
        {
            return import_handle_rectangle(dataset, parent);
        }
        case __GO_ARC__:
        {
            return import_handle_arc(dataset, parent);
        }
        case __GO_SEGS__:
        {
            return import_handle_segs(dataset, parent);
        }
        case __GO_GRAYPLOT__:
        {
            return import_handle_grayplot(dataset, parent);
        }
        case __GO_MATPLOT__:
        {
            return import_handle_matplot(dataset, parent);
        }
        case __GO_FEC__:
        {
            return import_handle_fec(dataset, parent);
        }
        case __GO_LEGEND__:
        {
            return import_handle_legend(dataset, parent);
        }
        case __GO_TEXT__:
        {
            return import_handle_text(dataset, parent);
        }
        case __GO_AXIS__:
        {
            return import_handle_axis(dataset, parent);
        }
        case __GO_LIGHT__:
        {
            return import_handle_light(dataset, parent);
        }
        case __GO_UIMENU__:
        {
            return import_handle_uimenu(dataset, parent);
        }
        case __GO_UICONTEXTMENU__:
        {
            return import_handle_uicontextmenu(dataset, parent);
        }
        case __GO_UICONTROL__:
        {
            return import_handle_uicontrol(dataset, parent);
        }
    }
    return -1;
}

void update_link_path(int legend, Links::PathList& paths)
{
    //find legend parent axes ( origin of path items )
    int type = 0;
    int* ptype = &type;
    int axes = legend; //start point
    int* paxes = &axes;
    getGraphicObjectProperty(legend, __GO_PARENT_AXES__, jni_int, (void**)&paxes);
    std::vector<int> links;
    //loop on child following path index
    for (auto & path : paths)
    {
        int current = axes;
        for (int j = 0; j < path.size(); ++j)
        {
            int index = path[path.size() - 1 - j];
            int count = 0;
            int* pcount = &count;
            getGraphicObjectProperty(current, __GO_CHILDREN_COUNT__, jni_int, (void**)&pcount);
            if (count == 0 || index >= count)
            {
                getGraphicObjectProperty(current, __GO_TYPE__, jni_int, (void**)&ptype);
                break;
            }

            int* children = nullptr;
            getGraphicObjectProperty(current, __GO_CHILDREN__, jni_int_vector, (void**)&children);

            current = children[index];

            releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, count);
        }

        links.push_back(current);
    }

    setGraphicObjectProperty(legend, __GO_LINKS__, links.data(), jni_int_vector, static_cast<int>(links.size()));
}

static bool getHandleBoolProperty(int uid, int prop, int* data)
{
    int val = 0;
    int* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_bool, (void **)&pVal);
    if (pVal == nullptr)
    {
        return false;
    }

    *data = val;
    return true;
}

static bool getHandleIntProperty(int uid, int prop, int* data)
{
    int val = 0;
    int* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_int, (void **)&pVal);
    if (pVal == nullptr)
    {
        return false;
    }

    *data = val;
    return true;
}

static bool getHandleDoubleProperty(int uid, int prop, double* data)
{
    double val = 0;
    double* pVal = &val;
    getGraphicObjectProperty(uid, prop, jni_double, (void **)&pVal);
    if (pVal == nullptr)
    {
        return false;
    }

    *data = val;
    return true;
}

static void getHandleStringProperty(int uid, int prop, char** str)
{
    getGraphicObjectProperty(uid, prop, jni_string, (void **)str);
}


static void getHandleBoolVectorProperty(int uid, int prop, int** vals)
{
    getGraphicObjectProperty(uid, prop, jni_bool_vector, (void **)vals);
}

static void getHandleIntVectorProperty(int uid, int prop, int** vals)
{
    getGraphicObjectProperty(uid, prop, jni_int_vector, (void **)vals);
}

static void getHandleDoubleVectorProperty(int uid, int prop, double** vals)
{
    getGraphicObjectProperty(uid, prop, jni_double_vector, (void **)vals);
}

static void getHandleStringVectorProperty(int uid, int prop, char*** vals)
{
    getGraphicObjectProperty(uid, prop, jni_string_vector, (void **)vals);
}

static bool export_handle_generic(int parent, int uid, const HandleProp& props);
static bool export_handle_layout_options(int parent, int uid);
static bool export_handle_userdata(int parent, int uid);
static bool export_handle_tag(int parent, int uid);
static bool export_handle_figure(int parent, int uid);
static bool export_handle_axes(int parent, int uid);
static bool export_handle_label(int parent, int uid);
static bool export_handle_champ(int parent, int uid);
static bool export_handle_children(int parent, int uid);

static bool export_handle_generic(int parent, int uid, const HandleProp& props)
{
    for (auto & prop : props)
    {
        const char* name = prop.first.data();
        std::vector<int> info(prop.second);

        //scalar variable
        if (info.size() == 3)
        {
            int go = info[1];
            int type = info[2];

            switch (type)
            {
                case jni_bool:
                {
                    std::vector<int> dims = {1, 1};
                    int val = 0;
                    getHandleBoolProperty(uid, go, &val);
                    writeBooleanMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), &val);
                    break;
                }
                case jni_double:
                {
                    std::vector<int> dims = {1, 1};
                    double val = 0;
                    getHandleDoubleProperty(uid, go, &val);
                    writeDoubleMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), &val);
                    break;
                }
                case jni_int:
                {
                    std::vector<int> dims = {1, 1};
                    int val = 0;
                    getHandleIntProperty(uid, go, &val);
                    writeIntegerMatrix6(parent, name, H5T_NATIVE_INT32, "32", static_cast<int>(dims.size()), dims.data(), &val);
                    break;
                }
                case jni_string:
                {
                    std::vector<int> dims = {1, 1};
                    char* val;
                    getHandleStringProperty(uid, go, &val);
                    writeStringMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), &val);
                    releaseGraphicObjectProperty(go, val, jni_string, 1);
                    break;
                }
            }
        }
        else //vector variable
        {
            int go = info[1];
            int type = info[2];
            int row = info[3];
            int col = info.size() > 3 ? info[4] : -1;

            if (row < 0)
            {
                row = -row;
            }
            else
            {
                getHandleIntProperty(uid, row, &row);
            }

            if (col < 0)
            {
                col = -col;
            }
            else
            {
                getHandleIntProperty(uid, col, &col);
            }

            switch (type)
            {
                case jni_bool_vector:
                {
                    std::vector<int> dims = {row, col};
                    int* vals;
                    getHandleBoolVectorProperty(uid, go, &vals);
                    writeBooleanMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), vals);
                    releaseGraphicObjectProperty(go, vals, jni_bool_vector, row * col);
                    break;
                }
                case jni_double_vector:
                {
                    std::vector<int> dims = {row, col};
                    double* vals;
                    getHandleDoubleVectorProperty(uid, go, &vals);
                    writeDoubleMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), vals);
                    releaseGraphicObjectProperty(go, vals, jni_double_vector, row * col);
                    break;
                }
                case jni_int_vector:
                {
                    std::vector<int> dims = {row, col};
                    int* vals;
                    getHandleIntVectorProperty(uid, go, &vals);
                    writeIntegerMatrix6(parent, name, H5T_NATIVE_INT32, "32", static_cast<int>(dims.size()), dims.data(), vals);
                    releaseGraphicObjectProperty(go, vals, jni_int_vector, row * col);
                    break;
                }
                case jni_string_vector:
                {
                    std::vector<int> dims = {row, col};
                    char** vals;
                    getHandleStringVectorProperty(uid, go, &vals);
                    writeStringMatrix6(parent, name, static_cast<int>(dims.size()), dims.data(), vals);
                    releaseGraphicObjectProperty(go, vals, jni_string_vector, row * col);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }

    //user_data
    export_handle_userdata(parent, uid);
    //tag
    export_handle_tag(parent, uid);
    //children
    export_handle_children(parent, uid);
    return true;
}

static bool export_handle_layout_options(int parent, int uid)
{
    int layout_type = 0;
    getHandleIntProperty(uid, __GO_LAYOUT__, &layout_type);
    if (layout_type == 0 || layout_type == 1) //LAYOUT_NONE or LAYOUT_GRIDBAG
    {
        //nothing to save
        return true;
    }

    int layout = openList6(parent, "layout_options", g_SCILAB_CLASS_HANDLE);

    switch (layout_type)
    {
        case 2: //grid
        {
            std::vector<int> dims = {1, 2};
            int* grid = nullptr;
            getHandleIntVectorProperty(uid, __GO_GRID_OPT_GRID__, &grid);
            writeIntegerMatrix6(layout, "grid", H5T_NATIVE_INT32, "32", 2, dims.data(), grid);
            releaseGraphicObjectProperty(__GO_GRID_OPT_GRID__, grid, jni_int_vector, 2);

            int* pad = nullptr;
            getHandleIntVectorProperty(uid, __GO_GRID_OPT_PADDING__, &pad);
            writeIntegerMatrix6(layout, "padding", H5T_NATIVE_INT32, "32", 2, dims.data(), pad);
            releaseGraphicObjectProperty(__GO_GRID_OPT_PADDING__, pad, jni_int_vector, 2);
            break;
        }
        case 3: //border
        {
            std::vector<int> dims = {1, 2};
            int* pad = nullptr;
            getHandleIntVectorProperty(uid, __GO_BORDER_OPT_PADDING__, &pad);
            writeIntegerMatrix6(layout, "padding", H5T_NATIVE_INT32, "32", 2, dims.data(), pad);
            releaseGraphicObjectProperty(__GO_BORDER_OPT_PADDING__, pad, jni_int_vector, 2);
            break;
        }
    }

    closeList6(layout);

    return true;
}
static bool export_handle_tag(int parent, int uid)
{
    char* tag = nullptr;
    getHandleStringProperty(uid, __GO_TAG__, &tag);
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;
    writeStringMatrix6(parent, "tag", 2, dims, &tag);
    releaseGraphicObjectProperty(__GO_TAG__, tag, jni_string, 1);
    return true;
}

static bool export_handle_userdata(int parent, int uid)
{

    int size = 0;
    getHandleIntProperty(uid, __GO_USER_DATA_SIZE__, &size);

    if (size == 0)
    {
        std::vector<int> dims = {0, 0};
        writeDoubleMatrix6(parent, "userdata", 2, dims.data(), NULL);
    }
    else
    {
        int *data = NULL;
        getHandleIntVectorProperty(uid, __GO_USER_DATA__, &data);

        types::InternalType* pUD = nullptr;
        if (size == 1)
        {
            //32 bits
            int* p = (int*)data;
            pUD = ((types::InternalType*) * p);
        }
        else
        {
            //64 bits
            long long* p = (long long*)data;
            pUD = ((types::InternalType*) * p);
        }

        export_data(parent, "userdata", pUD);
        //do not release, data is a reference on data in model
        //releaseGraphicObjectProperty(__GO_USER_DATA__, data, jni_int_vector, size);
    }

    return true;
}

static bool export_handle_datatips(int parent, int uid)
{
    int count = 0;
    getHandleIntProperty(uid, __GO_DATATIPS_COUNT__, &count);
    int node = openList6(parent, "datatips", g_SCILAB_CLASS_HANDLE);
    int* datatips = nullptr;

    if (count != 0)
    {
        getHandleIntVectorProperty(uid, __GO_DATATIPS__, &datatips);
    }

    for (int i = 0; i < count; ++i)
    {
        if (export_handle(node, std::to_string(i), datatips[i]) == false)
        {
            releaseGraphicObjectProperty(__GO_DATATIPS__, datatips, jni_int_vector, count);
            closeList6(node);
            return false;
        }
    }

    releaseGraphicObjectProperty(__GO_DATATIPS__, datatips, jni_int_vector, count);
    closeList6(node);
    return true;
}

static bool export_handle_border(int dataset, int uid);

static bool export_handle_border_none(int dataset, int uid)
{
    //nothing to do
    closeList6(dataset);
    return true;
}

static bool export_handle_border_line(int dataset, int uid)
{
    bool ret = false;
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;

    //color
    char* color = nullptr;
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_COLOR__, &color);
    writeStringMatrix6(dataset, "color", 2, dims, &color);
    releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_COLOR__, color, jni_string, 1);

    //thickness
    int thickness = 0;
    ret = getHandleIntProperty(uid, __GO_LINE_THICKNESS__, &thickness);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "thickness", H5T_NATIVE_INT32, "32", 2, dims, &thickness);
    }

    //rounded
    int rounded = 0;
    ret = getHandleBoolProperty(uid, __GO_UI_FRAME_BORDER_ROUNDED__, &rounded);
    if (ret)
    {
        writeBooleanMatrix6(dataset, "rounded", 2, dims, &rounded);
    }

    closeList6(dataset);
    return true;
}

static bool export_handle_border_bevel(int dataset, int uid)
{
    bool ret = false;
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;
    char* data = nullptr;

    //type
    int type = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_TYPE__, &type);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "type", H5T_NATIVE_INT32, "32", 2, dims, &type);
    }

    //highlight out
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "highlight_out", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, data, jni_string, 1);
    }

    //highlight in
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "highlight_in", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, data, jni_string, 1);
    }

    //shadow out
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_SHADOW_OUT__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "shadow_out", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_SHADOW_OUT__, data, jni_string, 1);
    }

    //shadow in
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_SHADOW_IN__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "shadow_in", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_SHADOW_IN__, data, jni_string, 1);
    }
    closeList6(dataset);
    return true;
}

static bool export_handle_border_soft_bevel(int dataset, int uid)
{
    return export_handle_border_bevel(dataset, uid);
}

static bool export_handle_border_etched(int dataset, int uid)
{
    bool ret = false;
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;
    char* data = nullptr;

    //type
    int type = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_TYPE__, &type);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "type", H5T_NATIVE_INT32, "32", 2, dims, &type);
    }

    //highlight out
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "highlight_out", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, data, jni_string, 1);
    }

    //shadow out
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_SHADOW_OUT__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "shadow_out", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_SHADOW_OUT__, data, jni_string, 1);
    }

    closeList6(dataset);
    return true;
}

static bool export_handle_border_titled(int dataset, int uid)
{
    bool ret = false;
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;
    char* data = nullptr;

    //title_border
    int title = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_TITLE__, &title);
    if (ret)
    {
        int node = openList6(dataset, "title_border", g_SCILAB_CLASS_HANDLE);
        export_handle_border(node, title);
    }

    //title
    getHandleStringProperty(uid, __GO_TITLE__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "title", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_TITLE__, data, jni_string, 1);
    }

    //justification
    int justification = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_JUSTIFICATION__, &justification);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "justification", H5T_NATIVE_INT32, "32", 2, dims, &justification);
    }

    //fontname
    getHandleStringProperty(uid, __GO_UI_FONTNAME__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "fontname", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_UI_FONTNAME__, data, jni_string, 1);
        data = nullptr;
    }

    //fontangle
    getHandleStringProperty(uid, __GO_UI_FONTANGLE__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "fontangle", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_UI_FONTANGLE__, data, jni_string, 1);
        data = nullptr;
    }

    //fontsize
    int fonsize = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FONTSIZE__, &fonsize);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "fontsize", H5T_NATIVE_INT32, "32", 2, dims, &fonsize);
    }

    //fontweight
    getHandleStringProperty(uid, __GO_UI_FONTWEIGHT__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "fontweight", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_UI_FONTWEIGHT__, data, jni_string, 1);
        data = nullptr;
    }

    //position
    int position = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_POSITION__, &position);
    if (ret)
    {
        writeIntegerMatrix6(dataset, "position", H5T_NATIVE_INT32, "32", 2, dims, &position);
    }

    //color
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_COLOR__, &data);
    if (data)
    {
        writeStringMatrix6(dataset, "color", 2, dims, &data);
        releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_COLOR__, data, jni_string, 1);
        data = nullptr;
    }

    closeList6(dataset);
    return true;
}

static bool export_handle_border_empty(int dataset, int uid)
{
    int dims[2];
    dims[0] = 1;
    dims[1] = 4;
    double* pos = nullptr;

    //position
    getHandleDoubleVectorProperty(uid, __GO_POSITION__, &pos);
    if (pos)
    {
        writeDoubleMatrix6(dataset, "position", 2, dims, pos);
        releaseGraphicObjectProperty(__GO_POSITION__, pos, jni_double_vector, 4);
    }

    closeList6(dataset);
    return true;
}

static bool export_handle_border_compound(int dataset, int uid)
{
    bool ret = false;
    //out_border
    int out_border = 0;

    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_OUT_BORDER__, &out_border);
    if (ret)
    {
        int node = openList6(dataset, "out_border", g_SCILAB_CLASS_HANDLE);
        export_handle_border(node, out_border);

        //title_border
        int in_border = 0;
        getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_IN_BORDER__, &in_border);
        node = openList6(dataset, "in_border", g_SCILAB_CLASS_HANDLE);
        export_handle_border(node, in_border);
    }

    closeList6(dataset);
    return true;
}

static bool export_handle_border_matte(int dataset, int uid)
{
    int dims[2];
    dims[0] = 1;
    dims[1] = 4;
    char* data = nullptr;
    double* pos = nullptr;

    //position
    dims[0] = 1;
    dims[1] = 4;
    getHandleDoubleVectorProperty(uid, __GO_POSITION__, &pos);
    writeDoubleMatrix6(dataset, "position", 2, dims, pos);
    releaseGraphicObjectProperty(__GO_POSITION__, pos, jni_double_vector, 4);

    //color
    dims[0] = 1;
    dims[1] = 1;
    getHandleStringProperty(uid, __GO_UI_FRAME_BORDER_COLOR__, &data);
    writeStringMatrix6(dataset, "color", 2, dims, &data);
    releaseGraphicObjectProperty(__GO_UI_FRAME_BORDER_COLOR__, data, jni_string, 1);

    closeList6(dataset);
    return true;
}

static bool export_handle_border(int dataset, int uid)
{
    int style = 0;
    getHandleIntProperty(uid, __GO_UI_FRAME_BORDER_STYLE__, &style);
    int dims[2];
    dims[0] = 1;
    dims[1] = 1;

    writeIntegerMatrix6(dataset, "style", H5T_NATIVE_INT32, "32", 2, dims, &style);

    switch (style)
    {
        default:
        case NONE:
            return export_handle_border_none(dataset, uid);
        case LINE:
            return export_handle_border_line(dataset, uid);
        case BEVEL:
            return export_handle_border_bevel(dataset, uid);
        case SOFTBEVEL:
            return export_handle_border_soft_bevel(dataset, uid);
        case ETCHED:
            return export_handle_border_etched(dataset, uid);
        case TITLED:
            return export_handle_border_titled(dataset, uid);
        case EMPTY:
            return export_handle_border_empty(dataset, uid);
        case COMPOUND:
            return export_handle_border_compound(dataset, uid);
        case MATTE:
            return export_handle_border_matte(dataset, uid);
    }
}

static bool export_handle_uicontrol(int parent, int uid)
{
    bool ret = false;
    if (export_handle_generic(parent, uid, UicontrolHandle::getPropertyList()) == false)
    {
        return false;
    }

    //string
    int size = 0;
    getHandleIntProperty(uid, __GO_UI_STRING_SIZE__, &size);
    int col = 0;
    getHandleIntProperty(uid, __GO_UI_STRING_COLNB__, &col);

    int dims[2];

    if (col == 0)
    {
        dims[0] = 1;
        dims[1] = 1;
        char* empty = "";
        writeStringMatrix6(parent, "string", 2, dims, &empty);

    }
    else
    {
        int row = size / col;
        dims[0] = row;
        dims[1] = col;
        char** string = nullptr;
        getHandleStringVectorProperty(uid, __GO_UI_STRING__, &string);
        writeStringMatrix6(parent, "string", 2, dims, string);
        releaseGraphicObjectProperty(__GO_UI_STRING__, string, jni_string_vector, size);
    }

    //border
    int border = 0;
    ret = getHandleIntProperty(uid, __GO_UI_FRAME_BORDER__, &border);
    if (ret)
    {
        int ub = openList6(parent, "border", g_SCILAB_CLASS_HANDLE);
        export_handle_border(ub, border);
    }

    closeList6(parent);
    return true;
}


static bool export_handle_uicontextmenu(int parent, int uid)
{
    if (export_handle_generic(parent, uid, UicontextmenuHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_uimenu(int parent, int uid)
{
    if (export_handle_generic(parent, uid, UimenuHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_light(int parent, int uid)
{
    if (export_handle_generic(parent, uid, LightHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_axis(int parent, int uid)
{
    if (export_handle_generic(parent, uid, AxisHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_text(int parent, int uid)
{
    if (export_handle_generic(parent, uid, TextHandle::getPropertyList()) == false)
    {
        return false;
    }

    //text
    int* dims = nullptr;
    getHandleIntVectorProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, &dims);
    char** text;
    getHandleStringVectorProperty(uid, __GO_TEXT_STRINGS__, &text);
    writeStringMatrix6(parent, "text", 2, dims, text);
    closeList6(parent);
    return true;
}

//find parent axes of a entity and return path ( via children index )
bool get_entity_path(int entity, std::vector<int>& path)
{
    path.clear();

    while (true)
    {
        int parent = 0;
        getHandleIntProperty(entity, __GO_PARENT__, &parent);
        int count = 0;
        getHandleIntProperty(parent, __GO_CHILDREN_COUNT__, &count);
        //get children of parent to find "my" index
        int* children = nullptr;
        getHandleIntVectorProperty(parent, __GO_CHILDREN__, &children);
        bool find = false;
        for (int i = 0; i < count; ++i)
        {
            if (children[i] == entity)
            {
                path.push_back(i);
                find = true;
                break;
            }
        }

        releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, count);
        if (find == false)
        {
            return false;
        }

        int type = 0;
        getHandleIntProperty(parent, __GO_TYPE__, &type);
        if (type == __GO_AXES__)
        {
            break;
        }

        entity = parent;
    }

    return true;
}

static bool export_handle_legend(int parent, int uid)
{
    if (export_handle_generic(parent, uid, LegendHandle::getPropertyList()) == false)
    {
        return false;
    }

    //links
    int node = openList6(parent, "links", g_SCILAB_CLASS_HANDLE);
    int link = 0;
    getHandleIntProperty(uid, __GO_LINKS_COUNT__, &link);
    int* links = nullptr;
    getHandleIntVectorProperty(uid, __GO_LINKS__, &links);
    for (int i = 0; i < link; ++i)
    {
        std::vector<int> path;
        if (get_entity_path(links[i], path))
        {
            int dims[2];
            dims[0] = 1;
            dims[1] = static_cast<int>(path.size());
            writeIntegerMatrix6(node, std::to_string(i).data(), H5T_NATIVE_INT32, "32", 2, dims, path.data());
        }
    }

    releaseGraphicObjectProperty(__GO_LINKS__, links, jni_int_vector, link);
    closeList6(node);

    //text
    int* dims = nullptr;
    getHandleIntVectorProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, &dims);
    char** text;
    getHandleStringVectorProperty(uid, __GO_TEXT_STRINGS__, &text);
    writeStringMatrix6(parent, "text", 2, dims, text);
    closeList6(parent);
    return true;
}

static bool export_handle_fec(int parent, int uid)
{
    if (export_handle_generic(parent, uid, FecHandle::getPropertyList()) == false)
    {
        return false;
    }

    //triangles
    int indices = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_INDICES__, &indices);
    int vect = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_VERTICES_BY_ELEM__, &vect);
    double* triangles = nullptr;
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_FEC_ELEMENTS__, &triangles);

    int dims[2];
    dims[0] = indices;
    dims[1] = vect + 2;
    writeDoubleMatrix6(parent, "triangles", 2, dims, triangles);
    releaseGraphicObjectProperty(__GO_DATA_MODEL_FEC_ELEMENTS__, triangles, jni_double_vector, dims[0] * dims[1]);

    closeList6(parent);
    return true;
}

static bool export_handle_matplot(int parent, int uid)
{
    if (export_handle_generic(parent, uid, MatplotHandle::getPropertyList()) == false)
    {
        return false;
    }

    int row = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_X__, &row);
    int col = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_Y__, &col);
    int datatype = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_MATPLOT_DATA_TYPE__, &datatype);
    int imagetype = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__, &imagetype);
    int size = (row - 1) * (col - 1);

    //data can be char, uchar, short, ushort, ... hide in a double*
    //save double like this but need to compute exact dimensions to
    //store it

    double* data = nullptr;
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &data);

    switch (datatype)
    {
        case MATPLOT_Char:
        case MATPLOT_HM1_Char:
        case MATPLOT_HM1_UChar:
            size /= (sizeof(double) / sizeof(char));
            break;
        case MATPLOT_HM3_Char:
        case MATPLOT_HM3_UChar:
            size /= (sizeof(double) / sizeof(char));
            size *= 3;
            break;

        case MATPLOT_HM3_Double:
            size *= 3;
            break;
        case MATPLOT_HM4_Char:
        case MATPLOT_HM4_UChar:
            size /= (sizeof(double) / sizeof(char));
            size *= 4;
            break;
        case MATPLOT_HM4_Double:
            size *= 4;
            break;
        case MATPLOT_UChar:
            size /= (sizeof(double) / sizeof(char));
            if ((ImageType)imagetype == MATPLOT_RGB)
            {
                size *= 3;
            }
            else if ((GLType)imagetype == MATPLOT_GL_RGBA)
            {
                size *= 4;
            }
            break;
        case MATPLOT_Int:
        case MATPLOT_UInt:
            size /= (sizeof(double) / sizeof(int));
            break;
        case MATPLOT_Short:
        case MATPLOT_UShort:
            size /= (sizeof(double) / sizeof(short));
            break;
        case MATPLOT_Double:
        case MATPLOT_HM1_Double:
        default:
            break;
    }

    int dims[2];
    dims[0] = 1;
    dims[1] = size;

    writeDoubleMatrix6(parent, "data", 2, dims, data);
    releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, data, jni_double_vector, size);
    closeList6(parent);
    return true;
}

static bool export_handle_grayplot(int parent, int uid)
{
    if (export_handle_generic(parent, uid, GrayplotHandle::getPropertyList()) == false)
    {
        return false;
    }

    int row = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_X__, &row);
    int col = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_Y__, &col);

    double* dataX = nullptr;
    double* dataY = nullptr;
    double* dataZ = nullptr;

    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);

    int dims[2];
    dims[0] = 1;
    dims[1] = row;
    writeDoubleMatrix6(parent, "data_x", 2, dims, dataX);

    dims[0] = 1;
    dims[1] = col;
    writeDoubleMatrix6(parent, "data_y", 2, dims, dataY);

    dims[0] = row;
    dims[1] = col;
    writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ);

    closeList6(parent);
    return true;
}

static bool export_handle_segs(int parent, int uid)
{
    if (export_handle_generic(parent, uid, SegsHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_arc(int parent, int uid)
{
    if (export_handle_generic(parent, uid, ArcHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_rectangle(int parent, int uid)
{
    if (export_handle_generic(parent, uid, RectangleHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_datatip(int parent, int uid)
{
    if (export_handle_generic(parent, uid, DatatipHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

static bool export_handle_polyline_shift(int parent, int uid, const std::string& name, int go_set, int go_data)
{
    int set = 0;
    getHandleBoolProperty(uid, go_set, &set);
    if (set)
    {
        int count = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__, &count);
        double* data = nullptr;
        getHandleDoubleVectorProperty(uid, go_data, &data);

        int dims[2];
        dims[0] = 1;
        dims[1] = count;
        writeDoubleMatrix6(parent, name.data(), 2, dims, data);

        releaseGraphicObjectProperty(uid, data, jni_double_vector, count);
    }
    else
    {
        int dims[2];
        dims[0] = 0;
        dims[1] = 0;
        writeDoubleMatrix6(parent, name.data(), 2, dims, NULL);
    }

    return true;
}

static bool export_handle_polyline(int parent, int uid)
{
    if (export_handle_datatips(parent, uid) == false)
    {
        return false;
    }

    if (export_handle_generic(parent, uid, PolylineHandle::getPropertyList()) == false)
    {
        return false;
    }

    //x_shift
    export_handle_polyline_shift(parent, uid, "x_shift", __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_X_COORDINATES_SHIFT__);
    //y_shift
    export_handle_polyline_shift(parent, uid, "y_shift", __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__);
    //z_shift
    export_handle_polyline_shift(parent, uid, "z_shift", __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__);

    //interp_color_vector
    int set = 0;
    getHandleBoolProperty(uid, __GO_INTERP_COLOR_VECTOR_SET__, &set);
    if (set)
    {
        int count = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__, &count);
        int* data = nullptr;
        getHandleIntVectorProperty(uid, __GO_INTERP_COLOR_VECTOR__, &data);

        int dims[2];
        dims[0] = 1;
        dims[1] = count;
        writeIntegerMatrix6(parent, "interp_color_vector", H5T_NATIVE_INT32, "32", 2, dims, data);
        releaseGraphicObjectProperty(uid, data, jni_int_vector, count);
    }
    else
    {
        int dims[2];
        dims[0] = 0;
        dims[1] = 0;
        writeIntegerMatrix6(parent, "interp_color_vector", H5T_NATIVE_INT32, "32", 2, dims, NULL);
    }

    //data
    int count = 0;
    getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_ELEMENTS__, &count);

    int dims[2];
    dims[0] = 1;
    dims[1] = count;

    double* dataX = nullptr;
    double* dataY = nullptr;
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
    getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);

    writeDoubleMatrix6(parent, "data_x", 2, dims, dataX);
    writeDoubleMatrix6(parent, "data_y", 2, dims, dataY);

    releaseGraphicObjectProperty(__GO_DATA_MODEL_X__, dataX, jni_double_vector, count);
    releaseGraphicObjectProperty(__GO_DATA_MODEL_Y__, dataY, jni_double_vector, count);

    getHandleIntProperty(uid, __GO_DATA_MODEL_Z_COORDINATES_SET__, &set);
    if (set)
    {
        double* dataZ = nullptr;
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);
        writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, dataZ, jni_double_vector, count);
    }
    else
    {
        //[]
        dims[0] = 0;
        dims[1] = 0;
        writeDoubleMatrix6(parent, "data_z", 2, dims, NULL);
    }

    closeList6(parent);
    return true;
}

static bool export_handle_surface(int parent, int uid)
{
    return export_handle_generic(parent, uid, SurfaceHandle::getPropertyList());
}

static bool export_handle_plot3d(int parent, int uid)
{
    bool ret = export_handle_surface(parent, uid);
    if (ret)
    {
        double* colors = NULL;
        double* dataX = NULL;
        double* dataY = NULL;
        double* dataZ = NULL;

        //data
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);

        int row = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_X__, &row);
        int col = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_Y__, &col);

        int* xDims = nullptr;
        int* yDims = nullptr;
        getHandleIntVectorProperty(uid, __GO_DATA_MODEL_X_DIMENSIONS__, &xDims);
        getHandleIntVectorProperty(uid, __GO_DATA_MODEL_Y_DIMENSIONS__, &yDims);

        int dims[2];
        dims[0] = xDims[0];
        dims[1] = xDims[1];
        writeDoubleMatrix6(parent, "data_x", 2, dims, dataX);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_X__, dataX, jni_double_vector, dims[0] * dims[1]);

        dims[0] = yDims[0];
        dims[1] = yDims[1];
        writeDoubleMatrix6(parent, "data_y", 2, dims, dataY);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Y__, dataY, jni_double_vector, dims[0] * dims[1]);

        dims[0] = row;
        dims[1] = col;
        writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, dataZ, jni_double_vector, dims[0] * dims[1]);

        releaseGraphicObjectProperty(__GO_DATA_MODEL_X_DIMENSIONS__, xDims, jni_int_vector, 2);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Y_DIMENSIONS__, dataZ, jni_int_vector, 2);
    }

    closeList6(parent);
    return ret;
}

static bool export_handle_fac3d(int parent, int uid)
{
    bool ret = export_handle_surface(parent, uid);
    if (ret)
    {
        double* colors = NULL;
        double* dataX = NULL;
        double* dataY = NULL;
        double* dataZ = NULL;

        //data
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_X__, &dataX);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Y__, &dataY);
        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_Z__, &dataZ);

        int row = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_VERTICES_PER_GON__, &row);
        int col = 0;
        getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_GONS__, &col);

        int dims[2];
        dims[0] = row;
        dims[1] = col;

        writeDoubleMatrix6(parent, "data_x", 2, dims, dataX);
        writeDoubleMatrix6(parent, "data_y", 2, dims, dataY);
        writeDoubleMatrix6(parent, "data_z", 2, dims, dataZ);

        releaseGraphicObjectProperty(__GO_DATA_MODEL_X__, dataX, jni_double_vector, dims[0] * dims[1]);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Y__, dataY, jni_double_vector, dims[0] * dims[1]);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_Z__, dataZ, jni_double_vector, dims[0] * dims[1]);

        getHandleDoubleVectorProperty(uid, __GO_DATA_MODEL_COLORS__, &colors);
        if (colors)
        {
            int numColors = 0;
            getHandleIntProperty(uid, __GO_DATA_MODEL_NUM_COLORS__, &numColors);
            if (numColors == col)
            {
                dims[0] = 1;
            }
            else
            {
                dims[0] = row;
            }

            dims[1] = col;
        }
        else
        {
            //export []
            dims[0] = 0;
            dims[1] = 0;
        }

        writeDoubleMatrix6(parent, "colors", 2, dims, colors);
        releaseGraphicObjectProperty(__GO_DATA_MODEL_COLORS__, colors, jni_double_vector, dims[0] * dims[1]);

        //cdata_mapping
        int cdata = 0;
        getHandleIntProperty(uid, __GO_DATA_MAPPING__, &cdata);
        dims[0] = 1;
        dims[1] = 1;
        writeIntegerMatrix6(parent, "cdata_mapping", H5T_NATIVE_INT32, "32", 2, dims, &cdata);

    }

    closeList6(parent);
    return ret;
}


static bool export_handle_champ(int parent, int uid)
{
    if (export_handle_generic(parent, uid, ChampHandle::getPropertyList()) == false)
    {
        return false;
    }

    //data
    int* dimensions = NULL;
    double* arrowBasesX = NULL;
    double* arrowBasesY = NULL;
    double* arrowDirectionsX = NULL;
    double* arrowDirectionsY = NULL;
    int dims[2];
    getHandleIntVectorProperty(uid, __GO_CHAMP_DIMENSIONS__, &dimensions);

    //base X
    getHandleDoubleVectorProperty(uid, __GO_BASE_X__, &arrowBasesX);
    dims[0] = 1;
    dims[1] = dimensions[0];
    writeDoubleMatrix(parent, "base_x", 2, dims, arrowBasesX);
    releaseGraphicObjectProperty(__GO_BASE_X__, arrowBasesX, jni_double_vector, dims[1]);

    //base y
    getHandleDoubleVectorProperty(uid, __GO_BASE_Y__, &arrowBasesY);
    dims[0] = 1;
    dims[1] = dimensions[1];
    writeDoubleMatrix(parent, "base_y", 2, dims, arrowBasesY);
    releaseGraphicObjectProperty(__GO_BASE_Y__, arrowBasesY, jni_double_vector, dims[1]);

    //direction x
    getHandleDoubleVectorProperty(uid, __GO_DIRECTION_X__, &arrowDirectionsX);
    dims[0] = dimensions[0];
    dims[1] = dimensions[1];
    writeDoubleMatrix(parent, "direction_x", 2, dims, arrowDirectionsX);
    releaseGraphicObjectProperty(__GO_DIRECTION_X__, arrowDirectionsX, jni_double_vector, dims[0] * dims[1]);

    //direction y
    getHandleDoubleVectorProperty(uid, __GO_DIRECTION_Y__, &arrowDirectionsY);
    dims[0] = dimensions[0];
    dims[1] = dimensions[1];
    writeDoubleMatrix(parent, "direction_y", 2, dims, arrowDirectionsY);
    releaseGraphicObjectProperty(__GO_DIRECTION_Y__, arrowDirectionsY, jni_double_vector, dims[0] * dims[1]);

    releaseGraphicObjectProperty(__GO_CHAMP_DIMENSIONS__, dimensions, jni_int_vector, 2);
    closeList6(parent);
    return true;
}
static bool export_handle_label(int parent, int uid)
{
    if (export_handle_generic(parent, uid, LabelHandle::getPropertyList()) == false)
    {
        return false;
    }

    //text
    int* dimensions = nullptr;
    char** text = nullptr;

    getHandleIntVectorProperty(uid, __GO_TEXT_ARRAY_DIMENSIONS__, &dimensions);
    getHandleStringVectorProperty(uid, __GO_TEXT_STRINGS__, &text);

    std::vector<int> dims = {dimensions[0], dimensions[1]};
    releaseGraphicObjectProperty(__GO_TEXT_ARRAY_DIMENSIONS__, dimensions, jni_int_vector, 2);

    writeStringMatrix6(parent, "text", 2, dims.data(), text);
    releaseGraphicObjectProperty(__GO_TEXT_STRINGS__, text, jni_string_vector, dims[0] * dims[1]);

    closeList6(parent);
    return true;
}
static bool export_handle_axes(int parent, int uid)
{
    if (export_handle_generic(parent, uid, AxesHandle::getPropertyList()) == false)
    {
        return false;
    }

    //title
    int title = 0;
    getHandleIntProperty(uid, __GO_TITLE__, &title);
    export_handle(parent, "title", title);

    //x_label
    int x_label = 0;
    getHandleIntProperty(uid, __GO_X_AXIS_LABEL__, &x_label);
    export_handle(parent, "x_label", x_label);

    //y_label
    int y_label = 0;
    getHandleIntProperty(uid, __GO_Y_AXIS_LABEL__, &y_label);
    export_handle(parent, "y_label", y_label);

    //z_label
    int z_label = 0;
    getHandleIntProperty(uid, __GO_Z_AXIS_LABEL__, &z_label);
    export_handle(parent, "z_label", z_label);


    closeList6(parent);
    return true;
}

static bool export_handle_figure(int parent, int uid)
{
    if (export_handle_generic(parent, uid, FigureHandle::getPropertyList()) == false)
    {
        return false;
    }

    //layout_options
    export_handle_layout_options(parent, uid);

    closeList6(parent);
    return true;
}

static bool export_handle_compound(int parent, int uid)
{
    if (export_handle_generic(parent, uid, CompoundHandle::getPropertyList()) == false)
    {
        return false;
    }

    closeList6(parent);
    return true;
}

bool export_handle(int parent, const std::string& name, int uid)
{
    //get handle type
    int type = 0;
    getHandleIntProperty(uid, __GO_TYPE__, &type);

    //create handle node in __refs__
    int h = openList6(parent, name.data(), g_SCILAB_CLASS_HANDLE);

    bool ret = false;
    switch (type)
    {
        case __GO_FIGURE__:
        {
            ret = export_handle_figure(h, uid);
            break;
        }
        case __GO_AXES__:
        {
            ret = export_handle_axes(h, uid);
            break;
        }
        case __GO_LABEL__:
        {
            ret = export_handle_label(h, uid);
            break;
        }
        case __GO_CHAMP__:
        {
            ret = export_handle_champ(h, uid);
            break;
        }
        case __GO_FAC3D__:
        {
            ret = export_handle_fac3d(h, uid);
            break;
        }
        case __GO_PLOT3D__:
        {
            ret = export_handle_plot3d(h, uid);
            break;
        }
        case __GO_POLYLINE__:
        {
            ret = export_handle_polyline(h, uid);
            break;
        }
        case __GO_DATATIP__:
        {
            ret = export_handle_datatip(h, uid);
            break;
        }
        case __GO_COMPOUND__:
        {
            ret = export_handle_compound(h, uid);
            break;
        }
        case __GO_RECTANGLE__:
        {
            ret = export_handle_rectangle(h, uid);
            break;
        }
        case __GO_ARC__:
        {
            ret = export_handle_arc(h, uid);
            break;
        }
        case __GO_SEGS__:
        {
            ret = export_handle_segs(h, uid);
            break;
        }
        case __GO_GRAYPLOT__:
        {
            ret = export_handle_grayplot(h, uid);
            break;
        }
        case __GO_MATPLOT__:
        {
            ret = export_handle_matplot(h, uid);
            break;
        }
        case __GO_FEC__:
        {
            ret = export_handle_fec(h, uid);
            break;
        }
        case __GO_LEGEND__:
        {
            ret = export_handle_legend(h, uid);
            break;
        }
        case __GO_TEXT__:
        {
            ret = export_handle_text(h, uid);
            break;
        }
        case __GO_AXIS__:
        {
            ret = export_handle_axis(h, uid);
            break;
        }
        case __GO_LIGHT__:
        {
            ret = export_handle_light(h, uid);
            break;
        }
        case __GO_UIMENU__:
        {
            ret = export_handle_uimenu(h, uid);
            break;
        }
        case __GO_UICONTEXTMENU__:
        {
            ret = export_handle_uicontextmenu(h, uid);
            break;
        }
        case __GO_UICONTROL__:
        {
            ret = export_handle_uicontrol(h, uid);
            break;
        }
        default:
        {
        }

    }

    return ret;
}

static bool export_handle_children(int parent, int uid)
{
    int count = 0;
    getHandleIntProperty(uid, __GO_CHILDREN_COUNT__, &count);
    int node = openList6(parent, "children", g_SCILAB_CLASS_HANDLE);
    int* children = nullptr;

    if (count != 0)
    {
        getHandleIntVectorProperty(uid, __GO_CHILDREN__, &children);
    }

    int index = 0;
    for (int i = 0; i < count; ++i)
    {
        int child = children[i];
        int hidden = 0;
        getHandleBoolProperty(child, __GO_HIDDEN__, &hidden);
        if (hidden == 0)
        {
            if (export_handle(node, std::to_string(index), child) == false)
            {
                releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, count);
                closeList6(node);
                return false;
            }

            ++index;
        }
    }

    releaseGraphicObjectProperty(__GO_CHILDREN__, children, jni_int_vector, count);
    closeList6(node);
    return true;
}

int add_current_entity(int dataset)
{
    int type = 0;
    getHandleInt(dataset, "type", &type);

    switch (type)
    {
        case __GO_FIGURE__:
            return import_handle(dataset, -1);
            break;
        case __GO_AXES__:
        {
            //add handle to current figure
            getOrCreateDefaultSubwin();
            int iCurrentFigure = getCurrentFigure();
            return import_handle(dataset, iCurrentFigure);
            break;
        }
        default:
            //add handle as child of current axes ( take care of compound ! )
            return -1;
    }
}
