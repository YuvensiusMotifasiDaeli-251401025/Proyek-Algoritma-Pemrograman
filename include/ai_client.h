#pragma once
// ============================================================
//  ai_client.h  —  Integrasi Anthropic Claude API via libcurl
// ============================================================
#include <string>
#include <vector>
#include <sstream>
#include <curl/curl.h>
#include "config.h"

// ── Struct pesan chat ────────────────────────────────────────
struct ChatMessage {
    std::string role;     // "user" atau "assistant"
    std::string content;
};

// ── Callback untuk libcurl ───────────────────────────────────
static size_t _write_cb(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

// ── Escape JSON string ───────────────────────────────────────
inline std::string json_escape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 16);
    for (unsigned char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:
                if (c < 0x20) {
                    char buf[8];
                    snprintf(buf, sizeof(buf), "\\u%04x", c);
                    out += buf;
                } else {
                    out += c;
                }
        }
    }
    return out;
}

// ── Ekstrak teks dari respons JSON Claude ────────────────────
inline std::string extract_text(const std::string& json) {
    // Cari pola: "text":"..."
    const std::string key = "\"text\":\"";
    size_t pos = json.find(key);
    if (pos == std::string::npos) {
        // Coba cari error message
        size_t ep = json.find("\"message\":\"");
        if (ep != std::string::npos) {
            ep += 11;
            size_t end = json.find('"', ep);
            return "[Error] " + json.substr(ep, end - ep);
        }
        return "[Gagal membaca respons AI]";
    }
    pos += key.size();

    // Unescape manual
    std::string result;
    bool escape = false;
    for (size_t i = pos; i < json.size(); ++i) {
        char c = json[i];
        if (escape) {
            switch (c) {
                case 'n':  result += '\n'; break;
                case 't':  result += '\t'; break;
                case '"':  result += '"';  break;
                case '\\': result += '\\'; break;
                default:   result += c;    break;
            }
            escape = false;
        } else if (c == '\\') {
            escape = true;
        } else if (c == '"') {
            break;
        } else {
            result += c;
        }
    }
    return result;
}

// ── Kirim request ke Claude API ──────────────────────────────
inline std::string ai_chat(
        const std::string& system_prompt,
        const std::vector<ChatMessage>& history) {

    CURL* curl = curl_easy_init();
    if (!curl) return "[libcurl init gagal]";

    // Build JSON messages array
    std::ostringstream msgs;
    msgs << "[";
    for (size_t i = 0; i < history.size(); ++i) {
        if (i > 0) msgs << ",";
        msgs << "{\"role\":\"" << history[i].role << "\","
             << "\"content\":\"" << json_escape(history[i].content) << "\"}";
    }
    msgs << "]";

    // Build full body
    std::ostringstream body;
    body << "{"
         << "\"model\":\"" << ANTHROPIC_MODEL << "\","
         << "\"max_tokens\":" << ANTHROPIC_MAX_TOK << ","
         << "\"system\":\"" << json_escape(system_prompt) << "\","
         << "\"messages\":" << msgs.str()
         << "}";

    std::string body_str = body.str();
    std::string response_str;

    // Headers
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("x-api-key: " + std::string(ANTHROPIC_API_KEY)).c_str());
    headers = curl_slist_append(headers, "anthropic-version: 2023-06-01");

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.anthropic.com/v1/messages");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_str.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_str);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    CURLcode res = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        return std::string("[libcurl error] ") + curl_easy_strerror(res);

    return extract_text(response_str);
}
