#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


int main() {
    std::string S, T;
    std::cin >> S >> T;
    
    int len_S = S.length();
    int len_T = T.length();
    
    std::vector<std::vector<int>> matrix(len_S + 1, std::vector<int>(len_T + 1, 0));
    
    for (int i = 0; i <= len_S; i++) {
        matrix[i][0] = i;
    }
    for (int i = 0; i <= len_T; i++) {
        matrix[0][i] = i;
    }
    
    for (int i = 1; i <= len_S; i++) {
        for (int j = 1; j <= len_T; j++) {
            if (S[i - 1] == T[j - 1]) {
                matrix[i][j] = matrix[i - 1][j - 1];
            }
            else {
                int del_cost = matrix[i - 1][j] + 1;
                int ins_cost = matrix[i][j - 1] + 1;
                int rep_cost = matrix[i - 1][j - 1] + 1;
                matrix[i][j] = min({del_cost, ins_cost, rep_cost});
            }
        }
    }
    
    std::cout << matrix[len_S][len_T] << std::endl;
    
    return 0;
}


