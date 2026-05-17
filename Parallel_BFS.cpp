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

    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    g.displayGraph();

    g.parallelBFS(0);

    return 0;
}