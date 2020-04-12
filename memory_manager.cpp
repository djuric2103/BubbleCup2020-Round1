#include <bits/stdc++.h>

using namespace std;
typedef struct treap_node *p_treap_node;
typedef struct list_node *p_list_node;
unordered_map<p_list_node , reference_wrapper<p_treap_node>> listToTreap;
unordered_map<string, p_list_node> nameToList;
long long left_space, disk_size;
p_list_node head, last;

struct treap_node {
    long long key, prior;
    treap_node *l, *r;
    p_list_node list_node;
    treap_node() {}
    treap_node(long long key, long long prior, p_list_node list_node) : key(key), prior(prior), l(NULL), r(NULL), list_node(list_node) {};
};

struct list_node {
    long long index;
    long long size;
    bool deleted;
    list_node *prev, *next;
    list_node() {};
    list_node(long long index, long long size, bool deleted) : index(index), size(size), prev(NULL), next(NULL), deleted(deleted) {};
};

inline void put_in_map(p_treap_node &treap_node) {
    if(treap_node == NULL) return;
    listToTreap.erase(treap_node -> list_node);
    reference_wrapper<p_treap_node > a(treap_node);
    listToTreap.insert({treap_node -> list_node, a});
}

struct Treap {
    p_treap_node notFound;
    p_treap_node root;
    Treap() {
        notFound = new treap_node(-1,-1, NULL);
        root = NULL;
    }

private:
    void split(p_treap_node curr, long long key, p_treap_node &l, p_treap_node &r) {
        if (curr == NULL) l = r = NULL;
        else if (key < curr->key) {
            split(curr->l, key, l, curr->l);
            r = curr;
            put_in_map(r);
        }else {
            split(curr->r, key, curr->r, r);
            l = curr;
            put_in_map(l);
        }
    }

    void insert(p_treap_node &curr, p_treap_node it) {
        if (curr == NULL) {
            curr = it;
            put_in_map(curr);
        }else if (it->prior < curr->prior) {
            split(curr, it->key, it->l, it->r);
            curr = it;
            put_in_map(curr);
        }
        else insert(it->key <= curr->key ? curr->l : curr->r, it);
    }


    void merge(p_treap_node &curr, p_treap_node l, p_treap_node r) {
        if (l == NULL || r == NULL) {
            curr = l ? l : r;
        }
        else if (l->prior < r->prior) {
            merge(l->r, l->r, r);
            curr = l;
        }else {
            merge(r->l, l, r->l);
            curr = r;
        }
        put_in_map(curr);
    }

    p_treap_node &getFirst(p_treap_node &curr, long long size) {
        if (curr == NULL) return notFound;
        if (curr->key == size) return curr;
        if(curr -> key < size) return getFirst(curr -> r, size);
        p_treap_node& temp = getFirst(curr->l, size);
        if (temp == notFound || temp -> key >= curr -> key) return curr;
        return temp;
    }

public:
    p_treap_node &getFirst(long long size) {
        return getFirst(root, size);
    }
    void erase_and_join(p_treap_node &s, p_list_node l_node) {
        erase(s);
        create_and_insert_node(l_node);
    }

    void erase_and_join(p_list_node listNode, p_list_node l_node) {
        p_treap_node &s = (listToTreap.find(listNode-> prev) -> second).get();
        erase(s);
        p_treap_node &p = (listToTreap.find(listNode-> next) -> second).get();
        erase(p);
        create_and_insert_node(l_node);
    }

    void erase(p_treap_node &s) {
        listToTreap.erase(s->list_node);
        merge(s, s->l, s->r);
    }

    void create_and_insert_node(p_list_node list_node) {
        p_treap_node s = new treap_node(list_node->size, list_node->index, list_node);
        insert(root, s);
    }
};

Treap t;



inline long long returnNUM(string s) {
    long long ind = s.find_first_of('b') - 1;
    long long siz = stoi(s.substr(0, ind));
    switch (s[ind]) {
        case 'K': return siz;
        case 'M': return 1024 * siz;
        case 'G': return 1024 * 1024 * siz;
    }
}

inline void add(long long &curr_size, long long segment_size, long long &left) {
    double to_add = min(segment_size - curr_size, left);
    curr_size += to_add;
    left -= to_add;
}

inline void precentage(long long empty, long long size_of_segment) {
    if (4*empty <= size_of_segment) cout << "[#]";
    else if (4*empty > size_of_segment && 4*empty <= 3 * size_of_segment) cout <<"[-]";
    else cout << "[ ]";
}

void print_disk() {
    long long size_of_segment = disk_size / 8;
    long long k = 8;
    p_list_node curr = head;
    long long left_empty = 0, left_full = 0;
    while (k--) {
        long long empty = 0, full = 0;
        for (; empty + full != size_of_segment; curr = curr->next) {
            if (left_empty != 0) add(empty, size_of_segment - full, left_empty);
            else if (left_full != 0) add(full, size_of_segment - empty, left_full);
            if (empty + full == size_of_segment) break;
            if (curr->deleted) {
                left_empty = curr->size;
                add(empty, size_of_segment - full, left_empty);
            } else {
                left_full = curr->size;
                add(full, size_of_segment - empty, left_full);
            }
        }
        precentage(empty, size_of_segment);
    }
    cout << "\n";
}

void add_in_list(p_list_node old, p_list_node first, p_list_node second) {
    if (first != second) {
        first->next = second;
        second->prev = first;
    }
    first->prev = old->prev;
    second->next = old->next;

    if (old->prev == NULL) head = first;
    else first->prev->next = first;
    if (old->next != NULL) second->next->prev = second;
}

p_list_node mergeNodes(p_list_node first, p_list_node second) {
    p_list_node newNode = new list_node(first->index,first->size + second->size + (first->next != second ? first->next->size : 0),true);
    newNode->prev = first->prev;
    newNode->next = second->next;
    if (newNode->prev == NULL) head = newNode;
    else newNode->prev->next = newNode;
    if (newNode->next != NULL) newNode->next->prev = newNode;
    return newNode;
}

void insert(string name, long long length, p_treap_node &t_node) {
    p_list_node listNode = t_node->list_node;
    long long totalSize = t_node->key;
    p_list_node full = new list_node(t_node-> list_node -> index, length, false);
    nameToList.insert({name, full});
    t.erase(t_node);

    if (totalSize == length) add_in_list(listNode, full, full);
    else {
        p_list_node empty = new list_node(full->index + length, totalSize - length, true);
        t.create_and_insert_node(empty);
        add_in_list(listNode, full, empty);
    }
}

void removeSeg(p_list_node listNode) {
    if ((listNode->prev == NULL || listNode->prev->deleted == false) && (listNode->next == NULL || listNode->next->deleted == false)) {
        p_list_node newNode = new list_node(listNode -> index, listNode -> size, true);
        add_in_list(listNode, newNode, newNode);
        t.create_and_insert_node(newNode);
    } else if ((listNode->prev != NULL && listNode->prev->deleted == true) && (listNode->next != NULL && listNode->next->deleted == true)) {
        p_list_node newNode = mergeNodes(listNode->prev, listNode->next);
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
}

inline void delete_from_list(p_list_node l_node) {
    if (l_node->prev == NULL) head = l_node->next;
    else l_node -> prev -> next = l_node -> next;
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
    ios_base :: sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long long n;
    while (1) {
        cin >> n;
        if (n == 0) break;
        string disk;
        cin >> disk;
        disk_size = returnNUM(disk);
        left_space = disk_size;
        t = Treap();
        nameToList = unordered_map<string, p_list_node>();
        listToTreap = unordered_map<p_list_node , reference_wrapper<p_treap_node>> ();
        head = new list_node(0, disk_size, true);
        last = new list_node(disk_size+1, 0, false);
        head -> next = last;
        last -> prev = head;
        t.create_and_insert_node(head);
        bool optimized = true;
        long long i = 0;
        for (; i < n; i++) {
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
                if (s == t.notFound) {
                    optimize();
                    p_treap_node &s1 = t.getFirst(length);
                    insert(name, length, s1);
                    optimized = true;
                } else {
                    insert(name, length, s);
                    optimized = false;
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
                if(p-> next != last) optimized = false;
                continue;
            }
            if(!optimized) optimize();
            optimized = true;
        }
        if (i == n) print_disk();
        for(string temp; i<n; i++, getline(cin, temp));
    }
    return 0;
}
