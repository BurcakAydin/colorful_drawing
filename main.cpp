#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream> // for std::cout in save operation

// --------------------------------------------------------
// Helper function to draw a line of points 
void drawLine(std::vector<sf::Vertex>& lines,
              const sf::Vector2i& start,
              const sf::Vector2i& end,
              sf::Color color)
{
    // Simple linear interpolation approach
    int dx = end.x - start.x;
    int dy = end.y - start.y;
    int steps = std::max(std::abs(dx), std::abs(dy));

    if (steps == 0) {
        // If no movement, just add a single point
        lines.push_back(sf::Vertex(sf::Vector2f(static_cast<float>(start.x),
                                                static_cast<float>(start.y)),
                                   color));
        return;
    }

    float xInc = static_cast<float>(dx) / steps;
    float yInc = static_cast<float>(dy) / steps;
    float x = static_cast<float>(start.x);
    float y = static_cast<float>(start.y);

    for (int i = 0; i <= steps; i++) {
        lines.push_back(sf::Vertex(sf::Vector2f(x, y), color));
        x += xInc;
        y += yInc;
    }
}
// --------------------------------------------------------

// Spray brush function
void drawSpray(std::vector<sf::Vertex>& lines,
               const sf::Vector2i& position,
               sf::Color color,
               float radius)
{
    for (int i = 0; i < 200; ++i) {
        float angle    = static_cast<float>(rand() % 360) * 3.14159f / 180.f;
        float distance = static_cast<float>(rand() % static_cast<int>(radius));
        float x_offset = std::cos(angle) * distance;
        float y_offset = std::sin(angle) * distance;
        lines.push_back(sf::Vertex(sf::Vector2f(position.x + x_offset,
                                                position.y + y_offset),
                                   color));
    }
}

// Star brush function
void drawStar(std::vector<sf::Vertex>& lines,
              const sf::Vector2i& position,
              sf::Color color,
              float size)
{
    const int numPoints = 10;
    float angleStep     = 2.f * 3.14159f / numPoints;

    for (int i = 0; i < numPoints; ++i) {
        float angle = i * angleStep;
        float x     = position.x + std::cos(angle) * size;
        float y     = position.y + std::sin(angle) * size;
        lines.push_back(sf::Vertex(sf::Vector2f(x, y), color));
    }
}

// Circle brush function
void drawCircle(std::vector<sf::Vertex>& lines,
                const sf::Vector2i& position,
                sf::Color color,
                float radius)
{
    const int numPoints = 60;
    float angleStep     = 2.f * 3.14159f / numPoints;

    for (int i = 0; i < numPoints; ++i) {
        float angle = i * angleStep;
        float x     = position.x + std::cos(angle) * radius;
        float y     = position.y + std::sin(angle) * radius;
        lines.push_back(sf::Vertex(sf::Vector2f(x, y), color));
    }
}

int main() {
    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Drawing App");
    window.setFramerateLimit(60);

    // Vector to store all drawn vertices
    std::vector<sf::Vertex> lines;

    // Drawing status and last mouse position (for pencil)
    bool drawing = false;
    sf::Vector2i lastMousePos;

    // Current brush type (1 = pencil, 2 = spray, 3 = star, 4 = circle)
    int brushType = 1;

    // Brush color
    sf::Color currentColor = sf::Color::White;

    // Brush thickness (not yet directly used, but adjustable)
    float thickness = 2.0f;

    // Background color
    sf::Color bgColor(50, 50, 50); // Darker background for better contrast

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // Start drawing when left mouse is pressed
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                drawing = true;
                lastMousePos = sf::Mouse::getPosition(window);
            }

            // Stop drawing when left mouse is released
            if (event.type == sf::Event::MouseButtonReleased &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                drawing = false;
            }

            // Handle key presses
            if (event.type == sf::Event::KeyPressed) {
                // Change brush type
                if (event.key.code == sf::Keyboard::Num1) {
                    brushType = 1; // Pencil (continuous line)
                } else if (event.key.code == sf::Keyboard::Num2) {
                    brushType = 2; // Spray
                } else if (event.key.code == sf::Keyboard::Num3) {
                    brushType = 3; // Star
                } else if (event.key.code == sf::Keyboard::Num4) {
                    brushType = 4; // Circle
                }

                // Color shortcuts
                if (event.key.code == sf::Keyboard::R) {
                    currentColor = sf::Color::Red; // Red
                } 
                else if (event.key.code == sf::Keyboard::B) {
                    currentColor = sf::Color(173, 216, 230); // Light Blue
                }
                else if (event.key.code == sf::Keyboard::O) {
                    currentColor = sf::Color(255, 165, 0);   // Orange
                }
                else if (event.key.code == sf::Keyboard::G) {
                    currentColor = sf::Color(144, 238, 144); // Light Green
                }
                else if (event.key.code == sf::Keyboard::W) {
                    currentColor = sf::Color::White; // Whites
                }
                else if (event.key.code == sf::Keyboard::D) {
                    currentColor = sf::Color::Black;         // Black
                }

                // Clear drawing
                if (event.key.code == sf::Keyboard::C) {
                    lines.clear();
                }

                // Save drawing
                if (event.key.code == sf::Keyboard::S) {
                    sf::RenderTexture rtex;
                    rtex.create(window.getSize().x, window.getSize().y);
                    rtex.clear(bgColor);
                    // Draw all points onto this render texture
                    if (!lines.empty()) {
                        rtex.draw(&lines[0], lines.size(), sf::Points);
                    }
                    rtex.display();

                    // Save to an image file
                    sf::Image screenshot = rtex.getTexture().copyToImage();
                    if (screenshot.saveToFile("drawing.png")) {
                        std::cout << "Saved current drawing to 'drawing.png'\n";
                    } else {
                        std::cout << "Failed to save image.\n";
                    }
                }
            }
        }

        // If we're currently drawing, capture mouse movement
        if (drawing) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            if (brushType == 1) {
                // Pencil: draw a continuous line between last and current position
                drawLine(lines, lastMousePos, mousePos, currentColor);
            } else if (brushType == 2) {
                drawSpray(lines, mousePos, currentColor, 10.0f);
            } else if (brushType == 3) {
                drawStar(lines, mousePos, currentColor, 15.0f);
            } else if (brushType == 4) {
                drawCircle(lines, mousePos, currentColor, 15.0f);
            }

            lastMousePos = mousePos;
        }

        // Render everything
        window.clear(bgColor);
        if (!lines.empty()) {
            // Draw all vertices as points
            window.draw(&lines[0], lines.size(), sf::Points);
        }
        window.display();
    }

    return 0;
}
