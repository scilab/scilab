function createFigure(uid) {
    var __parent__ = getScilab();
    var __temp__ = createElement("DIV");
    __temp__.id = getIdString(uid);
    __temp__.className = 'GO_FIGURE';
    __parent__.appendChild(__temp__);
    addWindow(__temp__);
    return __temp__;
}

function createPushButton(uid) {
    var __temp__ = createCommonIUControl(uid, 'BUTTON', 'GO_UI_PUSHBUTTON');
    //add bootstrap class
    var $temp = $("#" + __temp__.id);
    $temp.addClass("btn");
    
    return __temp__;
}

function createFrame(uid) {
    var __temp__ = createCommonIUControl(uid, 'DIV', 'GO_UI_FRAME');

    //add bootstrap class
    var $temp = $("#" + __temp__.id);
    $temp.addClass("panel");
    $temp.addClass("panel-default");
    $temp.css("margin", "0");
    
    var __header__ = createCommonIUControl(uid, 'DIV', 'panel-heading');
    __header__.id = getIdString(uid, "_header");
    __header__.style.display = 'none';
    
    var __footer__ = createCommonIUControl(uid, 'DIV', 'panel-footer');
    __footer__.id = getIdString(uid, "_footer");
    __footer__.style.display = 'none';
    
    var __child__ = createCommonIUControl(uid, 'DIV', 'panel-body');
    __child__.id = getIdString(uid, "_body");
    
    __temp__.appendChild(__header__);
    __temp__.appendChild(__child__);
    __temp__.appendChild(__footer__);
    return __temp__;
}

function createText(uid) {
    var __temp__ = createCommonIUControl(uid, 'LABEL', 'GO_UI_TEXT');
//    var $temp = $("#" + __temp__.id);
//    $temp.addClass("label");
//    $temp.addClass("label-default");
    return __temp__;
}

function createEdit(uid) {
    var __temp__ = createCommonIUControl(uid, 'INPUT', 'GO_UI_EDIT');
    __temp__.type = 'text';

    //add bootstrap class
    var $temp = $("#" + __temp__.id);
    $temp.addClass("form-control");
    return __temp__;
}

function createCheckbox(uid) {
    //for checkbox we need to create 4 elements.

    //a div to enclose others
    var __main__ = createCommonIUControl(uid, 'DIV', 'GO_UI_CHECKBOX');

    //add bootstrap class
    var __jqmain__ = $("#" + __main__.id);
    __jqmain__.addClass("checkbox");
    __main__.style.position = 'absolute';
    __main__.style.margin = '0';
    __main__.style.display = 'flex';
    __main__.style.alignItems = 'center';
    
    //the label of the checkbox
    var __label__ = createCommonIUControl(uid, 'LABEL', 'GO_UI_CHECKBOX_LABEL');
    __label__.id = getIdString(uid, '_label');
    __main__.appendChild(__label__);

     //the checkbox itself
   var __temp__ = createCommonIUControl(uid, 'INPUT', 'GO_UI_CHECKBOX_RADIO');
    __temp__.type = 'checkbox';
    __temp__.id = getIdString(uid, '_checkbox');
    __temp__.style.marginLeft = '-15px';
    __label__.appendChild(__temp__);
    
    //and a span to esay update text value
    var __span__ = createCommonIUControl(uid, 'SPAN', '');
    __span__.id = getIdString(uid, '_span');
    __label__.appendChild(__span__);

    return __main__;
}

function createRadio(uid) {
    //for checkbox we need to create 4 elements.

    //a div to enclose others
    var __main__ = createCommonIUControl(uid, 'DIV', 'GO_UI_RADIOBUTTON');

    //add bootstrap class
    var __jqmain__ = $("#" + __main__.id);
    __jqmain__.addClass("radio");
    __main__.style.position = 'absolute';
    __main__.style.margin = '0';
    __main__.style.display = 'flex';
    __main__.style.alignItems = 'center';
 
    //the label of the radio
    var __label__ = createCommonIUControl(uid, 'LABEL', 'GO_UI_RADIOBUTTON_LABEL');
    __label__.id = getIdString(uid, '_label');
    __main__.appendChild(__label__);

     //the radio itself
   var __temp__ = createCommonIUControl(uid, 'INPUT', 'GO_UI_RADIOBUTTON_RADIO');
    __temp__.type = 'radio';
    __temp__.id = getIdString(uid, '_radio');
    __temp__.style.marginLeft = '4px';
    __temp__.style.marginBottom = '12px';
    __label__.appendChild(__temp__);
    
    //and a span to esay update text value
    var __span__ = createCommonIUControl(uid, 'SPAN', '');
    __span__.id = getIdString(uid, '_span');
    __label__.appendChild(__span__);

    return __main__;
}

function createSlider(uid) {
    var __temp__ = createCommonIUControl(uid, 'INPUT', 'GO_UI_SLIDER');
    __temp__.type = 'range';
    return __temp__;
}

function createListbox(uid) {
    var __temp__ = createCommonIUControl(uid, 'SELECT', 'GO_UI_LISTBOX');
    return __temp__;
}

function createCombobox(uid) {
    var __temp__ = createCommonIUControl(uid, 'SELECT', 'GO_UI_POPUPMENU');
    __temp__.size = 1;
    var $temp = $("#" + __temp__.id);
    $temp.addClass("form-control");
    return __temp__;
}

function createSpinner(uid) {
    var __temp__ = createCommonIUControl(uid, 'INPUT', 'GO_UI_SPINNER');
    __temp__.type = 'number';
    //$("#" + getIdString(uid)).TouchSpin({verticalbuttons: true});
    return __temp__;
}

function createTab(uid) {
    var __temp__ = createCommonIUControl(uid, 'DIV', 'GO_UI_TAB');

    //create a list for tab headers
    var __ul__ = createElement('UL');
    __ul__.id = getIdString(uid, '_ul');
    __temp__.appendChild(__ul__);

    var $ul = $("#" + __ul__.id);
    $ul.addClass("nav");
    $ul.addClass("nav-tabs");

    var __content__ = createElement('DIV');
    __content__.id = getIdString(uid, '_tabs');
    __temp__.appendChild(__content__);

    var $content = $("#" + __content__.id);
    $content.addClass("tab-content");

    return __temp__;
}

function deleteObject(uid) {
    $child = getJElementById(uid);
    if($child) {
        if($child.hasClass('GO_FIGURE')) {
            delWindow(uid);
        } else {
            $child.remove();
        }
    }
}