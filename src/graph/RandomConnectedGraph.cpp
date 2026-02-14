#include "graph/RandomConnectedGraph.h"
#include <iostream>
#include <fstream>
#include <map>
#include <assert.h>

void writeMatrixToCSV(const std::vector<std::vector<double>>& matrix, 
                      const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "can not open file:" << filename << std::endl;
        return;
    }
    
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            file << matrix[i][j];
            if (j < matrix[i].size() - 1) {
                file << ",";  
            }
        }
        file << "\n"; 
    }
    
    file.close();
    // std::cout << "matrix saved " << filename << std::endl;
}

void modify_to_com_g(std::vector<std::pair<size_t, size_t>> &edges, const size_t n) {
    edges.clear();
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            edges.push_back({i, j});
        }
    }
}

void modify_to_star_g(std::vector<std::pair<size_t, size_t>> &edges, const size_t n, int k) {
    edges.clear();

    int l = (n - 1) / k;
    for (int i = 2; i <= n; i++) {
        if (i % l == 2 % l) {
            edges.push_back({1, i});
        } 
        else {
            edges.push_back({i - 1, i});
        }
    }
}

RandomConnectedGraph::RandomConnectedGraph(size_t n, size_t m): numNodes(n) {
    if (m == -1) {
        std::uniform_int_distribution<size_t> dist(n - 1, (n - 1) * n / 2);
        size_t i = dist(rng);
        m = i; 
    }
    if (!(m >= n - 1))
        throw std::runtime_error("m >= n - 1 required!");
    if (m > (n - 1) * n / 2)
        throw std::runtime_error("m <= n * (n - 1) / 2 required!");
    for (int i = 1; i < n; i++) {
        size_t u = i + 1;
        std::uniform_int_distribution<size_t> dist(1, i);
        size_t v = dist(rng) ;
        edges.push_back({v, u});
    }
    std::uniform_int_distribution<size_t> dist(1, n);
    for (int i = 0; i < m - (n - 1); i++) {
        while (true) {
            size_t u = dist(rng);
            size_t v = dist(rng);
            while (v == u) {
                v = dist(rng);
            }
            if (v < u) {
                std::swap(u, v);
            }
            bool is_exist = false;
            for (const auto [fi, se]: edges) {
                if (fi == u && se == v) {
                    is_exist = true;
                    break;
                }
            }
            if (!is_exist) {
                edges.push_back({u, v});
                break;
            }
        }
    }

    // 指定图：
    int k = 4;
    // modify_to_star_g(edges, numNodes, k);
    modify_to_com_g(edges, numNodes);

    std::map<std::pair<int ,int>, bool> mp;
    for (auto [fi, se]: edges) {
        mp[{fi - 1, se - 1}] = true;
    }

    std::vector<std::vector<double>> matB;
    std::vector<std::vector<double>> matCtmp;

    for (int i = 1; i < numNodes; i++) {
        std::vector<double> tmp(numNodes - 1, 0);
        double tot = 0;
        for (int j = 1; j < numNodes; j++) {
            if (i != j && (mp.count({i, j}) || mp.count({j, i}))) {
                tmp[j - 1] = 1.0 / edges.size();
                tot += tmp[j - 1];
            }
        }
        if (mp.count({i, 0}) || mp.count({0, i})) {
            tot += 1.0 / edges.size();
        } 
        if (fabs(1 - tot) <= 1e-8) tmp[i - 1] = 0;
        else tmp[i - 1] = 1 - tot;
        matB.push_back(tmp);
    }

    for (int i = 1; i < numNodes; i++) {
        int flag = 0;
        for (auto [fi, se]: edges) {
            fi--;
            se--;
            if ((fi == i && se == 0) || (se == i && fi == 0)) {
                flag = 1;
            }
        }
        if (flag) {
            matCtmp.push_back({1.0 / edges.size()});
        }
        else {
            matCtmp.push_back({0});
        }
    }
    writeMatrixToCSV(matB, "/home/maoqianfei/PP/src/util/RandomWalk/B_matrix.csv");
    writeMatrixToCSV(matCtmp, "/home/maoqianfei/PP/src/util/RandomWalk/C_matrix.csv");
    std::cout << "matrix dump!" << std::endl;
}

bool RandomConnectedGraph::hasEdges() const {
    return numNodes >= 2;
}

std::pair<size_t, size_t> RandomConnectedGraph::selectRandomEdge() {
    
    /*
    std::cout << "debug edges ... " << std::endl;
    for (std::pair<size_t, size_t> p: edges) {
        std::cout << p.first << " " << p.second << '\n';
    }
    */

    std::uniform_int_distribution<size_t> dist(0, edges.size() - 1);
    size_t i = dist(rng);
    return { edges[i].first - 1, edges[i].second - 1};
}