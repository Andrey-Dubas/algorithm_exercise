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

    size_t size() const
    {
        return m_Edges.size();
    }

private:
        std::vector<std::vector<int> > m_Edges;
};

typedef unsigned int uint;

void dfs_impl(const Graph& graph, int vertex, std::vector<bool>& marked, std::vector<int>& result)
{
    for (int next: graph.adjacents(vertex))
    {
        if (!marked[next])
        {
            marked[next] = true;
            dfs_impl(graph, next, marked, result);
        }
    }
    result.push_back(vertex);

}

std::vector<int> dfs(const Graph& graph, int vertex)
{
    std::vector<bool> marked(graph.size(), false);
    marked[vertex] = true;
    std::vector<int> result;

    dfs_impl(graph, vertex, marked, result);

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
    graph.add(1, 2);

    graph.add(2, 3);
    graph.add(2, 4);

    std::vector<int> expect = {3, 4, 2, 1, 0};

    CPPUNIT_ASSERT_EQUAL(expect, dfs(graph, 0));


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
    
    std::vector<int> expect = {3, 2, 1, 0};

    std::vector<int> result = dfs(graph, 0);

    CPPUNIT_ASSERT_EQUAL(expect, result);
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
    

}

int main()
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    runner.run();
    return 0;
}
