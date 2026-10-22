#ifndef REDIS_DATABASE_H
#define REDIS_DATABASE_H

#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <chrono>
// One shared database for the whole server. Many client threads call it at the
// same time, so every public function must lock db_mutex before touching data.
class RedisDatabase {
    private:
        std::unordered_map<std::string, std::chrono::steady_clock::time_point> expiry_map;
        bool isExpired(const std::string& key);   // helper: the caller must already hold the lock
public:
    static RedisDatabase& getInstance();

    // --- common ---
    void flushAll();                                         // delete everything

    // --- key / value ---
    void set(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& value);    // true if the key exists
    bool del(const std::string& key);                        // true if the key existed
    std::vector<std::string> keys();                         // all keys
    std::string type(const std::string& key);                // "string" or "none"
    bool expire(const std::string& key, int seconds);        // false if the key does not exist

    // --- persistence (stubs for now; the real version comes later) ---
    bool dump(const std::string& filename);
    bool load(const std::string& filename);

private:
    RedisDatabase() = default;
    RedisDatabase(const RedisDatabase&) = delete;
    RedisDatabase& operator=(const RedisDatabase&) = delete;

    std::mutex db_mutex;
    std::unordered_map<std::string, std::string> kv_store;   // later: replace with YOUR hash table
    // TODO (expiry step): store an expiry time for each key
};

#endif
