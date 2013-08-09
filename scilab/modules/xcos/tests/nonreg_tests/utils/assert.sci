function assert(varargin)

    assertion = varargin(1)
    status.ok = evstr(assertion)

    if ~status.ok
        cmd = "sprintf(''" + varargin(2) + "''"
        for k = 3:length(varargin)
            if type(varargin(k)) == 10 // string ?
                cmd = cmd + ", ''" + varargin(k) + "''"
            else // not a string: convert to string
                cmd = cmd + ", " + string(varargin(k))
            end
        end
        cmd = cmd + ")"

        msg = evstr(cmd)

        status.details = "Assertion failed: " + assertion + "\n  " + msg
    else
        status.details = ""
    end

    printf(status.details)
endfunction

