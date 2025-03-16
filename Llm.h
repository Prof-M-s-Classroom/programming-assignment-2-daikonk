#ifndef LLM_H
#define LLM_H
#include <curl/curl.h>
#include <curl/easy.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using namespace std;

class LLM {
public:
  string api_key;
  string theme;
  CURL *curl;

  LLM(string api_key, string theme) {
    this->api_key = api_key;
    this->theme = theme;
    curl_global_init(CURL_GLOBAL_ALL);
    this->curl = curl_easy_init();
  };

  ~LLM() {
    if (curl) {
      curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
  }

  string generate(string servicer) {
    string res;

    if (curl) {
      if (servicer == "1") {
        res = generateGPT();
      } else if (servicer == "3") {
        res = generateClaude();
      } else if (servicer == "2") {
        res = generateGemini();
      }
    }
    return res;
  };

private:
  string prompt = "Hello!";

  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              std::string *output) {
    size_t total_size = size * nmemb;
    output->append((char *)contents, total_size);
    return total_size;
  }

  string generateGPT() {
    string res = "";

    if (curl) {
      curl_easy_setopt(curl, CURLOPT_URL,
                       "https://api.openai.com/v1/chat/completions");

      struct curl_slist *headers = NULL;
      headers = curl_slist_append(headers, "Content-Type: application/json");
      string auth_header = "Authorization: Bearer " + api_key;
      headers = curl_slist_append(headers, auth_header.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

      json request_body = {
          {"model", "gpt-4o"},
          {"messages", json::array({{{"role", "user"}, {"content", prompt}}})}};

      string stringified = request_body.dump();

      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, stringified.c_str());

      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);

      CURLcode status = curl_easy_perform(curl);

      if (status != CURLE_OK) {
        res = "{ 'error': " + string(curl_easy_strerror(status)) + " }";
      }

      curl_slist_free_all(headers);
    }

    json res_json = json::parse(res);

    return res_json["choices"][0]["message"]["content"];
  }

  string generateClaude() {
    string res;
    return res;
  }

  string generateGemini() {
    string res;
    return res;
  }
};

#endif // NODE_H
