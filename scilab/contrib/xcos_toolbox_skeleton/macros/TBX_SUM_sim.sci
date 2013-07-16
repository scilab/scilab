//
// This file is part of the Xcos skeleton toolbox
//
// see license.txt for more licensing information


function block=TBX_SUM_sim(block,flag)
    function verboseMessage(debugMessage)
        disp("[verboseMessage] time = "+string(scicos_time())+"] {"+block.label+"} SUM Simulation: "+debugMessage);
    endfunction

    select flag
    case -5 // Error
        verboseMessage("Calling with flag Error(-5)")

    case 0 // Derivative State Update
        verboseMessage("Calling with flag DerivativeStare(0)")

    case 1 // Output Update
        verboseMessage("Calling with flag OutputUpdate(1)")
        outptr = block.outptr;
        inptr = block.inptr;
        outptr(1) = sum(inptr(1));
        block.outptr = outptr;


    case 2 // State Update
        verboseMessage("Calling with flag StateUpdate(2)")

    case 3 // OutputEventTiming
        verboseMessage("Calling with flag OutputEventTiming(3)")

    case 4 // Initialization
        verboseMessage("Calling with flag Initialization(4)")

    case 5 // Ending
        verboseMessage("Calling with flag Ending(5)")

    case 6 // Reinitialization
        verboseMessage("Calling with flag Reinitialization(6)")

    case 9 // Zero-Crossing
        verboseMessage("Calling with flag Zero-Crossing(9)")

    else // Unknown flag
        verboseMessage("Calling with unknonw flag("+string(flag)+")")

    end
endfunction
