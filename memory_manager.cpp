#include <bits/stdc++.h>

using namespace std;
typedef struct treap_node *p_treap_node;
typedef struct list_node *p_list_node;
std::unordered_map<int, reference_wrapper<p_treap_node>> listToTreap;

struct treap_node {
    int key, prior;
    treap_node *l, *r;
    p_list_node list_node;

    treap_node() {}

    treap_node(int key, int prior, p_list_node list_node, int asd) : key(key), prior(prior), l(NULL), r(NULL),
                                                                     list_node(list_node) {
    }
};

struct list_node {
    int index;
    int size;
    bool deleted;
    //string name;
    list_node *prev, *next;

    list_node() {};

    list_node(int index, int size, bool deleted) : index(index), size(size), prev(NULL), next(NULL),
                                                   deleted(deleted) {};
    //list_node(int index, int size, string name,bool deleted) : index(index), size(size), name(name), prev(NULL), next(NULL), deleted(deleted) {};

};




struct Treap {
    p_treap_node notFound;
    p_treap_node root;

    Treap() {
        notFound = new treap_node(-1,-1, NULL, 1);//t.create_and_insert_node(-1, -1, NULL);
        root = NULL;
    }

private:
    void split(p_treap_node t, int key, p_treap_node &l, p_treap_node &r) {
        if (!t)
            l = r = NULL;
        else if (key < t->key)
            split(t->l, key, l, t->l), r = t;
        else
            split(t->r, key, t->r, r), l = t;
    }

    p_treap_node &insert(p_treap_node &t, p_treap_node it) {
        if (!t) {
            t = it;
            return t;
        }else if (it->prior < t->prior) {
            split(t, it->key, it->l, it->r), t = it;
            return t;
        }
        else
            insert(it->key < t->key ? t->l : t->r, it);
    }

    void merge(p_treap_node &t, p_treap_node l, p_treap_node r) {
        if (l == NULL || r == NULL)
            t = l ? l : r;
        else if (l->prior < r->prior)
            merge(l->r, l->r, r), t = l;
        else
            merge(r->l, l, r->l), t = r;
    }

    /*void erase(p_treap_node &t, int key) {
        if (t->key == key) {
            listToTreap.erase(t->list_node);
            merge(t, t->l, t->r);
        }
        else
            erase(key < t->key ? t->l : t->r, key);
    }*/

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

    p_treap_node &getFirst(p_treap_node &curr, int size) {
        if (!curr) {
            return notFound;
        }
        if (curr->key >= size) return curr;
        return getFirst(curr->r, size);
    }

public:
    void insert(p_treap_node it) {
        insert(root, it);
    }

    /*void erase(int key) {
        erase(root, key);
    }*/
    p_treap_node &getFirst(int size) {
        getFirst(root, size);
    }

    /*void erase_and_join(p_treap_node &s, int n_key, int n_prior, p_list_node l_node) {
        int o_key = s->key, o_prior = s->prior;
        erase(s);
        p_treap_node newNode = t.create_and_insert_node(o_key + n_key, min(n_prior, o_prior), l_node);
        insert(newNode);
        //return newNode;
    }

    void erase_and_join(p_treap_node &s, p_treap_node &p, int n_key, int n_prior, p_list_node l_node) {
        int k = s->key + p->key + n_key, pri = min(min(s->prior, p->prior), n_prior);

        erase(s);
        erase(p);
        p_treap_node newNode = t.create_and_insert_node(k, pri, l_node);
        //p_treap_node & pnn =newNode;

        insert(newNode);
        //return newNode;

    }*/

    void erase_and_join(p_treap_node &s, p_list_node l_node) {
        erase(s);
        create_and_insert_node(l_node);

        //insert(newNode);
        //return newNode;
    }

    void erase_and_join(p_treap_node &s, p_treap_node &p, p_list_node l_node) {
        erase(s);
        erase(p);
        create_and_insert_node(l_node);
        //p_treap_node & pnn =newNode;

        //insert(newNode);
        //return newNode;
    }

    void erase(p_treap_node &s) {
        //listToTreap.erase(s->list_node);
        merge(s, s->l, s->r);
    }

    void printLRR() {
        print(root);
    }

    void printRLR() {
        print1(root);
    }

    void create_and_insert_node(p_list_node list_node) {
        if(listToTreap.find(list_node -> index) != listToTreap.end())
            listToTreap.erase(listToTreap.find(list_node -> index));

        p_treap_node s = new treap_node(list_node->size, list_node->index, list_node, 0);
        p_treap_node& t = insert(root, s);
        //p_treap_node &s1 = s;
        //listToTreap.erase(list_node -> index);
        //reference_wrapper<p_treap_node> a{s};
        listToTreap.erase(list_node -> index);
        //listToTreap.insert({list_node -> index, a});
        //p_treap_node as = (listToTreap.find(list_node -> index) -> second).get();
        listToTreap.insert({list_node -> index, t});
        if(list_node -> index == 126){
            p_treap_node &asd = (listToTreap.find(list_node -> index) ->second) . get();
            cout << list_node -> index << '\t' <<  asd->key << endl;
        }
        //cout << "S key "<<s->key <<"WRAP key "<< as->key << endl;
        //if()


        //listToTreap[list_node] = s;
        //cout << "INSERTED "<<list_node << '\t' << s << endl;

        //return s;
    }
};


Treap t;
p_list_node head;

unordered_map<string, p_list_node> nameToList;
int left_space, disk_size;

int returnInt(string s) {
    int ind = s.find_first_of('b') - 1;
    int siz = stoi(s.substr(0, ind));
    switch (s[ind]) {
        case 'K':
            return siz;
        case 'M':
            return 1024 * siz;
        case 'G':
            return 1024 * 1024 * siz;
    }
}

void add(int &curr_size, int segment_size, int &left) {
    int to_add = min(segment_size - curr_size, left);
    curr_size += to_add;
    left -= to_add;
}

void precentage(int empty, int full) {
    double ratio = (full) / ((double) (empty + full));
    //cout << ratio << endl;
    if (ratio >= 0.75)
        cout << "#";
    else if (ratio < 0.25)
        cout << " ";
    else
        cout << "-";
}

void print_disk() {
    int size_of_segment = disk_size / 8;
    int k = 8;
    p_list_node curr = head;
    int left_empty = 0, left_full = 0;
    while (k--) {
        cout << "[";
        int empty = 0, full = 0;
        for (; empty + full != size_of_segment; curr = curr->next) {
            if (left_empty != 0) {
                add(empty, size_of_segment - full, left_empty);
            } else if (left_full != 0) {
                add(full, size_of_segment - empty, left_full);
            }
            if (empty + full == size_of_segment) break;
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
    for (p_list_node curr = head; curr != NULL; curr = curr->next)
        cout << "SIZE" << curr->size << "\tIndex" << curr->index << "\tDeleted" << curr->deleted << endl;
    cout << endl;
    t.printLRR();
    cout << endl;
    t.printRLR();
    cout << "____________________________________________\n";
}

void insert(string name, int length, p_treap_node &t_node) {
    p_list_node listNode = t_node->list_node;
    int totalSize = t_node->key;
    p_list_node full = new list_node(t_node->prior, length, false);
    nameToList[name] = full;
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
    } else if ((listNode->prev != NULL && listNode->prev->deleted == true) &&
               (listNode->next != NULL && listNode->next->deleted == true)) {
        p_treap_node &prevInTreap = (listToTreap.find(listNode->prev -> index) -> second).get();
        p_treap_node &nextInTreap = (listToTreap.find(listNode->next -> index) -> second).get();
        p_list_node newNode = mergeNodes(listNode->prev, listNode->next);
        t.erase_and_join(prevInTreap, nextInTreap, newNode);
    } else if (listNode->prev != NULL && listNode->prev->deleted == true) {
        p_treap_node &prevInTreap = (listToTreap.find(listNode->prev ->index) ->second).get();
        //cout << "PREVIOUS" << prevInTreap->key <<"\t" << prevInTreap -> prior << endl;
        //t.eraseWithKeyAndPriority(prevInTreap -> key, prevInTreap -> size);
        p_list_node newNode = mergeNodes(listNode->prev, listNode);
        t.erase_and_join(prevInTreap, newNode);
    } else if (listNode->next != NULL && listNode->next->deleted == true) {
        p_treap_node &nextInTreap = (listToTreap.find(listNode->next -> index)->second).get();
        p_list_node newNode = mergeNodes(listNode, listNode->next);
        t.erase_and_join(nextInTreap, newNode);
    }
    //print_list();
}

p_list_node last;

void delete_from_list(p_list_node l_node) {
    if (l_node->prev == NULL) {
        head = l_node->next;
    }else{
        l_node -> prev -> next = l_node -> next;
    }
    l_node -> next -> prev = l_node -> prev;
}

void addRestPriorLast() {
    p_list_node new_node = new list_node(last -> prev -> index + 1, left_space, true);
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
    listToTreap = std::unordered_map<int, reference_wrapper<p_treap_node>> ();
    addRestPriorLast();
}

/*Treap t = Treap();
    p_list_node head = new list_node(5, 6, "asd");
    t.insert(t.create_and_insert_node(9, 8, &head));
    t.insert(t.create_and_insert_node(9, 7, &head));
    t.insert(t.create_and_insert_node(9, 4, &head));
    t.insert(t.create_and_insert_node(9, 5, &head));
    t.insert(t.create_and_insert_node(1, 4, &head));
    t.insert(t.create_and_insert_node(1, 2, &head));
    t.printLRR();
    cout << endl;
    p_list_node head1 = new list_node(51, 61, "asasd");
    p_treap_node &s = t.getFirst(9);
    listToTreap.insert({head1->name, s});
    t.erase(listToTreap.at(head1 -> name) -> second);
    t.printLRR();
    cout << endl;
*/

int main() {
    int n;
    while (1) {
        cin >> n;
        if (n == 0) break;
        string a;
        cin >> a;
        disk_size = returnInt(a);
        left_space = disk_size;
        t = Treap();
        nameToList = unordered_map<string, p_list_node>();
        listToTreap = std::unordered_map<int, reference_wrapper<p_treap_node>> ();
        head = new list_node(0, disk_size, true);
        last = new list_node(disk_size+1, 0, false);
        head -> next = last;
        last -> prev = head;
        t.create_and_insert_node(head);
        //listToTreap.insert({head, t.getFirst(0)});
        int i = 0;
        for (; i < n; i++) {
            //print_list();
            //print_list();
            cout << i << endl;
            if(i == 25)
                cout << "Here" << endl;
            string op;
            cin >> op;
            if (op.compare("insere") == 0) {
                string name;
                cin >> name;
                string t_len;
                cin >> t_len;

                int length = returnInt(t_len);

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
