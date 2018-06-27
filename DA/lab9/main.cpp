#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <algorithm>

bool BFS(std::vector<std::vector<int> > &graph, int start, int end, std::vector<int> &parent)
{
    std::vector<bool> visited(graph.size());
    std::queue <int> q;
    q.push(start);
    visited[start] = true;
    parent[start] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v = 1; v < graph.size(); ++v) {
            if (!visited[v] && graph[u][v]) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return visited[end];
}

long long FordFulkerson(std::vector<std::vector<int> > &graph, int start, int end)
{
    int u, v, maxFlow = 0;
    std::vector<std::vector<int> > resGraph = graph;
    std::vector<int> parent(graph.size());

    while (BFS(resGraph, start, end, parent)) {
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
    std::cout << "Ford-Fulkerson with BFS time: " << (double)(end - start) / CLOCKS_PER_SEC << " sec." << std::endl;
    return 0;
}