#include "spfLogger.h"

int main()
{
  spf::Logger logger("test.log");

  logger.log_debug("testing %.10f", 3.14);
  logger.log_debug("testing %4d, %.3g", 1234, 1.23e-3);


  return 0;
}
