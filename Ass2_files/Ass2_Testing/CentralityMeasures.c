/*
Author: Kim
Date: 2019/11/15
Student ID: z5191441
Purpose: Calculate the closeness, betweenness, normalized betweenness centrality with a given graph.
Structure: Using the Dijkstra's Algorithm to calculate the minimal distance.
For centrality basically loop through each node iteratively and apply formula. For betweenness it is a bit more complex, need to use recursion to calculate the num of prenodes and possible reaches and divide them with each other then add up all the sum. 
Constrains: Only a small portion of centrality are implemented in this code. Lot more might be implemented and studied in the future.
Reference: <stdio.h> library
           <stdlib.h> library
           "Graph.h" given by unsw comp2521 acadamic group
           "Dijkstra.h" given by unsw comp2521 acadamic group and implemented by kim.
*/

#include"CentralityMeasures.h"
#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include"Dijkstra.h"

//this function is going to calculate the centrality
double centralcal(Graph g,Vertex v);
//this function is going to do the calculation for betweenness
void betweencal(Graph g,double *values);
//this function is going to count how many predprocessors total
double countprocessor(Vertex st,Vertex to,Graph g,ShortestPaths sp);
//this is going to fill betweenness values array
void fill_bet(double pr,double *values,int to,int st,Graph g);

//this function is like the main function for closeness
NodeValues closenessCentrality(Graph g){
    //initialize a basic NodeValues
    NodeValues close;
    close.numNodes = GraphNumVertices(g);
    close.values = malloc(close.numNodes*sizeof(double));
    //start from vertex zero
    Vertex i = 0;
   //loop through all vertex 
    while(i < close.numNodes){
        (close.values)[i] = centralcal(g,i);
        i++;
    }
    return close;            
}

//this calculate the centrality of each one!
double centralcal(Graph g,Vertex v){
    //the sum
    double sum = 0;
    ShortestPaths sp = dijkstra(g, v);
    //how many can reach
    double reach =0;
    //use while loop to calculate the reach
    for(int i = 0;i<GraphNumVertices(g);i++){
        sum+= (sp.dist)[i];
        if((sp.dist)[i]!=0){
            reach++;
        }
    }
    freeShortestPaths(sp);
    //for the special case sum is 0(cant divide by 0!)
    if (sum==0){       
        return 0;
    }
    //apply centrality formula
    double central = reach*reach/(GraphNumVertices(g)-1)/sum;
    return central;
}

//this function is like the main function for betweenness
NodeValues betweennessCentrality(Graph g){
    //initialize the NODEVALUES
    NodeValues between;
    int ver_num = GraphNumVertices(g);
    between.numNodes = ver_num;
    between.values = malloc(ver_num*sizeof(double));
    //initialize them to be 0 at first 
    for(int i = 0;i<ver_num;i++){
        between.values[i] = 0;
    }   
    //apply to calculate the real values
    betweencal(g,between.values);    
    return between;
}

//this function do the real job to calculate the values and fill them
//between node cant be end or start!
void betweencal(Graph g,double *values){
    //vertex_st start from 0
    int node_st = 0;
    //use while loop to do each vertex
    while(node_st <GraphNumVertices(g)){
        ShortestPaths sp = dijkstra(g, node_st);
        //and each to vertex
        int node_to = 0;
        while(node_to <GraphNumVertices(g)){
            //this counts how many preprocessors in the paths totally
            double pr = countprocessor(node_st,node_to,g,sp);
            PredNode* cur =sp.pred[node_to];
            while(cur!=NULL){
                //fill values array
                fill_bet(pr,values,cur->v,node_st,g); 
                cur=cur->next;
            }        
            node_to++;
        }
        node_st++;
        freeShortestPaths(sp);
    }
}        

//this fills the betweenness values recursively
void fill_bet(double pr,double *values,int to,int st,Graph g){
    //if pr is 0 
    if (pr==0){
        return;
    }
    //base case:
    if(to == st){
        return;
    }
    ShortestPaths sp = dijkstra(g, st);
    //count pr again
    double cp = countprocessor(st,to,g,sp);//
    //add up the value
    values[to]+=cp/pr;
    PredNode *cur = (sp.pred)[to];
    //go through the graph and fill each one
    while(cur!=NULL){
        fill_bet(pr,values,cur->v,st,g);
        cur=cur->next;
    }
    freeShortestPaths(sp);
}

//this counts the preprocessors on the paths totally
//sp.pred can has more than one node!!!!!
//better do it recursively
double countprocessor(Vertex st,Vertex to,Graph g,ShortestPaths sp){
    //base case:
    if(to == st){
        return 1;
    }
    double count = 0;
    PredNode *cur = sp.pred[to];
    //loop to count;
    while(cur!=NULL){
        //count with recursion;
        count+=countprocessor(st,cur->v,g,sp);
        cur=cur->next;
    }
    //freeShortestPaths(sp);
    return count;    
}

//this calculate the normalised betweenness
NodeValues betweennessCentralityNormalised(Graph g){
    NodeValues between = betweennessCentrality(g);
    int ver_num = GraphNumVertices(g);
    for(int i = 0;i<ver_num;i++){
        between.values[i] = between.values[i]/(ver_num-1)/(ver_num-2);
    }
    return between;
}

//this prints the node values
void showNodeValues(NodeValues nvs){
    for (int i = 0;i<nvs.numNodes;i++){
        printf("%d: %lf\n",i,(nvs.values)[i]);
    }
}

//this free the associated memory of the node
void freeNodeValues(NodeValues nvs){
    free(nvs.values);    
}
