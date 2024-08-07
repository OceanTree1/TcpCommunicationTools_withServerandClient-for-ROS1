#include <iostream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <thread>
// 带线程安全的字典
template <typename Key, typename Value>
class ConcurrentDictionary {
private:
    std::unordered_map<Key, Value> dictionary;
    std::mutex mutex;

public:
    // 写入操作
    void Insert(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(mutex);
        dictionary[key] = value;
    }

    // 尝试获取字典中的值
    bool TryGetValue(const Key& key, Value& value) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = dictionary.find(key);
        if (it != dictionary.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

    //找Value，使用方式if(GetValueForKey(inputkey,findValue))
    bool GetValueForKey(const Key& key, Value& value) { 
        std::lock_guard<std::mutex> lock(mutex);
        auto it = dictionary.find(key);
        if (it != dictionary.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

    // 将相同Key中的Value进行对比
    bool Compare(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = dictionary.find(key);
        if (it != dictionary.end() && it->second == value) {
            return true;    // 返回正确代表发现是一样的
        }
        return false;   // 返回false代表不是一样的
    }

    // 寻找字典中是否有这个Key
    bool ContainsKey(const Key& key) { 
        std::lock_guard<std::mutex> lock(mutex);
        return dictionary.find(key) != dictionary.end();
    }

    // 去除字典中的值
    void Remove(const Key& key) {
        std::lock_guard<std::mutex> lock(mutex);
        dictionary.erase(key);
    }

    // 清空字典
    void Clear() {
        std::lock_guard<std::mutex> lock(mutex);
        dictionary.clear();
    }
};