
#include "babylog.h"

/* another c-file that also uses babylog */

void foo()
{
    babylog_printf("one log-message from foo() in file '%s'\n", __FILE__);
    babylog_printf("another log from foo() in file '%s'\n", __FILE__);
}

