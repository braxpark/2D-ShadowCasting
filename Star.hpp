#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

const float radianFactor = (1.f / 360.f) * (2.f * M_PI);
struct vec2
{

    float x;
    float y;

    bool operator==(const vec2 &a) const
    {
        return (x == a.x and y == a.y);
    }
};

//-----------------------------------------------------------------------------
// Basic vector arithmetic operations

vec2 subtract(vec2 a, vec2 b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}
vec2 negate(vec2 v)
{
    v.x = -v.x;
    v.y = -v.y;
    return v;
}
vec2 perpendicular(vec2 v)
{
    vec2 p = {v.y, -v.x};
    return p;
}
float dotProduct(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
float lengthSquared(vec2 v) { return v.x * v.x + v.y * v.y; }

//-----------------------------------------------------------------------------
// Triple product expansion is used to calculate perpendicular normal vectors
// which kinda 'prefer' pointing towards the Origin in Minkowski space

vec2 tripleProduct(vec2 a, vec2 b, vec2 c)
{

    vec2 r;

    float ac = a.x * c.x + a.y * c.y; // perform a.dot(c)
    float bc = b.x * c.x + b.y * c.y; // perform b.dot(c)

    // perform b * a.dot(c) - a * b.dot(c)
    r.x = b.x * ac - a.x * bc;
    r.y = b.y * ac - a.y * bc;
    return r;
}

//-----------------------------------------------------------------------------
// This is to compute average center (roughly). It might be different from
// Center of Gravity, especially for bodies with nonuniform density,
// but this is ok as initial direction of simplex search in GJK.

vec2 averagePoint(const std::vector<vec2> vertices, size_t count)
{
    vec2 avg = {0.f, 0.f};
    for (size_t i = 0; i < count; i++)
    {
        avg.x += vertices[i].x;
        avg.y += vertices[i].y;
    }
    avg.x /= count;
    avg.y /= count;
    return avg;
}

//-----------------------------------------------------------------------------
// Get furthest vertex along a certain direction

size_t indexOfFurthestPoint(const std::vector<vec2> vertices, size_t count, vec2 d)
{

    float maxProduct = dotProduct(d, vertices[0]);
    size_t index = 0;
    for (size_t i = 1; i < count; i++)
    {
        float product = dotProduct(d, vertices[i]);
        if (product > maxProduct)
        {
            maxProduct = product;
            index = i;
        }
    }
    return index;
}

//-----------------------------------------------------------------------------
// Minkowski sum support function for GJK

vec2 support(const std::vector<vec2> vertices1, size_t count1,
             const std::vector<vec2> vertices2, size_t count2, vec2 d)
{

    // get furthest point of first body along an arbitrary direction
    size_t i = indexOfFurthestPoint(vertices1, count1, d);

    // get furthest point of second body along the opposite direction
    size_t j = indexOfFurthestPoint(vertices2, count2, negate(d));

    // subtract (Minkowski sum) the two points to see if bodies 'overlap'
    return subtract(vertices1[i], vertices2[j]);
}

//-----------------------------------------------------------------------------
// The GJK yes/no test

int iter_count = 0;

int gjk(const std::vector<vec2> vertices1, size_t count1,
        const std::vector<vec2> vertices2, size_t count2)
{

    size_t index = 0; // index of current vertex of simplex
    vec2 a, b, c, d, ao, ab, ac, abperp, acperp, simplex[3];

    vec2 position1 = averagePoint(vertices1, count1); // not a CoG but
    vec2 position2 = averagePoint(vertices2, count2); // it's ok for GJK )

    // initial direction from the center of 1st body to the center of 2nd body
    d = subtract(position1, position2);

    // if initial direction is zero – set it to any arbitrary axis (we choose X)
    if ((d.x == 0) && (d.y == 0))
        d.x = 1.f;

    // set the first support as initial point of the new simplex
    a = simplex[0] = support(vertices1, count1, vertices2, count2, d);

    if (dotProduct(a, d) <= 0)
        return 0; // no collision

    d = negate(a); // The next search direction is always towards the origin, so the next search direction is negate(a)

    while (1)
    {
        iter_count++;

        a = simplex[++index] = support(vertices1, count1, vertices2, count2, d);

        if (dotProduct(a, d) <= 0)
            return 0; // no collision

        ao = negate(a); // from point A to Origin is just negative A

        // simplex has 2 points (a line segment, not a triangle yet)
        if (index < 2)
        {
            b = simplex[0];
            ab = subtract(b, a);           // from point A to B
            d = tripleProduct(ab, ao, ab); // normal to AB towards Origin
            if (lengthSquared(d) == 0)
                d = perpendicular(ab);
            continue; // skip to next iteration
        }

        b = simplex[1];
        c = simplex[0];
        ab = subtract(b, a); // from point A to B
        ac = subtract(c, a); // from point A to C

        acperp = tripleProduct(ab, ac, ac);

        if (dotProduct(acperp, ao) >= 0)
        {

            d = acperp; // new direction is normal to AC towards Origin
        }
        else
        {

            abperp = tripleProduct(ac, ab, ab);

            if (dotProduct(abperp, ao) < 0)
                return 1; // collision

            simplex[0] = simplex[1]; // swap first element (point C)

            d = abperp; // new direction is normal to AB towards Origin
        }

        simplex[1] = simplex[2]; // swap element in the middle (point B)
        --index;
    }

    return 0;
}

bool isCollidingWithWorld(sf::RenderWindow &window, std::vector<sf::Vector2f> points){
    //only need to check middle 2 points, the far edge of each beam
    sf::Vector2u windowSize = window.getSize();
    for(int i = 1; i < 3; i++){
        if(points[i].x > 0 and points[i].x < windowSize.x){
            if(points[i].y > 0 and points[i].y < windowSize.y){
                return false;
            }
        }
    }
    return true;
}
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

std::vector<sf::Vector2f> toVector2f(std::vector<vec2> v)
{
    size_t size = v.size();
    std::vector<sf::Vector2f> result;
    for (size_t i = 0; i < size; i++)
    {
        result.push_back(sf::Vector2f(v[i].x, v[i].y));
    }
    return std::move(result);
}

std::vector<vec2> toVec2(std::vector<sf::Vector2f> v)
{
    size_t size = v.size();
    std::vector<vec2> result;
    for (size_t i = 0; i < size; i++)
    {
        vec2 res = {v[i].x, v[i].y};
        result.push_back(std::move(res));
    }
    return std::move(result);
}

std::vector<vec2> getVec2FromConvexShape(sf::ConvexShape &shape)
{
    std::vector<sf::Vector2f> current;
    size_t size = shape.getPointCount();
    for (size_t i = 0; i < size; i++)
    {
        current.push_back(shape.getPoint(i));
    }
    std::vector<vec2> result = toVec2(std::move(current));
    return std::move(result);
}

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
void setPoints(std::vector<sf::Vector2f> &points, sf::ConvexShape &shape)
{
    size_t size = points.size();
    shape.setPointCount(size);
    for (size_t i = 0; i < size; i++)
    {
        shape.setPoint(i, points[i]);
    }
}

//  STAR CLASS
//
//
//
class Star
{
    public:
        Star();
        size_t numberOfBeams = 256;
        sf::Vector2f starPosition = {200.f, 200.f};
        bool starPositionHasChanged = false;
        std::vector<sf::RectangleShape> beams;
        void update(sf::Vector2f mousePos, std::vector<sf::ConvexShape> &staticShapes, sf::RenderWindow &window);
        std::vector<vec2> getVec2PointsFromRectangleShape(sf::RectangleShape &shape);
};

Star::Star() {
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

std::vector<vec2> Star::getVec2PointsFromRectangleShape(sf::RectangleShape &shape){
    sf::Vector2f pos = shape.getPosition(), size = shape.getSize();
    float x = size.x, y = size.y, theta = shape.getRotation() * radianFactor;
    std::vector<sf::Vector2f> points = { // A -> B -> C -> D
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
    std::vector<vec2> result = toVec2(points);
    return result;
}

void Star::update(sf::Vector2f mousePos, std::vector<sf::ConvexShape> &staticShapes, sf::RenderWindow &window){
    for(size_t i = 0; i < numberOfBeams; i++){
        beams[i].setSize(sf::Vector2f(1.f, 2.f));
        beams[i].setPosition(mousePos);
        bool works = true;
        while(works){
            sf::Vector2f size = beams[i].getSize();
            float angle = beams[i].getRotation();
            float x = size.x, y = size.y, theta = angle * radianFactor;

            float xComp = x * cos(theta) - y * sin(theta), yComp = x * sin(theta) + y * cos(theta);

            xComp += mousePos.x;
            yComp += mousePos.y;

            std::vector<vec2> points = getVec2PointsFromRectangleShape(beams[i]);
            size_t pointsCount = points.size();

            bool collidedWithShapes = false;
            for(auto &shape : staticShapes){
                if(!collidedWithShapes){
                    std::vector<vec2> staticPoints = getVec2FromConvexShape(shape);
                    size_t staticCount = staticPoints.size();
                    if(gjk(points,pointsCount, staticPoints, staticCount)){
                        collidedWithShapes = true;
                    }
                }
            }

            float increment = 8.f;
            if(xComp < 0 or xComp > window.getSize().x or yComp < 0 or yComp > window.getSize().y or collidedWithShapes) {
                works = false;
                size.x -= increment / 2.f;
                beams[i].setSize(size);
            }
            else{
                size.x += increment;
                beams[i].setSize(size);
            }
        }
    }
}