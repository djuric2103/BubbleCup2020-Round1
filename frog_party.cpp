#include <bits/stdc++.h>

using namespace std;

const int dx[] = {-1, 1};

int N, M, Q;

int main()
{
    ios_base::sync_with_stdio(false);

    scanf("%d", &Q);
    while (Q--) {
        scanf("%d %d", &N, &M);
        printf("YES\n");

        int pos[] = {M - 1, M + 1};
        int state = 2 * M > N;
        int cur = M;
        while (cur != 1 && cur != N) {
            printf("%d %d\n", cur, pos[state]);
            cur = pos[state];
            pos[state] += dx[state];
            state ^= 1;
        }

        int len = N - 2 * min(M - 1, N - M) - (M == 1 ||  M == N);
        int offset = 2 * M > N ? 1 : N - len + 1;
        int _cur = 2 * M > N ? (len + 1) / 2 - 1 : len - (len + 1) / 2;
        pos[0] = _cur - 1; pos[1] = _cur + 1;
        state = len % 2;
        state ^= 2 * M > N;
        for (int i = 1; i < len; i++) {
            printf("%d %d\n", offset + _cur, offset + pos[state]);
            _cur = pos[state];
            pos[state] += dx[state];
            state ^= 1;
        }

        _cur += offset;
        if (1 <= min(cur, _cur) && max(cur, _cur) <= N) printf("%d %d\n", cur, _cur);
    }

    return 0;
}
