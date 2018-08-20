#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;

typedef unsigned int uint;

class Graph
{
public:
    Graph(size_t size)
        : m_Edges(size)
    {}

    const vector<int>& adjacents(int v) const
    {
        return m_Edges[v];
    }

    void add(int v1, int v2)
    {
        m_Edges[v1].push_back(v2);
        m_Edges[v2].push_back(v1);
    }

    void deleteEdge(int v1, int v2)
    {
        for (uint i = 0; i < adjacents(v1).size(); ++i)
        {
            if (m_Edges[v1][i] == v2)
            {
                m_Edges[v1].erase(m_Edges[v1].begin() + i);
            }
        }

        for (uint i = 0; i < adjacents(v2).size(); ++i)
        {
            if (m_Edges[v2][i] == v1)
            {
                m_Edges[v2].erase(m_Edges[v2].begin() + i);
            }
        }
    }

    size_t size() const
    {
        return m_Edges.size();
    }

private:
        std::vector<std::vector<int> > m_Edges;
};

typedef unsigned int uint;
int hasEulerPath(const Graph& graph)
{
    std::vector<int> oddVertice;

    std::cout << std::endl;

    for (uint i = 0; i < graph.size(); ++i)
    {
        if (graph.adjacents(i).size() % 2 == 1)
        {
            oddVertice.push_back(i);
        }
    }

    switch (oddVertice.size())
    {
        case 0:
            return graph.size();
        case 2:
            return oddVertice[0];
        default:
            return -1;
    }
}

int dfs_count_impl(const Graph& graph, int vertex, std::vector<bool>& marked)
{
    int result = 1;
    for (int next: graph.adjacents(vertex))
    {
        if (!marked[next])
        {
            marked[next] = true;
            result += dfs_count_impl(graph, next, marked);
        }
    }
    return result;
}

int dfs_count(const Graph& graph, int vertex)
{
    std::vector<bool> marked(graph.size(), false);
    marked[vertex] = true;

    return dfs_count_impl(graph, vertex, marked);

}

std::vector<int> getEulerPath(Graph& graph)
{
    int vertex = hasEulerPath(graph);

    if (static_cast<size_t>(vertex) == graph.size())
    {
        vertex = 0;
    }
    std::vector<int> result = {vertex};

    while (!graph.adjacents(vertex).empty())
    {
        const std::vector<int>& adj = graph.adjacents(vertex);

        int next;
        for (uint i = 0; i < adj.size(); ++i)
        {
            next = adj[i];

            if (adj.size() == 1)
            {
                graph.deleteEdge(next, vertex);
                break;
            }

            int count1 = dfs_count(graph, vertex);

            graph.deleteEdge(next, vertex);
            int count2 = dfs_count(graph, vertex);

            if (count1 == count2)
            {
                break;
            }

            graph.add(vertex, next);
        }

        vertex = next;
        result.push_back(vertex);
    }

    return result;
}

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

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
    Graph graph(10);

    graph.add(0, 1);
    graph.add(0, 2);
    graph.add(1, 2);

    /*
      0
      | \
      |  \
      1---2
    */
    

    CPPUNIT_ASSERT_EQUAL(10, hasEulerPath(graph));


}

void Test::test2()
{

    Graph graph(10);

    graph.add(0, 1);
    graph.add(0, 2);

    graph.add(1, 2);

    graph.add(1, 3);
    graph.add(2, 3);

    /*
         0
        / \
       /   \
      1-----2
       \   /
        \ /
         3
    */
    
    int oddVertex = hasEulerPath(graph);
    CPPUNIT_ASSERT_EQUAL(1, oddVertex);
}

void Test::test3()
{
    Graph graph(10);

    graph.add(0, 1);
    graph.add(0, 2);

    graph.add(1, 2);

    graph.add(1, 3);
    graph.add(2, 3);

    graph.add(0, 3);

    /*
    //      0
    //    / | \
    //   /  |  \
    //  1-------2
    //   \  |  /
    //    \ | /
    //      3
    */
    
    int oddVertex = hasEulerPath(graph);
    CPPUNIT_ASSERT_EQUAL(-1, oddVertex);

}

void Test::test4()
{

    Graph graph(10);

    graph.add(0, 1);
    graph.add(0, 2);
    graph.add(1, 2);

    /*
      0
      | \
      |  \
      1---2
    */
    

    std::vector<int> expect = {0, 1, 2, 0};
    std::vector<int> result = getEulerPath(graph);
    std::sort(result.begin(), result.end()-1);
    CPPUNIT_ASSERT_EQUAL(expect, result);
}

int main()
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    runner.run();
    return 0;
}
