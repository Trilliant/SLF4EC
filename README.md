# SLF4EC
## Simple Logging Facade for Embedded C

This library was developed with the same ideas as [SLF4J], [slf4net], [SLF4C] and other popular logging facade of similar name. The idea is to provide a lightweight, flexible solution for logging that can be re-used between projects and modules.

## Goals and features

### A lightweight logging solution
Very few lines of code are actually compiled in the binary. Most of the magic is done at compile time using macros.

### Support for logging categories
Logs are categorized according to a subsystem. For example, you could have categories called "Network", "GUI", "Power", etc.
Based on these categories you can read your logs easily by only looking at one subsystem or even filter what will be sent to the loggers by specifying a log level at which the category is active.

### Optimized for embedded development
- Using a few defines, you can control what log levels will actually be compiled in the final binary as well as if location informations will be included or not (file, function name and line number where the event occurred).
- No dynamic memory allocation. Everything happens on the stack.

### A simple API
Inside your application, simply call the logger in similar fashion to the examples below:
```C
logInfo(Network, "Network initialized successfully");
logFatal(CPU, "An error occurred %d times: %s", nbErrors, errorMessage);
```
These entries are replaced at compile time by a dummy call to save memory if the specific level is below the threshold of what is specified during compilation.

### Flexible runtime configuration
- Configuration of which configured loggers are active and what levels they will log can be changed at runtime.
- Configuration of which configured categories are active and what levels they will log can be changed at runtime.

### Add any logger you want
As an example, a logger to stdout is provided. But you can implement any logger you wish by providing 2 function pointers as defined in `slf4ecTypes.h`. The provided example shows how to do this. You could thus add new loggers that would write the entries to a file, send them over a UDP packet or do whatever else you desire.

### Multiple hosts support
SLF4EC currently compiles on Linux and Windows (through MSYS) using GNU Makefiles.

### Multiple compilers support
SLF4EC currently supports GCC (MinGW on Windows) and IAR (Windows only).

### Cross compilation support
Currently, the following targets are supported out of the box:
* x86 (using GCC)
* ARM Cortex-M4 (using GCC or IAR)

Other targets should work just as well since there are no external dependencies.

### Unit tests
Currently the library itself is 100% covered by unit tests, using [cmockery2].

### Quality through [sonarqube]
This library is being scanned by the [sonar-cxx], [Cppheck], [Vera++] and [RATS] plugins.

### API documentation
APIs are documented using doxygen.

### Licensing
Licensed using the MIT License to encourage sharing among other projects, open source or not.

[SLF4J]:http://www.slf4j.org/
[slf4net]:https://github.com/englishtown/slf4net
[SLF4C]:https://github.com/SLF4C/SLF4C
[cmockery2]:https://github.com/Trilliant/cmockery2
[sonarqube]:http://www.sonarqube.org/
[sonar-cxx]:https://github.com/wenns/sonar-cxx
[Cppheck]:http://cppcheck.sourceforge.net/
[Vera++]:https://bitbucket.org/verateam/vera/wiki/Home
[RATS]:https://code.google.com/p/rough-auditing-tool-for-security/