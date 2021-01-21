// Dijkstra ADT implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <unistd.h>
#include <string.h>

#include "Graph.h"
#include "Dijkstra.h"
#include "PQ.h"
static void PNInsert(ShortestPath sp,Vertex v);
ShortestPath dijkstra(Graph g, Vertex src){
    //malloc a shortestpath struct and related stuff
    ShortestPaths sp;

    //pq of vertices ordered by dist[]value;
    PQ vpq = PQNew();
     
    //initialze dist[all] to -1 //should i modify it to inf?
    int vertice = GraphNumVertices(g);
    for(int i = 0;i<vertice;i++){
        (sp.dist)[i] = -1;
    }
    //initialize dist[src] to 0;(double check this?? )
    (sp.dist)[src] = 0;
    //initialise pred[] to all NULL;
    for(int t = 0;t<vertice;t++){
        (sp.pred)[t] = NULL;
    }
    //add source vertex to pq
    Item source;
    source.key = src;
    source.weight = 0;
    PQAdd(vpq, source);
    
    Vertex u;
    while(!PQIsEmpty(vpq)){
        //take vertex with minimum dist[]value off PQ
        Item iu = PQueueLeave(vpq);
        u = iu.key;
        //loop through all adjacent out of u
        AdjList adjacent = GraphOutIncident(g, u);
        AdjList cur = adjacent;
        while(cur != NULL){
            //if there is a shorter path to v through u
            //havnt touch before            
            if(sp.dist[cur->v] == -1){
                (sp.dist)[cur->v] = (sp.dist)[u]+cur->weight;
                
            }else{ // been modified before
                if((sp.dist)[cur->v] > (sp.dist)[u]+cur->weight){
                    (sp.dist)[cur->v] =(sp.dist)[u]+cur->weight;
                }
            }
            //add prednode here
            PNInsert(sp.pred[cur->v],u);
            Item new;
            new.key = cur->v;
            new.value = cur->weight;
            PQAdd(new,cur->v);
        }
        //free the adjacent list
        cur = adjacent;
        while (cur!= NULL){
            AdjList hold = cur;
            cur = cur->next;
            free(hold);
        } 
    }
    PQFree(vpq);
    return sp;
}                


static void PNInsert(PredNode* pn,Vertex w ){
    PredNode* new = malloc(sizeof(new));
    new->next = NULL;
    new->v=w;
    if(pn == NULL){
        pn = new;
    }else{
        PredNode* cur = pn;
        while(cur->next != NULL){
            cur = cur->next;
        }
        cur->next = new;
    }
}




    

void showShortestPaths(ShortestPaths sps) {

}

void freeShortestPaths(ShortestPaths sps) {

}

