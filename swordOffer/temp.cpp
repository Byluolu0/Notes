#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    bool getLine(vector<vector<int> >& matrix, vector<int>& out, int& l, int& r, int& t, int& b, int op) {
        if (l > r || t > b) {
            return false;
        }

        if (op == 0) {
            for (int i = l; i <= r; i++) {
                out.push_back(matrix[t][i]);
            }
            t++;
        } else if (op == 1) {
            for (int i = t; i <= b; i++) {
                out.push_back(matrix[i][r]);
            }
            r--;
        } else if (op == 2) {
            for (int i = r; i >= l; i--) {
                out.push_back(matrix[b][i]);
            }
            b--;
        } else if (op == 3) {
            for (int i = b; i >= t; i--) {
                out.push_back(matrix[i][l]);
            } 
            l++;
        }

        return true;
    }

    vector<int> printMatrix(vector<vector<int> > matrix) {
        if (matrix.size() == 0) {
            return vector<int>();
        }

        if (matrix.size() == 1) {
            return matrix[0];
        }

        int rowT = 0;
        int colL = 0;
        int rowB = matrix.size() - 1;
        int colR = matrix[0].size() - 1;

        vector<int> result;
        int op = 0;
        while(getLine(matrix, result, colL, colR, rowT, rowB, op)) {
            op = (op + 1) % 4;
        }

        return result;
    }
};

int main() {
    Solution s;
    vector<vector<int>> src{
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
    }
    vector<int> result = s.printMatrix(src);
    for (auto i = result.begin(); i != result.end(); i++) {
        cout << *i << " ";
    }
    cout << endl;
    return 0;
}