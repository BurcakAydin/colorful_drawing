#include <SFML/Graphics.hpp>
#include <iostream>

// Constants for the window size
const unsigned WINDOW_WIDTH  = 800;
const unsigned WINDOW_HEIGHT = 600;

// Gravity and jump settings
const float GRAVITY     = 980.f;   // "pixels per second^2" (approx)
const float JUMP_SPEED  = -400.f;  // negative = upwards
const float MOVE_SPEED  = 200.f;   // horizontal movement speed

int main()
{
    // Create window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jumping Square");
    window.setFramerateLimit(60);

    // --- Create a "Mario" rectangle (50x50) ---
    sf::RectangleShape mario(sf::Vector2f(50.f, 50.f));
    mario.setFillColor(sf::Color::Red);
    // Initial position: near bottom of the screen
    mario.setPosition(100.f, WINDOW_HEIGHT - 50.f - 50.f); // (x, y)

    // --- Ground (platform) ---
    // We'll just have one big ground rectangle at the bottom
    sf::RectangleShape ground(sf::Vector2f(static_cast<float>(WINDOW_WIDTH), 50.f));
    ground.setFillColor(sf::Color(139, 69, 19)); // Brown-ish
    ground.setPosition(0.f, WINDOW_HEIGHT - 50.f); // place at bottom

    // --- Variables for movement/physics ---
    // We'll keep track of Mario's vertical velocity
    float velocityX = 0.f;
    float velocityY = 0.f;
    bool  isOnGround = false;

    // --- Main game loop ---
    sf::Clock clock; // to measure frame time
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Calculate delta time (in seconds)
        float dt = clock.restart().asSeconds();

        // --- Input handling ---
        velocityX = 0.f;

        // Move left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            velocityX = -MOVE_SPEED;
        }
        // Move right
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            velocityX = MOVE_SPEED;
        }

        // Jump (only if on ground)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (isOnGround)
            {
                velocityY = JUMP_SPEED; 
                isOnGround = false;
            }
        }

        // --- Apply gravity ---
        velocityY += GRAVITY * dt;

        // --- Update position ---
        sf::Vector2f pos = mario.getPosition();
        pos.x += velocityX * dt;
        pos.y += velocityY * dt;

        // --- Ground collision check ---
        float groundTop = ground.getPosition().y; // y-position of top of the ground
        float marioBottom = pos.y + mario.getSize().y;

        // If Mario is below or at ground level, snap him to ground
        if (marioBottom >= groundTop)
        {
            pos.y = groundTop - mario.getSize().y; // place on top of ground
            velocityY = 0.f;
            isOnGround = true;
        }

        // --- Keep Mario in the window horizontally (simple bounds) ---
        if (pos.x < 0.f) pos.x = 0.f;
        if (pos.x + mario.getSize().x > WINDOW_WIDTH)
            pos.x = WINDOW_WIDTH - mario.getSize().x;

        // Set final position
        mario.setPosition(pos);

        // --- Drawing ---
        window.clear(sf::Color(107, 140, 255)); // a simple sky blue background
        window.draw(ground);
        window.draw(mario);
        window.display();
    }

    return 0;
}
