static void init(){}
static void fini(){}
static void preinit(){}
typedef void (*fp)();
fp __init_array_start[]={init};
fp __init_array_end[]={(init)+1};
fp __fini_array_start[]={fini};
fp __fini_array_end[]={fini+1};

fp __preinit_array_start[]={preinit};
fp __preinit_array_end[]={preinit+1};



