#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

enum state_cell {
    NONE = 0,
    DELETE = 1,
    INSERT = 2,
    REPLACE = 3
};

int main() {
    int cost_replace, cost_insert, cost_delete;
    cin >> cost_replace >> cost_insert >> cost_delete;
    string S, T;
    cin >> S >> T;
    
    int len_S = S.length();
    int len_T = T.length();
    vector<vector<int>> matrix(len_S + 1, vector<int>(len_T + 1, 0));
    vector<vector<state_cell>> matrix_solve(len_S + 1, vector<state_cell>(len_T + 1, NONE));
    

    for (int i = 0; i <= len_S; i++) {
        matrix[i][0] = i * cost_delete;
        matrix_solve[i][0] = DELETE;
    }
    for (int i = 0; i <= len_T; i++) {
        matrix[0][i] = i * cost_insert;
        matrix_solve[0][i] = INSERT;
    }


    
    for (int i = 1; i <= len_S; i++) {
        for (int j = 1; j <= len_T; j++) {

            if (S[i - 1] == T[j - 1]) {
                matrix[i][j] = matrix[i - 1][j - 1];
                matrix_solve[i][j] = NONE;
            } else {
                int del_cost = matrix[i - 1][j] + cost_delete;
                int ins_cost = matrix[i][j - 1] + cost_insert;
                int rep_cost = matrix[i - 1][j - 1] + cost_replace;
                int min_cost = min({del_cost, ins_cost, rep_cost});

                if (min_cost == del_cost) {
                    matrix[i][j] = del_cost;
                    matrix_solve[i][j] = DELETE;
                } 
                else if (min_cost ==  ins_cost) {
                    matrix[i][j] = ins_cost;
                    matrix_solve[i][j] = INSERT;
                } 
                else {
                    matrix[i][j] = rep_cost;
                    matrix_solve[i][j] = REPLACE;
                }
            }
        }
    }
    
    string operations;
    
    int i = len_S, j = len_T;
    while (i > 0 || j > 0) {
        if (matrix_solve[i][j] == REPLACE) {
            operations += 'R';
            i--;
            j--;
        } 
        else if (i > 0 && matrix_solve[i][j] == NONE) {
            operations += 'M';
            i--;
            j--;
        } 
        else if (i > 0 && matrix_solve[i][j] == DELETE) {
            operations += 'D';
            i--;
        } 
        else if (j > 0 && matrix_solve[i][j] == INSERT) {
            operations += 'I';
            j--;
        }
    }

    reverse(operations.begin(), operations.end());
    cout << operations << endl;
    cout << S << endl;
    cout << T << endl;
    
    return 0;
}

