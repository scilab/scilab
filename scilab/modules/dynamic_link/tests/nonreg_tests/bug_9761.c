// Please be sure that you build in "release mode"
void bug_9761(void)
{
#ifndef NDEBUG
#error "NDEBUG is not defined."
#endif
}
