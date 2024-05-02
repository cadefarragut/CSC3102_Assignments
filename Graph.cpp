// Cade Farragut and Abdulla Asad

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <climits>
#include <queue>
#include <sstream>

using namespace std;

class Edge;

class Vertex {
public:
    string code;
    int dvalue;
    Edge* parentedge;
    vector<Edge*> adjlist;
   Vertex(string code) : code(code), dvalue(INT_MAX), parentedge(nullptr) {} 
};

class Edge {
public:
    int depttime, arrtime;
    Vertex* origin;
    Vertex* dest;
    string airline, flno;

    Edge(const string& airline, const string& flno, Vertex* origin, Vertex* dest, int depttime, int arrtime)
        : airline(airline), flno(flno), origin(origin), dest(dest), depttime(depttime), arrtime(arrtime) {}
};

class CompareVertices {
public:
    bool operator()(const Vertex* v1, const Vertex* v2) {
        return v1->dvalue > v2->dvalue; // Min-heap based on dvalue
    }
};

class Graph {
public:
    map<string, Vertex*> vmap;
    vector<Edge*> elist;
    priority_queue<Vertex*, vector<Vertex*>, CompareVertices> heap;

    void readAirports(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            exit(EXIT_FAILURE);
        }

        string line;
        while (getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty()) 
                continue;

            Vertex* v = new Vertex(line);
            vmap[line] = v;
        }

        file.close(); 
    }

    void readFlights(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            exit(EXIT_FAILURE);
        }

        string line;
        getline(file, line);
        while (getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty()) 
                continue;

            istringstream iss(line);
            string airline, flno, originCode, destCode;
            int depttime, arrtime;

            if (!(iss >> airline >> flno >> originCode >> destCode >> depttime >> arrtime)) {
                continue;
            }

            if (vmap.find(originCode) == vmap.end() || vmap.find(destCode) == vmap.end()) {
                continue;
            }

            Vertex* origin = vmap[originCode];
            Vertex* dest = vmap[destCode];
            Edge* e = new Edge(airline, flno, origin, dest, depttime, arrtime);
            elist.push_back(e);
            origin->adjlist.push_back(e);
        }

        file.close(); 
    }

    int dijkstra(const string& srcCode, const string& sinkCode, int starttime) {
        if (vmap.find(srcCode) == vmap.end() || vmap.find(sinkCode) == vmap.end()) {
            return -1; 
        }

        for (auto& pair : vmap) {
            pair.second->dvalue = INT_MAX;
            pair.second->parentedge = nullptr;
        }

        Vertex* src = vmap[srcCode];
        src->dvalue = starttime;
        heap.push(src);
        map<Vertex*, bool> visited;

        while (!heap.empty()) {
            Vertex* u = heap.top();
            heap.pop();

            if (visited[u]) continue;  
            visited[u] = true;         

            if (u->code == sinkCode) {
                return u->dvalue;
            }

            for (Edge* e : u->adjlist) {
                if (e->depttime >= u->dvalue) {
                    relax(e);
                }
            }
        }

        return -1;
    }

    void decreaseKey(Vertex* v, int key) {
        v->dvalue = key;
        heap.push(v);
    }

    void relax(Edge* e) {
        Vertex* u = e->origin;
        Vertex* v = e->dest; 

        if (u->dvalue <= e->depttime && e->arrtime < v->dvalue) {
            v->dvalue = e->arrtime;
            v->parentedge = e;
            decreaseKey(v, e->arrtime);
        }
    }

    void printPath(const string& code) {
        Edge* e = vmap[code]->parentedge;
        if (!e) return;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        return EXIT_FAILURE;
    }

    string airportsFile = "airports.txt";
    string flightsFile = "flights.txt";

    Graph graph;
    graph.readAirports(airportsFile);
    graph.readFlights(flightsFile);

    int startTime = stoi(argv[3]);
    int shortestPath = graph.dijkstra(argv[1], argv[2], startTime);
    cout << shortestPath << endl;

    return 0;
}