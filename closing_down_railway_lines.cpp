#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>
#include <set>
#include <queue>
#include <iomanip>
#include <math.h>

using namespace std;

int fi(int a, vector<int> &p)
{
    if (p[a] != p[p[a]])
    {
        p[a] = fi(p[a], p);
    }
    return p[a];
}

void uni(int a, int b, vector<int> &p)
{
    int x = fi(a, p), y = fi(b, p);
    if (x != y)
    {
        p[x] = y;
    }
}

double euclidianDistanceSquared(pair<double, double> a, pair<double, double> b)
{
    return (a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second);
}

double euclidianDistance(pair<double, double> a, pair<double, double> b)
{
    return sqrt(euclidianDistanceSquared(a, b));
}

void overwriteEdges(int n, int m, vector<vector<pair<int, int>>> &edges, vector<pair<int, int>> &allEdges, vector<int> &parent)
{
    for (int i = 0; i < n; i++)
    {
        edges[i].clear();
    }
    for (int i = 0; i < m; i++)
    {
        if (fi(allEdges[i].first, parent) != fi(allEdges[i].second, parent))
        {
            edges[allEdges[i].first].push_back({ allEdges[i].second, i });
            edges[allEdges[i].second].push_back({ allEdges[i].first, i });
        }
    }
}

bool ccw(pair<int, int> a, pair<int, int> b, pair<int, int> c)
{
    return (c.second - a.second) * (b.first - a.first) > (b.second - a.second) * (c.first - a.first);
}

bool intersect(pair<int, int> a1, pair<int, int> a2, pair<int, int> b1, pair<int, int> b2)
{
    return ccw(a1, b1, b2) != ccw(a2, b1, b2) && ccw(a1, a2, b1) != ccw(a1, a2, b2);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int t;
    cin >> t;
    long long n, m, k, l;

    for (int z = 0; z < t; z++)
    {
        // INPUT
        cin >> n >> m >> k >> l;
        vector<int> intersectionCount(m, 0), parent(n);
        vector<pair<int, int>> coords(n);
        vector<vector<pair<int, int>>> edges(n);
        vector<pair<int, int>> allEdges(m);
        vector<int> result, edgesLeft(n, 0);
        vector<bool> taken(m, false);

        for (int i = 0; i < n; i++)
        {
            cin >> coords[i].first >> coords[i].second;
            parent[i] = i;
        }
        for (int i = 0; i < m; i++)
        {
            cin >> allEdges[i].first >> allEdges[i].second;
            allEdges[i].first--;
            allEdges[i].second--;
            edges[allEdges[i].first].push_back({ allEdges[i].second, i });
            edges[allEdges[i].second].push_back({ allEdges[i].first, i });
            edgesLeft[allEdges[i].first]++;
            edgesLeft[allEdges[i].second]++;
        }


        // TAKE OBLIGATORY EDGES

        bool progress;
        do
        {
            progress = false;
            for (int i = 0; i < n; i++)
            {
                if (edgesLeft[i] == 1)
                {
                    for (int j = 0; j < edges[i].size(); j++)
                    {
                        if (fi(i, parent) != fi(edges[i][j].first, parent))
                        {
                            uni(i, edges[i][j].first, parent);
                            result.push_back(edges[i][j].second);
                            taken[edges[i][j].second] = true;
                            progress = true;
                            edgesLeft[i]--;
                            edgesLeft[edges[i][j].first]--;
                            break;
                        }
                    }
                }
            }
        } while (progress);
        // overwriteEdges(n, m, edges, allEdges, parent);
        


        for (int i = 0; i < m; i++)
        {
            if (fi(allEdges[i].first, parent) == fi(allEdges[i].second, parent))
            {
                continue;
            }
            for (int j = i + 1; j < m; j++)
            {
                if (fi(allEdges[j].first, parent) == fi(allEdges[j].second, parent))
                {
                    continue;
                }
                if (intersect(coords[allEdges[i].first], coords[allEdges[i].second], coords[allEdges[j].first], coords[allEdges[j].second]))
                {
                    intersectionCount[i]++;
                    intersectionCount[j]++;
                }
            }
        }

        vector<pair<double, int>> options;
        for (int i = 0; i < m; i++)
        {
            if (fi(allEdges[i].first, parent) == fi(allEdges[i].second, parent))
            {
                continue;
            }
            double costEstimate = k * euclidianDistance(coords[allEdges[i].first], coords[allEdges[i].second]) + 10.0 * l * intersectionCount[i] / (m - result.size());
            for (int j = 0; j < result.size(); j++)
            {
                if (intersect(coords[allEdges[i].first], coords[allEdges[i].second], coords[allEdges[result[j]].first], coords[allEdges[result[j]].second]))
                {
                    costEstimate += l;
                }
            }
            options.push_back({ costEstimate, i });
        }
        sort(options.begin(), options.end());

        for (int i = 0; result.size() != n - 1; i++)
        {
            if (fi(allEdges[options[i].second].first, parent) == fi(allEdges[options[i].second].second, parent))
            {
                continue;
            }
            for (int j = i + 1; j < options.size(); j++)
            {
                if (intersect(coords[allEdges[options[i].second].first], coords[allEdges[options[i].second].second], coords[allEdges[options[j].second].first], coords[allEdges[options[j].second].second]))
                {
                    options[j].first += l;
                    options[j].first -= 10.0 * intersectionCount[options[j].second] / (m - result.size());
                    intersectionCount[options[j].second]--;
                    options[j].first += 10.0 * intersectionCount[options[j].second] / (m - (result.size() + 1));
                }
            }
            uni(allEdges[options[i].second].first, allEdges[options[i].second].second, parent);
            result.push_back(options[i].second);
            sort(options.begin(), options.end());
            i = -1;
        }

        cout << "case " << z + 1 << " Y\n";
        for (int i = 0; i < result.size(); i++)
        {
            cout << result[i] + 1 << '\n';
        }




    }
}