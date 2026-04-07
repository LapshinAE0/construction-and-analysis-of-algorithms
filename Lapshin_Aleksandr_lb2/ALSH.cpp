#include <iostream>
#include <vector>

using namespace std;

int INFINITY = 1e5;



double lower_bound(vector<vector<int>>& matrix, vector<bool>& visited, vector<int>& path, int n) {
    int start = path[0];
    int end = path.back();

#ifdef LOG
    cout << "Ищем нижнюю оценку дороги из вершины " << start << " в " << end << endl;
#endif

    int min_in = INFINITY;
    int min_out = INFINITY;
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {

#ifdef LOG
            cout << "Вершина " << i << ": ";
            cout << "   ребро в " << start << " = " << matrix[i][start];
            cout << "   ребро из " << end << " = " << matrix[end][i] << endl;
#endif

            if (matrix[i][start] < min_in) {
                min_in = matrix[i][start];
            }
            if (matrix[end][i] < min_out) {
                min_out = matrix[end][i];
            }
        }
    }

    int lower = (min_in + min_out) / 2;

#ifdef LOG
    cout << "Нижняя граница " << lower << endl;
#endif

    return lower;
}




int main() {
    int n;
    cin >> n;
    
    vector<vector<int>> matrix(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
            if (matrix[i][j] == 0) {
                matrix[i][j] = INFINITY;
            }
        }
    }
    
    vector<bool> visited(n, false);
    vector<int> path;
    
    path.push_back(0);
    visited[0] = true;
    int last = 0;
    int total_cost = 0;
    
#ifdef LOG
    cout << "Стартовая вершина: 0" << endl;
    cout << "Начальный путь: 0" << endl;
    cout << "Начальная стоимость: 0" << endl;
#endif

    while (path.size() < n) {


#ifdef LOG
        cout << "Текущий путь: ";
        for (int v : path) cout << v << " ";
        cout << "\nТекущая позиция: " << last << endl;
        cout << "Накопленная стоимость: " << total_cost << endl;
        cout << "Осталось посетить: " << n - path.size() << " вершин" << endl;
        cout << endl;
#endif

        
        if (path.size() == n - 1) {
#ifdef LOG
            cout << "Осталась последняя вершина" << endl;
#endif
            int next = -1;
            for (int i = 0; i < n; i++) {
                if (!visited[i]) {
                    next = i;
                    break;
                }
            }
            

            if (next == -1 || matrix[last][next] == INFINITY) {
                cout << "no path" << endl;
                return 0;
            }

#ifdef LOG
            cout << "Последняя непосещенная вершина: " << next << endl;
            cout << "Стоимость перехода из " << last << " в " << next << ": " << matrix[last][next] << endl;
#endif
            path.push_back(next);
            total_cost += matrix[last][next];
            last = next;
            break;
        }
        


        double best_value = INFINITY;
        int best_next = -1;
        
        for (int next = 0; next < n; next++) {
            if (!visited[next] && matrix[last][next] != INFINITY) {

#ifdef LOG
                cout << "\nРассматриваем вершину " << next << endl;
                cout << "  Стоимость перехода из " << last << " в " << next << ": " << matrix[last][next] << endl;
#endif

                vector<bool> temp_visited = visited;
                vector<int> temp_path = path;
                
                temp_visited[next] = true;
                temp_path.push_back(next);
                
                double L = lower_bound(matrix, temp_visited, temp_path, n);
                
                double value = matrix[last][next] + L;
                
                if (value < best_value) {
#ifdef LOG
                cout << "старая лучшая цена равна " << best_value << endl;
                cout << "новая лучшая цена равна " << value << endl;
#endif
                    best_value = value;
                    best_next = next;
                }
            }
        }
        
        if (best_next == -1) {
            cout << "no path" << endl;
            return 0;
        }
        
        path.push_back(best_next);
        total_cost += matrix[last][best_next];
        visited[best_next] = true;
        last = best_next;
    }
    

#ifdef LOG
    cout << "Текущий путь (все вершины посещены): ";
    for (int v : path) cout << v << " ";
    cout << endl;
    cout << "Последняя вершина: " << last << endl;
    cout << "Стоимость возврата " << last << " в 0: " << matrix[last][0] << endl;
#endif


    if (matrix[last][0] == INFINITY) {
        cout << "no path" << endl;
        return 0;
    }
    
    total_cost += matrix[last][0];
    path.push_back(0);
    
    cout << total_cost << endl;
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) cout << " ";
    }
    cout << endl;
    
    return 0;
}

