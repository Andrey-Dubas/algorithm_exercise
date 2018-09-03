#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

typedef unsigned int uint;


typedef std::vector<std::vector<float> > Matrix;
typedef std::vector<float> Vector;


struct Cord
{
    int row, column;
    Cord(int r, int c)
        : row(r)
        , column(c)
    { }
};


class Simplex
{
public:
    Simplex(const Matrix& matrix, const Vector& rightSide, const Vector& mainEquation)
        : m_Matrix(matrix)
        , m_RightHand(rightSide)
        , m_MainEquation(mainEquation)
        , result(0)
    {
        uint basisVariables = m_Matrix.size();

        for (uint i = 0; i < basisVariables; ++i)
        {
            m_MainEquation.push_back(0);

            for (uint j = 0; j < basisVariables; ++j)
            {
                if (i == j)
                {
                    m_Matrix[i].push_back(1);
                }
                else
                {
                    m_Matrix[i].push_back(0);
                }
            }
        }
    }

    Cord findPivot() const
    {
        int column = -1;
        float maxTarget = 0;
        for (uint i = 0; i < m_MainEquation.size(); ++i)
        {
            if (m_MainEquation[i] > maxTarget)
            {
                maxTarget = m_MainEquation[i];
                column = i;
            }
        }

        if (column == -1)
        {
            return Cord(-1, -1);
        }


        float minRatio = 99999;
        int row = -1;
        for (uint i = 0; i < m_RightHand.size(); ++i)
        {
            if (m_RightHand[i] >= 0) // preserves feasibility
            {
                float curRatio = m_RightHand[i]/m_Matrix[i][column];

                if (curRatio < minRatio)
                {
                    minRatio = curRatio;
                    row = i;
                }
            }
        }

        return Cord(row, column);
    }

    void elimination(const Cord& cord)
    {
        uint row = cord.row;
        uint column = cord.column;
        float mul = m_Matrix[row][column];

        for (float& el: m_Matrix[row])
        {
            el /= mul;
        }
        m_RightHand[row] /= mul;

        const Vector& eliminationRow = m_Matrix[row];


        for (unsigned int i = 0; i < m_Matrix.size(); ++i)
        {
            if (i != row)
            {
                float mul = m_Matrix[i][cord.column];

                for (uint j = 0; j < m_Matrix[i].size(); ++j)
                {
                    m_Matrix[i][j] -= eliminationRow[j]*mul;
                }

                m_RightHand[i] -= m_RightHand[row] * mul;
            }

        }


        mul = m_MainEquation[cord.column];
        for (uint i = 0; i < m_MainEquation.size(); ++i)
        {
            m_MainEquation[i] -= eliminationRow[i]*mul;
        }

        result += m_RightHand[row]*mul;
    }

    float run()
    {
        while (true)
        {
            Cord cord = findPivot();
            if (cord.row == -1) break;

            elimination(cord);
        }

        return result;

    }

    const Matrix& getMatrix() const
    {
        return m_Matrix;
    }

    Matrix& getMatrix()
    {
        return m_Matrix;
    }

    Vector& getRight()
    {
        return m_RightHand;
    }

    private:
        Matrix m_Matrix;
        Vector m_RightHand;
        Vector m_MainEquation;
        float result;
};


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
    Matrix matrix = 
    {
        {5 , 15},
        {4 , 4},
        {35, 20}
    };

    Vector mainEq = {13, 23};
    Vector rightPart = {480, 160, 1190};

    Simplex s(matrix, rightPart, mainEq);

    Cord cord = s.findPivot();

    CPPUNIT_ASSERT_EQUAL(1, cord.column);
    CPPUNIT_ASSERT_EQUAL(0, cord.row);

}

void Test::test2()
{

    Matrix matrix = 
    {
        {5 , 15},
        {4 , 4},
        {35, 20}
    };

    Vector mainEq = {13, 23};
    Vector rightPart = {480, 160, 1190};

    Simplex s(matrix, rightPart, mainEq);

    Cord cord(0, 1);
    cord.row = 0;
    cord.column = 1;

    s.elimination(cord);
    
    Matrix& m = s.getMatrix();

    CPPUNIT_ASSERT_EQUAL(1.0f, m[cord.row][cord.column]);
    m[cord.row][cord.column] = 0;

    for (uint i = 0; i < m.size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0f, m[i][cord.column], 0.01);
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.33, m[cord.row][0], 0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.06, m[cord.row][2], 0.01);


    Vector& v = s.getRight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(32, v[cord.row], 1);
}

void Test::test3()
{
    Matrix matrix = 
    {
        {5 , 15},
        {4 , 4},
        {35, 20}
    };

    Vector mainEq = {13, 23};
    Vector rightPart = {480, 160, 1190};

    Simplex s(matrix, rightPart, mainEq);

    float result = s.run();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(800, result, 10);
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
