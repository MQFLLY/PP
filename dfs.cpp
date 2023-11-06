#include<bits/stdc++.h>
using namespace std;
#define pii pair<string,string>
#define fi first
#define se second

mt19937 rnd(time(0));

struct Edge{
    map<string,int> to;
    double p;
};

int cnt;
int idx;

map<pii,pii> rule;

map<map<string,int>,int> mp;
map<string,int> v;
map<string,int> ch;
map<map<string,int>,vector<Edge>> EdgeMap;

struct matrix{
    double mat[500][500];
}MATRIX;

matrix multiply(matrix &a,matrix &b){
    matrix c;
    for(int i = 0;i < idx;i++)
        for(int j = 0;j < idx;j++)
            c.mat[i][j] = 0;
    for(int i=0;i<idx;i++)
        for(int j=0;j<idx;j++)
            for(int k=0;k<idx;k++)
                c.mat[i][j]+=a.mat[i][k]*b.mat[k][j];
    return c;
}

matrix mpow(matrix &a,long long m){
    matrix res;
    for(int i = 0;i < idx;i++)
        for(int j = 0;j < idx;j++)
            res.mat[i][j] = 0;
    for(int i=0;i<idx;i++) res.mat[i][i]=1;
    while(m>0){
        if(m&1) res=multiply(res,a);
        a=multiply(a,a);
        m>>=1;
    }
    return res;
}



void dfs(map<string,int> &v){
    if(mp.count(v)) return;
    mp[v] = idx++;
    
    map<string,int> tmp = v;
    double tot = 0;
    for(auto it:rule) {
        string a = it.fi.fi;
        string b = it.fi.se;
        string c = it.se.fi;
        string d = it.se.se;
        if(a != b && v[a] > 0 && v[b] > 0) {
            tot += v[a] * v[b];
            double remain = v[a] * v[b]; 
            v[a]--;
            v[b]--;
            v[c]++;
            v[d]++;
            EdgeMap[tmp].push_back(Edge{v,remain});
            dfs(v);
            v[a]++;
            v[b]++;
            v[c]--;
            v[d]--;
        }
        else if(a == b && v[a] >= 2) {
            tot += v[a] * (v[a] - 1) / 2;
            double remain = v[a] * (v[a] - 1) / 2;
            v[a] -= 2;
            v[c]++;
            v[d]++;
            EdgeMap[tmp].push_back(Edge{v,remain});
            dfs(v);
            v[a] += 2;
            v[c]--;
            v[d]--;
        }
    }
    //cout << "---------" << ++cnt << "----------\n";
    ++cnt;
    
    cout << "(";
    for(auto it:v) {
        
        cout <<  it.se << ',';
    }
    puts(")"); 
    int count = 0;
    if(EdgeMap[tmp].size()) 
       count = count; 
    else {
        MATRIX.mat[mp[tmp]][mp[tmp]] = 1;
    }    
    for(int i = 0;i < EdgeMap[tmp].size();i++){
        EdgeMap[tmp][i].p /= tot;

       // cout <<  "*******" << ++count << "********" << '\n';
       // cout << "p = " << EdgeMap[tmp][i].p << '\n'; 
        MATRIX.mat[mp[tmp]][mp[EdgeMap[tmp][i].to]] = EdgeMap[tmp][i].p;
	/*
        for(auto it:EdgeMap[tmp][i].to) {
             cout << it.fi << " : " << it.se << '\n';
        }
        cout << "********" << '\n';
    	*/
     }
}

string toS(int x){
	string ans;
	while(x){
		ans += ('0' + x % 10);
		x /= 10;
	}
	reverse(ans.begin(),ans.end());
	return ans;
}

string tostring(char ch){
	string tmp;
	tmp += ch;
	return tmp;
}

int main(){
		//Input n and k , the number of agent and the number of group
		int n,k;
    cin >> n >> k;
		//rule
    pair<string,string> p0 = make_pair("0","0");
    pair<string,string> p1 = make_pair("1","1");
    rule[p0] = p1;
    rule[p1] = p0;
    rule[make_pair("0","1")] = make_pair("a","2");
    rule[make_pair("a","2")] = make_pair("0","1");
    for(int i = 2;i <= k - 2;i++) {
    	rule[make_pair("0",toS(i))] = make_pair(tostring(i - 1 + 'a'),toS(i + 1));
    	rule[make_pair("1",toS(i))] = make_pair(tostring(i - 1 + 'a'),toS(i + 1));
		rule[make_pair(tostring(i -1 + 'a'),toS(i + 1))] = make_pair("0",toS(i));
    }
    rule[make_pair("0",toS(k - 1))] = make_pair(tostring(k - 2 + 'a'),tostring(k -1 +  'a'));
    rule[make_pair("1",toS(k - 1))] = make_pair(tostring(k - 2 + 'a'),tostring(k -1 +  'a'));
    for(auto it:rule){
        	ch[it.fi.fi]++;
			ch[it.fi.se]++;
			ch[it.se.fi]++;
			ch[it.se.se]++;
    }
    for(auto it:ch){
        v[it.fi] = 0;
    }
    v["0"] = n;

    //compute all the situations
		dfs(v);


    for(int i = 0;i < idx;i++){
        for(int j = 0;j < idx;j++){
            printf("%.2f ",MATRIX.mat[i][j]);
        }
        puts("");
    }
    matrix res = mpow(MATRIX,1000000000);
    puts("--------------------------------");
    for(int i = 0;i < idx;i++){
        for(int j = 0;j < idx;j++){
            printf("%.2f ",res.mat[i][j]);
        }
        puts("");
    }
    cout << idx << '\n';
    
}
