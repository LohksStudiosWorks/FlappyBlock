#pragma once

#include <string>

#if (IS_ANDROID == 0)
std::string get_latest_version();
#endif