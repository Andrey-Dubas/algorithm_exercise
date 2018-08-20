#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;

typedef unsigned int uint;

class Digraph
{
public:
    Digraph(size_t size)
        : m_Edges(size)
    {}

    const vector<int>& adjacents(int v) const
    {
        return m_Edges[v];
    }

    void add(int from, int to)
    {
        m_Edges[from].push_back(to);
    }

    size_t size() const
    {
        return m_Edges.size();
    }

private:
        std::vector<std::vector<int> > m_Edges;
};

Digraph getReversedGraph(const Digraph& graph)
{
    Digraph reversed(graph.size());

    for (uint i = 0; i < graph.size(); ++i)
    {
        for (int to: graph.adjacents(i))
        {
            reversed.add(to, i);
        }
    }

    return reversed;
}

void dfs_impl(const Digraph& graph, int vertex, std::vector<bool>& marked, std::vector<int>& result)
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

std::vector<int> getTopologicalOrder(const Digraph& graph, int vertex, std::vector<bool>& marked)
{
    std::vector<int> result;
    marked[vertex] = true;
    dfs_impl(graph, vertex, marked, result);
    return result;
}

std::vector<int> getStrongComponents(const Digraph& graph)
{
    Digraph reversed = getReversedGraph(graph);

    std::vector<bool> marked(reversed.size(), false);
    std::vector<int> order;

    for (uint i = 0; i < marked.size(); ++i)
    {
        if (!marked[i])
        {
            std::vector<int> passed = getTopologicalOrder(reversed, i, marked);

            order.insert(order.end(), passed.begin(), passed.end());
        }
    }

    std::reverse(order.begin(), order.end());

    marked = std::vector<bool>(marked.size(), false);

    std::vector<int> result(graph.size(), -1);
    for (int v: order)
    {
        if (!marked[v])
        {
            std::vector<int> temp = getTopologicalOrder(graph, v, marked);

            int minV = graph.size();

            for (int i: temp)
            {
                if (i < minV)
                {
                    minV = i;
                }
            }

            for (int i: temp)
            {
                result[i] = minV;
            }
        }
    }

    return result;




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

    Digraph graph(13);

    graph.add(0, 1);
    graph.add(0, 5);

    graph.add(2, 0);
    graph.add(2, 3);

    graph.add(3, 2);
    graph.add(3, 5);

    graph.add(4, 3);
    graph.add(4, 2);

    graph.add(5, 4);

    graph.add(6, 0);
    graph.add(6, 8);
    graph.add(6, 9);

    graph.add(7, 6);
    graph.add(7, 9);

    graph.add(8, 6);

    graph.add(9, 10);
    graph.add(9, 11);

    graph.add(10, 12);

    graph.add(11, 4);
    graph.add(11, 12);

    graph.add(12, 9);

    std::vector<int> expect = {0, 1, 0, 0, 0, 0, 6, 7, 6, 9, 9, 9, 9};

    std::vector<int> result = getStrongComponents(graph);

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
