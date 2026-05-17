#include <iostream>
#include <vector>
#include <stack>
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

    // Parallel DFS
    void parallelDFS(int start) {

        vector<bool> visited(V, false);

        stack<int> st;

        st.push(start);

        cout << "\nDFS Traversal:\n";

        while(!st.empty()) {

            int node;

            // Safe Pop
            #pragma omp critical
            {
                node = st.top();
                st.pop();
            }

            if(!visited[node]) {

                // Mark Visited
                #pragma omp critical
                {
                    if(!visited[node]) {

                        visited[node] = true;
                        cout << node << " ";
                    }
                }

                // Push Neighbors
                #pragma omp parallel for
                for(int i = 0; i < adj[node].size(); i++) {

                    int nbr = adj[node][i];

                    if(!visited[nbr]) {

                        #pragma omp critical
                        {
                            if(!visited[nbr]) {
                                st.push(nbr);
                            }
                        }
                    }
                }
            }
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

    g.parallelDFS(0);

    return 0;
}