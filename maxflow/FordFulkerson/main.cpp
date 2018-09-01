#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <limits>
#include <iostream>
#include <assert.h>

typedef unsigned int uint;

class Edge
{
    public:
    Edge(int from, int to, int capacity)
        : m_from(from)
        , m_to(to)
        , m_capacity(capacity)
        , m_flow(0)
    {
    }

    int from() const
    {
        return m_from; 
    }

    int to() const
    {
        return m_to; 
    }

    int other(int vertex) const
    {
        return vertex == m_to ? m_from : m_to;
    }

    int capacity() const
    {
        return m_capacity; 
    }

    int flow() const
    {
        return m_flow;
    }

    int remainingFlow(int from) const
    {
        assert(from == m_from || from == m_to);

        if (from == m_from)
        {
            return m_capacity - m_flow;
        }
        else
        {
            return m_flow;
        }
    }

    void pushFlow(int flow, int from)
    {
        assert(remainingFlow(from) >= flow);
        
        if (from == m_from)
        {
            m_flow += flow;
        }
        else
        {
            m_flow -= flow;
        }

    }

private:
    int m_from, m_to;
    int m_capacity;
    int m_flow;
};

class Graph
{
    public:
        Graph(size_t size)
            : m_FromEdges(size)
            , m_Edges(size)
        {}

        void add(const Edge& edge)
        {
            std::list<Edge>& list = m_FromEdges[edge.from()];
            list.push_back(edge);

            Edge& pushed = list.back();
            m_Edges[edge.from()].push_back(&pushed);
            m_Edges[edge.to()].push_back(&pushed);
        }

        size_t size() const
        {
            return m_Edges.size();
        }

        std::vector<Edge*> adjacents(int vertex)
        {
            return m_Edges[vertex];
        }

        const std::vector<Edge*> adjacents(int vertex) const
        {
            return m_Edges[vertex];
        }

    private:
        std::vector<std::list<Edge>>    m_FromEdges;
        std::vector<std::vector<Edge*>> m_Edges;
};



bool operator==(const Edge& left, const Edge& right)
{
    return left.remainingFlow(left.to()) == right.remainingFlow(left.to());
}

// bool operator<(const Edge& left, const Edge& right)
// {
//     return left.remainingFlow() < right.remainingFlow();
// }


int getPathImpl(Graph& graph, int vertex, const int target, std::vector<bool>& marked, std::vector<Edge*>& result)
{
    marked[vertex] = true;

    if (vertex == target)
    {
        return 9999;
    }

    for (Edge* edge: graph.adjacents(vertex))
    {
        int next = edge->other(vertex);
        int remain = edge->remainingFlow(vertex);

        if (marked[next] == false && remain)
        {
            int flow = getPathImpl(graph, next, target, marked, result);
            if (flow > 0)
            {
                result.push_back(edge);
                return std::min(remain, flow);
            }
        }
    }

    return 0;
}

std::pair<std::vector<Edge*>, bool> getAugmentedPath(Graph& graph, int source, int target)
{
    std::vector<bool> marked(graph.size(), false);
    std::vector<Edge*> result;

    int flow = getPathImpl(graph, source, target, marked, result);

    return std::make_pair(result, flow);
}

void dfs_SourceGroupImpl(const Graph& graph, int vertex, std::vector<bool>& marked)
{
    marked[vertex] = true;

    for (const Edge* edge: graph.adjacents(vertex))
    {
        int next = edge->other(vertex);
        if (edge->remainingFlow(vertex) && !marked[next])
        {
            dfs_SourceGroupImpl(graph, next, marked);
        }
    }

}

std::vector<int> dfs_SourceGroup(const Graph& graph, int source)
{
    std::vector<bool> marked(graph.size(), false);

    dfs_SourceGroupImpl(graph, source, marked);

    std::vector<int> result;
    for (unsigned int i = 0; i < graph.size(); ++i)
    {
        if (marked[i]) { result.push_back(i); }
    }

    return result;
}

std::vector<int> FordFulkerson(Graph& graph, int source, int target)
{
    do
    {
        std::pair<std::vector<Edge*>, bool> res = getAugmentedPath(graph, source, target);
        int vertex = source;
        std::reverse(res.first.begin(), res.first.end());
        if (res.second)
        {
            for (Edge* e: res.first)
            {
                e->pushFlow(res.second, vertex);
                vertex = e->other(vertex);

            }
        }
        else
        {
            break;
        }
    } while (true);

    std::vector<int> result = dfs_SourceGroup(graph, 0);
    return result;
}



#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

std::ostream& operator<<(std::ostream& os, const Edge& v)
{
    os << v.from() << " -> " << v.to() << ", capacity: " << v.capacity() << ", flow: " << v.flow();
    return os;

}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
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

// std::ostream& operator<<(std::ostream& os, const std::vector<Edge>& v)
// {
//     os << '[';
// 
//     if (!v.empty())
//     {
//         os << v[0];
// 
//         for (auto it = v.begin()+1; it != v.end(); ++it)
//         {
//             os << ", " << *it;
//         }
//     }
//     os << ']';
//     return os;
// }


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

    graph.add(Edge(0, 1, 10));
    graph.add(Edge(0, 2, 5));
    graph.add(Edge(0, 3, 15));

    graph.add(Edge(1, 4, 9));
    graph.add(Edge(1, 5, 15));

    graph.add(Edge(2, 5, 8));
    graph.add(Edge(2, 3, 4));

    graph.add(Edge(3, 6, 16));

    graph.add(Edge(4, 5, 15));
    graph.add(Edge(4, 7, 10));

    graph.add(Edge(5, 6, 15));
    graph.add(Edge(5, 7, 10));

    graph.add(Edge(6, 2, 6));
    graph.add(Edge(6, 7, 10));

    std::vector<int> vertice = FordFulkerson(graph, 0, 7);

    std::sort(vertice.begin(), vertice.end());

    std::vector<int> expect = {0, 2, 3, 6};

    CPPUNIT_ASSERT_EQUAL(expect, vertice);

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
