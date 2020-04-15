//#pragma GCC optimize ("Ofast")
#include <bits/stdc++.h>
#define greatest 48780700
#define logn 25
using namespace std;
const int maxn = greatest + 2;
int ludic[maxn / 64 + 10];
int offset[2509954];
const int BIT_SIZE = maxn - 1;
int BIT[maxn];
int sol[2509954];
int sol_size = 0;

inline void update(int x, int v) {
    while (x < BIT_SIZE) BIT[x] += v, x += x & -x;
}

inline int sum(int x) {
    int ret = 0;
    while (x) ret += BIT[x], x -= x & -x;
    return ret;
}

inline int query(int x) {
    int sum = 0, ret = 0;
    for (int i = logn; i >= 0; i--)
        if (ret + (1 << i) < BIT_SIZE && sum + BIT[ret + (1 << i)] < x)
            sum += BIT[ret + (1 << i)],
                    ret += 1 << i;
    return ++ret;
}

void bitWiseSieve(int n){
    for (int i = 1; i <= n; i++) BIT[i] = i & -i;
    for (int i = 2; i <= n; i += 2) BIT[i] /= 2;
    for (int i = 0; i <= logn; i++) BIT[1 << i]++;
    sol[0] = 1, sol[1] = 2, offset[0] = 0, offset[1] = 0;
    int sum_n = -1;
    int curr = 2;
    int left = sum(n);
    for (int i = 3; i <= n; i += 2) {
        if (!(ludic[i/64] & (1 << ((i >> 1) & 31)))) {
            //if(!ludic[i/2]){
            curr++;
            sol[curr - 1] = i;
            if(i <= 2*n/41) {
                int s = curr;
                for (int j = curr + i; j < left + 1; j += i-1) {
                    int k = query(j);
                    if (k >= n + 1) break;
                    ludic[k/64] |= (1 << ((k >> 1) & 31));
                    //ludic[k/2] = true;
                    //j--;
                    left--;
                    s += (i - 1);
                    update(k, -1);
                }
                //update(2,0-ob);
                offset[curr - 1] = left - s;

                continue;
            }
            if(sum_n == -1) sum_n = sum(n);
            offset[curr - 1] = sum_n - curr;
        }
    }
}

inline int binary(int q){
    int low = 0, high = sol_size - 1, mid, ans = -1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (sol[mid] <= q) ans = mid + 1, low = mid + 1;
        else high = mid - 1;
    }
    return ans;
}

int k = 2509954;

inline int return_num(int n){
    int additional = n - greatest;
    int cout = 0;
    for(int i = 1; i < k;  i++){
        int toRem = (offset[i] + additional)/sol[i];
        if(toRem == 0) cout++;
        else cout = 0;
        additional -= toRem;
    }
    k -= cout;
    return additional + sol_size;
}

int main(){
    int t;
    scanf("%d",&t);
    bitWiseSieve(greatest);
    sol_size = 2509954;
    vector<int> qu;
    vector<int> so;
    for(int i = 0; i < t; i++){
        int temp;
        scanf("%d",&temp);
        qu.push_back(temp);
        so.push_back(temp);
    }
    unordered_map<int, int> a;
    sort(so.begin(), so.end());

    for(int i = t - 1; i >= 0; i--){
        if(so[i] > greatest)a.insert({so[i], return_num(so[i])});
        else a.insert({so[i], binary(so[i])});
    }

    for(int  i = 0; i < t; i++){
       printf("%d\n", a.find(qu[i]) -> second);
    }

    return 0;
}