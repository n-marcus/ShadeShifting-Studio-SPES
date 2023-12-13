ControlBar controlBar;
int controlBarHeight = 150;

class ControlBar{ 
    float xPos, yPos, barWidth, barHeight;
    float bottomPadding = 10;
    ControlBar(float _xPos, float _yPos, float _barWidth, float _barHeight) {
        xPos = _xPos;
        yPos = _yPos;
        barWidth = _barWidth;
        barHeight = _barHeight;
    }


    void update() { 
        //title
        textSize(32);
        text("Shade Shifting", 50, 50);



        //bottom seperator line
        strokeWeight(1);
        stroke(255);
        line(xPos, yPos + barHeight -bottomPadding, xPos + barWidth, yPos + barHeight -bottomPadding);

    }
}