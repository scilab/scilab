NAME          TESTPROB
ROWS
 N  COST
 E  LIM1
 E  LIM2
 E  LIM3
 E  LIM4
 E  LIM5
 E  LIM6
COLUMNS
    XONE      COST                 1   LIM1                 1
    XONE      LIM2                 1   LIM5                 1
    YTWO      COST                 4   LIM1                 1
    YTWO      LIM5                 1  
    XONE1     COST                 1   LIM3                 1
    XONE1     LIM4                 1   LIM6                 1
    YTWO1     COST                 4   LIM3                 1
    YTWO1     LIM6                 1  
RHS
    RHS1      LIM1                 2   LIM2                 1
    RHS2      LIM3                 2   LIM4                 1
    RHS3      LIM5                 2   LIM6                 2

BOUNDS
 UP BND1      XONE                 4
 LO BND1      YTWO                -1
 UP BND1      YTWO                 1
 UP BND1      XONE1                4
 LO BND1      YTWO1               -1
 UP BND1      YTWO1                1
ENDATA




