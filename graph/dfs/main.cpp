#include <vector>
#include <string>
#include <algorithm>
#include <limits>

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

    size_t size() const
    {
        return m_Edges.size();
    }

private:
        std::vector<std::vector<int> > m_Edges;
};

void dfs_impl(const Graph& graph, int vertex, int target, std::vector<bool>& marked)
{
    for (int next: graph.adjacents(vertex))
    {
        if (!marked[next])
        {
            marked[next] = true;
            dfs_impl(graph, next, target, marked);
        }
    }
}

std::vector<bool> dfs(const Graph& graph, int startVertex, int target)
{
    std::vector<bool> marked(graph.size(), false);

    marked[startVertex] = true;
    dfs_impl(graph, startVertex, target, marked);

    return marked;
}



#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

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
    graph.add(3, 2);
    graph.add(1, 2);
    graph.add(3, 5);
    graph.add(3, 4);
    
    graph.add(6, 7);
    graph.add(8, 7);

    std::vector<bool> marked = dfs(graph, 0, 0);

    CPPUNIT_ASSERT(marked[1]);
    CPPUNIT_ASSERT(marked[2]);
    CPPUNIT_ASSERT(marked[3]);
    CPPUNIT_ASSERT(marked[4]);
    CPPUNIT_ASSERT(marked[5]);

    CPPUNIT_ASSERT(!marked[6]);
    CPPUNIT_ASSERT(!marked[7]);
    CPPUNIT_ASSERT(!marked[8]);
    CPPUNIT_ASSERT(!marked[9]);

    
    marked = dfs(graph, 6, 0);

    CPPUNIT_ASSERT(marked[8]);
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
