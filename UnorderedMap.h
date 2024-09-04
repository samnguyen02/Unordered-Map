#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode *_head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept : _map(map), _ptr(ptr) {}

    public:
        basic_iterator(): _map(nullptr), _ptr(nullptr) {}

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;

        reference operator*() const {
            return (*_ptr).val;
        }

        pointer operator->() const {
            return &((*_ptr).val);
        }

        basic_iterator &operator++() {
            if (_ptr->next != nullptr) {
                _ptr = (*_ptr).next;
                return *this;
            }
            
            size_t bucket = _map->_bucket(_ptr->val);
            while (++bucket < _map->_bucket_count) {
                _ptr = _map->_buckets[bucket];
                if (_ptr != nullptr) {
                    return *this;
                }
            }
            _ptr = nullptr;
            
            return *this;
        }

        basic_iterator operator++(int) {
            basic_iterator temp = *this;
            ++*this;
            return temp;
        }
        
        bool operator==(const basic_iterator &other) const noexcept {
            return _ptr == other._ptr;
        }
        bool operator!=(const basic_iterator &other) const noexcept {
            return _ptr != other._ptr;
        }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            HashNode * _node;

            explicit local_iterator( HashNode * node ) noexcept: _node(node) {}

        public:
            local_iterator(): _node(nullptr) {}

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;

            reference operator*() const {
                return (*_node).val;
            }

            pointer operator->() const {
                return &((*_node).val);
            }
            
            local_iterator & operator++() {
                _node = _node->next;
            }

            local_iterator operator++(int) {
                local_iterator temp = local_iterator(_node);
                _node = (*_node).next;
                return temp;
            }

            bool operator==(const local_iterator &other) const noexcept {
                return _node == other._node;
            }

            bool operator!=(const local_iterator &other) const noexcept {
                return _node != other._node;
            }
    };

private:

    size_type _bucket(size_t code) const {
        return _range_hash(code, _bucket_count);
    }

    size_type _bucket(const Key & key) const {
        return _bucket(_hash(key));
    }

    size_type _bucket(const value_type & val) const {
        return _bucket(_hash(val.first));
    }

    HashNode*& _find(size_type code, size_type bucket, const Key & key) {
        HashNode** curr = &_buckets[bucket];
        while (*curr) {
            if (_hash((*curr)->val.first) == code && _equal(key, (*curr)->val.first)) {
                return *curr;
            }
            curr = &(*curr)->next;
        }
        return *curr;
    }

    HashNode*& _find(const Key & key) {
        return _find(_hash(key), _bucket(key), key);
    }

    HashNode * _insert_into_bucket(size_type bucket, value_type && value) {
        HashNode*& bucketll = _buckets[bucket];
        HashNode* node = new HashNode(std::move(value), bucketll);
        if (_head == nullptr || bucket <= _bucket(_head->val)) {
            _head = node;
        }
        _buckets[bucket] = node;
        _size++;
        return node;
    }

    void _move_content(UnorderedMap & src, UnorderedMap & dst) { /* TODO */ }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { },
                const key_equal & equal = key_equal { })
                : _hash(hash), _equal(equal), _size(0), _head(nullptr) {
                    _bucket_count = next_greater_prime(bucket_count);
                    _buckets = new HashNode*[_bucket_count]();
                }

    ~UnorderedMap() {
        clear();
    }

    UnorderedMap(const UnorderedMap & other): _head(nullptr), _size(0){
        _bucket_count = other._bucket_count;
        _hash = other._hash;
        _equal = other._equal;
        _buckets = new HashNode*[_bucket_count]();
        for (const_iterator i = other.cbegin(); i != other.cend(); i++) {
            insert((*(i._ptr)).val);
        }
    }

    UnorderedMap(UnorderedMap && other) {
        _bucket_count = other._bucket_count;

        _hash = other._hash;
        other._hash = Hash{};

        _equal = other._equal;
        other._equal = key_equal{};

        _size = other._size;
        other._size = 0;

        _head = other._head;
        other._head = nullptr;

        _buckets = other._buckets;
        other._buckets = new HashNode*[other._bucket_count]();
    }

    UnorderedMap & operator=(const UnorderedMap & other) {
        if (this == &other) {
            return *this;
        }
        clear();
        delete _buckets;

        _bucket_count = other._bucket_count;
        _hash = other._hash;
        _equal = other._equal;
        _size = 0;
        _head = nullptr;
        _buckets = new HashNode*[_bucket_count]();
        for (const_iterator i = other.cbegin(); i != other.cend(); i++) {
            insert((*(i._ptr)).val);
        }
    }

    UnorderedMap & operator=(UnorderedMap && other) {
        if (this == &other) {
            return *this;
        }
        clear();
        delete _buckets;

        _bucket_count = other._bucket_count;

        _hash = other._hash;
        other._hash = Hash{};

        _equal = other._equal;
        other._equal = key_equal{};

        _size = other._size;
        other._size = 0;

        _head = other._head;
        other._head = nullptr;

        _buckets = other._buckets;
        other._buckets = new HashNode*[other._bucket_count]();
    }

    void clear() noexcept {
        while (_size != 0) {
            erase(begin());
        }
    }

    size_type size() const noexcept {
        return _size;
    }

    bool empty() const noexcept {
        return _size == 0;
    }

    size_type bucket_count() const noexcept {
        return _bucket_count;
    }

    iterator begin() {
        return iterator(this, _head);
    }
    
    iterator end() {
        return iterator(this, nullptr);
    }

    const_iterator cbegin() const {
        return const_iterator(this, _head);
    }

    const_iterator cend() const {
        return const_iterator(this, nullptr);
    }

    local_iterator begin(size_type n) {
        return local_iterator(_buckets[n]);
    }

    local_iterator end(size_type n) {
        return local_iterator(nullptr);
    }

    size_type bucket_size(size_type n) {
        size_type count = 0;
        local_iterator it = local_iterator(_buckets[n]);
        while (it != local_iterator(nullptr)) {
            it++;
            count++;
        }
        return count;
    }

    float load_factor() const {
        float elements = static_cast<float>(size());
        return elements / bucket_count();
    }

    size_type bucket(const Key & key) const {
        return _bucket(key); 
    }

    std::pair<iterator, bool> insert(value_type && value) {
        HashNode* search = _find(_hash(value.first), _bucket(value), value.first);

        if (search != nullptr) {
            return std::make_pair(
                iterator(this, search),
                false
            );
        }
        else {
            search = _insert_into_bucket(_bucket(value.first), std::move(value));
            return std::make_pair(
                iterator(this, search),
                true
            );
        }
    }

    std::pair<iterator, bool> insert(const value_type & value) {
        HashNode* search = _find(_hash(value.first), _bucket(value), value.first);

        if (search != nullptr) {
            return std::make_pair(
                iterator(this, search),
                false
            );
        }
        else {
            value_type temp = value;
            search = _insert_into_bucket(_bucket(temp.first), std::move(temp));
            return std::make_pair(
                iterator(this, search),
                true
            );
        }
    }

    iterator find(const Key & key) {
        return iterator(this, _find(key));
    }

    T& operator[](const Key & key) {
        HashNode* node = _find(key);
        if (node != nullptr) {
            return (*node).val.second;
        }
        else {
            auto ret = std::make_pair(key, mapped_type{});
            return insert(ret).first->second;
        }
    }

    iterator erase(iterator pos) {
        HashNode*& node = _find(pos._ptr->val.first);
        if (node == _head) {
            iterator it = iterator(this, node);
            _head = (++it)._ptr;
        }

        HashNode* del_node = node;
        iterator del_it = iterator(this, del_node);
        del_it++;

        node = (*node).next;
        delete del_node;
        
        _size--;
        return del_it;
    }

    size_type erase(const Key & key) {
        HashNode*& node = _find(key);

        if (node == nullptr) {
            return 0;
        }

        if (node == _head) {
            iterator it = iterator(this, node);
            it++;
            _head = it._ptr;
        }

        HashNode* del_node = node;
        node = node->next;
        delete del_node;
        _size--;
        return 1;
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}