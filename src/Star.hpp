#ifndef STAR_H
#define STAR_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

float M_PI = atan(1) * 4.0f;
const float radianFactor = (1.f / 360.f) * (2.f * M_PI);


// ----------------------------------------------------
//gjk insipration from https://github.com/kroitor/gjk.c
// ----------------------------------------------------

enum class Collisions {GJK};

sf::Vector2f subtract(const sf::Vector2f a, const sf::Vector2f b)
{
    sf::Vector2f c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

// Basic vector arithmetic operations

sf::Vector2f negate(const sf::Vector2f a)
{
    sf::Vector2f c;
    c.x = -a.x;
    c.y = -a.y;
    return c;
}

sf::Vector2f perpendicular(sf::Vector2f a)
{
    sf::Vector2f c;
    c.x = a.y;
    c.y = -a.x;
    return c;
}

float dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
    float result = 0.0f;
    result += (a.x * b.x);
    result += (a.y * b.y);
    return result;
}

float lengthSquared(const sf::Vector2f& a)
{
    float result = 0.0f;
    result += (a.x * a.x);
    result += (a.y * a.y);
    return result;
}

sf::Vector2f tripleProduct(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c)
{
    sf::Vector2f result;
    float ac = dot(a, c);
    float bc = dot(b, c);

    result.x = (b.x * ac) - (a.x * bc);
    result.y = (b.y * ac) - (a.y * bc);
    return result;
}

sf::Vector2f averagePoint(const std::vector<sf::Vector2f>& vertices)
{
    int size = vertices.size();
    sf::Vector2f result(0.0f, 0.0f);
    for(size_t i = 0; i < size; i++)
    {
        result.x += (vertices[i].x);
        result.y += (vertices[i].y);
    }
    result.x /= (size);
    result.y /= (size);
    return result;
}

size_t indexOfFurthestPoint(const std::vector<sf::Vector2f>& vertices, const sf::Vector2f& direction)
{
    int size = vertices.size();
    float maxProduct = dot(direction, vertices[0]);
    size_t idx = 0;
    for(size_t i = 1; i < size; i++)
    {
        float currentProduct = dot(direction, vertices[i]);
        if(currentProduct > maxProduct)
        {
            maxProduct = currentProduct;
            idx = i;
        }
    }
    return idx;
}

sf::Vector2f support(const std::vector<sf::Vector2f>& vertices1,
                     const std::vector<sf::Vector2f>& vertices2,
                     const sf::Vector2f& direction)
{
    size_t i = indexOfFurthestPoint(vertices1, direction);
    size_t j = indexOfFurthestPoint(vertices2, negate(direction));

    sf::Vector2f result = subtract(vertices1[i], vertices2[j]);
    return result;
}

//gjk algorithm
bool gjk(const std::vector<sf::Vector2f>& vertices1, const std::vector<sf::Vector2f>& vertices2)
{
    size_t idx = 0;
    sf::Vector2f a, b, c, d, ao, ab, ac, abperp, acperp;
    std::vector<sf::Vector2f> simplex(3);
    
    sf::Vector2f pos1 = averagePoint(vertices1);
    sf::Vector2f pos2 = averagePoint(vertices2);

    d = subtract(pos1, pos2);

    if(d.x == 0 && d.y == 0)
    {
        d.x = 1.0f;
    }

    a = simplex[0] = support(vertices1, vertices2, d);
    if(dot(a, d) <= 0)
        return false;
    else
    {
        d = negate(a);
        while(true)
        {
            a = simplex[++idx] = support(vertices1, vertices2, d);

            if(dot(a, d) <= 0)
                return false;
            else
            {
                ao = negate(a);
                if(idx < 2)
                {
                    b = simplex[0]; 
                    ab = subtract(b, a);
                    d = tripleProduct(ab, ao, ab);
                    if(lengthSquared(d) == 0)
                        d = perpendicular(ab);
                    continue;
                }

                b = simplex[1];
                c = simplex[0];
                ab = subtract(b, a);
                ac = subtract(c, a);
                acperp = tripleProduct(ab, ac, ac);
                if(dot(acperp, ao) >= 0)
                {
                    d = acperp;
                }
                else
                {
                    abperp = tripleProduct(ac, ab, ab);
                    if(dot(abperp, ao) < 0)
                        return true;
                    else
                    {
                        simplex[0] = simplex[1];
                        d = abperp;
                    }
                    
                }
            }
            simplex[1] = simplex[2];
            --idx;
        }
    }  
    return false; 
}

// Determine if some set of points exceeds the World boundary
// The window is treated as the world boundary
bool isCollidingWithWorld(sf::RenderWindow &window, std::vector<sf::Vector2f> points){
    //only need to check middle 2 points, the far edge of each beam relative to the Star
    sf::Vector2u windowSize = window.getSize();
    for(int i = 1; i < 3; i++){
        if(points[i].x > 0 && points[i].x < windowSize.x){
            if(points[i].y > 0 && points[i].y < windowSize.y){
                return false;
            }
        }
    }
    return true;
}

// Get a vector of Vector2f points from a sf::ConvexShape
std::vector<sf::Vector2f> getPoints(sf::ConvexShape &shape)
{
    size_t size = shape.getPointCount();
    std::vector<sf::Vector2f> result;
    for (size_t i = 0; i < size; i++)
    {
        result.push_back(std::move(shape.getPoint(i)));
    }
    return std::move(result);
}


// Print to terminal, the point values of each point in a ConvexShape
void printPoints(sf::ConvexShape &shape)
{
    size_t size = shape.getPointCount();
    for (size_t i = 0; i < size; i++)
    {
        sf::Vector2f point = shape.getPoint(i);
        std::cout << point.x << " " << point.y << "\n";
    }
    std::cout << "-------------------------------------------\n";
}

// Set points in some ConvexShape, shape from a list of sf::Vector2f
void setPoints(std::vector<sf::Vector2f> &points, sf::ConvexShape &shape)
{
    size_t size = points.size();
    shape.setPointCount(size);
    for (size_t i = 0; i < size; i++)
    {
        shape.setPoint(i, points[i]);
    }
}

// STAR CLASS
class Star
{
    public:
        enum class Collision{None, GJK, TWOLINES};
        Collision current = Collision::GJK;
        Star();
        size_t numberOfBeams = 256;
        sf::Vector2f starPosition = {200.f, 200.f};
        bool starPositionHasChanged = false;
        std::vector<sf::RectangleShape> beams;
        void update(sf::Vector2f mousePos, std::vector<sf::ConvexShape> &staticShapes, sf::RenderWindow &window);
        std::vector<sf::Vector2f> getPointsFromRectangleShape(sf::RectangleShape &shape);
};

Star::Star() 
{
    sf::RectangleShape shape;
    float angleFactor = 360.f / (float)numberOfBeams;
    for(size_t i = 0; i < numberOfBeams; i++){
        float theta = angleFactor * (float)i;
        shape.setRotation(theta);
        shape.setSize(sf::Vector2f(100.f, 1.f));
        shape.setFillColor(sf::Color::Yellow);
        beams.push_back(shape);
    }
}

std::vector<sf::Vector2f> Star::getPointsFromRectangleShape(sf::RectangleShape &shape)
{
    sf::Vector2f pos = shape.getPosition(), size = shape.getSize();
    float x = size.x, y = size.y, theta = shape.getRotation() * radianFactor;


    // Points are defined in this order:
    // A -> B -> C -> D
    //
    // A -- B
    // |    |
    // D -- C

    std::vector<sf::Vector2f> points = {
        sf::Vector2f(0.f, 0.f),
        sf::Vector2f(x, 0.f),
        sf::Vector2f(x, y),
        sf::Vector2f(0.f, y),
    };
    for(auto &point : points) {
        float tempX = point.x, tempY = point.y;
        point.x = tempX * cos(theta) - tempY * sin(theta);
        point.y = tempX * sin(theta) + tempY * cos(theta);

        point.x += pos.x;
        point.y += pos.y;
    }
    return points;
}

void Star::update(sf::Vector2f mousePos, std::vector<sf::ConvexShape> &staticShapes, sf::RenderWindow &window)
{
    // For each beam:
    // Determine if the beam collides with either the world or a shape in the world.
    // If so, stop marching, else increase length by some increment and continue;
    for(size_t i = 0; i < numberOfBeams; i++)
    {
        beams[i].setSize(sf::Vector2f(1.f, 3.f));
        beams[i].setPosition(mousePos);
        bool works = true;
        sf::Vector2f size = beams[i].getSize();
        float angle = beams[i].getRotation();
        float theta = angle * radianFactor;
        bool first = true;
        int count = 0;
        while(works){
            if(count > 0)
                first = false;
            count++;
            float x = size.x, y = size.y;
            float xComp = x * cos(theta) - y * sin(theta);
            float yComp = x * sin(theta) + y * cos(theta);

            xComp += mousePos.x;
            yComp += mousePos.y;

            std::vector<sf::Vector2f> points = getPointsFromRectangleShape(beams[i]);
        
            bool collidedWithShapes = false;
            
            for(auto &shape : staticShapes)
            {
                if(!collidedWithShapes)
                {
                    switch(current) 
                    {
                        case Collision::GJK: 
                        {
                            std::vector<sf::Vector2f> staticPoints = getPoints(shape);
                            int idx = 0;
                            if(gjk(points, staticPoints))
                            {
                                collidedWithShapes = true;

                                // if this is the first iteration and there is a collision -> mouse is inside of a shape to begin, no need to continue;
                                if(first)
                                {
                                    works = false;
                                    break;
                                }
                                // decrease the ray until it is no longer colliding with either world boundaries or any valid shape in the scene
                                while(gjk(points, staticPoints)) 
                                {
                                    size.x-=2.f;
                                    beams[i].setSize(size);
                                    points = getPointsFromRectangleShape(beams[i]);
                                }
                                idx++;
                            }
                            break;
                        }
                    }
                }
            }
            float increment = 80.f;
            if(xComp < 0 || xComp > window.getSize().x || yComp < 0 || yComp > window.getSize().y || collidedWithShapes)
            {
                works = false;
                beams[i].setSize(size);
            }
            else{
                size.x += increment;
                beams[i].setSize(size);
            }
        }
    }
}

#endif // ! STAR_H
