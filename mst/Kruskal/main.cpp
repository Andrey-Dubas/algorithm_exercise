#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;

typedef unsigned int uint;

class Edge
{
    public:
    Edge(int from, int to, int weight)
        : m_from(from)
        , m_to(to)
        , m_weight(weight)
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

    int weight() const
    {
        return m_weight; 
    }

private:
    int m_from, m_to, m_weight;

};

class Graph
{
    public:
        Graph(size_t size)
            : m_Edges(size)
        {}

        void add(const Edge& edge)
        {
            m_Edges[edge.from()].push_back(edge);
            m_Edges[edge.to()].push_back(edge);
        }

        size_t size() const
        {
            return m_Edges.size();
        }

        const std::vector<Edge>& adjacents(int vertex) const
        {
            return m_Edges[vertex];
        }

        void deleteEdge(const Edge& e)
        {
            std::vector<Edge>& v = m_Edges[e.from()];
            for (std::vector<Edge>::iterator it = v.begin(); it != v.end(); ++it)
            {
               if (it->to() == e.to())
               {
                   v.erase(it);
                   break;
               } 
            }


            std::vector<Edge>& v1 = m_Edges[e.to()];
            for (std::vector<Edge>::iterator it = v1.begin(); it != v1.end(); ++it)
            {
               if (it->from() == e.from())
               {
                   v1.erase(it);
                   break;
               } 
            }
        }

    private:
        std::vector<std::vector<Edge>> m_Edges;
};



bool operator==(const Edge& left, const Edge& right)
{
    return left.weight() == right.weight();
}

bool operator<(const Edge& left, const Edge& right)
{
    return left.weight() < right.weight();
}


class UnionFind
{
public:
    UnionFind(uint size)
        : m_Connect(size)
        , m_Depth(size, 1)
    {
        for (uint i = 0; i < size; ++i)
        {
            m_Connect[i] = i;
        }
    }

    void connect(int v1, int v2)
    {
        int root1 = root(v1);
        int root2 = root(v2);

        if (m_Depth[root1] > m_Depth[root2])
        {
            m_Connect[root2] = root1;
        }
        else if (m_Depth[root1] > m_Depth[root2])
        {
            m_Connect[root1] = root2;
        }
        else
        {
            m_Connect[root1] = root2;
            ++m_Depth[root2];
        }
    }

    bool isConnected(int v1, int v2)
    {
        return root(v1) == root(v2);
    }

private:

    int root(int v)
    {

        int nextv;
        while (v != (nextv = m_Connect[v]))
        {
            m_Connect[v] = m_Connect[nextv];
            v = nextv;
        }

        return nextv;
    }

    std::vector<int> m_Connect;
    std::vector<int> m_Depth;

};


std::vector<Edge> Kruskal(std::vector<Edge>& graphEdges, uint graphSize)
{
    std::vector<Edge> result;
    UnionFind uf(graphSize);

    std::sort(graphEdges.begin(), graphEdges.end());

    for (const Edge& edge: graphEdges)
    {
        if (!uf.isConnected(edge.from(), edge.to()))
        {
            uf.connect(edge.from(), edge.to());
            result.push_back(edge);
        }
    }

    return result;
}



#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

std::ostream& operator<<(std::ostream& os, const Edge& v)
{
    os << v.from() << " -> " << v.to() << " weight: " << v.weight();
    return os;

}
std::ostream& operator<<(std::ostream& os, const std::vector<Edge>& v)
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
    std::vector<Edge> vector;

    vector.emplace_back(0, 7, 16);
    vector.emplace_back(2, 3, 17);
    vector.emplace_back(1, 7, 19);
    vector.emplace_back(3, 6, 52);
    vector.emplace_back(0, 2, 26);
    vector.emplace_back(5, 7, 28);
    vector.emplace_back(1, 3, 29);
    vector.emplace_back(1, 5, 32);
    vector.emplace_back(2, 7, 34);
    vector.emplace_back(4, 5, 35);
    vector.emplace_back(4, 7, 37);
    vector.emplace_back(1, 2, 36);
    vector.emplace_back(0, 4, 38);
    vector.emplace_back(6, 2, 40);
    vector.emplace_back(6, 4, 93);

    std::vector<Edge> result = Kruskal(vector, 8);

    std::vector<Edge> expect;
    // expect.emplace_back(0, 7, 16);
    // expect.emplace_back(2, 3, 17);
    // expect.emplace_back(1, 7, 19);
    // expect.emplace_back(5, 7, 28);
    // expect.emplace_back(1, 3, 29);
    // expect.emplace_back(2, 7, 34);
    // expect.emplace_back(4, 7, 37);
    // expect.emplace_back(3, 6, 52);
    //
    
    expect.emplace_back(0, 7, 16);
    expect.emplace_back(2, 3, 17);
    expect.emplace_back(1, 7, 19);
    expect.emplace_back(0, 2, 26);
    expect.emplace_back(5, 7, 28);
    expect.emplace_back(4, 5, 35);
    expect.emplace_back(6, 2, 40);

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
