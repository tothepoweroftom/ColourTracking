//
//  PositionRectangle.cpp
//  ColourTracking
//
//  Created by Tom Power on 27/06/2017.
//
//

#include <stdio.h>
#include "PositionRectangle.h"

void PositionRectangle::init(int x, int y) {
    xStart = x-1;
    xEnd = x+1;
    yStart = y-1;
    yEnd = y+1;
}

bool PositionRectangle::isMember(int x, int y) {
    return x>= xStart && y >=yStart && x <=xEnd && y <=yEnd;
}

bool PositionRectangle::isWithin(int x, int y, int within) {
     return x >= xStart - within && y >= yStart - within && x <= xEnd + within && y <= yEnd + within;
}

bool PositionRectangle::doesCollide(PositionRectangle rect){
    return this->isMember(rect.xStart, rect.yStart) || this->isMember(rect.xStart, rect.yEnd)
    || this->isMember(rect.xEnd, rect.yStart) || this->isMember(rect.xEnd, rect.yEnd);
}

bool PositionRectangle::isWithin(PositionRectangle rect, int within) {
    return this->isMember(rect.xStart - within, rect.yStart - within) || this->isMember(rect.xStart - within, rect.yEnd + within)
    || this->isMember(rect.xEnd + within, rect.yStart - within) || this->isMember(rect.xEnd + within, rect.yEnd + within);
}


void PositionRectangle::addRectangle(PositionRectangle rect) {
    this->addMember(rect.xStart, rect.yStart);
    this->addMember(rect.xStart, rect.yEnd);
    this->addMember(rect.xEnd, rect.yStart);
    this->addMember(rect.xEnd, rect.yStart);
}

void PositionRectangle::addMember(int x, int y) {
    if (x <= xStart) {
        xStart = x - 1;
    }
    if (y <= yStart) {
        yStart = y - 1;
    }
    if (x >= xEnd) {
        xEnd = x + 1;
    }
    if (y >= yEnd) {
        yEnd = y + 1;
    }
}

int PositionRectangle::getXLen() {
    return xEnd - xStart;
}

int PositionRectangle::getYLen() {
    return yEnd - yStart;
}
