#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <iomanip>


using namespace std;



int char_to_idx(char c) {
    switch(c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        case 'N': return 4;
    }
    return -1;
}

char idx_to_char(int i) {
    switch(i) {
        case 0: return 'A';
        case 1: return 'C';
        case 2: return 'G';
        case 3: return 'T';
        case 4: return 'N';
    }
    return '?';
}

struct node {
    int next[5];
    int final_link;
    int suffix_link;
    int parent;
    int pchar;
    int pattern_id;

    node() {
        for (int i = 0; i < 5; i++) {
            next[i] = -1;
        }
        suffix_link = -1;
        parent = -1;
        pattern_id = -1;
    }
};

vector<node> bor;
vector<string> patterns;
vector<int> pattern_lengths;


void print_queue(queue<int> q) {
    while (!q.empty()) {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;
}

void print_automaton() {
    cout << "\nАВТОМАТ" << endl;
    cout << left << setw(4) << "V" 
         << setw(17) << "строка" 
         << setw(6) << "suff"
         << setw(8) << "final"
         << setw(18) << "терминал"
         << "переходы" << endl;
    cout << string(50, '-') << endl;
    
    for (int v = 0; v < bor.size(); v++) {
        cout << setw(4) << v;
        
        string str = "";
        int temp = v;
        while (temp != 0) {
            str = idx_to_char(bor[temp].pchar) + str;;
            temp = bor[temp].parent;
        }
        cout << setw(12) << (str.empty() ? "root" : str);
        
        cout << setw(6) << bor[v].suffix_link;
        cout << setw(8) << bor[v].final_link;
        cout << setw(12) << (bor[v].pattern_id != -1 ? patterns[bor[v].pattern_id] : "-");
        
        for (int c = 0; c < 5; c++) {
            if (bor[v].next[c] != -1 && bor[v].next[c] != 0) {
                cout << idx_to_char(c) << "→" << bor[v].next[c] << " ";
            }
        }
        cout << endl;
    }
}

void print_tree(int v = 0, string indent = "") {
    if (v == 0) {
        cout << "0 (root, suff=" << bor[0].suffix_link << ", final=" << bor[0].final_link << ")" << endl;
    }
    
    vector<int> children;
    for (int c = 0; c < 5; c++) {
        if (bor[v].next[c] != -1 && bor[v].next[c] != 0) {
            children.push_back(bor[v].next[c]);
        }
    }
    
    for (int i = 0; i < children.size(); i++) {
        bool last = (i == children.size() - 1);
        int u = children[i];
        
        cout << indent;
        if (last) cout << "└── ";
        else cout << "├── ";
        
        cout << u << " (suff=" << bor[u].suffix_link 
             << ", final=" << bor[u].final_link;
        
        if (bor[u].pattern_id != -1) {
            cout << ", term=\"" << patterns[bor[u].pattern_id] << "\"";
        }
        cout << ")" << endl;
        
        if (last) print_tree(u, indent + "    ");
        else print_tree(u, indent + "│   ");
    }
}



void add_string(string& s, int pattern_id) {
    cout << "Добавляем в бор считанную строчку " << s << endl;

    int v = 0;
    for (char ch : s) {
        int c = char_to_idx(ch);
        if (bor[v].next[c] == -1) {
            bor[v].next[c] = bor.size();
            bor.emplace_back();
            bor.back().parent = v;
            bor.back().pchar = c;
        }

        v = bor[v].next[c];
    }
    bor[v].pattern_id = pattern_id;

    cout << "\nДобавлена \"" << s << "\"" << endl;
    print_tree();

}




void build_bor_suffix() {
    cout << "\n\nСоздаем очередь для BFS, чтобы построить суффиксные и конечные ссылки в боре" << endl;
    queue<int> q;
    bor[0].final_link = 0;
    
    cout << "   Обрабатываем корень (первые узлы бора - алфавита {A,C,G,T,N})" << endl;
    for (int c = 0; c < 5; c++) {
        if (bor[0].next[c] != -1) {
            int v = bor[0].next[c];
            bor[v].suffix_link = 0;
            bor[v].final_link = 0;
            q.push(v);
        } else {
            bor[0].next[c] = 0;
        }
    }

    cout << "   Присвоили первым узлам суффкисные и конечные ссылки на корень" << endl;
    print_tree();

    while (!q.empty()) {
        int v = q.front();
        cout << "       Берем первый элменет из очереди и удаляем его из очереди: v = " << v << endl;
        cout << "       Обрабатываем для этой ноды бора все возмоные дороги {A=0,C=1,G=2,T=3,N=4}" << endl;
        q.pop();
        
        for (int c = 0; c < 5; c++) {
            int child = bor[v].next[c];
            cout << "           Вычислили ребенка по элементу с индексом = " << c << endl;
            cout << "           child = bor[v].next[c] = " << child << endl;
    
            if (child != -1) {
                q.push(child);
                cout << "               Ребенок есть, добавляем его в очередь, очередь = ";
                print_queue(q);

                cout << "               Идем по суффиксным ссылкам родителя (v): link = bor[v].suffix_link = " << bor[v].suffix_link << endl;
                int link = bor[v].suffix_link;
                while (bor[link].next[c] == -1) {
                    link = bor[link].suffix_link;
                    cout << "               Так как из этой ноды нету дороги по символу с индексом " << c << " то идем в след префиксную ссылку: link = bor[link].suffix_link = " << bor[link].suffix_link << endl;
                }
                bor[child].suffix_link = bor[link].next[c];

                
                cout << "               Теперь находим конечную ссылку (v): link = bor[v].suffix_link = " << bor[v].suffix_link << endl;
                link = bor[child].suffix_link;
                if (bor[link].pattern_id != -1) {
                    cout << "               Так как нода " << bor[link].pattern_id << " является терминальной, то записываем ее как конечную" << endl;
                    bor[child].final_link = link;
                } else {
                    cout << "               Так как нода " << bor[link].pattern_id << " не является терминальной, то записываем в конечную ссылку конечную ссылку этой ноды" << endl;
                    bor[child].final_link = bor[link].final_link;
                }
            }
        }
    }
    cout << "\n\n ПОЛУЧЕННОЕ ДЕРЕВО:" << endl;
    print_tree();
}

void get_max_lens() {
    cout << "\n\nВычислияем длину самой длинной цепочки из суффиксных ссылок и самой длинной цепочки из конечных ссылок в автомате" << endl;
    int max_suffix_link = 0;
    int max_suffix_node = 0;

    for (int v = 1; v < bor.size(); v++) {
        cout << "\nСчитаем длину суффиксной ссылки для v = " << v << endl;
        int len = 0;
        int temp = v;
        while (temp != 0) {
            len++;
            temp = bor[temp].suffix_link;
        }
        cout << "   Макс длина = " << max_suffix_link << endl;
        cout << "   Найденная длина = " << max_suffix_link << endl;
        if (len > max_suffix_link) {
            cout << "       Обновляем максимально длинную суффиксную ссылку" << endl;
            max_suffix_link = len;
            max_suffix_node = v;
        }
    }

    int max_final_link = 0;
    int max_final_node = 0;

    for (int v = 1; v < bor.size(); v++) {
        cout << "\nСчитаем длину конечной ссылки для v = " << v << endl;
        int len = 0;
        int temp = v;
        while (temp != 0) {
            len++;
            temp = bor[temp].final_link;
        }
        cout << "   Макс длина = " << max_suffix_link << endl;
        cout << "   Найденная длина = " << max_suffix_link << endl;
        if (len > max_final_link) {
            cout << "       Обновляем максимально длинную конечную ссылку" << endl;
            max_final_link = len;
            max_final_node = v;
        }
    }

    cout << "\nМакс цепочка suffix_link: " << max_suffix_link << " (вершина " << max_suffix_node << ")" << endl;
    cout << "Макс цепочка final_link: " << max_final_link << " (вершина " << max_final_node << ")" << endl;
}

int go(int v, int c) {
    cout << "go из вершины " << v << " по символу " << c << endl;
    if (bor[v].next[c] != -1) {
        cout << "путь по такому символу из такой вершины есть, поэтому переходим туда" << endl;
        return bor[v].next[c];
    }

    if (v == 0) {
        cout << "Вершина - нулевая и дороги нету по этому символу, поэтому возвращаем 0" << endl;
        return 0;
    }

    
    cout << "Вершина не нулевая и дороги нету, поэтому рекурсивно вызываем снова go на суффикс нынешней вершины bor[v].suffix_link = " << bor[v].suffix_link << endl;
    return go(bor[v].suffix_link, c);
}

struct target {
    int pos;
    int pat;
    
    bool operator<(const target& other) const {
        if (pos != other.pos) return pos < other.pos;
        return pat < other.pat;
    }
};




int main() {
    string text;
    getline(cin, text);
    
    int n;
    cin >> n;
    cin.ignore();


    patterns.resize(n);
    pattern_lengths.resize(n);
    
    bor.emplace_back();
    
    for (int i = 0; i < n; i++) {
        getline(cin, patterns[i]);
        pattern_lengths[i] = patterns[i].length();
        add_string(patterns[i], i);
    }
    

    build_bor_suffix();
    
    get_max_lens();

    print_automaton();

    vector<target> results;
    int v = 0;
    
    cout << "\n\nАнализируем исходный текст = " << text << endl;
    for (int i = 0; i < text.length(); i++) {
        cout << "   Считываем чар = " << text[i] << endl;
        int c = char_to_idx(text[i]);
        cout << "       Переходим из вершины = " << v << " по ветке с индексом символа = " << c << endl;
        v = go(v, c);
        cout << "       Перешли в вершину = " << v << endl;
        int temp = v;
        while (temp != 0) {
            if (bor[temp].pattern_id != -1) {
                int start_pos = i - pattern_lengths[bor[temp].pattern_id] + 1;
                cout << "           Нашли слово = " << patterns[bor[temp].pattern_id] << ". Индекс вхождения = " << start_pos << endl;
                results.push_back({start_pos + 1, bor[temp].pattern_id + 1});
            }
            temp = bor[temp].final_link;
        }
    }
    
    cout << "\n\nСортруем результат и выводим" << endl;
    sort(results.begin(), results.end());
    
    for (const auto& occ : results) {
        cout << occ.pos << " " << occ.pat << "\n";
    }
    
    return 0;
}

/*
NTAG
3
TAGT
TAG
T
*/
