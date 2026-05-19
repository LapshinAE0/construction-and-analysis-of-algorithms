#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>


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
        final_link = -1;
        parent = -1;
        pchar = -1;
        pattern_id = -1;
    }
};

vector<node> bor;
vector<string> patterns;
vector<int> pattern_lengths;

#ifdef LOG
void print_queue(queue<int> q) {
    while (!q.empty()) {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;
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
#endif

void add_string(string& s, int pattern_id) {
#ifdef LOG
    cout << "Добавляем в бор строку \"" << s << "\"" << endl;
#endif

    int v = 0;
    for (char ch : s) {
        int c = char_to_idx(ch);
        if (bor[v].next[c] == -1) {
            bor[v].next[c] = bor.size();
            bor.emplace_back();
            bor.back().parent = v;
            bor.back().pchar = c;
#ifdef LOG
            cout << "  Новая вершина " << bor.size() - 1 << " по символу '" << ch << "'" << endl;
#endif
        }
        v = bor[v].next[c];
    }
    bor[v].pattern_id = pattern_id;

#ifdef LOG
    cout << "  Вершина " << v << " терминал \"" << s << "\"" << endl;
    cout << "Бор после добавления:" << endl;
    print_tree();
    cout << endl;
#endif
}

void build_bor() {
#ifdef LOG
    cout << "\nПОСТРОЕНИЕ БОРА" << endl;
    cout << "Инициализация детей корня:" << endl;
#endif

    queue<int> q;
    bor[0].final_link = 0;
    bor[0].suffix_link = 0;
    
    for (int c = 0; c < 5; c++) {
        if (bor[0].next[c] != -1) {
            int v = bor[0].next[c];
            bor[v].suffix_link = 0;
            bor[v].final_link = 0;
            q.push(v);
#ifdef LOG
            cout << "  Вершина " << v << ": suff=0, final=0, добавлена в очередь" << endl;
#endif
        } else {
            bor[0].next[c] = 0;
        }
    }

#ifdef LOG
    cout << "\nBFS обход:" << endl;
#endif

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        
#ifdef LOG
        cout << "  Обрабатываем вершину " << v << " (suff=" << bor[v].suffix_link << ")" << endl;
#endif
        
        for (int c = 0; c < 5; c++) {
            int child = bor[v].next[c];
            if (child != -1) {
                q.push(child);
                
                int link = bor[v].suffix_link;
                while (bor[link].next[c] == -1) {
                    link = bor[link].suffix_link;
                }
                bor[child].suffix_link = bor[link].next[c];
                
#ifdef LOG
                cout << "    Ребёнок " << child << " по '" << idx_to_char(c) << "': suff=" << bor[child].suffix_link;
#endif
                
                link = bor[child].suffix_link;
                if (bor[link].pattern_id != -1) {
                    bor[child].final_link = link;
#ifdef LOG
                    cout << ", final=" << link << " (терминал \"" << patterns[bor[link].pattern_id] << "\")";
#endif
                } else if (link == 0) {
                    bor[child].final_link = 0;
#ifdef LOG
                    cout << ", final=0";
#endif
                } else {
                    bor[child].final_link = bor[link].final_link;
#ifdef LOG
                    cout << ", final=" << bor[child].final_link << " (от " << link << ")";
#endif
                }
#ifdef LOG
                cout << endl;
#endif
            }
        }
    }

#ifdef LOG
    cout << "\nБор построен:" << endl;
    print_tree();
#endif
}

int go(int v, int c) {
#ifdef LOG
    cout << "  go(" << v << ", '" << idx_to_char(c) << "') → ";
#endif

    if (bor[v].next[c] != -1) {
#ifdef LOG
        cout << bor[v].next[c] << " (прямой переход)" << endl;
#endif
        return bor[v].next[c];
    }

    if (v == 0) {
#ifdef LOG
        cout << "0 (корень)" << endl;
#endif
        return 0;
    }

#ifdef LOG
    cout << "рекурсия через suff=" << bor[v].suffix_link << endl;
#endif
    return go(bor[v].suffix_link, c);
}

void get_max_lens() {
    int max_suffix_link = 0;
    int max_suffix_node = 0;
    
    for (int v = 1; v < bor.size(); v++) {
        int len = 0;
        int temp = v;
        while (temp != 0) {
            len++;
            temp = bor[temp].suffix_link;
        }
        if (len > max_suffix_link) {
            max_suffix_link = len;
            max_suffix_node = v;
        }
    }
    
    int max_final_link = 0;
    int max_final_node = 0;
    
    for (int v = 1; v < bor.size(); v++) {
        int len = 0;
        int temp = v;
        while (temp != 0) {
            len++;
            temp = bor[temp].final_link;
        }
        if (len > max_final_link) {
            max_final_link = len;
            max_final_node = v;
        }
    }
    
#ifdef LOG
    cout << "Макс цепочка suffix_link: " << max_suffix_link << " (вершина " << max_suffix_node << ")" << endl;
    cout << "Макс цепочка final_link: " << max_final_link << " (вершина " << max_final_node << ")" << endl;
#endif
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
    
    build_bor();
    get_max_lens();
    
#ifdef LOG
    cout << "\nПОИСК" << endl;
    cout << "Текст: \"" << text << "\"" << endl;
#endif
    
    vector<target> results;
    int v = 0;
    
    for (int i = 0; i < text.length(); i++) {
        int c = char_to_idx(text[i]);
        
#ifdef LOG
        cout << "i=" << i << " '" << text[i] << "': ";
#endif
        
        v = go(v, c);
        
#ifdef LOG
        cout << "  состояние: " << v;
#endif
        
        int temp = v;
        while (temp != 0) {
            if (bor[temp].pattern_id != -1) {
                int start_pos = i - pattern_lengths[bor[temp].pattern_id] + 1;
#ifdef LOG
                cout << "  НАЙДЕНО: \"" << patterns[bor[temp].pattern_id] << "\" (поз." << start_pos + 1 << ")";
#endif
                results.push_back({start_pos + 1, bor[temp].pattern_id + 1});
            }
            temp = bor[temp].final_link;
        }
#ifdef LOG
        cout << endl;
#endif
    }
    
#ifdef LOG
    cout << "\nСортируем результат" << endl;
#endif
    sort(results.begin(), results.end());
    
    for (auto& res : results) {
        cout << res.pos << " " << res.pat << "\n";
    }
    
    return 0;
}