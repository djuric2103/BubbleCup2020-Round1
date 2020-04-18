#include <bits/stdc++.h>
#define MAX 1e9
using namespace std;

string defaultCode;

struct pairhash {
public:
    template <typename T, typename U>
    size_t operator()(const pair<T, U> &x) const{
        return hash<T>()(x.first) ^ hash<U>()(x.second);
    }
    template <typename T, typename U, typename V>
    size_t operator()(const pair<pair<T, U >, V> &x) const{
        return hash<T>()(x.first.first) ^ hash<U>()(x.first.second) ^ hash<V>()(x.second);
    }
    template <typename T, typename U, typename V, typename Q>
    size_t operator()(const pair<pair<T, U >, pair<V, Q>> &x) const{
        return hash<T>()(x.first.first) ^ hash<U>()(x.first.second) ^ hash<V>()(x.second.first) ^ hash<Q>()(x.second.second);
    }
};

string validName(string f) {
    int fsize = f.size();
    f[0] = toupper(f[0]);
    for (int i = 1; i < fsize; ++i) {
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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n >> defaultCode;
        vector<contact> list;
        cin.ignore();
        unordered_map<pair<string, string>, int, pairhash> nameToInd;
        unordered_map<pair<string, string>, int, pairhash> numberToIndices;
        unordered_set<pair<pair<string,string>, pair<string, string>>, pairhash> name_num;
        int inserted = 0;
        for (int i = 0; i < n; ++i) {
            string response;
            getline(cin, response);
            int ind_dig = response.find_first_of("0123456789");
            int ind_letter = response.find_first_not_of("0123456789/,- ");
            string numberpart, namepart;
            if (ind_dig < ind_letter) {
                numberpart = response.substr(0, ind_letter - 1);
                namepart = response.substr(ind_letter);
            } else {
                namepart = response.substr(0, ind_dig - 1);
                numberpart = response.substr(ind_dig);
            }
            string fname, sname;
            int del_ind = namepart.find(' ');
            if (del_ind != -1) {
                fname = namepart.substr(0, del_ind);
                sname = namepart.substr(del_ind + 1);
            } else {
                del_ind = namepart.find(',');
                sname = namepart.substr(0, del_ind);
                fname = namepart.substr(del_ind + 1);
            }

            numberpart.erase(remove(numberpart.begin(), numberpart.end(), ' '), numberpart.end());
            int ind = numberpart.find_first_of("-/");
            contact c(fname, sname);
            phone_number p;
            if (ind != -1) {
                phone_number p1(numberpart.substr(0, ind), numberpart.substr(ind + 1));
                p = p1;
            } else {
                phone_number p1(numberpart);
                p = p1;
            }
            pair<string, string> keyName = {c.fname, c.sname};
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
            pair<string, string> keyNumber = {p.code, p.number};

            if (name_num.find({keyName, keyNumber}) == name_num.end()) {
                if(numberToIndices.find(keyNumber) == numberToIndices.end()) numberToIndices[keyNumber] = index;
                else if(index != numberToIndices[keyNumber] ) numberToIndices[keyNumber] = MAX;
                name_num.insert({keyName, keyNumber});
            }
        }

        int good = 0;
        for (int i = 0; i < list.size(); ++i) {
            vector<phone_number> nums = list[i].numbers;

            for (int j = nums.size() - 1; j >= 0; --j) {
                if(numberToIndices[{nums[j].code, nums[j].number}] == i){
                    good++;
                    list[i].set_correct(j);
                    break;
                }
            }
        }
        cout << good << "\n";
        sort(list.begin(), list.end());
        for(int i = 0; i < list.size(); list[i].print(), ++i);
    }
    return 0;
}
