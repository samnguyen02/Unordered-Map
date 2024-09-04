# Priority Queue and Unordered map Report

Reports for Programming Assignment 5 and 6 (Priority Queue and Unordered Map) have been combined into this one report. There will be one question on Priority Queues and one question on Unordered Maps. 

You will answer orally. We are expecting responses to be precise and concise. 1-3 sentences about each is sufficient. We may ask clarifying questions. Be prepared. (See the list of questions below.) Since we have many reports to complete in a single lab period, the report will be limited to two minutes.

The oral reports will be scored as "exemplary" (5 points), "mostly sufficient" or "insufficient" (0 points). The point value for "mostly sufficient" will depend on the difficulty of the question. The points will be awarded in the programming assignment category.

Priority Queue Questions:
=========================

Question 1
----------
Explain the Process of `Upheap` and `Downheap` with respect to the `push()` and `pop()` functions. How do these functions help keep the internal heap perfectly balanced? 

If you have 1000 elements (numbered 0...999) that you want to insert into a heap, what permutation of these elements will insert into the heap in the shortest amount of time? What permutations will insert in the most amount of time? Explain your reasoning. 

Question 2
----------
In your priority queue implementation, you used a `std::vector` as your internal data structure. Why is this internal data structure preferable to a linked list? Think about the runtime of `push_back()` and the `access operator[]`.

What is the runtime of `Upheap` given a doubly linked list? The list has a pointer to the head and tail nodes. Let the number of elements in the heap be `n`.

Question 3
----------
Programming assignment 2 was on sorting vectors. How can we sort a vector from lowest to highest using a minimum priority queue? Describe the implementation and the runtime. Let the number of elements in the vector be `n`. Compare its runtime and the number of comparisons to another comparison based algorithm. 

Question 4
----------
Discuss the algorithm you used for `push()` and `pop()` operations. Describe an alternate implementation (For example, if you implemented it recursively, explain how you it could be written iteratively. If you did it iterativey, explain how you could have written in recursively). Analyze any differences in the time and space complexity that may exist between the two implementations (iterative and recursive).

Unordered Map Questions:
========================

Question 5
----------

You have an Unordered Map with 10 buckets. Explain your implementation of the `find()` function and how its runtime changes when the only elements present in your Unordered Map are multiples of 10, 5, 2, or any natural number. Let there be `n` elements in your Unordered Map. How could you change the number of buckets to better suit each case.

Question 6
----------

Where are the `_size`, `_buckets[]`, `_head`, and `HashNodes` of our unordered map implementation stored (stack, heap, bss, text)? Explain the Copy and Move Operators and how each of these instance fields are copied/moved in these operators. 

Question 7
----------

If you implemented your unordered map using the single list, your iterator classes likely had the following instance fields:
```C++
class iterator {
    ...
    HashNode* _node;
    ...
}
class local_iterator {
    ...
    UnorderedMap* _map;
    HashNode* _node;
    size_type _bucket;
    ...
}
```

While if you implemented your unordered map using the multi-list implementation, your iterator classes likely had these instance fields:
```C++
class iterator {
    ...
    UnorderedMap* _map;
    HashNode* _node;
    size_type _bucket;
    ...
}
class local_iterator {
    ...
    HashNode* _node;
    ...
}
```
Why do the operations of these iterators require different instance fields with the different implementations? Specifically, the `operator++`.

Question 8
----------

In the multi-list implementation, we stored an individual list in each bucket. But we can utilize other data structure in each bucket. What would be the runtime of the `find()` function given there is a linked list at every bucket? Let the number of elements in a the unordered map be `n` and the number buckets be `b`.

What is the runtime of the `find()` function given there is a binary search tree (on the hash) at every bucket?

What is the runtime of the `find()` function given there is another unordered map at every bucket of the first. Assume that the bucket count of the second unordered map is large enough that every element is in its own bucket. Is this implementation faster than a single, larger unordered map?
