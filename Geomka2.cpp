#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <condition_variable>
#include <cmath>

#define Love 0;
using namespace std;
using namespace sf;

int width = 1800;
int height = 400;


vector<vector<double>> A = {{-1, 3, -3, 1},
                         {2, -5, 4, -1},
                         {-1, 0, 1, 0},
                         {0, 2, 0, 0}};

RenderWindow window(sf::VideoMode(width, height), "Orange", Style::Default);

double FindElement(vector<vector<double>>& m1, vector<vector<double>>& m2, int r, int c)
{
    double result = 0;
    for (int i = 0; i < m1[0].size(); i++)
    {
        result += m1[r][i] * m2[i][c];
    }
    return result;
}

vector<vector<double>> MultiplyMatrixes(vector<vector<double>>& m1, vector<vector<double>>& m2)
{
    vector<vector<double>> result(m1.size(), vector<double>(m2[0].size()));
    for (int i = 0; i < m1.size(); i++)
    {
        for (int j = 0; j < m2[0].size(); j++)      
        {
            result[i][j] = FindElement(m1, m2, i, j);
        }
    }
    return result;
}

vector<vector<double>> CreateVector(double t)
{
    vector<vector<double>> vec(1, vector<double>(4));
    double p = 1;
    for (int i = 3; i != -1; i--)
    {
        vec[0][i] = 0.5 * p;
        p *= t;
    }
    return vec;
}

vector<double> C(double t, vector<vector<double>>& coordinates)
{
    vector<vector<double>> vec = CreateVector(t);
    vector<vector<double>> result = MultiplyMatrixes(vec, A);
    result = MultiplyMatrixes(result, coordinates);
    return result[0];
}

void AddRemovePoint(vector<vector<double>>& points)
{
    vector<double> newPoint;
    newPoint.push_back(Mouse::getPosition(window).x);
    newPoint.push_back(Mouse::getPosition(window).y);

    bool toAdd = true;

    for (size_t i = 0; i < points.size(); i++)
    {
        if (abs(points[i][0]-newPoint[0])<4 && abs(points[i][1]-newPoint[1])<4)
        {
            points.erase(points.begin() + i);
            toAdd = false;
            i--;
        }
    }

    if (toAdd)
    {
        points.push_back(newPoint);
        sort(points.begin(), points.end());
    }
}

void FindSegment(vector<vector<double>>& coordinates, vector<vector<double>>& curve)
{
    for (double i = 0; i < 1; i+=0.005)
    {
        curve.push_back(C(i, coordinates));
    }
}

void FindAllCurve(vector<vector<double>>& points, vector<vector<double>>& curve)
{
    for (size_t i = 0; i < points.size()-3; i++)
    {
        vector<vector<double>> pt;
        for (size_t j = 0; j < 4; j++)
        {
            pt.push_back(points[i + j]);
        }
        FindSegment(pt, curve);
    }
}

void DrawPoints(vector<vector<double>>& points, RenderWindow& window)
{
    for (size_t i = 0; i < points.size(); i++)
    {
        CircleShape dot(4.f);
        dot.setFillColor(Color::Red);
        dot.setPosition(Vector2f(points[i][0], points[i][1]));
        window.draw(dot);
    }
}



int main()
{
    vector<CircleShape> kokoko;
    vector<vector<double>> points;
    vector<vector<double>> curve;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                AddRemovePoint(points);
                curve.clear();
                if (points.size() >= 4)
                {
                    FindAllCurve(points, curve);
                }
            }
        }
        window.clear(Color::Yellow);
        DrawPoints(points, window);
        for (size_t i = 0; i < curve.size(); i++)
        {
            CircleShape dot(1.f);
            dot.setFillColor(Color::Black);
            dot.setPosition(Vector2f(curve[i][0], curve[i][1]));
            window.draw(dot);
        }
        window.display();
    }
    vector<vector<double>> coordinates = { {0, 0}, {1, 1}, {2, -1}, {3, 0} };
    vector<double> res = C((1.0 / 3), coordinates);
    sort(A.begin(), A.end());
    for (auto a: points)
    {
        cout << a[0] << " " << a[1] << endl;
    }
    return Love;
}
