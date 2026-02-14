#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
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


#define MAX_SIZE 8000

int cn2;
int cnt;    //the total number of state
map<PII,PII> rule;  //rule of PP
map<map<string,int>,int> mp; //memorize the state for dfs
map<string,int> v;  
map<string,int> ch;         //temorary store the whole space
map<map<string,int>,vector<Edge>> EdgeMap; 
map<string,int> col_to_stat[MAX_SIZE];

double equals(double a,double b){
    return fabs(a - b) < 1e-5;
}

class matrix{
public:
    int idx;
    double mat[MAX_SIZE][MAX_SIZE];
    void show_matrix();
    void show_matrix_infpow(int n);
    matrix mpow(int m);
    matrix(){}
    matrix(int n):idx(n){}
    // 深拷贝的复制构造函数
    matrix(const matrix &other) {
        idx = other.idx;
        for (int i = 0; i < other.idx;i++) {
            for (int j = 0; j < other.idx;j++) {
                mat[i][j] = other.mat[i][j];
            }
        }
    }

    // 深拷贝的赋值运算符重载
    matrix& operator=(const matrix &other) {
        if (this == &other) {
            return *this; // 处理自我赋值
        }
        this -> idx = other.idx;
        for (int i = 0; i < this -> idx; i++) {
            for (int j = 0; j < this -> idx; j++) {
                this->mat[i][j] = other.mat[i][j];
            }
        }
        return *this;
    }
    // 解非齐次线性方程组 Ax = b
    void solve_nonhomogeneous_system(const matrix& b) {
        // 假设 A 是一个 n x n 的矩阵，b 是一个 n x 1 的列向量

        int n = idx;

        // 构建增广矩阵 [A|b]
        static matrix augmentedMatrix(n);
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n; j++) {
                augmentedMatrix.mat[i][j] = mat[i][j];
            }
            augmentedMatrix.mat[i][n] = b.mat[i][0];
        }

        // 高斯消元法求解增广矩阵
        for (int i = 0; i < n - 1; i++) {
            // 将主元归一化为1
            double pivot = augmentedMatrix.mat[i][i];
            for (int j = i; j <= n; j++) {
                augmentedMatrix.mat[i][j] /= pivot;
            }

            // 消元
            for (int k = 0; k < n - 1; k++) {
                if (k != i) {
                    double factor = augmentedMatrix.mat[k][i];
                    for (int j = i; j <= n; j++) {
                        augmentedMatrix.mat[k][j] -= factor * augmentedMatrix.mat[i][j];
                    }
                }
            }
        }

        // 提取解向量 x
        for (int i = 0; i < n; i++) {
            mat[i][0] = augmentedMatrix.mat[i][n];
        }
    }
}MATRIX;

matrix multiply(matrix &a,matrix &b){
    int idx = a.idx;
    matrix c(idx);
    for(int i = 0;i < idx;i++)
        for(int j = 0;j < idx;j++)
            c.mat[i][j] = 0;
    for(int i=0;i<idx;i++)
        for(int j=0;j<idx;j++)
            for(int k=0;k<idx;k++)
                c.mat[i][j]+=a.mat[i][k]*b.mat[k][j];
    return c;
}

matrix matrix::mpow(int m){
    const int idx = this -> idx;
    matrix res(idx);
    matrix a = *this;
    for(int i = 0;i < idx;i++)
        for(int j = 0;j < idx;j++)
            res.mat[i][j] = 0;
    for(int i = 0;i < idx;i++) 
        res.mat[i][i] = 1;
    while(m > 0){
        if(m & 1) res = multiply(res,a);
        a = multiply(a,a);
        m >>= 1;
        cout << m << '\n';
    }
    return res;
}

void swap_mat_line_col(int i,int j, int idx) {
    swap(col_to_stat[i], col_to_stat[j]);
    for (int k = 0; k  < idx; k++) {
        swap(MATRIX.mat[i][k], MATRIX.mat[j][k]);
    }
    for (int k = 0; k < idx; k++) {
        swap(MATRIX.mat[k][i], MATRIX.mat[k][j]);
    }
}



void matrix::show_matrix(){
    /*
    for (int i = idx - 1; i >= 0; i--) {
        int mx_idx = 0;
        int mx = -1;
        for (int j = 0; j <= i; j++) {
           if (col_to_stat[j]["r3"] > mx) {
                mx = col_to_stat[j]["r3"];
                mx_idx = j;
            }
        }
        swap_mat_line_col(mx_idx, i, idx);
    }
    printf("%d\n", idx);
    */
    for(int i = 0;i < idx;i++){
        printf("[");
        for (auto [fi, se]: col_to_stat[i]) {
            cout << fi << ":" << se << ",";
        }
        printf("] ");
        for(int j = 0;j < idx;j++){
            printf("%.2f ", mat[i][j]);
        }
        puts("");
    }
    // puts("--------------------");
}


matrix tmp_;
matrix v_;

void output_expected_tran(){
    int col;
    tmp_.idx = MATRIX.idx;
    for (int i = 0; i < MATRIX.idx; i++) {
        for (int j = 0; j < MATRIX.idx; j++) {
            tmp_.mat[i][j] = MATRIX.mat[i][j];
        }
    }
    for (int i = 0; i < tmp_.idx; i++) {
        if (tmp_.mat[i][i] == 1) {
            col = i;
            break;
        }
    }
    for(int i = 0;i < tmp_.idx;i++)
        for(int j = 0;j < tmp_.idx;j++)
            if(i != j) tmp_.mat[i][j] *= -1;
    for(int i = 0;i < tmp_.idx;i++)
        tmp_.mat[i][i] = 1 - tmp_.mat[i][i];
    v_.idx = tmp_.idx;
    for(int i = 0;i < v_.idx;i++)
        v_.mat[i][0] = 1;
    v_.mat[col][0] = 0;
    // cout << tmp.idx << '\n';
    swap(v_.mat[col][0],v_.mat[tmp_.idx - 1][0]);
    for(int i = 0;i < tmp_.idx;i++){
        swap(tmp_.mat[col][i],tmp_.mat[tmp_.idx - 1][i]);
    }
    for(int i = 0;i < tmp_.idx - 1;i++){
        swap(tmp_.mat[i][col],tmp_.mat[i][tmp_.idx - 1]);
    }
    tmp_.solve_nonhomogeneous_system(v_);
    //tmp.show_matrix();
    cout << "Expected converge time:" << tmp_.mat[0][0] << '\n';
}

/*
void matrix::show_matrix_infpow(int m){
    matrix res = MATRIX.mpow(m);
    int idx = res.idx; 
    int num_zero = 0;
    int num_one = 0;
    set<int> row;
    for(int i = 0;i < idx;i++){
        for(int j = 0;j < idx;j++){
            //printf("%.2f ",res.mat[i][j]);
            if(equals(res.mat[i][j],1)) {
                num_one++;
                row.insert(j);
            }
            else if(equals(res.mat[i][j],0)){
                num_zero++;
            }
        }
    }
    map<string,int> test;
    for(auto it:mp){
        if(it.SE == idx) {
            test = it.FI; 
            break;
        
}
    }
    
    if(num_one == idx && row.size() == 1 && num_zero == idx * (idx-  1) && !EdgeMap[test].size()) {
        puts("converge");
    }
    else if(num_one != idx){
        puts("wrong 1");
    }
    else if(row.size() != 1) {
        puts("wrong 2");
    }
    else if(num_zero == idx * (idx - 1)){
        puts("wrong 3");
    }
    else{
        puts("wrong 4");
    }
    if(row.size())  output_expected_tran(*row.begin());
    else cout<<"err"<<'\n';
}
*/


void dfs(map<string, int> &currentState) {
    // 如果当前状态已经被处理过，则直接返回
    if (mp.count(currentState)) return;

    // 记录当前状态及其索引
    mp[currentState] = MATRIX.idx;
    col_to_stat[MATRIX.idx] = currentState;
    MATRIX.idx++;

    double totalPairs = 0; // 总配对数

    // 备份当前状态以便后续恢复
    map<string, int> originalState = currentState;

    // 遍历所有规则并尝试应用
    for (const auto& ruleItem : rule) {
        string a = ruleItem.first.first;
        string b = ruleItem.first.second;
        string c = ruleItem.second.first;
        string d = ruleItem.second.second;

        // 检查是否满足规则条件
        bool isPairRule = (a != b && currentState[a] > 0 && currentState[b] > 0);
        bool isSelfRule = (a == b && currentState[a] >= 2);

        if (isPairRule || isSelfRule) {
            // 计算该规则对应的配对数量
            double pairsCount = isPairRule ? currentState[a] * currentState[b] 
                                           : currentState[a] * (currentState[a] - 1) / 2;
            totalPairs += pairsCount;

            // 应用规则
            if (isPairRule) {
                currentState[a]--;
                currentState[b]--;
            } else {
                currentState[a] -= 2;
            }
            currentState[c]++;
            currentState[d]++;

            // 添加边到EdgeMap，并递归搜索新状态
            EdgeMap[originalState].emplace_back(currentState, pairsCount);
            dfs(currentState);

            // 撤销规则以尝试其他可能性
            if (isPairRule) {
                currentState[a]++;
                currentState[b]++;
            } else {
                currentState[a] += 2;
            }
            currentState[c]--;
            currentState[d]--;
        }
    }

    // 增加计数器
    ++cnt;

    // 如果没有可行的转移，则设置自环概率为1
    if (EdgeMap[originalState].empty()) {
        MATRIX.mat[mp[originalState]][mp[originalState]] = 1;
    } else {
        double sumProb = 0;
        // 更新MATRIX矩阵的概率值
        for (auto& edge : EdgeMap[originalState]) {
            edge.p /= cn2;
            sumProb += edge.p;
            MATRIX.mat[mp[originalState]][mp[edge.to]] = edge.p;
        }
        // 设置自环概率为剩余部分（1 - 已分配概率之和）
        MATRIX.mat[mp[originalState]][mp[originalState]] = 1 - sumProb;
    }
}

string toS(int x){
    if(!x) return "0";
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



int main(){
    //puts("DFS Program start...It shows all possible states");
	//Input n and k , the number of agent and the number of group
    //puts("Input number the number of agents,groups");
    int n,k;
    cin >> n >> k;
    cn2 = n * (n - 1) / 2;
	//rule
    init_rule(n,k);
    //compute all the situations
	dfs(v);
    output_expected_tran();
    // cout << MATRIX.idx << '\n';
    // MATRIX.show_matrix();
    
    // MATRIX.show_matrix_infpow(100000);

//    cout << "the number of states: " << MATRIX.idx << '\n';
}
