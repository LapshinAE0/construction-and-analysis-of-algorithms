#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int cost_replace, cost_insert, cost_delete;
string S, T;
int len_S, len_T;
vector<vector<int>> matrix;

struct editorial_instruction {
    string operations;
    string result_S;
};

vector<editorial_instruction> all_editorial_instructions;


void collectAllPaths(int i, int j, string operations, string res_S) {
    
    if (i == 0 && j == 0) {
        reverse(operations.begin(), operations.end());
        reverse(res_S.begin(), res_S.end());
        all_editorial_instructions.push_back({operations, res_S});

        cout << "Дошли до левого верхнего угла матрицы (0, 0)" << endl; 
        cout << "Добавляем в all_editorial_instructions новый случай:" << endl;
        cout << "   operations = " << operations << endl;
        cout << "   result_S = " << res_S << endl;

        return;
    }
    
    cout << "Проверяем три возможных перехода:" << endl;
    if (i > 0 && j > 0) {
        cout << "   Диагональный случай (i-1, j-1): операция M (совпадение) или R (замена):" << endl;
        int expected_cost;
        char operate;
        if (S[i - 1] == T[j - 1]) {
            expected_cost = matrix[i - 1][j - 1];
            operate = 'M';
            cout << "      Это перация M - символы  S[i - 1] = " << S[i - 1] << " и T[j - 1] = " << T[j - 1] << " совпали" << endl;
            cout << "      Ожидаемая стоимость expected_cost = " << expected_cost << endl;
            cout << "      Нужная стоимость matrix[i][j] = " << matrix[i][j] << endl;
        } else {
            expected_cost = matrix[i - 1][j - 1] + cost_replace;
            operate = 'R';
            cout << "      Это перация R - символы  S[i - 1] = " << S[i - 1] << " и T[j - 1] = " << T[j - 1] << " не совпали" << endl;
            cout << "      Ожидаемая стоимость expected_cost = " << expected_cost << endl;
            cout << "      Нужная стоимость matrix[i][j] = " << matrix[i][j] << endl;
        }
        
        if (matrix[i][j] == expected_cost) {
            cout << "      \nНужная стоимость совпала с ожидаемой стоимостью " << endl;
            cout << "      Значит, этот переход является частью оптимального пути" << endl;
            cout << "      Добавляем операцию " << operate << " к operations " << endl;
            cout << "      Добавляем символ S[i-1] = " << S[i-1] << " к res_S" << endl;
            cout << "      Рекурсивно идём в клетку (i-1, j-1)" << endl;
            
            collectAllPaths(i - 1, j - 1, operations + operate, res_S + S[i - 1]);
        }
    }
    
    if (i > 0) {
        cout << "   Верхний случай (i-1, j): операция D (удаление):" << endl;
        int expected_cost = matrix[i - 1][j] + cost_delete;
        cout << "      Ожидаемая стоимость expected_cost = " << expected_cost << endl;
        cout << "      Нужная стоимость matrix[i][j] = " << matrix[i][j] << endl;
        if (matrix[i][j] == expected_cost) {
            cout << "      \nНужная стоимость совпала с ожидаемой стоимостью " << endl;
            cout << "      Значит, этот переход является частью оптимального пути" << endl;
            cout << "      Добавляем операцию D к operations " << endl;
            cout << "      Добавляем символ S[i-1] = " << S[i-1] << " к res_S" << endl;
            cout << "      Рекурсивно идём в клетку (i-1, j)" << endl;

            collectAllPaths(i - 1, j, operations + 'D', res_S + S[i - 1]);
        }
    }
    
    if (j > 0) {
        cout << "   Левый случай (i-1, j): операция I (вставка):" << endl;
        int expected_cost = matrix[i][j - 1] + cost_insert;
        cout << "      Ожидаемая стоимость expected_cost = " << expected_cost << endl;
        cout << "      Нужная стоимость matrix[i][j] = " << matrix[i][j] << endl;
        if (matrix[i][j] == expected_cost) {
            cout << "      \nНужная стоимость совпала с ожидаемой стоимостью " << endl;
            cout << "      Значит, этот переход является частью оптимального пути" << endl;
            cout << "      Добавляем операцию I к operations " << endl;
            cout << "      Добавляем пробел к res_S (так как в S на этом месте ничего не было)" << endl;
            cout << "      Рекурсивно идём в клетку (i, j-1)" << endl;
            
            collectAllPaths(i, j - 1, operations + 'I', res_S + ' ');
        }
    }
}

void demonstrate_instruction(editorial_instruction& instruct) {
    cout << "Операции: " << instruct.operations << "\n\n";
    
    string current = S;
    int pos = 0;
    int step = 0;
    
    cout << "Исходная строка: \"" << current << "\"\n\n";
    
    for (char op : instruct.operations) {
        cout << "Шаг " << (step + 1) << ": ";
        switch (op) {
            case 'M':
                cout << "Совпадение '" << current[pos] << "' (позиция " << pos << ")";
                pos++;
                break;
            case 'R':
                cout << "Замена '" << current[pos] << "' на '" << T[step] << "'";
                current[pos] = T[step];
                pos++;
                break;
            case 'D':
                cout << "Удаление '" << current[pos] << "' (позиция " << pos << ")";
                current.erase(pos, 1);
                break;
            case 'I':
                cout << "Вставка '" << T[step] << "' на позицию " << pos;
                current.insert(pos, 1, T[step]);
                pos++;
                break;
        }
        cout << "\n  Строка стала: \"" << current << "\"\n";
        step++;
    }
    
    cout << "\nПолученная строка: \"" << current << "\"\n";
    cout << "    Нужная строка: \"" << T << "\"\n";
}

int main() {
    cout << "Входные данные:" << endl;
    cout << "   первая строка - три числа: цена операции replace, цена операции insert, цена операции delete" << endl;
    cout << "   вторая строка - S (какую преобразовываем)" << endl;
    cout << "   третья строка - T (в какую преобразовываем)" << endl;

    cin >> cost_replace >> cost_insert >> cost_delete;
    cin >> S >> T;
    len_S = S.length();
    len_T = T.length();
    
    matrix = vector<vector<int>>(len_S + 1, vector<int>(len_T + 1, 0));
    
    for (int i = 0; i <= len_S; i++) {
        matrix[i][0] = i * cost_delete;
    }
    for (int j = 0; j <= len_T; j++) {
        matrix[0][j] = j * cost_insert;
    }
    
    for (int i = 1; i <= len_S; i++) {
        for (int j = 1; j <= len_T; j++) {
            if (S[i - 1] == T[j - 1]) {
                matrix[i][j] = matrix[i - 1][j - 1];
            } else {
                int del_cost = matrix[i - 1][j] + cost_delete;
                int ins_cost = matrix[i][j - 1] + cost_insert;
                int rep_cost = matrix[i - 1][j - 1] + cost_replace;
                matrix[i][j] = min({del_cost, ins_cost, rep_cost});
            }
        }
    }
    
    cout << "\nМинимальная стоимость предписания: " << matrix[len_S][len_T] << endl;
    

    collectAllPaths(len_S, len_T, "", "");
    
    cout << "\nМинимальная стоимость предписания: " << matrix[len_S][len_T] << endl;
    cout << "\nВсе найденные редакционные предписания с минимальной ценой\n";
    cout << "Найдено предписаний: " << all_editorial_instructions.size() << "\n\n";
    
    for (size_t idx = 0; idx < all_editorial_instructions.size(); idx++) {
        cout << "Предписание №" << (idx + 1) << ":\n";
        cout << "  " << all_editorial_instructions[idx].operations << "\n";
        cout << "  " << all_editorial_instructions[idx].result_S << "\n";
        cout << "  " << T << "\n\n";
    }
    

    cout << "\nПродемонстрируем последнее редакционное предписание:\n";
    if (!all_editorial_instructions.empty()) {
        demonstrate_instruction(all_editorial_instructions[all_editorial_instructions.size() - 1]);
    }
    
    return 0;
}
