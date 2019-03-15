/*
 * 二维数组中的查找
 */

/*
 * 在一个二维数组中（每个一维数组的长度相同），每一行都按照从左到右递增的顺序排序，
 * 每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。
 */


class Solution {
public:
    bool Find(int target, vector<vector<int> > array) {
        int clen = array.size();
        if (clen == 0) {
            return false;
        }

        int rlen = array[0].size();
        if (rlen == 0) {
            return false;
        }


        for (int i = 0; i < clen; i++) {
            for (int j = 0; i < rlen; i++) {
                if (array[i][j] == target) {
                    return true;
                } else if (array[i][j] < target) {
                    continue;
                } else {
                    break;
                }
            }
        }

        return false;
    }
};