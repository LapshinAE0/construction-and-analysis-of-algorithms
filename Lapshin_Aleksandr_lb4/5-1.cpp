#include <iostream>
#include <string>
#include <vector>

using namespace std;


vector<size_t> prefix_function(string& s) {

#ifdef LOG
    cout << "Запускаем префикс-функцию для строчки: " << s << endl;
#endif

    size_t n = s.length();
    vector<size_t> pi(n);

#ifdef LOG
    cout << "Всегда pi[0] = 0" << endl;
#endif

    for (size_t i = 1; i < n; i++) {
        size_t j = pi[i-1];
        
#ifdef LOG
    cout << "Итерация для индекса i = " << i << endl;
    cout << "Рассматриваем подстроку: \"" << s.substr(0, i+1) << "\"" << endl;
    cout << "Берем значение массива pi от прошлого элемента: j = pi[" << i-1 << "] = " << j << endl;
#endif

        while ((j > 0) && (s[i] != s[j])) {

#ifdef LOG
            cout << "  s[" << i << "]= " << s[i] << "  != s[" << j << "]= " << s[j] << " , откат: j = pi[" << j-1 << "] = " << pi[j-1] << endl;
#endif

            j = pi[j-1];
        }
        
        if (s[i] == s[j]) {
            j++;
#ifdef LOG
            cout << "  СОВПАДЕНИЕ: s[" << i << "] == s[" << j - 1 << "]" << endl;
            cout << "      увеличиваем j на 1, j = " << j << endl;
#endif
        } else {
#ifdef LOG
            cout << "  НЕТ СОВПАДЕНИЯ: s[" << i << "] != s[" << j << "]" << endl;
            cout << "      j остается прежним, j = " << j << endl;
#endif
        }


        pi[i] = j;

#ifdef LOG
        cout << "  pi[" << i << "] = " << j << endl;
        cout << "  Текущий pi: [";
        for (size_t k = 0; k <= i; k++) {
            cout << pi[k];
            if (k < i) cout << ", ";
        }
        cout << "]" << endl;
#endif
    }

#ifdef LOG
    cout << "Итоговая префикс-функция: [";
    for (size_t i = 0; i < n; i++) {
        cout << pi[i];
        if (i < n-1) cout << ", ";
    }
    cout << "]" << endl;
#endif
    return pi;
}



vector<size_t> CMP(string s, string t, vector<size_t> pi) {

#ifdef LOG
    cout << "\nЗапускаем алгоритм Кнута-Морриса-Пратта" << endl;
    cout << "Текст: \"" << s << "\"" << endl;
    cout << "Шаблон: \"" << t << "\"" << endl;
    cout << "Префикс-функция шаблона: [";
    for (size_t i = 0; i < pi.size(); i++) {
        cout << pi[i];
        if (i < pi.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
#endif

    size_t n_s = s.length();
    size_t n_t = t.length();
    vector<size_t> result;

    size_t j = 0;
    size_t i = 0;
    while (i < n_s) {

        if(s[i] == t[j]) {
          
#ifdef LOG
            cout << "  Элементы строчек совпали: s[" << i << "] == t[" << j << "]" << endl;
            cout << "      увеличиваем j на 1, j = " << j + 1 << endl;
            cout << "      увеличиваем i на 1, i = " << i + 1<< endl;
#endif

            i++;
            j++;

            if(j == n_t) {

#ifdef LOG
            cout << "  Долшли до конца строчки-паттерна: j = " << j << " == len(t) = " << n_t << endl;
            cout << "      Добавляем в result индекс, с которого строчка совпадает i - len(t) = " << i - n_t << endl;
            cout << "      Откатываем j, j = pi[j-1] = " << pi[j-1] << endl;
#endif

                result.push_back(i - n_t);
                j = pi[j-1];
            }
        }
        else {

#ifdef LOG
            cout << "  Элементы строчек не совпали: s[" << i << "] != t[" << j << "]" << endl;
#endif
            if(j != 0) {
#ifdef LOG
            cout << "  Так как j != 0, то откатываем j, j = pi[j-1]" << pi[j-1] << endl;
#endif
                j = pi[j-1];
            } 
            else {
#ifdef LOG
            cout << "  Так как j = 0, то увеличиваем i на 1, i = " << i + 1 << endl;
#endif
                i++;
            }
        }

    }

    return result;
}

void print_answer(vector<size_t> result) {



    size_t n = result.size();

    if (n == 0) {
#ifdef LOG
    cout << "Так как не найденны индексы вхождений, выводим -1" << endl;
#endif
        cout << "-1" << endl;
        return;
    }

#ifdef LOG
    cout << "Печатаем все найденные индексы вхождений" << endl;
#endif
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