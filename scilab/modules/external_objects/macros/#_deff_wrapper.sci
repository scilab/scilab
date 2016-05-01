function #_deff_wrapper(name, definition, code)
    deff(definition, code);
    execstr(name + "=resume(" + name + ")");
endfunction
