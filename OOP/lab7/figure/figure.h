#ifndef FIGURE_H
#define FIGURE_H

class Figure
{
public:
    virtual double Square() = 0;
    virtual void Print() = 0;
    virtual ~Figure() {};
    virtual int Type() = 0;
};

#endif /* FIGURE_H */