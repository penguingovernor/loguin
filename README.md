# Loguin 🐧

A thread-safe logger implementation for C

![image](https://user-images.githubusercontent.com/13544676/82748221-28c4f980-9d55-11ea-841c-e287f17b4997.png)

## Getting the Code

If you'd like to introduce `loguin` into your project simply copy `loguin.h` and `loguin.c` into your project.

If you just want to simply call `loguin` without having to deal with building it, an include-only version is provided under `include/loguin_single.h`

## Building

Once you cloned this repo, call `make` to build the sample program.

## API

Add the following to the files you wish to use loguing with:

```c
// If using loguin.c and loguin.h
#include "loguin.h"
```

or

```c
// If only using loguin-single.h
#include "loguin_single.h"
```
See `include/login.h` for usage.
