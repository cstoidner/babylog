
// babylog test application

#include <stdio.h>

#define BABYLOG_BUFSIZE 256 /* plus trailing zero */ +1
#define BABYLOG_HAVE_IMPLEMENTATION

#include "babylog.h"

int main()
{
    printf("Babylog Test Application\n");

    printf("*** Creating some basic log messages...\n");

	babylog_printf("That is the 1st constant log message\n");
    babylog_printf("That is the 2nd constant log message\n");
    babylog_printf("Some %s log message\n", "formatted");
    babylog_printf("Some log with numbers hex(%x) dec(%d)\n", 0xdeadbeef, 42);
    babylog_printf("a message-");
    babylog_printf("-consisting-");
    babylog_printf("-of-");
    babylog_printf("%d segments\n", 4);

	printf("Entire Log Buffer:\n");
	printf("--- start ---\n");
    printf("%s", babylog_getLog());
    printf("--- end ---\n");
    printf("Log Status: %d\n", babylog_getStatus());

    printf("*** testing overflow...\n");

    babylog_reset();
    printf("\n\n\n");
    printf("should exactly fit\n");
    babylog_printf("1st 64 bytes message............:..............................\n");
    babylog_printf("2nd 64 bytes message............:..............................\n");
    babylog_printf("3rd 64 bytes message............:..............................\n");
    babylog_printf("4th 64 bytes message............:.............................\n");

    printf("Entire Log Buffer:\n");
    printf("--- start ---\n");
    printf("%s", babylog_getLog());
    printf("--- end ---\n");
    printf("Log Status: %d\n", babylog_getStatus());

    babylog_reset();
    printf("\n\n\n");
    printf("provoking overflow and ignore messages after overflow\n");
    babylog_printf("1st 64 bytes message............:..............................\n");
    babylog_printf("2nd 64 bytes message............:..............................\n");
    babylog_printf("3rd 64 bytes message............:..............................\n");
    babylog_printf("4th 64 bytes message............:.............................\n");
    babylog_printf("5th leads with 1st byte to overflow, hopefully ignored\n");
    babylog_printf("6th overflow occured before, so also ignored\n");

    printf("Entire Log Buffer:\n");
    printf("--- start ---\n");
    printf("%s", babylog_getLog());
    printf("--- end ---\n");
    printf("Log Status: %d\n", babylog_getStatus());

    printf("done.\n");
}

