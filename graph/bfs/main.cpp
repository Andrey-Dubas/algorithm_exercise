#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <queue>

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

class BFSResult
{
public:
    BFSResult(int startVertex, const std::vector<int>& relations)
        : m_StartVertex(startVertex)
        , m_Neighbour(relations)
    {

    }

    bool isPath(int v) const
    {
        return m_Neighbour[v] != -1;
    }

    std::vector<int> pathTo(int v) const
    {
        std::vector<int> result;

        while (v != -1)
        {
            result.push_back(v);
            v = m_Neighbour[v];
        }

        std::reverse(result.begin(), result.end());

        return result;
    }


    private:
        int m_StartVertex;
        std::vector<int> m_Neighbour;
};

BFSResult bfs(const Graph& graph, int startVertex)
{
    std::queue<int> queue;
    std::vector<bool> marked(graph.size(), false);
    marked[startVertex] = true;
    queue.push(startVertex);
    std::vector<int> path(graph.size(), -1);

    while (!queue.empty())
    {
        int vertex = queue.back();
        queue.pop();

        for (int next: graph.adjacents(vertex))
        {
            if (!marked[next])
            {
                marked[next] = true;
                queue.push(next);
                path[next] = vertex;
            }
        }
    }


    BFSResult result(startVertex, path);
    return result;

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

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v)
{
    os << '[';

    if (!v.empty())
    {
        os << v[0];

        for (uint i = 1; i < v.size(); ++i)
        {
            os << ", " << v[i];
        }
    }
    os << ']';
    return os;
}

void Test::test1()
{
    Graph graph(10);

    graph.add(0, 1);
    graph.add(1, 2);

    BFSResult result = bfs(graph, 0);

    std::vector<int> path = result.pathTo(2);
    std::vector<int> expected = {0, 1, 2};

    CPPUNIT_ASSERT_EQUAL(expected, path);
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
