#include "Hexagon.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include <math.h>
#include <iostream>

Hexagon::Hexagon(float x1,float y1,float x2,float y2,float x3,float y3,
                 float x4,float y4,float x5,float y5,float x6,float y6,
                 bool isUsable) {
    this->isUsable=isUsable;
    this->isEmpty=true;
    this->currentPlayer=0;
    this->isPossibilityToMoveClose=false;
    this->isPossibilityToMoveFar=false;
    this->point1=sf::Vector2f(x1,y1);
    this->point2=sf::Vector2f(x2,y2);
    this->point3=sf::Vector2f(x3,y3);
    this->point4=sf::Vector2f(x4,y4);
    this->point5=sf::Vector2f(x5,y5);
    this->point6=sf::Vector2f(x6,y6);
}
auto Hexagon::drawHexagon(sf::RenderWindow &window) -> void {
    sf::ConvexShape hexagon;
    hexagon.setPointCount(6);
    hexagon.setPoint(0, point1);
    hexagon.setPoint(1, point2);
    hexagon.setPoint(2, point3);
    hexagon.setPoint(3, point4);
    hexagon.setPoint(4, point5);
    hexagon.setPoint(5, point6);
    hexagon.setFillColor(sf::Color(147, 132, 209));
    hexagon.setOutlineThickness(2.f);
    hexagon.setOutlineColor(sf::Color::Cyan);
    window.draw(hexagon);
}
auto Hexagon::drawHexagonTaken(sf::RenderWindow &window ,int currentPlayer) -> void {
    float radius=50.f;
    sf::ConvexShape hexagon;
    hexagon.setPointCount(6);
    hexagon.setPoint(0, point1);
    hexagon.setPoint(1, point2);
    hexagon.setPoint(2, point3);
    hexagon.setPoint(3, point4);
    hexagon.setPoint(4, point5);
    hexagon.setPoint(5, point6);
    hexagon.setFillColor(sf::Color(147, 132, 209));
    hexagon.setOutlineThickness(2.f);
    hexagon.setOutlineColor(sf::Color::Cyan);

    sf::CircleShape circle(35.f);
    circle.setOrigin(35,35);
    circle.setPosition(point1.x+radius,point1.y);
    if(currentPlayer==1) circle.setFillColor(sf::Color::Red);
    else circle.setFillColor(sf::Color::Blue);
    window.draw(hexagon);
    window.draw(circle);
}
auto Hexagon::drawMovePossibilityClose(sf::RenderWindow &window)  -> void {
    isPossibilityToMoveClose=true;
    sf::ConvexShape hexagon;
    hexagon.setPointCount(6);
    hexagon.setPoint(0, point1);
    hexagon.setPoint(1, point2);
    hexagon.setPoint(2, point3);
    hexagon.setPoint(3, point4);
    hexagon.setPoint(4, point5);
    hexagon.setPoint(5, point6);
    hexagon.setFillColor(sf::Color::Green);
    hexagon.setOutlineThickness(2.f);
    hexagon.setOutlineColor(sf::Color::Cyan);
    window.draw(hexagon);

}
auto Hexagon::drawMovePossibilityFar(sf::RenderWindow &window) -> void {
    sf::ConvexShape hexagon;
    hexagon.setPointCount(6);
    hexagon.setPoint(0, point1);
    hexagon.setPoint(1, point2);
    hexagon.setPoint(2, point3);
    hexagon.setPoint(3, point4);
    hexagon.setPoint(4, point5);
    hexagon.setPoint(5, point6);
    hexagon.setFillColor(sf::Color(32,178,170));
    hexagon.setOutlineThickness(2.f);
    hexagon.setOutlineColor(sf::Color::Cyan);
    window.draw(hexagon);
}
auto Hexagon::isCursorIn(float xPos, float yPos) const -> bool{
    if(xPos >= point1.x && xPos<point2.x){
        float a1= (point2.y-point1.y)/(point2.x-point1.x);
        float b1= point2.y-a1*point2.x;
        float a2= (point6.y-point1.y)/(point6.x-point1.x);
        float b2= point6.y-a2*point6.x;

        if(yPos<=(a2*xPos + b2) && yPos>=(a1*xPos + b1) && yPos<=point6.y && yPos>=point2.y){
            return true;
        } else{
            return false;
        }
    } else if(xPos>=point2.x && xPos<=point3.x){
        if(yPos<=point6.y && yPos>=point2.y){
            return true;
        } else{
            return false;
        }
    } else if(xPos>point3.x && xPos<=point4.x){
        float a1= (point4.y-point3.y)/(point4.x-point3.x);
        float b1= point3.y-a1*point3.x;
        float a2= (point5.y-point4.y)/(point5.x-point4.x);
        float b2= point5.y-(a2*point5.x);
        if(yPos<=(a2*xPos + b2) && yPos>=(a1*xPos + b1) && yPos<=point5.y && yPos>=point2.y){

            return true;
        } else{
            return false;
        }
    } else{
        return false;
    }
};

