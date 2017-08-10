#include <stack-c.h>

int ex2c(double * a, int * ma, int * na,
         double * b, int * mb, int * nb)
{
    int i;

    for (i = 0; i < (*ma) * (*na); i++)
    {
        a[i] = 2 * a[i];
    }
    for (i = 0; i < (*mb) * (*nb); i++)
    {
        b[i] = 3 * b[i];
    }

    return (0);
}

int sci_optional_parameters(char * fname)
{
    int m1, n1, l1;

    // optional names must be stored in alphabetical order in opts
    static rhs_opts opts[] = {{ -1, "v1", "d", 0, 0, 0},
        { -1, "v2", "d", 0, 0, 0},
        { -1, NULL, NULL, 0, 0}
    };

    int minrhs = 1, maxrhs = 1;
    int minlhs = 1, maxlhs = 3;
    int nopt, iopos, res;
    char buffer_name[csiz]; // csiz used for character coding

    nopt = NumOpt();

    CheckRhs(minrhs, maxrhs + nopt);
    CheckLhs(minlhs, maxlhs);

    // first non optional argument
    GetRhsVar( 1, "c", &m1, &n1, &l1);

    if (get_optionals(fname, opts) == 0)
    {
        return 0;
    }

    // default values if optional arguments are not given:  v1=[99] and v2=[3]

    sciprint("number of optional parameters = %d\n", NumOpt());
    sciprint("first optional parameters = %d\n", FirstOpt());
    sciprint("FindOpt(v1) = %d\n", FindOpt("v1", opts));
    sciprint("FindOpt(v2) = %d\n", FindOpt("v2", opts));

    if (IsOpt(1, buffer_name))
    {
        sciprint("parameter 1 is optional: %s\n", buffer_name);
    }
    if (IsOpt(2, buffer_name))
    {
        sciprint("parameter 2 is optional: %s\n", buffer_name);
    }
    if (IsOpt(3, buffer_name))
    {
        sciprint("parameter 3 is optional: %s\n", buffer_name);
    }

    iopos = Rhs;

    if (opts[0].position == -1)
    {
        iopos++;
        opts[0].position = iopos;
        opts[0].m = 1;
        opts[0].n = 1;
        opts[0].type = "d";
        CreateVar(opts[0].position, opts[0].type, &opts[0].m, &opts[0].n, &opts[0].l);
        *stk(opts[0].l) = 99.0;
    }

    if (opts[1].position == -1)
    {
        iopos++ ;
        opts[1].position = iopos;
        opts[1].m = 1;
        opts[1].n = 1;
        opts[1].type = "d";
        CreateVar(opts[1].position, opts[1].type, &opts[1].m, &opts[1].n, &opts[1].l);
        *stk(opts[1].l) = 3;
    }

    ex2c(stk(opts[0].l), &opts[0].m, &opts[0].n,
         stk(opts[1].l), &opts[1].m, &opts[1].n);

    // return the first argument (unchanged ) then v1 and v2

    LhsVar(1) = 1;
    LhsVar(2) = opts[0].position;
    LhsVar(3) = opts[1].position;

    return 0;
}

