#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <assert.h>

using namespace std;

struct Constraint{
    vector<uint32_t> variable_id;
    int32_t sum;
};

void output_blank_file(void); // 12 * 12 matrix

void write_solution(const vector<vector<string>> &matrix);

vector<string> split_by_dot(const string &str);

vector<vector<string>> read_from_txt(void);

void output_matrix(const vector<vector<string>> &matrix);
void output_matrix(const vector<vector<uint32_t>> &matrix);

vector<int32_t> get_available_values(const vector<vector<string>> &matrix);

void get_candidate_values_and_constraints(const vector<vector<string>> &matrix,
                                          vector<pair<uint32_t, uint32_t>> &candidate_value_coordinates,
                                          vector<vector<uint32_t>> &candidates_map,
                                          vector<Constraint> &constraints_vct);

void get_dfs_order(const uint32_t number_of_candidate, const vector<Constraint> &constraints_vct,
                   vector<uint32_t> &dfs_order,
                   vector<vector<Constraint>> &dfs_check_point,
                   vector<uint32_t> &id2index);

uint32_t number_of_available_value;
uint32_t number_of_candidate_value;
uint32_t number_of_constraint;

vector<int32_t> available_values;
vector<uint32_t> available_values_lock;

vector<pair<uint32_t, uint32_t>> candidate_value_coordinates;
vector<vector<uint32_t>> candidates_map;
vector<Constraint> constraints_vct;

vector<uint32_t> id2index;
vector<uint32_t> dfs_order;
vector<vector<Constraint>> dfs_check_point;

vector<vector<string>> matrix;

vector<int32_t> dfs_values;


void init() {
    matrix = read_from_txt();
    // output_matrix(matrix);

    available_values = get_available_values(matrix);
    // for (const auto &p : available_values) cout << p << " "; cout << endl;
    number_of_available_value = available_values.size();
    // cout << "number of available value = " << number_of_available_value << endl;
    available_values_lock.resize(number_of_available_value, 0);

    get_candidate_values_and_constraints(matrix, candidate_value_coordinates, candidates_map, constraints_vct);
//    output_matrix(candidates_map);
//    for (const auto &p : constraints_vct) {
//        cout << p.sum << " = ";
//        for (const auto &e : p.variable_id) cout << e << " ";
//        cout << endl;
//    }
    number_of_candidate_value = candidate_value_coordinates.size() - 1;
    assert(number_of_candidate_value == number_of_available_value);
    dfs_values.resize(number_of_candidate_value);
    number_of_constraint = constraints_vct.size();

    get_dfs_order(number_of_candidate_value, constraints_vct, dfs_order, dfs_check_point, id2index);

//    for (const auto &p : dfs_order) cout << p << " "; cout << endl;
//
//    for (uint32_t i = 0; i < number_of_candidate_value; i++) {
//        cout << i + 1 << " :" << endl;
//        for (auto &p : dfs_check_point[i]) {
//            cout << p.sum << " = ";
//            for (auto &e : p.variable_id)
//                cout << dfs_order[e] << " ";
//            cout << endl;
//        }
//    }
}


bool dfs(uint32_t depth) {
    bool ret = false;

    if (depth == number_of_candidate_value) return true;
    for (uint32_t i = 0; i < number_of_available_value; i++) {
        if (available_values_lock[i]) continue;
        available_values_lock[i] = true;
        dfs_values[depth] = available_values[i];
        bool flag = true;
        for (const auto &p : dfs_check_point[depth]) {
            if (!flag) break;
            int sum = 0;
            for (const auto &e : p.variable_id) sum += dfs_values[e];
            if (sum != p.sum) flag = false;
        }
        if (flag) {
            if (dfs(depth + 1))
                return true;
        }
        available_values_lock[i] = false;
    }
    return ret;
}


bool solve() {
    return dfs(0);
}

void output() {
    vector<vector<string>> out_matrix(12, vector<string> (12));
    for (uint32_t index = 0; index < number_of_candidate_value; index++) {
        uint32_t id = dfs_order[index];
        uint32_t x, y;
        tie(x, y) = candidate_value_coordinates[id];
        out_matrix[x][y] = to_string(dfs_values[index]);
    }
    for (uint32_t i = 0; i < 12; i++) {
        for (uint32_t j = 0; j < 12; j++) {
            if (isalpha(matrix[i][j].back())) {
                out_matrix[i][j] = matrix[i][j];
            }
        }
    }
    write_solution(out_matrix);
}

int main() {
    int32_t op;
    cout << "input file name : input_file.txt" << endl;
    cout << "output file name : solution.txt" << endl;
    cout << "U D L R is for these sum constraint block" << endl;
    cout << "U : up, D : down, L : left, R : right" << endl;
    cout << "for example" << endl;
    cout << "****puzzle****************" << endl;
    cout << "1   2  3  4  8L" << endl;
    cout << "7   2          " << endl;
    cout << "10U 2          " << endl;
    cout << "    8U         " << endl;
    cout << "**************************" << endl;
    cout << "\n\n\n" << endl;
    cout << "****solution**************" << endl;
    cout << "3   2 2 1 8L" << endl;
    cout << "7   2       " << endl;
    cout << "10U 4       " << endl;
    cout << "    8U       " << endl;
    cout << "**************************" << endl;
    cout << "input a operation code and press enter" << endl;
    cout << "-1 : exit" << endl;
    cout << "0 : generate a blank file" << endl;
    cout << "1 : solve" << endl;
    cin >> op;
    // op = 1;
    if (op == -1) return 0;
    else if (op == 0) output_blank_file();
    else if (op == 1) {
        init();
        uint32_t result = solve();
        if (!result) {
            cout << "no solution!" << endl;
            return 0;
        }
        output();
    } else {
        cout << "hemmmmmmmmmmmm......." << endl;
    }
    return 0;
}

void output_blank_file() { // 12 * 12 matrix
    ofstream out("input_file.txt", ios::out);
    assert(out.is_open());
    for (uint32_t i = 0; i < 12; i++) {
        for (uint32_t j = 1; j < 12; j++) {
            out << "      ,";
        }
        out << "      " << endl;
    }
    out.close();
    out.clear();
}

vector<string> split_by_dot(const string &str) {
    vector<string> ret;
    string tmp;
    for (auto c : str) {
        if (c == ' ') continue;
        if (c == ',') {
            ret.emplace_back(tmp);
            tmp.clear();
        } else {
            tmp += c;
        }
    }
    ret.emplace_back(tmp);
    assert(ret.size() == 12); // assure 12 column each row
    return ret;
}

vector<vector<string>> read_from_txt() {
    vector<vector<string>> ret;
    ifstream in("input_file.txt", ios::in);
    assert(in.is_open());
    while (true) {
        string tmp;
        getline(in, tmp);
        while (tmp.back() == '\r' || tmp.back() == '\n')
            tmp.pop_back();
        if (tmp.empty()) break;
        ret.emplace_back(split_by_dot(tmp));
    }
    in.close();
    in.clear();
    assert(ret.size() == 12); // assure that 12 row
    return ret;
}

void write_solution(const vector<vector<string>> &matrix) {
    ofstream out("solution.txt", ios::out);
    for (uint32_t i = 0; i < 12; i++) {
        for (uint32_t j = 0; j < 12; j++) {
            if (j != 0) out << ",";
            out << setw(6) << matrix[i][j];
        }
        out << endl;
    }
    out.close();
    out.clear();
}

void output_matrix(const vector<vector<string>> &matrix) {
    for (uint32_t i = 0; i < 12; i++) {
        for (uint32_t j = 0; j < 12; j++)
            cout << setw(6) << matrix[i][j] << " ";
        cout << endl;
    }
}

void output_matrix(const vector<vector<uint32_t>> &matrix) {
    for (uint32_t i = 0; i < 12; i++) {
        for (uint32_t j = 0; j < 12; j++)
            cout << setw(6) << ((matrix[i][j] == 0) ? "" : to_string(matrix[i][j])) << " ";
        cout << endl;
    }
}

vector<int32_t> get_available_values(const vector<vector<string>> &matrix) {
    vector<int32_t> ret;
    for (const auto &p : matrix) {
        for (const auto &e : p) {
            if (e.empty()) continue;
            if (isalpha(e.back())) continue;
            ret.emplace_back(stoi(e));
        }
    }
    return ret;
}

void get_candidate_values_and_constraints(const vector<vector<string>> &matrix,
                                          vector<pair<uint32_t, uint32_t>> &candidate_value_coordinates,
                                          vector<vector<uint32_t>> &candidates_map,
                                          vector<Constraint> &constraints_vct) {
    uint32_t cur_id = 0;
    candidates_map.resize(12, vector<uint32_t> (12, 0));
    for (uint32_t i = 0; i < 12; i++) {
        for (uint32_t j = 0; j < 12; j++) {
            auto &e = matrix[i][j];
            if (e.empty()) continue;
            if (isalpha(e.back())) continue;
            candidates_map[i][j] = ++cur_id;
        }
    }
    candidate_value_coordinates.resize(cur_id + 1);
    for (uint32_t i = 0; i < 12; i++)
        for (uint32_t j = 0; j < 12; j++)
            if (candidates_map[i][j] != 0)
                candidate_value_coordinates[candidates_map[i][j]] = make_pair(i, j);

    map<char, pair<int32_t, int32_t>> direction;
    direction['L'] = make_pair(0, -1);
    direction['R'] = make_pair(0, 1);
    direction['U'] = make_pair(-1, 0);
    direction['D'] = make_pair(1, 0);
    for (int32_t i = 0; i < 12; i++) {
        for (int32_t j = 0; j < 12; j++) {
            if (matrix[i][j].empty() || !isalpha(matrix[i][j].back())) continue;
            auto e = matrix[i][j];
            int dx, dy;
            tie(dx, dy) = direction[e.back()];
            e.pop_back();
            Constraint constraint;
            constraint.sum = stoi(e);
            int x = i + dx;
            int y = j + dy;
            while (0 <= x && x < 12 && 0 <= y && y < 12 && candidates_map[x][y] != 0) {
                constraint.variable_id.emplace_back(candidates_map[x][y]);
                x += dx;
                y += dy;
            }
            constraints_vct.emplace_back(constraint);
        }
    }
}

void get_dfs_order(const uint32_t number_of_candidates, const vector<Constraint> &constraints_vct,
                   vector<uint32_t> &dfs_order,
                   vector<vector<Constraint>> &dfs_check_point,
                   vector<uint32_t> &id2index) {

    id2index.resize(number_of_candidates);
    dfs_check_point.resize(number_of_candidates);

    vector<uint32_t> constraint_satisfied(constraints_vct.size(), 0);
    set<uint32_t> fixed_candidate_id;
    for (uint32_t t = 0; t < constraints_vct.size(); t++) {
        uint32_t tmp_constraint = 0;
        uint32_t tmp_cost = UINT32_MAX;
        for (uint32_t i = 0; i < constraints_vct.size(); i++) {
            if (constraint_satisfied[i]) continue;
            uint32_t cost = 0;
            for (const auto &p : constraints_vct[i].variable_id)
                if (!fixed_candidate_id.count(p))
                    cost++;
            if (cost < tmp_cost) {
                tmp_constraint = i;
                tmp_cost = cost;
            }
        }
        constraint_satisfied[tmp_constraint] = true;
        for (const auto &p : constraints_vct[tmp_constraint].variable_id) {
            if (!fixed_candidate_id.count(p)) {
                dfs_order.emplace_back(p);
                id2index[p] = dfs_order.size() - 1;
                fixed_candidate_id.insert(p);
            }
        }
        Constraint cur_constraint = constraints_vct[tmp_constraint];
        for (auto &p : cur_constraint.variable_id)
            p = id2index[p];
        dfs_check_point[dfs_order.size() - 1].emplace_back(cur_constraint);
    }
    for (uint32_t i = 1; i <= number_of_candidates; i++) {
        if (!fixed_candidate_id.count(i))
            dfs_order.emplace_back(i);
        id2index[i] = dfs_order.size() - 1;
        fixed_candidate_id.insert(i);
    }
}
