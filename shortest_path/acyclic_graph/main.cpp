#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iostream>
#include <assert.h>

using namespace std;

typedef unsigned int uint;

class Edge
{
    int m_from, m_to;
    float m_weight;
public:
    explicit Edge(int from, int to, float weight)
        : m_from(from), m_to(to), m_weight(weight)
    { }

    int from() const { return m_from; }
    int to() const { return m_to; }
    float weight() const { return m_weight; }
};


class Graph
{
    std::vector< std::vector<Edge> > m_edges;
public:
    Graph(int size)
        : m_edges(size)
    { }

    int size() const { return m_edges.size(); }

    void add(const Edge& e) { m_edges[e.from()].push_back(e); }
    void add(int from, int to, float weight)
    { 
        Edge e(from, to, weight);
        add(e);
    }

    const std::vector<Edge>& connected(int vertex) const 
    {
        return m_edges[vertex];
    }
};

enum State
{
    UNVISITED = 0,
    DISCOVERED,
    WITHIN_PATH
};

bool isAcyclicImpl(const Graph& graph, int vertex, std::vector<State>& marked)
{
    marked[vertex] = WITHIN_PATH;

    for (const Edge& edge: graph.connected(vertex))
    {
        int next = edge.to();
        if (marked[next] == UNVISITED)
        {
            if (isAcyclicImpl(graph, next, marked) == false)
            {
                return false;
            }
        }
        if (marked[next] == WITHIN_PATH)
        {
            return false;
        }
    }

    marked[vertex] = DISCOVERED;

    return true;
}

bool isAcyclic(const  Graph& graph)
{
    int vertex = 0;
    for (; vertex < graph.size(); ++vertex)
    {
        if (!graph.connected(vertex).empty())
        { break;}
    }

    if (vertex != graph.size())
    {
        std::vector<State> marked(graph.size(), UNVISITED);
        return isAcyclicImpl(graph, vertex, marked);
    }

    return false;
}


void dfs_impl(const Graph& graph, int vertex, std::vector<bool>& marked, std::vector<int>& result)
{
    marked[vertex] = true;

    for (const Edge& edge: graph.connected(vertex))
    {
        int next = edge.to();

        if (marked[next] == false)
        {
            dfs_impl(graph, next, marked, result);
        }
    }
    result.push_back(vertex);
}


std::vector<int> getTopologicalOrder(const Graph& graph, int vertex)
{
    std::vector<int> result;
    std::vector<bool> marked(graph.size(), false);

    dfs_impl(graph, vertex, marked, result);
    return result;
}

struct Result
{
    int from;
    int path;
};

void relax(const Graph& graph, int vertex, std::vector<Result>& results)
{
    for (const Edge& edge: graph.connected(vertex))
    {
        int next = edge.to();
        if (results[vertex].path + edge.weight() < results[next].path)
        {
            results[next].path = results[vertex].path + edge.weight();
            results[next].from = vertex;
        }
    }
}


std::ostream& operator<<(std::ostream& os, const std::vector<int>& v)
{
    os << '[';

    if (!v.empty())
    {
        os << v[0];

        for (auto it = v.begin()+1; it != v.end(); ++it)
        {
            os << ", " << *it;
        }
    }
    os << ']';
    return os;
}

std::vector<Result> getAcyclicMst(const Graph& graph, int source)
{
    std::vector<Result> result(graph.size(), {-1, 999999});
    result[source] = {source, 0};

    std::vector<int> order = getTopologicalOrder(graph, source);
    std::reverse(order.begin(), order.end());

    for (int vertex: order)
    {
        relax(graph, vertex, result);
    }

    return result;
}


#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

bool operator==(const Result& res1, const Result& res2)
{
    return res1.from == res2.from && res1.path == res2.path;
}

std::ostream& operator<<(std::ostream& os, const Result& edge)
{
    os << "{from: " << edge.from << ", path: " << edge.path << '}';
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Result>& v)
{
    os << '[';

    if (!v.empty())
    {
        os << v[0];

        for (auto it = v.begin()+1; it != v.end(); ++it)
        {
            os << ", " << *it;
        }
    }
    os << ']';
    return os;
}


class Test: public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( Test ); 

    CPPUNIT_TEST( test1 );
    CPPUNIT_TEST( test2 );
    CPPUNIT_TEST( test3 );
    CPPUNIT_TEST( test4 );

    CPPUNIT_TEST_SUITE_END();

public:
    void test1();
    void test2();
    void test3();
    void test4();
};
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

void Test::test1()
{

    Graph graph(8);

    graph.add(0, 1, 5);
    graph.add(0, 4, 9);
    graph.add(0, 7, 8);

    graph.add(1, 2, 12);
    graph.add(1, 3, 15);
    graph.add(1, 7, 4);

    graph.add(2, 3, 3);
    graph.add(2, 6, 11);

    graph.add(3, 6, 9);

    graph.add(4, 5, 4);
    graph.add(4, 6, 20);
    graph.add(4, 7, 5);

    graph.add(5, 2, 1);
    graph.add(5, 6, 13);

    graph.add(7, 2, 7);
    graph.add(7, 5, 6);

    std::vector<Result> expect = {{0, 0}, {0, 5}, {5, 14}, {2, 17}, {0, 9}, {4, 13}, {2, 25}, {0, 8}};

    std::vector<Result> result = getAcyclicMst(graph, 0);

    CPPUNIT_ASSERT_EQUAL(expect, result);

}

void Test::test2()
{

}

void Test::test3()
{

}

void Test::test4()
{

}

int main()
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    runner.run();
    return 0;
}
