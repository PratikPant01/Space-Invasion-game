#include "UI.h"
#include "Game.h"
#include "cosmicalien.h"
#include "cmath"
#include "spritesheet.h"
UI::UI(sf::RenderWindow& win) : window(win) {
    // Initialize the health bar
    this->initializeHealthBar();
    this->initGUI();
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    this->set_menu_objects(desktopMode.width, desktopMode.height);
    this->set_instructions_objects(desktopMode.width, desktopMode.height);

	if (!backgroundMusic.openFromFile("Sound/back.wav"))
	{
		std::cout << "Error: Could not load background music" << std::endl;
	}
	else {
		backgroundMusic.setVolume(50);
		backgroundMusic.setLoop(true);
		backgroundMusic.play();
	}

}

void UI::initializeHealthBar() {
    // Background bar
    this->hpback.setSize(sf::Vector2f(55.f, 3.f)); // Set size for background


    // Health bar (it will be dynamically resized later)
    this->hpbar.setSize(sf::Vector2f(55.f, 3.f)); // Initial size, can be updated later



}


void UI::updateHealthBar(float healthPercent, float posx, float posy, sf::Color color)
{
    // Calculate the width based on health percentage
    float maxBarWidth = 68.f;
    float currentBarWidth = healthPercent * maxBarWidth;


    this->hpback.setPosition(posx - 2, posy + 3); // Set position for the background bar
    this->hpback.setFillColor(sf::Color(25, 25, 25));
    this->hpback.setOutlineThickness(1.f);
    this->hpback.setOutlineColor(sf::Color::Black);

    this->hpbar.setSize(sf::Vector2f(currentBarWidth, 3.f));
    this->hpbar.setPosition(posx - 2, posy + 3); // Set position for the background bar
    this->hpbar.setOutlineThickness(1.f);
    this->hpbar.setOutlineColor(sf::Color::Black);


    // Change color based on health percentage
    if (healthPercent > 0.4f) {
        this->hpbar.setFillColor(color); // Green for healthy

    }
    else {
        int alpha = std::sin(clock.getElapsedTime().asSeconds() * 10) * 127 + 128; // Pulse effect
        this->hpbar.setFillColor(sf::Color(255, 0, 0, alpha)); // Pulsing red
    }

}
void UI::updateHearts(int lives, float healthPercent) {
    for (int i = 0; i < 3; ++i) {
        int frameIndex = 4; // Default to empty heart

        if (i < lives) { // Only update hearts for remaining lives
            if (i == lives - 1) {
                // Update the heart corresponding to the last remaining life
                if (healthPercent > 0.8f) frameIndex = 0; // Full heart
                else if (healthPercent > 0.6f) frameIndex = 1; // 80% heart
                else if (healthPercent > 0.4f) frameIndex = 2; // 60% heart
                else if (healthPercent > 0.1f) frameIndex = 3; // 40% heart
            }
            else {
                // Hearts not corresponding to the last remaining life stay full
                frameIndex = 0;
            }
        }

        const int frameWidth = 669;
        const int frameHeight = 603;

        // Map hearts array index (left-to-right)
        hearts[i].setTextureRect(sf::IntRect(frameWidth * frameIndex, 0, frameWidth, frameHeight));
    }
}


void UI::updatebossHealthBar(float healthPercent, float posx, float posy, sf::Color color) {
    // Calculate the width based on health percentage
    float maxBarWidth = 70.f;
    float currentBarWidth = healthPercent * maxBarWidth;

    this->bosshpback.setPosition(posx, posy); // Set position for the background bar
    this->bosshpback.setFillColor(sf::Color(25, 25, 25));

    this->bosshpbar.setSize(sf::Vector2f(currentBarWidth, 3.f));
    this->bosshpbar.setPosition(posx, posy); // Set position for the background bar
    // Change color based on health percentage

    this->bosshpbar.setFillColor(color); // Green for healthy


}
void UI::updateexplosion(float posx, float posy)
{
    // Set up the explosion sprite position, scale, and origin for centering
    this->explosion.setPosition(posx, posy);
    this->explosion.setScale(2.0f, 2.0f);
    this->explosion.setOrigin(54, 50); // Assuming the center of the frame

    // Set the texture for the explosion
    this->explosion.setTexture(exptext);
    
    // Update to the next frame of the explosion sprite sheet
    static int frame = 0; // Track the current frame
    int frameWidth = 108;
    int frameHeight = 100;

    // Update the texture rectangle to show the current frame
    this->explosion.setTextureRect(sf::IntRect(frameWidth * frame, 0, frameWidth, frameHeight));

    // Increment the frame for the next update
    frame++;
    if (frame >= 10) { // Assuming there are 10 frames in the animation
        frame = 0; // Reset to the first frame after the last frame
    }

    // Optional: Make the explosion fade out
    int alpha = std::max(0, 255 - frame * 25); // Reduces opacity over time
    this->explosion.setColor(sf::Color(255, 255, 255, alpha));

    // Render explosion
    window.draw(this->explosion);
}





void UI::updateScoreAndLevel(int score, int level, float x, float y)
{

    this->ptext.setString("Score:" + std::to_string(score) + "\nLevel:" + std::to_string(level));
    this->ptext.setCharacterSize(30);
    this->ptext.setPosition(x, y);
    this->finalscore = score;
}


void UI::endgame()
{
    window.draw(GameOver);
    window.draw(score);
    window.draw(Replay);
    window.draw(Close);


}

void UI::initGUI() {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    if (!this->font.loadFromMemory(CosmicAlien_ttf, CosmicAlien_ttf_len)) {
        std::cout << "Error: Font not loaded\n";
    }
    if (!this->heartTexture.loadFromMemory(spritesheet_png, spritesheet_png_len)) {
        std::cout << "Error: Font not loaded\n";
    }
    else
    {
        std::cout << "Texture loaded successfully. Size: "
            << this->heartTexture.getSize().x << "x"
            << this->heartTexture.getSize().y << std::endl;
    }
    for (int i = 0; i < 3; ++i) {
        hearts[i].setTexture(heartTexture);
        hearts[i].setScale(0.07f, 0.07f); // Adjust size (10% of original texture size)
        hearts[i].setPosition(0.05 * desktopMode.width + i * 50.f, desktopMode.height * 0.05); // Increase spacing between hearts

        // Full heart frame initially
        hearts[i].setTextureRect(sf::IntRect(0, 0, 669, 603));
        hearts[i].setColor(sf::Color(255, 50, 50, 255)); // Fully opaque red

    }


    this->ptext.setFont(this->font);
    this->ptext.setCharacterSize(25);
    this->ptext.setFillColor(sf::Color::Red);

    this->initializeSounds();


}

void UI::render(sf::RenderTarget* target) {


    // Draw background bar first
    target->draw(this->hpback);

    // Draw the health bar on top of the background
    target->draw(this->hpbar);
    for (int i = 0; i < 3; ++i) {
        target->draw(hearts[i]);
    }

    target->draw(this->bosshpback);
    target->draw(this->bosshpbar);


    // Optionally, draw the score and level text
    target->draw(this->ptext);

	// Optionally, draw the explosion animation
	target->draw(this->explosion);

	// Optionally, draw the music button
	target->draw(this->music_button_outline);
	target->draw(this->music_button);

    

}

void UI::update() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    static bool musicButtonPressed = false;

    // Play button
    if (play_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        play_button_outline.setFillColor(sf::Color(0, 255, 0, 100));
        play_button.setFillColor(sf::Color(100, 255, 100));
        play_button.setScale(1.1f, 1.1f);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            play_button.setScale(1.05f, 1.05f);
            this->playClickSound();
        }
    }
    else {
        play_button_outline.setFillColor(sf::Color(0, 0, 0, 150));
        play_button.setFillColor(sf::Color::White);
        play_button.setScale(1.0f, 1.0f);
    }

    // Instructions button
    if (instructions_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        instructions_button_outline.setFillColor(sf::Color(0, 255, 255, 100));
        instructions_button.setFillColor(sf::Color(100, 255, 255));
        instructions_button.setScale(1.1f, 1.1f);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            instructions_button.setScale(1.05f, 1.05f);
            this->playClickSound();
        }
    }
    else {
        instructions_button_outline.setFillColor(sf::Color(0, 0, 0, 150));
        instructions_button.setFillColor(sf::Color::White);
        instructions_button.setScale(1.0f, 1.0f);
    }

    // Scores button
    if (scores_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        scores_button_outline.setFillColor(sf::Color(0, 255, 255, 100));
        scores_button.setFillColor(sf::Color(100, 255, 255));
        scores_button.setScale(1.1f, 1.1f);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            scores_button.setScale(1.05f, 1.05f);
            this->playClickSound();
        }
    }
    else {
        scores_button_outline.setFillColor(sf::Color(0, 0, 0, 150));
        scores_button.setFillColor(sf::Color::White);
        scores_button.setScale(1.0f, 1.0f);
    }

    // Music button with improved toggle logic
    if (music_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        music_button_outline.setFillColor(sf::Color(0, 255, 255, 100));
        music_button.setFillColor(sf::Color(100, 255, 255));
        music_button.setScale(1.1f, 1.1f);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!musicButtonPressed) {  // Only trigger once when button is first pressed
                musicButtonPressed = true;
                playClickSound();
                
                if (isMusicOn) {
                    backgroundMusic.stop();
                    music_button.setString("MUSIC: OFF");
                } else {
                    backgroundMusic.play();
                    music_button.setString("MUSIC: ON");
                }
                isMusicOn = !isMusicOn;
            }
        } else {
            musicButtonPressed = false;  // Reset when button is released
        }
    }
    else {
        music_button_outline.setFillColor(sf::Color(0, 0, 0, 150));
        music_button.setFillColor(sf::Color::White);
        music_button.setScale(1.0f, 1.0f);
    }

    // Back button
    if (back_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        back_button_outline.setFillColor(sf::Color(0, 255, 255, 100));
        back_button.setFillColor(sf::Color(100, 255, 255));
        back_button.setScale(1.1f, 1.1f);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            back_button.setScale(1.05f, 1.05f);
            this->playClickSound();
        }
    }
    else {
        back_button_outline.setFillColor(sf::Color(0, 0, 0, 150));
        back_button.setFillColor(sf::Color::White);
        back_button.setScale(1.0f, 1.0f);
    }

    // Replay button
    if (Replay_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        Replay_outline.setFillColor(sf::Color(0, 255, 255, 100));
        Replay.setFillColor(sf::Color(100, 255, 255));
        Replay.setScale(1.1f, 1.1f);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            Replay.setScale(1.05f, 1.05f);
        }
    }
    else {
        Replay_outline.setFillColor(sf::Color(0, 0, 0, 150));
        Replay.setFillColor(sf::Color::White);
        Replay.setScale(1.0f, 1.0f);
    }

    // Close button
    if (Close_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        Close_outline.setFillColor(sf::Color(0, 255, 255, 100));
        Close.setFillColor(sf::Color(100, 255, 255));
        Close.setScale(1.1f, 1.1f);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            Close.setScale(1.05f, 1.05f);
            this->playClickSound();
        }
    }
    else {
        Close_outline.setFillColor(sf::Color(0, 0, 0, 150));
        Close.setFillColor(sf::Color::White);
        Close.setScale(1.0f, 1.0f);
    }

    static bool musicButtonPressed = false;
    static bool creditsButtonPressed = false;  // Add tracking for credits button

    // Play button handling (existing code)
    if (play_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        play_button_outline.setFillColor(sf::Color(0, 255, 0, 100));
        play_button.setFillColor(sf::Color(100, 255, 100));
        play_button.setScale(1.1f, 1.1f);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            play_button.setScale(1.05f, 1.05f);
            this->playClickSound();
        }
    }
    else {
        play_button_outline.setFillColor(sf::Color(0, 0, 0, 150));
        play_button.setFillColor(sf::Color::White);
        play_button.setScale(1.0f, 1.0f);
    }

    // Credits button handling - Add this section
    if (credits_button_outline.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        credits_button_outline.setFillColor(sf::Color(0, 255, 255, 100));
        credits_button.setFillColor(sf::Color(100, 255, 255));
        credits_button.setScale(1.1f, 1.1f);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!creditsButtonPressed) {  // Only trigger once when button is first pressed
                creditsButtonPressed = true;
                this->playClickSound();
                // Add any state change or callback here
                std::cout << "Credits button clicked!" << std::endl; // Debug output
            }
        }
        else {
            creditsButtonPressed = false;  // Reset when button is released
        }
    }
    else {
        credits_button_outline.setFillColor(sf::Color(0, 0, 0, 150));
        credits_button.setFillColor(sf::Color::White);
        credits_button.setScale(1.0f, 1.0f);
        creditsButtonPressed = false;
    }
}


void UI::set_menu_objects(int width, int height)
{
    // Sci-fi Color Palette
    sf::Color titleColor(0, 255, 255);       // Bright Cyan
    sf::Color buttonTextColor(255, 255, 255);// Bright White
    sf::Color buttonOutlineColor(0, 255, 255, 180); // Translucent Cyan
    sf::Color buttonBgColor(0, 0, 0, 150);   // Semi-transparent Black

    // Title with futuristic styling
    title.setString("SPACE INVASION");
    title.setFont(font);
    title.setFillColor(titleColor);
    title.setOutlineColor(sf::Color(0, 255, 255, 100));
    title.setOutlineThickness(3);

    float baseHeight = 800.0f;
    int titleSize = static_cast<int>(height * 50.0f / baseHeight);
    int buttonSize = static_cast<int>(height * 30.0f / baseHeight);
    title.setCharacterSize(titleSize);

    // Centered title positioning
    sf::FloatRect titleBounds = title.getLocalBounds();
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    title.setPosition(
        centerX - titleBounds.width / 2.0f,
        centerY - 200
    );

    // Play Button
    play_button.setString("PLAY");
    play_button.setFont(font);
    play_button.setCharacterSize(buttonSize);
    play_button.setFillColor(buttonTextColor);
    play_button.setPosition(width * 0.5f - play_button.getGlobalBounds().width * 0.5f, height * 0.5f);

    play_button_outline.setSize(sf::Vector2f(play_button.getGlobalBounds().width + 50, play_button.getGlobalBounds().height + 40));
    play_button_outline.setPosition(width * 0.5f - play_button.getGlobalBounds().width * 0.5f - 25, height * 0.5f + 7);
    play_button_outline.setOutlineColor(buttonOutlineColor);
    play_button_outline.setFillColor(buttonBgColor);
    play_button_outline.setOutlineThickness(3.0f);

    // Instructions Button
    instructions_button.setString("HOW TO PLAY");
    instructions_button.setFont(font);
    instructions_button.setCharacterSize(buttonSize);
    instructions_button.setFillColor(buttonTextColor);
    instructions_button.setPosition(0.05f * width, height * 0.9f);

    instructions_button_outline.setPosition(0.05f * width - 20, height * 0.9f);
    instructions_button_outline.setSize(sf::Vector2f(instructions_button.getGlobalBounds().width + 40, instructions_button.getGlobalBounds().height + 30));
    instructions_button_outline.setOutlineColor(buttonOutlineColor);
    instructions_button_outline.setFillColor(buttonBgColor);
    instructions_button_outline.setOutlineThickness(3.0f);

    // Scores Button
    scores_button.setString("SCORES");
    scores_button.setFont(font);
    scores_button.setCharacterSize(buttonSize);
    scores_button.setFillColor(buttonTextColor);
    scores_button.setPosition(0.95f * width - scores_button.getGlobalBounds().width, height * 0.9f);

    scores_button_outline.setPosition(0.95f * width - scores_button.getGlobalBounds().width - 20, height * 0.9f);
    scores_button_outline.setSize(sf::Vector2f(scores_button.getGlobalBounds().width + 40, scores_button.getGlobalBounds().height + 30));
    scores_button_outline.setOutlineColor(buttonOutlineColor);
    scores_button_outline.setFillColor(buttonBgColor);
    scores_button_outline.setOutlineThickness(3.0f);


    if (isMusicOn) {
        music_button.setString("MUSIC: ON");
    }
    else {
        music_button.setString("MUSIC: OFF");
    }
	music_button.setFont(font);
	music_button.setCharacterSize(buttonSize);
	music_button.setFillColor(buttonTextColor);
    music_button.setPosition(width - music_button.getGlobalBounds().width - 20, 20); // Top-right corner

    music_button_outline.setSize(sf::Vector2f(music_button.getGlobalBounds().width + 40, music_button.getGlobalBounds().height + 30));
    music_button_outline.setPosition(width - music_button.getGlobalBounds().width - 40, 15); // Align with the button
    music_button_outline.setOutlineColor(buttonOutlineColor);
    music_button_outline.setFillColor(buttonBgColor);
    music_button_outline.setOutlineThickness(3.0f);

    credits_button.setString("CREDITS");
    credits_button.setFont(font);
    credits_button.setCharacterSize(buttonSize);
    credits_button.setFillColor(buttonTextColor);
    credits_button.setPosition(20, 20); // Top-left corner position

    credits_button_outline.setSize(sf::Vector2f(credits_button.getGlobalBounds().width + 40, credits_button.getGlobalBounds().height + 30));
    credits_button_outline.setPosition(15, 15); // Slightly offset from the text for padding
    credits_button_outline.setOutlineColor(buttonOutlineColor);
    credits_button_outline.setFillColor(buttonBgColor);
    credits_button_outline.setOutlineThickness(3.0f);


   

   

}


void UI::load_menu()
{
    // Ensure all menu elements are drawn in the correct order
    window.draw(title);  // Draw title first

    // Draw button outlines first (background)
    window.draw(play_button_outline);
    window.draw(instructions_button_outline);
    window.draw(scores_button_outline);

    // Draw actual button text on top of outlines
    window.draw(play_button);
    window.draw(instructions_button);
    window.draw(scores_button);

	window.draw(music_button_outline);
	window.draw(music_button);

    window.draw(credits_button_outline);
    window.draw(credits_button);

}

void UI::set_instructions_objects(int width, int height)
{
    // title
    how_to_play_title.setString("HOW TO PLAY?");
    how_to_play_title.setFont(font);
    how_to_play_title.setFillColor(sf::Color::White);
    how_to_play_title.setCharacterSize(72);
    how_to_play_title.setPosition(width / 2 - how_to_play_title.getGlobalBounds().width / 2, 0.1 * height);

    // first point
    objectives.setString(
        "HOW TO PLAY: \n"
        "1. Use arrow keys to move your spaceship: \n"
        "   - UP arrow: Move up \n"
        "   - DOWN arrow: Move down \n"
        "   - LEFT arrow: Move left \n"
        "   - RIGHT arrow: Move right \n\n"
        "2. Press SPACE to shoot bullets and destroy enemies. \n"
        "3. Collect power-ups to enhance your abilities: \n"
        "   - Some power-ups increase your fire rate. \n"
        "   - Others provide shields to protect you from enemy attacks. \n\n"
        "4. Avoid enemy attacks! \n"
        "   - If you get hit by an enemy's bullet or collide with an enemy, you lose health. \n\n"
        "5. Destroy enemies to earn points: \n"
        "   - The more enemies you destroy, the higher your score! \n"
        "   - Some enemies may drop valuable items like extra lives or score boosts. \n\n"
        "6. Boss battles: \n"
        "   - Defeat the boss to proceed to the next level. \n"
        "   - The boss has multiple stages, so be prepared for tougher challenges. \n\n"
        "7. Press ESC to pause the game at any time. \n\n"
        "Good luck, and may you save the galaxy!"
    );

      objectives.setFont(font);
    objectives.setFillColor(sf::Color::White);
    objectives.setCharacterSize(32);
    objectives.setPosition(0.15 * width, 300);

    // back button
    back_button.setString("BACK");
    back_button.setFont(font);
    back_button.setFillColor(sf::Color::White);
    back_button.setCharacterSize(35);
    back_button.setPosition(0.05 * width, 0.05 * height);

    back_button_outline.setFillColor(sf::Color::Black);
    back_button_outline.setOutlineColor(sf::Color::White);
    back_button_outline.setOutlineThickness(2.0f);
    back_button_outline.setSize(sf::Vector2f(back_button.getGlobalBounds().width + 20, back_button.getGlobalBounds().height + 20));
    back_button_outline.setPosition(0.05 * width - 10, 0.05 * height);
}

void UI::load_instructions()
    
{

    sf::Color currentColor = objectives.getFillColor();
    if (currentColor.a < 255) {
        currentColor.a += 5;  // Increase alpha for fade-in
        objectives.setFillColor(currentColor);
    }

    window.draw(how_to_play_title);
    window.draw(objectives);
    window.draw(back_button_outline);
    window.draw(back_button);
}

void UI::set_scores(int x)
{

}

void UI::load_score()
{
    this->window.draw(score);
}




void UI::load_end_page()
{
}

void UI::load_game()
{
}
void UI::initializeSounds() {
    if (!clickBuffer.loadFromFile("Sound/item.wav")) {
        std::cout << "Error: Could not load click sound" << std::endl;
    }
    clickSound.setBuffer(clickBuffer);
}

void UI::playClickSound() {
    clickSound.play();
}

void UI::set_credits_objects(int width, int height) {
    // Credits title
    credits_title.setString("CREDITS");
    credits_title.setFont(font);
    credits_title.setFillColor(sf::Color::White);
    credits_title.setCharacterSize(72);
    credits_title.setPosition(width / 2 - credits_title.getGlobalBounds().width / 2, 0.1 * height);

    // Credits text
    credits_text.setString(
        "SPACE INVASION\n\n"
        "Game Development Team:\n\n"
        "Programming:\n"
        "- Lead Programmer: [Your Name]\n"
        "- Game Engine: SFML\n\n"
        "Art & Design:\n"
        "- Character Design: [Artist Name]\n"
        "- UI Design: [Designer Name]\n\n"
        "Sound & Music:\n"
        "- Sound Effects: [Sound Designer]\n"
        "- Background Music: [Composer]\n\n"
        "Special Thanks:\n"
        "- Beta Testers\n"
        "- SFML Community\n"
        "- Our Families and Friends\n\n"
        "© 2024 Space Invasion Team\n"
        "All Rights Reserved"
    );

    credits_text.setFont(font);
    credits_text.setFillColor(sf::Color::White);
    credits_text.setCharacterSize(32);
    credits_text.setPosition(0.15 * width, 300);

    // Back button (reuse existing back button properties)
    // The back button is already defined in the class
}

void UI::load_credits() {
    sf::Color currentColor = credits_text.getFillColor();
    if (currentColor.a < 255) {
        currentColor.a += 5;  // Increase alpha for fade-in
        credits_text.setFillColor(currentColor);
    }

    window.draw(credits_title);
    window.draw(credits_text);
    window.draw(back_button_outline);
    window.draw(back_button);
}