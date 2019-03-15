/*
 * 从尾到头打印链表
 */

/*
 * 输入一个链表，按链表值从尾到头的顺序返回一个ArrayList。
 */

/**
*  struct ListNode {
*        int val;
*        struct ListNode *next;
*        ListNode(int x) :
*              val(x), next(NULL) {
*        }
*  };
*/

/*
 * 思路：
 * 递归，回溯时输出
 */
class Solution {
public:
    void visitAndPrint(ListNode* node, vector<int>& result) {
        if (node == nullptr) {
            return;
        }

        visitAndPrint(node->next, result);
        result.push_back(node->val);
    }

    vector<int> printListFromTailToHead(ListNode* head) {
        vector<int> result;
        visitAndPrint(head, result);
        return result;
    }
};