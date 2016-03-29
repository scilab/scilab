function createFigure(uid) {
    var __parent__ = document.getElementById('scilab');
    var __temp__ = createElement("DIV");
    __temp__.id = getIdString(uid);
    __temp__.className = 'GO_FIGURE';
    __parent__.appendChild(__temp__);
    __parent__.innerHTML += '<br>';
    return __temp__;
}

function createPushButton(uid) {
    return createCommonIUControl(uid, 'BUTTON', 'btn');
}

function createFrame(uid) {
    return createCommonIUControl(uid, 'DIV', 'GO_UI_FRAME');
}

function createText(uid) {
    return createCommonIUControl(uid, 'LABEL', 'GO_UI_TEXT');
}

function createEdit(uid) {
    var __temp__ = createCommonIUControl(uid, 'INPUT', 'GO_UI_EDIT');
    __temp__.type = 'text';
    return __temp__;
}

function createCheckbox(uid) {
    //for checkbox we need to create 3 elements.

    //a div to enclose others
    var __main__ = createCommonIUControl(uid, 'DIV', 'GO_UI_CHECKBOX');
    //the checkbox itself
    var __temp__ = createCommonIUControl(uid, 'INPUT', 'GO_UI_CHECKBOX_CHECKBOX');
    __temp__.type = 'checkbox';
    __temp__.id = getIdString(uid, '_checkbox');
    __main__.appendChild(__temp__);

    //the label of the checkbox
    var __label__ = createCommonIUControl(uid, 'LABEL', 'GO_UI_CHECKBOX_LABEL');
    __label__.id = getIdString(uid, '_label');
    __label__.htmlFor = __temp__.id;
    __main__.appendChild(__label__);

    return __main__;
}

function createRadio(uid) {
    //for checkbox we need to create 3 elements.

    //a div to enclose others
    var __main__ = createCommonIUControl(uid, 'DIV', 'GO_UI_RADIOBUTTON');
    //the radio itself
    var __temp__ = createCommonIUControl(uid, 'INPUT', 'GO_UI_RADIOBUTTON_RADIO');
    __temp__.type = 'radio';
    __temp__.id = getIdString(uid, '_radio');
    __main__.appendChild(__temp__);

    //the label of the radio
    var __label__ = createCommonIUControl(uid, 'LABEL', 'GO_UI_RADIOBUTTON_LABEL');
    __label__.id = getIdString(uid, '_label');
    __label__.htmlFor = __temp__.id;
    __main__.appendChild(__label__);

    return __main__;
}

function createSlider(uid) {
    var __temp__ = createCommonIUControl(uid, 'INPUT', 'GO_UI_SLIDER');
    __temp__.type = 'range';
    return __temp__;
}

function createListbox(uid) {
    return createCommonIUControl(uid, 'SELECT', 'GO_UI_LISTBOX');
}

function createCombobox(uid) {
    var __temp__ = createCommonIUControl(uid, 'SELECT', 'GO_UI_POPUPMENU');
    __temp__.size = 1;
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
    return __temp__;
}

function deleteObject(uid) {
    var __child__ = getElementById(uid);
    if(__child__) {
        __child__.parentNode.removeChild(__child__);
    }
}