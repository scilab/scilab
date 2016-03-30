//helper call by scilab to insert gridbad child in good order.
function gridbagHelperTD(parent, child, pos) {
    for(var i = 0 ; i < parent.cells.length ; ++i){
        var __tr__ = parent.cells[i];
        var idxstr = __tr__.id.indexOf('_');
        var idx = parseInt(__tr__.id.substring(idxstr+1));
        if(idx > pos) {
            parent.insertBefore(child, __tr__);
            return;
        }
    }

    //not found, insert @last pos
    parent.insertBefore(child, null);
}

function gridbagHelperTR(parent, child, pos) {
    for(var i = 0 ; i < parent.rows.length ; ++i){
        var __tr__ = parent.rows[i];
        var idxstr = __tr__.id.indexOf('_');
        var idx = parseInt(__tr__.id.substring(idxstr+1));
        if(idx > pos) {
            parent.insertBefore(child, __tr__);
            return;
        }
    }

    //not found, insert @last pos
    parent.insertBefore(child, null);
}

function tabSelectHelper(tab, index) {
    index = index === 0 ? index : index - 1;
    //console.log(tab.id + ": " + index);
    var b = '#' + tab.id + ' li:eq(' + index + ') a';
    $(b).tab('show');
}

function addTabHelper(uid, child) {
    var __child__ = getElementById(child);
    var __li__ = createElement('LI');
    __li__.id = getIdString(child, '_li');

    //move tab child to tab-content container
    var __tab__ = getElementById(uid, '_tabs');
    __tab__.appendChild(__child__);

    //create input button
    var __tablabel__ = createElement('a');
    __tablabel__.id = getIdString(child, '_tab');

    //add button in li
    __li__.appendChild(__tablabel__);

    //add li as first child in ul
    var __ul__ = getElementById(uid, '_ul');
    __ul__.insertBefore(__li__, __ul__.firstChild);
    var $ul = $('#' + __ul__.id);
    var $tab = $('#' + __tablabel__.id);
    $tab.attr('data-toggle', 'tab');
    $tab.attr('href', '#' + __child__.id);

    var $child = $('#' + __child__.id);
    $child.addClass('tab-pane');
    $child.addClass('fade');

    //update child properties
    __child__.style.position = 'absolute';
    __child__.style.left = '';
    __child__.style.bottom = '';
    __child__.style.width = '100%';
    __child__.style.height = 'inherit';
    
    updateTabHeight(getUID($ul.parent().attr('id')));
    
    //add event listener on new tab
    $('#' + __tablabel__.id).on('show.bs.tab', onTab);

}

function updateTabHeight(uid) {
    var $tab = getJElementById(uid);
    
    var $ul = $tab.children('ul');
    var $tabs = $tab.children('div');
    $tabs.height($tab.height() - $ul.height() - 13);
}