#include<bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define pii pair<int,int>
mt19937 rnd(time(0));


const int maxn = 1e5 + 5;

pii gen(int n){
    int x = rnd() % n + 1;
    int y = rnd() % n + 1;
    while(x == y) {
        y = rnd() % n + 1;
    }
    return make_pair(x,y);
}

string sta[maxn];
map<pair<string,string>,pair<string,string>> rule;

void update(int x,int y){
   if(rule.count(make_pair(sta[x],sta[y]))) {
        auto res = rule[make_pair(sta[x],sta[y])];
        sta[x] = res.fi;
        sta[y] = res.se;  
   }
   else if(rule.count(make_pair(sta[x],sta[y]))) {
        auto res = rule[make_pair(sta[x],sta[y])];
        sta[x] = res.fi;
        sta[y] = res.se;
   }
}

string toString(int n){
    string tmp;
    while(n){
        tmp.push_back('0' + n % 10);
        n /= 10;
    }
    reverse(tmp.begin(),tmp.end());
    return tmp;
}


string to_res(int n){
    return "r" + toString(n);
}

string to_medium(int n){
    return "m" + toString(n);
}

string to_d(int n){
    return "d" + toString(n);
}

bool check(int n,int k){
    int cnt = 0;
    int cnt2 = 0;
    for(int i = 1;i <= n;i++){
        if(sta[i] == to_res(k)) cnt++; 
        else if(sta[i] == to_res(k - 1)) cnt2++;
    }    
    if(cnt == n / k && cnt2 != cnt) assert(0);
    return cnt == n / k;
}

int go(int n,int k){
    int time = 0;
    while(1){
        if(check(n,k)){
            break;
        }
        auto p = gen(n);
        update(p.fi,p.se);
        time++;
    }
    return  time;
}

void init_rule(int k){
    rule[make_pair("i0","i0")] = make_pair("i1","i1");
    rule[make_pair("i1","i1")] = make_pair("i0","i0");
    for(int i = 1;i <= k - 2;i++){
        rule[make_pair(to_d(i),"i0")] = make_pair(to_d(i),"i1");
        rule[make_pair(to_d(i),"i1")] = make_pair(to_d(i),"i0");
    }
    for(int i = 1;i <= k;i++){
        rule[make_pair(to_res(i),"i0")] = make_pair(to_res(i),"i1");
        rule[make_pair(to_res(i),"i1")] = make_pair(to_res(i),"i0");
    }
    rule[make_pair("i0","i1")] = make_pair("r1","m2");
    for(int i = 2;i <= k - 2;i++){
        rule[make_pair("i0",to_medium(i))] = make_pair(to_res(i),to_medium(i + 1));
        rule[make_pair("i1",to_medium(i))] = make_pair(to_res(i),to_medium(i + 1));
    }
    rule[make_pair("i0",to_medium(k - 1))] = make_pair(to_res(k - 1),to_res(k));
    rule[make_pair("i1",to_medium(k - 1))] = make_pair(to_res(k - 1),to_res(k));
    for(int i = 2;i <= k - 1;i++){
        for(int j = 2;j <= k - 1;j++){
            rule[make_pair(to_medium(i),to_medium(j))] = make_pair(to_d(i - 1),to_d(j - 1));
        }
    }
    for(int i = 2;i <= k - 2;i++){
        rule[make_pair(to_d(i),to_res(i))] = make_pair(to_d(i - 1),"i0");
    }
    rule[make_pair("d1","r1")] = make_pair("i0","i1");
}

int main(){
    int n,k;
    puts("Input number the number of agents,groups,times,epoch");
    cin >> n >> k;
    init_rule(k);
    /*
    for(auto it:rule){
        cout << it.fi.fi << ' ' << it.fi.se << ' ' << it.se.fi << ' ' << it.se.se << '\n';
    }*/
    int tot_time = 0;
    int T,epoch;
    cin >> T >> epoch;
    for(int i = 0;i < T;i++){
        for(int j = 1;j <= n;j++)
            sta[j] = "i0";
        int now = go(n,k);
        tot_time += now;
        if((i + 1) % epoch == 0) {
            cout << "the " << i / epoch << "'s epoch : " << tot_time / (i + 1) << '\n';
        }
        cout << now << '\n';
    }

    cout << "average time : " << tot_time / T; 
}

