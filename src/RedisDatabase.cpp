#include "../include/RedisDatabase.h"

RedisDatabase& RedisDatabase::getInstance() {
    static RedisDatabase instance;   // created once, shared by everyone
    return instance;
}

// Every function below is a TODO: write it yourself, one at a time.
// After each one, rebuild and test it with redis-cli (see the steps in the chat).
// Remember: lock the mutex first, e.g.  std::lock_guard<std::mutex> lock(db_mutex);

void RedisDatabase::flushAll() {
    // TODO: remove every key
    std::lock_guard<std::mutex> lock(db_mutex);
    kv_store.clear();
}

void RedisDatabase::set(const std::string& key, const std::string& value) {
    // TODO: store the value under the key (overwrite if it already exists)
    std::lock_guard<std::mutex> lock(db_mutex);
    kv_store[key] = value;
}

bool RedisDatabase::get(const std::string& key, std::string& value) {
    // TODO: if the key exists, put its value in `value` and return true; otherwise return false
    std::lock_guard<std::mutex> lock(db_mutex);
    auto it = kv_store.find(key);
    if (it != kv_store.end()) {
        value = it->second;
        return true;
    }
    return false;
}

bool RedisDatabase::del(const std::string& key) {
    // TODO: remove the key; return true if it existed, false otherwise
    std::lock_guard<std::mutex> lock(db_mutex);
    return kv_store.erase(key) > 0;
}

std::vector<std::string> RedisDatabase::keys() {
    // TODO: return a vector with every key
    std::lock_guard<std::mutex> lock(db_mutex);
    std::vector<std::string> result;
    for (const auto& pair : kv_store) {
        result.push_back(pair.first);
    }
    return result;
}

std::string RedisDatabase::type(const std::string& key) {
    // TODO: return "string" if the key exists, otherwise "none"
    std::lock_guard<std::mutex> lock(db_mutex);
    if (kv_store.find(key) != kv_store.end()) {
        return "string";
    }
    return "none";
}

bool RedisDatabase::expire(const std::string& key, int seconds) {
    // TODO (later step): make the key disappear after `seconds`; return false if the key does not exist
    std::lock_guard<std::mutex> lock(db_mutex);
    auto it = kv_store.find(key);
    if (it != kv_store.end()) {
        // For now, we'll just remove the key
        kv_store.erase(it);
        return true;
    }
    return false;
}

bool RedisDatabase::dump(const std::string& filename) {
    // TODO (later step): save all data to a file
    (void)filename;
    return false;
}

bool RedisDatabase::load(const std::string& filename) {
    // TODO (later step): read the file back into memory
    (void)filename;
    return false;
}
