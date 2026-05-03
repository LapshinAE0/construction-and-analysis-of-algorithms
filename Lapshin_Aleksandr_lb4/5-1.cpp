#include <iostream>
#include <string>
#include <vector>

using namespace std;


vector<size_t> prefix_function(string s) {
    size_t n = s.length();
    vector<size_t> pi(n);
    
    for (size_t i = 1; i < n; i++) {
        size_t j = pi[i-1];
        
        while ((j > 0) && (s[i] != s[j]))
            j = pi[j-1];
        
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

vector<size_t> CMP(string s, string t, vector<size_t> pi) {
    size_t n_s = s.length();
    size_t n_t = t.length();
    vector<size_t> result;

    size_t j = 0;
    size_t i = 0;
    while (i < n_s) {

        if(s[i] == t[j]) {
            i++;
            j++;

            if(j == n_t) {
                result.push_back(i - n_t);
                j = pi[j-1];
            }
        }
        else {
            if(j != 0) {
                j = pi[j-1];
            } 
            else {
                i++;
            }
        }

    }

    return result;
}

void print_answer(vector<size_t> result) {
    size_t n = result.size();

    if (n == 0) {
        cout << "-1" << endl;
        return;
    }

    for (size_t i = 0; i < n; i++) {
        cout << result[i];
        if (i != result.size() - 1) cout << ",";
    }
    cout << endl;
}

int main() {
    string t;
    string p;
    getline(cin, p);
    getline(cin, t);

    if ((t.empty() || p.empty()) && !(t.empty() && p.empty())) {
        cout << -1 << endl;
        return 0;
    } else if(t.empty() && p.empty()) {
        cout << 0 << endl;
        return 0;
    }
    
    vector<size_t> pref = prefix_function(p);

    vector<size_t> result = CMP(t, p, pref);

    print_answer(result);

    return 0;
}