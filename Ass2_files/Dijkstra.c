#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "PQ.h"
#include "Graph.h"
#include "Dijkstra.h"
/**
 * Finds  all  shortest  paths  from  a given source vertex to all other
 * vertices as discussed in the lectures.
 *
 * This  function  offers  one **additional feature** - if there is more
 * than one shortest path from the source vertex to  another  vertex,  a
 * vertex  could  have  multiple predecessors (see spec for an example).
 * The function must keep track of multiple predecessors for each vertex
 * (if  they  exist)  by storing the predecessor(s) in the corresponding
 * linked list for that vertex. This will be discussed in detail in  the
 * lecture.
 * 
 * For  example,  suppose that while finding the shortest paths from the
 * source vertex 0 to all other vertices, we discovered  that  vertex  1
 * has two possible predecessors on different shortest paths. 
 * 
 * Node 0
 *   Distance
 *     0 : X
 *     1 : 2
 *     2 : 1
 *   Preds
 *     0 : NULL
 *     1 : [0]->[2]->NULL
 *     2 : [0]->NULL
 * 
 * Node 1
 *   Distance
 *     0 : 2
 *     1 : X
 *     2 : 3
 *   Preds
 *     0 : [1]->NULL
 *     1 : NULL
 *     2 : [0]->NULL
 * 
 * Node 2
 *   Distance
 *     0 : 3
 *     1 : 1
 *     2 : X
 *   Preds
 *     0 : [1]->NULL
 *     1 : [2]->NULL
 *     2 : NULL
 * 
 * The  function  returns  a 'ShortestPaths' structure with the required
 * information:
 * - the number of vertices in the graph
 * - the source vertex
 * - distance array
 * - array of predecessor lists
 */
static void PNInsert(ShortestPath sp,Vertex v);
ShortestPaths dijkstra(Graph g, Vertex src){
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




    
/**
 * This  function  is  for  you to print out the ShortestPaths structure
 * while you are debugging/testing your implementation. 
 * 
 * We  will  not call this function during testing, so you may print out
 * the given ShortestPaths structure in whatever format you want.
 */
void showShortestPaths(ShortestPaths sps){
}
/**
 * Frees all memory associated with the given ShortestPaths structure.
 */
void freeShortestPaths(ShortestPaths sps){
}
    free(dist);
    PredNode *cur = sps.pred
