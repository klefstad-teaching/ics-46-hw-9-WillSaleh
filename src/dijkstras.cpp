#include "dijkstras.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <stack>

vector<int> dijkstra_shortest_path(const Graph &G, int source, vector<int> &previous) {
    int n = G.numVertices;
    vector<int> distances(n, INF);
    previous.assign(n, -1);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>>pq;
    distances[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int current = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (current > distances[u]) {
            continue;
        }

        for (const Edge& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            int new_dist = distances[u] + weight;

            if (new_dist < distances[v]) {
                distances[v] = new_dist;
                previous[v] = u;
                pq.push({new_dist, v});
            }
        }
    }
    return distances;
}

vector<int> extract_shortest_path(const vector<int> &, const vector<int> &previous, int destination) {
    if (previous[destination] == -1) {
        return {};
    }

    stack<int> s;
    for (int v = destination; v != -1; v = previous[v]) {
        s.push(v);
    }

    vector<int> path;
    while (!s.empty()) {
        path.push_back(s.top());
        s.pop();
    }

    return path;
}

void print_path(const vector<int>& v, int total) {
    if (v.empty()) {
        cout << "No path found.\n";
        return;
    }
    
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i != v.size() - 1) {
            cout << " "; 
        }
    }
    cout << endl;
    cout << "Total cost is " << total << endl;
}