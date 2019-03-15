/*
 * 重建二叉树
 */

/*
 * 输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。
 * 例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。
 */

/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

/*
 * 思路：
 * 递归方法，通过前序第一个元素构造父节点，然后分割前序和中序数组去构造子树
 * {1,2,4,7,3,5,6,8}  {4,7,2,1,5,3,8,6}  ---> 父：1  左：{2,4,7} {4,7,2}  右：{3,5,6,8} {5,3,8,6}
 * {2,4,7} {4,7,2} ---> 父：2  左 {4,7} {4,7}
 * {4,7} {4,7} ---> 父：4  右 {7} {7}
 * {7} {7} ---> 父：7
 * {3,5,6,8} {5,3,8,6} ---> 父：3  左 {5} {5} 右 {6, 8} {8, 6}
 * {5} {5} ---> 父：5
 * {6, 8} {8, 6} ---> 父：6  左 {8} {8}
 * {8} {8} ---> 父：8
 */

class Solution {
public:
    TreeNode* reConstruct(vector<int>& pre,vector<int>& vin, int preL, int preR, int vinL, int vinR, unordered_map<int, int>& vinIndexMap, unordered_map<int, int>& preIndexMap) {
        if (preL > preR || vinL > vinR || preL < 0 || preR < 0 || vinL < 0 || vinR < 0) {
            return NULL;
        }
        int curVal = pre[preL];
        TreeNode* cur = new TreeNode(curVal);
        int mid = vinIndexMap[curVal];

        int preRR = -1;
        for (int i = vinL; i <= mid - 1; i++) {
            if (preRR == -1 || preRR < preIndexMap[vin[i]]) {
                preRR = preIndexMap[vin[i]];
            } 
        }

        int preLL = -1;
        for (int i = mid + 1; i <= vinR; i++) {
            if (preLL == -1 || preLL > preIndexMap[vin[i]]) {
                preLL = preIndexMap[vin[i]];
            } 
        }

        TreeNode* left = reConstruct(pre, vin, preL + 1, preRR, vinL, mid - 1, vinIndexMap, preIndexMap);
        TreeNode* right = reConstruct(pre, vin, preLL, preR, mid + 1, vinR, vinIndexMap, preIndexMap);
        cur->left = left;
        cur->right = right;
        return cur; 
    }

    TreeNode* reConstructBinaryTree(vector<int> pre,vector<int> vin) {
        unordered_map<int, int> vinIndexMap;
        unordered_map<int, int> preIndexMap;

        for (int i = 0; i < vin.size(); i++) {
            vinIndexMap[vin[i]] = i;
        }

        for (int i = 0; i < pre.size(); i++) {
            preIndexMap[pre[i]] = i;
        }

        return reConstruct(pre, vin, 0, pre.size() - 1, 0, vin.size() - 1, vinIndexMap, preIndexMap);
    }
};