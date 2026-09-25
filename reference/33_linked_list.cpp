#include <bits/stdc++.h>
using namespace std;

struct ListNode {
  int val;
  ListNode* next;

  ListNode(int v) : val(v), next(nullptr) {}
};

ListNode* reverse_list(ListNode* head) {
  ListNode* prev = nullptr;
  while (head) {
    ListNode* nxt = head->next;
    head->next = prev;
    prev = head;
    head = nxt;
  }
  return prev;
}

ListNode* merge_sorted(ListNode* a, ListNode* b) {
  ListNode dummy(0);
  ListNode* t = &dummy;
  while (a && b) {
    if (b->val < a->val) {
      t->next = b;
      b = b->next;
    } else {
      t->next = a;
      a = a->next;
    }
    t = t->next;
  }
  t->next = a ? a : b;
  return dummy.next;
}

ListNode* middle(ListNode* head) {
  ListNode *slow = head, *fast = head;
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }
  return slow;
}

ListNode* remove_nth_from_end(ListNode* head, int n) {
  ListNode dummy(0);
  dummy.next = head;
  ListNode *fast = &dummy, *slow = &dummy;
  for (int i = 0; i < n; ++i) fast = fast->next;
  while (fast->next) {
    fast = fast->next;
    slow = slow->next;
  }
  slow->next = slow->next->next;
  return dummy.next;
}

ListNode* cycle_start(ListNode* head) {
  ListNode *slow = head, *fast = head;
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast) {
      slow = head;
      while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
      }
      return slow;
    }
  }
  return nullptr;
}

bool is_palindrome(ListNode* head) {
  ListNode* second_half = reverse_list(middle(head));
  ListNode* first_half = head;
  while (second_half) {
    if (first_half->val != second_half->val) return false;
    first_half = first_half->next;
    second_half = second_half->next;
  }
  return true;
}
