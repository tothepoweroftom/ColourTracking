//
//  PositionRectangle.h
//  ColourTracking
//
//  Created by Tom Power on 27/06/2017.
//
//

#ifndef PositionRectangle_h
#define PositionRectangle_h

class PositionRectangle {
    

    
public:
    int xStart, xEnd, yStart, yEnd;
    void init(int x, int y);
    bool isMember(int x, int y);
    bool isWithin(int x, int y, int within);
    bool doesCollide(PositionRectangle rect);
    bool isWithin(PositionRectangle rect);
    bool isWithin(PositionRectangle rect, int within);
    void addRectangle(PositionRectangle rect);
    void addMember(int x, int y);
    int getXLen();
    int getYLen();
    
};

#endif /* PositionRectangle_h */
