#include "Settings.hpp"
#include "Reel.hpp"
#include <vector>

#ifndef WINCALCULATOR_HPP
#define WINCALCULATOR_HPP
class Payline
{
	int line[Settings::reelCount];
public:
	void load()
	{
	}
	// position on payline
	int linePos(int reel) const
	{
		return this->line[reel];
	}
};

class WinFigure
{
    virtual int evaluate(const Window& win){
        return 1;
    }
public:
    bool testWin(const Window& win, int &value){
        value = this->evaluate(win);
        if(value != 0)
            return true;
        else
            return false;
    }
};

class NHorizontal : public WinFigure
{
    Payline *lines;
    int least;

    NHorizontal(Payline* lines, int least){
        this->lines = lines;
        this->least = least;
    }

    int evaluate(const Window& win){
        int value = 0;
        for(int i=0; i<Settings::paylineCount; i++){
            value += this::evaluatePayline(this.lines[i], win);
        }
        return value;
    }

    int evaluatePayline(Payline* line, const Window& win){
        int symbol;
        int count;
        int current;
        int winSymbol;
        int max = 0;
        int y;

        for(int reel=0; reel<Settings::reelCount; reel++){
            position = line->linePos(reel);
            current = win->getPosition(reel, position);
            if(reel=0 || current != symbol){
                if(count >= this->least && max < count){
                    max = count;
                    winSymbol = symbol;
                }
                symbol = current;
                count = 1;
            } else {
                count++;
            }
        }
        return max;
    }
};

// Calculates the win of one spin (given as a Window of symbols)
class WinCalculator
{
    std::vector<WinFigure*> figures;

    void addFigure(const WinFigure* figure){
        figures.push_back(figure);
    }

	int paylineWin(const Window& window, const Payline& payline) const
	{
		return 0;
	}
public:
	int win(const Window& window, const Payline* paylines) const
	{
		int partialWin = 0;
		for (int i=0; i<Settings::paylineCount; i++)
		{
			partialWin += this->paylineWin(window, paylines[i]);
		}
		return partialWin;
	}
};

#endif