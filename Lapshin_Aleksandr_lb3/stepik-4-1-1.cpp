#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    int cost_replace, cost_insert, cost_delete;
    cin >> cost_replace >> cost_insert >> cost_delete;
    
    string S, T;
    cin >> S >> T;
    
    int n = S.length();
    int m = T.length();
    
    vector<vector<int>> matrix(n + 1, vector<int>(m + 1, 0));
    
    
    for (int i = 0; i <= n; i++) {
        matrix[i][0] = i * cost_delete;
    }
    for (int j = 0; j <= m; j++) {
        matrix[0][j] = j * cost_insert;
    }
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (S[i - 1] == T[j - 1]) {
                matrix[i][j] = matrix[i - 1][j - 1];
            } else {
                int del_cost = matrix[i - 1][j] + cost_delete;
                int ins_cost = matrix[i][j - 1] + cost_insert;
                int rep_cost = matrix[i - 1][j - 1] + cost_replace;
                
                if (del_cost <= ins_cost && del_cost <= rep_cost) {
                    matrix[i][j] = del_cost;
                } else if (ins_cost <= del_cost && ins_cost <= rep_cost) {
                    matrix[i][j] = ins_cost;
                } else {
                    matrix[i][j] = rep_cost;
                }
            }
        }
    }

    cout << matrix[n][m] << endl;
    
    return 0;
}