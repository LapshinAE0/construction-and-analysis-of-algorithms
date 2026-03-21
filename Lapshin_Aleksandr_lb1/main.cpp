#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include <fstream>


using namespace std;

long long operations_count = 0;

int CountEmptyCell(vector<vector<int>>& board) {
    int N = board.size();
    int count = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) {
                count++;
            } 
            operations_count++;
        }
    }
    return count;
}


bool FullBoard(vector<vector<int>>& board, int& x, int& y) {
    int N = board.size();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            operations_count++;
            if (board[i][j] == 0) {
                x = i;
                y = j;
                return false;
            } 
        }
    }
    return true;
}

bool CanPlace(vector<vector<int>>& board, int x, int y, int size) {
    int N = board.size();
    operations_count++;

    if (x + size > N || y + size > N) {
        return false;
    }
    for (int i = x ; i < x + size; i++) {
        for (int j = y ; j < y + size; j++) {
            operations_count++;
            if (board[i][j] == 1) {
                return false;
            }
        }
    }
    return true;
}

void PlaceSquare(vector<vector<int>>& board, int x, int y, int size) {
    for (int i = x ; i < x + size; i++) {
        for (int j = y ; j < y + size; j++) {
            operations_count++;
            board[i][j] = 1;
        }
    }
}

struct Square {
    int x, y, size;
};


struct StateBoard {
    vector<vector<int>> board;
    vector<Square> placed_sq;
};

void PrintBoard(int N, vector<Square>& solution) {
    cout << "\n";
    vector<vector<int>> board(N, vector<int>(N, 0));

    int number = 1;
    for (const auto& sq : solution) {
        int x = sq.x;
        int y = sq.y;
        int size = sq.size;
        for (int i = x; i < x + size; ++i) {
            for (int j = y; j < y + size; ++j) {
                board[i][j] = number;
            }
        }
        ++number;
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << board[j][i] << " ";
        }
        cout << endl;
    }
    cout << "\n\n";

}

void FormedSolution(vector<Square> solution, int N) {
    cout << solution.size() << endl;
    for (auto& square : solution) {
        cout << square.x + 1 << " "
            << square.y + 1 << " "
            << square.size << "\n";
    }
    PrintBoard(N, solution);
}


vector<Square> EvenSolution(int N) {
    vector<Square> solution;
    int half = N / 2;
    
    solution.push_back({0, 0, half});
    solution.push_back({0, half, half});
    solution.push_back({half, 0, half});
    solution.push_back({half, half, half});
    operations_count += 4;
    
    return solution;
}


vector<Square> InitBestSolution(int N) {
    vector<Square> solution;
    Square sq = {0, 0, N - 1};
    solution.push_back(sq);

    for (int i = 0; i < N; i++) {
        Square sq = {N - 1, i, 1};
        solution.push_back(sq);
        operations_count++;

    }
    
    for (int i = 0; i < N - 1; i++) {
        Square sq = {i, N - 1, 1};
        solution.push_back(sq);
        operations_count++;
    }
    return solution;
}



void write_results_to_file(int N, long long operations) {
    ofstream file("results.csv", ios::app);
    if (file.is_open()) {
        file << N << "," << operations << "\n";
        file.close();
    }
}



int main() {
    
    int N;
    cin >> N;

    if (N % 2 == 0) {
        #ifdef LOG
        cout << "Длина стороны квадрата четная, поэтому ставим 4 квадрата:" << endl;
        cout << "Ставаим квадрат размера N = " << N / 2 << " на координаты x = " << 0 + 1 << " y = " << 0 + 1 << endl;
        cout << "Ставаим квадрат размера N = " << N / 2 << " на координаты x = " << 0 + 1 << " y = " << N / 2 + 1 << endl;
        cout << "Ставаим квадрат размера N = " << N / 2 << " на координаты x = " << N / 2 + 1 << " y = " << 0 + 1 << endl;
        cout << "Ставаим квадрат размера N = " << N / 2 << " на координаты x = " << N / 2 + 1 << " y = " << N / 2 + 1 << endl;
        #endif
        FormedSolution(EvenSolution(N), N);
        operations_count++;
        write_results_to_file(N, operations_count);
        return 0;
    }

    stack<StateBoard> st;
    StateBoard start;
    start.board = vector<vector<int>>(N, vector<int>(N, 0));
    st.push(start);
    vector<Square> best_solution = InitBestSolution(N);
    

    while(!st.empty()) {
        StateBoard current = st.top();
        st.pop();
        operations_count++;



        #ifdef LOG
        cout << "\nИзвлекаем состояние доски из стека" << endl;
        cout << "Текущее количество квадратов: " << current.placed_sq.size() << endl;
        if (!current.placed_sq.empty()) {
            cout << "Последний поставленный квадрат: размер = "\
                << current.placed_sq.back().size 
                << " на координаты x = " << current.placed_sq.back().x + 1 << ", y = " 
                << current.placed_sq.back().y + 1 << endl;
        }
        cout << "Доска в этом состоянии:" << endl;
        PrintBoard(N, current.placed_sq);
        #endif
        
        int x, y;
        if (FullBoard(current.board, x, y)) {
            if (current.placed_sq.size() < best_solution.size()) {
                best_solution = current.placed_sq;
            }
            operations_count++;
            #ifdef LOG
            cout << "Доска заполнилась! Квадратов: " << current.placed_sq.size() << endl;
            #endif
            continue;
        }
        
        if (current.placed_sq.size() >= best_solution.size()) {
            operations_count++;
            #ifdef LOG
            cout << "Количество квадратов на доске больше или равно количеству квадратов в лучшем решении!\n" << endl;
            #endif
            continue;
        }
        int m = min(N - x, N - y);
        if (current.placed_sq.size() + CountEmptyCell(current.board) / (m * m) >= best_solution.size()) {
            #ifdef LOG
            cout << "При наилучшем исходе размещения квадратов, количество квадратов будет больше или равно количеству квадратов в лучшем решении!\n" << endl;
            #endif
            operations_count++;
            continue;
        }
        int min_size = (current.placed_sq.empty()) ? N / 2 : 0;
        int max_size = min({N - x, N - y, N - 2});
        while (max_size > 0 && !CanPlace(current.board, x, y, max_size)) {
            operations_count++;
            max_size--;
        }
        
        for(int size = max_size; size > min_size; size--) {
            
            operations_count++;

            if (CanPlace(current.board, x, y, size)) {
                StateBoard new_state1 = current;
                PlaceSquare(new_state1.board, x, y, size);
                new_state1.placed_sq.push_back({x, y, size});
                st.push(new_state1);
                operations_count++;

                #ifdef LOG
                cout << "Ставаим квадрат размера N = " <<  size << " на координаты x = " << x + 1 << " y = " << y + 1 << endl;
                PrintBoard(N, new_state1.placed_sq);
                cout << "Операций = " << operations_count << endl;
                #endif
                if (current.placed_sq.empty()) {
                    int second_size = N - size;
                
                    StateBoard new_state2 = current;
                    PlaceSquare(new_state2.board, 0, 0, size);
                    PlaceSquare(new_state2.board, size, 0, second_size);
                    PlaceSquare(new_state2.board, 0, size, second_size);
                    new_state2.placed_sq.push_back({x, y, size});
                    new_state2.placed_sq.push_back({size, 0, second_size});
                    new_state2.placed_sq.push_back({0, size, second_size});
                    st.push(new_state2);

                    operations_count += 3;

                    #ifdef LOG
                    cout << "Ставаим три квадрата размера N = " <<  size << " , " << second_size << " , " << second_size <<\
                     " на координаты x = " << x + 1 << " y = " << y + 1 << "\n" <<\
                      "x = " << size + 1 << " y = " << 0 + 1 << "\n" <<\
                      "x = " << 0 + 1 << " y = " << size + 1 << endl;
                    PrintBoard(N, new_state2.placed_sq);
                    cout << "Операций = " << operations_count << endl;
                    #endif

                }
            }
        }
    }

    write_results_to_file(N, operations_count);
    FormedSolution(best_solution, N);
    return 0;
}