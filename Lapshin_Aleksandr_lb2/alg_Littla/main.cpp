#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <iomanip> 


using namespace std;
int INFNITY = numeric_limits<int>::max() / 2;
double best_cost = INFNITY;
vector<int> best_route;

struct Node_matrix {
    vector<vector<int>> matrix;
    int n;
    double lower_bound;
    vector<pair<int,int>> path;
    vector<int> row_ids;
    vector<int> col_ids;

    Node_matrix(vector<vector<int>> m, int size, double bound, vector<pair<int,int>> p, vector<int> row, vector<int> col) 
        : matrix(m), n(size), lower_bound(bound), path(p), row_ids(row), col_ids(col)  {}
};


void print_matrix(vector<vector<int>> matrix, int n) {
    cout << '\n';
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << " " << matrix[i][j] << " ";
        }
        cout << '\n';
    }
    cout << '\n' << endl;
}


int normalization_matrix(vector<vector<int>>& matrix, int n) {

#if LOG
    cout << "Матрица до нормализации: ";
    print_matrix(matrix, n);
#endif

    int lower_bound = 0;
    vector<int> min_column(n, INFNITY);
    vector<int> min_row(n, INFNITY);
    
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < min_row[i]) {
                 min_row[i] = matrix[i][j];
            }
        }
    }

    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] -= min_row[i];
        }
    }

#if LOG
    cout << "Матрица после нормализации по строчкам: ";
    print_matrix(matrix, n);
#endif

    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < min_column[j]) {
                min_column[j] = matrix[i][j];
            }
        }
    }

    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] -= min_column[j];
        }
    }

#if LOG
    cout << "Матрица после нормализации по столбцам: ";
    print_matrix(matrix, n);
#endif
    

    int min_c = accumulate(min_column.begin(), min_column.end(), 0);
    int min_r = accumulate(min_row.begin(), min_row.end(), 0);

    lower_bound = min_c + min_r;
    return lower_bound;
}

int update_cell_x_y(vector<vector<int>> matrix, int n, int i, int j, int& max_grade_cell) {
    int min_i = INFNITY;
    int min_j = INFNITY;

    for (int new_i = 0; new_i < n; new_i++) {
        for (int new_j = 0; new_j < n; new_j++) {
            if (new_i == i && new_j == j) {
                continue;
            }
            if (new_i == i && matrix[new_i][new_j] < min_i) {
                min_i = matrix[new_i][new_j];
            }
            if (new_j == j && matrix[new_i][new_j] < min_j) {
                min_j = matrix[new_i][new_j];
            }
        }
    }

    int new_grade = min_i + min_j;

    if (new_grade > max_grade_cell) {
        max_grade_cell = new_grade;
        return 1; 
    }
    return 0;
}

void evaluation_zero_cell(vector<vector<int>> matrix, int n, int& max_grade_cell_x, int& max_grade_cell_y, int& max_grade_cell) {
    max_grade_cell_x = -1;
    max_grade_cell_y = -1;
    max_grade_cell = -1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 0) {
                if (update_cell_x_y(matrix, n, i, j, max_grade_cell)) {
                    max_grade_cell_x = i;
                    max_grade_cell_y = j;
                }
            }
        }
    }
}


vector<vector<int>> branching_new_solution_add_solution(vector<vector<int>> matrix, int& n, int max_grade_cell_x, int max_grade_cell_y) {
    matrix[max_grade_cell_y][max_grade_cell_x] = INFNITY;
    matrix.erase(matrix.begin() + max_grade_cell_x);

    for (auto& row : matrix) {
        row.erase(row.begin() + max_grade_cell_y);
    }

    n--;
    return matrix;
}

vector<vector<int>> branching_new_solution_delete_solution(vector<vector<int>> matrix, int max_grade_cell_x, int max_grade_cell_y) {
    matrix[max_grade_cell_x][max_grade_cell_y] = INFNITY;
    return matrix;
}

// vector<int> reconstruct_route(Node_matrix& node) {
//     vector<int> route;
    
//     route.push_back(0);
    
//     for (auto& edge : node.path) {

// #if LOG
//         cout << "Мы дошли до матрицы 2x2 и теперь восстанавливаем путь:" << endl;
//         cout << edge.first << "   " << edge.second << endl;
// #endif
//         route.push_back(edge.second);
//     }
    
//     return route;
// }


vector<int> reconstruct_route(Node_matrix& node) {
    vector<int> route;
    route.push_back(0);
#if LOG
    cout << "Мы дошли до матрицы 2x2 и теперь восстанавливаем путь:" << endl;
    for (auto& edge : node.path) {
        cout << edge.first << "   " << edge.second << endl;
    }
#endif
    int current = 0;

    while (route.size() < node.path.size() + 1) {
        bool found = false;
        for (auto& edge : node.path) {
            if (edge.first == current) {
                route.push_back(edge.second);
                current = edge.second;
                found = true;
                break;
            }
        }
        if (!found) break;
    }
    
    return route;
}

// void handle_last_edges(Node_matrix& node) {
//     vector<pair<int,int>> zeros;
//     for (int i = 0; i < node.n; i++) {
//         for (int j = 0; j < node.n; j++) {
//             if (node.matrix[i][j] == 0) {
//                 zeros.push_back({i, j});
//             }
//         }
//     }
    
//     if (zeros.size() == 2) {
//         node.path.push_back(zeros[0]);
//         node.path.push_back(zeros[1]);
        
//         vector<int> route = reconstruct_route(node);

//         if (node.lower_bound < best_cost) {
// #if LOG
//             cout << "Этот путь оказался наилучшим решением" << endl;
//             cout << "Новое наилучшее решение" << node.lower_bound << endl;
//             cout << "Прошлое наилучшее решение" << best_cost << endl;
            
// #endif
//             best_cost = node.lower_bound;
//             best_route = route;
//         }
        
// #if LOG
//         cout << "Found route: ";
//         for (int city : route) {
//             cout << city << " ";
//         }
//         cout << endl;
//         cout << "Cost: " << node.lower_bound << endl;
// #endif
//     }
// }

void handle_last_edges(Node_matrix& node) {
    vector<pair<int,int>> zeros;
    for (int i = 0; i < node.n; i++) {
        for (int j = 0; j < node.n; j++) {
            if (node.matrix[i][j] == 0) {
                zeros.push_back({i, j});
            }
        }
    }
    
    if (zeros.size() == 2) {
        int from1 = node.row_ids[zeros[0].first];
        int to1 = node.col_ids[zeros[0].second];
        int from2 = node.row_ids[zeros[1].first];
        int to2 = node.col_ids[zeros[1].second];
        
        node.path.push_back({from1, to1});
        node.path.push_back({from2, to2});
        
    vector<int> route = reconstruct_route(node);
    
        if (node.lower_bound < best_cost) {
#if LOG
        cout << "Этот путь оказался наилучшим решением" << endl;
        cout << "Новое наилучшее решение" << node.lower_bound << endl;
        cout << "Прошлое наилучшее решение" << best_cost << endl;
        
#endif
            best_cost = node.lower_bound;
            best_route = route;
        }
    }
}


void recursion(Node_matrix struct_matrix) {

    if (struct_matrix.lower_bound >= best_cost) {
        return;
    }

    double lower_bound = struct_matrix.lower_bound;
    vector<vector<int>> matrix = struct_matrix.matrix;
    int n = struct_matrix.n;
    vector<pair<int,int>> puth = struct_matrix.path;
    
    if (n == 2) {
        handle_last_edges(struct_matrix);
        return;
    }


    lower_bound = lower_bound + normalization_matrix(matrix, n);
    int max_grade_cell_x, max_grade_cell_y, max_grade_cell;
    evaluation_zero_cell(matrix, n, max_grade_cell_x, max_grade_cell_y, max_grade_cell);

#if LOG
    cout << "max_grade_cell_x = " << max_grade_cell_x << endl;
    cout << "max_grade_cell_y = " << max_grade_cell_y << endl;
    cout << "max_grade_cell = " << max_grade_cell << endl;
#endif

    int n_add_solution = n;
    vector<vector<int>> matrix_add_solution = branching_new_solution_add_solution(matrix, n_add_solution, max_grade_cell_x, max_grade_cell_y);
    double lower_bound_add_solution = lower_bound + normalization_matrix(matrix_add_solution, n_add_solution);
    vector<pair<int,int>> puth_add_solution = puth;
    
    vector<int> new_row_ids = struct_matrix.row_ids;
    vector<int> new_col_ids = struct_matrix.col_ids;
    new_row_ids.erase(new_row_ids.begin() + max_grade_cell_x);
    new_col_ids.erase(new_col_ids.begin() + max_grade_cell_y);

    int from_real = struct_matrix.row_ids[max_grade_cell_x];
    int to_real = struct_matrix.col_ids[max_grade_cell_y];
    puth_add_solution.push_back({from_real, to_real});

    Node_matrix new_struct_matrix_add_solution(matrix_add_solution, n_add_solution, lower_bound_add_solution, puth_add_solution, new_row_ids, new_col_ids);
    recursion(new_struct_matrix_add_solution);


    int n_delete_solution = n;
    vector<vector<int>> matrix_delete_solution = branching_new_solution_delete_solution(matrix, max_grade_cell_x, max_grade_cell_y);
    double lower_bound_delete_solution = lower_bound + normalization_matrix(matrix_delete_solution, n_delete_solution);
    vector<pair<int,int>> puth_delete_solution = puth;

    Node_matrix new_struct_matrix_delete_solution(matrix_delete_solution, n_delete_solution, lower_bound_delete_solution, puth_delete_solution, struct_matrix.row_ids, struct_matrix.col_ids);
    recursion(new_struct_matrix_delete_solution);

}


int main() {
    int n;
    cin >> n;
    vector<vector<int>> matrix(n, vector<int>(n));
    double lower_bound = 0;
    
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
            if (i == j) {
                matrix[i][j] = INFNITY;
            }
        }
    }
    vector<int> ids(n);
    iota(ids.begin(), ids.end(), 0);
    Node_matrix m(matrix, n, lower_bound, {}, ids, ids);
    recursion(m);

    for (int i = 0; i < best_route.size() - 1; i++) {
        if (i != best_route.size() - 1 && i != 0) {
            cout << " ";
        }
        cout << best_route[i];
    }
    cout << '\n' << fixed << setprecision(1) << best_cost << endl;
    return 0;
}

