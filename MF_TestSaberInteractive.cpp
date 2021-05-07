#include <iostream>
#include <vector>
#include <type_traits>

/**
 * -------------------------------------------------------------------------------------------------------------------------
 * #1
 */
template <typename T>
constexpr bool is_char_short_int_long_longlong = 
    std::is_same_v<T, char>  || 
    std::is_same_v<T, short> ||
    std::is_same_v<T, int>   ||
    std::is_same_v<T, long>  ||
    std::is_same_v<T, long long>;

template <typename T, class=std::enable_if_t<!is_char_short_int_long_longlong<T>>>
void PrintBinaryInt(T N) = delete;    

template <typename T>  
void PrintBinaryInt(T N) {
    if (N < 0) {
        N -= 1;
    }
    const size_t byte_size = 8;
    for(int i = sizeof(T) * byte_size - 1; i >= 0; --i) {
        std::cout << (N >> i & 1);
    }
    std::cout << std::endl;
}
/**
 * -------------------------------------------------------------------------------------------------------------------------
 * #2
 */
void RemoveDups(char* pStr) {
    if (!pStr || !*pStr) return;

    auto pNewStr = pStr;
    while (*pStr) {
        if (*pStr == *pNewStr) {
            ++pStr;
        }
        else {
            *++pNewStr = *pStr;
        }
    }
    *++pNewStr = '\0';
}
/**
 * -------------------------------------------------------------------------------------------------------------------------
 * #3
 */
struct ListNode {
    ListNode* prev;
    ListNode* next;
    ListNode* random;
    std::string data;
};

class List {
public:
    void Serialize(FILE* file);
    void Deserialize(FILE* file);

private:
    ListNode* head;
    ListNode* tail;
    int count;
};

void List::Serialize(FILE* file) {
    if (!file || !head || !tail || count <= 0) return;

    fprintf(file, "%i\n", count);

    int offset = 0;
    for (auto node = head; node != nullptr; node = node->next) {
        auto num = reinterpret_cast<int*>(&node->prev);
        *num = offset++;
        fprintf(file, "%s\n", node->data.c_str());
    }

    for (auto node = head; node != nullptr; node = node->next) {
        if (node->random) {
            auto num = reinterpret_cast<int*>(&node->random->prev);
            offset = *num;
            fprintf(file, "%i\n", offset);
        }
        else {
            fprintf(file, "%i\n", -1);
        }
    }

    ListNode* prev(nullptr);
    for (auto node = head; node != nullptr; node = node->next) {
        node->prev = prev;
        prev = node;
    }
}

void List::Deserialize(FILE* file)
{
    count = 0;
    if (!file) return;

    fscanf(file, "%i", &count);
    if (count <= 0) return;

    std::vector<ListNode*> vListNodes(count);
    char data[100];

    {// head & tail 
        fscanf(file, "%s", data);
        tail = new ListNode;
        vListNodes[0] = tail;

        head = tail;
        tail->prev = nullptr;
        tail->next = nullptr;
        tail->data = data;
    }

    for (int i = 1; i < count; ++i) {
        fscanf(file, "%s", data);

        tail = new ListNode;
        vListNodes[i] = tail;
        
        tail->prev = vListNodes[i - 1];
        tail->prev->next = tail;
        tail->next = nullptr;
        tail->data = data;
    }

    for (const auto& node : vListNodes) {
        int i;
        fscanf(file, "%i", &i);
        if (i == -1) {
            node->random = nullptr;
        }
        else {
            node->random = vListNodes[i];
        }
    }
}
