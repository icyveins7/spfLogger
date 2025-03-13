#include "spfLogger.h"

int main()
{
  spf::Logger logger("test.log");

  logger().debug("testing %.10f\n", 3.14);
  logger().debug("testing %4d, %.3g\n", 1234, 1.23e-3);

  logger().info("testing %4s\n", "just checking");
  logger().error("oh no %4d, %.3g\n", 12345, 1.23e6);

  // You cannot do this, for good reason!
  // FILE *fe;
  // spf::PrivateLogWriter(std::source_location::current(), fe, fe);


  double x = 1234.5;
  logger().inspect(x, "%.4f\n");
  logger().inspect(x, "%7.3g\n");

  return 0;
}
