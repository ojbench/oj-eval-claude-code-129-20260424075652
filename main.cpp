#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

using namespace std;

struct VariableValue {
    bool is_int;
    long long int_val;
    string str_val;

    VariableValue() : is_int(true), int_val(0) {}
    VariableValue(long long v) : is_int(true), int_val(v) {}
    VariableValue(string v) : is_int(false), str_val(v) {}
};

struct VariableInfo {
    VariableValue value;
    int level;
};

unordered_map<string, vector<VariableInfo>> symbol_table;
vector<vector<string>> scope_stack;
int current_level = 0;

void indent() {
    current_level++;
    scope_stack.push_back({});
}

void dedent() {
    if (current_level == 0) return;
    for (const string& name : scope_stack.back()) {
        symbol_table[name].pop_back();
        if (symbol_table[name].empty()) {
            symbol_table.erase(name);
        }
    }
    scope_stack.pop_back();
    current_level--;
}

bool is_declared_in_current_scope(const string& name) {
    auto it = symbol_table.find(name);
    if (it == symbol_table.end()) return false;
    return it->second.back().level == current_level;
}

VariableInfo* get_variable(const string& name) {
    auto it = symbol_table.find(name);
    if (it == symbol_table.end()) return nullptr;
    return &it->second.back();
}

bool is_integer(const string& s) {
    if (s.empty()) return false;
    size_t start = 0;
    if (s[0] == '-' || s[0] == '+') {
        if (s.length() == 1) return false;
        start = 1;
    }
    for (size_t i = start; i < s.length(); ++i) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}

void solve() {
    int n;
    if (!(cin >> n)) return;
    scope_stack.push_back({}); // global scope

    for (int i = 0; i < n; ++i) {
        string cmd;
        if (!(cin >> cmd)) break;
        if (cmd == "Indent") {
            indent();
        } else if (cmd == "Dedent") {
            if (current_level == 0) {
                cout << "Invalid operation" << endl;
            } else {
                dedent();
            }
        } else if (cmd == "Declare") {
            string type, name, val_str;
            cin >> type >> name >> val_str;
            if (is_declared_in_current_scope(name)) {
                cout << "Invalid operation" << endl;
            } else {
                if (type == "int") {
                    if (is_integer(val_str)) {
                        symbol_table[name].push_back({VariableValue(stoll(val_str)), current_level});
                        scope_stack.back().push_back(name);
                    } else {
                        cout << "Invalid operation" << endl;
                    }
                } else {
                    symbol_table[name].push_back({VariableValue(val_str), current_level});
                    scope_stack.back().push_back(name);
                }
            }
        } else if (cmd == "Add") {
            string res_name, v1_name, v2_name;
            cin >> res_name >> v1_name >> v2_name;
            VariableInfo* res = get_variable(res_name);
            VariableInfo* v1 = get_variable(v1_name);
            VariableInfo* v2 = get_variable(v2_name);

            if (!res || !v1 || !v2 || v1->value.is_int != v2->value.is_int || res->value.is_int != v1->value.is_int) {
                cout << "Invalid operation" << endl;
            } else {
                if (v1->value.is_int) {
                    res->value.int_val = v1->value.int_val + v2->value.int_val;
                } else {
                    res->value.str_val = v1->value.str_val + v2->value.str_val;
                }
            }
        } else if (cmd == "SelfAdd") {
            string name, val_str;
            cin >> name >> val_str;
            VariableInfo* var = get_variable(name);
            if (!var) {
                cout << "Invalid operation" << endl;
            } else {
                if (var->value.is_int) {
                    if (is_integer(val_str)) {
                        var->value.int_val += stoll(val_str);
                    } else {
                        cout << "Invalid operation" << endl;
                    }
                } else {
                    var->value.str_val += val_str;
                }
            }
        } else if (cmd == "Print") {
            string name;
            cin >> name;
            VariableInfo* var = get_variable(name);
            if (!var) {
                cout << "Invalid operation" << endl;
            } else {
                cout << name << ":";
                if (var->value.is_int) cout << var->value.int_val;
                else cout << var->value.str_val;
                cout << endl;
            }
        } else {
            // Unknown command? The problem says "Invalid operation" for malformed instructions
            // But it doesn't specify if we should consume the rest of the line.
            // Let's assume commands are always one of the 6.
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
