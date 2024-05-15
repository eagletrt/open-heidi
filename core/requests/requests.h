#ifndef REQUESTS_H
#define REQUESTS_H

#include <nlohmann/json.hpp>

struct response_t {
  size_t code;
  nlohmann::json body;
};

#endif // !REQUESTS_H



