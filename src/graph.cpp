#include "graph.h"
#include "wikinode.h"
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#define ARTICLES "../dataset/articles.tsv"
#define LINKS "../dataset/links.tsv"
#define NUM_ARTICLES 4604
#define NUM_LINKS 119882

using namespace std;

Graph::Graph(){
    createGraphFromFile(ARTICLES, LINKS);
}


Graph::~Graph(){
    /* TODO bc memory ew */
}

/*
 * Returns pointer to WikiNode based on article name.
 * @param page_name Name of the page to find.
 * @return Pointer to WikiNode corresponding to page_name or NULL if does not exist.
 */
WikiNode* Graph::getPage(string page_name){
    map<string, WikiNode*>::iterator it = name_node_map.find(page_name);
    return (it != name_node_map.end()) ? it->second : NULL;
}

void printProgress(int count, int total){
    cout << setprecision(2);
    int percent = 100*(float)count/total;
    cout << "\r[";
    for(int i = 0; i < 100; i++){
        if(i < percent) 
            cout << "#";
        else
            cout << " ";
    }
    cout << "]" << percent << "%";
}

/*
 * Parses Wiki data to create graph.
 * @param articles_path Path to file containing graph data
 * @param links_path Path to file containing links data
 */
void Graph::createGraphFromFile(string articles_path, string links_path){
    /* 
    Parses through and create WikiNodes. Add these via pointer to the map.
    Dataset says to use URLDecorder (Java) to decode article names.
    https://docs.oracle.com/javase/7/docs/api/java/net/URLDecoder.html
    We can just implement this ourselves and it shouldn't be too bad.

    General process:
    - For each article name in articles.tsv, create a WikiNode
        > Article names can be decoded when displaying names/paths. AKA dw abt it right now
    - For each link in links.tsv, addLink() to current WikiNode
    - Call addNode() on graph to insert node
     */
    ifstream articles(articles_path), links(links_path);
    string name, line, linked;

    /* Add nodes for each article (no links yet) */
    cout << "\n-----LOADING ARTICLES-----" << endl;
    int count = 1;
    while(getline(articles, name)){
        addNode(new WikiNode(name));
        printProgress(count++, NUM_ARTICLES);
    }

    /* Go through link lines (article + spaces + linked article)*/
    cout << "\n-----LOADING LINKS-----" << endl;
    count = 0;

    while(getline(links, line)){
        name = line.substr(0, line.find('	'));        //up to tab is the article name
        linked = line.substr(line.find('	') + 1);    //past the tab is the linked article
        getPage(name)->addConnection(getPage(linked));  //add a link from "name" to "linked"
        printProgress(count++, NUM_LINKS);
    }
    cout << "\n-----DONE-----" << endl;
}

/* 
 * Inserts a new node into the graph.
 * @param node Pointer to node to be added
 */
void Graph::addNode(WikiNode* node){
    name_node_map.insert({node->getName(), node});
}