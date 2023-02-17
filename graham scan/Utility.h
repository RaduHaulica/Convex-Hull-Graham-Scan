#pragma once

const float pi = 3.141592f;
const float MAXY = 800.0f;

/**
* returns a vertex array based on provided point vector and color
*/
sf::VertexArray createVertexArray(std::vector<sf::Vector2f> v, sf::Color color)
{
    sf::VertexArray va;
    va.setPrimitiveType(sf::PrimitiveType::LinesStrip);
    for (int i = 0; i < v.size(); i++)
    {
        if (i == 0)
        {
            va.append(sf::Vertex(v[i], sf::Color::Red));
        }
        else
        {
            va.append(sf::Vertex(v[i], color));
        }
    }
    return va;
}

/**
* returns the angle between the offset vector and the point vector
*/
float getAngle(sf::Vector2f point, sf::Vector2f offset)
{
    return atan2(-offset.y + point.y, -offset.x + point.x);
}


/**
* checks whether a given point is to the left (rotationally) of the [origin1, origin2] segment
*/
bool isLeftOf(sf::Vector2f origin1, sf::Vector2f origin2, sf::Vector2f point)
{
    sf::Vector2f segment = { -(origin2.y - origin1.y), origin2.x - origin1.x }; // B-A then rotate left
    sf::Vector2f movedPoint = point - origin1; // C-A
    float dotProduct = segment.x * movedPoint.x + segment.y * movedPoint.y;
    if (dotProduct < 0)
    {
        //std::cout << "right" << std::endl;
        return false;
    }
    //std::cout << "left" << std::endl;
    return true;
}

sf::Vector2f getSecond(std::vector<sf::Vector2f> points)
{
    points.pop_back();
    return points.back();
}