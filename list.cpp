#include <iostream>
#include <list>
#include <ranges>
#include <chrono>
#include <cstddef>
#include <set>
#include <map>

#define T_Convertible     template <typename _T> \
requires std::is_convertible_v<_T, T>


template <typename T, size_t Capacity>
class NodeAllocator {
    T* memory_pool;
    T** empty_spots;

    size_t offset = 0;
    size_t capacity;

    size_t empty_offset = 0;

    void move(auto&& other) noexcept {
        memory_pool  = other.memory_pool;
        empty_spots  = other.empty_spots;
        offset       = other.offset;
        capacity     = other.capacity;
        empty_offset = other.empty_offset;

        other.memory_pool  = nullptr;
        other.empty_spots  = nullptr;
        other.offset       = 0;
        other.capacity     = 0;
        other.empty_offset = 0;
    }
public:
    using value_type = T;

    template <typename U>
    struct rebind {
        using other = NodeAllocator<U, Capacity>;
    };

    NodeAllocator() : capacity(Capacity) {
        memory_pool = static_cast<T*>(::operator new(sizeof(T) * capacity));
        empty_spots = static_cast<T**>(::operator new(sizeof(T*) * capacity));
    }

    NodeAllocator(size_t capacity) : capacity(capacity) {
        memory_pool = static_cast<T*>(::operator new(sizeof(T) * capacity));
        empty_spots = static_cast<T**>(::operator new(sizeof(T*) * capacity));    
    }

    NodeAllocator(const NodeAllocator&) = delete;
    NodeAllocator& operator=(const NodeAllocator&) = delete; 

    NodeAllocator(NodeAllocator&& other) noexcept {
        move(other);
    }

    NodeAllocator& operator=(NodeAllocator&& other) noexcept {
        if (this != &other) {
            move(other);
        }
        return *this;
    }

    ~NodeAllocator() noexcept {
        clear();
    }

    T* allocate(size_t) noexcept {
        if (empty_offset >= 1) {
            --empty_offset;
            empty_spots[empty_offset]->~T();
            return empty_spots[empty_offset];
        } else {

            T* ptr = memory_pool + offset;
            offset += 1;
            return ptr;
        }
    }

    void deallocate(T* ptr, size_t) noexcept {
        empty_spots[empty_offset++] = ptr;
    }

    size_t get_capacity() const noexcept {
        return capacity;
    }

    const T* const get_pointer() const noexcept {
        return memory_pool;
    }

    void clear() noexcept {
        if (memory_pool) {
            for (int i = 0; i < offset; ++i) {
                (memory_pool + i)->~T();
            }
            offset = 0;
            empty_offset = 0;

            ::operator delete(empty_spots);
            ::operator delete(memory_pool);
            memory_pool = nullptr;
            empty_spots = nullptr;
        }            
    }
};

template <typename T, size_t Capacity = 24>
class List {
    class Iterator;
    struct Node {
        T element;
        Node* next = nullptr;

        template <typename _T>
        requires std::is_convertible_v<_T, T>
        Node(_T&& element) : element(std::forward<_T>(element)) {}
    };

    NodeAllocator<Node, Capacity> allocator;

    Node* head = nullptr;
    Node* tail = nullptr;

    size_t length = 0;

    template <typename Iterator>
    constexpr void _insert_range(size_t n, Iterator from, Node* begin, Node* end) {
        length += n;

        if (from.current == head) {
            Node* rest = head;
            head = begin;
            end->next = rest;
        } else if (from.prev == tail || !from.current) {
            tail->next = begin;
            tail = end;
        } else {            
            from.prev->next = begin;
            end->next = from.current;            
        }

        if (!tail) {
            tail = end;
        }
    }

    constexpr void _resize(size_t new_capacity) noexcept(
        std::is_nothrow_move_assignable_v<T>)
    {
        NodeAllocator<Node, Capacity> new_allocator(new_capacity);

        if (head) {
            Node* new_list = new (new_allocator.allocate(1)) Node(std::move(head->element));
            Node* current = head->next;

            Node* new_head = new_list;

            while (current != nullptr) {
                Node* next = current->next;
                new_list->next = new (new_allocator.allocate(1)) Node(std::move(current->element));
                current = next;
                new_list = new_list->next;
            }

            tail = new_list;
            head = new_head;   
        }
        allocator.clear();
        allocator = std::move(new_allocator);
    }

    constexpr Iterator _revalidate_iterator(ptrdiff_t distance) noexcept {
        if (distance == 0) {
            return begin();
        } else if (distance == length) {
            return end();
        } else {
            return Iterator(head + distance - 1, head + distance);
        }
    }

    constexpr void _confirm_avail_mem(size_t n) noexcept {
        if (n + length > allocator.get_capacity()) {
            _resize(allocator.get_capacity() * 1.5 + n);          
        } 
    }

    constexpr void _confirm_avail_mem(size_t n, Iterator& from) noexcept {
        if (n + length > allocator.get_capacity()) {
            ptrdiff_t dist = from.current - head;
            _resize(allocator.get_capacity() * 1.5 + n);
            from = _revalidate_iterator(dist);              
        } 
    }

    template <char C>
    constexpr const List& _compare(const List& other) const noexcept {
        Node* _this = head;
        Node* _other = other.head;

        while (_this && _other) {
            if constexpr (C == '>')
            {
                if (_this->element > _other->element)
                    return *this;
                else if (_this->element < _other->element)
                    return other;       
            }
            else if constexpr (C == '=')
            {
                if (_this->element != _other->element)
                    return other;
                return *this;
            } 
            else if constexpr (C == '<')
            {
                if (_this->element < _other->element) 
                    return *this;
                else if (_this->element > _other->element)
                    return other; 
            }

            _this  = _this->next;
            _other = _other->next;
        }

        if constexpr (C == '>') 
        {
            if (!_other) 
                return *this;
            return other;
        } 
        else if constexpr (C == '<') 
        {
            if (!_this) 
                return *this;
            return other;
        }

        return *this;
    }

public:
    class Iterator {
    friend class List<T, Capacity>;
        mutable Node* prev;
        mutable Node* current;

    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::input_iterator_tag;

        explicit Iterator(Node* prev, Node* current) : prev(prev), current(current) {}
        explicit Iterator(Node* head) : current(head), prev(nullptr) {}

        Iterator& operator = (const Iterator& other) {
            prev = other.prev;
            current = other.current;

            return *this;
        }

        decltype(auto) operator ++ (this auto&& self) {
            self.prev = self.current;
            self.current = self.current->next;
            return std::forward<decltype(self)>(self);
        }

        Iterator operator ++ (int) const {
            Iterator temp = *this;
            prev = current;
            current = current->next;

            return temp;
        }

        Iterator operator + (int increment) const {
            Node* it = current;

            for (int i = 0; i < increment; ++i) {
                prev = it;
                it = it->next;
            }

            return Iterator(prev, it);
        }

        decltype(auto) operator * (this auto&& self) {
            return std::forward<decltype(self)>(self).current->element;
        }

        bool operator != (const Iterator& end) const {
            return current != end.current;
        }

        bool operator != (std::nullptr_t) const {
            return current != nullptr;
        }

        bool operator == (std::nullptr_t) const {
            return current == nullptr;
        }

        bool operator == (const Iterator& other) const {
            return current == other.current;
        }
    };

    explicit List(size_t capacity = Capacity) : allocator(capacity) {}

    template <typename InputIterator>
    List(InputIterator first, InputIterator last) : allocator(Capacity > std::distance(first, last) ? Capacity : Capacity + std::distance(first, last)) {
        tail = insert_range(begin(), first, last).prev;
    }

    template <typename... Args>
    explicit List(Args&&... args) : allocator(Capacity > sizeof...(args) ? Capacity : (sizeof...(args) + Capacity)) {
        tail = insert_range(begin(), std::forward<Args>(args)...).prev;
    }

    List(const List& other) : allocator(other.allocator.get_capacity()) {
        _insert_range(other.length, begin(), other.head, other.tail);
    }

    List& operator = (const List& other) noexcept {
        if (this != &other) {
            clear();
            _confirm_avail_mem(other.length + 1);
            _insert_range(other.length, begin(), other.head, other.tail);
        }

        return *this;
    }

    List(List&& other) noexcept {
        head = other.head;
        tail = other.tail;
        length = other.length;
        allocator = std::move(other.allocator);

        other.head = nullptr;   
        other.tail = nullptr;   
        other.length = 0;
    }

    List& operator = (List&& other) noexcept {
        if (this != &other) {
            head = other.head;
            tail = other.tail;
            length = other.length;
            allocator = std::move(other.allocator);
    
            other.head = nullptr;   
            other.tail = nullptr;   
            other.length = 0;
        }

        return *this;
    }

    ~List() noexcept {
        clear();
    }

    constexpr void reserve(size_t elements) noexcept {
        if (length < elements)
            _resize(elements);
    }

    T_Convertible constexpr Iterator insert_front(_T&& element) noexcept {
        _confirm_avail_mem(length + 1);

        Node* node = new (allocator.allocate(1)) Node(std::forward<_T>(element));
        
        if (!head) {
            head = tail = node;
        } else {
            node->next = head;
            head = node;            
        }

        length++;
        return Iterator(head, head->next);
    }

    T_Convertible constexpr Iterator insert_back(_T&& element) noexcept {
        _confirm_avail_mem(length + 1);

        Node* node = new (allocator.allocate(1)) Node(std::forward<_T>(element));

        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }

        length++;
        return end();
    }

    T_Convertible constexpr Iterator insert(Iterator at, _T&& element) noexcept {
        _confirm_avail_mem(length + 1, at);

        if (at.current == head) _UNLIKELY {
            return insert_front(std::forward<_T>(element));
        }

        if (at.current == nullptr) _UNLIKELY {
            return insert_back(std::forward<_T>(element));
        }

        Node* node = new (allocator.allocate(1)) Node(std::forward<_T>(element));

        at.prev->next = node;
        node->next = at.current;

        length++;
        return Iterator(node, node->next);
    }

    template <typename Range>
    requires std::is_convertible_v<
        typename std::iterator_traits<Range>::iterator_category, std::input_iterator_tag
    >
    constexpr Iterator insert_range(Iterator from, Range begin, Range end) noexcept {
        _confirm_avail_mem(std::distance(begin, end), from);

        Node* temp = new (allocator.allocate(1)) Node(*begin);
        Node* tempPtr = temp;

        for (Range it = std::next(begin); it != end; ++it) {
            Node* node = new (allocator.allocate(1)) Node(*it);
            temp->next = node;
            temp = temp->next;
        }
        _insert_range(std::distance(begin, end), from, tempPtr, temp);
        return Iterator(temp, temp->next);
    }

    template <typename Range>
    requires std::is_convertible_v<
        typename std::iterator_traits<Range>::iterator_category, std::input_iterator_tag
    >
    constexpr Iterator insert_range(Range begin, Range end) noexcept {
        return insert_range(begin(), begin, end);
    }

    template <typename Iteratable>
    constexpr Iterator insert_range(Iterator from, Iteratable&& container) noexcept
    requires requires {
        { std::begin(container) };
        { std::end(container) };
    }
    {
        return insert_range(from,
            std::begin(std::forward<Iteratable>(container)),
            std::end(std::forward<Iteratable>(container))
        );
    }

    template <typename... Ts>
    requires std::conjunction_v<std::is_convertible<T, Ts>...>
    constexpr Iterator insert_range(Iterator from, Ts&&... elements) noexcept(
        std::conjunction_v<std::is_nothrow_convertible<T, Ts>...>)
    {
        return insert_range(from, std::array<T, sizeof...(elements)>{std::forward<Ts>(elements)...});
    }

    template <typename Generator>
    requires std::is_convertible_v<std::invoke_result_t<Generator>, T>
    constexpr Iterator insert_range(Iterator from, size_t n, Generator&& gen) noexcept(
        std::is_nothrow_invocable_r_v<std::invoke_result_t<Generator>, Generator, T>)
    {
        _confirm_avail_mem(n, from);

        Node* temp = new (allocator.allocate(1)) Node(gen());
        Node* temp_ptr = temp;

        for (int i = 0; i < n - 1; ++i) {
            Node* node = new (allocator.allocate(1)) Node(gen());
            temp->next = node;
            temp = temp->next;
        }
        _insert_range(n, from, temp_ptr, temp);
        return Iterator(temp, temp->next);
    }

    Iterator pop_front() noexcept {
        Node* temp = head;

        if (!head->next) {
            head = tail = nullptr;
        } else {
            head = head->next;
        }

        allocator.deallocate(temp, 1);

        length--;

        return begin();
    }

    Iterator pop_back() noexcept {
        if (!head) return Iterator(nullptr);

        if (head == tail) {
            allocator.deallocate(head, 1);

            head = tail = nullptr;
        } else {
            Node* current = head;

            while (current->next != tail) {
                current = current->next;
            }

            allocator.deallocate(tail, 1);
            
            tail = current;
            tail->next = nullptr;  
        }
        length--;
        return Iterator(tail);
    }

    Iterator erase(Iterator at) noexcept {
        if (at.current == head) _UNLIKELY {
            return pop_front();
        }

        Node* temp = at.current;

        at.prev->next = at.current->next;

        allocator.deallocate(temp, 1);
        length--;
        return Iterator(at.prev, at.prev->next);
    }

    Iterator erase_range(Iterator from, Iterator to) noexcept {
        Node* current = from.current;

        while (current != to.current) {
            Node* next = current->next;
            allocator.deallocate(current, 1);
            current = next;
            length--;
        }

        if (from.current == head) {
            head = current;
        } else {
            from.prev->next = current;
        }

        return Iterator(current);
    }

    Iterator erase_range(Iterator from) noexcept {
        return erase_range(from, end());
    }

    T_Convertible constexpr void assign(size_t n, _T&& val) noexcept {
        clear();
        _confirm_avail_mem(n + 1);

        Node* node = new (allocator.allocate(1)) Node(std::forward<_T>(val));

        head = tail = node;

        Node* current = head;

        for (int i = 0; i < n - 1; ++i) {
            current->next = new (allocator.allocate(1)) Node(std::forward<_T>(val));
            current = current->next;
        }

        tail = current;
        length = n;
    }

    template <typename Range>
    requires std::is_convertible_v<
        typename std::iterator_traits<Range>::iterator_category, std::input_iterator_tag
    >
    constexpr void assign(Range first, Range last) noexcept {
        clear();
        _confirm_avail_mem(std::distance(first, last) + 1);

        tail = insert_range(begin(), first, last).prev;
    }

    T_Convertible constexpr void assign(std::initializer_list<_T> ini_list) {
        assign(ini_list.begin(), ini_list.end());
    }

    template <typename Generator>
    requires std::is_convertible_v<std::invoke_result_t<Generator>, T>
    constexpr void assign(size_t n, Generator&& gen) noexcept(
        std::is_nothrow_invocable_r_v<std::invoke_result_t<Generator>, Generator, T>)
    {
        clear();
        _confirm_avail_mem(n + 1);

        Node* node = new (allocator.allocate(1)) Node(gen());

        head = tail = node;

        Node* current = head;

        for (int i = 0; i < n - 1; ++i) {
            current->next = new (allocator.allocate(1)) Node(gen());
            current = current->next;
        }

        tail = current;
        length = n;
    }

    void clear() noexcept {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            allocator.deallocate(temp, 1);
        }
        length = 0;
    }

    constexpr void swap(const List& other) noexcept
    {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(length, other.length);
        std::swap(allocator, other.allocator);
    }

    void unique() noexcept 
    requires requires(T left, T right) {
        { left == right } -> std::convertible_to<bool>;
    }
    {
        for (auto it = begin() + 1; it != end() && it != nullptr;) {
            if (it.prev->element == it.current->element) {
                it = erase(it);
            } else {
                ++it;
            }
        }
    }

    void unique_all() noexcept 
    requires requires(T left, T right) {
        { left < right } -> std::convertible_to<bool>;
    }
    {
        std::set<T> visited;

        for (auto it = begin(); it != end() && it != nullptr;) {
            if (visited.contains(*it)) {
                it = erase(it);
            } else {
                visited.insert(*it);
                ++it;
            }
        }
    }

    _NODISCARD constexpr Iterator find(const T& to_find, Iterator from, Iterator to) const noexcept {
        for (auto it = from; it != to && it != nullptr; ++it) {
            if (it.current->element == to_find) {
                return Iterator(it.current);
            }
        }
        return to;
    }

    constexpr Iterator find(const T& to_find) const noexcept {
        return find(to_find, begin(), end());
    }

    template <typename Predicate>
    requires std::is_convertible_v<std::invoke_result_t<Predicate, T>, bool>
    _NODISCARD constexpr Iterator find_if(Iterator from, Iterator to, Predicate&& predicate) const noexcept(
        std::is_nothrow_invocable_r_v<bool, Predicate, T>) 
    {
        for (auto it = from; it != to && it != nullptr; ++it) {
            if (predicate(*it)) {
                return Iterator(it.current);
            }
        }
        return to;
    }

    template <typename Predicate>
    requires std::is_convertible_v<std::invoke_result_t<Predicate, T>, bool>
    _NODISCARD constexpr Iterator find_if(Predicate&& predicate) const noexcept(
        std::is_nothrow_invocable_r_v<bool, Predicate, T>) 
    {
        return find_if(begin(), end(), std::forward<Predicate>(predicate));
    }

    constexpr void remove(const T& to_remove, Iterator from, Iterator to) {
        for (auto it = from; it != to && it != nullptr;) {
            if (to_remove == *it) {
                it = erase(it);
            } else {
                ++it;
            }
        }
    }

    constexpr void remove(const T& to_remove) noexcept {
        remove(to_remove, begin(), end());
    }

    template <typename Predicate>
    requires std::is_convertible_v<std::invoke_result_t<Predicate, T>, bool>
    constexpr void remove_if(Iterator from, Iterator to, Predicate&& predicate) noexcept(
        std::is_nothrow_invocable_r_v<bool, Predicate, T>) 
    {
        for (auto it = from; it != to && it != nullptr;) {
            if (predicate(*it)) {
                it = erase(it);
            } else {
                ++it;
            }
        }
    }

    template <typename Predicate>
    requires std::is_convertible_v<std::invoke_result_t<Predicate, T>, bool>
    constexpr void remove_if(Predicate&& predicate) noexcept(
        std::is_nothrow_invocable_r_v<bool, Predicate, T>) 
    {
        remove_if(begin(), end(), std::forward<Predicate>(predicate));
    }

    template <typename Predicate>
    requires std::is_invocable_r_v<void, Predicate, T>
    constexpr void for_each(Predicate&& predicate) noexcept(
        std::is_nothrow_invocable_r_v<void, Predicate, T>)
    {
        for (auto it = begin(); it != nullptr; ++it) {
            predicate(*it);
        }
    }

    template <typename Predicate>
    requires std::is_convertible_v<std::invoke_result_t<Predicate, T>, bool>
    _NODISCARD constexpr List filter(Predicate&& predicate) noexcept(
        std::is_nothrow_invocable_r_v<bool, Predicate, T>) 
    {
        List temp(allocator.get_capacity());

        Iterator it = find_if(std::forward<Predicate>(predicate));

        if (it != end()) {
            Node* node = new (temp.allocator.allocate(1)) Node(it.current->element);
            Node* begin = node;

            for (; it != nullptr; ++it) {
                if (predicate(*it)) {
                    node->next = new (temp.allocator.allocate(1)) Node(*it);
                    node = node->next;
                }
            }
            temp._insert_range(node - begin, temp.begin(), begin, node);
        }
        return temp;
    }

    _NODISCARD constexpr List sublist(Iterator from, Iterator to) const noexcept {
        return {from, to};
    }

    constexpr void merge(List& other) noexcept(
        std::is_nothrow_move_assignable_v<T>)
    {
        _confirm_avail_mem(other.length + length + 1);

        Node* other_head = other.begin().current;

        Node* tail_ptr = tail;

        while (other_head != nullptr) {
            tail_ptr->next = new (allocator.allocate(1)) Node(std::move(other_head->element));
            tail_ptr = tail_ptr->next;
            Node* temp = other_head;
            other_head = other_head->next;
        }
        other.allocator.clear();
        other.head = nullptr;
        other.tail = nullptr;
        other.length = 0;
    }

    template <typename SortMethod = std::less<T>> // also make merge sort sometime
    constexpr void sort(SortMethod&& sort_method = SortMethod{}) noexcept(
        std::is_nothrow_invocable_r_v<bool, SortMethod, T, T>)  
    requires requires(T left, T right) {
        { left < right } -> std::convertible_to<bool>;
    }
    {
        std::set<T, SortMethod> sorted;

        Node* current = head;

        while (current) {
            sorted.insert(current->element);
            current = current->next;
        }
        assign(sorted.begin(), sorted.end());
    }

    constexpr bool operator == (const List& other) const noexcept 
    requires requires(T left, T right) {
        { left == right } -> std::convertible_to<bool>;
    }
    {
        if (length != other.length) 
            return false;

        return &_compare<'='>(other) == this;
    }

    constexpr bool operator != (const List& other) const noexcept 
    requires requires(T left, T right) {
        { left != right } -> std::convertible_to<bool>;
    }
    {
        return &_compare<'='>(other) != this;
    }

    constexpr bool operator > (const List& other) const noexcept
    requires requires(T left, T right) {
        { left > right } -> std::convertible_to<bool>;
    }
    {
        return &_compare<'>'>(other) == this;
    }

    constexpr bool operator < (const List& other) const noexcept
    requires requires(T left, T right) {
        { left < right } -> std::convertible_to<bool>;
    }
    {
        return &_compare<'<'>(other) == this;
    }

    constexpr List operator + (const List& other) noexcept {
        List temp(length + other.length + 1);

        temp._insert_range(length, temp.begin(), head, tail);
        temp._insert_range(other.length, temp.end(), other.head, other.tail);
    }

    constexpr List& operator += (const List& other) noexcept {
        _confirm_avail_mem(length + other.length + 1);
        _insert_range(other.length, end(), other.head, other.tail);
    }

    constexpr size_t size() const {
        return length;
    }

    constexpr bool empty() const {
        return length;
    }

    constexpr size_t capacity() {
        return allocator.get_capacity();
    }

    constexpr Iterator begin() const {
        return Iterator(head);
    }

    constexpr Iterator end() const {
        return Iterator(tail, nullptr);
    }

    constexpr decltype(auto) front(this auto&& self) noexcept {
        return std::forward<decltype(self)>(self)->head->element;
    }

    constexpr decltype(auto) back(this auto&& self) noexcept {
        return std::forward<decltype(self)>(self)->tail->element;
    }

    constexpr const auto& get_allocator() const {
        return allocator;
    }
};




template <size_t MultiplyFactor>
struct NumberGenerator {
    int x = 1;

    int operator ()() {
        return x *= MultiplyFactor;
    }
};

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    List<int, 10> list;
    int i = 0;


    auto print = [](auto&&v){
        std::cout << v << std::endl;
    };

  //  list.for_each([](auto&&val){ std::cout << val << std::endl;});

  //  list.insert_back(3);
 //   list.insert_back(3);
  //  list.assign(5, 5);

    list.insert_range(list.begin(), 11, NumberGenerator<2>{});
    list.erase(list.begin() + 5);

    list.insert_back(65);
    list.insert_back(66);
    list.erase_range(list.begin(), list.begin() + 3);
    list.assign(5, NumberGenerator<2>{});
    list.insert_range(list.begin() + 2, 5,6,7);
    list.insert_front(5);
    list.insert_back(10);
    list.erase(list.begin() + 1);
    list.remove_if([](auto&&v){return false;});
    auto it = list.find_if([](auto&&v){return false;});
    list.unique_all();

    auto n = list.filter([](auto&&v){
        return v % 2 == 0;
    });

    n.for_each(print);



    std::cout << "-" << std::endl << std::endl;
    list.sort();
    list.for_each(print);

    auto f = n;
    std::cout << "-" << std::endl << std::endl;
   // f.for_each(print);

    if (n == f) {
        std::cout << "n==list\n";
    }

    if (list > n) {
        std::cout << "LIST > N\n";
    }

    if (n > list) {
        std::cout << "N > LIST\n";
    }

    if (n < list) {
        std::cout << "N < LIST\n";
    }

    if (list < n) {
        std::cout << "LIST < N\n";
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
}