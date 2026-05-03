#include <iostream>
#include <string>
#include <vector>

using namespace std;


vector<size_t> prefix_function(string& s) {
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

int CMP(string& s, string& t, vector<size_t>& pi) {
    size_t n_s = s.length();
    size_t n_t = t.length();

    size_t j = 0;
    size_t i = 0;
    while (i < n_s * 2) {

        if(s[i % n_s] == t[j]) {
            i++;
            j++;

            if(j == n_t) {
                return (i - n_t);
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

    return -1;
}

int main() {
    string t;
    string p;
    cin >> p >> t;

    if (t.length() != p.length()) {
        cout << -1 << endl;
        return 0;
    }

    if (t.empty()) {
        cout << 0 << endl;
        return 0;
    }

    vector<size_t> pref = prefix_function(t);

    cout << CMP(p, t, pref) << endl;

    return 0;
}
