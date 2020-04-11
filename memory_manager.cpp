#include <bits/stdc++.h>

using namespace std;
typedef struct treap_node *p_treap_node;
typedef struct list_node *p_list_node;
std::unordered_map<p_list_node , reference_wrapper<p_treap_node>> listToTreap;

struct treap_node {
    long long key, prior;
    treap_node *l, *r;
    p_list_node list_node;

    treap_node() {}

    treap_node(long long key, long long prior, p_list_node list_node, long long asd) : key(key), prior(prior), l(NULL), r(NULL),
                                                                     list_node(list_node) {
    }
};

struct list_node {
    long long index;
    long long size;
    bool deleted;
    list_node *prev, *next;

    list_node() {};

    list_node(long long index, long long size, bool deleted) : index(index), size(size), prev(NULL), next(NULL),
                                                   deleted(deleted) {};

};

void put_in_map(p_treap_node &treap_node) {
    if(treap_node == NULL) return;
    listToTreap.erase(treap_node -> list_node);
    reference_wrapper<p_treap_node > a(treap_node);
    listToTreap.insert({treap_node -> list_node, a});
}

struct Treap {
    p_treap_node notFound;
    p_treap_node root;

    Treap() {
        notFound = new treap_node(-1,-1, NULL, 1);//t.create_and_insert_node(-1, -1, NULL);
        root = NULL;
    }

private:
    void split(p_treap_node t, long long key, p_treap_node &l, p_treap_node &r) {
        if (!t)
            l = r = NULL;
        else if (key < t->key) {
            split(t->l, key, l, t->l);
            r = t;
            put_in_map(r);
        }else {
            split(t->r, key, t->r, r);
            l = t;
            put_in_map(l);
        }
    }

    void insert(p_treap_node &t, p_treap_node it) {
        if (!t) {
            t = it;
            put_in_map(t);
        }else if (it->prior < t->prior) {
            split(t, it->key, it->l, it->r);
            t = it;
            put_in_map(t);
        }
        else insert(it->key < t->key ? t->l : t->r, it);
    }


    void merge(p_treap_node &t, p_treap_node l, p_treap_node r) {
        if (l == NULL || r == NULL) {
            t = l ? l : r;
            put_in_map(t);
        }
        else if (l->prior < r->prior) {
            merge(l->r, l->r, r);
            t = l;
            put_in_map(t);
        }else {
            merge(r->l, l, r->l);
            t = r;
            put_in_map(t);
        }
    }

    void print(p_treap_node curr) {
        if (curr) {
            print(curr->l);
            cout << curr->key << '\t' << curr->prior << endl;
            print(curr->r);
        }
    }

    void print1(p_treap_node curr) {
        if (curr) {
            cout << (curr->key) << '\t' << (curr->prior) << endl;
            print1(curr->l);
            print1(curr->r);
        }
    }

    p_treap_node &getFirst(p_treap_node &curr, long long size) {
        if (!curr) {
            return notFound;
        }
        if (curr->key >= size) return curr;
        return getFirst(curr->r, size);
    }

public:
    p_treap_node &getFirst(long long size) {
        getFirst(root, size);
    }
    void erase_and_join(p_treap_node &s, p_list_node l_node) {
        erase(s);
        create_and_insert_node(l_node);
    }

    void erase_and_join(p_list_node listNode, p_list_node l_node) {
        p_treap_node &s = (listToTreap.find(listNode-> prev) -> second).get();
        erase(s);
        //printLRR();
        //cout << endl;
        //printRLR();
        //cout << endl;
        p_treap_node &p = (listToTreap.find(listNode-> next) -> second).get();
        erase(p);
        //printLRR();
        //cout << endl;
        //printRLR();
        //cout << endl;
        create_and_insert_node(l_node);
        //printLRR();
        //cout << endl;
        //printRLR();
        //cout << endl;
    }

    void erase(p_treap_node &s) {
        listToTreap.erase(s->list_node);
        merge(s, s->l, s->r);
    }

    void printLRR() {
        print(root);
    }

    void printRLR() {
        print1(root);
    }

    void create_and_insert_node(p_list_node list_node) {
        p_treap_node s = new treap_node(list_node->size, list_node->index, list_node, 0);
        insert(root, s);
    }
};


bool check_list();

Treap t;
p_list_node head;
p_list_node last;
unordered_map<string, p_list_node> nameToList;
long long left_space, disk_size;

long long returnNUM(string s) {
    long long ind = s.find_first_of('b') - 1;
    long long siz = stoi(s.substr(0, ind));
    switch (s[ind]) {
        case 'K':
            return siz;
        case 'M':
            return 1024 * siz;
        case 'G':
            return 1024 * 1024 * siz;
    }
}

void add(double &curr_size, double segment_size, double &left) {
    double to_add = min(segment_size - curr_size, left);
    curr_size += to_add;
    left -= to_add;
}

void precentage(long long empty, long long full) {
    double ratio = ((double)full) / ((double) (empty + full));
    //cout << ratio;
    if (ratio >= 0.75)
        cout << "#";
    else if (ratio < 0.25)
        cout << " ";
    else
        cout << "-";
}

void print_disk() {
    double size_of_segment = disk_size / 8.0;
    long long k = 8;
    p_list_node curr = head;
    double left_empty = 0, left_full = 0;
    while (k--) {
        cout << "[";
        double empty = 0, full = 0;
        for (; empty + full != size_of_segment; curr = curr->next) {
            //if(curr -> size < 0)
            //    cout << "HERE" << endl;
            if (left_empty != 0) {
                add(empty, size_of_segment - full, left_empty);
            } else if (left_full != 0) {
                add(full, size_of_segment - empty, left_full);
            }
            if (empty + full == size_of_segment) break;
            //if(curr -> size < 0)
            //    cout << "HERE" << endl;
            if (curr->deleted) {
                left_empty = curr->size;
                add(empty, size_of_segment - full, left_empty);
            } else {
                left_full = curr->size;
                add(full, size_of_segment - empty, left_full);
            }
        }
        precentage(empty, full);
        cout << "]";
    }
    cout << "\n";
}

void add_in_list(p_list_node old, p_list_node first, p_list_node second) {
    if (first != second) {
        //cout << 1 << endl;
        first->next = second;
        second->prev = first;
    }
    //cout << old -> size << endl;
    //cout << "OUT"<<endl;
    first->prev = old->prev;
    second->next = old->next;
    //cout << "OUT2"<<endl;

    if (old->prev == NULL) {
        //cout << 2 << endl;
        head = first;
    } else {
        //cout << 3 << endl;
        first->prev->next = first;
    }
    if (old->next != NULL) {
        //cout << 4 << endl;
        second->next->prev = second;
    }
}

p_list_node mergeNodes(p_list_node first, p_list_node second) {
    p_list_node newNode = new list_node(first->index,
                                        first->size + second->size + (first->next != second ? first->next->size : 0),
                                        true);
    newNode->prev = first->prev;
    newNode->next = second->next;
    if (newNode->prev == NULL) {
        head = newNode;
    } else {
        newNode->prev->next = newNode;
    }
    if (newNode->next != NULL) {
        newNode->next->prev = newNode;
    }
    return newNode;
}

void print_list() {
    stack<p_list_node > sta;
    for (p_list_node curr = head; curr != NULL; curr = curr->next) {
        cout << "SIZE" << curr->size << "\tIndex" << curr->index << "\tDeleted" << curr->deleted << endl;
        sta.push(curr);
    }
    for(p_list_node curr = last;  curr != NULL; curr = curr -> prev){
        if(curr != sta.top()){
            cout << "GRSKA RAZLICITE LISTE \n";
        }
        sta.pop();
    }
    if(!sta.empty()){
        cout << "GRSKA RAZLICITE LISTE 1\n";

    }

    cout << endl;
    t.printLRR();
    cout << endl;
    t.printRLR();
    cout << "____________________________________________\n";
}

void insert(string name, long long length, p_treap_node &t_node) {
    p_list_node listNode = t_node->list_node;
    long long totalSize = t_node->key;
    p_list_node full = new list_node(t_node->prior, length, false);
    nameToList.insert({name, full});
    t.erase(t_node);

    //cout << "\t" << totalSize << '\t' << length << endl;
    if (totalSize == length) {
        add_in_list(listNode, full, full);
    } else {
        p_list_node empty = new list_node(full->index + length, totalSize - length, true);
        //cout << empty -> size << '\t'  << empty -> index << endl;
        t.create_and_insert_node(empty);
        //cout << "\nOUT" << endl;
        //cout << "IN TREAP: " << listToTreap.at(empty) -> key << "\t" << listToTreap.at(empty) -> prior <<endl;
        add_in_list(listNode, full, empty);
        //cout << "ADDED"<<endl;
    }

    //print_list();
}

void removeSeg(p_list_node listNode) {
    //cout << "REMOVE"<< listNode -> size << '\t' << listNode -> index << endl;
    if ((listNode->prev == NULL || listNode->prev->deleted == false) && (listNode->next == NULL || listNode->next->deleted == false)) {
        //listNode->deleted = true;
        p_list_node newNode = new list_node(listNode -> index, listNode -> size, true);
        add_in_list(listNode, newNode, newNode);
        t.create_and_insert_node(newNode);
    } else if ((listNode->prev != NULL && listNode->prev->deleted == true) && (listNode->next != NULL && listNode->next->deleted == true)) {

        //cout << "PREVI "<<prevInTreap -> prior << "\t Next in treap " << nextInTreap -> prior << endl;
        p_list_node newNode = mergeNodes(listNode->prev, listNode->next);
        //cout << "HERE" << endl;
        t.erase_and_join(listNode, newNode);
    } else if (listNode->prev != NULL && listNode->prev->deleted == true) {
        p_treap_node &prevInTreap = (listToTreap.find(listNode->prev) ->second).get();
        p_list_node newNode = mergeNodes(listNode->prev, listNode);
        t.erase_and_join(prevInTreap, newNode);
    } else if (listNode->next != NULL && listNode->next->deleted == true) {
        p_treap_node &nextInTreap = (listToTreap.find(listNode->next)->second).get();
        p_list_node newNode = mergeNodes(listNode, listNode->next);
        t.erase_and_join(nextInTreap, newNode);
    }
    //print_list();
}


void delete_from_list(p_list_node l_node) {
    if (l_node->prev == NULL) {
        head = l_node->next;
    }else{
        l_node -> prev -> next = l_node -> next;
    }
    l_node -> next -> prev = l_node -> prev;
}

void addRestPriorLast() {
    p_list_node new_node = new list_node(last -> prev -> index + last -> prev -> size, left_space, true);
    last -> index = new_node -> index + new_node -> size +1;
    new_node -> prev = last -> prev;
    new_node -> prev -> next = new_node;
    new_node -> next = last;
    last -> prev = new_node;
    t.create_and_insert_node(new_node);
}

void treap_optimize_pass(p_treap_node p) {
    if (p == NULL) return;

    delete_from_list(p->list_node);
    treap_optimize_pass(p->l);
    treap_optimize_pass(p->r);
}

void optimize() {
    if (left_space == disk_size || left_space == 0) return;
    treap_optimize_pass(t.root);
    t = Treap();
    listToTreap = std::unordered_map<p_list_node , reference_wrapper<p_treap_node>> ();
    addRestPriorLast();
}

int main() {
    long long n;
    while (1) {
        cin >> n;
        if (n == 0) break;
        string a;
        cin >> a;
        disk_size = returnNUM(a);
        left_space = disk_size;
        t = Treap();
        nameToList = unordered_map<string, p_list_node>();
        listToTreap = std::unordered_map<p_list_node , reference_wrapper<p_treap_node>> ();
        head = new list_node(0, disk_size, true);
        last = new list_node(disk_size+1, 0, false);
        head -> next = last;
        last -> prev = head;
        t.create_and_insert_node(head);
        //listToTreap.insert({head, t.getFirst(0)});
        long long i = 0;
        for (; i < n; i++) {
            //print_list();
            //cout << i << endl;
            //if(i == 3)
            //  cout << "Here" << endl;
//            if(i == 94)
//                cout << "HERE" << endl;
           // if(!check_list()){
             //   cout << "ERROR WITH INDICES " << i << endl;
           // }
            string op;
            cin >> op;
            if (op.compare("insere") == 0) {
                string name;
                cin >> name;
                string t_len;
                cin >> t_len;

                long long length = returnNUM(t_len);

                if (left_space - length < 0) {
                    cout << "ERRO: disco cheio\n";
                    break;
                }
                p_treap_node& s = t.getFirst(length);
                //cout << s -> key << endl;
                if (s == t.notFound) {//|| s -> key < length){
                    //cout << "OPTIMIZATION" << endl;
                    optimize();
                    //print_list();
                    p_treap_node &s1 = t.getFirst(length);
                    insert(name, length, s1);
                } else {
                    insert(name, length, s);
                }
                left_space -= length;

                continue;
            }
            if (op.compare("remove") == 0) {
                string name;
                cin >> name;
                if (nameToList.find(name) == nameToList.end()) continue;
                p_list_node p = nameToList.find(name)->second;
                left_space += p->size;
                removeSeg(p);
                nameToList.erase(name);
                continue;
            }
            /*if(i == 16)
                cout << "HERE" << endl;*/
            optimize();
        }
        //print_list();
        if (i == n) print_disk();
        for(; i<n; i++){
            string temp;
            getline(std::cin, temp);
        }
    }

    return 0;
}

bool check_list() {
    stack<p_list_node > sta;
    for (p_list_node curr = head; curr != NULL; curr = curr->next) {
        sta.push(curr);
    }
    for(p_list_node curr = last;  curr != NULL; curr = curr -> prev){
        if(curr != sta.top()){
            cout << "GRSKA RAZLICITE LISTE \n";
        }
        sta.pop();
    }
    if(!sta.empty()){
        cout << "GRSKA RAZLICITE LISTE 1\n";

    }
    int max_ind = -1;
    for(p_list_node curr = head; curr != NULL; curr = curr -> next){
        if(curr -> index <= max_ind) return false;
        if(curr -> index > max_ind)
            max_ind = curr -> index;
    }
    return true;
}

/*
100
800Kb
insere arq0 43Kb
insere arq1 364Kb
remove arq0
remove arq1
insere arq2 89Kb
insere arq3 331Kb
insere arq4 17Kb
remove arq4
remove arq2
remove arq3
insere arq5 344Kb
insere arq6 213Kb
insere arq7 121Kb
insere arq8 35Kb
remove arq6
insere arq9 87Kb
remove arq8
insere arq10 12Kb
remove arq5
insere arq11 132Kb
remove arq10
insere arq12 44Kb
insere arq13 97Kb
remove arq13
insere arq14 156Kb
remove arq7
insere arq15 35Kb
insere arq16 16Kb
remove arq9
insere arq17 59Kb
remove arq15
insere arq18 46Kb
remove arq16
insere arq19 85Kb
insere arq20 61Kb
insere arq21 85Kb
remove arq14
insere arq22 123Kb
insere arq23 69Kb
insere arq24 14Kb
insere arq25 24Kb
remove arq17
insere arq26 26Kb
remove arq19
insere arq27 20Kb
insere arq28 76Kb
insere arq29 18Kb
remove arq12
remove arq29
insere arq30 25Kb
remove arq24
remove arq21
insere arq31 34Kb
insere arq32 4Kb
remove arq22
insere arq33 65Kb
insere arq34 72Kb
remove arq34
remove arq31
insere arq35 38Kb
remove arq18
remove arq28
remove arq35
remove arq25
insere arq36 137Kb
insere arq37 68Kb
insere arq38 22Kb
insere arq39 12Kb
insere arq40 36Kb
remove arq37
insere arq41 87Kb
insere arq42 33Kb
insere arq43 5Kb
insere arq44 22Kb
insere arq45 17Kb
insere arq46 4Kb
remove arq41
insere arq47 13Kb
insere arq48 24Kb
insere arq49 34Kb
remove arq36
insere arq50 13Kb
insere arq51 30Kb
insere arq52 19Kb
insere arq53 21Kb
insere arq54 2Kb
insere arq55 22Kb
insere arq56 21Kb
insere arq57 23Kb
remove arq39
remove arq44
remove arq23
insere arq58 9Kb
insere arq59 6Kb
insere arq60 22Kb
insere arq61 9Kb
insere arq62 20Kb
insere arq63 14Kb
remove arq20
remove arq40
 */

/*
18
40Kb
insere arq001 5Kb
insere arq002 5Kb
insere arq003 5Kb
insere arq004 5Kb
insere arq005 5Kb
insere arq006 5Kb
insere arq007 5Kb
insere arq008 5Kb
remove arq002
remove arq003
remove arq007
insere arq002 1Kb
insere arq003 14Kb
remove arq001
remove arq004
remove arq005
otimiza
otimiza
 */
