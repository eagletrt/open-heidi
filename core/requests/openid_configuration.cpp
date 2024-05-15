#include "openid_configuration.h"

static
size_t
_write_callback(
  char *ptr,
  size_t size,
  size_t nmemb,
  void *data
) {
  nlohmann::json *body = static_cast<nlohmann::json *>(data);
  *body = nlohmann::json::parse(std::string(ptr));
  return size * nmemb;
}

response_t
request_discover_openid_configuration(
  const std::string &url
) {
  size_t return_code;
  nlohmann::json body;

  CURLcode curl_ret;
  CURL *hnd;

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
  curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, _write_callback);
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &body);

  curl_ret = curl_easy_perform(hnd);
  curl_easy_getinfo (hnd, CURLINFO_RESPONSE_CODE, &return_code);

  curl_easy_cleanup(hnd);
  hnd = NULL;

  return {return_code, body};
}
