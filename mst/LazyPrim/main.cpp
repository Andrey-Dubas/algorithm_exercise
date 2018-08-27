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


class Heap
{

private:
    std::vector<Edge> m_Edges;

private:

    int leftIndex(int position)
    {
        return position * 2 + 1;
    }

    int rightIndex(int position)
    {
        return position * 2 + 2;
    }

    int parentIndex(int position)
    {
        return (position-1)/2;
    }

//-----------------------------------------------
    Edge& left(int position)
    {
        return m_Edges[leftIndex(position)];
    }

    Edge& right(int position)
    {
        return m_Edges[rightIndex(position)];
    }

    Edge& parent(int position)
    {
        return m_Edges[parentIndex(position)];
    }

    bool m_Comparator(const Edge& e1, const Edge& e2)
    {
        return e1 < e2;
    }
//---------------------------------------------

    int size() const
    {
        return m_Edges.size();
    }

    int sinkOnce(int position)
    {
        int currentPosition = position;
        if (leftIndex(position) < size())
        {
            if (m_Comparator(left(position), m_Edges[position]))
            {
                std::swap(left(position), m_Edges[position]);
                currentPosition = leftIndex(position);
            }
        }

        if (rightIndex(position) < size())
        {
            if (m_Comparator(right(position), m_Edges[position]))
            {
                std::swap(right(position), m_Edges[position]);

                if (position == currentPosition) // an element wasn't swapped
                {
                    currentPosition = rightIndex(position);
                }
            }
        }

        return currentPosition;
    }

    int promoteOnce(int position)
    {
        if (m_Comparator(m_Edges[position], parent(position)))
        {
            std::swap(m_Edges[position], parent(position));
            return parentIndex(position);
        }
        return position;
    }


    int sink(int position)
    {
        int nextPosition;
        while (position != (nextPosition = sinkOnce(position)))
        {
            position = nextPosition;
        }

        return position;
    }

    int promote(int position)
    {
        int nextPosition;
        while (position != (nextPosition = promoteOnce(position)))
        {
            position = nextPosition;
        }

        return position;
    }


//
public:
    Heap()
        : m_Edges()
    {}

    bool empty() const
    {
        return size() == 0;
    }

    void add(const Edge& edge)
    {
        int pos = m_Edges.size();

        m_Edges.push_back(edge);

        promote(pos);
    }

    Edge pop()
    {
        Edge e = m_Edges[0];

        m_Edges[0] = m_Edges.back();
        m_Edges.pop_back();

        sink(0);

        return e;
    }



};

std::ostream& operator<<(std::ostream& os, const Edge& v)
{
    os << v.from() << " -> " << v.to() << " weight: " << v.weight();
    return os;
}

std::vector<Edge> LazyPrim(const Graph& graph)
{
    std::vector<Edge> result;

    Heap heap;

    int vertex = 0;
    std::vector<bool> attached(graph.size(), false);
    attached[vertex] = true;
    int counter = graph.size()-1;

    do
    {
        const std::vector<Edge>& edges = graph.adjacents(vertex);

        for (const Edge& e: edges)
        {
            if (attached[e.other(vertex)] == false)
            {
                heap.add(e);
            }

        }

        Edge e = heap.pop();
        if (attached[e.to()] && attached[e.from()]) continue;
        --counter;
        result.push_back(e);
        vertex = attached[e.to()] ? e.from(): e.to() ;
        attached[vertex] = true;

    } while (counter);

    return result;
}



#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

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

    vector.emplace_back(0, 2, 26);
    vector.emplace_back(0, 4, 38);
    vector.emplace_back(0, 7, 16);

    vector.emplace_back(1, 7, 19);
    vector.emplace_back(1, 3, 29);
    vector.emplace_back(1, 5, 32);
    vector.emplace_back(1, 2, 36);

    vector.emplace_back(2, 3, 17);
    vector.emplace_back(2, 7, 34);

    vector.emplace_back(3, 6, 52);

    vector.emplace_back(4, 5, 35);
    vector.emplace_back(4, 7, 37);

    vector.emplace_back(5, 7, 28);

    vector.emplace_back(6, 2, 40);
    vector.emplace_back(6, 4, 93);

    Graph graph(8);
    for (const Edge& e: vector)
    {
        graph.add(e);
    }

    std::vector<Edge> result = LazyPrim(graph);
    std::sort(result.begin(), result.end());

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
    expect.emplace_back(1, 7, 19);
    expect.emplace_back(0, 2, 26);
    expect.emplace_back(2, 3, 17);
    expect.emplace_back(5, 7, 28);
    expect.emplace_back(4, 5, 35);
    expect.emplace_back(6, 2, 40);

    std::sort(expect.begin(), expect.end());

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
