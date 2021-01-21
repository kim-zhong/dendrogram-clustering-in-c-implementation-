/*
Author: Kim
Date: 2019/11/15
Student ID: z5191441
Purpose: implement the Dijkstra's Algorithm to find the shortest path from a given vertex to all other nodes and show them out and free them.
Structure: Using a priorty Queue to store the nodes and a dist array and a prednode array to contain related info. Using while loop to fill each vertex iteratively.
Constrains: Actually we might be able to implement with other structure (e.g. with sets). To calculate the shortestpath we might have other algos as well.
Reference: <Ctype.h> library
           <stdio.h> library
           <stdlib.h> library
           <unistd.h> library
           <string.h> library
           "PQ.h" given by unsw comp2521 acadamic group
           "Graph.h" given by unsw comp2521 acadamic group
           "Dijkstra.h" given by unsw comp2521 acadamic group
*/



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

//this function can help insert a node into the prednode linklist
static void PNInsert(ShortestPaths sp,Vertex u,Vertex w);
//this function will free the prednode related linklist memory
static void dropPN(ShortestPaths sp,Vertex u);

ShortestPaths dijkstra(Graph g, Vertex src){
    //malloc a shortestpath struct and related stuff
    ShortestPaths sp;
    int vertice = GraphNumVertices(g);
    sp.pred = malloc(vertice*sizeof(PredNode*));
    sp.dist = malloc(vertice*sizeof(int));
    sp.numNodes = vertice;
    sp.src = src;
    //pq of vertices ordered by dist[]value;
    PQ vpq = PQNew();    
    //initialze dist[all] to -1 
    for(int i = 0;i<vertice;i++){
        (sp.dist)[i] = -1;
    }
    //initialize dist[src] to 0;
    (sp.dist)[src] = 0;
    //initialise pred[] to all NULL;
    for(int t = 0;t<vertice;t++){
        (sp.pred)[t] = NULL;
    }
    //add source vertex to pq
    ItemPQ source;
    source.key = src;
    source.value = 0;
    PQAdd(vpq, source);
    
    Vertex u;
    while(!PQIsEmpty(vpq)){
        //take vertex with minimum dist[]value off PQ
        ItemPQ iu = PQDequeue(vpq);
        u = iu.key;
        //loop through all adjacent out of u        
        AdjList adjacent = GraphOutIncident(g, u);        
        AdjList cur = adjacent;
        while(cur != NULL){
            //if there is a shorter path to v through u
            //havnt touch before            
            if(sp.dist[cur->v] == -1){
                (sp.dist)[cur->v] = (sp.dist)[u]+cur->weight;
                //add prednode here
                PNInsert(sp,cur->v,u);                
                ItemPQ new;
                new.key = cur->v;
                new.value = cur->weight;
                PQAdd(vpq,new);
            // been modified before
            }else{ 
                if((sp.dist)[cur->v] > (sp.dist)[u]+cur->weight){
                    (sp.dist)[cur->v] =(sp.dist)[u]+cur->weight;
                    //add prednode here
                    dropPN(sp,cur->v);
                    (sp.pred)[cur->v]=NULL;
                    PNInsert(sp,cur->v,u);
                    //add to the queue
                    ItemPQ new;
            	    new.key = cur->v;
            	    new.value = cur->weight;
            	    PQAdd(vpq,new);
                }else if((sp.dist)[cur->v] == (sp.dist)[u]+cur->weight){
                    //if not in the pred then do, otherwise not
                    //add prednode here                   
                    PNInsert(sp,cur->v,u);                    
                    ItemPQ new;
            	    new.key = cur->v;
            	    new.value = cur->weight;
            	    PQAdd(vpq,new);
                }
            }
            cur = cur->next;                      
        }
        
    }
    //process those isolated vertices if any
    int i = 0;
    while(i<vertice){
        if((sp.dist)[i]==-1){
            (sp.dist)[i]=0;
        }
        i++;
    }
    PQFree(vpq);
    return sp;
}                
//this function will help insert a node to the prenode linklist
static void PNInsert(ShortestPaths sp,Vertex u,Vertex w){
    //when the prednode list is empty
    if((sp.pred)[u]== NULL){
        (sp.pred)[u] = malloc(sizeof(PredNode));
        (sp.pred)[u]->v = w;
        (sp.pred)[u]->next = NULL;
    }else{
       //check duplicate
       PredNode *check = (sp.pred)[u];
       while(check!=NULL){
           if(check->v == w){
               return;
           }
           check = check->next;
       }
       //create a node
       PredNode* new = malloc(sizeof(PredNode));
       new->v = w;
       new->next = NULL;
       //loop and add to the linklist
       PredNode* cur = (sp.pred)[u];
       while(cur->next != NULL){
           cur = cur->next;
       }
       cur->next = new;
    }
}       

//this function will release the associated memory with the prednode
static void dropPN(ShortestPaths sp,Vertex u){
    //if already null then do nothing
    if((sp.pred)[u] == NULL){
        return;
    }
    //otherwise loop to free!
    PredNode* cur = (sp.pred)[u];
    while(cur!=NULL){
        PredNode* hold = cur;
        cur= cur->next;
        free (hold);
    }
}

   
/**
 * This  function  is  for  you to print out the ShortestPaths structure
 * while you are debugging/testing your implementation. 
 * 
 * We  will  not call this function during testing, so you may print out
 * the given ShortestPaths structure in whatever format you want.
 */
//a function to show the sps since it is not essential for test then
//i dont put too much comment
void showShortestPaths(ShortestPaths sps){
    printf("Node %d\n",sps.src);
    printf("Distance\n");
    int i = 0;
    while(i < sps.numNodes){
        printf("%d : %d\n",i,(sps.dist)[i]);
        i++;
    }
    i=0;
    printf("Preds:\n");
    while(i < sps.numNodes){
        printf("%d :",i);
        PredNode* cur = (sps.pred)[i];
        while(cur!=NULL){
            printf("%d",cur->v);
            cur= cur->next;
        }
        i++;
    }
}

/**
 * Frees all memory associated with the given ShortestPaths structure.
 */
void freeShortestPaths(ShortestPaths sps){
    free(sps.dist);
    int num =sps.numNodes;
    for(int i = 0;i<num;i++){
        dropPN(sps,i);
    }
    free(sps.pred);
    
}
