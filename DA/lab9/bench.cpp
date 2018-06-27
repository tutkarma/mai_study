#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <algorithm>

bool DFS(std::vector<std::vector<int> > &graph, std::vector<int> &parent, std::vector<bool> visited, int x, int t)
{
    if (x == t)
        return true;
    visited[x] = true;

    for (int i = 1; i < graph.size(); ++i) {
        if (graph[x][i] > 0 && !visited[i]) {
            parent[i] = x;
            if (DFS(graph, parent, visited, i, t))
                return true;
        }
    }
    return false;
}

long long FordFulkerson(std::vector<std::vector<int> > &graph, int start, int end)
{
    int u, v;
    long long maxFlow = 0;
    std::vector<std::vector<int> > resGraph = graph;
    std::vector<int> parent(graph.size());
    std::vector<bool> visited(graph.size());

    while (DFS(resGraph, parent, visited, start, end)) {
        int flow = std::numeric_limits<int>::max();
        for (v = end; v != start; v = parent[v]) {
            u = parent[v];
            flow = std::min(flow, resGraph[u][v]);
        }

        for (v = end; v != start; v = parent[v]) {
            u = parent[v];
            resGraph[u][v] -= flow;
            resGraph[v][u] += flow;
        }

        maxFlow += flow;
    }

    return maxFlow;
}

int main(void)
{
    clock_t start = clock();
    int n = 0, m = 0, from = 0, to = 0;
    std::cin >> n >> m;
    std::vector<std::vector<int> > graph;
    graph.resize(n + 1);
    for (int i = 0; i < n + 1; ++i) {
        graph[i].resize(n + 1);
    }
    for (int i = 0; i < m; ++i) {
        std::cin >> from >> to;
        std::cin >> graph[from][to];
    }

    FordFulkerson(graph, 1, n);
    //std::cout << FordFulkerson(graph, 1, n) << std::endl;
    clock_t end = clock();
    std::cout << "Ford-Fulkerson with DFS time: " << (double)(end - start) / CLOCKS_PER_SEC << " sec." << std::endl;
    return 0;
}
