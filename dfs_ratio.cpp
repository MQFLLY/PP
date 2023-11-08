#include<bits/stdc++.h>
using namespace std;
#define PII pair<string,string>
#define FI first
#define SE second

//generate the random number
mt19937 rnd(time(0));


//edge between two states
struct Edge{
    map<string,int> to; //the next state
    double p;           //possibility of transition
};

int cnt;    //the total number of state
int idx;    

map<PII,PII> rule;  //rule of PP
map<map<string,int>,int> mp; //memorize the state for dfs
map<string,int> v;  
map<string,int> ch;         //temorary store the whole space
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
    for(int i = 0;i < idx;i++) 
        res.mat[i][i] = 1;
    while(m > 0){
        if(m & 1) res = multiply(res,a);
        a = multiply(a,a);
        m >>= 1;
    }
    return res;
}



void dfs(map<string,int> &v){
    if(mp.count(v)) return;
    mp[v] = idx++;
    
    map<string,int> tmp = v;
    double tot = 0;
    for(auto it:rule) {
        string a = it.FI.FI;
        string b = it.FI.SE;
        string c = it.SE.FI;
        string d = it.SE.SE;
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
    /* 
    cout << "(";
    for(auto it:v) {
        cout <<  it.SE << ',';
    }
    puts(")"); 
    */
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
             cout << it.FI << " : " << it.SE << '\n';
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

string to_res(int n){
    return "r" + toS(n);
}

string to_medium(int n){
    return "m" + toS(n);
}


string tostring(char ch){
	string tmp;
	tmp += ch;
	return tmp;
}


void init_rule(int n,int k){
    rule[make_pair("i0","i0")] = make_pair("i1","i1");
    rule[make_pair("i1","i1")] = make_pair("i0","i0");
    rule[make_pair("i0","i1")] = make_pair("r1","m1");
    rule[make_pair("r1","m1")] = make_pair("i0","i1");
    for(int i = 2;i <= k - 2;i++) {
    	rule[make_pair("i0",to_medium(i - 1))] = make_pair(to_medium(i),to_res(i));
    	rule[make_pair("i1",to_medium(i - 1))] = make_pair(to_medium(i),to_res(i));
		rule[make_pair(to_medium(i),to_res(i))] = make_pair("i0",to_medium(i - 1));
    }
    rule[make_pair("i0",to_medium(k - 2))] = make_pair(to_res(k - 1),to_res(k));
    rule[make_pair("i1",to_medium(k - 2))] = make_pair(to_res(k - 1),to_res(k));
    for(auto it:rule){
        	ch[it.FI.FI]++;
			ch[it.FI.SE]++;
			ch[it.SE.FI]++;
			ch[it.SE.SE]++;
    }
    for(auto it:ch){
        v[it.FI] = 0;
    }
    v["i0"] = n;
}

//10 3
//1 2 2
void trans_rule(vector<int> v,int k,int new_k){
    puts("Before trans:");
     for(auto it:rule){
        cout << it.FI.FI << ' ' << it.FI.SE << ' ' << it.SE.FI << ' ' << it.SE.SE << '\n';
    }   
    
    vector<PII> erase_v;
    map<PII,PII> add_map;
    map<string,string> to_new;
    for(int i = 1;i <= new_k;i++){
        int idx = 0;
        int sum = 0;
        for(int j = 0;j < k;j++){
           if(i <= v[j] + sum) {
                idx = j + 1;
                //cout << "i: " << i << " -> " << idx << '\n'; 
                to_new[to_res(i)] = to_res(idx);
                break;
           } 
           sum += v[j]; 
        }
    }
    for(auto it:rule){
        bool flag = 0;
        pair<PII,PII> tmp = it;
        for(auto itt:to_new){
           if(tmp.FI.FI == itt.FI) tmp.FI.FI = itt.SE,flag |= 1;
           if(tmp.FI.SE == itt.FI) tmp.FI.SE = itt.SE,flag |= 1;
           if(tmp.SE.FI == itt.FI) tmp.SE.FI = itt.SE,flag |= 1;
           if(tmp.SE.SE == itt.FI) tmp.SE.SE = itt.SE,flag |= 1;
        }
        if(flag) {
            erase_v.push_back(it.FI);
            add_map[tmp.FI] = tmp.SE;
        }
    }
    for(auto it:erase_v){
        rule.erase(it);
    }
    for(auto it:add_map){
        rule[it.FI] = it.SE;
    }
    puts("After trans:");
    for(auto it:rule){
        cout << it.FI.FI << ' ' << it.FI.SE << ' ' << it.SE.FI << ' ' << it.SE.SE << '\n';
    }
}

void show_matrix(){
    for(int i = 0;i < idx;i++){
        for(int j = 0;j < idx;j++){
            printf("%.2f ",MATRIX.mat[i][j]);
        }
        puts("");
    }
    puts("--------------------");
}

bool equals(double a,double b){
    return fabs(a - b) < 1e-4;
}

void show_matrix_infpow(){
    matrix res = mpow(MATRIX,1000000000);
    
    int num_zero = 0;
    int num_one = 0;
    set<int> row;
    for(int i = 0;i < idx;i++){
        for(int j = 0;j < idx;j++){
            printf("%.2f ",res.mat[i][j]);
            if(equals(res.mat[i][j],1)) {
                num_one++;
                row.insert(j);
            }
            else if(equals(res.mat[i][j],0)){
                num_zero++;
            }
        }
        puts("");
    }
    if(num_one == idx && row.size() == 1 && num_zero == idx * (idx-  1)) {
        puts("converge");
    }
    else if(num_one != idx){
        puts("wrong 1");
    }
    else if(row.size() != 1) {
        puts("wrong 2");
    }
    else {
        puts("wrong 3");
    }
}

int gcd(int a,int b){
    return !b ? a : gcd(b,a % b);
}

int gcd_all(vector<int>& v){
    if(v.size() == 2) return gcd(v[0],v[1]);
    int tmp = v.back();
    v.pop_back();
    return gcd(tmp,gcd_all(v));
}

int main(){
	//Input n and k , the number of agent and the number of group
    puts("Input number the number of agents,groups and the ratio");
    puts("Please ensure the n you input can be divide by the sum of ratio");
    int n,k;
    cin >> n >> k;
    vector<int> ratio;
    for(int i = 0;i < k;i++){
        int tmp;
        cin >> tmp;
        ratio.push_back(tmp);
    }
    auto cal = ratio;
    int g = gcd_all(cal);
    int new_k = 0;
    for(int i = 0;i < k;i++){
        ratio[i] /= g;
        new_k += ratio[i];
    }
    sort(ratio.begin(),ratio.end());

    /*
    for(auto it:ratio){
        cout << it << ' ';
    }
    puts("");*/

	//rule
    init_rule(n,new_k);
    trans_rule(ratio,k,new_k);

    //compute all the situations
	dfs(v);

    show_matrix();
    show_matrix_infpow();
    cout << "the number of states: " << idx << '\n';
}
