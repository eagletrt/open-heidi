#include "server.h"

AuthCallbackServer::AuthCallbackServer(const std::string &client_id) {
  this->client_id = std::string(client_id);
  this->redirect_uri = "http://localhost:8080/callback";
  this->state = std::format("{:x}", random_int());

  if (!this->srv.is_valid()) {
    std::cerr << "Server has an invalid configuration" << std::endl;
    throw std::exception();
  }

  this->srv.Get(
      "/callback", [&](const httplib::Request &req, httplib::Response &res) {
        if (!req.has_param("scope") || !req.has_param("state") ||
            !req.has_param("code")) {
          res.set_content("Missing parameters", "text/plain");
          res.status = httplib::StatusCode::BadRequest_400;
          return;
        }

        if (state != req.get_param_value("state")) {
          res.set_content("Invalid state", "text/plain");
          res.status = httplib::StatusCode::BadRequest_400;
          return;
        }

        this->response = CallbackResponse{req.get_param_value("scope"),
                                          req.get_param_value("state"),
                                          req.get_param_value("code")};

        res.set_redirect("https://rroll.to/OyhFjM");
        this->cv.notify_all();
      });
}

std::string AuthCallbackServer::start() {
  this->server_thread = std::thread([this] {
    try {
      this->srv.listen("localhost", 8080);
      this->lock.lock();
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  });

  return std::format(
      "https://accounts.google.com/o/oauth2/v2/auth?"
      "client_id={}&"
      "redirect_uri={}&"
      "scope=openid%20profile%20email&offline_access&"
      "response_type=code&"
      "prompt=consent&"
      "access_type=offline&"
      "state={}&"
      "hd=eagletrt.it",
      client_id, redirect_uri, state);
}

void AuthCallbackServer::stop() {
  this->srv.stop();
  this->server_thread.join();
}

bool AuthCallbackServer::is_running() { return this->srv.is_running(); }

CallbackResponse AuthCallbackServer::wait_response(int timeout) {
  std::unique_lock<std::mutex> lock(this->lock);
  std::cv_status w = this->cv.wait_for(lock, std::chrono::seconds(timeout));
  if (w == std::cv_status::timeout) {
    std::cerr << "Timeout reached" << std::endl;
    throw std::exception();
  }
  return this->response.value();
}

int AuthCallbackServer::random_int() {
  static thread_local std::mt19937 generator;
  std::uniform_int_distribution<uint64_t> distribution(
      0, std::numeric_limits<std::uint64_t>::max());
  return distribution(generator);
}
