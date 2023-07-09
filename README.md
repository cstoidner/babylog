# babylog
easy as pie debug-message logger, intended for bootcode or other minimalist environments

## how to use babylog 
Example use for babylog is available with test application.

### Concept of embedding babylog into an applicationn
Inention of babylog is to make it very easy using it in any application, espcially very minimalistic application (e.g. 1st step boot code (e.g. U-Boot's SPL). Therefor babylog has to important elements:
   * use of as less as possible stdlib functionality
     * babylog_vsnprint() can be optionally used; 
       if vsnprintf() not available provided by the target environment 
       then it can be disabled and instead basic function without C-formatting
       feature can be used: `babylog_putNamed_i()`, `babylog_putNamed_s()`, ... 
   * easy way to integrate babylog header and implementation into target application;
     the idea is to have prototypes and implementation in a c header-file, and use
     include to embed the babylog's implementation in exactly one c-file of the target
     appliction.
     * so babylog's whole babylog (function prototypes and implenentation) is located
       the one and only header file `babylog.h`. That header file is intended for two
       purposes:
       * 1) include that file in exactly one of your c-files to have babylog's
            prototypes available in that file; and set the preprocessor macro
            `BABYLOG_HAVE_IMPLEMENTATION` in the line before the `include` directive.
            That macro tells `babylog.h` to activate babylog's function-bodies and
            so embeds the implementation of babylog into that c-file.
       * 2) when (a) above done, then optionally include `babylog.h` also in any
            other c-file of your application, but without the macro
            `BABYLOG_HAVE_IMPLEMENTATION`. So for those c-file `babylog.h` does not
            activate the function-bodies but provides the prototypes only.


Build test application with command line:

  `gcc test.c test-another-file.c -o test.out`

Then run with:

  `./test.out`

The test application shows:
  * how to embed babylog into one c-file, just with include (i.E. prototypes AND code implementation)
  * how to use babylog also on other c-file, just with include (on those others just prototypes)
  * how to create log-messages
  * how to get and output log-buffer
  * how detection of log-buffer overflow works
