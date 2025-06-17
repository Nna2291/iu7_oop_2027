#include "hashmap/HashMap.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <list/List.h>
#include <cmath>

class TestRunner {
private:
    int totalTests = 0;
    int passedTests = 0;
    
public:
    void runTest(const std::string& testName, bool condition) {
        totalTests++;
        std::cout << "Test: " << std::left << std::setw(40) << testName;
        if (condition) {
            std::cout << "PASSED" << std::endl;
            passedTests++;
        } else {
            std::cout << "FAILED" << std::endl;
        }
    }
    
    void printSummary() {
        std::cout << "\n=== TEST SUMMARY ===" << std::endl;
        std::cout << "Total tests: " << totalTests << std::endl;
        std::cout << "Passed: " << passedTests << std::endl;
        std::cout << "Failed: " << (totalTests - passedTests) << std::endl;
        std::cout << "Success rate: " << std::fixed << std::setprecision(1) 
                  << (totalTests > 0 ? (double)passedTests / totalTests * 100 : 0) << "%" << std::endl;
    }
};

template <typename T>
void printList(const List<T>& list, const std::string& prefix = "") {
    std::cout << prefix << "[";
    bool first = true;
    for (const auto& item : list) {
        if (!first) std::cout << ", ";
        std::cout << item;
        first = false;
    }
    std::cout << "]" << std::endl;
}

template <typename K, typename V>
void printMap(const HashMap<K, V>& map, const std::string& prefix = "") {
    std::cout << prefix << "{";
    bool first = true;
    for (const auto& pair : map) {
        if (!first) std::cout << ", ";
        std::cout << pair.first << ": " << pair.second;
        first = false;
    }
    std::cout << "}" << std::endl;
}

void testListBasicOperations(TestRunner& runner) {
    std::cout << "\n=== LIST BASIC OPERATIONS ===" << std::endl;
    
    List<int> list;
    
    // Test empty list
    runner.runTest("Empty list size is 0", list.getSize() == 0);
    
    // Test pushBack
    list.pushBack(10);
    runner.runTest("pushBack(10) - size is 1", list.getSize() == 1);
    runner.runTest("pushBack(10) - back element is 10", list.getBack() == 10);
    runner.runTest("pushBack(10) - front element is 10", list.getFront() == 10);
    
    // Test pushFront
    list.pushFront(5);
    runner.runTest("pushFront(5) - size is 2", list.getSize() == 2);
    runner.runTest("pushFront(5) - front element is 5", list.getFront() == 5);
    runner.runTest("pushFront(5) - back element is 10", list.getBack() == 10);
    
    printList(list, "Current list: ");
    
    // Test popFront
    list.popFront();
    runner.runTest("popFront() - size is 1", list.getSize() == 1);
    runner.runTest("popFront() - front element is 10", list.getFront() == 10);
    
    // Test popBack
    list.popBack();
    runner.runTest("popBack() - size is 0", list.getSize() == 0);
}

void testListAdvancedOperations(TestRunner& runner) {
    std::cout << "\n=== LIST ADVANCED OPERATIONS ===" << std::endl;
    
    List<int> list;
    
    // Test resize
    list.resize(3);
    runner.runTest("resize(3) - size is 3", list.getSize() == 3);
    runner.runTest("resize(3) - all elements are 0", 
                   list[0] == 0 && list[1] == 0 && list[2] == 0);
    
    list.resize(5, 7);
    runner.runTest("resize(5, 7) - size is 5", list.getSize() == 5);
    runner.runTest("resize(5, 7) - new elements are 7", 
                   list[3] == 7 && list[4] == 7);
    
    printList(list, "After resize: ");
    
    // Test initializer list assignment
    list = {1, 2, 3, 4, 5};
    runner.runTest("Assignment {1,2,3,4,5} - size is 5", list.getSize() == 5);
    runner.runTest("Assignment {1,2,3,4,5} - correct values", 
                   list[0] == 1 && list[2] == 3 && list[4] == 5);
    
    printList(list, "After assignment: ");
    
    // Test element access
    runner.runTest("list[3] access", list[3] == 4);
    runner.runTest("list.at(2) access", list.at(2) == 3);
    
    // Test find and erase
    auto it = std::find(list.begin(), list.end(), 3);
    bool found = (it != list.end());
    runner.runTest("find(3) - element found", found);
    
    if (found) {
        list.erase(it);
        runner.runTest("erase(3) - size decreased", list.getSize() == 4);
        auto it2 = std::find(list.begin(), list.end(), 3);
        runner.runTest("erase(3) - element not found after erase", it2 == list.end());
    }
    
    printList(list, "After erase: ");
    
    // Test clear
    list.clear();
    runner.runTest("clear() - size is 0", list.getSize() == 0);
}

void testListConstructors(TestRunner& runner) {
    std::cout << "\n=== LIST CONSTRUCTORS ===" << std::endl;
    
    // Test size constructor
    List<int> list1(5, 7);
    runner.runTest("List(5, 7) - size is 5", list1.getSize() == 5);
    runner.runTest("List(5, 7) - all elements are 7", 
                   list1[0] == 7 && list1[2] == 7 && list1[4] == 7);
    
    // Test size-only constructor
    List<int> list2(3);
    runner.runTest("List(3) - size is 3", list2.getSize() == 3);
    runner.runTest("List(3) - elements are default initialized", 
                   list2[0] == 0 && list2[1] == 0 && list2[2] == 0);
    
    printList(list1, "List(5, 7): ");
    printList(list2, "List(3): ");
}

void testHashMapBasicOperations(TestRunner& runner) {
    std::cout << "\n=== HASHMAP BASIC OPERATIONS ===" << std::endl;
    
    HashMap<std::string, int> map;
    
    // Test empty map
    runner.runTest("Empty map size is 0", map.getSize() == 0);
    
    // Test insert
    auto result1 = map.insert("hello", 10);
    runner.runTest("insert('hello', 10) - success", result1.second == true);
    runner.runTest("insert('hello', 10) - size is 1", map.getSize() == 1);
    
    auto result2 = map.insert("world", 20);
    runner.runTest("insert('world', 20) - success", result2.second == true);
    runner.runTest("insert('world', 20) - size is 2", map.getSize() == 2);
    
    // Test duplicate insert
    auto result3 = map.insert("hello", 30);
    runner.runTest("insert duplicate 'hello' - fails", result3.second == false);
    runner.runTest("insert duplicate 'hello' - size unchanged", map.getSize() == 2);
    
    printMap(map, "After inserts: ");
    
    // Test pair insert
    map.insert(std::make_pair("test", 15));
    runner.runTest("insert(make_pair('test', 15)) - size is 3", map.getSize() == 3);
    
    printMap(map, "After pair insert: ");
}

void testHashMapAdvancedOperations(TestRunner& runner) {
    std::cout << "\n=== HASHMAP ADVANCED OPERATIONS ===" << std::endl;
    
    HashMap<std::string, int> map;
    map.insert("apple", 1);
    map.insert("banana", 2);
    map.insert("cherry", 3);
    
    // Test insert_or_assign
    map.insert_or_assign("apple", 10);
    runner.runTest("insert_or_assign existing key", map.at("apple") == 10);
    
    map.insert_or_assign("date", 4);
    runner.runTest("insert_or_assign new key - size increased", map.getSize() == 4);
    runner.runTest("insert_or_assign new key - value correct", map.at("date") == 4);
    
    // Test element access
    runner.runTest("operator[] access", map["banana"] == 2);
    runner.runTest("at() access", map.at("cherry") == 3);
    
    // Test find
    auto it = map.find("banana");
    runner.runTest("find('banana') - found", it != map.end());
    runner.runTest("find('banana') - correct value", it->second == 2);
    
    auto it2 = map.find("nonexistent");
    runner.runTest("find('nonexistent') - not found", it2 == map.end());
    
    // Test contains
    runner.runTest("contains('apple')", map.contains("apple"));
    runner.runTest("contains('nonexistent') - false", !map.contains("nonexistent"));
    
    printMap(map, "Before erase: ");
    
    // Test erase
    bool erased = map.erase("banana");
    runner.runTest("erase('banana') - success", erased);
    runner.runTest("erase('banana') - size decreased", map.getSize() == 3);
    runner.runTest("erase('banana') - element not found", !map.contains("banana"));
    
    bool erasedNonexistent = map.erase("nonexistent");
    runner.runTest("erase('nonexistent') - fails", !erasedNonexistent);
    
    printMap(map, "After erase: ");
    
    // Test clear
    map.clear();
    runner.runTest("clear() - size is 0", map.getSize() == 0);
}

void testHashMapConstructors(TestRunner& runner) {
    std::cout << "\n=== HASHMAP CONSTRUCTORS ===" << std::endl;
    
    // Test initializer list constructor
    HashMap<std::string, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
    runner.runTest("Initializer list constructor - size is 3", map.getSize() == 3);
    runner.runTest("Initializer list constructor - values correct", 
                   map.at("a") == 1 && map.at("b") == 2 && map.at("c") == 3);
    
    printMap(map, "From initializer list: ");
    
    // Test bucket operations
    runner.runTest("bucket_count() > 0", map.bucket_count() > 0);
    
    // Test hash policy
    float loadFactor = map.max_load_factor();
    runner.runTest("max_load_factor() > 0", loadFactor > 0);
    
    map.max_load_factor(0.8f);
    runner.runTest("max_load_factor(0.8)", map.max_load_factor() == 0.8f);
}

void testHashMapRehashing(TestRunner& runner) {
    std::cout << "\n=== HASHMAP REHASHING TESTS ===" << std::endl;
    
    // Test 1: Automatic rehashing when exceeding load factor
    {
        std::cout << "\n--- Test 1: Automatic Rehashing ---" << std::endl;
        HashMap<int, std::string> map;
        map.max_load_factor(0.5f);  // Very low load factor to force rehashing
        
        auto initialBucketCount = map.bucket_count();
        std::cout << "Initial bucket count: " << initialBucketCount << std::endl;
        
        // Insert elements to trigger rehashing
        for (int i = 0; i < 10; ++i) {
            map.insert(i, "value" + std::to_string(i));
            std::cout << "After inserting " << i << ": size=" << map.getSize() 
                      << ", buckets=" << map.bucket_count() 
                      << ", load_factor=" << (static_cast<float>(map.getSize()) / map.bucket_count()) << std::endl;
        }
        
        auto finalBucketCount = map.bucket_count();
        runner.runTest("Automatic rehashing - bucket count increased", finalBucketCount > initialBucketCount);
        runner.runTest("Automatic rehashing - all elements preserved", map.getSize() == 10);
        
        // Verify all elements are still accessible
        bool allElementsFound = true;
        for (int i = 0; i < 10; ++i) {
            if (!map.contains(i) || map.at(i) != "value" + std::to_string(i)) {
                allElementsFound = false;
                break;
            }
        }
        runner.runTest("Automatic rehashing - all elements accessible", allElementsFound);
        
        float currentLoadFactor = static_cast<float>(map.getSize()) / map.bucket_count();
        runner.runTest("Automatic rehashing - load factor within limit", currentLoadFactor <= map.max_load_factor());
        
        printMap(map, "After automatic rehashing: ");
    }
    
    // Test 2: Manual rehashing with rehash()
    {
        std::cout << "\n--- Test 2: Manual Rehashing ---" << std::endl;
        HashMap<std::string, int> map = {{"apple", 1}, {"banana", 2}, {"cherry", 3}, {"date", 4}};
        
        auto initialBucketCount = map.bucket_count();
        std::cout << "Before manual rehash - buckets: " << initialBucketCount << ", size: " << map.getSize() << std::endl;
        
        // Manual rehash to a larger size
        map.rehash(50);
        
        auto afterRehashBucketCount = map.bucket_count();
        std::cout << "After manual rehash - buckets: " << afterRehashBucketCount << ", size: " << map.getSize() << std::endl;
        
        runner.runTest("Manual rehashing - bucket count increased", afterRehashBucketCount >= 50);
        runner.runTest("Manual rehashing - size unchanged", map.getSize() == 4);
        
        // Verify all elements are still accessible
        runner.runTest("Manual rehashing - apple found", map.contains("apple") && map.at("apple") == 1);
        runner.runTest("Manual rehashing - banana found", map.contains("banana") && map.at("banana") == 2);
        runner.runTest("Manual rehashing - cherry found", map.contains("cherry") && map.at("cherry") == 3);
        runner.runTest("Manual rehashing - date found", map.contains("date") && map.at("date") == 4);
        
        printMap(map, "After manual rehashing: ");
    }
    
    // Test 3: Reserve functionality
    {
        std::cout << "\n--- Test 3: Reserve Functionality ---" << std::endl;
        HashMap<int, std::string> map;
        
        auto initialBucketCount = map.bucket_count();
        std::cout << "Before reserve - buckets: " << initialBucketCount << std::endl;
        
        // Reserve space for 100 elements
        map.reserve(100);
        
        auto afterReserveBucketCount = map.bucket_count();
        std::cout << "After reserve(100) - buckets: " << afterReserveBucketCount << std::endl;
        
        runner.runTest("Reserve - bucket count increased", afterReserveBucketCount > initialBucketCount);
        
        // Calculate expected minimum bucket count for 100 elements
        auto expectedMinBuckets = static_cast<size_t>(std::ceil(100.0f / map.max_load_factor()));
        runner.runTest("Reserve - sufficient buckets allocated", afterReserveBucketCount >= expectedMinBuckets);
        
        // Insert elements and verify no additional rehashing occurs
        auto bucketsBeforeInserts = map.bucket_count();
        for (int i = 0; i < 50; ++i) {
            map.insert(i, "reserved" + std::to_string(i));
        }
        auto bucketsAfterInserts = map.bucket_count();
        
        runner.runTest("Reserve - no rehashing during inserts", bucketsBeforeInserts == bucketsAfterInserts);
        runner.runTest("Reserve - all elements inserted", map.getSize() == 50);
        
        std::cout << "After inserting 50 elements - buckets: " << bucketsAfterInserts 
                  << ", load_factor: " << (static_cast<float>(map.getSize()) / map.bucket_count()) << std::endl;
    }
    
    // Test 4: Different load factor settings
    {
        std::cout << "\n--- Test 4: Different Load Factor Settings ---" << std::endl;
        
        // Test with very strict load factor
        HashMap<int, int> strictMap;
        strictMap.max_load_factor(0.25f);  // Very strict
        
        auto strictInitialBuckets = strictMap.bucket_count();
        for (int i = 0; i < 5; ++i) {
            strictMap.insert(i, i * 10);
        }
        auto strictFinalBuckets = strictMap.bucket_count();
        
        runner.runTest("Strict load factor - more rehashing", strictFinalBuckets > strictInitialBuckets);
        
        // Test with relaxed load factor
        HashMap<int, int> relaxedMap;
        relaxedMap.max_load_factor(2.0f);  // Very relaxed
        
        auto relaxedInitialBuckets = relaxedMap.bucket_count();
        for (int i = 0; i < 10; ++i) {  // Reduced number to avoid rehashing
            relaxedMap.insert(i, i * 10);
        }
        auto relaxedFinalBuckets = relaxedMap.bucket_count();
        
        runner.runTest("Relaxed load factor - less rehashing", relaxedFinalBuckets == relaxedInitialBuckets);
        
        std::cout << "Strict map (load_factor=0.25): " << strictFinalBuckets << " buckets for 5 elements" << std::endl;
        std::cout << "Relaxed map (load_factor=2.0): " << relaxedFinalBuckets << " buckets for 10 elements" << std::endl;
    }
    
    // Test 5: Prime number bucket sizes
    {
        std::cout << "\n--- Test 5: Prime Number Bucket Sizes ---" << std::endl;
        HashMap<int, int> map;
        
        // Function to check if number is prime (simple version)
        auto isPrime = [](size_t n) -> bool {
            if (n < 2) return false;
            for (size_t i = 2; i * i <= n; ++i) {
                if (n % i == 0) return false;
            }
            return true;
        };
        
        // Check initial bucket count is prime
        auto initialBuckets = map.bucket_count();
        runner.runTest("Initial bucket count is prime", isPrime(initialBuckets));
        
        // Force rehashing and check bucket count is still prime
        map.rehash(100);
        auto afterRehashBuckets = map.bucket_count();
        runner.runTest("After rehash bucket count is prime", isPrime(afterRehashBuckets));
        
        std::cout << "Initial buckets: " << initialBuckets << " (prime: " << isPrime(initialBuckets) << ")" << std::endl;
        std::cout << "After rehash: " << afterRehashBuckets << " (prime: " << isPrime(afterRehashBuckets) << ")" << std::endl;
    }
}



int main() {
    TestRunner runner;
    
    std::cout << "=== RUNNING COMPREHENSIVE TESTS ===" << std::endl;
    
    testListBasicOperations(runner);
    testListAdvancedOperations(runner);
    testListConstructors(runner);
    
    testHashMapBasicOperations(runner);
    testHashMapAdvancedOperations(runner);
    testHashMapConstructors(runner);
    testHashMapRehashing(runner);
    
    runner.printSummary();
    
    return 0;
}
