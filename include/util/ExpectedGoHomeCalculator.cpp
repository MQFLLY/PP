#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Suppose a drunkard starts at position 0 and wants to return to position n. Each time, the drunkard moves from position i to i-1 with a probability of pᵢ, and to i+1 with a probability of qᵢ. Calculate the expected number of steps to go from position 0 to position n.Assume start from 0

vector<double> gaussianElimination(vector<vector<double>> A, vector<double> b) {
    int n = b.size();
    
    for (int i = 0; i < n; ++i) {
        A[i].push_back(b[i]);
    }
    
    for (int i = 0; i < n; ++i) {
        int maxRow = i;
        for (int j = i; j < n; ++j) {
            if (abs(A[j][i]) > abs(A[maxRow][i])) {
                maxRow = j;
            }
        }
        
        swap(A[i], A[maxRow]);
        
        double pivot = A[i][i];
        if (abs(pivot) < 1e-10) {
            cerr << "no solution" << endl;
            return {};
        }
        for (int j = i; j <= n; ++j) {
            A[i][j] /= pivot;
        }
        
        for (int j = 0; j < n; ++j) {
            if (j != i && abs(A[j][i]) > 1e-10) {
                double factor = A[j][i];
                for (int k = i; k <= n; ++k) {
                    A[j][k] -= factor * A[i][k];
                }
            }
        }
    }
    
    vector<double> x(n);
    for (int i = 0; i < n; ++i) {
        x[i] = A[i][n];
    }
    
    return x;
}

double calculateExpectedSteps(int n, const vector<double>& p, const vector<double>& q) {
    if (n <= 0) return 0.0;
    if (p.size() < n || q.size() < n) {
        cerr << "illegal p array" << endl;
        return -1.0;
    }
    
    vector<vector<double>> A(n, vector<double>(n, 0.0));
    vector<double> b(n, 1.0);  
    
    A[0][0] = 1.0;
    if (n > 1) {
        A[0][1] = -q[0];
    }
    
    for (int i = 1; i <= n-2; ++i) {
        A[i][i-1] = -p[i];
        A[i][i] = 1.0;
        A[i][i+1] = -q[i];
    }
    
    if (n >= 2) {
        A[n-1][n-2] = -p[n-1];
        A[n-1][n-1] = 1.0;
    }
    
    vector<double> x = gaussianElimination(A, b);
    
    return x.empty() ? -1.0 : x[0];
}

int main() {
    int n = 5;
    vector<double> p = {0.0, 0.3, 0.7, 0.7, 0.1};  
    vector<double> q = p;
    for (auto& it: q) {
        it = 1 - it;
    } 
    
    
    double result = calculateExpectedSteps(n, p, q);
    cout << result << endl;

    return 0;
}