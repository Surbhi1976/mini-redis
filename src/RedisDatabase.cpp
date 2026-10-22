#include "../include/RedisDatabase.h"

RedisDatabase& RedisDatabase::getInstance() {
    static RedisDatabase instance;   // created once, shared by everyone
    return instance;
}

// Every function below is a TODO: write it yourself, one at a time.
// After each one, rebuild and test it with redis-cli (see the steps in the chat).
// Remember: lock the mutex first, e.g.  std::lock_guard<std::mutex> lock(db_mutex);

void RedisDatabase::flushAll() {
    //  remove every key
    std::lock_guard<std::mutex> lock(db_mutex);
    kv_store.clear();
    expiry_map.clear();
}

void RedisDatabase::set(const std::string& key, const std::string& value) {
    //  store the value under the key (overwrite if it already exists)
    std::lock_guard<std::mutex> lock(db_mutex);
    kv_store[key] = value;
    expiry_map.erase(key);
}

bool RedisDatabase::get(const std::string& key, std::string& value) {
    // if the key exists, put its value in `value` and return true; otherwise return false
    std::lock_guard<std::mutex> lock(db_mutex);
    if (isExpired(key)) return false; 
    auto it = kv_store.find(key);
    if (it != kv_store.end()) {
        value = it->second;
        return true;
    }
    return false;
}

bool RedisDatabase::del(const std::string& key) {
    //  remove the key; return true if it existed, false otherwise
    std::lock_guard<std::mutex> lock(db_mutex);
    if (isExpired(key)) return false;
    expiry_map.erase(key);  
    return kv_store.erase(key) > 0;
}
std::vector<std::string> RedisDatabase::keys() {
    std::lock_guard<std::mutex> lock(db_mutex);
    std::vector<std::string> all, result;
    for (const auto& pair : kv_store) {
        all.push_back(pair.first);
    }
    for (const auto& k : all) {
        if (!isExpired(k)) result.push_back(k);
    }
    return result;
}

std::string RedisDatabase::type(const std::string& key) {
    // return "string" if the key exists, otherwise "none"
    std::lock_guard<std::mutex> lock(db_mutex);
    if (isExpired(key)) return "none";
    if (kv_store.find(key) != kv_store.end()) {
        return "string";
    }
    return "none";
}
bool RedisDatabase::expire(const std::string& key, int seconds) {
    std::lock_guard<std::mutex> lock(db_mutex);
    if (kv_store.find(key) == kv_store.end()) return false;   // no such key
    expiry_map[key] = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
    return true;
}

bool RedisDatabase::isExpired(const std::string& key) {
    auto it = expiry_map.find(key);
    if (it == expiry_map.end()) return false;               // this key has no timer

    if (std::chrono::steady_clock::now() >= it->second) {   // its time has passed
        // TODO: erase `key` from kv_store
        kv_store.erase(key);
        // TODO: erase `key` from expiry_map
        expiry_map.erase(key);
        return true;
    }
    return false;
}

bool RedisDatabase::dump(const std::string& filename) {
    //   save all data to a file
    (void)filename;
    return false;
}

bool RedisDatabase::load(const std::string& filename) {
    //   read the file back into memory
    (void)filename;
    return false;
}
