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
    int suffix_link;
    int final_link;
    int parent;
    int pchar;
    vector<int> pattern_ids;
    
    node() {
        for (int i = 0; i < 5; i++) {
            next[i] = -1;
        }
        suffix_link = -1;
        final_link = -1;
        parent = -1;
        pchar = -1;
    }
};

vector<node> bor;
vector<string> fragments;
vector<int> fragment_pos;

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
        
        cout << u << " (suff=" << bor[u].suffix_link << ", final=" << bor[u].final_link;
        
        if (!bor[u].pattern_ids.empty()) {
            cout << ", terms=[";
            for (int j = 0; j < bor[u].pattern_ids.size(); j++) {
                if (j > 0) cout << ",";
                cout << bor[u].pattern_ids[j];
            }
            cout << "]";
        }
        cout << ")" << endl;
        
        if (last) print_tree(u, indent + "    ");
        else print_tree(u, indent + "│   ");
    }
}
#endif

void add_string(const string& s, int fragment_id) {
#ifdef LOG
    cout << "Добавляем фрагмент \"" << s << "\" (id=" << fragment_id << ")" << endl;
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
            cout << "  Новая вершина " << bor.size() - 1 << " по '" << ch << "'" << endl;
#endif
        }
        v = bor[v].next[c];
    }
    bor[v].pattern_ids.push_back(fragment_id);

#ifdef LOG
    cout << "  Вершина " << v << " терминал, фрагменты: [";
    for (int j = 0; j < bor[v].pattern_ids.size(); j++) {
        if (j > 0) cout << ",";
        cout << bor[v].pattern_ids[j];
    }
    cout << "]" << endl;
    cout << "Бор:" << endl;
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
    bor[0].suffix_link = 0;
    bor[0].final_link = 0;
    
    for (int c = 0; c < 5; c++) {
        if (bor[0].next[c] != -1) {
            int v = bor[0].next[c];
            bor[v].suffix_link = 0;
            bor[v].final_link = 0;
            q.push(v);
#ifdef LOG
            cout << "  Вершина " << v << ": suff=0, final=0, в очередь" << endl;
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
        cout << "  Вершина " << v << " (suff=" << bor[v].suffix_link << ")" << endl;
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
                cout << "    child=" << child << " по '" << idx_to_char(c) 
                     << "': suff=" << bor[child].suffix_link;
#endif
                
                int slink = bor[child].suffix_link;
                if (!bor[slink].pattern_ids.empty()) {
                    bor[child].final_link = slink;
#ifdef LOG
                    cout << ", final=" << slink << " (терминал)";
#endif
                } else if (slink == 0) {
                    bor[child].final_link = 0;
#ifdef LOG
                    cout << ", final=0";
#endif
                } else {
                    bor[child].final_link = bor[slink].final_link;
#ifdef LOG
                    cout << ", final=" << bor[child].final_link << " (от " << slink << ")";
#endif
                }
#ifdef LOG
                cout << endl;
#endif
            }
        }
    }

#ifdef LOG
    cout << "\n Бор построен:" << endl;
    print_tree();
#endif
}

int go(int v, int c) {
    if (bor[v].next[c] != -1) {
        return bor[v].next[c];
    }
    if (v == 0) {
        return 0;
    }
    return go(bor[v].suffix_link, c);
}

int main() {
    string text, pattern;
    char joker;
    
    getline(cin, text);
    getline(cin, pattern);
    cin >> joker;

#ifdef LOG
    cout << "Текст: \"" << text << "\"" << endl;
    cout << "Шаблон: \"" << pattern << "\"" << endl;
    cout << "Джокер: '" << joker << "'" << endl;
    cout << "Разбиваем шаблон на фрагменты" << endl;
#endif
    
    string current = "";
    
    for (int i = 0; i < pattern.length(); i++) {
        if (pattern[i] == joker) {
            if (!current.empty()) {
                fragments.push_back(current);
                fragment_pos.push_back(i - current.length());
#ifdef LOG
                cout << "Фрагмент \"" << current << "\" на позиции " << fragment_pos.back() << endl;
#endif
                current = "";
            }
        } else {
            current += pattern[i];
        }
    }

    if (!current.empty()) {
        fragments.push_back(current);
        fragment_pos.push_back(pattern.length() - current.length());
#ifdef LOG
        cout << "Фрагмент \"" << current << "\" на позиции " << fragment_pos.back() << endl;
#endif
    }

#ifdef LOG
    cout << "\nВсего фрагментов: " << fragments.size() << endl;
#endif
    
    bor.emplace_back();
    
    for (int i = 0; i < fragments.size(); i++) {
        add_string(fragments[i], i);
    }
    
    build_bor();
    
#ifdef LOG
    cout << "\nПОИСК" << endl;
#endif
    
    vector<int> matches(text.length() + 1, 0);
    int v = 0;
    
    for (int i = 0; i < text.length(); i++) {
        int c = char_to_idx(text[i]);
        
#ifdef LOG
        cout << "i=" << i << " '" << text[i] << "': ";
#endif
        
        v = go(v, c);
        
#ifdef LOG
        cout << "v=" << v << endl;
#endif
        
        int temp = v;
        while (temp != 0) {
            for (int frag_id : bor[temp].pattern_ids) {
                int frag_len = fragments[frag_id].length();
                int pattern_start = i - frag_len - fragment_pos[frag_id] + 1;
                
#ifdef LOG
                cout << "  фрагмент " << frag_id << " (\"" << fragments[frag_id] << "\"), pattern_start=" << pattern_start;
#endif
                
                if (pattern_start >= 0 && pattern_start + pattern.length() <= text.length()) {
                    matches[pattern_start]++;
#ifdef LOG
                    cout << " OK, matches[" << pattern_start << "]=" << matches[pattern_start];
#endif
                    
                    if (matches[pattern_start] == (int)fragments.size()) {
#ifdef LOG
                        cout << "\nНашлось совпадение всей стрчоки!" << endl;
#endif
                        cout << pattern_start + 1 << "\n";
                    }
                }
#ifdef LOG
                cout << endl;
#endif
            }
            temp = bor[temp].final_link;
        }
    }
    
    return 0;
}