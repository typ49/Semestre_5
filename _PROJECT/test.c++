#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Jeu de Gravité");

    sf::CircleShape player(20);
    player.setFillColor(sf::Color::Green);
    player.setPosition(400, 300);

    float playerVelocity = 0;
    bool isJumping = false;
    const float gravity = 0.5;
    const float jumpStrength = -15;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && !isJumping)
            {
                playerVelocity = jumpStrength;
                isJumping = true;
            }
        }

        playerVelocity += gravity;
        player.move(0, playerVelocity);

        if (player.getPosition().y > 500) // Simule le sol
        {
            player.setPosition(player.getPosition().x, 500);
            isJumping = false;
        }

        window.clear();
        window.draw(player);
        window.display();
    }

    return 0;
}
