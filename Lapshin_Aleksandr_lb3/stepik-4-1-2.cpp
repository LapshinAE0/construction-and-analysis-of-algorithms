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
    
    vector<vector<int>> dir(n + 1, vector<int>(m + 1, -1));
    
    for (int i = 0; i <= n; i++) {
        matrix[i][0] = i * cost_delete;
        if (i > 0) dir[i][0] = 1;
    }
    for (int j = 0; j <= m; j++) {
        matrix[0][j] = j * cost_insert;
        if (j > 0) dir[0][j] = 2;
    }
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (S[i - 1] == T[j - 1]) {
                matrix[i][j] = matrix[i - 1][j - 1];
                dir[i][j] = 0;
            } else {
                int del_cost = matrix[i - 1][j] + cost_delete;
                int ins_cost = matrix[i][j - 1] + cost_insert;
                int rep_cost = matrix[i - 1][j - 1] + cost_replace;
                
                if (del_cost <= ins_cost && del_cost <= rep_cost) {
                    matrix[i][j] = del_cost;
                    dir[i][j] = 1;
                } else if (ins_cost <= del_cost && ins_cost <= rep_cost) {
                    matrix[i][j] = ins_cost;
                    dir[i][j] = 2;
                } else {
                    matrix[i][j] = rep_cost;
                    dir[i][j] = 0;
                }
            }
        }
    }
    
    string operations;
    string result_S, result_T;
    
    int i = n, j = m;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && dir[i][j] == 0) {
            if (S[i - 1] == T[j - 1]) {
                operations = 'M' + operations;
            } else {
                operations = 'R' + operations;
            }
            result_S = S[i - 1] + result_S;
            result_T = T[j - 1] + result_T;
            i--;
            j--;
        } else if (i > 0 && dir[i][j] == 1) {
            operations = 'D' + operations;
            result_S = S[i - 1] + result_S;
            result_T = ' ' + result_T;
            i--;
        } else if (j > 0 && dir[i][j] == 2) {
            operations = 'I' + operations;
            result_S = ' ' + result_S;
            result_T = T[j - 1] + result_T;
            j--;
        }
    }
    
    cout << operations << endl;
    cout << result_S << endl;
    cout << result_T << endl;
    
    return 0;
}