#include "misc.h"

#include <cstdlib>

namespace misc
{

void open_url(const std::string& url)
{
  // https://stackoverflow.com/users/12703286/jonathan-six
  // https://stackoverflow.com/a/76964675/2038264
  // https://creativecommons.org/licenses/by-sa/4.0/
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  std::system(("start " + url).c_str());
#elif __APPLE__
  std::system(("open " + url).c_str());
#elif __linux__
  std::system(("xdg-open " + url).c_str());
#else
#error "Unknown compiler"
#endif
}

}
