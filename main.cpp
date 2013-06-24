/* 
 * File:   main.cpp
 * Author: artur
 *
 * Created on 28 kwiecień 2012, 13:14
 */
#include<cstdio>
#include <iostream>
#include <cstdlib>
#include<vector>
#include<set>
#include <numeric>
using namespace std;
const int infty = 1000000000;
int verticesNum;
int edgesNum;
vector< vector< pair<int, int> > > comparisionMatrix;
vector<bool> verticesInMST;
vector<int> edgeWeight;
vector<int> mstClosingVertices;

struct cmp // warunek kopca
{

    bool operator() (const int &a, const int &b) {
        if (edgeWeight[a] < edgeWeight[b]) return true;
        if (edgeWeight[a] > edgeWeight[b]) return false;
        return a<b;
    }
};

set<int, cmp> kopiec; // kopiec kolejka prio zawsze bierzemy najbliższego sąsiada aktualnego MST

void primDijkstra(int s) {
    int v, u, c;

    edgeWeight.clear();
    edgeWeight.resize(verticesNum, infty);

    // MST to drzewo zawierające wierzchołki, których nie w zbiorze kopiec,
    // czyli na razie jest ono pustym drzewem.
    verticesInMST.clear();
    verticesInMST.resize(verticesNum, false);

    mstClosingVertices.resize(verticesNum);

    edgeWeight[s] = 0;
    mstClosingVertices[s] = s;

    kopiec.clear();


    for (int i = 0; i < verticesNum; i++) { //wrzucam wierzchołki do kopca
        kopiec.insert(i);

    }


    while (!kopiec.empty()) //przechodzimy kopiec do skutku
    {
        u = *(kopiec.begin()); // weź wierzchołek najbliżej drzewa MST

        kopiec.erase(kopiec.begin());


        verticesInMST[u] = true; // dodajemy wierzchołek v do drzewa MST
        // a dodał go wierzchołek pi[u]

        for (int i = 0; i < comparisionMatrix[u].size(); i++) //połączenia danego wierzchołka i koszty
        {
            v = comparisionMatrix[u][i].first;
            if (!verticesInMST[v]) {
                c = comparisionMatrix[u][i].second;
                if (c < edgeWeight[v]) // w alg. Dijkstry jest tutaj edgeWeight[u]+c < edgeWeight[v]
                {
                    // uaktualniamy wagę wierzchołka v - poprawnośc przez indukcję, dla co raz większego drzewa mst wybieramy krawędzie o najmiejszej wadze
                    kopiec.erase(kopiec.find(v)); //bo już nie spełnia warunku kopca
                    edgeWeight[v] = c; // w alg. Dijkstry jest tutaj edgeWeight[v]=edgeWeight[u]+c;
                    kopiec.insert(v); //bo moga być dołączone inne a trzeba przebudowac kopiec/
                    mstClosingVertices[v] = u; // domykam krawędź
                }
            }
        }
    }
}

/*
 * 
 */
int main(int argc, char** argv) {

    int a, b, c;
    FILE *input = fopen("graph.txt", "r+");
    fscanf(input, "%d %d", &verticesNum, &edgesNum);
    comparisionMatrix.resize(verticesNum);
    bool directed = false;
    fscanf(input, "%d", &directed);
    for (int i = 0; i < edgesNum; i++) {
        fscanf(input, "%d %d %d", &a, &b, &c); // c = koszt krawędzi od a do b
        comparisionMatrix[a].push_back(make_pair(b, c));
        if (!directed)
            comparisionMatrix[b].push_back(make_pair(a, c)); // uwaga na spójność grafu
        // comparisionMatrix[b].push_back(make_pair(a, c));
    }

    //START
    int s = 0;
    fscanf(input, "%d", &s);

    primDijkstra(s);

    int weightMST = std::accumulate(edgeWeight.begin(), edgeWeight.end(), 0);
    if (weightMST < infty) {
        printf("[PRIM] start od V %d  koszt MST w grafie %s wynosi %d\n", s, (directed) ? "skierowanym" : "nie skierowanym", weightMST);

        for (int i = 0; i < verticesNum; i++) {
            if (edgeWeight[i] != 0)
                printf("%d -- %d [Koszt: %d]\n", i, mstClosingVertices[i], edgeWeight[i]);
        }
    } else {
        printf("[PRIM] start od V %d w grafie %s , graf nie jest spójny\n", s, (directed) ? "skierowanym" : "nie skierowanym");
    }
    return 0;
}

