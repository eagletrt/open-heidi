#include "token.h"

#include "../../external/cpp-httplib/httplib.h"

std::optional<auth_token_endpoint_body_t> request_token(
    const std::string &host, const std::string &path, const std::string &code) {
  httplib::Client cli(host.c_str());

  httplib::MultipartFormDataItems items = {{"code", code}};

  httplib::Result res = cli.Post(path, items);
  std::cout << res.value().body << std::endl;
  if (res) {
    auth_token_endpoint_body_t body;
    nlohmann::json parsed = nlohmann::json::parse(res.value().body);
    body.access_token = parsed["access_token"];
    body.token_type = parsed["token_type"];
    body.refresh_token = parsed["refresh_token"];
    body.expires_in = parsed["expires_in"];
    body.scope = parsed["scope"];
    return body;
  } else {
    std::cerr << "Error: " << res.error() << std::endl;
    return std::nullopt;
  }
}
