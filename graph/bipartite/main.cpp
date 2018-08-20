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

enum State
{
    NONE,
    RED,
    BLACK
};

State nextState(State state)
{
    if (state == RED) { return BLACK;}
    return RED;
}

bool isBipartiteImpl(const Graph& graph, int vertex, std::vector<State>& states)
{
    State curState = states[vertex];

    for (int next: graph.adjacents(vertex))
    {
        if (states[next] == NONE)
        {
            states[next] = nextState(curState);
            if (isBipartiteImpl(graph, next, states) == false)
            {
                return false;
            }
        }
        else if (states[next] != nextState(states[vertex]))
        {
            return false;
        }
    }

    return true;
}

bool isBipartite(const Graph& graph)
{
    std::vector<State> states(graph.size(), NONE);
    states[0] = RED;
    return isBipartiteImpl(graph, 0, states);
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
    graph.add(1, 2);

    //   0
    //   | \
    //   |  \
    //   1---2
    

    CPPUNIT_ASSERT(!isBipartite(graph));


}

void Test::test2()
{

    Graph graph(10);

    graph.add(0, 1);
    graph.add(0, 2);
    graph.add(1, 3);
    graph.add(2, 3);

    //     0
    //    / \
    //   /    \
    //  1      2
    //   \    /
    //    \  /
    //     3
    

    CPPUNIT_ASSERT(isBipartite(graph));
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
