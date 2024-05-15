#ifndef REQUESTS_OPENID_CONFIGURATION_H
#define REQUESTS_OPENID_CONFIGURATION_H

#include "requests.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <curl/curl.h>

response_t
request_discover_openid_configuration(
  const std::string &url
);

#endif // !REQUESTS_OPENID_CONFIGURATION_H
