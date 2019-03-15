/*
 * 替换空格
 */

/*
 * 请实现一个函数，将一个字符串中的每个空格替换成“%20”。
 * 例如，当字符串为We Are Happy.则经过替换之后的字符串为We%20Are%20Happy。
 */

class Solution {
public:
    void replaceSpace(char *str,int length) {
        char temp[length];
        strncpy(temp, str, length);
        
        int idx = 0;
        for (int i = 0; i < length; i++) {
            char ch = temp[i];
            if (ch == ' ') {
                str[idx] = '%';
                str[idx + 1] = '2';
                str[idx + 2] = '0';
                idx += 3;
            } else {
                str[idx++] = ch;
            }
        }
    }
};