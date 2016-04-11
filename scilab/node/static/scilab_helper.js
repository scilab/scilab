//helper call by scilab to insert gridbad child in good order.
function gridbagHelperTD(parent, child, pos) {
    for(var i = 0 ; i < parent.cells.length ; ++i){
        var tr = parent.cells[i];
        var idxstr = tr.id.indexOf('_');
        var idx = parseInt(tr.id.substring(idxstr+1));
        if(idx > pos) {
            parent.insertBefore(child, tr);
            return;
        }
    }

    //not found, insert @last pos
    parent.insertBefore(child, null);
}

function gridbagHelperTR(parent, child, pos) {
    for(var i = 0 ; i < parent.rows.length ; ++i){
        var tr = parent.rows[i];
        var idxstr = tr.id.indexOf('_');
        var idx = parseInt(tr.id.substring(idxstr+1));
        if(idx > pos) {
            parent.insertBefore(child, tr);
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
    var elem = getElementById(child);
    var li = createElement('LI');
    li.id = getIdString(child, '_li');

    //move tab child to tab-content container
    var tab = getElementById(uid, '_tabs');
    tab.appendChild(elem);

    //create input button
    var tablabel = createElement('a');
    tablabel.id = getIdString(child, '_tab');

    //add button in li
    li.appendChild(tablabel);

    //add li as first child in ul
    var ul = getElementById(uid, '_ul');
    ul.insertBefore(li, ul.firstChild);
    var $ul = $('#' + ul.id);
    var $tab = $('#' + tablabel.id);
    $tab.attr('data-toggle', 'tab');
    $tab.attr('href', '#' + elem.id);

    var $child = $('#' + elem.id);
    $child.addClass('tab-pane');
    $child.addClass('fade');

    //update child properties
    elem.style.position = 'absolute';
    elem.style.left = '';
    elem.style.bottom = '';
    elem.style.width = '100%';
    elem.style.height = 'inherit';
    
    updateTabHeight(getUID($ul.parent().attr('id')));
    
    //add event listener on new tab
    $('#' + tablabel.id).on('show.bs.tab', onTab);

}

function updateTabHeight(uid) {
    var $tab = getJElementById(uid);
    
    var $ul = $tab.children('ul');
    var $tabs = $tab.children('div');
    $tabs.height($tab.height() - $ul.height() - 13);
}