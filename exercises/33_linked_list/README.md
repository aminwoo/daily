# 33 — Linked lists

The node type is defined in your file (the tests build lists out of it):

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

ListNode* reverse_list(ListNode* head);                   // in place, O(1) extra
ListNode* merge_sorted(ListNode* a, ListNode* b);         // stable: on ties, a's node first
ListNode* middle(ListNode* head);                         // second middle for even length ([1,2,3,4] -> 3)
ListNode* remove_nth_from_end(ListNode* head, int n);     // 1 <= n <= length; one pass
ListNode* cycle_start(ListNode* head);                    // node where the cycle begins, or nullptr; O(1) extra (Floyd)
bool      is_palindrome(ListNode* head);                  // O(1) extra space (reverse the second half)
```

No node is ever allocated or freed by these functions — you only relink
`next` pointers. Except for the node intentionally detached by
`remove_nth_from_end`, the returned list must contain exactly the input nodes;
the tests compare node addresses, so don't replace or lose any.
`cycle_start` must not modify the list. `is_palindrome` may leave the list
in any state.

Dummy head nodes (`ListNode dummy(0); dummy.next = head;`) remove nearly
every special case in `merge_sorted` and `remove_nth_from_end`.
