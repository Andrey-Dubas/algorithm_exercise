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
    Edge(int from, int to, float weight)
        : m_from(from), m_to(to), m_weight(weight)
    { }

    Edge(char from, char to, float weight)
        : m_from(from - 'A'), m_to(to - 'A'), m_weight(weight)
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

    size_t size() const { return m_edges.size(); }

    void add(const Edge& e) { m_edges[e.from()].push_back(e); }
    void add(int from, int to, float weight)
    { 
        Edge e(from, to, weight);
        add(e);
    }

    void add(char from, char to, float weight)
    { 
        Edge e(from, to, weight);
        add(e);
    }

    const std::vector<Edge>& connected(int vertex) const 
    {
        return m_edges[vertex];
    }
};

struct Result
{
    int from;
    int path;
};

void relax(const Graph& graph, int vertex, std::vector<Result>& results, std::vector<bool>& relaxed)
{
    for (const Edge& edge: graph.connected(vertex))
    {
        int next = edge.to();
        if (results[vertex].path + edge.weight() < results[next].path)
        {
            results[next].path = results[vertex].path + edge.weight();
            results[next].from = vertex;
            relaxed[next] = true;
            
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

std::vector<Result> BellmanFord(const Graph& graph, int source)
{
    std::vector<Result> result(graph.size(), {-1, 999999});
    result[source] = {source, 0};

    std::vector<int> verticeToUpdate;

    for (unsigned int i = 0; i < graph.size(); ++i)
    {
        verticeToUpdate.push_back(i);
    }

    for (unsigned int i = 0; i < graph.size(); ++i)
    {
        std::vector<bool> nextRoundUpdate(graph.size(), false);
        for (unsigned int j = 0; j < graph.size(); ++j)
        {
            relax(graph, j, result, nextRoundUpdate);
        }

        verticeToUpdate.clear();
        for (unsigned int j = 0; j < nextRoundUpdate.size(); ++j)
        {
            if (nextRoundUpdate[j] == true)
            {
                verticeToUpdate.push_back(j);
            }
        }
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
    char letter[] = "A";
    letter[0] += edge.from;
    os << "{from: " << letter << ", path: " << edge.path << '}';
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

    std::vector<Result> result = BellmanFord(graph, 0);

    CPPUNIT_ASSERT_EQUAL(expect, result);

}

void Test::test2()
{
    Graph graph(5);

    // A B C D E
    // 0 1 2 3 4
    
    graph.add('A', 'B', -1);
    graph.add('A', 'C', 4);

    graph.add('B', 'C', 3);
    graph.add('B', 'D', 2);
    graph.add('B', 'E', 2);


    graph.add('D', 'B', 1);
    graph.add('D', 'C', 5);

    graph.add('E', 'D', -3);


    std::vector<Result> expect = {{0, 0}, {0, -1}, {1, 2}, {4, -2}, {1, 1}};

    std::vector<Result> result = BellmanFord(graph, 0);

    CPPUNIT_ASSERT_EQUAL(expect, result);

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
