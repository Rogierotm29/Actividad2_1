#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

/* ===== KMP (usa LPS) ===== */
vector<int> buildLPS(const string& pat){
    vector<int> lps(pat.size(), 0);
    int len = 0;
    for(size_t i = 1; i < pat.size(); ){
        if(pat[i] == pat[len]) lps[i++] = ++len;
        else if(len) len = lps[len - 1];
        else lps[i++] = 0;
    }
    return lps;
}

vector<int> kmpSearch(const string& txt, const string& pat){
    if(pat.empty()) return {};
    vector<int> lps = buildLPS(pat), pos;
    size_t i = 0, j = 0;
    while(i < txt.size()){
        if(txt[i] == pat[j]){
            i++; j++;
            if(j == pat.size()){ pos.push_back(int(i - j)); j = lps[j - 1]; }
        }else if(j) j = lps[j - 1];
        else i++;
    }
    return pos;
}

/* ===== Z-Algorithm (Z-array) ===== */
vector<int> zFunction(const string& s){
    vector<int> Z(s.size(), 0);
    int L = 0, R = 0;
    for(size_t i = 1; i < s.size(); ++i){
        if((int)i <= R) Z[i] = min(R - (int)i + 1, Z[i - L]);
        while(i + Z[i] < s.size() && s[Z[i]] == s[i + Z[i]]) Z[i]++;
        if((int)i + Z[i] - 1 > R){ L = (int)i; R = (int)i + Z[i] - 1; }
    }
    return Z;
}

vector<int> zSearch(const string& txt, const string& pat){
    if(pat.empty()) return {};
    string s = pat + '$' + txt; // '$' separador
    vector<int> Z = zFunction(s), pos;
    for(size_t i = pat.size() + 1; i < s.size(); ++i)
        if(Z[i] == (int)pat.size()) pos.push_back(int(i - pat.size() - 1));
    return pos;
}

/* ===== Manacher (palíndromo más largo) ===== */
string longestPalSubstring_Manacher(const string& s){
    if(s.empty()) return "";
    string t = "^";
    for(char c : s){ t += '#'; t += c; }
    t += "#$";

    vector<int> P(t.size(), 0);
    int C = 0, R = 0;
    for(size_t i = 1; i < t.size() - 1; ++i){
        size_t mirror = 2*C - i;
        if((int)R > (int)i) P[i] = min(R - (int)i, P[mirror]);
        while(t[i + 1 + P[i]] == t[i - 1 - P[i]]) P[i]++;
        if((int)i + P[i] > R){ C = (int)i; R = (int)i + P[i]; }
    }
    int maxLen = 0, center = 0;
    for(size_t i = 1; i < P.size() - 1; ++i)
        if(P[i] > maxLen){ maxLen = P[i]; center = (int)i; }
    int start = (center - maxLen) / 2;
    return s.substr(start, maxLen);
}

/* ===== Suffix Array (naive) ===== */
vector<int> suffixArrayNaive(const string& s){
    vector<pair<string,int>> suf;
    suf.reserve(s.size());
    for(size_t i = 0; i < s.size(); ++i) suf.emplace_back(s.substr(i), (int)i);
    sort(suf.begin(), suf.end(), [](const auto& a, const auto& b){ return a.first < b.first; });
    vector<int> sa; sa.reserve(suf.size());
    for(auto& p : suf) sa.push_back(p.second);
    return sa;
}

/* ===== CLI ===== */
void runKMP(){
    cout << "\n[KMP] Ingresa texto: ";
    string txt; getline(cin, txt);
    cout << "[KMP] Ingresa patron: ";
    string pat; getline(cin, pat);

    auto pos = kmpSearch(txt, pat);
    cout << "Indices: ";
    if(pos.empty()) cout << "(ninguno)\n";
    else{ for(size_t i = 0; i < pos.size(); ++i){ if(i) cout << ", "; cout << pos[i]; } cout << "\n"; }

    auto lps = buildLPS(pat);
    cout << "LPS: [";
    for(size_t i = 0; i < lps.size(); ++i){ cout << lps[i] << (i+1<lps.size()? " ":""); }
    cout << "]\n";
}

void runZ(){
    cout << "\n[Z] Ingresa texto: ";
    string txt; getline(cin, txt);
    cout << "[Z] Ingresa patron: ";
    string pat; getline(cin, pat);

    auto pos = zSearch(txt, pat);
    cout << "Indices: ";
    if(pos.empty()) cout << "(ninguno)\n";
    else{ for(size_t i = 0; i < pos.size(); ++i){ if(i) cout << ", "; cout << pos[i]; } cout << "\n"; }

    string cat = pat + '$' + txt;
    auto Z = zFunction(cat);
    cout << "Z(\"" << cat << "\"): [";
    for(size_t i = 0; i < Z.size(); ++i){ cout << Z[i] << (i+1<Z.size()? " ":""); }
    cout << "]\n";
}

void runManacher(){
    cout << "\n[Manacher] Ingresa cadena: ";
    string s; getline(cin, s);
    string pal = longestPalSubstring_Manacher(s);
    cout << "Palindromo mas largo: \"" << pal << "\" (len " << pal.size() << ")\n";
}

void runSuffixArray(){
    cout << "\n[Suffix Array] Ingresa cadena: ";
    string s; getline(cin, s);
    auto sa = suffixArrayNaive(s);
    cout << "SA: [";
    for(size_t i = 0; i < sa.size(); ++i){ cout << sa[i] << (i+1<sa.size()? " ":""); }
    cout << "]\nSufijos ordenados:\n";
    for(int idx : sa) cout << idx << ": " << s.substr((size_t)idx) << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(&cout);

    while(true){
        cout << "\n==== Menu ====\n"
                "1) KMP con LPS\n"
                "2) Z-Algorithm\n"
                "3) Manacher\n"
                "4) Suffix Array (naive)\n"
                "0) Salir\n"
                "Opcion: ";
        int op;
        if(!(cin >> op)) return 0;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(op == 0) break;
        else if(op == 1) runKMP();
        else if(op == 2) runZ();
        else if(op == 3) runManacher();
        else if(op == 4) runSuffixArray();
        else cout << "Opcion invalida.\n";
    }
    return 0;
}
