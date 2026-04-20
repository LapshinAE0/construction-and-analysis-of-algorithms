#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


int main() {
    int cost_replace, cost_insert, cost_delete;
    cin >> cost_replace >> cost_insert >> cost_delete;
    
    std::string S, T;
    std::cin >> S >> T;
    
    int n = S.length();
    int m = T.length();
    
    std::vector<std::vector<int>> matrix(n + 1, std::vector<int>(m + 1, 0));
    
    for (int i = 0; i <= n; i++) {
        matrix[i][0] = i;
    }
    for (int j = 0; j <= m; j++) {
        matrix[0][j] = j;
    }
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (S[i - 1] == T[j - 1]) {
                matrix[i][j] = matrix[i - 1][j - 1];
            }
            else {
                matrix[i][j] = min({matrix[i - 1][j] + 1, matrix[i][j - 1] + 1, matrix[i - 1][j - 1] + 1});
            }
        }
    }
    
    std::cout << matrix[n][m] << std::endl;
    
    return 0;
}

