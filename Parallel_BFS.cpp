#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

class Graph {

    int V;
    vector<vector<int>> adj;

public:

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Add Edge
    void addEdge(int u, int v) {

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Display Graph
    void displayGraph() {

        cout << "Graph:\n";

        for(int i = 0; i < V; i++) {

            cout << i << " -> ";

            for(int nbr : adj[i]) {
                cout << nbr << " ";
            }

            cout << endl;
        }
    }

    // Parallel BFS
    void parallelBFS(int start) {

        vector<bool> visited(V, false);

        vector<int> curr, next;

        curr.push_back(start);
        visited[start] = true;

        cout << "\nBFS Traversal:\n";

        while(!curr.empty()) {

            next.clear();

            #pragma omp parallel for
            for(int i = 0; i < curr.size(); i++) {

                int node = curr[i];

                #pragma omp critical
                cout << node << " ";

                for(int nbr : adj[node]) {

                    if(!visited[nbr]) {

                        #pragma omp critical
                        {
                            if(!visited[nbr]) {

                                visited[nbr] = true;
                                next.push_back(nbr);
                            }
                        }
                    }
                }
            }

            curr = next;
        }
    }
};

int main() {

    int V, e;

    cout << "Enter number of vertices: ";
    cin >> V;

    Graph g(V);

    cout << "Enter number of edges: ";
    cin >> e;

    cout << "Enter edges (u v):\n";

    for (int i = 0; i < e; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter starting vertex: ";
    cin >> start;
    
    g.displayGraph();	

    g.parallelBFS(start);

    return 0;
}
