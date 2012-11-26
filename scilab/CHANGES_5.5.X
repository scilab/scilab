                    Changes between version 5.4.X and 5.5.0
                    =======================================


Syntax changes
===============

* Vector ^ scalar syntax declared obsolete. See bug #11524


Obsolete
=========

* The third argument of poly function will be more strict.
    Only the following string will be allowed : "roots", "coeff", "c", "r".

* Arguments option and direction of gsort will be more strict in all case.
  Only the following string will be allowed : option : "r" "c" "g" "lr" "lc"
                                              direction : "d" "i"
