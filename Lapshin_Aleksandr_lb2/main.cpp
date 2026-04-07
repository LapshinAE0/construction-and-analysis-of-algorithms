#include <iostream>
#include <vector>

using namespace std;

int INFINITY = 1e5;
vector<vector<int>> matrix_orig;
vector<vector<int>> matrix_mask_last;
vector<vector<int>> matrix_path;
int n;

int recursion(int mask, int last) {
    int full_mask = (1 << n) - 1;
#ifdef LOG
    cout << "\nrecursion: mask = " << mask << ", last = " << last << endl;
#endif

    if (mask == full_mask) {
#ifdef LOG
        cout << "  Все вершины посещены" << endl;
#endif
        return matrix_orig[last][0];
    }
    
    if (matrix_mask_last[mask][last] != -1) {
#ifdef LOG
        cout << "  Этот случай уже вычислен вычислен! matrix_mask_last[" << mask << "][" << last << "] = " << matrix_mask_last[mask][last] << endl;
#endif
        return matrix_mask_last[mask][last];
    }
    
    int min_cost = INFINITY;
    int best_next_point = -1;
    
    for (int point = 0; point < n; point++) {

        bool puth_exists = matrix_orig[last][point] != INFINITY;
        bool point_not_in_mask = !(mask & (1 << point));

        if (point_not_in_mask && puth_exists) {

#ifdef LOG
            cout << "   Рассматриваем переход из " << last << " в " << point;
            cout << "   Стоимость перехода = " << matrix_orig[last][point] << endl;
#endif

            int new_mask = mask | (1 << point);
            int cost = matrix_orig[last][point] + recursion(new_mask, point);

#ifdef LOG
            cout << "    Стоимость через " << point << " = " << cost << endl;
#endif

            if (cost < min_cost) {

#ifdef LOG
                cout << "Найдено новое лучшее решение! Было " << min_cost << ", стало " << cost << endl;
#endif

                min_cost = cost;
                best_next_point = point;
            }

        }
    }

#ifdef LOG
    cout << "  Лучший переход из " << last << " в " << best_next_point << " стоит " << min_cost << endl;;
    cout << "  Сохраняем matrix_path[" << mask << "][" << last << "] = " << best_next_point << endl;
    cout << "  Сохраняем matrix_mask_last[" << mask << "][" << last << "] = " << min_cost << endl;
#endif
    
    matrix_path[mask][last] = best_next_point;
    
    matrix_mask_last[mask][last] = min_cost;
    return min_cost;
}




vector<int> reconstr_puth() {
#ifdef LOG
    cout << "\nВосстанавливаем путь" << endl;
#endif
    vector<int> path;
    path.push_back(0);
    int mask = 1 << 0;
    int last = 0;
    int full_mask = (1 << n) - 1;
    
    while (mask != full_mask) {
        int next = matrix_path[mask][last];
#ifdef LOG
        cout << "mask = " << mask << ", last = " << last << endl;
        cout << "matrix_path[" << mask << "][" << last << "] = " << next << endl;
#endif
        path.push_back(next);
        mask |= (1 << next);
        last = next;
#ifdef LOG
        cout << "Добавляем " << next << " в путь" << endl;
        cout << "Новая маска: " << mask << endl;
        cout << "Текущий путь: ";
        for (int v : path) cout << v << " ";
#endif
    }
    
    path.push_back(0);

#ifdef LOG
    cout << "\nДобавляем возврат в 0" << endl;
#endif

    return path;
}


int main() {
    cin >> n;
    
    matrix_orig.assign(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> matrix_orig[i][j];
            if (matrix_orig[i][j] == 0) {
                matrix_orig[i][j] = INFINITY;
            }
        }
    }
    
    matrix_mask_last.assign(1 << n, vector<int>(n, -1));
    matrix_path.assign(1 << n, vector<int>(n, -1));
    
    int start_mask = 1 << 0;
    int result = recursion(start_mask, 0);
    
    if (result == INFINITY) {
        cout << "no path";
        return 0;
    }
    
    vector<int> full_path = reconstr_puth();
    
    cout << result << endl;
    for (int i = 0; i < full_path.size(); i++) {
        cout << full_path[i];
        if (i < full_path.size() - 1) cout << " ";
    }
    cout << endl;
    
    return 0;
}
