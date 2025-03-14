#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <iostream>
#include <string>
#include <windows.h>
#include "vec2.h"

int WIDTH = 1280;
int HEIGHT = 720;
double Aspect = double(WIDTH) / double(HEIGHT);

const double L0 = 0.25f; // Constraining length
bool DoSim = false;

using vec2 = hpvec2;

struct Body {
    vec2 Position;
    vec2 PrevPosition;
    vec2 Velocity;
    vec2 Acceleration;
    vec2 Dimensions; // <- only use for rectangles 
	double Radius; // <- only use for circles
    double Mass = 1.;
    double Angle;
	double AngularVelocity;
};


vec2 Normalize(const vec2& x) {
	double Length = std::sqrt(x.x * x.x + x.y * x.y);
	return vec2(x.x / Length, x.y / Length);    
}

Body Bob;
Body Cart;

void StepPhysics(double dt) {
    vec2 AspectScale = vec2(1.0f, 1.0f / Aspect);

	vec2 Gravity = vec2(0.0f, -9.8f * 0.4) ;

    // Update bob 
    {
        Bob.Acceleration = Gravity;
        vec2 PseudoForce = -Bob.Mass * Cart.Acceleration;
        Bob.Acceleration += PseudoForce / Bob.Mass;

        double PrevAngle = Bob.Angle;
        Bob.Velocity += Bob.Acceleration * dt;
        Bob.PrevPosition = Bob.Position;
        Bob.Position += Bob.Velocity * dt;

        // Solve for constraints 
		vec2 Delta = Bob.Position - Cart.Position;
		Delta.x *= AspectScale.x;
		Delta.y *= AspectScale.y;
		double Distance = std::sqrt(Delta.x * Delta.x + Delta.y * Delta.y);
        double DeltaX = Distance - L0;
		Bob.Position.x -= Normalize(Delta).x * DeltaX;
		Bob.Position.y -= Normalize(Delta).y * DeltaX;
        Bob.Velocity = (Bob.Position - Bob.PrevPosition) / dt;
        Bob.Velocity *= 0.99f;
        Bob.Acceleration = vec2(0.,0.);


        vec2 NewDelta = Bob.Position - Cart.Position;
        NewDelta.x *= AspectScale.x;
        NewDelta.y *= AspectScale.y;
        Bob.Angle = std::atan2(NewDelta.x, NewDelta.y); 

        Bob.AngularVelocity = (Bob.Angle - PrevAngle) / dt;

    }

    // Update Cart
    {
        // Solve for acceleration
		Cart.Velocity += Cart.Acceleration * dt;
        Cart.PrevPosition = Cart.Position;
        Cart.Position += Cart.Velocity * dt ;
        Cart.Velocity = (Cart.Position - Cart.PrevPosition) / dt;
        Cart.Velocity *= 0.99f;
		Cart.Acceleration = vec2(0.0f, 0.0f);
    }
}

void SetSFMLWindowSize(sf::RenderWindow& window, HWND hwnd) {
    RECT rect;
    if (GetClientRect(hwnd, &rect)) {
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        window.setSize(sf::Vector2u(width, height));
    }
}

int main()
{
    Bob.Position = vec2(0.5f, 0.5f);
    Bob.PrevPosition = Bob.Position;
	Cart.Position = vec2(0.5f, 0.5);
	Cart.PrevPosition = Cart.Position;

    sf::ContextSettings Settings;
    Settings.antialiasingLevel = 16; 
    
    sf::RenderWindow Window(sf::VideoMode(WIDTH, HEIGHT), "Neural Evolution", sf::Style::Titlebar | sf::Style::Close, Settings);
    Window.setFramerateLimit(120);
   // HWND handle = Window.getSystemHandle();
   // ShowWindow(handle, SW_MAXIMIZE);


    sf::Font Font;
    if (!Font.loadFromFile("C:\\Windows\\Fonts\\consolab.ttf")) {
        std::cout << "font vapo" << std::endl;
    }

    sf::Text FpsText;
    FpsText.setFont(Font);
    FpsText.setCharacterSize(18);
    FpsText.setFillColor(sf::Color::White);
    FpsText.setPosition(10.f, 10.f);


    sf::Text Info;
    Info.setFont(Font);
    Info.setCharacterSize(18);
    Info.setFillColor(sf::Color::White);
    Info.setPosition(10.f, 30.f);

    sf::Text TopText;
    TopText.setFont(Font);
    TopText.setString("NEAT");
    TopText.setCharacterSize(36);
    TopText.setFillColor(sf::Color::White);
    TopText.setPosition(
        Window.getSize().x / 2.0f - TopText.getLocalBounds().width / 2.0f,
        20.f
    );

    sf::CircleShape Circle(10.f); 

    Circle.setFillColor(sf::Color::Green);

    sf::RectangleShape PlayerRect(sf::Vector2f(50.f, 30.f)); 
    PlayerRect.setFillColor(sf::Color::Blue);

    sf::RectangleShape ConnectingRect;
    ConnectingRect.setFillColor(sf::Color::Red);

    const double MoveSpeed = 300.0f;

    sf::Clock Clock;

    sf::Clock FpsClock;
    double FpsUpdateTime = 0.0f;
    unsigned int FrameCount = 0;
    double CurrentFps = 0.0f;

    sf::RectangleShape Line(sf::Vector2f(WIDTH, 1));


    StepPhysics(0.016);

    while (Window.isOpen())
    {
		//SetSFMLWindowSize(Window, handle);
		WIDTH = Window.getSize().x;
		HEIGHT = Window.getSize().y;
		Aspect = double(WIDTH) / double(HEIGHT);
        Cart.Dimensions = vec2(50. / double(WIDTH), 30. / double(HEIGHT));

		double WidthAR = double(WIDTH);
		double HeightAR = double(HEIGHT);


        Circle.setPosition(
            double(WidthAR) * Bob.Position.x,
            double(HeightAR) * (1.-Bob.Position.y)
        );

        sf::Time DeltaTime = Clock.restart();
        double Dt = DeltaTime.asSeconds();

        if (DoSim) {
            StepPhysics(Dt);
        }
        FpsUpdateTime += Dt;
        FrameCount++;

        if (FpsUpdateTime >= 0.2f)
        {
            CurrentFps = FrameCount / FpsUpdateTime;
            std::stringstream Ss;
            Ss << "FPS : " << std::fixed << std::setprecision(1) << CurrentFps;
            FpsText.setString(Ss.str());

            FpsUpdateTime = 0.0f;
            FrameCount = 0;
        }


        sf::Event Event;
        while (Window.pollEvent(Event))
        {
            if (Event.type == sf::Event::Closed)
                Window.close();

            if (Event.type == sf::Event::KeyPressed) {
                if (Event.key.code == sf::Keyboard::Space) {
                    DoSim = !DoSim;
                }
            }
        }

        double SpeedCard = 500.f ;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			SpeedCard *= 6.f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            Cart.Acceleration.x += -Dt * SpeedCard;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            Cart.Acceleration.x += Dt * SpeedCard;
        }

        if (Cart.Position.x - Cart.Dimensions.x / 2.0f < 0) {
            Cart.Position.x = Cart.Dimensions.x / 2.0f;
            Cart.Velocity = vec2(0.0f, 0.0f);
        }
        if (Cart.Position.x + Cart.Dimensions.x / 2.0f > 1) {
            Cart.Position.x = 1. - (Cart.Dimensions.x / 2.0f);
            Cart.Velocity = vec2(0.0f, 0.0f);
        }

        PlayerRect.setPosition(Cart.Position.x * double(WidthAR) - (Cart.Dimensions.x / 2.) * double(WidthAR), 
            (1.0f - Cart.Position.y) * double(HeightAR) - (Cart.Dimensions.y / 2.) * double(HeightAR));

        sf::Vector2f Position = PlayerRect.getPosition();
        sf::Vector2f Size = PlayerRect.getSize();

        sf::Vector2f CircleCenter(
            Circle.getPosition().x + Circle.getRadius(),
            Circle.getPosition().y + Circle.getRadius()
        );

        sf::Vector2f PlayerCenter(
            PlayerRect.getPosition().x + PlayerRect.getSize().x / 2.0f,
            PlayerRect.getPosition().y + PlayerRect.getSize().y / 2.0f
        );

        double DeltaX = PlayerCenter.x - CircleCenter.x;
        double DeltaY = PlayerCenter.y - CircleCenter.y; 
        double Distance = std::sqrt(DeltaX * DeltaX + DeltaY * DeltaY);
        double Angle = std::atan2(DeltaY, DeltaX);
        ConnectingRect.setSize(sf::Vector2f(Distance, 5.f));
        ConnectingRect.setPosition(CircleCenter);
        ConnectingRect.setRotation(Angle * 180.f / 3.1415927f); 
        ConnectingRect.setOrigin(0, 2.5f);

        std::string temp = "Bob Angle : ";
        temp += std::to_string(Angle * 180.f / 3.1415927f); 
		temp += "\n\nCart Velocity : ";
		temp += std::to_string(Cart.Velocity.x) + "  ";
		temp += std::to_string(Cart.Velocity.y);
        temp += "\nCart Position : ";
        temp += std::to_string(Cart.Position.x) + "  ";
        temp += std::to_string(Cart.Position.y);
		temp += "\n\nBob Velocity : ";
		temp += std::to_string(Bob.Velocity.x) + "  ";
        temp += std::to_string(Bob.Velocity.y);
        temp += "\nBob Position : ";
        temp += std::to_string(Bob.Position.x) + "  ";
        temp += std::to_string(Bob.Position.y);
        temp += "\nBob Angular Velocity : ";
		temp += std::to_string(Bob.AngularVelocity);
        temp += "\nBob Angular Position : ";
		temp += std::to_string(Bob.Angle * 180.f / 3.1415927f);

        Info.setString(temp.c_str());

        Window.clear(sf::Color::Black);


        Line.setFillColor(sf::Color::White);
        Line.setPosition(0, HEIGHT / 2 + 16.);
        Window.draw(Line);

        Window.draw(Circle);
        Window.draw(PlayerRect);
        Window.draw(ConnectingRect);

        Window.draw(FpsText);
        Window.draw(Info);
        Window.draw(TopText);

        Window.display();
        
        std::string str = "Neural Physics by swr06";
		if (DoSim) {
			str += "   |  Running";
		}
		else {
			str += "   |  Paused";
		}

        Window.setTitle(str.c_str());
    }

    return 0;
}