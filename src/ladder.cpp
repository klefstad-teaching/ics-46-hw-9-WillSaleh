#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

vector<string> current_ladder;
set<string> word_list;

void error(string word1, string word2, string msg) {
    cout << "Error: " << msg << "\n";
    cout << "Words: " << word1 << "\" and \"" << word2 << "\n";
}

//Initializes base case 
void initialize_dp(vector<int> & prev, int len1) {
    for (int i = 0; i <= len1; i++) {
        prev[i] = i;
    }
}

//Computes edit distance 
void compute_edit_distance(const string& str1, const string & str2, vector<int>& prev, vector<int>& curr, int j) {
    curr[0] = j;
    for (size_t i = 1; i <= str1.size(); i++) {
        if (str1[i - 1] == str2[j - 1]) {
            curr[i] = prev[i - 1];
        } else {
            curr[i] = 1 + min({prev[i], curr[i - 1], prev[i - 1]});
        }
    }
}

//Checks if any element exceeds the threshold
bool exceeds_threshold(const vector<int>& row, int d) {
    return *min_element(row.begin(), row.end()) > d;
}

//Checks if edit distance between str1 and str2 is within d
bool edit_distance_within(const std::string &str1, const std::string &str2, int d) {
    if (abs((int)str1.size() - (int)str2.size()) > d) {
        return false;
    }
    if (str1.size() > str2.size()) {
        return edit_distance_within(str2, str1, d);
    }

    vector<int> prev (str1.size() + 1), curr(str1.size() + 1);
    initialize_dp(prev, str1.size());

    for (int j = 1; j <= str2.size(); j++) {
        compute_edit_distance(str1, str2, prev, curr, j);
        if (exceeds_threshold(curr, d)) {
            return false;
        }
        swap(prev, curr);
    }
    return prev[str1.size()] <= d;
}

//Checks if two words are adjacent
bool is_adjacent(const string & word1, const string & word2) {
    return edit_distance_within(word1, word2, 1);
}

//Uses BFS
vector<string> generate_word_ladder(const string &begin_word, const string &end_word, const set<string> &word_list) {
    if (begin_word == end_word) {
        return {begin_word};
    }
    if (word_list.find(end_word) == word_list.end()) {
        return {};
    }

    queue<vector<string>> ladder_queue;
    set<string> visited;
    ladder_queue.push({begin_word});
    visited.insert(begin_word);

    while (!ladder_queue.empty()) {
        int level_size = ladder_queue.size();
        set<string> words_this_level;

        for (int i = 0; i < level_size; i++) {
            vector<string> ladder = ladder_queue.front();
            ladder_queue.pop();
            string last_word = ladder.back();

            for (const string& word : word_list) {
                if (visited.find(word) == visited.end() && is_adjacent(last_word, word)) {
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(word);

                    if (word == end_word) {
                        return new_ladder;
                    }
                    visited.insert(word);
                    ladder_queue.push(new_ladder);
                }
            }
        }
        for (const string& word : words_this_level) {
            visited.insert(word);
        }
    }
    return {};
}

//Convert string to lowercase
string to_lowercase(const string& word) {
    string lower_word;
    for (char ch : word) {
        lower_word += tolower(ch);
    }
    return lower_word;
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    
    if (!file) {
        cerr << "Error: Cannot open file " << file_name << endl;
        exit(1); 
    }

    string word;
    bool hasWords = false;

    while (file >> word) {
        word_list.insert(to_lowercase(word));
        hasWords = true;
    }

    file.close();

    if (!hasWords) {
        cerr << "Error: Dictionary file is empty!\n";
        exit(1); 
    }

    cout << "Loaded " << word_list.size() << " words.\n"; 
}

void print_word_ladder(const vector<string> &ladder) {
    if (ladder.empty()) {
        cout << "No ladder found" << endl;
        return;
    }
    auto it = ladder.begin();
    cout << *it;
    ++it;

    while (it != ladder.end()) {
        cout << " -> " << *it;
        ++it;
    }

    cout << endl;
}

void verify_word_ladder() {
    if (current_ladder.empty()) {
        cout << "Invalid ladder\n"; 
        return;
    }

    cout << "Verifying ladder: ";
    for (const auto& word : current_ladder) cout << word << " ";
    cout << endl;

    for (size_t i = 1; i < current_ladder.size(); i++) {
        if (word_list.find(current_ladder[i]) == word_list.end()) {
            cout << "Invalid ladder: Word \"" << current_ladder[i] << "\" not in the dictionary\n"; 
            return;
        }
    }

    for (size_t i = 0; i < current_ladder.size() - 1; i++) {
        if (!is_adjacent(current_ladder[i], current_ladder[i + 1])) {
            cout << "Invalid ladder: \"" << current_ladder[i] << "\" and \"" << current_ladder[i + 1] << "\" are not adjacent.\n";
            return;
        }
    }

    cout << "Valid word ladder!\n";
}