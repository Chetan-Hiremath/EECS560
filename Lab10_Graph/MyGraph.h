#ifndef _MYGRAPH_H_
#define _MYGRAPH_H_

#include <iostream>
#include <fstream>

#include "MyVector.h"
#include "MyLinkedList.h"
#include "MyQueue.h"
#include "MyStack.h"
#include "MyHashTable.h"


// the graph is supposed to be a undirected graph
// no multi-edge is allowed (assumed to be ensured from the input)

typedef long long VertexIDType;
typedef long long EdgeIDType;

template <typename VertexDataType, typename EdgeDataType>
class MyGraph
{
  public:

    class Vertex
    {
      public: 
        // default constructor
        Vertex()
        {
            return;
        }

        // vertex copy constructor
        explicit Vertex(const VertexDataType& vertex_data) :
            data(vertex_data)
        {
            return;
        }

        // vertex move constructor
        explicit Vertex(VertexDataType && vertex_data) :
            data(std::move(vertex_data))
        {
            return;
        }

        // copy assignment
        Vertex& operator=(const Vertex& rhs)
        {
            id = rhs.id;
            tag = rhs.tag;
            data = rhs.data;
            return *this;
        }

        // move assignment
        Vertex& operator=(Vertex && rhs)
        {
            id = rhs.id;
            tag = rhs.tag;
            data = std::move(rhs.data);
            return *this;
        }

        VertexIDType id;        // the unique ID of the vertex
        int tag;                // a reserved tag variable
        VertexDataType data;    // the property associated with the vertex

        friend class MyGraph;

    };

    class Edge
    {
      public:
        // default constructor
        Edge()
        {
            return;
        }

        // copy constructor
        // sid: source vertex ID; vid: target vertex ID
        explicit Edge(const VertexIDType sid, const VertexIDType tid, const EdgeDataType& edge_data) :
            src(sid),
            tgt(tid),
            data(edge_data)
        {
            return;
        }

        // move constructor
        // sid: source vertex ID; vid: target vertex ID
        explicit Edge(const VertexIDType sid, const VertexIDType tid, EdgeDataType && edge_data) :
            src(sid),
            tgt(tid),
            data(std::move(edge_data))
        {
            return;
        }

        // copy assignment
        Edge& operator=(const Edge& rhs)
        {
            id = rhs.id;
            src = rhs.src;
            tgt = rhs.tgt;
            tag = rhs.tag;
            data = rhs.data;
            return *this;
        }

        // move assignment
        Edge& operator=(Edge && rhs)
        {
            id = rhs.id;
            src = rhs.src;
            tgt = rhs.tgt;
            tag = rhs.tag;
            data = std::move(rhs.data);
            return *this;
        }
      
        EdgeIDType id;          // the unique ID of the edge
        VertexIDType src, tgt;  // the IDs for the source vertex and the target vertex of the edge
        int tag;                // the reserved tag varaible
        EdgeDataType data;      // the property associated with the edge

        friend class MyGraph;

    };

    // default constructor of graph
    // initialize the graph with a size of 0
    MyGraph() :
        current_vertex_ID(0),
        current_edge_ID(0),
        num_vertices(0),
        num_edges(0),
        vertex_set{0},
        edge_set{0},
        vertex_map{},
        edge_map{},
        adj_list{}
    {   
        return;
    }

    // destructor
    ~MyGraph()
    {
        for(size_t i = 0; i < vertex_set.size(); ++ i)
        {
            delete vertex_set[i];
        }
        for(size_t i = 0; i < edge_set.size(); ++ i)
        {
            delete edge_set[i];
        }
        for(size_t i = 0; i < adj_list.size(); ++ i)
        {
            adj_list[i]->clear();
            delete adj_list[i];
        }
        return;
    }

    // adds an vertex v to the graph (copy)
    // assign ID according the to oder of vertex insertion
    // for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex
    // returns the assigned vertex ID for v
    VertexIDType addVertex(const VertexDataType& v_data)
    {
      HashedObj<VertexIDType, size_t> newVertex(current_vertex_ID, num_vertices);
      vertex_map.insert(newVertex);
      Vertex* aVertex = new Vertex();
      aVertex->id = current_vertex_ID;
      aVertex->data = v_data;
      vertex_set.push_back(aVertex);
      adj_list.push_back(new MyLinkedList<EdgeIDType>);
      num_vertices++;
      return current_vertex_ID++;
    }

    // adds an vertex v to the graph (move)
    // assign ID according the to oder of vertex insertion
    // for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex
    // returns the assigned vertex ID for v
    VertexIDType addVertex(VertexDataType && v_data)
    {
      HashedObj<VertexIDType, size_t> newVertex(current_vertex_ID, num_vertices);
      vertex_map.insert(newVertex);
      Vertex* aVertex = new Vertex();
      aVertex->id = current_vertex_ID;
      aVertex->data = std::move(v_data);
      vertex_set.push_back(aVertex);
      adj_list.push_back(new MyLinkedList<EdgeIDType>);
      num_vertices++;
      return current_vertex_ID++;
    }

    // retrieves the information of the vertex specified by the vid
    // returns the vertex
    Vertex* getVertex(const VertexIDType vid)
    {
      return vertex_set[vertexID2SetPos(vid)];
    }

    // adds an edge e between the source node src and the target node tgt (copy)
    // adds to both of the src's adjacency list and tgt's adjacency list (as undirected graph)
    // assign ID according the to oder of edge insertion
    // for example, assign ID 1 to the first edge, 2 to the second edge, ..., and n to the nth edge
    // returns the assigned edge ID for e
    EdgeIDType addEdge(const VertexIDType sid, const VertexIDType tid, const EdgeDataType& e_data)
    {
      HashedObj<EdgeIDType, size_t> newEdge(current_edge_ID, num_edges);
      edge_map.insert(newEdge);
      Edge* aEdge = new Edge();
      aEdge->id = current_edge_ID;
      aEdge->data = e_data;
      aEdge->src = sid;
      aEdge->tgt = tid;
      edge_set.push_back(aEdge);
      num_edges++;
      HashedObj<VertexIDType, size_t> Sid, Tid;
      if (vertex_map.retrieve(sid, Sid) && vertex_map.retrieve(tid, Tid))
      {
        adj_list[Sid.value]->push_back(current_edge_ID);
        adj_list[Tid.value]->push_back(current_edge_ID);
      }
      return current_edge_ID++;
    }

    // adds an edge e between the source node src and the target node tgt (move)
    // adds to both of the src's adjacency list and tgt's adjacency list (as undirected graph)
    // assign ID according the to oder of vertex insertion
    // for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex
    // returns the assigned edge ID for e
    EdgeIDType addEdge(const VertexIDType sid, const VertexIDType tid, EdgeDataType && e_data)
    {
      HashedObj<EdgeIDType, size_t> newEdge(current_edge_ID, num_edges);
      edge_map.insert(newEdge);
      Edge* aEdge = new Edge();
      aEdge->id = current_edge_ID;
      aEdge->data = std::move(e_data);
      aEdge->src = sid;
      aEdge->tgt = tid;
      edge_set.push_back(aEdge);
      num_edges++;
      HashedObj<VertexIDType, size_t> Sid, Tid;
      if (vertex_map.retrieve(sid, Sid) && vertex_map.retrieve(tid, Tid))
      {
        adj_list[Sid.value]->push_back(current_edge_ID);
        adj_list[Tid.value]->push_back(current_edge_ID);
      }
      return current_edge_ID++;
    }

    // retrieves the information of the edge specified by the eid
    // returns the edge
    Edge* getEdge(const EdgeIDType eid)
    {
      return edge_set[edgeID2SetPos(eid)];
    }

    // checks whether an exists between src and tgt
    // if yes, return TRUE and record the corresponding edge ID in eid
    // if no, return FALSE
    bool probeEdge(const VertexIDType sid, const VertexIDType tid, EdgeIDType& eid)
    {
      HashedObj<VertexIDType, size_t> Sid, Tid;
      if (vertex_map.retrieve(sid, Sid) && vertex_map.retrieve(tid, Tid))
      {
        auto & bucket = adj_list[Sid.value];
        for (auto itr = bucket->begin(); itr != bucket->end(); itr++)
        {
          size_t index = edgeID2SetPos(*itr);
          Edge* pEdge = edge_set[index];
          if ((pEdge->src == sid) && (pEdge->tgt == tid))
          {
            eid = pEdge->id;
            return true;
          }
          if ((pEdge->src == tid) && (pEdge->tgt == sid))
          {
            eid = pEdge->id;
            return true;
          }
        }
        return false;
      }
      return false;
    }

    // returns the degree of the vertex specified by vid
    size_t degree(const VertexIDType vid)
    {
      return adj_list[vertexID2SetPos(vid)]->size();
    }

    // deletes the vertex specified by vid from the graph
    // also delete all edges associated with it
    // expected time complexity: O(d^2), where d is the average degree of the graph
    void deleteVertex(const VertexIDType vid)
    {
      size_t vertexPosition = vertexID2SetPos(vid);
      Vertex* dVertex = vertex_set[vertexPosition];
      auto & bucket = adj_list[vertexPosition];
      MyVector<EdgeIDType> dVer;
      for (auto itr = bucket->begin(); itr != bucket->end(); itr++)
      {
        dVer.push_back(*itr);
      }
      for (int i = 0; i < dVer.size(); ++i)
      {
        deleteEdge(dVer[i]);
      }
      bucket->clear();
      delete adj_list[vertexPosition];
      vertex_map.remove(vertex_set[vertexPosition]->id);
      delete vertex_set[vertexPosition];
      if (vertexPosition != (num_vertices - 1))
      {
        vertex_set[vertexPosition] = vertex_set[num_vertices - 1];
        HashedObj<VertexIDType, size_t> dVertex (vertex_set[num_vertices - 1]->id, vertexPosition);
        vertex_map.remove(vertex_set[vertexPosition]->id);
        vertex_map.insert(dVertex);
        adj_list[vertexPosition] = adj_list[num_vertices - 1];
      }
      vertex_set.pop_back();
      adj_list.pop_back();
      num_vertices--;
    }

    // deletes the edge specified by eid from the graph
    // expected time complexity: O(d), where d is the average degree of the graph
    void deleteEdge(const EdgeIDType eid)
    {
      size_t edgePosition = edgeID2SetPos(eid);
      Edge* dEdge = edge_set[edgePosition];
      size_t sourceIndex = vertexID2SetPos(dEdge->src);
      size_t targetIndex = vertexID2SetPos(dEdge->tgt);
      auto & bucketS = adj_list[sourceIndex];
      auto & bucketT = adj_list[targetIndex];
      EdgeIDType Id = edgeSetPos2ID(edgePosition);
      auto itrS = bucketS->find(bucketS->begin(), bucketS->end(), Id);
      if (itrS != bucketS->end())
      {
        bucketS->erase(itrS);
      }
      auto itrT = bucketT->find(bucketT->begin(), bucketT->end(), Id);
      if (itrT != bucketT->end())
      {
        bucketT->erase(itrT);
      }
       edge_map.remove(eid); 
       delete dEdge;
       if (edgePosition != (num_edges - 1))
       {
         edge_set[edgePosition] = edge_set[num_edges - 1];
         HashedObj <EdgeIDType, size_t> deleteMap(edge_set[num_edges - 1]->id, edgePosition);
         edge_map.remove(edge_set[num_edges - 1]->id);
         edge_map.insert(deleteMap);
       } 
       edge_set.pop_back();
       num_edges--;
    }

    // performs breath-first-search on the graph, starting from v_src
    // record the path in path
    // outgoing edge visit order should be determined based on their ID (smaller ID visited earlier)
    // the source vertex should also be included in the traversal path
    void breadthFirstSearch(const VertexIDType v_src, MyVector<VertexIDType>& path)
    {
      size_t vIndex = vertexID2SetPos(v_src);
      for (size_t i = 0; i < num_vertices; i++)
      {
        if(i == vIndex)
        {
          vertex_set[i]->tag = 1;
        }
        else
        {
          vertex_set[i]->tag = 0;
        }
      }
      MyQueue<Vertex*> dQueue; 
      dQueue.enqueue(vertex_set[vIndex]);
      while (!dQueue.empty())
      {
        Vertex* newVertex = dQueue.front(); 
        dQueue.dequeue();
        size_t index = vertexID2SetPos(newVertex->id);
        auto & bucket = adj_list[index];
        for (auto itr = bucket->begin(); itr != bucket->end(); itr++)
        {
          size_t eIndex = edgeID2SetPos(*itr);
          VertexIDType nVertex = edge_set[eIndex]->src;
          if (nVertex == newVertex->id)
          {
            nVertex = edge_set[eIndex]->tgt;
          }
          vIndex = vertexID2SetPos(nVertex);
          if (vertex_set[vIndex]->tag == 0)
          {
            vertex_set[vIndex]->tag = 1;
            dQueue.enqueue(vertex_set[vIndex]);
          }
        }
        path.push_back(newVertex->id);
      }
    }

    // performs depth-first-search on the graph, starting from v_src
    // record the path in path
    // outgoing edge visit order should be determined based on their ID (smaller ID visited earlier)
    // the source vertex should also be included in the traversal path
    void depthFirstSearch(const VertexIDType v_src, MyVector<VertexIDType>& path)
    {
      size_t vIndex = vertexID2SetPos(v_src);
      for (size_t i = 0; i < num_vertices; i++)
      {
        vertex_set[i]->tag = 0;
      }
      MyStack<Vertex*> dStack;
      dStack.push(vertex_set[vIndex]);
      while (!dStack.empty())
      {
        Vertex* newVertex = dStack.top();
        dStack.pop();
        vIndex = vertexID2SetPos(newVertex->id);
        if (vertex_set[vIndex]->tag == 1)
        {
          continue;
        }
        auto & bucket = adj_list[vIndex];
        for (auto itr = bucket->begin(); itr != bucket->end(); itr++)
        {
          size_t eIndex = edgeID2SetPos(*itr);
          VertexIDType nVertex = edge_set[eIndex]->src;
          if (nVertex == newVertex->id)
          {
            nVertex = edge_set[eIndex]->tgt;
          }   
          dStack.push(vertex_set[vertexID2SetPos(nVertex)]);
        }
        vertex_set[vIndex]->tag = 1;
        path.push_back(vertex_set[vIndex]->id);    
      }
    }

    // determines whether the two nodes vid1 and vid2 are connected
    // if yes, return TRUE
    // if no, return FASLE
    bool isConnected(const VertexIDType vid1, const VertexIDType vid2)
    {
      size_t vIndex = vertexID2SetPos(vid1);
      for (size_t i = 0; i < num_vertices; i++)
      {
        vertex_set[i]->tag = 0;
      }
      MyStack<Vertex*> dStack;
      dStack.push(vertex_set[vIndex]);
      while (!dStack.empty())
      {
        Vertex* newVertex = dStack.top();
        dStack.pop();
        vIndex = vertexID2SetPos(newVertex->id);
        if (vertex_set[vIndex]->tag == 1)
        {
          continue;
        }
        auto & bucket = adj_list[vIndex];
        for (auto itr = bucket->begin(); itr != bucket->end(); itr++)
        {
          size_t eIndex = edgeID2SetPos(*itr);
          VertexIDType nVertex = edge_set[eIndex]->src;
          if (nVertex == newVertex->id)
          {
            nVertex = edge_set[eIndex]->tgt;
          }
          if (nVertex == vid2)
          {
            return true;
          }
          dStack.push(vertex_set[vertexID2SetPos(nVertex)]);
        }
        vertex_set[vIndex]->tag = 1;
      }
      return false;
    }

    // returns the number of vertices in the graph
    size_t numVertices(void) const
    {
        return num_vertices;
    }

    // returns the number of edges in the graph
    size_t numEdges(void) const
    {
        return num_edges;
    }

    // checks whether the graph is empty (no vertex)
    bool empty(void) const
    {
        return (num_vertices == 0);
    }

    // prints the content in vertex_set
    void printInfoVertexSet(void) 
    {
        for(size_t i = 0; i < num_vertices; ++ i)
        {
            std::cout << "Array Position: " << i << "\t";
            std::cout << "Vertex ID: " << vertex_set[i]->id << "\t";
            std::cout << "Vertex data: " << vertex_set[i]->data << std::endl;
        }
        return;
    }

    // prints the content in edge_set
    void printInfoEdgeSet(void) 
    {
        for(size_t i = 0; i < num_edges; ++ i)
        {
            std::cout << "Array Position: " << i << "\t";
            std::cout << "Edge ID: " << edge_set[i]->id << "\t";
            std::cout << "Edge ends: " << edge_set[i]->src << "\t" << edge_set[i]->tgt << "\t";
            std::cout << "Edge data: " << edge_set[i]->data << std::endl;
        }
        return;
    }

    // prints the content in vertex_map
    void printInfoVertexMap(void) 
    {
        MyVector<VertexIDType> v_keys;
        vertex_map.keys(v_keys);
        for(size_t i = 0; i < v_keys.size(); ++ i)
        {
            std::cout << "VertexID-Position:\t" << v_keys[i] << "\t" << vertexID2SetPos(v_keys[i]) << std::endl;
        }
        return;
    }

    // prints the content in edge_map
    void printInfoEdgeMap(void) 
    {
        MyVector<EdgeIDType> e_keys;
        edge_map.keys(e_keys);
        for(size_t i = 0; i < e_keys.size(); ++ i)
        {
            std::cout << "EdgeID-Position:\t" << e_keys[i] << "\t" << edgeID2SetPos(e_keys[i]) << std::endl;
        }
        return;
    }

    // prints the content in the adjacency list
    void printAdjList(void) 
    {
        for(size_t i = 0; i < adj_list.size(); ++ i)
        {
            std::cout << "Vertex: " << vertex_set[i]->id << " degree: " << degree(vertex_set[i]->id) << "\tIt is connected with: ";
            for(auto itr = adj_list[i]->begin(); itr != adj_list[i]->end(); ++ itr)
            {
                if(vertex_set[i]->id == getEdge(*itr)->src)
                    std::cout << getEdge(*itr)->tgt << "\t";
                else if(vertex_set[i]->id == getEdge(*itr)->tgt)
                    std::cout << getEdge(*itr)->src << "\t";
                else
                    throw std::domain_error("MyGraph::printAdjList: edge information may have been corrupted.");
            }
            std::cout << std::endl;
        }
    }

  private:
    VertexIDType current_vertex_ID;                 // the ID to be assigned to the next vertex (ID are assigned based on the order of insertion)
    EdgeIDType current_edge_ID;                     // the ID to be assigned to the next edge (IDs are assigned based on the order of insertion)
    size_t num_vertices;                            // the number of vertices
    size_t num_edges;                               // the number of edges
    MyVector<Vertex*> vertex_set;                   // the set of vertices
    MyVector<Edge*> edge_set;                       // the set of edges
    MyHashTable<VertexIDType, size_t> vertex_map;   // the mapping between a vertex ID and its index in vertex_set
    MyHashTable<EdgeIDType, size_t> edge_map;       // the mapping between an edge ID and its index in vertex_set
    MyVector<MyLinkedList<EdgeIDType>* > adj_list;  // the adjacency list (stores the IDs for the corresponding edges)

    // maps the vertex ID to its position in the vertex_set array
    size_t vertexID2SetPos(const VertexIDType vid)
    {
      HashedObj<VertexIDType, size_t> verPosition;
      if(vertex_map.retrieve(vid, verPosition))
      {
        return verPosition.value;
      }
      return 0;
    }

    // maps the edge ID to its position in the edge_set array
    size_t edgeID2SetPos(const VertexIDType eid)
    {
      HashedObj<EdgeIDType, size_t> edPosition;
      if(edge_map.retrieve(eid, edPosition))
      {
        return edPosition.value;
      }
      return 0;
    }

    // gets the vertex ID for one in a given position in the vertex_set array
    VertexIDType vertexSetPos2ID(const size_t vpos)
    {
      return vertex_set[vpos]->id;
    }

    // gets the edge ID for one in a given position in the edge_set array
    EdgeIDType edgeSetPos2ID(const size_t epos)
    {
      return edge_set[epos]->id;
    }
  
};

#endif  // _MYGRAPH_H_
