#ifndef LLM_H
#define LLM_H
#include <curl/curl.h>
#include <curl/easy.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

using json = nlohmann::json;
using namespace std;

class LLM {
public:
  string apiKey;
  string theme;
  string choice;
  string res;
  CURL *curl;

  LLM(string apiKey, string theme, string choice) {
    this->apiKey = apiKey;
    this->theme = theme;
    this->choice = choice;
    this->res = "";
    this->prompt = loadPrompt(theme);
    curl_global_init(CURL_GLOBAL_ALL);
    this->curl = curl_easy_init();
  };

  ~LLM() {
    if (curl) {
      curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
  }

  string generate() {
    string res;

    if (curl) {
      if (choice == "1") {
        res = generateGPT();
      } else if (choice == "2") {
        res = generateGemini();
      } else if (choice == "3") {
        res = generateClaude();
      } else {
        res = "INVALID CHOICE VALUE " + choice;
      }
    }
    return res;
  };

private:
  string prompt;

  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              std::string *output) {
    size_t total_size = size * nmemb;
    output->append((char *)contents, total_size);
    return total_size;
  }

  string loadPrompt(string theme) {
    std::ifstream file("prompt.txt");
    if (!file.is_open()) {
      std::cerr << "Failed to open file: prompt.txt" << endl;
      return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string prompt = buffer.str();

    size_t pos = prompt.find("{theme}");
    prompt.replace(pos, 8, theme);

    return prompt;
  }

  string generateGPT() {

    const string OpenAIUrl = "https://api.openai.com/v1/chat/completions";

    // Header Generation
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    string authHeader = "Authorization: Bearer " + apiKey;
    headers = curl_slist_append(headers, authHeader.c_str());

    // Body Construction
    json reqBody = {
        {"model", "gpt-4o"},
        {"messages", json::array({{{"role", "user"}, {"content", prompt}}})}};
    string stringifiedBody = reqBody.dump();

    CURLcode status = POSTRequest(OpenAIUrl, headers, stringifiedBody, res);

    curl_slist_free_all(headers);

    long httpCode = 0;
    if (!ensureSuccess(status, res, httpCode)) {
      return res;
    }

    try {
      json resJson = json::parse(res);
      return resJson["choices"][0]["message"]["content"];
    } catch (...) {
      return "Error parsing response..";
    }
  }

  string generateGemini() {

    const string GeminiUrl =
        "https://generativelanguage.googleapis.com/v1beta/models/"
        "gemini-flash-2:generateContent?key=" +
        apiKey;

    // Header Generation
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Body Construction
    json reqBody = {
        {"contents",
         json::array(
             {{{"role", "user"}, {"parts", json::array({{"text", prompt}})}}})},
        {"generationConfig", {{"temperature", 0.7}}}};
    string stringifiedBody = reqBody.dump();

    CURLcode status = POSTRequest(GeminiUrl, headers, stringifiedBody, res);

    curl_slist_free_all(headers);

    long httpCode = 0;
    if (!ensureSuccess(status, res, httpCode)) {
      return res;
    }

    try {
      json resJson = json::parse(res);
      return resJson["candidates"][0]["content"]["parts"][0]["text"];
    } catch (...) {
      return "Error parsing response";
    }
  }

  string generateClaude() {

    const string AnthropicUrl = "https://api.anthropic.com/v1/messages";

    // Header Generation
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    string authHeaders = "x-api-key: " + apiKey;
    headers = curl_slist_append(headers, authHeaders.c_str());
    headers = curl_slist_append(headers, "anthropic-version: 2023-06-01");

    // Body Construction
    json reqBody = {
        {"model", "claude-3-7-sonnet-20250219"},
        {"messages", json::array({{{"role", "user"}, {"content", prompt}}})}};
    string stringifiedBody = reqBody.dump();

    CURLcode status = POSTRequest(AnthropicUrl, headers, stringifiedBody, res);

    curl_slist_free_all(headers);

    long httpCode = 0;
    if (!ensureSuccess(status, res, httpCode)) {
      return res;
    }

    try {
      json resJson = json::parse(res);
      return resJson["content"][0]["text"];
    } catch (...) {
      return "Error parsing response";
    }
  }

  // Helpers, breaking both of these off reduces redundancy
  CURLcode POSTRequest(const string &url, const curl_slist *headers,
                       const string &data, string &res) {

    res.clear();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);

    return curl_easy_perform(curl);
  }

  bool ensureSuccess(CURLcode status, const string &res, long &httpCode) {
    if (status != CURLE_OK) {
      return false;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    return httpCode == 200;
  }
};
#endif // NODE_H
