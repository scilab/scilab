function getSubPadding(val) {
    if (val)
    {
        return val + 'px ';
    }

    return '0 ';
}
function getPadding(t, r, b, l) {
    var ret = '';

    ret += getSubPadding(t);
    ret += getSubPadding(r);
    ret += getSubPadding(b);
    ret += getSubPadding(l);
    return ret;
}

function isInputType(uid) {
    var __child__ = getElementById(uid);
    switch (__child__.className) {
        case 'GO_UI_EDIT' :
        case 'GO_UI_SLIDER':
        case 'GO_UI_SPINNER' :
            return true;
        default :
            return false;
    }
}

function getUID(id) {
    return parseInt(id.substring(3));
}

function createElement(type) {
    return document.createElement(type);
}

function getIdString(uid, suffix) {
    suffix = typeof suffix !== 'undefined' ? suffix : '';
    return 'uid' + uid + suffix;
}

function getElementById(uid, suffix) {
    suffix = typeof suffix !== 'undefined' ? suffix : '';
    return document.getElementById(getIdString(uid, suffix));
}

function createCommonIUControl(uid, htmlType, cssClass) {
    var __temp__ = createElement(htmlType);
    __temp__.id = getIdString(uid);
    __temp__.className = cssClass;

    //add item temporary in main scilabview div waiting __GO_PARENT__ update
    var __parent__ = document.getElementById('scilab');
    __parent__.appendChild(__temp__);
    return __temp__;
}
