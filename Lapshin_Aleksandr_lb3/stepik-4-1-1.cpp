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
    
    int len_S = S.length();
    int len_T = T.length();
    vector<vector<int>> matrix(len_S + 1, vector<int>(len_T + 1, 0));
    
    

    for (int i = 0; i <= len_S; i++) {
        matrix[i][0] = i * cost_delete;
    }
    for (int i = 0; i <= len_T; i++) {
        matrix[0][i] = i * cost_insert;
    }
    

    for (int i = 1; i <= len_S; i++) {
        for (int j = 1; j <= len_T; j++) {

            if (S[i - 1] == T[j - 1]) {
                matrix[i][j] = matrix[i - 1][j - 1];
            } 

            else {
                int del_cost = matrix[i - 1][j] + cost_delete;
                int ins_cost = matrix[i][j - 1] + cost_insert;
                int rep_cost = matrix[i - 1][j - 1] + cost_replace;
                int min_cost = min({del_cost, ins_cost, rep_cost});

                if (min_cost == del_cost) {
                    matrix[i][j] = del_cost;
                } 
                else if (min_cost == ins_cost) {
                    matrix[i][j] = ins_cost;
                } 
                else {
                    matrix[i][j] = rep_cost;
                }
            }

        }
    }

    cout << matrix[len_S][len_T] << endl;
    
    return 0;
}

