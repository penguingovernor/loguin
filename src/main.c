#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "loguin.h"

int main(int argc, char const *argv[])
{
  loguin_logger_t *logger = &loguin_default;
  const char *usageString = "Usage of loguin:\n\tloguin [nPenguins]\n";
  const int requiredArgs = 1;
  if(argc - 1 != requiredArgs) {
    loguin_printf(logger, "%s\n", usageString);
    loguin_fatalf(logger, EXIT_FAILURE, "wrong number of arguments; got %d wanted %d\n", argc - 1, requiredArgs);
  }

  const char *numPenguinsString = argv[1];
  for(size_t i = 0; i < strlen(numPenguinsString); i++) {
    if(!isdigit(numPenguinsString[i])) {
      loguin_fatalf(logger, EXIT_FAILURE, "could not convert %s to a number\n", numPenguinsString);
    }
  }

  int numPenguins = atoi(numPenguinsString);
  const int tooManyPenguins = 10;

  if(numPenguins > tooManyPenguins) {
    loguin_warnf(logger, "the ice can only hold %d penguins, it will break with %d!\n", tooManyPenguins, numPenguins);
    return EXIT_SUCCESS;
  }

  loguin_logf(logger, "the ice safely holds %d penguins\n", numPenguins);
  return EXIT_SUCCESS;
}
