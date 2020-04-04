#include <bits/stdc++.h>

using namespace std;

string defaultCode;

vector<string> split(const string &str, const string &delim) {
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

string validName(string f) {
    int fsize = f.size();
    f[0] = toupper(f[0]);
    for (int i = 1; i < fsize; i++) {
        f[i] = tolower(f[i]);
    }
    return f;
}

typedef struct phone_number {
    string code, number;

    phone_number() {

    }

    phone_number(string c, string n) {
        code = c;
        number = n;
    }

    phone_number(string n) {
        code = defaultCode;
        number = n;
    }

    void print() {
        cout << code << '-' << number;
    }
} phone_number;

typedef struct contact {
    string fname, sname;
    vector<phone_number> numbers;
    int correct = -1;

    contact(string f, string s) {
        fname = validName(f);
        sname = validName(s);
    }

    contact() {
    }

    void print() {
        if (correct != -1) {
            cout << fname << ' ' << sname << ": ";
            numbers[correct].print();
            cout << '\n';
        }
    }

    void addNumber(phone_number n) {
        numbers.push_back(n);
        print();
    }

    void set_correct(int j) {
        correct = j;
    }
} contact;

bool operator==(const contact &c1, const contact &c2) {
    return c1.fname.compare(c2.fname) == 0 && c1.sname.compare(c2.sname) == 0;
}

bool operator<(contact c1, contact c2) {
    int r = c1.sname.compare(c2.sname);
    if (r != 0)
        return r < 0;
    return c1.fname.compare(c2.fname) < 0;
}

int firstDigit(const string &s) {
    for (int i = 0; i < s.length(); i++) {
        if (isdigit(s[i]))
            return i;
    }
    return -1;
}

int firstLetter(const string &s) {
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i]))
            return i;
    }
    return -1;
}

int findDelimeter(const string &s) {
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '-' || s[i] == '/')
            return i;
    }
    return -1;
}

string additionWithSpace(string s1, string s2){
    ostringstream os;
    os << s1;
    os << " ";
    os << s2;
    return os.str();
}

int main() {
    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n >> defaultCode;
        scanf("\n");
        vector<contact> list;
        cin.clear();
        unordered_map<string, int> nameToInd;
        unordered_map<string, int> numberToIndices;
        unordered_set<string> name_num;
        int inserted = 0;
        for (int i = 0; i < n; i++) {
            string response;
            getline(cin, response);
            int ind_dig = firstDigit(response);
            int ind_letter = firstLetter(response);
            string numberpart, namepart;
            if (ind_dig < ind_letter) {
                numberpart = response.substr(0, ind_letter - 1);
                namepart = response.substr(ind_letter);
            } else {
                namepart = response.substr(0, ind_dig - 1);
                numberpart = response.substr(ind_dig);
            }
            string fname, sname;
            if (namepart.find(' ') != -1) {
                vector<string> r = split(namepart, " ");
                fname = r[0];
                sname = r[1];
            } else {
                vector<string> r = split(namepart, ",");
                fname = r[1];
                sname = r[0];
            }
            numberpart.erase(std::remove(numberpart.begin(), numberpart.end(), ' '), numberpart.end());
            int ind = findDelimeter(numberpart);
            contact c(fname, sname);
            phone_number p;
            if (ind != -1) {
                phone_number p1(numberpart.substr(0, ind), numberpart.substr(ind + 1));
                p = p1;
            } else {
                phone_number p1(numberpart);
                p = p1;
            }
            string keyName(additionWithSpace(c.fname, c.sname));
            int index = -1;
            if (nameToInd.find(keyName) != nameToInd.end()) {
                index = nameToInd[keyName];
                list[index].addNumber(p);
            } else {
                c.addNumber(p);
                index = inserted;
                inserted++;
                nameToInd[keyName] = index;
                list.push_back(c);
            }
            string keyNumber(additionWithSpace(p.code, p.number));

            if (name_num.find(additionWithSpace(keyName, keyNumber)) == name_num.end()) {
                if(numberToIndices.find(keyNumber) == numberToIndices.end()) {
                    numberToIndices[keyNumber] = index;
                }
                else if(index != numberToIndices[keyNumber] ){
                    numberToIndices[keyNumber] = 1 << 20;
                }
                name_num.insert(additionWithSpace(keyName, keyNumber));
            }
        }
        
        int good = 0;
        for (int i = 0; i < list.size(); i++) {
            vector<phone_number> nums = list[i].numbers;

            for (int j = nums.size() - 1; j >= 0; j--) {
                if(numberToIndices[additionWithSpace(nums[j].code, nums[j].number)] == i){
                    good++;
                    list[i].set_correct(j);
                    break;
                }
            }
        }
        cout << good << endl;
        sort(list.begin(), list.end());
        for(auto u : list){
            u.print();
        }
    }

    return 0;
}