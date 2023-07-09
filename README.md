# babylog
easy as pie debug-message logger, intended for bootcode or other minimalist environments

## Intention and idea of babylog
The intention of babylog is to ease debugging of very minimalistic applications (e.g. bootloaders / boot-code).

The idea for babylog came when deugging the SPL (Secondary Program Loader) of the bootloader "Das U-Boot". The SPL is U-Boot's very early bootcode that must be super small to fit in every SoC's (System-On-Chip) oboard S-RAM. Therefore:
   * SPL has just minimal C standard lib support
   * A significant portion of the SPL runs in a very early boot-state, where no interfaces are available to print debug messages (e.g. serial UART)

All that makes debugging very hard, especially for the inital system/board bring-up.

### How babylog ease deubgging
Babylog allows to put various debug messages into a static buffer and print them later to some interface (e.g. UART) whenever that interface was initialized.

Therefore babylog has to important elements:
   * use of as less as possible stdlib functionality
     * babylog's basic functions (`babylog_putNamed_i()`, `babylog_putNamed_s()`, ...)
       have almost no C stdlib dependency; they can be used to create a log message with
       a single value (e.g. `int` in decimal format, `int` in hex format, `string`, ...) and a name-string;
     * optionally the function `babylog_printf()` allows to create log message with
       the printf-like C-formatting style; `babylog_printf()` can be disabled if
       the target environmanet does not support `vsnprintf()`;
   * easy way to integrate babylog into the target application
     * the idea is to have all babylog's prototypes **and** implementation in just one c **header**-file (i.E. there is **no** *.c file at all);
       that header-file is used to embed (or include) the babylog's implementation into exactly one c-file of the target appliction.
     * So that `babylog.h` file is used for two purposes:
       * 1) Include that file in exactly one of your target application's c-files,
            to have all babylog's prototypes available in that file.
            Then set the preprocessor macro `BABYLOG_HAVE_IMPLEMENTATION` in the line
            before the `include` directive.
            That macro tells `babylog.h` to activate babylog's function-bodies and
            so embeds the implementation of babylog into that target application's c-file.
       * 2) When (a) above applied, then optionally include the same `babylog.h` also in any
            other c-file of the target application, but without the macro
            `BABYLOG_HAVE_IMPLEMENTATION`. So for those c-file `babylog.h` does not
            activate the function-bodies but provides all babylog's prototypes.

## How to use babylog 
Example use for babylog is available with the "test application".

Build the "test application" with command line:

  `gcc test.c test-another-file.c -o test.out`

Then run with:

  `./test.out`

The test application shows:
  * how to embed babylog into one c-file, just with include (i.E. prototypes AND code implementation)
  * how to use babylog also on other c-file, just with include (on those others just prototypes)
  * how to create log-messages
  * how to get and output log-buffer
  * how detection of log-buffer overflow works
