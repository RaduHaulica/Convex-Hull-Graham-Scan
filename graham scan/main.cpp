#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>

const float pi = 3.141592f;
const float MAXY = 800.0f;

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

float getAngle(sf::Vector2f point, sf::Vector2f offset)
{
    return atan2(-offset.y + point.y, -offset.x + point.x);
}

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

int main()
{
    float pixelWidth = 800.0f;
    sf::RenderWindow window(sf::VideoMode(pixelWidth, pixelWidth), "SFML works!");
    sf::View camera;
    camera.setCenter(400, 400);
    camera.setSize(800, -800); // -800 because y axis is inverted
    window.setView(camera);

    sf::Font font;
    font.loadFromFile("./Roboto-bold.ttf");

    sf::Text helpText;
    helpText.setFont(font);
    helpText.setFillColor(sf::Color::White);
    helpText.setCharacterSize(12);
    helpText.setPosition({ 0,50 });
    helpText.setString("Click or hold left mouse button to create points.\nPress right mouse button to start the algorithm visualization");
    helpText.setScale({ 1,-1 }); // to account for y axis inverted

    sf::Vector2i mousePos;
    sf::Vector2f mousePosWorld;

    // time
    sf::Clock frameClock;
    float dt;
    float stepTimer{ 0.02f };
    float stepTimerAccumulator{ 0.0f };
    float inputCooldown{ 0.025f };
    float inputCooldownAccumulator{ 0.0f };
    int pass{ 0 };

    std::vector<sf::Vector2f> points;
    std::vector<sf::Vector2f> pointsStack;
    std::vector<sf::Vector2f> solutionStack;
    bool addPoint = false;
    bool enoughPoints = false;
    bool scanStarted = false;
    bool sortedDisplayed = false;
    bool scanInitialized = false;
    bool found = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            //window.close();
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            addPoint = true;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            if (inputCooldownAccumulator == 0 && enoughPoints)
            {
                if (scanStarted)
                {
                    sortedDisplayed = true;
                }
                scanStarted = true;
                inputCooldownAccumulator = inputCooldown;
            }

        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePos);

        dt = frameClock.restart().asSeconds();
        inputCooldownAccumulator = std::fmax(0, inputCooldownAccumulator - dt);
        stepTimerAccumulator = std::fmax(0, stepTimerAccumulator - dt);

        // meat and potatoes
        if (addPoint && !scanStarted)
        {
            if (inputCooldownAccumulator == 0)
            {
                points.push_back({ mousePosWorld.x, mousePosWorld.y });
                inputCooldownAccumulator = inputCooldown;
            }
            addPoint = false;
            if (points.size() >= 3)
            {
                enoughPoints = true;
            }
        }

        if (scanStarted && !scanInitialized)
        {
            // lowest y -> points[0]
            for (int i = 1; i < points.size(); i++)
            {
                if (points[i].y < points[0].y)
                {
                    std::swap(points[0], points[i]);
                }
            }
            // order points from [1..n] by angle with ox axis, ascending

            //bool sorted = false;
            //while (!sorted)
            //{
            //    sorted = true;
            //    for (int j = 1; j < points.size()-1; j++)
            //    {
            //        if (getAngle(points[j], points[0]) > getAngle(points[j+1], points[0]))
            //        {
            //            std::swap(points[j], points[j+1]);
            //            sorted = false;
            //        }
            //    }
            //}

            for (int i = 1; i < points.size(); i++)
            {
                for (int j = i; j < points.size(); j++)
                {
                    if (getAngle(points[i], points[0]) > getAngle(points[j], points[0]))
                    {
                        std::swap(points[i], points[j]);
                    }
                }
            }
            for (int i = points.size() - 1; i >= 0; i--)
            {
                pointsStack.push_back(points[i]);
            }
            solutionStack.clear();
            solutionStack.push_back(pointsStack.back());
            pointsStack.pop_back();
            solutionStack.push_back(pointsStack.back());
            pointsStack.pop_back();
            solutionStack.push_back(pointsStack.back());
            pointsStack.pop_back();
            scanInitialized = true;
        }

        if (scanStarted && scanInitialized && sortedDisplayed && stepTimerAccumulator == 0 && pointsStack.size() > 0)
        {
            pass++;
            stepTimerAccumulator = stepTimer;
            sf::Vector2f old, maybe, newPoint;
            maybe = solutionStack.back();
            old = getSecond(solutionStack);
            newPoint = pointsStack.back();
            pointsStack.pop_back();

            if (isLeftOf(old, maybe, newPoint))
            {
                solutionStack.push_back(newPoint);
            }
            else
            {
                solutionStack.pop_back();
                pointsStack.push_back(newPoint);
            }
            if (pointsStack.size() == 0)
            {
                solutionStack.push_back(solutionStack[0]);
            }
        }


        //draw
        window.clear();

        sf::VertexArray pointsVA;
        for (int i = 0; i < points.size(); i++)
        {
            pointsVA.append({ points[i], sf::Color::White });
        }
        window.draw(pointsVA);

        if (stepTimerAccumulator == stepTimer)
        {
            //std::cout << "solution :" << solutionStack.size() << std::endl;
        }
        window.draw(createVertexArray(solutionStack, sf::Color::Red));
        window.draw(createVertexArray(pointsStack, sf::Color::Blue));

        window.draw(helpText);

        window.display();
    }

    return 0;
}